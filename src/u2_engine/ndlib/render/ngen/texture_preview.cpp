#include "texture_preview.h"
#include <array>
#include <algorithm>
#include <bit>
#include <cstring>
#include <new>
#include <utility>

bool u2_native_ReadTextureTablePreview(std::vector<U2NativeTexturePreview> *out,
    const uint8_t *page,size_t size,size_t metadata,const uint8_t *vram,size_t vram_size) {
    if(!out || !page || !vram || metadata>size || 0x40>size-metadata || vram_size>16*1024*1024) return false;
    const auto u32=[](const uint8_t *p) { return uint32_t(p[0])|(uint32_t(p[1])<<8)|(uint32_t(p[2])<<16)|(uint32_t(p[3])<<24); };
    const auto u64=[](const uint8_t *p) { uint64_t n=0;for(unsigned i=0;i<8;++i) n|=uint64_t(p[i])<<(8*i);return n; };
    const uint64_t type=u64(page+metadata+8);
    constexpr char expected[]="TEXTURE_TABLE";
    if(type>size || sizeof(expected)>size-type || std::memcmp(page+type,expected,sizeof(expected))) return false;
    const auto range=[&](uint64_t address,size_t bytes,size_t &offset) {
        const auto base=reinterpret_cast<uintptr_t>(page);
        if(address<base || address-base>size || bytes>size-(address-base)) return false;
        offset=size_t(address-base);return true;
    };
    size_t string_budget=size;
    const auto text=[&](uint64_t address,std::string &out_text) {
        size_t at=0;if(!range(address,1,at)) return false;
        const auto *end=static_cast<const uint8_t *>(std::memchr(page+at,0,size-at));
        if(!end || size_t(end-page-at)+1>string_budget) return false;
        string_budget-=size_t(end-page-at)+1;
        out_text.assign(reinterpret_cast<const char *>(page+at),end-page-at);return true;
    };
    // Original TEXTURE_TABLE login at 48EC43..48EC5C: count, uint32 UIDs,
    // resource-name pointers, descriptor pointers. Descriptors are 64 bytes.
    const uint32_t count=u32(page+metadata+0x20);
    size_t keys=0,names=0,descriptors=0;
    if(count>INT32_MAX || (count &&
        (!range(u64(page+metadata+0x28),0,keys) || count>(size-keys)/4 ||
         !range(u64(page+metadata+0x30),0,names) || count>(size-names)/8 ||
         !range(u64(page+metadata+0x38),0,descriptors) || count>(size-descriptors)/8))) return false;
    try {
        std::vector<U2NativeTexturePreview> result;
        result.reserve(count);
        size_t compressed_budget=vram_size,decoded_budget=16*1024*1024;
        for(size_t i=0;i<count;++i) {
            size_t descriptor=0;
            if(!range(u64(page+descriptors+i*8),64,descriptor)) return false;
            const auto *d=page+descriptor;
            U2NativeTexturePreview image;
            image.uid=u32(page+keys+i*4);
            if(!image.uid || image.uid==UINT32_MAX || image.uid!=u32(d+0x14)) return false;
            image.vram_offset=u64(d+8);
            const uint32_t bytes=u32(d+0x10);
            image.layers=u32(d+0x18);image.format=u32(d+0x1C);image.flags=u32(d+0x20);
            image.mip_count=u32(d+0x24);image.width=u32(d+0x28);image.height=u32(d+0x2C);image.layout_tag=u32(d+0x34);
            if(!bytes || bytes>compressed_budget || image.vram_offset>vram_size || bytes>vram_size-image.vram_offset ||
               !image.width || !image.height || !image.layers || !image.mip_count ||
               image.mip_count>static_cast<uint32_t>(std::bit_width(std::max(image.width,image.height))) ||
               !text(u64(page+names+i*8),image.resource_name) || !text(u64(d+0x38),image.cooked_name)) return false;
            compressed_budget-=bytes;
            image.resident_blocks.assign(vram+image.vram_offset,vram+image.vram_offset+bytes);
            // Deliberately scoped to the observed small resident BC1 layout.
            // Large tag-3 images and other formats remain compressed snapshots.
            const bool bc1=(image.format==71 && image.flags==0x01000000) ||
                           (image.format==72 && image.flags==0x010F0000);
            if(bc1 && image.layers==1 && image.layout_tag==2 && image.width<=64 && image.height<=64 &&
               std::has_single_bit(image.width) && std::has_single_bit(image.height)) {
                size_t required=0;
                for(uint32_t mip=0;mip<image.mip_count;++mip) {
                    const size_t w=std::max(1u,image.width>>mip),h=std::max(1u,image.height>>mip);
                    required+=(((w+3)/4+7)&~size_t{7})*(((h+3)/4+7)&~size_t{7})*8;
                }
                const size_t pixels=size_t(image.width)*image.height;
                if(required!=bytes || pixels>decoded_budget ||
                   !u2_native_DecodeThinBc1Preview(&image.pixels,image.resident_blocks.data(),bytes,
                        image.width,image.height,image.format==72)) return false;
                decoded_budget-=pixels;
            }
            result.push_back(std::move(image));
        }
        *out=std::move(result);return true;
    } catch(const std::bad_alloc &) { return false; }
}

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
