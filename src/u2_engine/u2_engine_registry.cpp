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

    /* process_system.cpp (Boot & Process Tree) */
    { "ProcessMgr_LookupProcessByPid",                   0x009BBCF0, reinterpret_cast<uintptr_t>(&u2_cpp_ProcessMgr_LookupProcessByPid) },
    { "Process_LookupProcessByPid",                      0x009BEF30, reinterpret_cast<uintptr_t>(&u2_cpp_Process_LookupProcessByPid) },
    { "Process_ValidateInternalPointer",                 0x009BE660, reinterpret_cast<uintptr_t>(&u2_cpp_Process_ValidateInternalPointer) },
    { "ProcessMgr_AddProcessForScriptUpdate",            0x009B8CD0, reinterpret_cast<uintptr_t>(&u2_cpp_ProcessMgr_AddProcessForScriptUpdate) },
    { "ProcessList_Alloc",                               0x009B0D70, reinterpret_cast<uintptr_t>(&u2_cpp_ProcessList_Alloc) },
    { "Process_DMenuStateProp",                          0x001FE420, reinterpret_cast<uintptr_t>(&u2_cpp_Process_DMenuStateProp) },
    { "Process_DMenuFloatProp_354",                      0x001FE7E0, reinterpret_cast<uintptr_t>(&u2_cpp_Process_DMenuFloatProp_354) },
    { "Process_DMenuBoolProp_43A",                       0x001FE8C0, reinterpret_cast<uintptr_t>(&u2_cpp_Process_DMenuBoolProp_43A) },
    { "Process_DMenuBoolProp_43F",                       0x001FE980, reinterpret_cast<uintptr_t>(&u2_cpp_Process_DMenuBoolProp_43F) },
    { "Process_DMenuBoolProp_439",                       0x001FECA0, reinterpret_cast<uintptr_t>(&u2_cpp_Process_DMenuBoolProp_439) },
    { "Process_DMenuBoolProp_438",                       0x001FED70, reinterpret_cast<uintptr_t>(&u2_cpp_Process_DMenuBoolProp_438) },
    { "Process_DMenuBoolProp_43B",                       0x001FEE40, reinterpret_cast<uintptr_t>(&u2_cpp_Process_DMenuBoolProp_43B) },

    /* save_hud.cpp (SaveBuffer, Settings, TaskGraph, PlayGo & HUD) */
    { "SaveBuffer_InitExternal",                         0x00CD0E10, reinterpret_cast<uintptr_t>(&u2_cpp_SaveBuffer_InitExternal) },
    { "SaveBuffer_DebugSetBuffer",                       0x00CD0E80, reinterpret_cast<uintptr_t>(&u2_cpp_SaveBuffer_DebugSetBuffer) },
    { "SaveBuffer_CopyFrom",                             0x00CD0EE0, reinterpret_cast<uintptr_t>(&u2_cpp_SaveBuffer_CopyFrom) },
    { "SaveBuffer_WriteBytes",                           0x00CD16A0, reinterpret_cast<uintptr_t>(&u2_cpp_SaveBuffer_WriteBytes) },
    { "SaveBuffer_WriteU64Count0",                       0x00CD1730, reinterpret_cast<uintptr_t>(&u2_cpp_SaveBuffer_WriteU64Count0) },
    { "SaveBuffer_WriteU64Count1",                       0x00CD17C0, reinterpret_cast<uintptr_t>(&u2_cpp_SaveBuffer_WriteU64Count1) },
    { "Hud_FreeGrenadeIndicator",                        0x00554F70, reinterpret_cast<uintptr_t>(&u2_cpp_Hud_FreeGrenadeIndicator) },
    { "Hud_ShowGrenadeIndicator",                        0x00554FD0, reinterpret_cast<uintptr_t>(&u2_cpp_Hud_ShowGrenadeIndicator) },
    { "GameTask_cbCollectTaskNode",                      0x00A5F970, reinterpret_cast<uintptr_t>(&u2_cpp_GameTask_cbCollectTaskNode) },
    { "Settings_FreeSetting",                            0x00A1D600, reinterpret_cast<uintptr_t>(&u2_cpp_Settings_FreeSetting) },
    { "GameLoading_IsChunkSetEntirelyWithin",            0x00B77AA0, reinterpret_cast<uintptr_t>(&u2_cpp_GameLoading_IsChunkSetEntirelyWithin) },
    { "GameLoading_GetPlayGoSpeed",                      0x00B770D0, reinterpret_cast<uintptr_t>(&u2_cpp_GameLoading_GetPlayGoSpeed) },

    /* Host AllocStack ownership is incompatible with the ELF pool/bitmap.
     * Keep the complete allocation/stack/release family on the native side. */
#if defined(U2_STANDALONE)
    { "AllocateAllocStack",                              0x00B36D40, reinterpret_cast<uintptr_t>(&u2_cpp_AllocateAllocStack) },
    { "FreeThreadAllocStack",                            0x00B35D30, reinterpret_cast<uintptr_t>(&u2_cpp_FreeThreadAllocStack) },
    { "PushAllocator",                                   0x00B360D0, reinterpret_cast<uintptr_t>(&u2_cpp_PushAllocator) },
    { "PushAllocator_Tag",                               0x00B35EA0, reinterpret_cast<uintptr_t>(&u2_cpp_PushAllocator_Tag) },
    { "PushAllocator_Ext",                               0x00B371A0, reinterpret_cast<uintptr_t>(&u2_cpp_PushAllocator_Ext) },
    { "PopAllocator",                                    0x00B361C0, reinterpret_cast<uintptr_t>(&u2_cpp_PopAllocator) },
    { "PopAllocator_Expected",                           0x00B377B0, reinterpret_cast<uintptr_t>(&u2_cpp_PopAllocator_Expected) },
    { "GetAllocatorStackDepth",                          0x00B362A0, reinterpret_cast<uintptr_t>(&u2_cpp_GetAllocatorStackDepth) },
#endif
    { "ProcessHeap_ScanForProcessDroppings",             0x009B7770, reinterpret_cast<uintptr_t>(&u2_cpp_ProcessHeap_ScanForProcessDroppings) },

    /* boot_state_machine.cpp (Boot & State Machine) */
    { "Settings_Init",                                   0x00A1C710, reinterpret_cast<uintptr_t>(&u2_cpp_Settings_Init) },
    { "Settings_NewSetting",                             0x00A1CD50, reinterpret_cast<uintptr_t>(&u2_cpp_Settings_NewSetting) },
    { "ProcessList_StartUp",                             0x009B0A30, reinterpret_cast<uintptr_t>(&u2_cpp_ProcessList_StartUp) },
    { "ProcessMgr_Init",                                 0x009B9530, reinterpret_cast<uintptr_t>(&u2_cpp_ProcessMgr_Init) },
    { "GameLoading_MountInitialArchives",                0x00B783F0, reinterpret_cast<uintptr_t>(&u2_cpp_GameLoading_MountInitialArchives) },
    { "GameLoading_PlayGoWait",                          0x00B78790, reinterpret_cast<uintptr_t>(&u2_cpp_GameLoading_PlayGoWait) },
    { "IntroState_Update",                               0x00A551B0, reinterpret_cast<uintptr_t>(&u2_cpp_IntroState_Update) },
    { "UCCApp_StateMachineUpdate",                       0x00D59A50, reinterpret_cast<uintptr_t>(&u2_cpp_UCCApp_StateMachineUpdate) },

    /* file_loader.cpp (streaming.cpp, file-loader.cpp) */
    { "LoadFileHelper",                                  0x00CED020, reinterpret_cast<uintptr_t>(&u2_cpp_LoadFileHelper) },
    { "Streaming_ResolvePath",                           0x00B70270, reinterpret_cast<uintptr_t>(&u2_cpp_Streaming_ResolvePath) },

    /* game/weapon/tranquilizer.cpp */
    { "Tranquilizer_Init",                               0x009E0CC0, reinterpret_cast<uintptr_t>(&u2_cpp_Tranquilizer_Init) },
    { "UnlockableManager_SetDefaultWeapon",              0x00A75450, reinterpret_cast<uintptr_t>(&u2_cpp_UnlockableManager_SetDefaultWeapon) },

    /* game/hud/save_hud.cpp & hud.cpp */
    { "Hud_ReadConfigFile",                              0x00552C70, reinterpret_cast<uintptr_t>(&u2_cpp_Hud_ReadConfigFile) },

    /* game/script/script_manager.cpp (ScriptManager DC Script Resolution) */
    { "ScriptManager_LookupPointer",                     0x00A15E70, reinterpret_cast<uintptr_t>(&u2_cpp_ScriptManager_LookupPointer) },
    { "ScriptManager_LookupPointerInModule",             0x00A17280, reinterpret_cast<uintptr_t>(&u2_cpp_ScriptManager_LookupPointerInModule) },

    /* audio_system.cpp (audiofile.cpp) */
    { "AudioFile_Initialize",                           0x003CE4A0, reinterpret_cast<uintptr_t>(&u2_cpp_AudioFile_Initialize) },
};

extern "C" size_t u2_get_engine_detours(const U2NativeDetour **out) {
    if (out) *out = s_engine_detours;
    return sizeof(s_engine_detours) / sizeof(s_engine_detours[0]);
}
