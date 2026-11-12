#pragma once
#include <algorithm>
#include <array>
#include <cmath>

namespace u2::standalone {
// Viewer-only directional light, unrelated to original materials or shaders.
// Inputs share the asset coordinate system. Do not modify stored normals.
inline bool preview_light_intensity(const std::array<float,3> &normal,
                                    const std::array<float,3> &toward_light,float *out) {
    if(!out) return false;
    for(float v : normal) if(!std::isfinite(v)) return false;
    for(float v : toward_light) if(!std::isfinite(v)) return false;
    const double n=std::hypot(double(normal[0]),double(normal[1]),double(normal[2]));
    const double l=std::hypot(double(toward_light[0]),double(toward_light[1]),double(toward_light[2]));
    if(!(n>0) || !(l>0)) return false;
    double dot=0;
    for(size_t i=0;i<3;++i) dot+=(normal[i]/n)*(toward_light[i]/l);
    *out=float(0.22+0.78*std::clamp(dot,0.0,1.0));
    return true;
}
} // namespace u2::standalone
