#pragma once
#include <SDL3/SDL.h>
#include <array>
#include <cmath>
#include <cstdio>
#include <vector>
#include "ndlib/io/package_manager.h"
#include "game/rendercamera.h"

namespace u2::standalone {

// Asset-position viewer, not the Level/instance/render pipeline. The captured
// reference lens is scoped to this preview, never used as an engine setting.
class TemplePreview {
public:
    bool load() {
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
        std::vector<std::vector<SDL_FRect>> batches;
        size_t visible=0,total=0;
        for (const auto &mesh : meshes) {
            auto &points=batches.emplace_back();
            points.reserve(mesh.positions.size());
            total+=mesh.positions.size();
            for (const auto &position : mesh.positions) {
                std::array<float,3> delta,view{};
                for(size_t i=0;i<3;++i) delta[i]=position[i]-camera.translated_position[i];
                for(size_t i=0;i<3;++i)
                    for(size_t j=0;j<3;++j) view[i]+=basis[i][j]*delta[j];
                if (!(view[2]>=0.24f && view[2]<=5000)) continue;
                const float nx=projection[0]*view[0]/view[2],ny=projection[5]*view[1]/view[2];
                if (!std::isfinite(nx) || !std::isfinite(ny) || nx < -1 || nx > 1 || ny < -1 || ny > 1) continue;
                points.push_back({(nx+1)*960-1.25f,(1-ny)*540-1.25f,2.5f,2.5f});
            }
            visible+=points.size();
        }
        if (!visible) return false;
        m_batches=std::move(batches);m_visible=visible;m_total=total;
        std::printf("[U2 Native Preview] temple meshes=%zu positions=%zu visible=%zu camera='%s' reference-view zoom=1.303225398 near=0.24 far=5000 (asset positions only)\n",
            meshes.size(),total,visible,camera.name.c_str());
        return true;
    }
    bool ready() const { return m_visible!=0; }
    void draw(SDL_Renderer *renderer) const {
        if (!ready()) return;
        SDL_SetRenderDrawColor(renderer,8,12,19,255);SDL_RenderClear(renderer);
        constexpr SDL_Color palette[]={{224,188,120,255},{120,189,235,255},{173,204,164,255},
            {236,154,121,255},{175,160,226,255},{117,209,196,255},{222,209,163,255}};
        for(size_t i=0;i<m_batches.size();++i) {
            const auto color=palette[i%std::size(palette)];
            SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
            if (!m_batches[i].empty()) SDL_RenderFillRects(renderer,m_batches[i].data(),static_cast<int>(m_batches[i].size()));
        }
        SDL_SetRenderDrawColor(renderer,8,12,19,230);
        const SDL_FRect bar{0,0,1920,56};SDL_RenderFillRect(renderer,&bar);
        SDL_SetRenderDrawColor(renderer,240,235,220,255);
        char caption[160];
        std::snprintf(caption,sizeof(caption),"TEMPLE - APERCU NATIF DES SOMMETS   |   %zu / %zu points   |   ECHAP : quitter",m_visible,m_total);
        SDL_RenderDebugText(renderer,24,24,caption);
    }
private:
    std::vector<std::vector<SDL_FRect>> m_batches;
    size_t m_visible{0},m_total{0};
};
} // namespace u2::standalone
