/* Uncharted 2: Among Thieves — Native C++ Engine Subsystems
 * Source unit:
 *   - src/game/camera/camerazoom.cpp
 */
#include "u2_engine.h"

using namespace u2::engine;

extern "C" {

void ABI u2_cpp_CameraZoom_OverideHorizontalAngularConstraints(const float *dir, float min_a, float max_a) {
    /* 0x0044BF00: CameraZoom::OverideHorizontalAngularConstraints(Vector const&, float, float) */
    if (!dir || !g_u2_image_base) return;
    *elf_ptr<uint8_t>(0x0147FDE0ull) = 1;
    *elf_ptr<float>(0x0104EB10ull) = min_a;
    std::memcpy(elf_ptr(0x011142B0ull), dir, 16);
    *elf_ptr<float>(0x011142ACull) = max_a;
}

void ABI u2_cpp_CameraZoom_ClearHorizontalAngularConstraints(void) {
    /* 0x0044BFA0: CameraZoom::ClearHorizontalAngularConstraints() */
    if (!g_u2_image_base) return;
    *elf_ptr<uint8_t>(0x0147FDE0ull) = 0;
    *elf_ptr<float>(0x0104EB10ull) = -1.0f;
    std::memcpy(elf_ptr(0x011142B0ull), elf_ptr(0x0147FDD0ull), 16);
    *elf_ptr<float>(0x011142ACull) = 0.0f;
}

void ABI u2_cpp_CameraZoom_OverideVerticalAngularConstraints(float min_a, float max_a) {
    /* 0x0044BFE0: CameraZoom::OverideVerticalAngularConstraints(float, float) */
    if (!g_u2_image_base) return;
    *elf_ptr<uint8_t>(0x0147FDE1ull) = 1;
    *elf_ptr<float>(0x0104EB14ull) = min_a;
    *elf_ptr<float>(0x0104EB18ull) = max_a;
}

void ABI u2_cpp_CameraZoom_ClearVerticalAngularConstraints(void) {
    /* 0x0044C060: CameraZoom::ClearVerticalAngularConstraints() */
    if (!g_u2_image_base) return;
    *elf_ptr<uint8_t>(0x0147FDE1ull) = 0;
    *elf_ptr<float>(0x0104EB14ull) = *elf_ptr<const float>(0x0104EB1Cull);
    *elf_ptr<float>(0x0104EB18ull) = *elf_ptr<const float>(0x0104EB20ull);
}

void ABI u2_cpp_CameraZoom_SetDefaultVerticalAngularConstraints(float min_a, float max_a) {
    /* 0x0044C090: CameraZoom::SetDefaultVerticalAngularConstraints(float, float) */
    if (!g_u2_image_base) return;
    *elf_ptr<float>(0x0104EB1Cull) = min_a;
    *elf_ptr<float>(0x0104EB20ull) = max_a;
    if (*elf_ptr<const uint8_t>(0x0147FDE1ull) == 0) {
        *elf_ptr<float>(0x0104EB14ull) = min_a;
        *elf_ptr<float>(0x0104EB18ull) = max_a;
    }
}

} // extern "C"
