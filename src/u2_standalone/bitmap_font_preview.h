#pragma once
#include <SDL3/SDL.h>
#include <cstdio>
#include "ndlib/render/ngen/font_preview.h"

namespace u2::standalone {
class BitmapFontPreview {
public:
    BitmapFontPreview()=default;
    BitmapFontPreview(const BitmapFontPreview &)=delete;
    BitmapFontPreview &operator=(const BitmapFontPreview &)=delete;
    ~BitmapFontPreview() { release_texture(); }
    void release_texture() {
        if(m_texture) SDL_DestroyTexture(m_texture);
        m_texture=nullptr;m_font={};
    }
    const U2NativeFontPreview &data() const { return m_font; }
    bool load(SDL_Renderer *renderer) {
        release_texture();
        U2NativeFontPreview font;
        if(!renderer || !u2_native_LoadMainFontPreview(&font)) {
            std::printf("[U2 Native Font] main.fnt or main_00.tga unavailable/unsupported\n");return false;
        }
        m_texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,int(font.atlas.width),int(font.atlas.height));
        if(!m_texture || !SDL_UpdateTexture(m_texture,nullptr,font.atlas.rgba.data(),int(font.atlas.width*4)) ||
           !SDL_SetTextureBlendMode(m_texture,SDL_BLENDMODE_BLEND) || !SDL_SetTextureScaleMode(m_texture,SDL_SCALEMODE_LINEAR)) {
            std::printf("[U2 Native Font] atlas upload failed: %s\n",SDL_GetError());release_texture();return false;
        }
        m_font=std::move(font);
        std::printf("[U2 Native Font] main.fnt glyphs=%zu kernings=%zu line-height=%d base=%d atlas=%dx%d packed B/G/R/A (proven slot 1 menu/UI font)\n",
            m_font.glyphs.size(),m_font.kernings.size(),m_font.line_height,m_font.baseline,m_font.atlas_width,m_font.atlas_height);
        return true;
    }
    void draw(SDL_Renderer *renderer,const U2NativeFontText &text,float x,float y,SDL_Color color) const {
        if(!m_texture) return;
        SDL_SetTextureColorMod(m_texture,color.r,color.g,color.b);SDL_SetTextureAlphaMod(m_texture,color.a);
        for(const auto &q : text.quads) {
            const SDL_FRect source{float(q.source_x),float(q.source_y),float(q.source_width),float(q.source_height)};
            const SDL_FRect destination{x+q.x,y+q.y,q.width,q.height};
            SDL_RenderTexture(renderer,m_texture,&source,&destination);
        }
    }
private:
    U2NativeFontPreview m_font;
    SDL_Texture *m_texture{nullptr};
};
} // namespace u2::standalone
