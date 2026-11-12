#pragma once
#include <SDL3/SDL.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include "ndlib/io/package_manager.h"

namespace u2::standalone {
// Static reconstruction of the captured title screen. Original logo pixels;
// reference-derived layout and temporary SDL debug font, no menu state/actions.
class TitlePreview {
public:
    TitlePreview()=default;
    TitlePreview(const TitlePreview &)=delete;
    TitlePreview &operator=(const TitlePreview &)=delete;
    ~TitlePreview() { release_texture(); }
    void release_texture() { if(m_logo) SDL_DestroyTexture(m_logo);m_logo=nullptr; }
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
        std::printf("[U2 Native Title] original logo BC1+BC4 decoded=%ux%u; static reference layout; temporary font; no menu transition\n",image.width,image.height);
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
        auto text=[&](float x,float y,float scale,const char *value) {
            float sx=1,sy=1;SDL_GetRenderScale(renderer,&sx,&sy);
            Uint8 r=0,g=0,b=0,a=0;SDL_GetRenderDrawColor(renderer,&r,&g,&b,&a);
            SDL_SetRenderScale(renderer,scale,scale);
            SDL_SetRenderDrawColor(renderer,0,0,0,220);
            SDL_RenderDebugText(renderer,x/scale+1,y/scale+1,value);
            SDL_SetRenderDrawColor(renderer,r,g,b,a);
            SDL_RenderDebugText(renderer,x/scale,y/scale,value);
            SDL_SetRenderScale(renderer,sx,sy);
        };
        auto centered=[&](float y,float scale,const char *value) {
            text((1920-float(std::strlen(value))*8*scale)*0.5f,y,scale,value);
        };
        // Prompt text and PS cross symbol observed in the French reference.
        // Static display only: neither this prompt nor Enter starts a game.
        constexpr float scale=2.5f,y=710;
        constexpr float left=(1920-(28*8*scale+40))*0.5f;
        SDL_SetRenderDrawColor(renderer,210,207,185,255);
        text(left,y,scale,"Appuyer sur ");
        text(left+12*8*scale+40,y,scale," pour commencer.");
        const float cx=left+12*8*scale+20,cy=y+9;
        SDL_SetRenderDrawColor(renderer,125,145,180,255);
        for(int i=0;i<48;++i) {
            const float a=float(i)*6.283185307f/48,b=float(i+1)*6.283185307f/48;
            SDL_RenderLine(renderer,cx+18*std::cos(a),cy+18*std::sin(a),cx+18*std::cos(b),cy+18*std::sin(b));
        }
        SDL_RenderLine(renderer,cx-9,cy-9,cx+9,cy+9);SDL_RenderLine(renderer,cx+9,cy-9,cx-9,cy+9);
        SDL_SetRenderDrawColor(renderer,205,205,200,255);
        centered(872,1.6f,"Uncharted(TM) 2: Among Thieves Remastered (C) 2009-2015 Sony Computer Entertainment America LLC. Published by Sony Computer");
        centered(896,1.6f,"Entertainment Europe. Created and developed by Naughty Dog, Inc. \"Uncharted\" is a trademark of Sony Computer Entertainment Europe. All");
        centered(920,1.6f,"rights reserved.");
        SDL_SetRenderDrawColor(renderer,8,12,19,220);
        const SDL_FRect status{0,1024,1920,56};SDL_RenderFillRect(renderer,&status);
        SDL_SetRenderDrawColor(renderer,215,215,215,255);
        centered(1044,1,"APERCU NATIF - ECRAN TITRE : transition non portee   |   F1 : decor seul   |   ECHAP : quitter");
    }
private:
    SDL_Texture *m_logo{nullptr};
};
} // namespace u2::standalone
