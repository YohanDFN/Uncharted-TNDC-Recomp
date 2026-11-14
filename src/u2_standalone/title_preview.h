#pragma once
#include <SDL3/SDL.h>
#include <cstdio>
#include <new>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include "bitmap_font_preview.h"
#include "ndlib/io/package_manager.h"
#include "ndlib/render/ngen/menu_preview.h"

namespace u2::standalone {
// Host preview of verified page lists, not an execution of the guest menu.
// Panel positions, colors and the two-branch presentation are viewer conventions.
class TitlePreview {
public:
    TitlePreview()=default;
    TitlePreview(const TitlePreview &)=delete;
    TitlePreview &operator=(const TitlePreview &)=delete;
    ~TitlePreview() { release_texture(); }
    void release_texture() {
        if(m_logo) SDL_DestroyTexture(m_logo);
        m_logo=nullptr;m_font.release_texture();m_text.clear();
    }
    bool ready() const { return m_logo!=nullptr && !m_text.empty(); }
    bool load(SDL_Renderer *renderer) {
        release_texture();
        try {
            U2NativeTexturePixels image;
            U2NativePostStartMenuPreview menu;
            if(!renderer || !u2_cpp_PackageMgr_LoadNativeTitleLogoPreview(&image) ||
               !m_font.load(renderer) || !u2_native_LoadPostStartMenuPreview(&menu)) {
                release_texture();return false;
            }
            m_logo=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,int(image.width),int(image.height));
            if(!m_logo || !SDL_UpdateTexture(m_logo,nullptr,image.rgba.data(),int(image.width*4)) ||
               !SDL_SetTextureBlendMode(m_logo,SDL_BLENDMODE_BLEND) || !SDL_SetTextureScaleMode(m_logo,SDL_SCALEMODE_LINEAR)) {
                release_texture();return false;
            }
            const SDL_Color gold{225,204,153,255}, white{244,244,239,255};
            const SDL_Color muted{183,192,204,255}, blue{204,219,235,255};
            auto add=[&](std::string_view value,float x,float width,float y,float height,SDL_Color color) {
                Text item;
                if(!u2_native_LayoutFontText(m_font.data(),value,height,&item.layout)) return false;
                // Fit long legal lines or localized labels within their column.
                if(item.layout.advance>width &&
                   !u2_native_LayoutFontText(m_font.data(),value,height*width/item.layout.advance,&item.layout)) return false;
                item.x=x+(width-item.layout.advance)*0.5f;item.y=y;item.color=color;
                m_text.push_back(std::move(item));return true;
            };
            bool ok=add("Apres X (PAGE_ROOT_MAIN 0x37656388) - listes verifiees : menu.bin + 3.common",180,1560,408,22,muted) &&
                add("PAGE_PROFILE (0x82D5C00B) | avec profil / save",200,700,460,20,gold) &&
                add(menu.profile.title,200,700,488,28,white) &&
                add("PAGE_NEWPROFILE (0x11FEFB39) | sans profil",1020,700,460,20,gold) &&
                add("Sans titre de page",1020,700,490,22,muted);
            // The verified lists occupy seven and four rows, including the spacer.
            ok=ok && menu.profile.controls.size()==7 && menu.new_profile.controls.size()==4;
            for(size_t i=0;ok && i<menu.profile.controls.size();++i) {
                const auto &control=menu.profile.controls[i];
                if(!control.is_spacer()) ok=add(control.label,200,700,532+float(i)*34,30,i==0 ? SDL_Color{255,238,198,255} : white);
            }
            for(size_t i=0;ok && i<menu.new_profile.controls.size();++i) {
                const auto &control=menu.new_profile.controls[i];
                if(!control.is_spacer()) ok=add(control.label,1020,700,532+float(i)*36,30,white);
            }
            const std::string navigation="[X] "+menu.select_label+"   |   [O] "+menu.back_label+"   |   "+menu.title_menu_label;
            ok=ok && add(navigation,180,1560,812,22,blue);
            for(size_t i=0;ok && i<3;++i) ok=add(k_legal[i],30,1860,870+float(i)*34,30,{205,205,200,255});
            ok=ok && add("Apercu post-X verifie (menu.bin + 3.common + main.fnt) | transitions non portees | F1 : decor seul | ECHAP : quitter",
                         30,1860,1043,18,{215,215,215,255});
            if(!ok) { release_texture();return false; }
            std::printf("[U2 Native Menu] menu.bin+3.common verified: %s (0x%08X, title='%s', controls=%zu) & %s (0x%08X, controls=%zu)\n",
                menu.profile.name.c_str(),menu.profile.id,menu.profile.title.c_str(),menu.profile.controls.size(),
                menu.new_profile.name.c_str(),menu.new_profile.id,menu.new_profile.controls.size());
            return true;
        } catch(const std::bad_alloc &) { release_texture();return false; }
    }
    void draw(SDL_Renderer *renderer) const {
        if(!ready()) return;
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer,0,0,0,65);
        const SDL_FRect shade{0,0,1920,1080};SDL_RenderFillRect(renderer,&shade);
        const SDL_FRect logo{260,44,1400,350};
        SDL_RenderTextureRotated(renderer,m_logo,nullptr,&logo,0,nullptr,SDL_FLIP_VERTICAL);
        SDL_SetRenderDrawColor(renderer,8,12,19,205);
        const SDL_FRect left{180,446,740,342}, right{1000,446,740,342};
        SDL_RenderFillRect(renderer,&left);SDL_RenderFillRect(renderer,&right);
        SDL_SetRenderDrawColor(renderer,8,12,19,220);
        const SDL_FRect status{0,1024,1920,56};SDL_RenderFillRect(renderer,&status);
        for(const auto &item : m_text) {
            m_font.draw(renderer,item.layout,item.x+1,item.y+1,{0,0,0,200});
            m_font.draw(renderer,item.layout,item.x,item.y,item.color);
        }
    }
private:
    struct Text { U2NativeFontText layout; float x{0},y{0}; SDL_Color color{255,255,255,255}; };
    SDL_Texture *m_logo{nullptr};
    BitmapFontPreview m_font;
    std::vector<Text> m_text;
    inline static constexpr const char *k_legal[]={
        "Uncharted™ 2: Among Thieves Remastered © 2009-2015 Sony Computer Entertainment America LLC. Published by Sony Computer",
        "Entertainment Europe. Created and developed by Naughty Dog, Inc. \"Uncharted\" is a trademark of Sony Computer Entertainment Europe. All",
        "rights reserved."};
};
} // namespace u2::standalone
