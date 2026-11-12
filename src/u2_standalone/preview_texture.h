#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include "ndlib/render/ngen/texture_preview.h"

namespace u2::standalone {
// Viewer convention: normalized repeat, bilinear first resident mip only.
// Does not reproduce the original anisotropic/trilinear sampler or material.
inline bool preview_texture_valid(const U2NativeTexturePixels &image) {
    return image.width && image.height && image.width<=4096 && image.height<=4096 &&
        image.rgba.size()==size_t(image.width)*image.height;
}
inline float preview_srgb_decode(float encoded) {
    return encoded<=0.04045f ? encoded/12.92f : std::pow((encoded+0.055f)/1.055f,2.4f);
}
inline float preview_srgb_encode(float linear) {
    return linear<=0.0031308f ? linear*12.92f : 1.055f*std::pow(linear,1/2.4f)-0.055f;
}
inline bool preview_sample_repeat(const U2NativeTexturePixels &image,float u,float v,float brightness,uint32_t *out) {
    if(!out || !preview_texture_valid(image) || !std::isfinite(u) || !std::isfinite(v) ||
       !std::isfinite(brightness) || brightness<0 || brightness>1) return false;
    // Reduce in double before conversion to bounded texel coordinates, including
    // large finite and negative UVs. Texel centers are (i+0.5)/dimension.
    const double x=(double(u)-std::floor(double(u)))*image.width-0.5;
    const double y=(double(v)-std::floor(double(v)))*image.height-0.5;
    const int ix=int(std::floor(x)),iy=int(std::floor(y));
    const float tx=float(x-ix),ty=float(y-iy);
    auto texel=[&](int px,int py) {
        px=(px+int(image.width))%int(image.width);py=(py+int(image.height))%int(image.height);
        return image.rgba[size_t(py)*image.width+px];
    };
    const std::array<uint32_t,4> colors{texel(ix,iy),texel(ix+1,iy),texel(ix,iy+1),texel(ix+1,iy+1)};
    uint32_t result=0;
    for(unsigned shift : {0u,8u,16u,24u}) {
        std::array<float,4> c;
        for(size_t i=0;i<4;++i) {
            c[i]=float((colors[i]>>shift)&255)/255;
            if(image.srgb && shift!=24) c[i]=preview_srgb_decode(c[i]);
        }
        float value=(c[0]*(1-tx)+c[1]*tx)*(1-ty)+(c[2]*(1-tx)+c[3]*tx)*ty;
        if(shift!=24) {
            value*=brightness;
            if(image.srgb) value=preview_srgb_encode(value);
        }
        result|=uint32_t(std::lround(std::clamp(value,0.0f,1.0f)*255))<<shift;
    }
    *out=result;return true;
}
} // namespace u2::standalone
