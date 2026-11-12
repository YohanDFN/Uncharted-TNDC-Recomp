#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct U2NativeTexturePixels {
    uint32_t width{0},height{0};
    bool srgb{false}; // Preserve encoded RGB values; no lighting/color-space conversion.
    std::vector<uint32_t> rgba; // R in low byte, A in high byte.
};

struct U2NativeTexturePreview {
    uint32_t uid{0},format{0},mip_count{0},width{0},height{0};
    uint32_t layers{0},layout_tag{0},flags{0};
    uint64_t vram_offset{0};
    std::string resource_name,cooked_name;
    std::vector<uint8_t> resident_blocks;
    U2NativeTexturePixels pixels; // Empty for formats/layouts not yet decoded.
};

// Owned snapshots from relocated CPU page pointers and a bounded VRAM extent.
// Metadata name/type offsets remain page-relative, as in the package reader.
bool u2_native_ReadTextureTablePreview(std::vector<U2NativeTexturePreview> *out,
    const uint8_t *page,size_t page_size,size_t metadata,
    const uint8_t *vram,size_t vram_size);

// First mip only: BC1 RGBA, Thin1DThin, single 2D layer/sample, block pitch
// rounded up to 8. Does not parse packages, create GPU images or run PS4 code.
bool u2_native_DecodeThinBc1Preview(U2NativeTexturePixels *out,const uint8_t *data,
                                    size_t bytes,uint32_t width,uint32_t height,bool srgb);
