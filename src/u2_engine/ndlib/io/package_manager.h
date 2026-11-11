// Uncharted 2: Among Thieves — Native package request registry
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

enum class U2NativePackageState : uint32_t {
    Missing = 0,
    Resolved = 1,
    Ready = 2,
};

struct U2NativePackageInfo {
    const char *name;
    const char *guest_path;
    uint32_t string_id;
    U2NativePackageState state;
    uint64_t file_size;
    bool header_inspected;
    uint32_t page_count;
    uint32_t page_table_offset;
};

extern "C" {
bool u2_cpp_PackageMgr_InspectHeader(const uint8_t *prefix, size_t prefix_size,
                                  uint64_t file_size, uint32_t *page_count,
                                  uint32_t *page_table_offset);
bool ABI u2_cpp_PackageMgr_RequestPackage(void *mgr, const char *name);
const U2NativePackageInfo *u2_cpp_PackageMgr_FindNativeRequest(const char *name);
size_t u2_cpp_PackageMgr_GetNativeRequestCount(void);
void u2_cpp_PackageMgr_ResetNativeRequestsForTests(void);
}
