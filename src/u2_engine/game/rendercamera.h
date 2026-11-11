#pragma once
#include <array>

// Explicit host-owned inputs to the centered finite perspective path.
// These are not guessed CAMERA_TABLE_1 fields or a guest RenderCamera layout.
struct U2NativePerspectiveParams {
    float height_over_width{0};
    float horizontal_zoom{0}; // cot(horizontal FOV in radians / 2)
    float near_plane{0};
    float far_plane{0};
};

// Four consecutive columns, matching RenderCamera+90..+C0 at 9DCCD0.
// Camera +Z maps near/far to depth 0/1, with the original negative X scale.
// Only centered, finite perspective is supported; no GPU conversion is applied.
// Both entry points preserve out on failure and never access guest memory.
bool u2_native_BuildCenteredPerspective(const U2NativePerspectiveParams &params,
                                       std::array<float, 16> *out);
bool u2_native_PerspectiveFromHorizontalFov(float radians, float height_over_width,
                                           float near_plane, float far_plane,
                                           std::array<float, 16> *out);
