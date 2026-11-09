/* Uncharted 2: Among Thieves — Native C++ Engine Detours Registry
 * Exposes all pure C++ engine subsystem detours as an independent module ("engine")
 * separate from the DC script runtime modules (player, npc, scene, event, audio).
 */
#include "u2_engine.h"

static const U2NativeDetour s_engine_detours[] = {
    /* catmull_rom.cpp */
    { "CatmullRomSpline_GetName",                        0x00468260, reinterpret_cast<uintptr_t>(&u2_cpp_CatmullRomSpline_GetName) },
    { "CatmullRomSpline_IsLooped",                       0x004684C0, reinterpret_cast<uintptr_t>(&u2_cpp_CatmullRomSpline_IsLooped) },
    { "CatmullRomSpline_GetControlPointCount",           0x00468520, reinterpret_cast<uintptr_t>(&u2_cpp_CatmullRomSpline_GetControlPointCount) },
    { "CatmullRomSpline_GetTotalArcLength",              0x00468D90, reinterpret_cast<uintptr_t>(&u2_cpp_CatmullRomSpline_GetTotalArcLength) },
    { "CatmullRomSpline_GetCumulSegArcLength",           0x00469F60, reinterpret_cast<uintptr_t>(&u2_cpp_CatmullRomSpline_GetCumulSegArcLength) },
    { "CatmullRomManager_OnSpawnGameObject",             0x004643E0, reinterpret_cast<uintptr_t>(&u2_cpp_CatmullRomManager_OnSpawnGameObject) },
    { "CatmullRomManager_OnDestroyGameObject",           0x00464490, reinterpret_cast<uintptr_t>(&u2_cpp_CatmullRomManager_OnDestroyGameObject) },
    { "CatmullRomManager_RemoveSpline",                  0x00465240, reinterpret_cast<uintptr_t>(&u2_cpp_CatmullRomManager_RemoveSpline) },
    { "CatmullRomManager_WalkSplines",                   0x00466320, reinterpret_cast<uintptr_t>(&u2_cpp_CatmullRomManager_WalkSplines) },
    { "SplineTracker_GetSpline0",                        0x00A24040, reinterpret_cast<uintptr_t>(&u2_cpp_SplineTracker_GetSpline0) },
    { "SplineTracker_GetSpline1",                        0x00A24080, reinterpret_cast<uintptr_t>(&u2_cpp_SplineTracker_GetSpline1) },
    { "SplineTracker_GetSplineNameId",                   0x00A240C0, reinterpret_cast<uintptr_t>(&u2_cpp_SplineTracker_GetSplineNameId) },
    { "SplineTracker_FinalizeDistance",                  0x00A24110, reinterpret_cast<uintptr_t>(&u2_cpp_SplineTracker_FinalizeDistance) },
    { "VehicleSplineTracker_GetCurrentVirtualDistance_m", 0x00ABA7B0, reinterpret_cast<uintptr_t>(&u2_cpp_VehicleSplineTracker_GetCurrentVirtualDistance_m) },
    { "VehicleSplineTracker_GetCurrentRealDistance_m",   0x00ABA840, reinterpret_cast<uintptr_t>(&u2_cpp_VehicleSplineTracker_GetCurrentRealDistance_m) },

    /* camera_zoom.cpp */
    { "CameraZoom_OverideHorizontalAngularConstraints",  0x0044BF00, reinterpret_cast<uintptr_t>(&u2_cpp_CameraZoom_OverideHorizontalAngularConstraints) },
    { "CameraZoom_ClearHorizontalAngularConstraints",    0x0044BFA0, reinterpret_cast<uintptr_t>(&u2_cpp_CameraZoom_ClearHorizontalAngularConstraints) },
    { "CameraZoom_OverideVerticalAngularConstraints",    0x0044BFE0, reinterpret_cast<uintptr_t>(&u2_cpp_CameraZoom_OverideVerticalAngularConstraints) },
    { "CameraZoom_ClearVerticalAngularConstraints",      0x0044C060, reinterpret_cast<uintptr_t>(&u2_cpp_CameraZoom_ClearVerticalAngularConstraints) },
    { "CameraZoom_SetDefaultVerticalAngularConstraints", 0x0044C090, reinterpret_cast<uintptr_t>(&u2_cpp_CameraZoom_SetDefaultVerticalAngularConstraints) },

    /* rail_vehicle.cpp */
    { "RailVehicle_InitBehavior",                        0x000013D0, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicle_InitBehavior) },
    { "RailVehicle_SetFollowDistance",                   0x00001460, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicle_SetFollowDistance) },
    { "RailVehicle_GetVehicleClass",                     0x00001DD0, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicle_GetVehicleClass) },
    { "RailVehicle_ResetWheels",                         0x00001ED0, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicle_ResetWheels) },
    { "RailVehicle_GetDamageable",                       0x00002430, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicle_GetDamageable) },
    { "RailVehicle_OnIgcBind",                           0x00005630, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicle_OnIgcBind) },
    { "RailVehicle_OnIgcUnbind",                         0x000056E0, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicle_OnIgcUnbind) },
    { "RailVehicleBehavior_GetDcBehavior",               0x00A8DE50, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicleBehavior_GetDcBehavior) },
    { "RailVehicleBehaviorContainer_GetCurrentBehavior", 0x00A8EA30, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicleBehaviorContainer_GetCurrentBehavior) },
    { "RailVehicleManager_GetMover",                     0x00A8FCE0, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicleManager_GetMover) },
    { "RailVehicleManager_DetachFromRail",               0x00A903E0, reinterpret_cast<uintptr_t>(&u2_cpp_RailVehicleManager_DetachFromRail) },
};

extern "C" size_t u2_get_engine_detours(const U2NativeDetour **out) {
    if (out) *out = s_engine_detours;
    return sizeof(s_engine_detours) / sizeof(s_engine_detours[0]);
}
