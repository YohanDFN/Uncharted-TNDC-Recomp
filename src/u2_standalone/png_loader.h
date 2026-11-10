// Uncharted 2: Among Thieves — Minimal PNG Decoder for Standalone Splash Screen
// Implements RFC 2083 minimal streaming PNG decode with miniz uncompress.
#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include "../gpu/third_party/miniz/miniz.h"

namespace u2::standalone {

struct DecodedImage {
    int32_t width{0};
    int32_t height{0};
    int32_t channels{4}; // RGBA
    std::vector<uint8_t> pixels; // 32-bit RGBA pixels
};

inline uint32_t read_be32(const uint8_t *p) noexcept {
    return (static_cast<uint32_t>(p[0]) << 24) |
           (static_cast<uint32_t>(p[1]) << 16) |
           (static_cast<uint32_t>(p[2]) << 8)  |
           (static_cast<uint32_t>(p[3]));
}

inline uint8_t paeth_predictor(int a, int b, int c) noexcept {
    int p = a + b - c;
    int pa = std::abs(p - a);
    int pb = std::abs(p - b);
    int pc = std::abs(p - c);
    if (pa <= pb && pa <= pc) return static_cast<uint8_t>(a);
    if (pb <= pc) return static_cast<uint8_t>(b);
    return static_cast<uint8_t>(c);
}

inline bool load_png(const std::string &filepath, DecodedImage &out) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) return false;

    std::streamsize size = file.tellg();
    if (size < 32) return false;
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> data(size);
    if (!file.read(reinterpret_cast<char *>(data.data()), size)) return false;

    // Check PNG signature: 89 50 4E 47 0D 0A 1A 0A
    static const uint8_t png_sig[8] = { 0x89, 'P', 'N', 'G', 0x0D, 0x0A, 0x1A, 0x0A };
    if (std::memcmp(data.data(), png_sig, 8) != 0) return false;

    size_t offset = 8;
    int32_t width = 0, height = 0;
    uint8_t bit_depth = 0, color_type = 0;
    std::vector<uint8_t> idat_data;

    while (offset + 8 <= data.size()) {
        uint32_t chunk_len = read_be32(data.data() + offset);
        const uint8_t *chunk_type = data.data() + offset + 4;
        offset += 8;

        if (offset + chunk_len > data.size()) break;

        if (std::memcmp(chunk_type, "IHDR", 4) == 0 && chunk_len >= 13) {
            width = read_be32(data.data() + offset);
            height = read_be32(data.data() + offset + 4);
            bit_depth = data[offset + 8];
            color_type = data[offset + 9];
        } else if (std::memcmp(chunk_type, "IDAT", 4) == 0) {
            idat_data.insert(idat_data.end(), data.data() + offset, data.data() + offset + chunk_len);
        } else if (std::memcmp(chunk_type, "IEND", 4) == 0) {
            break;
        }

        offset += chunk_len + 4; // Skip data + CRC
    }

    if (width <= 0 || height <= 0 || bit_depth != 8 || (color_type != 2 && color_type != 6) || idat_data.empty()) {
        return false;
    }

    int src_bpp = (color_type == 6) ? 4 : 3;
    size_t raw_line_size = 1 + static_cast<size_t>(width) * src_bpp;
    size_t raw_size = raw_line_size * height;
    std::vector<uint8_t> raw_decomp(raw_size);

    mz_ulong uncomp_len = static_cast<mz_ulong>(raw_size);
    int status = uncompress(raw_decomp.data(), &uncomp_len, idat_data.data(), static_cast<mz_ulong>(idat_data.size()));
    if (status != Z_OK || uncomp_len != raw_size) {
        return false;
    }

    out.width = width;
    out.height = height;
    out.channels = 4;
    out.pixels.resize(static_cast<size_t>(width) * height * 4);

    std::vector<uint8_t> prev_line(width * src_bpp, 0);
    std::vector<uint8_t> curr_line(width * src_bpp, 0);

    for (int y = 0; y < height; ++y) {
        const uint8_t *line_src = raw_decomp.data() + y * raw_line_size;
        uint8_t filter = line_src[0];
        const uint8_t *src_pixels = line_src + 1;

        for (int x = 0; x < width * src_bpp; ++x) {
            uint8_t raw = src_pixels[x];
            uint8_t a = (x >= src_bpp) ? curr_line[x - src_bpp] : 0;
            uint8_t b = prev_line[x];
            uint8_t c = (x >= src_bpp) ? prev_line[x - src_bpp] : 0;

            uint8_t val = 0;
            switch (filter) {
            case 0: val = raw; break;
            case 1: val = static_cast<uint8_t>(raw + a); break;
            case 2: val = static_cast<uint8_t>(raw + b); break;
            case 3: val = static_cast<uint8_t>(raw + ((a + b) / 2)); break;
            case 4: val = static_cast<uint8_t>(raw + paeth_predictor(a, b, c)); break;
            default: val = raw; break;
            }
            curr_line[x] = val;
        }

        uint8_t *dst_line = out.pixels.data() + static_cast<size_t>(y) * width * 4;
        for (int x = 0; x < width; ++x) {
            if (src_bpp == 4) {
                dst_line[x * 4 + 0] = curr_line[x * 4 + 0];
                dst_line[x * 4 + 1] = curr_line[x * 4 + 1];
                dst_line[x * 4 + 2] = curr_line[x * 4 + 2];
                dst_line[x * 4 + 3] = curr_line[x * 4 + 3];
            } else {
                dst_line[x * 4 + 0] = curr_line[x * 3 + 0];
                dst_line[x * 4 + 1] = curr_line[x * 3 + 1];
                dst_line[x * 4 + 2] = curr_line[x * 3 + 2];
                dst_line[x * 4 + 3] = 255;
            }
        }
        prev_line = curr_line;
    }

    return true;
}

} // namespace u2::standalone
