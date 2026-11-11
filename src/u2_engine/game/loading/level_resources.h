#pragma once
#include <cstddef>
#include <cstdint>

// Native representation of the one established Level property. This is not
// a guest Level layout or a substitute for its constructor.
struct U2NativeLevelInfo {
    uint32_t value{0};
    bool loaded{false};
};

bool u2_native_LoginLevelInfo(U2NativeLevelInfo *level, const uint8_t *page,
                             size_t page_size, size_t metadata_offset);
