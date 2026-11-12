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
    // Optional attribute ID 2, R16G16B16A16Float. Preserve all four components;
    // no normalization or assumption about the original lighting model.
    std::vector<std::array<float, 4>> normals;
    // Optional attribute ID 1, R16G16Float. Preserve negative and tiled coordinates.
    // Texture/material binding and sampler addressing remain separate work.
    std::vector<std::array<float, 2>> texcoords;
    std::vector<std::array<uint16_t, 3>> triangles;
};

// Supports the established first position stream: one attribute, ID/offset 0,
// R32G32B32Float (format 6), stride 12. Optional normal attribute ID 2 supports
// only offset 0, size/stride 8, format 10. Optional UV attribute ID 1 supports
// offset 0, size/stride 4, format 34. Other encodings/cross-page pointers fail.
bool u2_native_ReadGeometryPreview(std::vector<U2NativeMeshPreview> *out,
                                   const uint8_t *page, size_t page_size,
                                   size_t metadata_offset);
