// Uncharted 2: Among Thieves — CameraZoom Subsystem
// Clean C++23 Reimplementation (game/camera/)
#pragma once

#ifndef ABI
#if defined(_WIN32)
#define ABI __attribute__((sysv_abi))
#else
#define ABI
#endif
#endif

extern "C" {
void ABI u2_cpp_CameraZoom_OverideHorizontalAngularConstraints(const float *dir, float min_a, float max_a);
void ABI u2_cpp_CameraZoom_ClearHorizontalAngularConstraints(void);
void ABI u2_cpp_CameraZoom_OverideVerticalAngularConstraints(float min_a, float max_a);
void ABI u2_cpp_CameraZoom_ClearVerticalAngularConstraints(void);
void ABI u2_cpp_CameraZoom_SetDefaultVerticalAngularConstraints(float min_a, float max_a);
}
