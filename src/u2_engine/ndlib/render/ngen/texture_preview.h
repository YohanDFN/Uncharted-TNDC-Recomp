#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>

struct U2NativeTexturePixels {
    uint32_t width{0},height{0};
    bool srgb{false}; // Preserve encoded RGB values; no lighting/color-space conversion.
    std::vector<uint32_t> rgba; // R in low byte, A in high byte.
};

// First mip only: BC1 RGBA, Thin1DThin, single 2D layer/sample, block pitch
// rounded up to 8. Does not parse packages, create GPU images or run PS4 code.
bool u2_native_DecodeThinBc1Preview(U2NativeTexturePixels *out,const uint8_t *data,
                                    size_t bytes,uint32_t width,uint32_t height,bool srgb);
