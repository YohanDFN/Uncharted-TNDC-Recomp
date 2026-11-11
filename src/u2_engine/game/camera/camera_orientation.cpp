#include "camera_orientation.h"
#include "../loading/level_resources.h"
#include <cmath>

bool u2_native_CalculateCameraOrientation(const U2NativeCameraDefinition &camera,
                                         std::array<float, 4> *out) {
    if (!out || camera.uses_external_transform) return false;
    auto normalize = [](const std::array<float, 3> &v, std::array<float, 3> &unit) {
        for (float value : v) if (!std::isfinite(value)) return false;
        const float length_squared = (v[0] * v[0] + v[1] * v[1]) + v[2] * v[2];
        if (!(length_squared > 0) || !std::isfinite(length_squared)) return false;
        // The ELF uses reciprocal-square-root approximation plus one Newton
        // refinement. Scalar sqrt/division is compared within 2e-6, not bitwise.
        const float length = std::sqrt(length_squared);
        for (size_t i = 0; i < 3; ++i) unit[i] = v[i] / length;
        return true;
    };
    const auto &stored_a = camera.orientation_axis_a;
    const auto &stored_b = camera.orientation_axis_b;
    std::array<float, 3> a, b, c;
    if (!normalize(stored_a, a) || !normalize(stored_b, b)) return false;
    // 4356CA..435756 uses the original vectors for A x B, then normalizes it.
    const std::array<float, 3> cross{
        stored_a[1] * stored_b[2] - stored_a[2] * stored_b[1],
        stored_a[2] * stored_b[0] - stored_a[0] * stored_b[2],
        stored_a[0] * stored_b[1] - stored_a[1] * stored_b[0]};
    if (!normalize(cross, c)) return false;
    // Matrix columns C, A, B. Match the ELF's Bz / Cx / Ay branch choices,
    // including ties, and its xyzw signs (43578C..435A0B).
    std::array<float, 4> q;
    float t;
    if (b[2] < 0) {
        if (c[0] > a[1]) {
            t = ((1 + c[0]) - a[1]) - b[2];
            q = {t, a[0] + c[1], c[2] + b[0], a[2] - b[1]};
        } else {
            t = ((1 - c[0]) + a[1]) - b[2];
            q = {a[0] + c[1], t, a[2] + b[1], b[0] - c[2]};
        }
    } else if (c[0] < -a[1]) {
        t = ((1 - c[0]) - a[1]) + b[2];
        q = {c[2] + b[0], a[2] + b[1], t, c[1] - a[0]};
    } else {
        t = ((1 + c[0]) + a[1]) + b[2];
        q = {a[2] - b[1], b[0] - c[2], c[1] - a[0], t};
    }
    if (!(t > 0) || !std::isfinite(t)) return false;
    const float scale = 0.5f / std::sqrt(t);
    for (float &component : q) {
        component *= scale;
        if (!std::isfinite(component)) return false;
    }
    *out = q;
    return true;
}
