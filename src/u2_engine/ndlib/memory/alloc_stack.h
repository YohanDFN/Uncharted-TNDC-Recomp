// Uncharted 2: Among Thieves — AllocStack & Memory Subsystems
// Clean C++23 Reimplementation (ndlib/memory/)
#pragma once

#include <cstddef>
#include <cstdint>

#ifndef ABI
#if defined(_WIN32)
#define ABI __attribute__((sysv_abi))
#else
#define ABI
#endif
#endif

namespace u2::engine {

void *get_host_allocator() noexcept;
void  ensure_root_allocator() noexcept;
void *allocate_from_current_or_host(size_t size, size_t alignment = 16, const char *name = nullptr) noexcept;

} // namespace u2::engine

extern "C" {
void *       ABI u2_cpp_AllocateAllocStack(void);
void         ABI u2_cpp_FreeThreadAllocStack(void);
void         ABI u2_cpp_PushAllocator(void *allocator, const char *name, int32_t line, const char *file);
void         ABI u2_cpp_PushAllocator_Tag(void *tag_ptr, const char *name, int32_t line, const char *file);
void         ABI u2_cpp_PushAllocator_Ext(void *scope, const char *name, int32_t line, const char *file);
void *       ABI u2_cpp_PopAllocator(void);
void *       ABI u2_cpp_PopAllocator_Expected(void *scope);
uint32_t     ABI u2_cpp_GetAllocatorStackDepth(void);
void         ABI u2_cpp_ProcessHeap_ScanForProcessDroppings(void *self);
}
