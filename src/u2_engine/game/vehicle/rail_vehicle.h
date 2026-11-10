// Uncharted 2: Among Thieves — RailVehicle Subsystem
// Clean C++23 Reimplementation (game/vehicle/)
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
void   ABI u2_cpp_RailVehicle_InitBehavior(void *self, void *dc_behavior);
void   ABI u2_cpp_RailVehicle_SetFollowDistance(void *self, float begin_dist, float end_dist);
void * ABI u2_cpp_RailVehicle_GetVehicleClass(void *self);
void   ABI u2_cpp_RailVehicle_ResetWheels(void *self);
void * ABI u2_cpp_RailVehicle_GetDamageable(void *self, uint32_t index);
void   ABI u2_cpp_RailVehicle_OnIgcBind(void *self);
void   ABI u2_cpp_RailVehicle_OnIgcUnbind(void *self);

void * ABI u2_cpp_RailVehicleBehavior_GetDcBehavior(void *self);
void * ABI u2_cpp_RailVehicleBehaviorContainer_GetCurrentBehavior(void *self);

void * ABI u2_cpp_RailVehicleManager_GetMover(void *self, uint32_t idx);
void   ABI u2_cpp_RailVehicleManager_DetachFromRail(void *self, void *vehicle, int32_t rail_idx);
}
