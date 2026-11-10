// Uncharted 2: Among Thieves — Native C++ Engine Subsystems
// Clean C++23 reimplementation of Naughty Dog engine classes (src/game/...)
// isolated from the DC script runtime (src/runtime_u2_*.c).
#pragma once

#include "../runtime_u2_common.h"
#include <cstdint>
#include <cstddef>
#include <cstring>

namespace u2::engine {

/* Helper to resolve an offset relative to the loaded PS4 ELF image */
template <typename T = uint8_t>
inline T *elf_ptr(uintptr_t offset) noexcept {
    return reinterpret_cast<T *>(g_u2_image_base + offset);
}

template <typename Fn>
inline Fn elf_fn(uintptr_t offset) noexcept {
    return reinterpret_cast<Fn>(g_u2_image_base + offset);
}

struct ControlClickedHandler {
    uint32_t id{0};
    void (ABI *callback)(void *, uint32_t){nullptr};
    void *user_data{nullptr};
};

struct MenuHostState {
    int32_t mode{0}; // 0 = Hidden, 1 = Main Menu, 2 = InGame Pause, 3 = Loading Popup
    int32_t flags{0};
    int32_t selected_item{0};
    int32_t popup_id{0};
    int32_t popup_flags{0};
    uint32_t num_handlers{0};
    ControlClickedHandler handlers[128]{};
};

extern MenuHostState g_menu_host;

/* Memory subsystem helpers */
void *get_host_allocator() noexcept;
void  ensure_root_allocator() noexcept;
void *allocate_from_current_or_host(size_t size, size_t alignment = 16, const char *name = nullptr) noexcept;

} // namespace u2::engine

/* C++ Engine Detour Declarations (SysV x86-64 ABI) */
extern "C" {

/* file_loader.cpp (streaming.cpp, file-loader.cpp) */
bool         ABI u2_cpp_LoadFileHelper(void *file_data_out, const char *path);
void         ABI u2_cpp_Streaming_ResolvePath(const char *original_path, char *resolved_out);

/* game/weapon/tranquilizer.cpp */
void         ABI u2_cpp_Tranquilizer_Init(void);

/* catmull_rom.cpp (catmull-rom.cpp, catmull-rom-manager.cpp, spline-tracker.cpp, vehicle-spline-tracker.cpp) */
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

/* camera_zoom.cpp (camerazoom.cpp) */
void         ABI u2_cpp_CameraZoom_OverideHorizontalAngularConstraints(const float *dir, float min_a, float max_a);
void         ABI u2_cpp_CameraZoom_ClearHorizontalAngularConstraints(void);
void         ABI u2_cpp_CameraZoom_OverideVerticalAngularConstraints(float min_a, float max_a);
void         ABI u2_cpp_CameraZoom_ClearVerticalAngularConstraints(void);
void         ABI u2_cpp_CameraZoom_SetDefaultVerticalAngularConstraints(float min_a, float max_a);

/* rail_vehicle.cpp (rail-vehicle.cpp, rail-vehicle-behavior.cpp, rail-vehicle-manager.cpp) */
void         ABI u2_cpp_RailVehicle_InitBehavior(void *self, void *dc_behavior);
void         ABI u2_cpp_RailVehicle_SetFollowDistance(void *self, float begin_dist, float end_dist);
void *       ABI u2_cpp_RailVehicle_GetVehicleClass(void *self);
void         ABI u2_cpp_RailVehicle_ResetWheels(void *self);
void *       ABI u2_cpp_RailVehicle_GetDamageable(void *self, uint32_t index);
void         ABI u2_cpp_RailVehicle_OnIgcBind(void *self);
void         ABI u2_cpp_RailVehicle_OnIgcUnbind(void *self);

void *       ABI u2_cpp_RailVehicleBehavior_GetDcBehavior(void *self);
void *       ABI u2_cpp_RailVehicleBehaviorContainer_GetCurrentBehavior(void *self);

void *       ABI u2_cpp_RailVehicleManager_GetMover(void *self, uint32_t idx);
void         ABI u2_cpp_RailVehicleManager_DetachFromRail(void *self, void *vehicle, int32_t rail_idx);

/* process_system.cpp (process.cpp, process-mgr.cpp, processlist.cpp) */
void *       ABI u2_cpp_ProcessMgr_LookupProcessByPid(void *mgr, uint32_t pid);
void *       ABI u2_cpp_Process_LookupProcessByPid(void *root, uint32_t pid);
void         ABI u2_cpp_Process_ValidateInternalPointer(void *self);
void         ABI u2_cpp_ProcessMgr_AddProcessForScriptUpdate(void *mgr, void *proc);
void *       ABI u2_cpp_ProcessList_Alloc(void *proc);
uint8_t      ABI u2_cpp_Process_DMenuStateProp(void *self, int32_t cmd);
float        ABI u2_cpp_Process_DMenuFloatProp_354(void *self, int32_t cmd, float val);
uint8_t      ABI u2_cpp_Process_DMenuBoolProp_43A(void *self, int32_t cmd);
uint8_t      ABI u2_cpp_Process_DMenuBoolProp_43F(void *self, int32_t cmd);
uint8_t      ABI u2_cpp_Process_DMenuBoolProp_439(void *self, int32_t cmd);
uint8_t      ABI u2_cpp_Process_DMenuBoolProp_438(void *self, int32_t cmd);
uint8_t      ABI u2_cpp_Process_DMenuBoolProp_43B(void *self, int32_t cmd);

/* save_hud.cpp (savebuffer.cpp, hud.cpp, task.cpp, settings.cpp, game-loading.cpp) */
void         ABI u2_cpp_SaveBuffer_InitExternal(void *self, uint32_t magic, uint32_t size, void *buf);
void         ABI u2_cpp_SaveBuffer_DebugSetBuffer(void *self, void *buf);
void         ABI u2_cpp_SaveBuffer_CopyFrom(void *self, const void *src);
void         ABI u2_cpp_SaveBuffer_WriteBytes(void *self, const void *data, uint32_t size);
void         ABI u2_cpp_SaveBuffer_WriteU64Count0(void *self, const uint64_t *val);
void         ABI u2_cpp_SaveBuffer_WriteU64Count1(void *self, const uint64_t *val);
void         ABI u2_cpp_Hud_FreeGrenadeIndicator(void *self, uint32_t idx);
void         ABI u2_cpp_Hud_ShowGrenadeIndicator(void *self, uint32_t idx, uint8_t show);
int32_t      ABI u2_cpp_GameTask_cbCollectTaskNode(void *task, uint32_t key, void *collection);
void         ABI u2_cpp_Settings_FreeSetting(void *setting);
uint8_t      ABI u2_cpp_GameLoading_IsChunkSetEntirelyWithin(const char *chunk_set_name, const uint64_t *target_mask);
uint32_t     ABI u2_cpp_GameLoading_GetPlayGoSpeed(void);

/* memory_system.cpp (memory.cpp, process-heap.cpp) */
void *       ABI u2_cpp_AllocateAllocStack(void);
void         ABI u2_cpp_FreeThreadAllocStack(void);
void         ABI u2_cpp_PushAllocator(void *allocator, const char *name, int32_t line, const char *file);
void         ABI u2_cpp_PushAllocator_Tag(void *tag_ptr, const char *name, int32_t line, const char *file);
void         ABI u2_cpp_PushAllocator_Ext(void *scope, const char *name, int32_t line, const char *file);
void *       ABI u2_cpp_PopAllocator(void);
void *       ABI u2_cpp_PopAllocator_Expected(void *scope);
uint32_t     ABI u2_cpp_GetAllocatorStackDepth(void);
void         ABI u2_cpp_ProcessHeap_ScanForProcessDroppings(void *self);

/* boot_state_machine.cpp (settings.cpp, processlist.cpp, process-mgr.cpp, game-loading.cpp, uccapp.cpp, introstate.cpp) */
void         ABI u2_cpp_Settings_Init(void);
void *       ABI u2_cpp_Settings_NewSetting(void *self, uint64_t key, const char *name, uint32_t type, void *parent);
void         ABI u2_cpp_ProcessList_StartUp(void);
void         ABI u2_cpp_ProcessMgr_Init(void *mgr);
uint32_t     ABI u2_cpp_GameLoading_MountInitialArchives(void);
uint32_t     ABI u2_cpp_GameLoading_PlayGoWait(int32_t timeout_ms);
int32_t      ABI u2_cpp_IntroState_Update(void *self);
int32_t      ABI u2_cpp_UCCApp_StateMachineUpdate(void *self, float dt);

/* audio_system.cpp (audiofile.cpp) */
void         ABI u2_cpp_AudioFile_Initialize(void *self);

/* menu_system.cpp (menuhost.cpp, menudrawer.cpp, hud-quad.cpp, bp_menudatasource.cpp) */
void         ABI u2_cpp_MenuHost_SetMode(void *self, int32_t mode, int32_t flags);
int32_t      ABI u2_cpp_MenuHost_GetMode(void *self);
void         ABI u2_cpp_MenuHost_ShowWaitPopup(void *self, int32_t popup_id, int32_t flags);
void         ABI u2_cpp_MenuHost_AddControlClickedHandler(void *self, uint32_t id, void (ABI *cb)(void *, uint32_t), void *user_data);
void         ABI u2_cpp_MenuHost_TriggerControlClicked(void *self, uint32_t id);
int32_t      ABI u2_cpp_BpMenuDataSource_SetValueImpl(void *self, int32_t val_id, const void *val_data);
void         ABI u2_cpp_MenuDrawer_DrawQuad(void *self, const float *dest_rect, void *texture, const float *src_uv_rect);
void *       ABI u2_cpp_HudQuad_Begin_Actual(void *self, void *render_target, void *pipeline);
void         ABI u2_cpp_HudQuad_End_Actual(void *self, void *cmd_buf, const float *params);

} // extern "C"
