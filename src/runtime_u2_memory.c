/* Uncharted 2: Among Thieves (big2-ps4_Shipping.elf)
 * Native C Reimplementation of Naughty Dog's Memory Subsystem:
 *   - d:/dev/CP9/shared-ps4/src/corelib/memory/memory.cpp
 *   - AllocStack (Thread-Local Allocator Stack, 46 entries, 1840B depth)
 *   - PushAllocator / PopAllocator / GetAllocatorStackDepth
 */
#include "runtime.h"
#include "runtime_u2_memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

_Static_assert(sizeof(U2AllocEntry) == 40, "U2AllocEntry size must be 40 bytes");
_Static_assert(offsetof(U2AllocStack, m_allocatorStackSize) == 1840, "m_allocatorStackSize must be at offset 1840");

/* 12-byte absolute 64-bit jump patch: movabs rax, <addr>; jmp rax */
static void write_detour_jmp64(uint8_t *target, uintptr_t dest) {
    target[0] = 0x48;
    target[1] = 0xB8;
    memcpy(target + 2, &dest, 8);
    target[10] = 0xFF;
    target[11] = 0xE0;
}

/* Helper to get pointer to thread-local pAllocStack pointer (TLS offset -0x20 from guest TCB) */
static inline U2AllocStack **u2_mem_get_tls_slot(void) {
    uint64_t *tcb = runtime_thread_current_tcb();
    if (!tcb) return NULL;
    return (U2AllocStack **)((uintptr_t)tcb - 0x20);
}

U2AllocStack *u2_mem_get_thread_alloc_stack(void) {
    U2AllocStack **slot = u2_mem_get_tls_slot();
    return (slot && *slot) ? *slot : NULL;
}

void u2_mem_set_thread_alloc_stack(U2AllocStack *stack) {
    U2AllocStack **slot = u2_mem_get_tls_slot();
    if (slot) *slot = stack;
}

void *u2_mem_top_allocator(void) {
    U2AllocStack *s = u2_mem_get_thread_alloc_stack();
    if (!s || s->m_allocatorStackSize == 0) return NULL;
    return s->stack[s->m_allocatorStackSize - 1].allocator;
}

uint32_t u2_mem_get_stack_depth(void) {
    U2AllocStack *s = u2_mem_get_thread_alloc_stack();
    return s ? s->m_allocatorStackSize : 0;
}

void u2_mem_push_allocator(void *pAlloc, const char *name, int32_t line, const char *file) {
    U2AllocStack *s = u2_mem_get_thread_alloc_stack();
    if (!s) return;
    if (s->m_allocatorStackSize < U2_MAX_ALLOC_STACK_DEPTH) {
        uint32_t idx = s->m_allocatorStackSize++;
        s->stack[idx].allocator = pAlloc;
        s->stack[idx].name = name;
        s->stack[idx].line = line;
        s->stack[idx].file = file;
        s->stack[idx].flags = 1;
    }
}

void *u2_mem_pop_allocator(void) {
    U2AllocStack *s = u2_mem_get_thread_alloc_stack();
    if (!s || s->m_allocatorStackSize == 0) return NULL;
    uint32_t idx = --s->m_allocatorStackSize;
    void *alloc = s->stack[idx].allocator;
    memset(&s->stack[idx], 0, sizeof(U2AllocEntry));
    return alloc;
}

/* ============================================================================
 * Detours for Naughty Dog AllocStack Functions (SysV ABI)
 * ============================================================================ */

/* PushAllocator (direct IAllocator*, 0x00B360D0) */
static ABI void detour_PushAllocator_Direct(void *pAlloc, const char *name, int32_t line, const char *file) {
    u2_mem_push_allocator(pAlloc, name, line, file);
}

/* PopAllocator (0x00B361C0) */
static ABI void *detour_PopAllocator(void) {
    return u2_mem_pop_allocator();
}

/* GetAllocatorStackDepth (0x00B362A0) */
static ABI uint32_t detour_GetAllocatorStackDepth(void) {
    return u2_mem_get_stack_depth();
}

/* ============================================================================
 * Detour Installation
 * ============================================================================ */

void runtime_u2_memory_install_detours(uint8_t *image, uint64_t image_size) {
    /* Verify executable is big2-ps4_Shipping.elf (U2 ScriptManager signature at 0x00A14126) */
    static const uint8_t u2_exec_sig[14] = {
        0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x53, 0x48, 0x81, 0xE4, 0xE0, 0xFF, 0xFF, 0xFF
    };
    if (image_size < 0x00B36300 || memcmp(image + 0x00A14126, u2_exec_sig, sizeof(u2_exec_sig)) != 0) {
        return;
    }

    const char *native_mem_env = getenv("BB_U2_NATIVE_MEM");
    const int enable_native_mem = (native_mem_env && *native_mem_env && *native_mem_env != '0');

    if (!enable_native_mem) {
        printf("[U2 Recomp] Memory subsystem detours available (set BB_U2_NATIVE_MEM=1 to activate native C AllocStack)\n");
        fflush(stdout);
        return;
    }

    struct DetourEntry {
        const char *name;
        uint64_t    offset;
        uintptr_t   host_fn;
    };

    static const struct DetourEntry mem_detours[] = {
        { "PushAllocator_Direct",   0x00B360D0, (uintptr_t)detour_PushAllocator_Direct },
        { "PopAllocator",           0x00B361C0, (uintptr_t)detour_PopAllocator },
        { "GetAllocatorStackDepth", 0x00B362A0, (uintptr_t)detour_GetAllocatorStackDepth },
    };

    size_t installed = 0;
    for (size_t i = 0; i < sizeof(mem_detours) / sizeof(mem_detours[0]); ++i) {
        if (mem_detours[i].offset + 12 <= image_size) {
            write_detour_jmp64(image + mem_detours[i].offset, mem_detours[i].host_fn);
            ++installed;
        }
    }

    printf("[U2 Recomp] Installed %zu native C memory allocator detours (PushAllocator, PopAllocator, GetAllocatorStackDepth)\n", installed);
    fflush(stdout);
}
