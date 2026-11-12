#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include "texture_preview.h"

// Owned material references, not decoded images or shader/sampler bindings.
struct U2NativeTextureReferencePreview {
    std::string parameter_name;
    std::string source_path;
    std::string cooked_name;
    uint32_t cooked_name_hash{0};
    int32_t texture_index{-1}; // Geometry-local table, not a GPU texture index.
    int32_t image_index{-1}; // Owned TEXTURE_TABLE snapshot, independent of texture_index.
};

// Owned preview geometry; not a guest Mesh or a completed geometry login.
struct U2NativeMeshPreview {
    std::string name;
    // Original geometry-local material reference, resolved like PrepareBinary.
    // This is not a shader instance or a texture binding. Missing lookup = -1.
    std::string material_name;
    uint32_t material_name_hash{0};
    int32_t material_index{-1};
    std::vector<U2NativeTextureReferencePreview> material_textures;
    std::vector<std::array<float, 3>> positions;
    // Optional attribute ID 2, R16G16B16A16Float. Preserve all four components;
    // no normalization or assumption about the original lighting model.
    std::vector<std::array<float, 4>> normals;
    // Optional attribute ID 1, R16G16Float. Preserve negative and tiled coordinates.
    // GPU texture binding and sampler addressing remain separate work.
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

// Observed descriptor filename = geometry cooked name + literal ".ndb".
bool u2_native_TextureImageMatchesReference(const U2NativeTextureReferencePreview &reference,
                                           const U2NativeTexturePreview &image);

// Host preview association by cooked resource filename, not an engine shader binding.
// Missing/ambiguous names fail without changing any prior image indices.
bool u2_native_BindGeometryTexturePreviews(std::vector<U2NativeMeshPreview> *meshes,
                                         const std::vector<U2NativeTexturePreview> &images);
