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
    };
}

bool resolve_package_file(NativePackageRequest &request) {
    request.guest_path = "/app0/u2data/build/main/pak22/ps4/" + request.name + ".pak";
    const int fd = runtime_file_open(request.guest_path.c_str(), 0, 0);
    if (fd < 0) return false;

    const int64_t size = runtime_file_lseek(fd, 0, 2);
    runtime_file_close(fd);
    if (size < 0) return false;

    request.file_size = static_cast<uint64_t>(size);
    request.state = U2NativePackageState::Resolved;
    return true;
}

} // namespace

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
