#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <new>
#include <utility>
#include <vector>

namespace u2::standalone {

// Cached CPU depth image for the static asset viewer. No guest structures,
// original render-state replacement, lighting or material approximation.
class PreviewDepthImage {
public:
    using Vertex=std::array<float,4>;
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
    size_t triangle(const std::array<Vertex,3> &vertices,uint32_t color) {
        if(width<=0 || height<=0 || pixels.size()!=size_t(width)*height || depth.size()!=pixels.size()) return 0;
        std::array<Vertex,12> polygon{},clipped{};
        size_t count=3;
        for(size_t i=0;i<3;++i) {
            for(float value : vertices[i]) if(!std::isfinite(value)) return 0;
            polygon[i]=vertices[i];
        }
        for(size_t plane=0;plane<6;++plane) {
            size_t next_count=0;
            auto previous=polygon[count-1];
            float previous_distance=distance(previous,plane);
            if(!std::isfinite(previous_distance)) return 0;
            for(size_t i=0;i<count;++i) {
                const auto current=polygon[i];
                const float current_distance=distance(current,plane);
                if(!std::isfinite(current_distance)) return 0;
                if((previous_distance<0)!=(current_distance<0)) {
                    const float divisor=previous_distance-current_distance;
                    if(!std::isfinite(divisor) || divisor==0 || next_count==clipped.size()) return 0;
                    const float t=previous_distance/divisor;
                    Vertex intersection;
                    for(size_t axis=0;axis<4;++axis) {
                        intersection[axis]=previous[axis]+t*(current[axis]-previous[axis]);
                        if(!std::isfinite(intersection[axis])) return 0;
                    }
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
            const auto &v=polygon[i];
            if(!(v[3]>0)) return 0;
            const float x=v[0]/v[3],y=v[1]/v[3],z=v[2]/v[3];
            if(!std::isfinite(x) || !std::isfinite(y) || !std::isfinite(z)) return 0;
            screen[i]={(std::clamp(x,-1.0f,1.0f)+1)*0.5f*width,
                       (1-std::clamp(y,-1.0f,1.0f))*0.5f*height,std::clamp(z,0.0f,1.0f)};
        }
        size_t written=0;
        for(size_t i=1;i+1<count;++i) written+=rasterize(screen[0],screen[i],screen[i+1],color);
        return written;
    }

private:
    struct ScreenVertex { float x,y,z; };
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
    size_t rasterize(ScreenVertex a,ScreenVertex b,ScreenVertex c,uint32_t color) {
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
                depth[pixel]=z;pixels[pixel]=color;++written;
            }
        }
        return written;
    }
};
} // namespace u2::standalone
