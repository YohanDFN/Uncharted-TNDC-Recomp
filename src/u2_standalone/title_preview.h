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
    U2NativeMenuStateMachine &state_machine() { return m_state; }
    const U2NativeMenuStateMachine &state_machine() const { return m_state; }
    void reset_to_root() { m_state.reset_to_root();rebuild_layout(); }
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
        const SDL_FRect logo{250*sx,115*sy,1420*sx,355*sy};
        SDL_RenderTextureRotated(renderer,m_logo,nullptr,&logo,0,nullptr,SDL_FLIP_VERTICAL);
        for(const auto &item : m_text)
            m_font.draw_with_shadow(renderer,item.layout,item.x,item.y,item.color,item.shadow_offset);
    }
private:
    struct Text { U2NativeFontText layout; float x{0},y{0}; SDL_Color color{255,255,255,255}; float shadow_offset{2.0f}; };
    struct Hit { SDL_FRect rect; size_t index; };
    // Keep supported UTF-8 glyphs and controller tokens from the original strings.
    std::string font_text(std::string_view value) const {
        std::string result;
        for(size_t at=0;at<value.size();) {
            if(value[at]=='|') {
                const size_t close=value.find('|',at+1);
                if(close!=std::string_view::npos && close-at<=32) {
                    const auto token=value.substr(at+1,close-at-1);
                    if(token=="ENTER" || token=="X" || token=="C" || token=="BACK" || token=="S" || token=="T") {
                        if(m_font.symbol_data()) result.append(value.substr(at,close-at+1));
                        else if(token=="ENTER") result+="[ENTREE]";
                        else if(token=="C" || token=="BACK") result+="[O]";
                        else if(token=="X") result+="[X]";
                        else if(token=="S") result+="[S]";
                        else result+="[T]";
                        at=close+1;continue;
                    }
                }
            }
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
            if(valid && code==39 && m_font.data().glyphs.count(0x2019)) result+="\xE2\x80\x99";
            else if(valid && m_font.data().glyphs.count(code)) result.append(value.substr(start,at-start));
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
        const SDL_Color gold{240,200,40,255},muted{165,170,180,205};
        auto layout=[&](std::string_view value,float height,U2NativeFontText *out) {
            return u2_native_LayoutFontTextWithSymbols(m_font.data(),m_font.symbol_data(),font_text(value),height*sy,out);
        };
        auto add=[&](std::string_view value,float x,float width,float y,float height,SDL_Color color,float shadow) {
            Text item;
            if(!layout(value,height,&item.layout)) return false;
            if(item.layout.advance>width*sx &&
               !layout(value,height*width*sx/item.layout.advance,&item.layout)) return false;
            item.x=x*sx+(width*sx-item.layout.advance)*0.5f;item.y=y*sy;item.color=color;item.shadow_offset=shadow*sy;
            m_text.push_back(std::move(item));return true;
        };
        auto hit=[&](size_t index,float y,float height) {
            const auto &item=m_text.back();
            const float width=(std::max)(720*sx,item.layout.advance+40*sx);
            m_hits.push_back({{(float(m_width)-width)*0.5f,y*sy,width,height*sy},index});
        };
        bool ok=true;
        if(page->id==0x37656388u) {
            if(!page->controls.empty()) {
                ok=add(m_state.display_control_label(page->controls[0]),80,1760,665,32,{236,236,236,255},2);
                if(ok) hit(0,660,44);
            }
            std::string line;
            float y=830;
            for(size_t at=0;at<page->subtitle.size();) {
                while(at<page->subtitle.size() && page->subtitle[at]==' ') ++at;
                if(at==page->subtitle.size()) break;
                const size_t end=page->subtitle.find(' ',at);
                const size_t next=end==std::string::npos ? page->subtitle.size() : end;
                const std::string word=page->subtitle.substr(at,next-at);
                const std::string candidate=line.empty() ? word : line+" "+word;
                U2NativeFontText measured;
                if(!layout(candidate,22.5f,&measured)) { ok=false;break; }
                if(!line.empty() && measured.advance>1660*sx) {
                    ok=ok && add(line,130,1660,y,22.5f,{218,218,218,238},1.8f);y+=30;line=word;
                } else line=candidate;
                at=next;
            }
            if(!line.empty()) ok=ok && add(line,130,1660,y,22.5f,{218,218,218,238},1.8f);
        } else {
            if(!page->title.empty()) ok=add(page->title,80,1760,490,32,{230,218,175,248},2);
            float y=page->layout_y>0 ? 1080.0f*(0.17f+float(page->layout_y)*0.01f) : 540.0f;
            const float row=page->controls.empty() ? 44.0f : (std::min)(46.0f,340.0f/float(page->controls.size()));
            bool selectable=false;
            for(size_t i=0;i<page->controls.size();++i) {
                const auto &ctrl=page->controls[i];
                if(ctrl.is_spacer()) { y+=row;continue; }
                if(ctrl.is_selectable()) {
                    selectable=true;
                    const bool selected=i==current_cursor();
                    const bool added=add(m_state.display_control_label(ctrl),80,1760,y,selected ? 31.5f : 29.0f,
                        selected ? gold : SDL_Color{236,236,232,238},selected ? 2.5f : 2.0f);
                    ok=ok && added;
                    if(added) hit(i,y-3,row);
                } else if(!ctrl.label.empty()) ok=ok && add(ctrl.label,80,1760,y,25,muted,2);
                y+=row;
            }
            if(!selectable) ok=ok && add("Liste dynamique non portee",80,1760,820,25,muted,2);
            if(!m_state.last_status_note().empty()) ok=ok && add(m_state.last_status_note(),80,1760,900,22,{240,200,40,235},2);
            if(!page->hides_standard_prompts()) {
                const std::string select="|X| "+m_menu.select_label,back="|C| "+m_menu.back_label;
                U2NativeFontText select_text,back_text;
                if(!layout(select,25.5f,&select_text) || (m_state.can_go_back() && !layout(back,25.5f,&back_text))) ok=false;
                else {
                    float right=1760*sx;
                    auto prompt=[&](U2NativeFontText text) {
                        Text item;item.x=right-text.advance;item.y=975*sy;item.color={235,235,235,245};item.shadow_offset=2*sy;
                        right=item.x-42*sx;item.layout=std::move(text);m_text.push_back(std::move(item));
                    };
                    if(m_state.can_go_back()) prompt(std::move(back_text));
                    prompt(std::move(select_text));
                }
            }
        }
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
