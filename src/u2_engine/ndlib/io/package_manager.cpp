// Uncharted 2: Among Thieves — Native package request registry
// This implements only the proven request/deduplication stage of
// PackageMgr::RequestPackage (0x0056C0E0). PAK parsing and readiness are later
// stages and are deliberately not claimed here.

#include "u2_engine.h"

#include <array>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>

namespace {

constexpr size_t kMaxNativePackageRequests = 64;

struct NativePackageRequest {
    std::string name;
    std::string guest_path;
    uint32_t string_id{0};
    U2NativePackageState state{U2NativePackageState::Missing};
    uint64_t file_size{0};
    bool header_inspected{false};
    uint32_t page_count{0};
    uint32_t page_table_offset{0};
    U2NativePackageInfo view{};
};

std::array<NativePackageRequest, kMaxNativePackageRequests> s_requests;
size_t s_request_count = 0;
std::mutex s_request_mutex;

uint32_t string_id(const char *text) {
    uint32_t hash = 0x811C9DC5u;
    if (!text) return hash;
    for (const auto *p = reinterpret_cast<const unsigned char *>(text); *p; ++p) {
        hash = (hash ^ *p) * 0x01000193u;
    }
    return hash;
}

NativePackageRequest *find_request(uint32_t id) {
    for (size_t i = 0; i < s_request_count; ++i) {
        if (s_requests[i].string_id == id) return &s_requests[i];
    }
    return nullptr;
}

void refresh_view(NativePackageRequest &request) {
    request.view = {
        request.name.c_str(),
        request.guest_path.c_str(),
        request.string_id,
        request.state,
        request.file_size,
        request.header_inspected,
        request.page_count,
        request.page_table_offset,
    };
}

bool resolve_package_file(NativePackageRequest &request) {
    request.guest_path = "/app0/u2data/build/main/pak22/ps4/" + request.name + ".pak";
    const int fd = runtime_file_open(request.guest_path.c_str(), 0, 0);
    if (fd < 0) return false;

    const int64_t size = runtime_file_lseek(fd, 0, 2);
    std::array<uint8_t, 0x18> prefix{};
    const int64_t bytes = runtime_file_pread(fd, prefix.data(), prefix.size(), 0);
    bool inspected = size >= 0 && bytes == static_cast<int64_t>(prefix.size()) &&
        u2_cpp_PackageMgr_InspectHeader(prefix.data(), prefix.size(),
            static_cast<uint64_t>(size), &request.page_count, &request.page_table_offset);
    // Read every table entry without following its embedded values. Bound the
    // work by the file extent, avoiding allocations based on untrusted counts.
    std::array<uint8_t, 12> entry{};
    for (uint32_t i = 0; inspected && i < request.page_count; ++i) {
        const uint64_t offset = request.page_table_offset + uint64_t{i} * entry.size();
        inspected = runtime_file_pread(fd, entry.data(), entry.size(), offset) ==
                    static_cast<int64_t>(entry.size());
    }
    runtime_file_close(fd);
    if (size < 0) return false;

    request.file_size = static_cast<uint64_t>(size);
    request.state = U2NativePackageState::Resolved;
    request.header_inspected = inspected;
    std::printf("[U2 Native PackageMgr] '%s' header=%s pages=%u table=0x%X\n",
                request.name.c_str(), inspected ? "inspected" : "rejected",
                request.page_count, request.page_table_offset);
    return true;
}

} // namespace

// Proven by 0x00B6DE33..0x00B6DE7C: count at +0x10, signed
// table offset at +0x14, and a 12-byte entry stride. Other fields are unknown.
extern "C" bool u2_cpp_PackageMgr_InspectHeader(const uint8_t *prefix, size_t prefix_size,
                                               uint64_t file_size, uint32_t *page_count,
                                               uint32_t *page_table_offset) {
    if (!page_count || !page_table_offset) return false;
    *page_count = 0;
    *page_table_offset = 0;
    if (!prefix || prefix_size < 0x18 || file_size < prefix_size) return false;
    auto read_le32 = [](const uint8_t *p) {
        return uint32_t{p[0]} | (uint32_t{p[1]} << 8) |
               (uint32_t{p[2]} << 16) | (uint32_t{p[3]} << 24);
    };
    const uint32_t count = read_le32(prefix + 0x10);
    const uint32_t offset = read_le32(prefix + 0x14);
    if (offset > 0x7FFFFFFFu || offset < 0x18 || offset > file_size ||
        uint64_t{count} > (file_size - offset) / 12) return false;
    *page_count = count;
    *page_table_offset = offset;
    return true;
}

extern "C" bool ABI u2_cpp_PackageMgr_RequestPackage(void *mgr, const char *name) {
    (void)mgr;
    if (!name || !*name) return false;

    std::lock_guard lock(s_request_mutex);
    const uint32_t id = string_id(name);
    if (auto *existing = find_request(id)) {
        return existing->state != U2NativePackageState::Missing;
    }
    if (s_request_count >= s_requests.size()) {
        std::fprintf(stderr, "[U2 Native PackageMgr] request registry full (%zu entries)\n", s_requests.size());
        return false;
    }

    auto &request = s_requests[s_request_count++];
    request.name = name;
    request.string_id = id;
    const bool resolved = resolve_package_file(request);
    refresh_view(request);

    std::printf("[U2 Native PackageMgr] '%s' id=0x%08X state=%s path='%s' size=%llu\n",
                request.name.c_str(), request.string_id,
                resolved ? "resolved" : "missing", request.guest_path.c_str(),
                static_cast<unsigned long long>(request.file_size));
    std::fflush(stdout);
    return resolved;
}

extern "C" const U2NativePackageInfo *u2_cpp_PackageMgr_FindNativeRequest(const char *name) {
    if (!name || !*name) return nullptr;
    std::lock_guard lock(s_request_mutex);
    auto *request = find_request(string_id(name));
    return request ? &request->view : nullptr;
}

extern "C" size_t u2_cpp_PackageMgr_GetNativeRequestCount(void) {
    std::lock_guard lock(s_request_mutex);
    return s_request_count;
}

extern "C" void u2_cpp_PackageMgr_ResetNativeRequestsForTests(void) {
    std::lock_guard lock(s_request_mutex);
    for (auto &request : s_requests) request = {};
    s_request_count = 0;
}
