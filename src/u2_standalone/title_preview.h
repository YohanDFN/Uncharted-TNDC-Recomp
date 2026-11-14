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
// Host state and verified menu anchors/styles; text fitting and hits are viewer conventions.
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
        const float sx=float(m_width)/1280.0f,sy=float(m_height)/720.0f;
        auto project=[&](const U2NativeMenuRect &rect) {
            return SDL_FRect{rect.x*sx,rect.y*sy,rect.w*sx,rect.h*sy};
        };
        if(m_page_layout.show_logo) {
            const auto logo=project(m_page_layout.logo_rect);
            SDL_RenderTextureRotated(renderer,m_logo,nullptr,&logo,0,nullptr,SDL_FLIP_VERTICAL);
        }
        if(m_page_layout.show_box) {
            SDL_BlendMode previous_blend;
            SDL_Color previous_color;
            if(!SDL_GetRenderDrawBlendMode(renderer,&previous_blend) ||
               !SDL_GetRenderDrawColor(renderer,&previous_color.r,&previous_color.g,&previous_color.b,&previous_color.a)) return;
            SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
            const auto box=project(m_page_layout.page_box_rect);
            // DrawBox (0x005B8710): black panel with vertical alpha fades.
            const float levels[]{box.y-m_page_layout.box_fade_top*sy,box.y,
                                 box.y+box.h,box.y+box.h+m_page_layout.box_fade_bottom*sy};
            const float alphas[]{0.0f,m_page_layout.box_alpha,m_page_layout.box_alpha,0.0f};
            SDL_Vertex vertices[8]{};
            for(int level=0;level<4;++level) {
                vertices[2*level]={{box.x,levels[level]},SDL_FColor{0,0,0,alphas[level]},{0,0}};
                vertices[2*level+1]={{box.x+box.w,levels[level]},SDL_FColor{0,0,0,alphas[level]},{0,0}};
            }
            const int indices[]{0,1,2,1,3,2,2,3,4,3,5,4,4,5,6,5,7,6};
            SDL_RenderGeometry(renderer,nullptr,vertices,8,indices,18);
            if(m_page_layout.has_title) {
                SDL_SetRenderDrawColor(renderer,255,255,255,90);
                const auto title=project(m_page_layout.title_rect);
                SDL_RenderLine(renderer,title.x,title.y+title.h,title.x+title.w,title.y+title.h);
            }
            SDL_SetRenderDrawColor(renderer,previous_color.r,previous_color.g,previous_color.b,previous_color.a);
            SDL_SetRenderDrawBlendMode(renderer,previous_blend);
        }
        for(const auto &item : m_text) {
            m_font.draw(renderer,item.layout,item.x+item.shadow_offset*sx,item.y+item.shadow_offset*sy,
                {0,0,0,item.color.a},true);
            m_font.draw(renderer,item.layout,item.x,item.y,item.color);
        }
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
        if(!page || m_width<=0 || m_height<=0 || !u2_native_ComputeMenuPageLayout(*page,&m_page_layout)) return false;
        const float sx=float(m_width)/1280.0f,sy=float(m_height)/720.0f;
        const auto &geometry=m_page_layout;
        const auto &content=geometry.content_rect;
        const SDL_Color gold{240,200,40,255},muted{165,170,180,205};
        auto layout=[&](std::string_view value,float height,U2NativeFontText *out) {
            return u2_native_LayoutFontTextWithSymbols(m_font.data(),m_font.symbol_data(),font_text(value),height,out);
        };
        auto project_text=[&](U2NativeFontText &text) {
            text.advance*=sx;
            for(auto &quad : text.quads) {
                quad.x*=sx;quad.width*=sx;quad.y*=sy;quad.height*=sy;
            }
        };
        auto add=[&](std::string_view value,const U2NativeMenuRect &rect,float height,SDL_Color color,
                     float shadow_offset=-1.0f) {
            Text item;
            if(shadow_offset<0.0f) shadow_offset=geometry.shadow_offset;
            height=std::min(height,rect.h-2*shadow_offset);
            if(!layout(value,height,&item.layout)) return false;
            if(item.layout.advance>rect.w-2*shadow_offset) {
                height*=(rect.w-2*shadow_offset)/item.layout.advance;
                if(!layout(value,height,&item.layout)) return false;
            }
            item.x=(rect.x+(rect.w-item.layout.advance)*0.5f)*sx;
            item.y=(rect.y+(rect.h-height)*0.5f)*sy;
            item.color=color;item.shadow_offset=shadow_offset;
            project_text(item.layout);m_text.push_back(std::move(item));return true;
        };
        bool ok=true,selectable=false;
        if(geometry.has_title) ok=add(page->title,geometry.title_rect,
            std::min(geometry.font_height*geometry.title_scale,geometry.title_rect.h),{255,255,255,255});
        for(size_t i=0;i<page->controls.size();++i) {
            const auto &ctrl=page->controls[i];
            if(ctrl.is_spacer() || ctrl.label.empty()) continue;
            const U2NativeMenuRect row{content.x,content.y+float(i)*geometry.row_step,content.w,geometry.row_height};
            const bool selected=i==current_cursor();
            const bool added=add(m_state.display_control_label(ctrl),row,
                selected ? geometry.selected_font_height : geometry.font_height,
                ctrl.is_selectable() ? (selected ? gold : SDL_Color{236,236,232,238}) : muted,
                geometry.control_shadow_offset);
            ok=ok && added;
            if(ctrl.is_selectable()) {
                selectable=true;
                if(added) m_hits.push_back({{row.x*sx,row.y*sy,row.w*sx,row.h*sy},i});
            }
        }
        if(geometry.show_logo) {
            // Wrap with actual font metrics; anchor the legal block above y=642 in 720p.
            const float copyright_height=geometry.font_height*geometry.copyright_scale;
            const float copyright_shadow=geometry.shadow_offset*(copyright_height/42.0f);
            const float step=copyright_height+4.0f;
            std::vector<std::string> lines;
            std::string line;
            for(size_t at=0;at<page->subtitle.size();) {
                while(at<page->subtitle.size() && page->subtitle[at]==' ') ++at;
                if(at==page->subtitle.size()) break;
                const size_t end=page->subtitle.find(' ',at);
                const size_t next=end==std::string::npos ? page->subtitle.size() : end;
                const std::string word=page->subtitle.substr(at,next-at);
                const std::string candidate=line.empty() ? word : line+" "+word;
                U2NativeFontText measured;
                if(!layout(candidate,copyright_height,&measured)) { ok=false;break; }
                if(!line.empty() && measured.advance>content.w-2*copyright_shadow) {
                    lines.push_back(std::move(line));line=word;
                } else line=candidate;
                at=next;
            }
            if(!line.empty()) lines.push_back(std::move(line));
            const float y_start=geometry.copyright_bottom_y-float(lines.size())*step;
            for(size_t i=0;i<lines.size();++i)
                ok=add(lines[i],{content.x,y_start+float(i)*step,content.w,step},
                       copyright_height,{255,255,255,238},copyright_shadow) && ok;
        } else {
            const float note_y=content.y+content.h+12.0f;
            if(!selectable) ok=add("Liste dynamique non portee",{content.x,note_y,content.w,20},16,muted) && ok;
            if(!m_state.last_status_note().empty())
                ok=add(m_state.last_status_note(),{content.x,note_y+24,content.w,20},15,{240,200,40,235}) && ok;
        }
        if(geometry.show_prompts) {
            const std::string select="|X| "+m_menu.select_label,back="|C| "+m_menu.back_label;
            U2NativeFontText select_text,back_text;
            auto prompts=[&](float height) {
                return layout(select,height,&select_text) && (!m_state.can_go_back() || layout(back,height,&back_text));
            };
            if(!prompts(17)) ok=false;
            else {
                const float gap=m_state.can_go_back() ? 28.0f : 0.0f;
                const float width=select_text.advance+back_text.advance+gap;
                const float available=geometry.page_box_rect.w;
                if(width>available && !prompts(17*std::max(0.0f,available-gap)/(width-gap))) ok=false;
                else {
                    float right=geometry.prompts_right_x;
                    auto prompt=[&](U2NativeFontText text) {
                        Text item;item.x=(right-text.advance)*sx;item.y=geometry.prompts_y*sy;
                        right-=text.advance+gap;item.color={235,235,235,245};item.shadow_offset=geometry.shadow_offset;
                        project_text(text);item.layout=std::move(text);m_text.push_back(std::move(item));
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
    U2NativeMenuPageLayout m_page_layout;
    U2NativeSavedataStatus m_savedata;
    U2NativeMenuStateMachine m_state;
    std::vector<Text> m_text;
    std::vector<Hit> m_hits;
    int m_width{1920},m_height{1080};
    bool m_layout_ready{false};
};
} // namespace u2::standalone
