#pragma once
#include <SDL3/SDL.h>
#include <array>
#include <cmath>
#include <cstdio>
#include <set>
#include <vector>
#include "preview_clip.h"
#include "preview_depth.h"
#include "preview_lighting.h"
#include "ndlib/io/package_manager.h"
#include "game/rendercamera.h"

namespace u2::standalone {

// Asset-geometry viewer, not the Level/instance/render pipeline. The captured
// reference lens is scoped to this preview, never used as an engine setting.
class TemplePreview {
    enum class Mode { Solid, Wireframe, Points };
    struct WireBatch { std::vector<SDL_Vertex> vertices;std::vector<int> indices; };
    static constexpr SDL_Color palette[]={{224,188,120,255},{120,189,235,255},{173,204,164,255},
        {236,154,121,255},{175,160,226,255},{117,209,196,255},{222,209,163,255}};
public:
    TemplePreview()=default;
    TemplePreview(const TemplePreview &)=delete;
    TemplePreview &operator=(const TemplePreview &)=delete;
    ~TemplePreview() { release_texture(); }

    void release_texture() {
        if(m_solid_texture) SDL_DestroyTexture(m_solid_texture);
        m_solid_texture=nullptr;
    }
    bool load() {
        const Uint64 started=SDL_GetTicks();
        U2NativeCameraDefinition camera;
        U2NativeLevelOffset offset;
        std::vector<U2NativeMeshPreview> meshes;
        if (!u2_cpp_PackageMgr_GetNativeCamera("menu-temple",0,&camera) ||
            !camera.is_start || !camera.orientation_calculated || !camera.position_translated ||
            !u2_cpp_PackageMgr_GetNativeLevelOffset("menu-temple",&offset) ||
            offset.translation != std::array<float,3>{} ||
            !u2_cpp_PackageMgr_GetNativeGeometryPreview("menu-temple",&meshes)) return false;
        std::array<float,16> projection;
        if (!u2_native_BuildCenteredPerspective({1080.0f/1920.0f,1.3032253980636597f,0.24f,5000},&projection)) return false;
        const auto &q=camera.orientation_xyzw;
        const float x=q[0],y=q[1],z=q[2],w=q[3];
        const std::array<std::array<float,3>,3> basis{{
            {1-2*(y*y+z*z),2*(x*y+z*w),2*(x*z-y*w)},
            {2*(x*y-z*w),1-2*(x*x+z*z),2*(y*z+x*w)},
            {2*(x*z+y*w),2*(y*z-x*w),1-2*(x*x+y*y)}}};
        // Light toward the camera, offset upward and left in its reference view.
        // This is a viewer preset, not a recovered light from the original game.
        std::array<float,3> toward_light{};
        for(size_t i=0;i<3;++i) toward_light[i]=-0.35f*basis[0][i]+0.6f*basis[1][i]-basis[2][i];
        std::vector<std::vector<SDL_FRect>> batches;
        std::vector<WireBatch> wire_batches;
        PreviewDepthImage solid;
        const bool solid_ready=solid.reset(1920,1080,0xFF130C08);
        size_t depth_writes=0,lit_vertices=0;
        size_t visible=0,total=0,visible_edges=0,triangles=0;
        for (const auto &mesh : meshes) {
            auto &points=batches.emplace_back();
            auto &wire=wire_batches.emplace_back();
            const auto tint=palette[(batches.size()-1)%std::size(palette)];
            const SDL_FColor color{tint.r/255.0f,tint.g/255.0f,tint.b/255.0f,1};
            std::vector<float> brightness(mesh.positions.size(),1);
            bool normals_valid=mesh.normals.size()==mesh.positions.size();
            if(normals_valid) for(size_t v=0;v<mesh.normals.size();++v) {
                const auto &n=mesh.normals[v];
                if(!preview_light_intensity({n[0],n[1],n[2]},toward_light,&brightness[v])) { normals_valid=false;break; }
            }
            if(normals_valid) lit_vertices+=brightness.size();
            else {
                std::fill(brightness.begin(),brightness.end(),1);
                std::printf("[U2 Native Preview] lighting unavailable for '%s': keeping flat colors\n",mesh.name.c_str());
            }
            std::vector<std::array<float,4>> clip_vertices;
            clip_vertices.reserve(mesh.positions.size());
            points.reserve(mesh.positions.size());
            total+=mesh.positions.size();
            for (const auto &position : mesh.positions) {
                std::array<float,3> delta,view{};
                for(size_t i=0;i<3;++i) delta[i]=position[i]-camera.translated_position[i];
                for(size_t i=0;i<3;++i)
                    for(size_t j=0;j<3;++j) view[i]+=basis[i][j]*delta[j];
                clip_vertices.push_back({projection[0]*view[0],projection[5]*view[1],
                    projection[10]*view[2]+projection[14],view[2]});
                if (!(view[2]>=0.24f && view[2]<=5000)) continue;
                const float nx=projection[0]*view[0]/view[2],ny=projection[5]*view[1]/view[2];
                if (!std::isfinite(nx) || !std::isfinite(ny) || nx < -1 || nx > 1 || ny < -1 || ny > 1) continue;
                points.push_back({(nx+1)*960-1.25f,(1-ny)*540-1.25f,2.5f,2.5f});
            }
            visible+=points.size();
            triangles+=mesh.triangles.size();
            if(solid_ready) {
                const uint32_t rgba=uint32_t(tint.r)|(uint32_t(tint.g)<<8)|(uint32_t(tint.b)<<16)|0xFF000000u;
                for(const auto &triangle : mesh.triangles)
                    depth_writes+=solid.triangle({clip_vertices[triangle[0]],clip_vertices[triangle[1]],clip_vertices[triangle[2]]},rgba,
                        {brightness[triangle[0]],brightness[triangle[1]],brightness[triangle[2]]});
            }
            std::set<uint32_t> edges;
            for(const auto &triangle : mesh.triangles) for(size_t corner=0;corner<3;++corner) {
                uint16_t lo=triangle[corner],hi=triangle[(corner+1)%3];
                if(lo>hi) std::swap(lo,hi);
                if(lo==hi || !edges.insert((uint32_t(lo)<<16)|hi).second) continue;
                auto a=clip_vertices[lo],b=clip_vertices[hi];
                if(!clip_preview_line(a,b)) continue;
                const float ax=(a[0]/a[3]+1)*960,ay=(1-a[1]/a[3])*540;
                const float bx=(b[0]/b[3]+1)*960,by=(1-b[1]/b[3])*540;
                const float dx=bx-ax,dy=by-ay,length=std::hypot(dx,dy);
                if(!(length>0.0001f) || !std::isfinite(length)) continue;
                const float nx=-dy/length*0.55f,ny=dx/length*0.55f;
                const int base=static_cast<int>(wire.vertices.size());
                wire.vertices.push_back({{ax+nx,ay+ny},color,{0,0}});
                wire.vertices.push_back({{ax-nx,ay-ny},color,{0,0}});
                wire.vertices.push_back({{bx-nx,by-ny},color,{0,0}});
                wire.vertices.push_back({{bx+nx,by+ny},color,{0,0}});
                wire.indices.insert(wire.indices.end(),{base,base+1,base+2,base,base+2,base+3});
                ++visible_edges;
            }
        }
        if (!visible) return false;
        m_batches=std::move(batches);m_visible=visible;m_total=total;
        m_wire_batches=std::move(wire_batches);m_visible_edges=visible_edges;m_triangles=triangles;
        m_covered_pixels=0;
        for(float value : solid.depth) if(std::isfinite(value)) ++m_covered_pixels;
        release_texture();m_solid_image=std::move(solid);
        m_mode=m_covered_pixels ? Mode::Solid : Mode::Wireframe;
        std::printf("[U2 Native Preview] temple meshes=%zu positions=%zu visible=%zu triangles=%zu visible-edges=%zu camera='%s' reference-view zoom=1.303225398 near=0.24 far=5000\n",
            meshes.size(),total,visible,triangles,visible_edges,camera.name.c_str());
        std::printf("[U2 Native Preview] solid covered-pixels=%zu depth-passing-samples=%zu cache-ms=%llu (CPU depth image; static reference view)\n",
            m_covered_pixels,depth_writes,static_cast<unsigned long long>(SDL_GetTicks()-started));
        std::printf("[U2 Native Preview] lighting normals=%zu/%zu ambient=0.22 diffuse=0.78 (viewer preset; Gouraud shading)\n",lit_vertices,total);
        return true;
    }
    bool ready() const { return m_visible!=0; }
    void toggle_mode() {
        if(m_mode==Mode::Solid) m_mode=Mode::Wireframe;
        else if(m_mode==Mode::Wireframe) m_mode=Mode::Points;
        else m_mode=m_covered_pixels ? Mode::Solid : Mode::Wireframe;
        std::printf("[U2 Native Preview] mode=%s\n",m_mode==Mode::Solid ? "solid" : m_mode==Mode::Wireframe ? "wireframe" : "points");
        std::fflush(stdout);
    }
    void draw(SDL_Renderer *renderer) {
        if (!ready()) return;
        SDL_SetRenderDrawColor(renderer,8,12,19,255);SDL_RenderClear(renderer);
        if(m_mode==Mode::Solid && !m_solid_texture) {
            m_solid_texture=SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STATIC,1920,1080);
            if(!m_solid_texture || !SDL_UpdateTexture(m_solid_texture,nullptr,m_solid_image.pixels.data(),1920*int(sizeof(uint32_t)))) {
                std::printf("[U2 Native Preview] solid texture upload failed: %s\n",SDL_GetError());
                release_texture();m_mode=Mode::Wireframe;
            } else {
                SDL_SetTextureBlendMode(m_solid_texture,SDL_BLENDMODE_NONE);
                std::printf("[U2 Native Preview] solid image uploaded to SDL texture\n");
            }
        }
        if(m_mode==Mode::Solid) SDL_RenderTexture(renderer,m_solid_texture,nullptr,nullptr);
        else for(size_t i=0;i<m_batches.size();++i) {
            if(m_mode==Mode::Wireframe) {
                const auto &wire=m_wire_batches[i];
                if(!wire.indices.empty()) SDL_RenderGeometry(renderer,nullptr,wire.vertices.data(),static_cast<int>(wire.vertices.size()),
                    wire.indices.data(),static_cast<int>(wire.indices.size()));
                continue;
            }
            const auto color=palette[i%std::size(palette)];
            SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
            if (!m_batches[i].empty()) SDL_RenderFillRects(renderer,m_batches[i].data(),static_cast<int>(m_batches[i].size()));
        }
        SDL_SetRenderDrawColor(renderer,8,12,19,230);
        const SDL_FRect bar{0,0,1920,56};SDL_RenderFillRect(renderer,&bar);
        SDL_SetRenderDrawColor(renderer,240,235,220,255);
        char caption[160];
        if(m_mode==Mode::Solid) std::snprintf(caption,sizeof(caption),"TEMPLE - ECLAIRAGE DE PREVISUALISATION   |   %zu triangles   |   ESPACE : fil de fer   |   ECHAP : quitter",m_triangles);
        else if(m_mode==Mode::Wireframe) std::snprintf(caption,sizeof(caption),"TEMPLE - FIL DE FER NATIF   |   %zu triangles / %zu aretes visibles   |   ESPACE : sommets   |   ECHAP : quitter",m_triangles,m_visible_edges);
        else std::snprintf(caption,sizeof(caption),"TEMPLE - SOMMETS   |   %zu / %zu points   |   ESPACE : surfaces   |   ECHAP : quitter",m_visible,m_total);
        SDL_RenderDebugText(renderer,24,24,caption);
    }
private:
    std::vector<std::vector<SDL_FRect>> m_batches;
    std::vector<WireBatch> m_wire_batches;
    size_t m_visible{0},m_total{0};
    size_t m_visible_edges{0},m_triangles{0};
    PreviewDepthImage m_solid_image;
    SDL_Texture *m_solid_texture{nullptr};
    size_t m_covered_pixels{0};
    Mode m_mode{Mode::Wireframe};
};
} // namespace u2::standalone
