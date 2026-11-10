// Uncharted 2: Among Thieves — Streaming & PC VFS
// Clean C++23 Reimplementation (ndlib/io/)
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

extern "C" {
bool ABI u2_cpp_LoadFileHelper(void *file_data_out, const char *path);
void ABI u2_cpp_Streaming_ResolvePath(const char *original_path, char *resolved_out);
}
