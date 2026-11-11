#include "level_resources.h"
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
            result.push_back(std::move(camera));
        }
        *out = std::move(result);
        return true;
    } catch (const std::bad_alloc &) { return false; }
}
