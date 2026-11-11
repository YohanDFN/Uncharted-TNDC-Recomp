#include "camera_position.h"
#include "../loading/level_resources.h"
#include <cmath>

bool u2_native_TranslateCameraPosition(const U2NativeCameraDefinition &camera,
                                      const U2NativeLevelOffset &level_offset,
                                      std::array<float, 3> *out) {
    if (!out || !level_offset.decoded || camera.uses_external_transform) return false;
    std::array<float, 3> result;
    for (size_t i = 0; i < 3; ++i) {
        if (!std::isfinite(camera.position[i]) || !std::isfinite(level_offset.translation[i])) return false;
        result[i] = camera.position[i] + level_offset.translation[i];
        if (!std::isfinite(result[i])) return false;
    }
    *out = result;
    return true;
}
