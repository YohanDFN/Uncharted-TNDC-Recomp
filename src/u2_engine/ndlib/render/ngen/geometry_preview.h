#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

// Owned preview geometry; not a guest Mesh or a completed geometry login.
struct U2NativeMeshPreview {
    std::string name;
    std::vector<std::array<float, 3>> positions;
    std::vector<std::array<uint16_t, 3>> triangles;
};

// Supports the established first position stream: one attribute, ID/offset 0,
// R32G32B32Float (format 6), stride 12. Other encodings/cross-page pointers fail.
bool u2_native_ReadGeometryPreview(std::vector<U2NativeMeshPreview> *out,
                                   const uint8_t *page, size_t page_size,
                                   size_t metadata_offset);
