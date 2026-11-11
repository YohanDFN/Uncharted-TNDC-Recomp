// Uncharted 2: Among Thieves — Native package request registry
// Host-only PAK request registry. This is not a reimplementation of
// 0x0056C0E0: that historical attribution conflated LevelMgr and PackageMgr.
// The original Level loading path requests packages through 0x00B6CD10.

#include "u2_engine.h"

#include <array>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <vector>
#include <new>
#include <algorithm>
#include "game/loading/level_resources.h"

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
    bool first_page_verified{false};
    uint32_t first_page_offset{0};
    std::vector<uint8_t> first_page;
    uint32_t local_fixups{0};
    uint32_t pending_fixups{0};
    std::vector<uint8_t> second_page;
    bool pointers_relocated{false};
    std::vector<std::string> resource_types;
    std::vector<std::string> login_types;
    U2NativeLevelInfo level_info;
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
        request.first_page_verified,
        request.first_page_offset,
        static_cast<uint32_t>(request.first_page.size()),
        request.local_fixups,
        request.pending_fixups,
        static_cast<uint32_t>(request.second_page.size()),
        request.pointers_relocated,
        static_cast<uint32_t>(request.resource_types.size()),
        static_cast<uint32_t>(request.login_types.size()),
        request.level_info.loaded,
        request.level_info.value,
    };
}

uint32_t read_le32(const uint8_t *p) {
    return uint32_t{p[0]} | (uint32_t{p[1]} << 8) |
           (uint32_t{p[2]} << 16) | (uint32_t{p[3]} << 24);
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
    std::array<uint8_t, 12> first_entry{};
    for (uint32_t i = 0; inspected && i < request.page_count; ++i) {
        const uint64_t offset = request.page_table_offset + uint64_t{i} * entry.size();
        inspected = runtime_file_pread(fd, entry.data(), entry.size(), offset) ==
                    static_cast<int64_t>(entry.size());
        if (i == 0 && inspected) first_entry = entry;
    }
    // Incremental payload: menu-temple's two observed kind-0 pages.
    // Host-owned page storage stays fixed once pointers have been relocated;
    // no executable memory is allocated.
    if (inspected && request.page_count && request.name == "menu-temple") {
        const uint32_t offset = read_le32(first_entry.data());
        const uint32_t page_size = read_le32(first_entry.data() + 4);
        constexpr uint32_t kInspectionBudget = 4 * 1024 * 1024;
        if (read_le32(first_entry.data() + 8) == 0 && page_size >= 16 &&
            page_size <= kInspectionBudget && offset <= kInspectionBudget &&
            offset <= static_cast<uint64_t>(size) &&
            page_size <= static_cast<uint64_t>(size) - offset) {
            try {
                std::vector<uint8_t> page(page_size);
                if (runtime_file_pread(fd, page.data(), page.size(), offset) == page_size &&
                    u2_cpp_PackageMgr_CheckPage(first_entry.data(), first_entry.size(),
                        static_cast<uint64_t>(size), page.data(), page.size())) {
                    request.first_page = std::move(page);
                    request.first_page_offset = offset;
                    request.first_page_verified = true;
                    std::vector<uint8_t> header(offset);
                    if (runtime_file_pread(fd, header.data(), header.size(), 0) == offset &&
                        u2_cpp_PackageMgr_AuditFixups(header.data(), header.size(),
                            request.first_page.data(), request.first_page.size(),
                            &request.local_fixups, &request.pending_fixups)) {
                        std::printf("[U2 Native PackageMgr] fixups audited local=%u pending=%u (no writes)\n",
                                    request.local_fixups, request.pending_fixups);
                        std::array<uint8_t, 12> second_entry{};
                        if (request.page_count == 2 && runtime_file_pread(fd, second_entry.data(),
                            second_entry.size(), uint64_t{request.page_table_offset} + 12) == 12) {
                            const uint32_t offset1 = read_le32(second_entry.data());
                            const uint32_t size1 = read_le32(second_entry.data() + 4);
                            if (size1 >= 16 && size1 <= kInspectionBudget &&
                                offset1 <= static_cast<uint64_t>(size) &&
                                size1 <= static_cast<uint64_t>(size) - offset1) {
                                request.second_page.resize(size1);
                                if (runtime_file_pread(fd, request.second_page.data(), size1, offset1) == size1 &&
                                    u2_cpp_PackageMgr_CheckPage(second_entry.data(), second_entry.size(),
                                        static_cast<uint64_t>(size), request.second_page.data(), size1) &&
                                    u2_cpp_PackageMgr_ReadResourceTypes(request.first_page.data(),
                                        request.first_page.size(), &request.resource_types) &&
                                    u2_cpp_PackageMgr_ReadLoginTypes(header.data(), header.size(),
                                        request.first_page.data(), request.first_page.size(),
                                        request.second_page.data(), size1, &request.login_types) &&
                                    u2_cpp_PackageMgr_RelocatePages(header.data(), header.size(),
                                        request.first_page.data(), request.first_page.size(),
                                        request.second_page.data(), size1, &request.pointers_relocated)) {
                                    std::printf("[U2 Native PackageMgr] page1 verified bytes=%u; relocated=%u\n",
                                                size1, request.local_fixups + request.pending_fixups);
                                    std::printf("[U2 Native PackageMgr] resource catalog entries=%zu (login pending)\n",
                                                request.resource_types.size());
                                    for (size_t i = 0; i < request.login_types.size(); ++i)
                                        std::printf("[U2 Native PackageMgr] login[%zu]=%s (pending)\n", i, request.login_types[i].c_str());
                                    // The first login item is established as LEVEL_INFO_1.
                                    // Metadata name/type pointers are still page-relative.
                                    const uint8_t *pages[] = {request.first_page.data(), request.second_page.data()};
                                    const size_t sizes[] = {request.first_page.size(), request.second_page.size()};
                                    const uint32_t root_index = read_le32(header.data() + 8);
                                    const auto *root = pages[root_index] + read_le32(header.data() + 12);
                                    if (!request.login_types.empty() && request.login_types[0] == "LEVEL_INFO_1") {
                                        const uint32_t page_index = read_le32(root + 0x28);
                                        const uint32_t metadata = read_le32(root + 0x2C);
                                        if (u2_native_LoginLevelInfo(&request.level_info, pages[page_index], sizes[page_index], metadata))
                                            std::printf("[U2 Native Level] LEVEL_INFO_1 loaded value=0x%08X; remaining login=9\n", request.level_info.value);
                                    }
                                } else {
                                    request.second_page.clear();
                                    request.resource_types.clear();
                                    request.login_types.clear();
                                    std::fprintf(stderr, "[U2 Native PackageMgr] page1/relocation rejected\n");
                                }
                            }
                        }
                    }
                }
            } catch (const std::bad_alloc &) {
                std::fprintf(stderr, "[U2 Native PackageMgr] page inspection allocation failed\n");
            }
        }
        std::printf("[U2 Native PackageMgr] '%s' page0=%s offset=0x%X bytes=%zu\n",
                    request.name.c_str(), request.first_page_verified ? "verified" : "rejected",
                    offset, request.first_page.size());
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

// DoLogin 0xB6B9F2..0xB6BA49: root page/offset at PakHdr+8/+12,
// count at root+0x20 and signed (page, metadata offset) pairs at root+0x28.
// Input is raw pages, before metadata name/type normalization.
extern "C" bool u2_cpp_PackageMgr_ReadLoginTypes(const uint8_t *header, size_t header_size,
                                                const uint8_t *page0, size_t size0,
                                                const uint8_t *page1, size_t size1,
                                                std::vector<std::string> *types) {
    if (!header || header_size < 16 || !page0 || !page1 || !types) return false;
    const uint8_t *pages[] = {page0, page1};
    const size_t sizes[] = {size0, size1};
    const uint32_t root_page = read_le32(header + 8), root = read_le32(header + 12);
    if (root_page > 1 || root > 0x7FFFFFFFu || root > sizes[root_page] ||
        0x28 > sizes[root_page] - root) return false;
    const auto *data = pages[root_page] + root;
    const uint32_t count = read_le32(data + 0x20);
    if (count > 0x7FFFFFFFu || count > (sizes[root_page] - root - 0x28) / 8) return false;
    try {
        std::vector<std::string> result;
        for (size_t i = 0; i < count; ++i) {
            const auto *entry = data + 0x28 + i * 8;
            const uint32_t index = read_le32(entry), offset = read_le32(entry + 4);
            if (index > 1 || offset > 0x7FFFFFFFu || offset > sizes[index] ||
                32 > sizes[index] - offset) return false;
            uint64_t type_offset = 0;
            for (unsigned b = 0; b < 8; ++b) type_offset |= uint64_t{pages[index][offset + 8 + b]} << (b * 8);
            if (type_offset >= sizes[index]) return false;
            const auto *start = pages[index] + type_offset;
            const auto *end = static_cast<const uint8_t *>(std::memchr(start, 0, sizes[index] - type_offset));
            if (!end) return false;
            result.emplace_back(reinterpret_cast<const char *>(start), end - start);
        }
        *types = std::move(result);
        return true;
    } catch (const std::bad_alloc &) { return false; }
}

extern "C" const char *u2_cpp_PackageMgr_GetLoginType(const char *package, size_t index) {
    if (!package || !*package) return nullptr;
    std::lock_guard lock(s_request_mutex);
    const auto *request = find_request(string_id(package));
    return request && index < request->login_types.size() ? request->login_types[index].c_str() : nullptr;
}

// LoadingPages 0xB69FB0..0xB6A026: uint16 count at +0x12,
// 16-byte descriptors at +0x14, metadata offset at descriptor+8.
// Metadata's first two uint64 values are page-relative name/type strings.
// Parse before the original's separate metadata-pointer normalization stage.
extern "C" bool u2_cpp_PackageMgr_ReadResourceTypes(const uint8_t *page, size_t size,
                                                   std::vector<std::string> *types) {
    if (!page || !types || size < 0x14) return false;
    const uint32_t count = uint32_t{page[0x12]} | (uint32_t{page[0x13]} << 8);
    if (count > (size - 0x14) / 16) return false;
    try {
        std::vector<std::string> result;
        for (size_t i = 0; i < count; ++i) {
            const uint32_t metadata = read_le32(page + 0x14 + i * 16 + 8);
            if (metadata > 0x7FFFFFFFu || metadata > size || 32 > size - metadata) return false;
            std::string type;
            for (size_t field = 0; field < 2; ++field) {
                uint64_t offset = 0;
                for (unsigned b = 0; b < 8; ++b) offset |= uint64_t{page[metadata + field * 8 + b]} << (b * 8);
                if (offset >= size) return false;
                const auto *end = static_cast<const uint8_t *>(std::memchr(page + offset, 0, size - offset));
                if (!end) return false;
                if (field == 1) type.assign(reinterpret_cast<const char *>(page + offset), end - (page + offset));
            }
            result.push_back(std::move(type));
        }
        *types = std::move(result);
        return true;
    } catch (const std::bad_alloc &) {
        return false;
    }
}

extern "C" size_t u2_cpp_PackageMgr_CountResourceType(const char *package, const char *type) {
    if (!package || !*package || !type) return 0;
    std::lock_guard lock(s_request_mutex);
    const auto *request = find_request(string_id(package));
    if (!request) return 0;
    return static_cast<size_t>(std::count(request->resource_types.begin(), request->resource_types.end(), type));
}

extern "C" bool u2_cpp_PackageMgr_RelocatePages(const uint8_t *header, size_t header_size,
                                               uint8_t *page0, size_t size0, uint8_t *page1, size_t size1,
                                               bool *relocated) {
    if (!relocated || *relocated || !page0 || !page1) return false;
    uint32_t local = 0, pending = 0;
    if (!u2_cpp_PackageMgr_AuditFixups(header, header_size, page0, size0, &local, &pending)) return false;
    // Prepare every write first. No mutation is allowed until the entire plan
    // is valid. Vectors owning page0/page1 must not resize after this call.
    struct Write { uint32_t destination; uint32_t slot; uint64_t value; };
    try {
        std::vector<Write> writes;
        const uint32_t table = read_le32(header + 0x1C);
        uint8_t *pages[] = {page0, page1};
        const size_t sizes[] = {size0, size1};
        for (size_t group = 0; group < 8; ++group) {
            const auto *desc = header + table + group * 12;
            const uint32_t offset = read_le32(desc + 4), count = read_le32(desc + 8);
            for (size_t i = 0; i < count; ++i) {
                const auto *record = header + offset + i * 8;
                const uint32_t destination = read_le32(record) & 0x7FFF;
                const uint32_t slot = read_le32(record + 4);
                if (slot > 0x7FFFFFFFu || slot % 8 || slot > sizes[destination] ||
                    8 > sizes[destination] - slot) return false;
                uint64_t target = 0;
                for (unsigned b = 0; b < 8; ++b) target |= uint64_t{pages[destination][slot + b]} << (b * 8);
                const uint64_t base = reinterpret_cast<uintptr_t>(page0);
                if (target >= size0 || target > UINT64_MAX - base) return false;
                writes.push_back({destination, slot, base + target});
            }
        }
        std::sort(writes.begin(), writes.end(), [](const Write &a, const Write &b) {
            return a.destination < b.destination || (a.destination == b.destination && a.slot < b.slot);
        });
        for (size_t i = 1; i < writes.size(); ++i) {
            if (writes[i].destination == writes[i-1].destination && writes[i].slot == writes[i-1].slot) return false;
        }
        for (const auto &write : writes) {
            std::memcpy(pages[write.destination] + write.slot, &write.value, sizeof(write.value));
        }
        *relocated = true;
        return true;
    } catch (const std::bad_alloc &) {
        return false;
    }
}

// Read-only preflight of the eight groups consumed by FixUpPakPointers
// (0xB6E800). Only destination/base page 0 is available in this milestone.
extern "C" bool u2_cpp_PackageMgr_AuditFixups(const uint8_t *header, size_t header_size,
                                             const uint8_t *page, size_t page_size,
                                             uint32_t *local, uint32_t *pending) {
    if (!local || !pending) return false;
    *local = *pending = 0;
    if (!header || header_size < 0x20 || !page || read_le32(header + 0x18) != 8) return false;
    const uint32_t table = read_le32(header + 0x1C);
    if (table > 0x7FFFFFFFu || table > header_size || 96 > header_size - table) return false;
    uint64_t local_count = 0, pending_count = 0;
    for (size_t group = 0; group < 8; ++group) {
        const auto *desc = header + table + group * 12;
        const uint32_t count = read_le32(desc + 8);
        if (count > 0x7FFFFFFFu) return false;
        if (!count) continue;
        // Nonzero source-page groups need additional loaded pages.
        if (read_le32(desc) != 0) return false;
        const uint32_t offset = read_le32(desc + 4);
        if (offset > 0x7FFFFFFFu || offset > header_size || count > (header_size - offset) / 8) return false;
        for (size_t i = 0; i < count; ++i) {
            const auto *record = header + offset + i * 8;
            const uint32_t indices = read_le32(record);
            const uint32_t destination = indices & 0x7FFF;
            if ((indices & 0x8000) || (indices >> 16) != 0 || destination > 1) return false;
            if (destination == 1) { ++pending_count; continue; }
            const uint32_t slot = read_le32(record + 4);
            if (slot > 0x7FFFFFFFu || slot % 8 || slot > page_size || 8 > page_size - slot) return false;
            uint64_t target = 0;
            for (unsigned byte = 0; byte < 8; ++byte) target |= uint64_t{page[slot + byte]} << (byte * 8);
            if (target >= page_size) return false;
            ++local_count;
        }
    }
    if (local_count > UINT32_MAX || pending_count > UINT32_MAX) return false;
    *local = static_cast<uint32_t>(local_count);
    *pending = static_cast<uint32_t>(pending_count);
    return true;
}

extern "C" bool u2_cpp_PackageMgr_CheckPage(const uint8_t *entry, size_t entry_size,
                                            uint64_t file_size, const uint8_t *page, size_t page_size) {
    if (!entry || entry_size < 12 || !page || page_size < 16) return false;
    const uint32_t offset = read_le32(entry);
    const uint32_t size = read_le32(entry + 4);
    return read_le32(entry + 8) == 0 && offset <= file_size &&
           size <= file_size - offset && page_size == size &&
           read_le32(page + 0x0C) == size;
}

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
