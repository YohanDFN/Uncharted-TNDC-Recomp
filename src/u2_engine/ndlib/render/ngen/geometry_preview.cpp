#include "geometry_preview.h"
#include <bit>
#include <cmath>
#include <cstring>
#include <new>
#include <utility>

bool u2_native_ReadGeometryPreview(std::vector<U2NativeMeshPreview> *out,
                                   const uint8_t *page, size_t size, size_t metadata) {
    if (!out || !page || metadata > size || 0x78 > size - metadata) return false;
    auto u16 = [](const uint8_t *p) { return uint16_t(p[0] | (uint16_t(p[1]) << 8)); };
    auto u32 = [](const uint8_t *p) { return uint32_t(p[0]) | (uint32_t(p[1])<<8) | (uint32_t(p[2])<<16) | (uint32_t(p[3])<<24); };
    auto u64 = [](const uint8_t *p) {
        uint64_t value=0;
        for (unsigned i=0;i<8;++i) value |= uint64_t(p[i]) << (i*8);
        return value;
    };
    auto read_half = [&u16](const uint8_t *p,float &value) {
        const uint16_t half=u16(p);
        const unsigned exponent=(half>>10)&31, fraction=half&1023;
        if (exponent==31) return false;
        const float magnitude=exponent ? std::ldexp(float(1024+fraction),int(exponent)-25)
                                       : std::ldexp(float(fraction),-24);
        value=(half&0x8000) ? -magnitude : magnitude;
        return true;
    };
    constexpr char type[] = "GEOMETRY_1";
    const uint64_t type_offset=u64(page+metadata+8);
    if (type_offset > size || sizeof(type) > size-type_offset ||
        std::memcmp(page+type_offset,type,sizeof(type))) return false;
    auto range = [page,size](uint64_t address, size_t bytes, size_t &offset) {
        const auto base=reinterpret_cast<uintptr_t>(page);
        if (address<base || address-base>size || bytes>size-(address-base)) return false;
        offset=static_cast<size_t>(address-base);
        return true;
    };
    // GeoGroup mesh definitions: geometry payload+8/+10, stride 88 hex.
    // C29394/C29398 and C29660..C296F6; vertex count at MeshDesc+20
    // and stream count/pointer +28/+30 at C36E2E/C36EA2/C36F80..C36FAC.
    const uint32_t count=u32(page+metadata+0x28);
    size_t records=0;
    if (count && (!range(u64(page+metadata+0x30),0,records) || count>(size-records)/0x88)) return false;
    try {
        std::vector<U2NativeMeshPreview> result;
        result.reserve(count);
        size_t remaining_positions=size/12;
        size_t remaining_indices=size/2;
        for (size_t i=0;i<count;++i) {
            const auto *record=page+records+i*0x88;
            const uint32_t vertices=u32(record+0x20), streams=u32(record+0x28);
            // C36E27/C36E71: index count +24, index data +40, 16-bit format 39 hex.
            // The temple's complete buffers are matched to original GPU draws:
            // index_bytes=2, primitive=4 (TriangleList), primitive restart off.
            const uint32_t indices=u32(record+0x24);
            size_t index_data=0;
            if (!indices || indices%3 || indices>remaining_indices ||
                !range(u64(record+0x40),size_t(indices)*2,index_data)) return false;
            remaining_indices-=indices;
            size_t stream=0,attribute=0,data=0,name=0;
            if (!vertices || vertices>65536 || vertices>remaining_positions || !streams ||
                !range(u64(record+0x30),0,stream) || streams>(size-stream)/24 ||
                !range(u64(record+8),1,name)) return false;
            remaining_positions-=vertices;
            const auto *desc=page+stream;
            // C4E509..C4E543: u16 stride, u32 bytes, data pointer at +10.
            // Format 6 name is R32G32B32Float in the live format table +60.
            if (u16(desc)!=1 || u16(desc+2)!=12 || u32(desc+4)!=vertices*12 ||
                !range(u64(desc+8),4,attribute) || u32(page+attribute)!=0x060C0000 ||
                !range(u64(desc+0x10),vertices*12,data)) return false;
            const auto *end=static_cast<const uint8_t *>(std::memchr(page+name,0,size-name));
            if (!end) return false;
            U2NativeMeshPreview mesh;
            mesh.name.assign(reinterpret_cast<const char *>(page+name),end-(page+name));
            // C4E4F0 copies four-byte attribute definitions; descriptors are
            // 24 bytes apart at C36F80. Identify by ID, never by stream order.
            for (size_t s=0;s<streams;++s) {
                const auto *extra=page+stream+s*24;
                const uint16_t attributes=u16(extra);
                size_t definitions=0;
                if (!attributes || attributes>4 || !range(u64(extra+8),size_t(attributes)*4,definitions)) return false;
                for (size_t a=0;a<attributes;++a) {
                    if (page[definitions+a*4]==1) {
                        size_t uv_data=0;
                        // Format table entry 34 is R16G16Float in the original
                        // runtime. Canonical temple definition: 01 00 04 22.
                        if (!mesh.texcoords.empty() || attributes!=1 || u32(page+definitions)!=0x22040001 ||
                            u16(extra+2)!=4 || u32(extra+4)!=vertices*4 ||
                            !range(u64(extra+16),size_t(vertices)*4,uv_data)) return false;
                        mesh.texcoords.resize(vertices);
                        for (size_t v=0;v<vertices;++v) for (size_t component=0;component<2;++component)
                            if (!read_half(page+uv_data+v*4+component*2,mesh.texcoords[v][component])) return false;
                    }
                    if (page[definitions+a*4]!=2) continue;
                    size_t normal_data=0;
                    if (!mesh.normals.empty() || attributes!=1 || u32(page+definitions)!=0x0A080002 ||
                        u16(extra+2)!=8 || u32(extra+4)!=vertices*8 ||
                        !range(u64(extra+16),size_t(vertices)*8,normal_data)) return false;
                    mesh.normals.resize(vertices);
                    for (size_t v=0;v<vertices;++v) for (size_t component=0;component<4;++component)
                        if (!read_half(page+normal_data+v*8+component*2,mesh.normals[v][component])) return false;
                }
            }
            mesh.positions.resize(vertices);
            mesh.triangles.resize(indices/3);
            for (size_t triangle=0;triangle<mesh.triangles.size();++triangle) {
                for (size_t corner=0;corner<3;++corner) {
                    const uint16_t index=u16(page+index_data+(triangle*3+corner)*2);
                    if (index>=vertices) return false;
                    mesh.triangles[triangle][corner]=index;
                }
            }
            for (size_t v=0;v<vertices;++v) {
                for (size_t axis=0;axis<3;++axis) {
                    const float value=std::bit_cast<float>(u32(page+data+v*12+axis*4));
                    if (!std::isfinite(value)) return false;
                    mesh.positions[v][axis]=value;
                }
            }
            result.push_back(std::move(mesh));
        }
        *out=std::move(result);
        return true;
    } catch (const std::bad_alloc &) { return false; }
}
