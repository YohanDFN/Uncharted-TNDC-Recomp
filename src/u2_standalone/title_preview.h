#pragma once
#include <SDL3/SDL.h>
#include <algorithm>
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
// Host state and verified page data; placement, colors and hit rectangles are viewer conventions.
class TitlePreview {
public:
    TitlePreview()=default;
    TitlePreview(const TitlePreview &)=delete;
    TitlePreview &operator=(const TitlePreview &)=delete;
    ~TitlePreview() { release_texture(); }
    void release_texture() {
        if(m_logo) SDL_DestroyTexture(m_logo);
        m_logo=nullptr;m_font.release_texture();m_text.clear();m_hits.clear();m_layout_ready=false;
        m_state=U2NativeMenuStateMachine{};
    }
    bool ready() const { return m_logo && m_layout_ready; }
    bool load(SDL_Renderer *renderer) {
        release_texture();
        try {
            U2NativeTexturePixels image;
            if(!renderer || !u2_cpp_PackageMgr_LoadNativeTitleLogoPreview(&image) ||
               !m_font.load(renderer) || !u2_native_LoadPostStartMenuPreview(&m_menu) ||
               !u2_native_DetectSavedataStatus(&m_savedata) || !m_state.init(&m_menu,m_savedata)) {
                release_texture();return false;
            }
            m_logo=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,int(image.width),int(image.height));
            if(!m_logo || !SDL_UpdateTexture(m_logo,nullptr,image.rgba.data(),int(image.width*4)) ||
               !SDL_SetTextureBlendMode(m_logo,SDL_BLENDMODE_BLEND) || !SDL_SetTextureScaleMode(m_logo,SDL_SCALEMODE_LINEAR) ||
               !rebuild_layout()) {
                release_texture();return false;
            }
            std::printf("[native-title] state-machine initialized: page=%s (0x%08X) depth=%zu profile=%d campaign=%d\n",
                current_page()->name.c_str(),current_page()->id,stack_depth(),int(m_savedata.has_profile),int(m_savedata.has_campaign_save));
            return true;
        } catch(const std::bad_alloc &) { release_texture();return false; }
    }
    bool move_cursor(int delta) {
        const bool moved=m_state.move_cursor(delta);
        if(moved) rebuild_layout();
        return moved;
    }
    U2NativeMenuTransitionResult press_select() {
        auto result=m_state.press_select();rebuild_layout();return result;
    }
    U2NativeMenuTransitionResult press_back() {
        auto result=m_state.press_back();rebuild_layout();return result;
    }
    size_t stack_depth() const { return m_state.stack_depth(); }
    const U2NativeMenuPagePreview *current_page() const { return m_state.current_page(); }
    size_t current_cursor() const { return m_state.current_cursor(); }
    const U2NativeMenuStateMachine &state_machine() const { return m_state; }
    bool hover_at(float x,float y,int w,int h) {
        if(!resize_layout(w,h)) return false;
        for(const auto &hit : m_hits) {
            if(x>=hit.rect.x && x<hit.rect.x+hit.rect.w && y>=hit.rect.y && y<hit.rect.y+hit.rect.h) {
                const size_t index=hit.index;
                if(index==current_cursor()) return true;
                if(!m_state.set_cursor(index)) return false;
                rebuild_layout();return true;
            }
        }
        return false;
    }
    bool click_at(float x,float y,int w,int h) {
        if(!hover_at(x,y,w,h)) return false;
        return press_select().handled;
    }
    void draw(SDL_Renderer *renderer) {
        if(!m_logo) return;
        int w=0,h=0;
        if(!SDL_GetCurrentRenderOutputSize(renderer,&w,&h) || !resize_layout(w,h)) return;
        const float sx=float(w)/1920.0f,sy=float(h)/1080.0f;
        SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer,0,0,0,65);
        const SDL_FRect shade{0,0,float(w),float(h)};SDL_RenderFillRect(renderer,&shade);
        const SDL_FRect logo{380*sx,90*sy,1160*sx,290*sy};
        SDL_RenderTextureRotated(renderer,m_logo,nullptr,&logo,0,nullptr,SDL_FLIP_VERTICAL);
        SDL_SetRenderDrawColor(renderer,8,12,19,215);
        const SDL_FRect panel{360*sx,400*sy,1200*sx,440*sy};SDL_RenderFillRect(renderer,&panel);
        for(const auto &hit : m_hits) if(hit.index==current_cursor()) {
            SDL_SetRenderDrawColor(renderer,106,88,53,180);SDL_RenderFillRect(renderer,&hit.rect);
        }
        SDL_SetRenderDrawColor(renderer,8,12,19,220);
        const SDL_FRect bar{0,1000*sy,float(w),80*sy};SDL_RenderFillRect(renderer,&bar);
        for(const auto &item : m_text) {
            m_font.draw(renderer,item.layout,item.x+sx,item.y+sy,{0,0,0,200});
            m_font.draw(renderer,item.layout,item.x,item.y,item.color);
        }
    }
private:
    struct Text { U2NativeFontText layout; float x{0},y{0}; SDL_Color color{255,255,255,255}; };
    struct Hit { SDL_FRect rect; size_t index; };
    // Keep UTF-8 glyphs supported by the actual atlas. ASCII apostrophes are absent.
    std::string font_text(std::string_view value) const {
        std::string result;
        for(size_t at=0;at<value.size();) {
            const size_t start=at;
            const uint32_t first=static_cast<unsigned char>(value[at++]);
            uint32_t code=first;
            size_t count=0;
            if(first>=0xC2 && first<=0xDF) { code=first&31;count=1; }
            else if(first>=0xE0 && first<=0xEF) { code=first&15;count=2; }
            else if(first>=0xF0 && first<=0xF4) { code=first&7;count=3; }
            else if(first>=0x80) { result+=' ';continue; }
            if(count>value.size()-at) { result+=' ';break; }
            bool valid=true;
            for(size_t i=0;i<count;++i) {
                const auto next=static_cast<unsigned char>(value[at++]);
                if((next&0xC0)!=0x80) valid=false;
                code=(code<<6)|(next&63);
            }
            if(valid && code!=39 && m_font.data().glyphs.count(code)) result.append(value.substr(start,at-start));
            else result+=' ';
        }
        return result;
    }
    bool resize_layout(int w,int h) {
        if(w<=0 || h<=0) return false;
        if(w==m_width && h==m_height && m_layout_ready) return true;
        m_width=w;m_height=h;return rebuild_layout();
    }
    bool rebuild_layout() {
        m_text.clear();m_hits.clear();m_layout_ready=false;
        const auto *page=current_page();
        if(!page || m_width<=0 || m_height<=0) return false;
        const float sx=float(m_width)/1920.0f,sy=float(m_height)/1080.0f;
        const SDL_Color gold{225,204,153,255},white{244,244,239,255},muted{183,192,204,255};
        auto add=[&](std::string_view value,float x,float width,float y,float height,SDL_Color color) {
            Text item;
            const std::string safe=font_text(value);
            if(!u2_native_LayoutFontText(m_font.data(),safe,height*sy,&item.layout)) return false;
            if(item.layout.advance>width*sx &&
               !u2_native_LayoutFontText(m_font.data(),safe,height*sy*width*sx/item.layout.advance,&item.layout)) return false;
            item.x=x*sx+(width*sx-item.layout.advance)*0.5f;item.y=y*sy;item.color=color;
            m_text.push_back(std::move(item));return true;
        };
        std::string trail;
        for(const auto &entry : m_state.stack()) { if(!trail.empty()) trail+=" > ";trail+=entry.page->name; }
        bool ok=add(trail,80,1760,18,24,gold);
        const std::string save=std::string(m_savedata.has_profile ? "Profil detecte" : "Aucun profil")+" | "+
            (m_savedata.has_campaign_save ? "Sauvegarde active" : "Nouvelle partie");
        ok=ok && add(save,80,1760,54,22,muted) &&
            add(page->title.empty() ? "UNCHARTED 2" : page->title,400,1120,420,36,gold);
        const float row=page->controls.empty() ? 42.0f : (std::min)(46.0f,330.0f/float(page->controls.size()));
        float y=480;
        bool selectable=false;
        for(size_t i=0;i<page->controls.size();++i) {
            const auto &ctrl=page->controls[i];
            if(ctrl.is_spacer()) { y+=row;continue; }
            if(ctrl.is_selectable()) {
                selectable=true;
                const std::string label=m_state.display_control_label(ctrl);
                ok=ok && add(i==current_cursor() ? "> "+label+" <" : label,420,1080,y,row*0.72f,white);
                m_hits.push_back({{400*sx,(y-3)*sy,1120*sx,row*sy},i});
            } else if(!ctrl.label.empty()) ok=ok && add(ctrl.label,420,1080,y,row*0.65f,muted);
            y+=row;
        }
        if(!selectable) ok=ok && add("Liste dynamique non portee",420,1080,810,24,muted);
        if(!m_state.last_status_note().empty()) ok=ok && add(m_state.last_status_note(),100,1720,868,26,gold);
        std::string navigation="[ENTREE/X] "+m_menu.select_label;
        if(stack_depth()>1) navigation+="   [RET.ARR/O] "+m_menu.back_label;
        navigation+="   [F1] Masquer menu";
        ok=ok && add(navigation,60,1800,1024,26,white);
        m_layout_ready=ok;return ok;
    }
    SDL_Texture *m_logo{nullptr};
    BitmapFontPreview m_font;
    U2NativePostStartMenuPreview m_menu;
    U2NativeSavedataStatus m_savedata;
    U2NativeMenuStateMachine m_state;
    std::vector<Text> m_text;
    std::vector<Hit> m_hits;
    int m_width{1920},m_height{1080};
    bool m_layout_ready{false};
};
} // namespace u2::standalone
