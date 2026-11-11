#pragma once
#include <array>

struct U2NativeCameraDefinition;

// Host-only counterpart of the inline-vector path at 4355E6..435A11.
// Refuses external transforms and degenerate/nonfinite arithmetic; failure
// leaves out unchanged. It neither constructs a Camera nor applies a view.
bool u2_native_CalculateCameraOrientation(const U2NativeCameraDefinition &camera,
                                         std::array<float, 4> *out);
