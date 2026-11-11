#pragma once
#include <array>

struct U2NativeCameraDefinition;
struct U2NativeLevelOffset;

// Inline camera path at 435A11..435A55. Stored position plus Level translation.
// Host data only; failure preserves out. External transforms remain unsupported.
bool u2_native_TranslateCameraPosition(const U2NativeCameraDefinition &camera,
                                      const U2NativeLevelOffset &level_offset,
                                      std::array<float, 3> *out);
