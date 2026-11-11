#pragma once
#include <array>
#include <cmath>

namespace u2::standalone {
// Homogeneous line clipping for the established x/y [-w,w], z [0,w]
// perspective convention. Host preview utility; failure preserves both inputs.
inline bool clip_preview_line(std::array<float,4> &first, std::array<float,4> &last) {
    auto a=first,b=last;
    for (float value : a) if (!std::isfinite(value)) return false;
    for (float value : b) if (!std::isfinite(value)) return false;
    auto distance=[](const std::array<float,4> &v,size_t plane) {
        switch(plane) {
            case 0: return v[2];
            case 1: return v[3]-v[2];
            case 2: return v[3]+v[0];
            case 3: return v[3]-v[0];
            case 4: return v[3]+v[1];
            default: return v[3]-v[1];
        }
    };
    for(size_t plane=0;plane<6;++plane) {
        const float da=distance(a,plane),db=distance(b,plane);
        if (!std::isfinite(da) || !std::isfinite(db) || (da<0 && db<0)) return false;
        if ((da<0)!=(db<0)) {
            const float divisor=da-db;
            if (!std::isfinite(divisor) || divisor==0) return false;
            const float t=da/divisor;
            std::array<float,4> intersection;
            for(size_t i=0;i<4;++i) {
                intersection[i]=a[i]+t*(b[i]-a[i]);
                if (!std::isfinite(intersection[i])) return false;
            }
            if (da<0) a=intersection; else b=intersection;
        }
    }
    if (!(a[3]>0) || !(b[3]>0)) return false;
    first=a;last=b;
    return true;
}
} // namespace u2::standalone
