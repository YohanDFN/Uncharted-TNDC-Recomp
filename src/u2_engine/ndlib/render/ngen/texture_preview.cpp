#include "texture_preview.h"
#include <array>
#include <new>
#include <utility>

bool u2_native_DecodeThinBc1Preview(U2NativeTexturePixels *out,const uint8_t *data,
                                    size_t bytes,uint32_t width,uint32_t height,bool srgb) {
    // Scope/budget of the viewer, not a limit of the original texture format.
    if(!out || !data || !width || !height || width>4096 || height>4096) return false;
    const size_t columns=(width+3)/4,rows=(height+3)/4;
    const size_t pitch=(columns+7)&~size_t{7},padded_rows=(rows+7)&~size_t{7};
    if(bytes<pitch*padded_rows*8) return false;
    auto u16=[](const uint8_t *p) { return uint16_t(p[0]|(uint16_t(p[1])<<8)); };
    auto color565=[](uint16_t c) {
        const unsigned r=(c>>11)&31,g=(c>>5)&63,b=c&31;
        return std::array<unsigned,4>{(r<<3)|(r>>2),(g<<2)|(g>>4),(b<<3)|(b>>2),255};
    };
    try {
        U2NativeTexturePixels result;
        result.width=width;result.height=height;result.srgb=srgb;
        result.rgba.resize(size_t(width)*height);
        for(size_t y=0;y<rows;++y) for(size_t x=0;x<columns;++x) {
            // Thin microtile: interleave the low three block-coordinate bits.
            // Matches ComputePixelIndexWithinMicroTile in tiling.comp, mode 1.
            size_t morton=0;
            for(unsigned bit=0;bit<3;++bit) {
                morton|=((x>>bit)&1)<<(2*bit);
                morton|=((y>>bit)&1)<<(2*bit+1);
            }
            const size_t block=((y/8)*(pitch/8)+x/8)*64+morton;
            const auto *source=data+block*8;
            const uint16_t c0=u16(source),c1=u16(source+2);
            std::array<std::array<unsigned,4>,4> colors{};
            colors[0]=color565(c0);colors[1]=color565(c1);
            for(size_t channel=0;channel<3;++channel) {
                if(c0>c1) {
                    colors[2][channel]=(2*colors[0][channel]+colors[1][channel])/3;
                    colors[3][channel]=(colors[0][channel]+2*colors[1][channel])/3;
                } else colors[2][channel]=(colors[0][channel]+colors[1][channel])/2;
            }
            colors[2][3]=255;colors[3][3]=c0>c1 ? 255 : 0;
            for(unsigned by=0;by<4;++by) for(unsigned bx=0;bx<4;++bx) {
                const size_t px=x*4+bx,py=y*4+by;
                if(px>=width || py>=height) continue;
                const auto &c=colors[(source[4+by]>>(2*bx))&3];
                result.rgba[py*width+px]=uint32_t(c[0])|(uint32_t(c[1])<<8)|(uint32_t(c[2])<<16)|(uint32_t(c[3])<<24);
            }
        }
        *out=std::move(result);
        return true;
    } catch(const std::bad_alloc &) { return false; }
}
