// Uncharted 2: Among Thieves — Catmull-Rom Spline & Trackers
// Clean C++23 Reimplementation (ndlib/anim/)
#pragma once

#include <cstdint>

#ifndef ABI
#if defined(_WIN32)
#define ABI __attribute__((sysv_abi))
#else
#define ABI
#endif
#endif

extern "C" {
const char * ABI u2_cpp_CatmullRomSpline_GetName(void *self);
uint32_t     ABI u2_cpp_CatmullRomSpline_IsLooped(void *self);
uint32_t     ABI u2_cpp_CatmullRomSpline_GetControlPointCount(void *self);
float        ABI u2_cpp_CatmullRomSpline_GetTotalArcLength(void *self);
float        ABI u2_cpp_CatmullRomSpline_GetCumulSegArcLength(void *self, int32_t seg_idx);

void         ABI u2_cpp_CatmullRomManager_OnSpawnGameObject(void *mgr, void *game_obj);
void         ABI u2_cpp_CatmullRomManager_OnDestroyGameObject(void *mgr, void *game_obj);
void         ABI u2_cpp_CatmullRomManager_RemoveSpline(void *mgr, void *spline_data);
void         ABI u2_cpp_CatmullRomManager_WalkSplines(void *mgr, void (ABI *cb)(void *, void *), void *user_data, int32_t all_flag);

void *       ABI u2_cpp_SplineTracker_GetSpline0(void *self);
void *       ABI u2_cpp_SplineTracker_GetSpline1(void *self);
uint32_t     ABI u2_cpp_SplineTracker_GetSplineNameId(void *self);
void         ABI u2_cpp_SplineTracker_FinalizeDistance(void *self);

float        ABI u2_cpp_VehicleSplineTracker_GetCurrentVirtualDistance_m(void *self, int32_t mode);
float        ABI u2_cpp_VehicleSplineTracker_GetCurrentRealDistance_m(void *self, int32_t mode, uint32_t wrap);
}
