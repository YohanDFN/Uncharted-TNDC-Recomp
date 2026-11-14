#pragma once
#include <SDL3/SDL.h>
#include <array>
#include <cmath>
#include <cstdio>
#include "bitmap_font_preview.h"
#include "ndlib/io/package_manager.h"

namespace u2::standalone {
// Static reconstruction of the captured title screen. Original logo pixels and proven
// slot 1 Latin menu/UI font main.fnt (0x003F6D0C / 0x01450D18); reference-derived
// layout, geometric cross icon (instead of symbol3.fnt), and status bar; no menu actions.
class TitlePreview {
public:
    TitlePreview()=default;
    TitlePreview(const TitlePreview &)=delete;
    TitlePreview &operator=(const TitlePreview &)=delete;
    ~TitlePreview() { release_texture(); }
    void release_texture() { if(m_logo) SDL_DestroyTexture(m_logo);m_logo=nullptr;m_font.release_texture(); }
    bool ready() const { return m_logo!=nullptr; }
    bool load(SDL_Renderer *renderer) {
        release_texture();
        U2NativeTexturePixels image;
        if(!renderer || !u2_cpp_PackageMgr_LoadNativeTitleLogoPreview(&image)) return false;
        m_logo=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,int(image.width),int(image.height));
        if(!m_logo || !SDL_UpdateTexture(m_logo,nullptr,image.rgba.data(),int(image.width*4)) ||
           !SDL_SetTextureBlendMode(m_logo,SDL_BLENDMODE_BLEND) || !SDL_SetTextureScaleMode(m_logo,SDL_SCALEMODE_LINEAR)) {
            std::printf("[U2 Native Title] logo upload failed: %s\n",SDL_GetError());release_texture();return false;
        }
        if(!m_font.load(renderer)) { release_texture();return false; }
        for(size_t i=0;i<m_text.size();++i)
            if(!u2_native_LayoutFontText(m_font.data(),k_text[i],k_heights[i],&m_text[i])) {
                std::printf("[U2 Native Title] main.fnt cannot lay out text %zu\n",i);release_texture();return false;
            }
        std::printf("[U2 Native Title] original logo BC1+BC4 decoded=%ux%u; main.fnt (proven slot 1 menu font); static reference layout; no menu transition\n",image.width,image.height);
        return true;
    }
    void draw(SDL_Renderer *renderer) const {
        if(!ready()) return;
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer,0,0,0,65);
        const SDL_FRect shade{0,0,1920,1080};SDL_RenderFillRect(renderer,&shade);
        const SDL_FRect logo{160,140,1600,400};
        // The stored image is vertically inverted relative to the reference.
        SDL_RenderTextureRotated(renderer,m_logo,nullptr,&logo,0,nullptr,SDL_FLIP_VERTICAL);
        auto text=[&](size_t index,float x,float y) {
            Uint8 r=0,g=0,b=0,a=0;SDL_GetRenderDrawColor(renderer,&r,&g,&b,&a);
            m_font.draw(renderer,m_text[index],x+2,y+2,{0,0,0,220});
            m_font.draw(renderer,m_text[index],x,y,{r,g,b,a});
        };
        auto centered=[&](size_t index,float y) {
            text(index,(1920-m_text[index].advance)*0.5f,y);
        };
        // Prompt text and PS cross symbol observed in the French reference.
        // Static display only: neither this prompt nor Enter starts a game.
        constexpr float y=710;
        const float left=(1920-(m_text[0].advance+40+m_text[1].advance))*0.5f;
        SDL_SetRenderDrawColor(renderer,210,207,185,255);
        text(0,left,y);
        text(1,left+m_text[0].advance+40,y);
        const float cx=left+m_text[0].advance+20;
        const float cy=y+m_font.data().baseline*(k_heights[0]/m_font.data().line_height)-14;
        SDL_SetRenderDrawColor(renderer,125,145,180,255);
        for(int i=0;i<48;++i) {
            const float a=float(i)*6.283185307f/48,b=float(i+1)*6.283185307f/48;
            SDL_RenderLine(renderer,cx+18*std::cos(a),cy+18*std::sin(a),cx+18*std::cos(b),cy+18*std::sin(b));
        }
        SDL_RenderLine(renderer,cx-9,cy-9,cx+9,cy+9);SDL_RenderLine(renderer,cx+9,cy-9,cx-9,cy+9);
        SDL_SetRenderDrawColor(renderer,205,205,200,255);
        centered(2,870);centered(3,904);centered(4,938);
        SDL_SetRenderDrawColor(renderer,8,12,19,220);
        const SDL_FRect status{0,1024,1920,56};SDL_RenderFillRect(renderer,&status);
        SDL_SetRenderDrawColor(renderer,215,215,215,255);
        centered(5,1040);
    }
private:
    SDL_Texture *m_logo{nullptr};
    BitmapFontPreview m_font;
    std::array<U2NativeFontText,6> m_text;
    inline static constexpr std::array<float,6> k_heights{42,42,32,32,32,18};
    inline static constexpr const char *k_text[]={
        "Appuyer sur "," pour commencer.",
        "Uncharted™ 2: Among Thieves Remastered © 2009-2015 Sony Computer Entertainment America LLC. Published by Sony Computer",
        "Entertainment Europe. Created and developed by Naughty Dog, Inc. \"Uncharted\" is a trademark of Sony Computer Entertainment Europe. All",
        "rights reserved.",
        "APERCU NATIF - ECRAN TITRE : transition non portee   |   F1 : decor seul   |   ECHAP : quitter"};
};
} // namespace u2::standalone
