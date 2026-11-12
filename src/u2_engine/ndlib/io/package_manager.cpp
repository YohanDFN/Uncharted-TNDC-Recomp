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
#include "game/camera/camera_position.h"

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
    U2NativeLevelOffset level_offset;
    bool cameras_decoded{false};
    std::vector<U2NativeCameraDefinition> cameras;
    bool geometry_preview_decoded{false};
    std::vector<U2NativeMeshPreview> geometry_preview;
    bool texture_table_loaded{false};
    bool material_images_bound{false};
    uint64_t vram_file_offset{0},vram_bytes{0};
    std::vector<U2NativeTexturePreview> texture_images;
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
        request.cameras_decoded,
        static_cast<uint32_t>(request.cameras.size()),
        request.level_offset.decoded,
        request.texture_table_loaded,
        static_cast<uint32_t>(request.texture_images.size()),
        static_cast<uint32_t>(std::count_if(request.texture_images.begin(),request.texture_images.end(),
            [](const auto &image) { return !image.pixels.rgba.empty(); })),
        request.vram_file_offset,request.vram_bytes,
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
                                    // Snapshot resource data only; engine login ordering remains pending.
                                    for (size_t i = 0; i < request.login_types.size(); ++i) {
                                        if (request.login_types[i] != "LEVEL_OFFSET_1") continue;
                                        const uint32_t page_index = read_le32(root + 0x28 + i * 8);
                                        const uint32_t metadata = read_le32(root + 0x2C + i * 8);
                                        if (u2_native_ReadLevelOffset(&request.level_offset, pages[page_index], sizes[page_index], metadata))
                                            std::printf("[U2 Native Level] LEVEL_OFFSET_1 decoded translation=(%.9f, %.9f, %.9f) (engine login pending)\n",
                                                request.level_offset.translation[0], request.level_offset.translation[1], request.level_offset.translation[2]);
                                    }
                                    for (size_t i = 0; i < request.login_types.size(); ++i) {
                                        if (request.login_types[i] != "CAMERA_TABLE_1") continue;
                                        const uint32_t page_index = read_le32(root + 0x28 + i * 8);
                                        const uint32_t metadata = read_le32(root + 0x2C + i * 8);
                                        request.cameras_decoded = u2_native_ReadCameraDefinitions(&request.cameras,
                                            pages[page_index], sizes[page_index], metadata);
                                        std::printf("[U2 Native Camera] CAMERA_TABLE_1 decoded=%u definitions=%zu (engine login pending)\n",
                                                    request.cameras_decoded, request.cameras.size());
                                        for (auto &camera : request.cameras) {
                                            camera.position_translated = u2_native_TranslateCameraPosition(camera,
                                                request.level_offset, &camera.translated_position);
                                            if (camera.is_start) {
                                                std::printf("[U2 Native Camera] start='%s' position=(%.6f, %.6f, %.6f)\n",
                                                    camera.name.c_str(), camera.position[0], camera.position[1], camera.position[2]);
                                                std::printf("[U2 Native Camera] stored orientation A=(%.6f, %.6f, %.6f) B=(%.6f, %.6f, %.6f) external-transform=%u\n",
                                                    camera.orientation_axis_a[0], camera.orientation_axis_a[1], camera.orientation_axis_a[2],
                                                    camera.orientation_axis_b[0], camera.orientation_axis_b[1], camera.orientation_axis_b[2],
                                                    camera.uses_external_transform);
                                                if (camera.orientation_calculated)
                                                    std::printf("[U2 Native Camera] quaternion xyzw=(%.9f, %.9f, %.9f, %.9f)\n",
                                                        camera.orientation_xyzw[0], camera.orientation_xyzw[1],
                                                        camera.orientation_xyzw[2], camera.orientation_xyzw[3]);
                                                if (camera.position_translated)
                                                    std::printf("[U2 Native Camera] translated position=(%.9f, %.9f, %.9f)\n",
                                                        camera.translated_position[0], camera.translated_position[1], camera.translated_position[2]);
                                            }
                                        }
                                    }
                                    for (size_t i=0;i<request.login_types.size();++i) {
                                        if (request.login_types[i]!="GEOMETRY_1") continue;
                                        const uint32_t page_index=read_le32(root+0x28+i*8);
                                        const uint32_t metadata=read_le32(root+0x2C+i*8);
                                        request.geometry_preview_decoded=u2_native_ReadGeometryPreview(&request.geometry_preview,
                                            pages[page_index],sizes[page_index],metadata);
                                        size_t vertices=0,triangles=0,normals=0,texcoords=0,materials=0,texture_refs=0;
                                        for (const auto &mesh : request.geometry_preview) {
                                            vertices+=mesh.positions.size();triangles+=mesh.triangles.size();normals+=mesh.normals.size();
                                            texcoords+=mesh.texcoords.size();
                                            materials+=mesh.material_index>=0;
                                            texture_refs+=mesh.material_textures.size();
                                        }
                                        std::printf("[U2 Native Geometry] decoded=%u meshes=%zu positions=%zu triangles=%zu normals=%zu uv=%zu materials=%zu texture-refs=%zu (preview geometry; engine login pending)\n",
                                            request.geometry_preview_decoded,request.geometry_preview.size(),vertices,triangles,normals,texcoords,materials,texture_refs);
                                    }
                                    for(size_t i=0;i<request.login_types.size();++i) {
                                        if(request.login_types[i]!="TEXTURE_TABLE") continue;
                                        uint64_t vram_offset=0,vram_bytes=0;
                                        if(u2_cpp_PackageMgr_InspectVramExtent(header.data(),header.size(),uint64_t(size),&vram_offset,&vram_bytes)) {
                                            std::vector<uint8_t> vram(static_cast<size_t>(vram_bytes));
                                            const uint32_t page_index=read_le32(root+0x28+i*8),metadata=read_le32(root+0x2C+i*8);
                                            if(runtime_file_pread(fd,vram.data(),vram.size(),int64_t(vram_offset))==int64_t(vram.size()) &&
                                               u2_native_ReadTextureTablePreview(&request.texture_images,pages[page_index],sizes[page_index],metadata,
                                                                                 vram.data(),vram.size())) {
                                                request.texture_table_loaded=true;
                                                request.vram_file_offset=vram_offset;request.vram_bytes=vram_bytes;
                                            }
                                        }
                                        const size_t decoded=std::count_if(request.texture_images.begin(),request.texture_images.end(),
                                            [](const auto &image) { return !image.pixels.rgba.empty(); });
                                        std::printf("[U2 Native Textures] loaded=%u images=%zu decoded-bc1=%zu compressed-only=%zu vram-offset=0x%llX bytes=%llu (owned preview; engine login pending)\n",
                                            request.texture_table_loaded,request.texture_images.size(),decoded,request.texture_images.size()-decoded,
                                            static_cast<unsigned long long>(request.vram_file_offset),static_cast<unsigned long long>(request.vram_bytes));
                                    }
                                    if(request.geometry_preview_decoded && request.texture_table_loaded) {
                                        request.material_images_bound=u2_native_BindGeometryTexturePreviews(&request.geometry_preview,request.texture_images);
                                        size_t bound=0,decoded=0;
                                        for(const auto &mesh:request.geometry_preview) for(const auto &ref:mesh.material_textures) {
                                            if(ref.image_index<0 || size_t(ref.image_index)>=request.texture_images.size()) continue;
                                            ++bound;decoded+=!request.texture_images[size_t(ref.image_index)].pixels.rgba.empty();
                                        }
                                        std::printf("[U2 Native Material Images] linked=%u references=%zu decoded=%zu (owned preview; shader/sampler binding pending)\n",
                                            request.material_images_bound,bound,decoded);
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

// Observed uncompressed two-page package: VRAM follows the last CPU page.
// The declared header+20 extent must terminate exactly at EOF; no fixed offset.
extern "C" bool u2_cpp_PackageMgr_InspectVramExtent(const uint8_t *header,size_t header_size,uint64_t file_size,
                                                   uint64_t *offset,uint64_t *bytes) {
    if(!header || !offset || !bytes || header_size<0x24 || read_le32(header+0x10)!=2) return false;
    const uint32_t table=read_le32(header+0x14),length=read_le32(header+0x20);
    if(table>header_size || 24>header_size-table || length>16*1024*1024) return false;
    uint64_t end=header_size;
    for(size_t i=0;i<2;++i) {
        const auto *entry=header+table+i*12;
        const uint64_t begin=read_le32(entry),size=read_le32(entry+4);
        if(read_le32(entry+8)!=0 || size<16 || begin<end || begin>file_size || size>file_size-begin) return false;
        end=begin+size;
    }
    if(end>file_size || length!=file_size-end) return false;
    *offset=end;*bytes=length;return true;
}

extern "C" bool u2_cpp_PackageMgr_LoadNativeTitleLogoPreview(U2NativeTexturePixels *out) {
    if(!out) return false;
    const int fd=runtime_file_open("/app0/u2data/build/main/pak22/ps4/menu.pak",0,0);
    if(fd<0) return false;
    struct Close { int fd;~Close() { runtime_file_close(fd); } } close{fd};
    try {
        const int64_t file_size=runtime_file_lseek(fd,0,2);
        std::array<uint8_t,0x2C> prefix{};
        uint32_t count=0,table=0;
        if(file_size<0 || runtime_file_pread(fd,prefix.data(),prefix.size(),0)!=int64_t(prefix.size()) ||
           !u2_cpp_PackageMgr_InspectHeader(prefix.data(),prefix.size(),uint64_t(file_size),&count,&table) || count!=2) return false;
        std::array<uint8_t,24> entries{};
        if(runtime_file_pread(fd,entries.data(),entries.size(),table)!=int64_t(entries.size())) return false;
        const uint32_t header_bytes=read_le32(entries.data());
        if(header_bytes< uint64_t(table)+entries.size() || header_bytes>4*1024*1024) return false;
        std::vector<uint8_t> header(header_bytes);
        if(runtime_file_pread(fd,header.data(),header.size(),0)!=int64_t(header.size())) return false;
        // The menu VRAM bank exceeds the resident-loader's 16 MiB budget.
        // Validate its extent without allocating it; read just the two planes.
        uint64_t vram_start=header_bytes;
        for(size_t i=0;i<2;++i) {
            const auto *e=entries.data()+i*12;
            const uint64_t begin=read_le32(e),bytes=read_le32(e+4);
            if(read_le32(e+8)!=0 || begin<vram_start || begin>uint64_t(file_size) || bytes>uint64_t(file_size)-begin) return false;
            vram_start=begin+bytes;
        }
        const uint64_t vram_bytes=uint64_t(file_size)-vram_start;
        if(read_le32(header.data()+0x20)!=vram_bytes) return false;
        std::array<std::vector<uint8_t>,2> pages;
        for(size_t i=0;i<2;++i) {
            const auto *e=entries.data()+i*12;
            const uint32_t bytes=read_le32(e+4);
            if(bytes<16 || bytes>4*1024*1024) return false;
            pages[i].resize(bytes);
            if(runtime_file_pread(fd,pages[i].data(),bytes,read_le32(e))!=int64_t(bytes) ||
               !u2_cpp_PackageMgr_CheckPage(e,12,uint64_t(file_size),pages[i].data(),bytes)) return false;
        }
        std::vector<std::string> types;bool relocated=false;
        if(!u2_cpp_PackageMgr_ReadLoginTypes(header.data(),header.size(),pages[0].data(),pages[0].size(),pages[1].data(),pages[1].size(),&types) ||
           !u2_cpp_PackageMgr_RelocatePages(header.data(),header.size(),pages[0].data(),pages[0].size(),pages[1].data(),pages[1].size(),&relocated)) return false;
        const auto *root=pages[read_le32(header.data()+8)].data()+read_le32(header.data()+12);
        std::array<std::vector<uint8_t>,2> blocks;
        const char *cooked[]={
            "d:/dev/CP9/Data/U2/CookData/ND/build/texture2/art/general/menu/logo2.tga/3DA08A9F6983BA12.ndb",
            "d:/dev/CP9/Data/U2/CookData/ND/build/texture2/art/general/menu/logo2-alpha.tga/4C70F66839038231.ndb"};
        for(size_t login=0;login<types.size();++login) {
            if(types[login]!="TEXTURE_TABLE") continue;
            const uint32_t pi=read_le32(root+0x28+login*8),md=read_le32(root+0x2C+login*8);
            const auto &page=pages[pi];const size_t size=page.size();
            const auto range=[&](uint64_t address,size_t bytes,size_t &offset) {
                const auto base=reinterpret_cast<uintptr_t>(page.data());
                if(address<base || address-base>size || bytes>size-(address-base)) return false;
                offset=size_t(address-base);return true;
            };
            if(md>size || size-md<0x40) return false;
            const auto *m=page.data()+md;
            const auto u64=[](const uint8_t *p) { uint64_t n=0;for(unsigned i=0;i<8;++i)n|=uint64_t(p[i])<<(8*i);return n; };
            const uint32_t textures=read_le32(m+0x20);
            size_t keys=0,descriptors=0;
            if(textures>4096 || !range(u64(m+0x28),size_t(textures)*4,keys) || !range(u64(m+0x38),size_t(textures)*8,descriptors)) return false;
            for(size_t i=0;i<textures;++i) {
                size_t at=0,name=0;
                if(!range(u64(page.data()+descriptors+i*8),64,at)) return false;
                const auto *d=page.data()+at;
                if(!range(u64(d+0x38),1,name) || !std::memchr(page.data()+name,0,size-name)) return false;
                for(size_t slot=0;slot<2;++slot) {
                    if(std::strcmp(reinterpret_cast<const char*>(page.data()+name),cooked[slot])) continue;
                    if(!blocks[slot].empty()) return false;
                    const uint64_t offset=u64(d+8);const uint32_t bytes=read_le32(d+0x10);
                    if(read_le32(d+0x14)!=read_le32(page.data()+keys+i*4) || read_le32(d+0x18)!=1 ||
                       read_le32(d+0x1C)!=(slot==0 ? 71u : 80u) || read_le32(d+0x20)!=0x01000000 ||
                       read_le32(d+0x24)!=12 || read_le32(d+0x28)!=2048 || read_le32(d+0x2C)!=512 ||
                       read_le32(d+0x34)!=3 || bytes!=702464 || offset>vram_bytes || bytes>vram_bytes-offset) return false;
                    blocks[slot].resize(bytes);
                    if(runtime_file_pread(fd,blocks[slot].data(),bytes,int64_t(vram_start+offset))!=int64_t(bytes)) return false;
                }
            }
        }
        if(blocks[0].empty() || blocks[1].empty()) return false;
        return u2_native_BuildTitleLogoPreview(out,blocks[0].data(),blocks[0].size(),blocks[1].data(),blocks[1].size(),2048,512);
    } catch(const std::bad_alloc &) { return false; }
}

extern "C" bool u2_cpp_PackageMgr_LoadNativeDictionaryTexturePreview(const char *dictionary,const char *name,
                                                                    U2NativeTexturePreview *out) {
    if(!dictionary || !*dictionary || !name || !*name || !out) return false;
    size_t length=0;
    for(const char *c=dictionary;*c;++c) {
        if(++length>96 || !((*c>='a' && *c<='z') || (*c>='A' && *c<='Z') ||
             (*c>='0' && *c<='9') || *c=='-' || *c=='_')) return false;
    }
    try {
        const std::string path="/app0/u2data/build/main/texturedict1/ps4/"+std::string(dictionary)+".pak";
        const int fd=runtime_file_open(path.c_str(),0,0);
        if(fd<0) return false;
        struct Close { int fd;~Close() { runtime_file_close(fd); } } close{fd};
        const int64_t file_size=runtime_file_lseek(fd,0,2);
        std::array<uint8_t,0x2C> header{};
        if(file_size<int64_t(header.size()) || runtime_file_pread(fd,header.data(),header.size(),0)!=int64_t(header.size())) return false;
        uint32_t count=0,table=0;
        if(!u2_cpp_PackageMgr_InspectHeader(header.data(),header.size(),uint64_t(file_size),&count,&table) || count!=1 ||
           read_le32(header.data()+8)!=0) return false;
        std::array<uint8_t,12> entry{};
        if(runtime_file_pread(fd,entry.data(),entry.size(),table)!=int64_t(entry.size())) return false;
        const uint64_t begin=read_le32(entry.data()),bytes=read_le32(entry.data()+4);
        if(read_le32(entry.data()+8)!=0 || begin<uint64_t(table)+entry.size() || !bytes || bytes>4*1024*1024 ||
           begin>uint64_t(file_size) || bytes>uint64_t(file_size)-begin) return false;
        const uint64_t vram_start=begin+bytes,vram_bytes=uint64_t(file_size)-vram_start;
        if(read_le32(header.data()+0x20)!=vram_bytes) return false;
        std::vector<uint8_t> page(static_cast<size_t>(bytes));
        if(runtime_file_pread(fd,page.data(),page.size(),int64_t(begin))!=int64_t(page.size()) ||
           !u2_cpp_PackageMgr_CheckPage(entry.data(),entry.size(),uint64_t(file_size),page.data(),page.size())) return false;
        U2NativeDictionaryTextureDescriptor descriptor;
        if(!u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),read_le32(header.data()+12),name,vram_bytes)) return false;
        auto &image=descriptor.image;
        image.resident_blocks.resize(descriptor.stored_bytes);
        if(runtime_file_pread(fd,image.resident_blocks.data(),image.resident_blocks.size(),int64_t(vram_start+image.vram_offset))!=int64_t(image.resident_blocks.size()) ||
           !u2_native_DecodeThinBc1Preview(&image.pixels,image.resident_blocks.data(),image.resident_blocks.size(),image.width,image.height,image.format==72)) return false;
        *out=std::move(image);return true;
    } catch(const std::bad_alloc &) { return false; }
}

extern "C" bool u2_cpp_PackageMgr_GetNativeTexturePreview(const char *package,size_t index,U2NativeTexturePreview *out) {
    if(!package || !*package || !out) return false;
    std::lock_guard lock(s_request_mutex);
    const auto *request=find_request(string_id(package));
    if(!request || !request->texture_table_loaded || index>=request->texture_images.size()) return false;
    try { auto snapshot=request->texture_images[index];*out=std::move(snapshot);return true; }
    catch(const std::bad_alloc &) { return false; }
}

extern "C" bool u2_cpp_PackageMgr_GetNativeMaterialTexturePreview(const char *package,size_t mesh,size_t reference,
                                                                  U2NativeTexturePreview *out) {
    if(!package || !*package || !out) return false;
    std::lock_guard lock(s_request_mutex);
    const auto *request=find_request(string_id(package));
    if(!request || !request->material_images_bound || mesh>=request->geometry_preview.size()) return false;
    const auto &refs=request->geometry_preview[mesh].material_textures;
    if(reference>=refs.size() || refs[reference].image_index<0 || size_t(refs[reference].image_index)>=request->texture_images.size()) return false;
    const auto &image=request->texture_images[size_t(refs[reference].image_index)];
    if(!u2_native_TextureImageMatchesReference(refs[reference],image)) return false;
    try { auto snapshot=image;*out=std::move(snapshot);return true; }
    catch(const std::bad_alloc &) { return false; }
}

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

extern "C" bool u2_cpp_PackageMgr_GetNativeGeometryPreview(const char *package, std::vector<U2NativeMeshPreview> *out) {
    if (!package || !*package || !out) return false;
    std::lock_guard lock(s_request_mutex);
    const auto *request=find_request(string_id(package));
    if (!request || !request->geometry_preview_decoded) return false;
    try {
        auto copy=request->geometry_preview;
        *out=std::move(copy);
        return true;
    } catch (const std::bad_alloc &) { return false; }
}

extern "C" bool u2_cpp_PackageMgr_GetNativeLevelOffset(const char *package, U2NativeLevelOffset *out) {
    if (!package || !*package || !out) return false;
    std::lock_guard lock(s_request_mutex);
    const auto *request = find_request(string_id(package));
    if (!request || !request->level_offset.decoded) return false;
    *out = request->level_offset;
    return true;
}

extern "C" bool u2_cpp_PackageMgr_GetNativeCamera(const char *package, size_t index,
                                                 U2NativeCameraDefinition *out) {
    if (!package || !*package || !out) return false;
    std::lock_guard lock(s_request_mutex);
    const auto *request = find_request(string_id(package));
    if (!request || !request->cameras_decoded || index >= request->cameras.size()) return false;
    try {
        auto camera = request->cameras[index];
        *out = std::move(camera);
        return true;
    } catch (const std::bad_alloc &) { return false; }
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
