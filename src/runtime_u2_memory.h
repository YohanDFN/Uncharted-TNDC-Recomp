/* Uncharted 2: Among Thieves (big2-ps4_Shipping.elf)
 * Native C Reimplementation of Naughty Dog's Memory Subsystem:
 *   - d:/dev/CP9/shared-ps4/src/corelib/memory/memory.cpp
 *   - AllocStack (Thread-Local Allocator Stack, 46 entries, 1840B depth)
 *   - PushAllocator / PopAllocator / GetAllocatorStackDepth
 *   - InitializeThreadAllocStack / FreeThreadAllocStack / AllocateAllocStack
 */
#ifndef RUNTIME_U2_MEMORY_H
#define RUNTIME_U2_MEMORY_H

#include <stdint.h>
#include <stddef.h>

#define U2_MAX_ALLOC_STACK_DEPTH 46

/* 40-byte AllocStack entry matching Naughty Dog x86-64 memory layout */
typedef struct U2AllocEntry {
    void       *allocator;  /* +0x00: IAllocator pointer */
    const char *name;       /* +0x08: Tag or name string */
    int32_t     line;       /* +0x10: Source line number */
    int32_t     _pad14;     /* +0x14 */
    const char *file;       /* +0x18: Source file path string */
    uint32_t    flags;      /* +0x20: Context / flags (1 for direct IAllocator) */
    uint32_t    _pad24;     /* +0x24 */
} U2AllocEntry;

/* Thread-local AllocStack struct (1856 bytes = 0x740) */
typedef struct U2AllocStack {
    U2AllocEntry stack[U2_MAX_ALLOC_STACK_DEPTH]; /* 0x000..0x72F (46 * 40 = 1840 bytes) */
    uint32_t     m_allocatorStackSize;            /* 0x730 (offset 1840) */
    uint32_t     _pad734;                         /* 0x734 */
    uint64_t     _reserved[2];                    /* 0x738..0x747 (total size 1856 bytes) */
} U2AllocStack;

U2AllocStack *u2_mem_get_thread_alloc_stack(void);
void          u2_mem_set_thread_alloc_stack(U2AllocStack *stack);
void         *u2_mem_top_allocator(void);
uint32_t      u2_mem_get_stack_depth(void);
void          u2_mem_push_allocator(void *pAlloc, const char *name, int32_t line, const char *file);
void         *u2_mem_pop_allocator(void);
void          runtime_u2_memory_install_detours(uint8_t *image, uint64_t image_size);

#endif /* RUNTIME_U2_MEMORY_H */

