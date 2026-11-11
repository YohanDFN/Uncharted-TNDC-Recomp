// Uncharted 2: Among Thieves — Native package request registry
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "game/loading/level_resources.h"
#include "ndlib/render/ngen/geometry_preview.h"

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
    bool first_page_verified;
    uint32_t first_page_offset;
    uint32_t first_page_size;
    uint32_t local_fixups;
    uint32_t pending_fixups;
    uint32_t second_page_size;
    bool pointers_relocated;
    uint32_t catalog_resources;
    uint32_t login_resources;
    bool level_info_loaded;
    uint32_t level_info_value;
    bool camera_definitions_decoded;
    uint32_t camera_definitions;
    bool level_offset_decoded;
};

extern "C" {
bool u2_cpp_PackageMgr_GetNativeGeometryPreview(const char *package, std::vector<U2NativeMeshPreview> *out);
bool u2_cpp_PackageMgr_GetNativeLevelOffset(const char *package, U2NativeLevelOffset *out);
bool u2_cpp_PackageMgr_GetNativeCamera(const char *package, size_t index,
                                     U2NativeCameraDefinition *out);
bool u2_cpp_PackageMgr_ReadLoginTypes(const uint8_t *header, size_t header_size,
                                    const uint8_t *page0, size_t size0,
                                    const uint8_t *page1, size_t size1,
                                    std::vector<std::string> *types);
const char *u2_cpp_PackageMgr_GetLoginType(const char *package, size_t index);
bool u2_cpp_PackageMgr_ReadResourceTypes(const uint8_t *page, size_t size,
                                       std::vector<std::string> *types);
size_t u2_cpp_PackageMgr_CountResourceType(const char *package, const char *type);
bool u2_cpp_PackageMgr_RelocatePages(const uint8_t *header, size_t header_size,
                                   uint8_t *page0, size_t size0, uint8_t *page1, size_t size1,
                                   bool *relocated);
bool u2_cpp_PackageMgr_AuditFixups(const uint8_t *header, size_t header_size,
                                 const uint8_t *page, size_t page_size,
                                 uint32_t *local, uint32_t *pending);
bool u2_cpp_PackageMgr_CheckPage(const uint8_t *entry, size_t entry_size,
                               uint64_t file_size, const uint8_t *page, size_t page_size);
bool u2_cpp_PackageMgr_InspectHeader(const uint8_t *prefix, size_t prefix_size,
                                  uint64_t file_size, uint32_t *page_count,
                                  uint32_t *page_table_offset);
bool ABI u2_cpp_PackageMgr_RequestPackage(void *mgr, const char *name);
const U2NativePackageInfo *u2_cpp_PackageMgr_FindNativeRequest(const char *name);
size_t u2_cpp_PackageMgr_GetNativeRequestCount(void);
void u2_cpp_PackageMgr_ResetNativeRequestsForTests(void);
}
