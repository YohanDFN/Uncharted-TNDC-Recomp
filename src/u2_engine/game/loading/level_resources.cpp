#include "level_resources.h"
#include <cstring>

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
