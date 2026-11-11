#include "rendercamera.h"
#include <cmath>
#include <numbers>

bool u2_native_BuildCenteredPerspective(const U2NativePerspectiveParams &p,
                                       std::array<float, 16> *out) {
    if (!out || !std::isfinite(p.height_over_width) || !std::isfinite(p.horizontal_zoom) ||
        !std::isfinite(p.near_plane) || !std::isfinite(p.far_plane) ||
        !(p.height_over_width > 0) || !(p.horizontal_zoom > 0) ||
        !(p.near_plane > 0) || !(p.far_plane > p.near_plane)) return false;
    // 9DCBDA..9DCC58 with RenderCamera+270 = (1,1,0,0),
    // perspective flag +280 = 0 and user-defined matrix flag +282 = 0.
    const float depth_range = p.far_plane - p.near_plane;
    std::array<float, 16> result{};
    result[0] = -p.horizontal_zoom;
    result[5] = p.horizontal_zoom / p.height_over_width;
    result[8] = -0.0f;
    result[10] = p.far_plane / depth_range;
    result[11] = 1;
    result[14] = -(p.far_plane * p.near_plane) / depth_range;
    for (float value : result) if (!std::isfinite(value)) return false;
    if (!(result[5] > 0) || !(result[14] < 0)) return false;
    *out = result;
    return true;
}

bool u2_native_PerspectiveFromHorizontalFov(float radians, float height_over_width,
                                           float near_plane, float far_plane,
                                           std::array<float, 16> *out) {
    if (!std::isfinite(radians) || !(radians > 0) || !(radians < std::numbers::pi_v<float>))
        return false;
    // SetupCamera 9DE1C7..9DE1E0: 1 / tanf(FOV * 0.5).
    const float tangent = std::tan(radians * 0.5f);
    if (!(tangent > 0) || !std::isfinite(tangent)) return false;
    return u2_native_BuildCenteredPerspective({height_over_width, 1.0f / tangent, near_plane, far_plane}, out);
}
