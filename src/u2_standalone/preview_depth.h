#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <new>
#include <utility>
#include <vector>
#include "preview_texture.h"
#include "preview_lighting.h"

namespace u2::standalone {

// Cached CPU depth image for the static asset viewer. No guest structures,
// original render-state replacement or original material approximation.
class PreviewDepthImage {
public:
    using Vertex=std::array<float,4>;
    using Vec3=std::array<float,3>;
    struct NormalLighting {
        std::array<Vec3,3> normals;
        Vec3 tangent,bitangent,toward_light;
        NormalLighting() : normals{},tangent{},bitangent{},toward_light{} {}
    };
    // UV-derived frame in asset coordinates. A degenerate UV triangle uses Gouraud.
    static bool tangent_frame(const std::array<Vec3,3> &p,
                              const std::array<std::array<float,2>,3> &uv,NormalLighting *out) {
        if(!out) return false;
        const float du1=uv[1][0]-uv[0][0],dv1=uv[1][1]-uv[0][1];
        const float du2=uv[2][0]-uv[0][0],dv2=uv[2][1]-uv[0][1];
        const float det=du1*dv2-du2*dv1;
        if(!std::isfinite(det) || std::abs(det)<1e-12f) return false;
        for(size_t i=0;i<3;++i) {
            out->tangent[i]=((p[1][i]-p[0][i])*dv2-(p[2][i]-p[0][i])*dv1)/det;
            out->bitangent[i]=((p[2][i]-p[0][i])*du1-(p[1][i]-p[0][i])*du2)/det;
        }
        return normalize(out->tangent) && normalize(out->bitangent);
    }
    int width{0},height{0};
    std::vector<uint32_t> pixels;
    std::vector<float> depth;

    bool reset(int w,int h,uint32_t background) {
        constexpr size_t max_pixels=16*1024*1024;
        if(w<=0 || h<=0 || size_t(w)>max_pixels/size_t(h)) return false;
        try {
            std::vector<uint32_t> colors(size_t(w)*h,background);
            std::vector<float> depths(colors.size(),std::numeric_limits<float>::infinity());
            pixels=std::move(colors);depth=std::move(depths);width=w;height=h;
            return true;
        } catch(const std::bad_alloc &) { return false; }
    }

    // Clip the triangle to x/y [-w,w], z [0,w], then rasterize its convex fan.
    // Returns depth-passing sample writes, not the number of unique pixels.
    // Optional vertex brightness is interpolated with perspective correction.
    size_t triangle(const std::array<Vertex,3> &vertices,uint32_t color,
                    const std::array<float,3> &brightness={1,1,1},
                    const U2NativeTexturePixels *texture=nullptr,
                    const std::array<std::array<float,2>,3> &uv={},
                    bool alpha_cutout=false,
                    const U2NativeTexturePixels *normal_map=nullptr,
                    const NormalLighting &lighting={}) {
        if(width<=0 || height<=0 || pixels.size()!=size_t(width)*height || depth.size()!=pixels.size()) return 0;
        if(texture && !preview_texture_valid(*texture)) return 0;
        if(normal_map && !preview_texture_valid(*normal_map)) return 0;
        struct ClipVertex { Vertex position;float brightness;std::array<float,2> uv;Vec3 normal; };
        std::array<ClipVertex,12> polygon{},clipped{};
        size_t count=3;
        for(size_t i=0;i<3;++i) {
            for(float value : vertices[i]) if(!std::isfinite(value)) return 0;
            if(!std::isfinite(brightness[i]) || brightness[i]<0 || brightness[i]>1) return 0;
            if(texture || normal_map) for(float value : uv[i]) if(!std::isfinite(value)) return 0;
            polygon[i]={vertices[i],brightness[i],(texture || normal_map) ? uv[i] : std::array<float,2>{},lighting.normals[i]};
            if(normal_map) for(float value : lighting.normals[i]) if(!std::isfinite(value)) return 0;
        }
        for(size_t plane=0;plane<6;++plane) {
            size_t next_count=0;
            auto previous=polygon[count-1];
            float previous_distance=distance(previous.position,plane);
            if(!std::isfinite(previous_distance)) return 0;
            for(size_t i=0;i<count;++i) {
                const auto current=polygon[i];
                const float current_distance=distance(current.position,plane);
                if(!std::isfinite(current_distance)) return 0;
                if((previous_distance<0)!=(current_distance<0)) {
                    const float divisor=previous_distance-current_distance;
                    if(!std::isfinite(divisor) || divisor==0 || next_count==clipped.size()) return 0;
                    const float t=previous_distance/divisor;
                    ClipVertex intersection;
                    for(size_t axis=0;axis<4;++axis) {
                        intersection.position[axis]=previous.position[axis]+t*(current.position[axis]-previous.position[axis]);
                        if(!std::isfinite(intersection.position[axis])) return 0;
                    }
                    intersection.brightness=previous.brightness+t*(current.brightness-previous.brightness);
                    if(!std::isfinite(intersection.brightness)) return 0;
                    for(size_t axis=0;axis<2;++axis) {
                        intersection.uv[axis]=previous.uv[axis]+t*(current.uv[axis]-previous.uv[axis]);
                        if(!std::isfinite(intersection.uv[axis])) return 0;
                    }
                    for(size_t axis=0;axis<3;++axis)
                        intersection.normal[axis]=previous.normal[axis]+t*(current.normal[axis]-previous.normal[axis]);
                    clipped[next_count++]=intersection;
                }
                if(current_distance>=0) {
                    if(next_count==clipped.size()) return 0;
                    clipped[next_count++]=current;
                }
                previous=current;previous_distance=current_distance;
            }
            if(next_count<3) return 0;
            polygon=clipped;count=next_count;
        }
        std::array<ScreenVertex,12> screen{};
        for(size_t i=0;i<count;++i) {
            const auto &v=polygon[i].position;
            if(!(v[3]>0)) return 0;
            const float x=v[0]/v[3],y=v[1]/v[3],z=v[2]/v[3];
            if(!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) return 0;
            const float inverse_w=1/v[3];
            if(!std::isfinite(inverse_w)) return 0;
            screen[i]={(std::clamp(x,-1.0f,1.0f)+1)*0.5f*width,
                       (1-std::clamp(y,-1.0f,1.0f))*0.5f*height,std::clamp(z,0.0f,1.0f),
                       inverse_w,polygon[i].brightness*inverse_w,
                       polygon[i].uv[0]*inverse_w,polygon[i].uv[1]*inverse_w,
                       {polygon[i].normal[0]*inverse_w,polygon[i].normal[1]*inverse_w,polygon[i].normal[2]*inverse_w}};
            if(!std::isfinite(screen[i].u_over_w) || !std::isfinite(screen[i].v_over_w)) return 0;
        }
        size_t written=0;
        for(size_t i=1;i+1<count;++i) written+=rasterize(screen[0],screen[i],screen[i+1],color,texture,alpha_cutout,normal_map,lighting);
        return written;
    }

private:
    struct ScreenVertex { float x,y,z,inverse_w,brightness_over_w,u_over_w,v_over_w;Vec3 normal_over_w; };
    static bool normalize(Vec3 &v) {
        const float length=std::hypot(v[0],v[1],v[2]);
        if(!std::isfinite(length) || !(length>1e-12f)) return false;
        for(float &x : v) x/=length;
        return true;
    }
    static float distance(const Vertex &v,size_t plane) {
        switch(plane) {
            case 0: return v[2];
            case 1: return v[3]-v[2];
            case 2: return v[3]+v[0];
            case 3: return v[3]-v[0];
            case 4: return v[3]+v[1];
            default: return v[3]-v[1];
        }
    }
    static float edge(const ScreenVertex &a,const ScreenVertex &b,float x,float y) {
        return (b.x-a.x)*(y-a.y)-(b.y-a.y)*(x-a.x);
    }
    size_t rasterize(ScreenVertex a,ScreenVertex b,ScreenVertex c,uint32_t color,const U2NativeTexturePixels *texture,bool alpha_cutout,
                     const U2NativeTexturePixels *normal_map,const NormalLighting &lighting) {
        float area=edge(a,b,c.x,c.y);
        if(!std::isfinite(area) || area==0) return 0;
        if(area<0) { std::swap(b,c);area=-area; }
        const int left=std::max(0,int(std::ceil(std::min({a.x,b.x,c.x})-0.5f)));
        const int right=std::min(width-1,int(std::floor(std::max({a.x,b.x,c.x})-0.5f)));
        const int top=std::max(0,int(std::ceil(std::min({a.y,b.y,c.y})-0.5f)));
        const int bottom=std::min(height-1,int(std::floor(std::max({a.y,b.y,c.y})-0.5f)));
        size_t written=0;
        for(int y=top;y<=bottom;++y) for(int x=left;x<=right;++x) {
            const float e0=edge(b,c,x+0.5f,y+0.5f),e1=edge(c,a,x+0.5f,y+0.5f),e2=edge(a,b,x+0.5f,y+0.5f);
            if(e0<0 || e1<0 || e2<0) continue;
            // z/w is linear in screen space; do not perspective-correct depth again.
            const float interpolated=(e0*a.z+e1*b.z+e2*c.z)/area;
            if(!std::isfinite(interpolated)) continue;
            // Clipped vertex depths are in [0,1]; bound interpolation roundoff
            // at the exact clip planes so a far-plane surface has no holes.
            const float z=std::clamp(interpolated,0.0f,1.0f);
            const size_t pixel=size_t(y)*width+x;
            if(z<depth[pixel]) {
                const float denominator=e0*a.inverse_w+e1*b.inverse_w+e2*c.inverse_w;
                const float brightness=(e0*a.brightness_over_w+e1*b.brightness_over_w+e2*c.brightness_over_w)/denominator;
                if(!(denominator>0) || !std::isfinite(brightness)) continue;
                float amount=std::clamp(brightness,0.0f,1.0f);
                const float u=(e0*a.u_over_w+e1*b.u_over_w+e2*c.u_over_w)/denominator;
                const float v=(e0*a.v_over_w+e1*b.v_over_w+e2*c.v_over_w)/denominator;
                if(normal_map) {
                    Vec3 n{},t=lighting.tangent;
                    for(size_t i=0;i<3;++i)
                        n[i]=(e0*a.normal_over_w[i]+e1*b.normal_over_w[i]+e2*c.normal_over_w[i])/denominator;
                    if(normalize(n)) {
                        float dot=0;for(size_t i=0;i<3;++i) dot+=t[i]*n[i];
                        for(size_t i=0;i<3;++i) t[i]-=dot*n[i];
                        if(normalize(t)) {
                            Vec3 bvec{n[1]*t[2]-n[2]*t[1],n[2]*t[0]-n[0]*t[2],n[0]*t[1]-n[1]*t[0]};
                            float handedness=0;for(size_t i=0;i<3;++i) handedness+=bvec[i]*lighting.bitangent[i];
                            if(handedness<0) for(float &x : bvec) x=-x;
                            uint32_t sample=0;
                            if(preview_sample_repeat(*normal_map,u,v,1,&sample)) {
                                const float nx=float(sample&255)*(2.0f/255)-1;
                                const float ny=float((sample>>8)&255)*(2.0f/255)-1;
                                const float nz=float((sample>>16)&255)*(2.0f/255)-1;
                                Vec3 mapped{};
                                for(size_t i=0;i<3;++i) mapped[i]=t[i]*nx+bvec[i]*ny+n[i]*nz;
                                preview_light_intensity(mapped,lighting.toward_light,&amount);
                            }
                        }
                    }
                }
                uint32_t shaded=color&0xFF000000u;
                if(texture) {
                    if(!preview_sample_repeat(*texture,u,v,amount,&shaded)) continue;
                    if(alpha_cutout && (shaded>>24)<128) continue;
                } else for(unsigned shift : {0u,8u,16u})
                    shaded|=uint32_t(std::lround(float((color>>shift)&255)*amount))<<shift;
                depth[pixel]=z;pixels[pixel]=shaded;++written;
            }
        }
        return written;
    }
};
} // namespace u2::standalone
