// Uncharted 2: Among Thieves — Menu Subsystems (Phurba Dagger & Tibetan Temple)
// Clean C++23 Reimplementation (game/menu/)
#pragma once

#include <cstdint>

#ifndef ABI
#if defined(_WIN32)
#define ABI __attribute__((sysv_abi))
#else
#define ABI
#endif
#endif

namespace u2::engine {

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

} // namespace u2::engine

extern "C" {
void     ABI u2_cpp_MenuHost_SetMode(void *self, int32_t mode, int32_t flags);
int32_t  ABI u2_cpp_MenuHost_GetMode(void *self);
void     ABI u2_cpp_MenuHost_ShowWaitPopup(void *self, int32_t popup_id, int32_t flags);
void     ABI u2_cpp_MenuHost_AddControlClickedHandler(void *self, uint32_t id, void (ABI *cb)(void *, uint32_t), void *user_data);
void     ABI u2_cpp_MenuHost_TriggerControlClicked(void *self, uint32_t id);
int32_t  ABI u2_cpp_BpMenuDataSource_SetValueImpl(void *self, int32_t val_id, const void *val_data);
void     ABI u2_cpp_MenuDrawer_DrawQuad(void *self, const float *dest_rect, void *texture, const float *src_uv_rect);
void *   ABI u2_cpp_HudQuad_Begin_Actual(void *self, void *render_target, void *pipeline);
void     ABI u2_cpp_HudQuad_End_Actual(void *self, void *cmd_buf, const float *params);
}
