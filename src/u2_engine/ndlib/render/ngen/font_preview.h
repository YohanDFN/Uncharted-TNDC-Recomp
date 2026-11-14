#pragma once
#include <cstdint>
#include <map>
#include <string_view>
#include <vector>
#include "texture_preview.h"

struct U2NativeFontGlyph {
    int x{0},y{0},width{0},height{0},x_offset{0},y_offset{0},advance{0};
    unsigned channel{0};
};
struct U2NativeFontPreview {
    int line_height{0},baseline{0},atlas_width{0},atlas_height{0};
    bool is_packed{true};
    std::map<uint32_t,U2NativeFontGlyph> glyphs;
    std::map<uint64_t,int> kernings;
    // Packed: four white RGBA mask planes (B, G, R, A); unpacked: color RGBA.
    U2NativeTexturePixels atlas;
};
struct U2NativeFontQuad {
    float x{0},y{0},width{0},height{0};
    int source_x{0},source_y{0},source_width{0},source_height{0};
    uint8_t atlas_index{0};
    bool preserve_color{false};
};
struct U2NativeFontText {
    float advance{0};
    std::vector<U2NativeFontQuad> quads;
};

// Observed BMFont text, one packed page, uncompressed top-origin BGRA32 TGA.
// All results are host-owned; failure preserves output. No guest font object.
bool u2_native_ReadMainFontPreview(U2NativeFontPreview *out,std::string_view description,
                                   const uint8_t *tga,size_t tga_bytes);
bool u2_native_LoadMainFontPreview(U2NativeFontPreview *out);
// Single UTF-8 line; missing glyphs and malformed UTF-8 fail explicitly.
// pixel_height scales the declared lineHeight, not each glyph's ink bounds.
bool u2_native_LayoutFontText(const U2NativeFontPreview &font,std::string_view text,
                              float pixel_height,U2NativeFontText *out);

bool u2_native_ReadSymbolFontPreview(U2NativeFontPreview *out,std::string_view description,
                                     const uint8_t *tga,size_t tga_bytes);
bool u2_native_LoadSymbolFontPreview(U2NativeFontPreview *out);
bool u2_native_LayoutFontTextWithSymbols(const U2NativeFontPreview &main_font,
                                         const U2NativeFontPreview *symbol_font,std::string_view text,
                                         float pixel_height,U2NativeFontText *out);
