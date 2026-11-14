#include "font_preview.h"
#include "u2_engine.h"
#include <algorithm>
#include <charconv>
#include <cmath>
#include <cstring>
#include <new>
#include <string>

namespace {
using Fields=std::map<std::string,std::string>;
bool fields(std::string_view line,Fields &out) {
    while(!line.empty()) {
        const size_t begin=line.find_first_not_of(" \t\r");
        if(begin==std::string_view::npos) return true;
        line.remove_prefix(begin);
        const size_t equals=line.find('=');
        if(equals==std::string_view::npos || !equals || line.substr(0,equals).find_first_of(" \t")!=std::string_view::npos) return false;
        const std::string key(line.substr(0,equals));line.remove_prefix(equals+1);
        size_t end=0;std::string value;
        if(!line.empty() && line.front()=='"') {
            line.remove_prefix(1);end=line.find('"');
            if(end==std::string_view::npos) return false;
            value=line.substr(0,end);line.remove_prefix(end+1);
            if(!line.empty() && line.front()!=' ' && line.front()!='\t' && line.front()!='\r') return false;
        } else {
            end=line.find_first_of(" \t\r");
            if(end==std::string_view::npos) end=line.size();
            if(!end) return false;
            value=line.substr(0,end);line.remove_prefix(end);
        }
        if(!out.emplace(key,value).second) return false;
    }
    return true;
}
bool number(const Fields &f,const char *key,int &value) {
    const auto it=f.find(key);if(it==f.end()) return false;
    const auto &s=it->second;
    const auto parsed=std::from_chars(s.data(),s.data()+s.size(),value);
    return parsed.ec==std::errc{} && parsed.ptr==s.data()+s.size();
}
bool codepoint(int id) { return id>=0 && id<=0x10FFFF && !(id>=0xD800 && id<=0xDFFF); }
bool next_utf8(std::string_view text,size_t &at,uint32_t &value) {
    const unsigned first=static_cast<unsigned char>(text[at++]);
    if(first<0x80) { value=first;return true; }
    unsigned count=0;uint32_t minimum=0;
    if(first>=0xC2 && first<=0xDF) { count=1;value=first&31;minimum=0x80; }
    else if(first>=0xE0 && first<=0xEF) { count=2;value=first&15;minimum=0x800; }
    else if(first>=0xF0 && first<=0xF4) { count=3;value=first&7;minimum=0x10000; }
    else return false;
    if(count>text.size()-at) return false;
    for(unsigned i=0;i<count;++i) {
        const unsigned c=static_cast<unsigned char>(text[at++]);
        if((c&0xC0)!=0x80) return false;
        value=(value<<6)|(c&63);
    }
    return value>=minimum && value<=0x10FFFF && !(value>=0xD800 && value<=0xDFFF);
}
}

bool u2_native_ReadMainFontPreview(U2NativeFontPreview *out,std::string_view description,
                                   const uint8_t *tga,size_t bytes) {
    if(!out || description.empty() || description.size()>256*1024 || !tga || bytes<18 || bytes>4*1024*1024) return false;
    if(tga[0]!=0 || tga[1]!=0 || tga[2]!=2 || tga[16]!=32 || (tga[17]&0xF0)!=0x20) return false;
    const int width=int(tga[12])|(int(tga[13])<<8),height=int(tga[14])|(int(tga[15])<<8);
    if(!width || !height || width>2048 || height>2048 || size_t(width)*height>1024*1024 ||
       size_t(width)*height>(bytes-18)/4) return false;
    try {
        U2NativeFontPreview font;
        bool common=false,page=false,chars=false,kernings=false;
        int declared_chars=0,declared_pairs=0;
        while(!description.empty()) {
            const size_t newline=description.find('\n');
            std::string_view line=description.substr(0,newline);
            description.remove_prefix(newline==std::string_view::npos ? description.size() : newline+1);
            const size_t start=line.find_first_not_of(" \t\r");
            if(start==std::string_view::npos) continue;
            line.remove_prefix(start);
            const size_t split=line.find_first_of(" \t\r");
            const auto kind=line.substr(0,split);
            if(split==std::string_view::npos) return false;
            Fields f;if(!fields(line.substr(split),f)) return false;
            if(kind=="info") continue;
            if(kind=="common") {
                int pages=0,packed=0;
                if(common || !number(f,"lineHeight",font.line_height) || !number(f,"base",font.baseline) ||
                   !number(f,"scaleW",font.atlas_width) || !number(f,"scaleH",font.atlas_height) ||
                   !number(f,"pages",pages) || !number(f,"packed",packed) || pages!=1 || packed!=1 ||
                   font.line_height<=0 || font.line_height>4096 || font.baseline<0 || font.baseline>font.line_height ||
                   font.atlas_width!=width || font.atlas_height!=height) return false;
                common=true;
            } else if(kind=="page") {
                int id=0;const auto file=f.find("file");
                if(page || !number(f,"id",id) || id!=0 || file==f.end() || file->second!="main_00.tga") return false;
                page=true;
            } else if(kind=="chars") {
                if(chars || !number(f,"count",declared_chars) || declared_chars<=0 || declared_chars>1024) return false;
                chars=true;
            } else if(kind=="char") {
                int id=0,pi=0,channel=0;U2NativeFontGlyph g;
                if(font.glyphs.size()>=1024 || !number(f,"id",id) || !codepoint(id) || !number(f,"x",g.x) || !number(f,"y",g.y) ||
                   !number(f,"width",g.width) || !number(f,"height",g.height) || !number(f,"xoffset",g.x_offset) ||
                   !number(f,"yoffset",g.y_offset) || !number(f,"xadvance",g.advance) || !number(f,"page",pi) || !number(f,"chnl",channel) ||
                   pi!=0 || (channel!=1 && channel!=2 && channel!=4 && channel!=8) ||
                   g.x<0 || g.y<0 || g.width<0 || g.height<0 || g.x>width || g.y>height ||
                   g.width>width-g.x || g.height>height-g.y || g.x_offset < -4096 || g.x_offset>4096 ||
                   g.y_offset < -4096 || g.y_offset>4096 || g.advance<0 || g.advance>4096) return false;
                g.channel=unsigned(channel);
                if(!font.glyphs.emplace(uint32_t(id),g).second) return false;
            } else if(kind=="kernings") {
                if(kernings || !number(f,"count",declared_pairs) || declared_pairs<0 || declared_pairs>16384) return false;
                kernings=true;
            } else if(kind=="kerning") {
                int first=0,second=0,amount=0;
                if(font.kernings.size()>=16384 || !number(f,"first",first) || !number(f,"second",second) || !number(f,"amount",amount) ||
                   !codepoint(first) || !codepoint(second) || amount < -4096 || amount>4096 ||
                   !font.kernings.emplace((uint64_t(first)<<32)|uint32_t(second),amount).second) return false;
            } else return false;
        }
        if(!common || !page || !chars || font.glyphs.size()!=size_t(declared_chars) ||
           font.kernings.size()!=size_t(declared_pairs)) return false;
        for(const auto &[pair,amount] : font.kernings) {
            (void)amount;
            if(!font.glyphs.contains(uint32_t(pair>>32)) || !font.glyphs.contains(uint32_t(pair))) return false;
        }
        font.atlas.width=uint32_t(width);font.atlas.height=uint32_t(height*4);
        font.atlas.rgba.resize(size_t(width)*height*4);
        for(size_t plane=0;plane<4;++plane) for(size_t pixel=0;pixel<size_t(width)*height;++pixel)
            font.atlas.rgba[plane*size_t(width)*height+pixel]=0xFFFFFFu|(uint32_t(tga[18+pixel*4+plane])<<24);
        *out=std::move(font);return true;
    } catch(const std::bad_alloc &) { return false; }
}

bool u2_native_LoadMainFontPreview(U2NativeFontPreview *out) {
    if(!out) return false;
    try {
        auto read=[](const char *path,size_t budget,std::vector<uint8_t> &bytes) {
            const int fd=int(runtime_file_open(path,0,0));if(fd<0) return false;
            struct Close { int fd;~Close() { runtime_file_close(fd); } } close{fd};
            const int64_t size=runtime_file_lseek(fd,0,2);
            if(size<=0 || uint64_t(size)>budget) return false;
            bytes.resize(size_t(size));
            return runtime_file_pread(fd,bytes.data(),bytes.size(),0)==size;
        };
        std::vector<uint8_t> description,tga;
        if(!read("/app0/u2data/data/fonts/ps4/main.fnt",256*1024,description) ||
           !read("/app0/u2data/data/fonts/ps4/main_00.tga",4*1024*1024,tga)) return false;
        return u2_native_ReadMainFontPreview(out,{reinterpret_cast<const char*>(description.data()),description.size()},tga.data(),tga.size());
    } catch(const std::bad_alloc &) { return false; }
}

bool u2_native_LayoutFontText(const U2NativeFontPreview &font,std::string_view text,
                              float pixel_height,U2NativeFontText *out) {
    if(!out || font.line_height<=0 || font.line_height>4096 || text.size()>8192 ||
       !std::isfinite(pixel_height) || pixel_height<=0 || pixel_height>512) return false;
    try {
        U2NativeFontText result;result.quads.reserve(text.size());
        const float scale=pixel_height/float(font.line_height);
        bool has_previous=false;uint32_t previous=0;
        for(size_t at=0;at<text.size();) {
            uint32_t id=0;if(!next_utf8(text,at,id)) return false;
            const auto it=font.glyphs.find(id);if(it==font.glyphs.end()) return false;
            if(has_previous) {
                const auto k=font.kernings.find((uint64_t(previous)<<32)|id);
                if(k!=font.kernings.end()) result.advance+=k->second*scale;
            }
            const auto &g=it->second;
            int plane=0;
            if(g.channel==2) plane=1;else if(g.channel==4) plane=2;else if(g.channel==8) plane=3;
            else if(g.channel!=1) return false;
            if(g.width && g.height) result.quads.push_back({result.advance+g.x_offset*scale,g.y_offset*scale,
                g.width*scale,g.height*scale,g.x,g.y+plane*font.atlas_height,g.width,g.height});
            result.advance+=g.advance*scale;previous=id;has_previous=true;
        }
        *out=std::move(result);return true;
    } catch(const std::bad_alloc &) { return false; }
}
