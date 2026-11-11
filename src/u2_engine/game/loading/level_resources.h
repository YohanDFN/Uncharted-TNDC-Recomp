#pragma once
#include <cstddef>
#include <cstdint>
#include <array>
#include <string>
#include <vector>

// Native representation of the one established Level property. This is not
// a guest Level layout or a substitute for its constructor.
struct U2NativeLevelInfo {
    uint32_t value{0};
    bool loaded{false};
};

bool u2_native_LoginLevelInfo(U2NativeLevelInfo *level, const uint8_t *page,
                             size_t page_size, size_t metadata_offset);

// Owned snapshot of established camera-definition fields, not a live engine
// Camera or a guest Level. No pointer into a PAK page survives decoding.
struct U2NativeCameraDefinition {
    std::string name;
    std::array<float, 3> position{};
    bool is_start{false};
};

// CAMERA_TABLE_1 metadata name/type offsets remain page-relative; its payload
// and record name pointers must already be relocated into this same page.
bool u2_native_ReadCameraDefinitions(std::vector<U2NativeCameraDefinition> *out,
                                     const uint8_t *page, size_t page_size,
                                     size_t metadata_offset);
