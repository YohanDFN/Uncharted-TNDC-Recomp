// Uncharted 2: Among Thieves — Memory Subsystem (C++23)
// Clean C++23 reimplementation of:
//   - d:/dev/CP9/shared-ps4/src/corelib/memory/memory.cpp
//   - AllocStack (Thread-Local Allocator Stack, 46 entries, 1840B depth)
//   - PushAllocator / PopAllocator / GetAllocatorStackDepth / AllocateAllocStack
//   - ProcessHeap::ScanForProcessDroppings (src/game/process/process-heap.cpp)

#include "u2_engine.h"
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

namespace {

thread_local AllocStack *t_thread_alloc_stack = nullptr;

inline AllocStack **get_guest_tls_slot() noexcept {
    // In hybrid emulation mode, runtime_thread_current_tcb() returns the guest TCB
    // pAllocStack pointer is located at TLS offset -0x20 from guest TCB
    if (!g_u2_image_base) return nullptr;
    uint64_t *tcb = runtime_thread_current_tcb();
    if (!tcb) return nullptr;
    return reinterpret_cast<AllocStack **>(reinterpret_cast<uintptr_t>(tcb) - 0x20);
}

inline AllocStack *get_active_alloc_stack() noexcept {
    if (t_thread_alloc_stack) {
        return t_thread_alloc_stack;
    }
    AllocStack **slot = get_guest_tls_slot();
    if (slot && *slot) {
        t_thread_alloc_stack = *slot;
        return t_thread_alloc_stack;
    }
    return nullptr;
}

inline void set_active_alloc_stack(AllocStack *stack) noexcept {
    t_thread_alloc_stack = stack;
    AllocStack **slot = get_guest_tls_slot();
    if (slot) {
        *slot = stack;
    }
}

} // namespace

AllocStack *allocate_alloc_stack() noexcept {
    auto *stack = static_cast<AllocStack *>(std::calloc(1, sizeof(AllocStack)));
    if (!stack) return nullptr;
    stack->m_allocatorStackSize = 0;
    set_active_alloc_stack(stack);
    return stack;
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
}

void *pop_allocator() noexcept {
    AllocStack *s = get_active_alloc_stack();
    if (!s || s->m_allocatorStackSize == 0) return nullptr;
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

/* 0x00B360D0 — PushAllocator(void *allocator, const char *name, int32_t line, const char *file) */
void ABI u2_cpp_PushAllocator(void *allocator, const char *name, int32_t line, const char *file) {
    u2::engine::push_allocator(allocator, name, line, file);
}

/* 0x00B361C0 — PopAllocator() */
void * ABI u2_cpp_PopAllocator(void) {
    return u2::engine::pop_allocator();
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
