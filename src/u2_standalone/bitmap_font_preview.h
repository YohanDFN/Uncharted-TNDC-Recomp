#pragma once
#include <SDL3/SDL.h>
#include <cstdio>
#include <utility>
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
        if(m_symbol_texture) SDL_DestroyTexture(m_symbol_texture);
        m_texture=nullptr;m_font={};m_symbol_texture=nullptr;m_symbol_font={};
    }
    const U2NativeFontPreview &data() const { return m_font; }
    const U2NativeFontPreview *symbol_data() const { return m_symbol_texture ? &m_symbol_font : nullptr; }
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
        U2NativeFontPreview symbol_font;
        if(u2_native_LoadSymbolFontPreview(&symbol_font)) {
            m_symbol_texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,
                int(symbol_font.atlas.width),int(symbol_font.atlas.height));
            if(m_symbol_texture && SDL_UpdateTexture(m_symbol_texture,nullptr,symbol_font.atlas.rgba.data(),int(symbol_font.atlas.width*4)) &&
               SDL_SetTextureBlendMode(m_symbol_texture,SDL_BLENDMODE_BLEND) && SDL_SetTextureScaleMode(m_symbol_texture,SDL_SCALEMODE_LINEAR)) {
                m_symbol_font=std::move(symbol_font);
                std::printf("[U2 Native Font] symbol3.fnt glyphs=%zu line-height=%d base=%d atlas=%dx%d RGBA32 (proven slot 2 controller icon font)\n",
                    m_symbol_font.glyphs.size(),m_symbol_font.line_height,m_symbol_font.baseline,m_symbol_font.atlas_width,m_symbol_font.atlas_height);
            } else {
                std::printf("[U2 Native Font] symbol atlas upload failed: %s\n",SDL_GetError());
                if(m_symbol_texture) SDL_DestroyTexture(m_symbol_texture);
                m_symbol_texture=nullptr;
            }
        } else std::printf("[U2 Native Font] symbol3.fnt or symbol3_00.tga unavailable/unsupported; using text prompts\n");
        return true;
    }
    void draw(SDL_Renderer *renderer,const U2NativeFontText &text,float x,float y,SDL_Color color,bool force_color=false) const {
        for(const auto &q : text.quads) {
            SDL_Texture *tex=(q.atlas_index==1 && m_symbol_texture) ? m_symbol_texture : m_texture;
            if(!tex) continue;
            if(q.atlas_index==1 && q.preserve_color && !force_color) SDL_SetTextureColorMod(tex,255,255,255);
            else SDL_SetTextureColorMod(tex,color.r,color.g,color.b);
            SDL_SetTextureAlphaMod(tex,color.a);
            const SDL_FRect source{float(q.source_x),float(q.source_y),float(q.source_width),float(q.source_height)};
            const SDL_FRect destination{x+q.x,y+q.y,q.width,q.height};
            SDL_RenderTexture(renderer,tex,&source,&destination);
        }
    }
    void draw_with_shadow(SDL_Renderer *renderer,const U2NativeFontText &text,float x,float y,SDL_Color color,float shadow_offset=2.0f) const {
        draw(renderer,text,x+shadow_offset,y+shadow_offset,{0,0,0,Uint8((int(color.a)*195)/255)},true);
        draw(renderer,text,x,y,color);
    }
private:
    U2NativeFontPreview m_font,m_symbol_font;
    SDL_Texture *m_texture{nullptr},*m_symbol_texture{nullptr};
};
} // namespace u2::standalone
