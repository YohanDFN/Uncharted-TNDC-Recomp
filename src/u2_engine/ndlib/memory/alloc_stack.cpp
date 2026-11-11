// Uncharted 2: Among Thieves — Memory Subsystem (C++23)
// Clean C++23 reimplementation of:
//   - d:/dev/CP9/shared-ps4/src/corelib/memory/memory.cpp
//   - AllocStack (Thread-Local Allocator Stack, 46 entries, 1840B depth)
//   - PushAllocator / PopAllocator / GetAllocatorStackDepth / AllocateAllocStack
//   - ProcessHeap::ScanForProcessDroppings (src/game/process/process-heap.cpp)

#include "u2_engine.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <atomic>

namespace u2::engine {

constexpr size_t kMaxAllocatorStackDepth = 46;

struct AllocStackEntry {
    void       *allocator{nullptr}; // +0x00: IAllocator pointer
    const char *name{nullptr};      // +0x08: Tag or name string
    int32_t     line{0};            // +0x10: Source line number
    int32_t     _pad14{0};          // +0x14
    const char *file{nullptr};      // +0x18: Source file path string
    uint32_t    flags{0};           // +0x20: Context flags (1 for direct IAllocator)
    uint32_t    _pad24{0};          // +0x24
};
static_assert(sizeof(AllocStackEntry) == 40, "AllocStackEntry size must be 40 bytes");

struct AllocStack {
    AllocStackEntry stack[kMaxAllocatorStackDepth]; // 0x000..0x72F (46 * 40 = 1840 bytes)
    uint32_t        m_allocatorStackSize{0};        // 0x730 (offset 1840)
    uint32_t        _pad734{0};                     // 0x734
    uint8_t         _reserved[0x48]{};              // 0x738..0x77F (total size 1920 bytes = 0x780)
};
static_assert(offsetof(AllocStack, m_allocatorStackSize) == 0x730, "m_allocatorStackSize must be at offset 0x730");
static_assert(sizeof(AllocStack) == 0x780, "AllocStack size must be 0x780 bytes");

AllocStack *allocate_alloc_stack() noexcept;

namespace {

/* ============================================================================
 * Host Default Heap Allocator (SysV ABI compliant for guest ELF callers)
 * ============================================================================ */

struct HostAllocHeader {
    uint64_t magic;
    void    *raw;
};
static constexpr uint64_t kHostAllocMagic = 0x484F5354414C4C43ULL; // "HOSTALLC"

static void *host_aligned_alloc(size_t size, size_t alignment) noexcept {
    if (alignment < 16) alignment = 16;
    if (size == 0) size = 1;
    size_t total = size + alignment + sizeof(HostAllocHeader);
    void *raw = std::calloc(1, total);
    if (!raw) return nullptr;
    uintptr_t addr = reinterpret_cast<uintptr_t>(raw) + sizeof(HostAllocHeader);
    uintptr_t aligned = (addr + alignment - 1) & ~(alignment - 1);
    auto *hdr = reinterpret_cast<HostAllocHeader *>(aligned) - 1;
    hdr->magic = kHostAllocMagic;
    hdr->raw = raw;
    return reinterpret_cast<void *>(aligned);
}

static void host_aligned_free(void *ptr) noexcept {
    if (!ptr || reinterpret_cast<uintptr_t>(ptr) < 0x10000) return;
    auto *hdr = reinterpret_cast<HostAllocHeader *>(ptr) - 1;
    if (hdr->magic == kHostAllocMagic) {
        hdr->magic = 0;
        std::free(hdr->raw);
    }
}

int64_t ABI HostAllocator_DefaultStub(void *self) {
    (void)self;
    return 0;
}

const char * ABI HostAllocator_GetName(void *self) {
    (void)self;
    return "HostDefaultHeap";
}

void * ABI HostAllocator_Allocate(void *self, size_t size, size_t alignment, const char *name, uint32_t flags, const char *file) {
    (void)self;
    (void)name;
    (void)flags;
    (void)file;
    return host_aligned_alloc(size, alignment);
}

void ABI HostAllocator_Free(void *self, void *ptr) {
    (void)self;
    host_aligned_free(ptr);
}

bool ABI HostAllocator_Contains(void *self, void *ptr) {
    (void)self;
    if (!ptr || reinterpret_cast<uintptr_t>(ptr) < 0x10000) return false;
    const auto *hdr = reinterpret_cast<const HostAllocHeader *>(ptr) - 1;
    return hdr->magic == kHostAllocMagic;
}

uint32_t ABI HostAllocator_GetType(void *self) {
    (void)self;
    return 1; // EBP_AllocatorTypeForDebug_HeapAllocator
}

struct HostAllocatorVTable {
    uintptr_t funcs[32];
};

struct HostAllocator {
    const HostAllocatorVTable *vtable;
};

static HostAllocatorVTable s_host_vtable = []() {
    HostAllocatorVTable vt{};
    for (size_t i = 0; i < 32; ++i) {
        vt.funcs[i] = reinterpret_cast<uintptr_t>(&HostAllocator_DefaultStub);
    }
    vt.funcs[1]  = reinterpret_cast<uintptr_t>(&HostAllocator_GetName);
    vt.funcs[9]  = reinterpret_cast<uintptr_t>(&HostAllocator_Allocate);  // 0x48: Allocate(size, align, flags, name)
    vt.funcs[13] = reinterpret_cast<uintptr_t>(&HostAllocator_Free);      // 0x68: Free(ptr)
    vt.funcs[14] = reinterpret_cast<uintptr_t>(&HostAllocator_Contains);  // 0x70: Contains(ptr)
    vt.funcs[28] = reinterpret_cast<uintptr_t>(&HostAllocator_GetType);   // 0xE0: BP_GetAllocatorTypeForDebug()
    return vt;
}();

static HostAllocator s_host_allocator = { &s_host_vtable };

thread_local AllocStack *t_thread_alloc_stack = nullptr;

inline AllocStack *get_active_alloc_stack() noexcept {
    // Native stacks are owned by the host thread, never by the guest TLS pool.
    if (!t_thread_alloc_stack) {
        t_thread_alloc_stack = allocate_alloc_stack();
    }
    if (t_thread_alloc_stack) {
        if (t_thread_alloc_stack->m_allocatorStackSize == 0) {
            t_thread_alloc_stack->stack[0].allocator = &s_host_allocator;
            t_thread_alloc_stack->stack[0].name = "HostDefaultHeap";
            t_thread_alloc_stack->stack[0].line = 1;
            t_thread_alloc_stack->stack[0].file = "memory_system.cpp";
            t_thread_alloc_stack->stack[0].flags = 1;
            t_thread_alloc_stack->m_allocatorStackSize = 1;
        }
        return t_thread_alloc_stack;
    }
    return nullptr;
}

inline void set_active_alloc_stack(AllocStack *stack) noexcept {
    t_thread_alloc_stack = stack;
}

} // namespace

void *get_host_allocator() noexcept {
    return &s_host_allocator;
}

void ensure_root_allocator() noexcept {
    AllocStack *s = get_active_alloc_stack();
    if (!s) {
        s = allocate_alloc_stack();
    }
    if (s && s->m_allocatorStackSize == 0) {
        s->stack[0].allocator = &s_host_allocator;
        s->stack[0].name = "HostDefaultHeap";
        s->stack[0].line = 1;
        s->stack[0].file = "memory_system.cpp";
        s->stack[0].flags = 1;
        s->m_allocatorStackSize = 1;
    }
}

AllocStack *allocate_alloc_stack() noexcept {
    auto *stack = static_cast<AllocStack *>(std::calloc(1, sizeof(AllocStack)));
    if (!stack) return nullptr;
    stack->stack[0].allocator = &s_host_allocator;
    stack->stack[0].name = "HostDefaultHeap";
    stack->stack[0].line = 1;
    stack->stack[0].file = "memory_system.cpp";
    stack->stack[0].flags = 1;
    stack->m_allocatorStackSize = 1;
    set_active_alloc_stack(stack);
    return stack;
}

void free_alloc_stack(AllocStack *stack) noexcept {
    std::free(stack);
}

void push_allocator(void *allocator, const char *name, int32_t line, const char *file) noexcept {
    AllocStack *s = get_active_alloc_stack();
    if (!s) {
        s = allocate_alloc_stack();
        if (!s) return;
    }
    if (s->m_allocatorStackSize < kMaxAllocatorStackDepth) {
        uint32_t idx = s->m_allocatorStackSize++;
        s->stack[idx].allocator = allocator;
        s->stack[idx].name = name;
        s->stack[idx].line = line;
        s->stack[idx].file = file;
        s->stack[idx].flags = 1;
    }
    if (s->m_allocatorStackSize == 2) {
        printf("[MemDebug] AllocStack %p PUSHED TO 2 (tag=%s) by caller %p (image+0x%llx)\n",
               (void *)s, name ? name : "null", __builtin_return_address(0),
               g_u2_image_base ? (uint64_t)((uintptr_t)__builtin_return_address(0) - (uintptr_t)g_u2_image_base) : 0ULL);
        fflush(stdout);
    }
}

void *pop_allocator() noexcept {
    AllocStack *s = get_active_alloc_stack();
    if (!s) return nullptr;
    if (s->m_allocatorStackSize <= 1) {
        // Root allocator level: never pop below 1 to prevent inlined ND ALLOCATE macros from faulting on NULL.
        void *alloc = s->stack[0].allocator;
        s->stack[0].allocator = &s_host_allocator;
        s->stack[0].name = "HostDefaultHeap";
        s->stack[0].line = 1;
        s->stack[0].file = "memory_system.cpp";
        s->stack[0].flags = 1;
        s->m_allocatorStackSize = 1;
        return alloc;
    }
    uint32_t idx = --s->m_allocatorStackSize;
    void *alloc = s->stack[idx].allocator;
    std::memset(&s->stack[idx], 0, sizeof(AllocStackEntry));
    return alloc;
}

uint32_t get_allocator_stack_depth() noexcept {
    AllocStack *s = get_active_alloc_stack();
    return s ? s->m_allocatorStackSize : 0;
}

void *top_allocator() noexcept {
    AllocStack *s = get_active_alloc_stack();
    if (!s || s->m_allocatorStackSize == 0) return nullptr;
    return s->stack[s->m_allocatorStackSize - 1].allocator;
}

void *allocate_from_current_or_host(size_t size, size_t alignment, const char *name) noexcept {
    if (alignment < 16) alignment = 16;
    if (size == 0) size = 1;
    AllocStack *s = get_active_alloc_stack();
    if (s && s->m_allocatorStackSize > 0 && s->m_allocatorStackSize <= kMaxAllocatorStackDepth) {
        void *alloc = s->stack[s->m_allocatorStackSize - 1].allocator;
        if (alloc && reinterpret_cast<uintptr_t>(alloc) >= 0x10000 && (reinterpret_cast<uintptr_t>(alloc) & 7) == 0) {
            void **vtable = *reinterpret_cast<void ***>(alloc);
            if (vtable && reinterpret_cast<uintptr_t>(vtable) >= 0x10000 && (reinterpret_cast<uintptr_t>(vtable) & 7) == 0 && vtable[9]) {
                auto alloc_fn = reinterpret_cast<void * (ABI *)(void *, size_t, size_t, const char *, uint32_t, const char *)>(vtable[9]);
                void *res = alloc_fn(alloc, size, alignment, name ? name : "UNKNOWN", 0, nullptr);
                if (res) return res;
            }
        }
    }
    return HostAllocator_Allocate(get_host_allocator(), size, alignment, name ? name : "UNKNOWN", 0, nullptr);
}

void scan_for_process_droppings(void *self) noexcept {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000) return;
    // Check if debug flags are enabled in game state
    if (g_u2_image_base) {
        const uint8_t *debug_block = elf_ptr<const uint8_t>(0x11362F8);
        if (debug_block && debug_block[0x206] == 0) {
            return;
        }
    }
    const auto *heap = static_cast<const uint8_t *>(self);
    // Heap active flag at +0xF0
    if (heap[0xF0] == 0) {
        return;
    }
    // Block linked list scan starting at +0x40
    const uint8_t *cur = *reinterpret_cast<const uint8_t * const *>(heap + 0x40);
    const uint8_t *head = heap + 0x40;
    while (cur && cur != head && reinterpret_cast<uintptr_t>(cur) >= 0x10000) {
        // Block inspection: check next pointer at +0x10
        cur = *reinterpret_cast<const uint8_t * const *>(cur + 0x10);
    }
}

} // namespace u2::engine

extern "C" {

/* 0x00B36D40 — AllocateAllocStack() */
void * ABI u2_cpp_AllocateAllocStack(void) {
    return u2::engine::allocate_alloc_stack();
}

/* 0x00B35D30 — FreeThreadAllocStack() */
void ABI u2_cpp_FreeThreadAllocStack(void) {
    u2::engine::free_alloc_stack(u2::engine::t_thread_alloc_stack);
    u2::engine::t_thread_alloc_stack = nullptr;
}

/* 0x00B360D0 — PushAllocator(void *allocator, const char *name, int32_t line, const char *file) */
void ABI u2_cpp_PushAllocator(void *allocator, const char *name, int32_t line, const char *file) {
    u2::engine::push_allocator(allocator, name, line, file);
}

/* 0x00B361C0 — PopAllocator() */
void * ABI u2_cpp_PopAllocator(void) {
    return u2::engine::pop_allocator();
}

/* 0x00B377B0 — PopAllocator(void *scope) */
void * ABI u2_cpp_PopAllocator_Expected(void *scope) {
    (void)scope;
    return u2::engine::pop_allocator();
}

/* 0x00B35EA0 — PushAllocator(void *tag_ptr, const char *name, int32_t line, const char *file) */
void ABI u2_cpp_PushAllocator_Tag(void *tag_ptr, const char *name, int32_t line, const char *file) {
    (void)tag_ptr;
    u2::engine::push_allocator(u2::engine::get_host_allocator(), name ? name : "TagAlloc", line, file ? file : "tag.cpp");
}

/* 0x00B371A0 — ScopedTempAllocator::ScopedTempAllocator(void *scope, const char *name, int32_t line, const char *file) */
void ABI u2_cpp_PushAllocator_Ext(void *scope, const char *name, int32_t line, const char *file) {
    void *allocator = u2::engine::get_host_allocator();
    if (scope) {
        void **s = reinterpret_cast<void **>(scope);
        s[0] = allocator;
        s[1] = nullptr;
    }
    u2::engine::push_allocator(allocator, name ? name : "ScopedTempAlloc", line, file ? file : "scoped_temp.cpp");
}

/* 0x00B362A0 — GetAllocatorStackDepth() */
uint32_t ABI u2_cpp_GetAllocatorStackDepth(void) {
    return u2::engine::get_allocator_stack_depth();
}

/* 0x009B7770 — ProcessHeap::ScanForProcessDroppings(void *self) */
void ABI u2_cpp_ProcessHeap_ScanForProcessDroppings(void *self) {
    u2::engine::scan_for_process_droppings(self);
}

} // extern "C"
