#include "level_resources.h"
#include "../camera/camera_orientation.h"
#include <cstring>
#include <bit>
#include <cmath>
#include <new>
#include <utility>

// BigPackageLoginResItem 0x48EF3B..0x48EF61 (data-loading.cpp):
// StringId LEVEL_INFO_1 = 0xDD7F3F25, uint32 metadata+0x20 -> Level+0xA50.
// Here the destination is a host-owned property, never a guessed guest object.
bool u2_native_LoginLevelInfo(U2NativeLevelInfo *level, const uint8_t *page,
                             size_t page_size, size_t metadata_offset) {
    if (!level || level->loaded || !page || metadata_offset > page_size ||
        36 > page_size - metadata_offset) return false;
    uint64_t type_offset = 0;
    for (unsigned i = 0; i < 8; ++i)
        type_offset |= uint64_t{page[metadata_offset + 8 + i]} << (i * 8);
    constexpr char type[] = "LEVEL_INFO_1";
    if (type_offset > page_size || sizeof(type) > page_size - type_offset ||
        std::memcmp(page + type_offset, type, sizeof(type)) != 0) return false;
    const auto *p = page + metadata_offset + 0x20;
    const uint32_t value = uint32_t{p[0]} | (uint32_t{p[1]} << 8) |
                           (uint32_t{p[2]} << 16) | (uint32_t{p[3]} << 24);
    level->value = value;
    level->loaded = true;
    return true;
}

// LEVEL_OFFSET_1 hash A0C08245: 48E9E5..48EA2B copies three floats
// from metadata+20/+24/+28 into Level+20/+24/+28, with a null owner check.
bool u2_native_ReadLevelOffset(U2NativeLevelOffset *out, const uint8_t *page,
                              size_t page_size, size_t metadata_offset) {
    if (!out || !page || metadata_offset > page_size || 44 > page_size - metadata_offset)
        return false;
    uint64_t type_offset = 0;
    for (unsigned i = 0; i < 8; ++i)
        type_offset |= uint64_t{page[metadata_offset + 8 + i]} << (8 * i);
    constexpr char type[] = "LEVEL_OFFSET_1";
    if (type_offset > page_size || sizeof(type) > page_size - type_offset ||
        std::memcmp(page + type_offset, type, sizeof(type)) != 0) return false;
    U2NativeLevelOffset result;
    for (size_t axis = 0; axis < 3; ++axis) {
        const auto *p = page + metadata_offset + 0x20 + axis * 4;
        const uint32_t bits = uint32_t{p[0]} | (uint32_t{p[1]} << 8) |
                              (uint32_t{p[2]} << 16) | (uint32_t{p[3]} << 24);
        result.translation[axis] = std::bit_cast<float>(bits);
        if (!std::isfinite(result.translation[axis])) return false;
    }
    result.decoded = true;
    *out = result;
    return true;
}

// CameraManager message F2ACC8EC: 43535C/43536B read count/records,
// 435376/43538C use the record name, 435A21..435A2C use position +8/+C/+10,
// 4355E6..435629 read orientation vectors +20/+14 on the null +40 branch,
// and 435B28 advances by 0x48. Projection fields remain opaque.
bool u2_native_ReadCameraDefinitions(std::vector<U2NativeCameraDefinition> *out,
                                     const uint8_t *page, size_t page_size,
                                     size_t metadata_offset) {
    if (!out || !page || metadata_offset > page_size || 48 > page_size - metadata_offset)
        return false;
    auto le32 = [](const uint8_t *p) {
        return uint32_t{p[0]} | (uint32_t{p[1]} << 8) |
               (uint32_t{p[2]} << 16) | (uint32_t{p[3]} << 24);
    };
    auto le64 = [](const uint8_t *p) {
        uint64_t value = 0;
        for (unsigned i = 0; i < 8; ++i) value |= uint64_t{p[i]} << (8 * i);
        return value;
    };
    constexpr char type[] = "CAMERA_TABLE_1";
    const uint64_t type_offset = le64(page + metadata_offset + 8);
    if (type_offset > page_size || sizeof(type) > page_size - type_offset ||
        std::memcmp(page + type_offset, type, sizeof(type)) != 0) return false;

    // Check address arithmetic before converting a serialized pointer into an
    // offset. Reject cross-page references until that case is established.
    const auto base = reinterpret_cast<uintptr_t>(page);
    auto offset_in_page = [base, page_size](uint64_t address, size_t bytes, size_t *offset) {
        if (address < base || address - base > page_size ||
            bytes > page_size - (address - base)) return false;
        *offset = static_cast<size_t>(address - base);
        return true;
    };
    const uint32_t count = le32(page + metadata_offset + 0x20);
    size_t records = 0;
    if (count && (!offset_in_page(le64(page + metadata_offset + 0x28), 0, &records) ||
                  count > (page_size - records) / 0x48)) return false;
    try {
        std::vector<U2NativeCameraDefinition> result;
        result.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            const auto *record = page + records + i * 0x48;
            size_t name_offset = 0;
            if (!offset_in_page(le64(record), 1, &name_offset)) return false;
            const auto *start = page + name_offset;
            const auto *end = static_cast<const uint8_t *>(std::memchr(start, 0, page_size - name_offset));
            if (!end) return false;
            U2NativeCameraDefinition camera;
            camera.name.assign(reinterpret_cast<const char *>(start), end - start);
            camera.is_start = camera.name.find("_cam_start_") != std::string::npos;
            camera.uses_external_transform = le64(record + 0x40) != 0;
            for (size_t axis = 0; axis < 3; ++axis) {
                camera.position[axis] = std::bit_cast<float>(le32(record + 8 + axis * 4));
                camera.orientation_axis_a[axis] = std::bit_cast<float>(le32(record + 0x20 + axis * 4));
                camera.orientation_axis_b[axis] = std::bit_cast<float>(le32(record + 0x14 + axis * 4));
                if (!std::isfinite(camera.position[axis]) ||
                    !std::isfinite(camera.orientation_axis_a[axis]) ||
                    !std::isfinite(camera.orientation_axis_b[axis])) return false;
            }
            camera.orientation_calculated = u2_native_CalculateCameraOrientation(camera, &camera.orientation_xyzw);
            result.push_back(std::move(camera));
        }
        *out = std::move(result);
        return true;
    } catch (const std::bad_alloc &) { return false; }
}

// LIGHT_TABLE_1 wire format: 240-byte records, decoded by field rather than
// aliasing the serialized bytes as host structs. Publish only a complete table.
bool u2_native_ReadLightTable(U2NativeLightTable *out, const uint8_t *page,
                              size_t page_size, size_t metadata_offset) {
    if (!out || !page || metadata_offset > page_size || page_size-metadata_offset < 0x70) return false;
    auto le32=[](const uint8_t *p) {
        return uint32_t(p[0]) | (uint32_t(p[1])<<8) | (uint32_t(p[2])<<16) | (uint32_t(p[3])<<24);
    };
    auto le64=[](const uint8_t *p) {
        uint64_t value=0;
        for(unsigned i=0;i<8;++i) value|=uint64_t(p[i])<<(8*i);
        return value;
    };
    const auto *meta=page+metadata_offset;
    constexpr char type[]="LIGHT_TABLE_1";
    const uint64_t type_offset=le64(meta+8);
    if(type_offset>page_size || sizeof(type)>page_size-type_offset ||
       std::memcmp(page+type_offset,type,sizeof(type)) || le32(meta+0x14)!=0x00012345) return false;
    const uintptr_t base=reinterpret_cast<uintptr_t>(page);
    auto in_page=[&](uint64_t address,size_t bytes,size_t *offset) {
        if(address<base || address-base>page_size || bytes>page_size-(address-base)) return false;
        *offset=size_t(address-base);return true;
    };
    U2NativeLightTable result;
    const uint64_t dynamic=le64(meta+0x20);
    size_t dynamic_offset=0;
    if(dynamic) {
        if(!in_page(dynamic,1,&dynamic_offset)) return false;
        // The table's leading byte stores the dynamic-light count.
        result.dynamic_light_count=page[dynamic_offset];
    }
    const uint32_t count=le32(meta+0x3C);
    const uint64_t pointer=le64(meta+0x68);
    size_t records=0;
    if(pointer && !in_page(pointer,0,&records)) return false;
    if(count && (!pointer || count>(page_size-records)/240)) return false;
    try {
        result.probes.resize(count);
        for(size_t i=0;i<count;++i) {
            auto &probe=result.probes[i];
            for(size_t j=0;j<60;++j) {
                const float value=std::bit_cast<float>(le32(page+records+i*240+j*4));
                if(!std::isfinite(value)) return false;
                if(j<3) probe.position[j]=value;
                else if(j<30) probe.sh_direct[j-3]=value;
                else if(j<57) probe.sh_indirect[j-30]=value;
                else probe.sky_visibility[j-57]=value;
            }
        }
        result.decoded=true;*out=std::move(result);return true;
    } catch(const std::bad_alloc &) { return false; }
}
