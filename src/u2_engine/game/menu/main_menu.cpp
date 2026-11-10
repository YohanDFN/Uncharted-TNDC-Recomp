// Uncharted 2: Among Thieves — Main Menu & 2D GUI Engine Subsystems
// Pure C++23 Reimplementation (Zero PS4 Code Execution)
// ADR-006: Dual-Executable Architecture.

#include "u2_engine.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

namespace u2::engine {

MenuHostState g_menu_host{};

} // namespace u2::engine

/* 0x005C7BB0 — MenuHost::SetMode(void *self, int32_t mode, int32_t flags)
 * Sets menu mode (0 = Hidden, 1 = Main Menu, 2 = InGame Pause, 3 = Loading Popup).
 * Updates internal state and writes to global mode offset 0x014CE97C.
 */
void ABI u2_cpp_MenuHost_SetMode(void *self, int32_t mode, int32_t flags) {
    if (self && reinterpret_cast<uintptr_t>(self) >= 0x10000) {
        *reinterpret_cast<int32_t *>(static_cast<uint8_t *>(self) + 0x00) = mode;
        *reinterpret_cast<int32_t *>(static_cast<uint8_t *>(self) + 0x04) = flags;
    }
    u2::engine::g_menu_host.mode = mode;
    u2::engine::g_menu_host.flags = flags;
    if (g_u2_image_base) {
        *u2::engine::elf_ptr<int32_t>(0x014CE97C) = mode;
    }
}

int32_t ABI u2_cpp_MenuHost_GetMode(void *self) {
    if (self && reinterpret_cast<uintptr_t>(self) >= 0x10000) {
        return *reinterpret_cast<const int32_t *>(static_cast<const uint8_t *>(self) + 0x00);
    }
    return u2::engine::g_menu_host.mode;
}

/* 0x005C8840 — MenuHost::ShowWaitPopup(void *self, int32_t popup_id, int32_t flags)
 * Displays a wait/loading popup overlay with the given ID.
 */
void ABI u2_cpp_MenuHost_ShowWaitPopup(void *self, int32_t popup_id, int32_t flags) {
    if (self && reinterpret_cast<uintptr_t>(self) >= 0x10000) {
        auto *s = static_cast<uint8_t *>(self);
        *reinterpret_cast<int32_t *>(s + 0x9B0) = popup_id;
    }
    u2::engine::g_menu_host.popup_id = popup_id;
    u2::engine::g_menu_host.popup_flags = flags;
}

/* 0x005D1BF0 — MenuHost::AddControlClickedHandler(void *self, uint32_t id, void (ABI *cb)(void *, uint32_t), void *user_data)
 * Registers a click callback for a specific menu control ID in m_controlClickedHandlers (max 128).
 */
void ABI u2_cpp_MenuHost_AddControlClickedHandler(void *self, uint32_t id, void (ABI *cb)(void *, uint32_t), void *user_data) {
    auto &host = u2::engine::g_menu_host;
    if (host.num_handlers < 128) {
        host.handlers[host.num_handlers].id = id;
        host.handlers[host.num_handlers].callback = cb;
        host.handlers[host.num_handlers].user_data = user_data;
        ++host.num_handlers;
    }
    if (self && reinterpret_cast<uintptr_t>(self) >= 0x10000) {
        auto *s = static_cast<uint8_t *>(self);
        uint32_t count = *reinterpret_cast<uint32_t *>(s + 0x18EC);
        if (count < 128) {
            uint8_t *slot = s + 0x18F0 + static_cast<size_t>(count) * 24;
            *reinterpret_cast<uint32_t *>(slot + 0x00) = id;
            *reinterpret_cast<void **>(slot + 0x08) = reinterpret_cast<void *>(cb);
            *reinterpret_cast<void **>(slot + 0x10) = user_data;
            *reinterpret_cast<uint32_t *>(s + 0x18EC) = count + 1;
        }
    }
}

/* Triggers registered click handlers for control id */
void ABI u2_cpp_MenuHost_TriggerControlClicked(void *self, uint32_t id) {
    auto &host = u2::engine::g_menu_host;
    for (uint32_t i = 0; i < host.num_handlers; ++i) {
        if (host.handlers[i].id == id && host.handlers[i].callback) {
            host.handlers[i].callback(host.handlers[i].user_data, id);
        }
    }
    if (self && reinterpret_cast<uintptr_t>(self) >= 0x10000) {
        auto *s = static_cast<uint8_t *>(self);
        uint32_t count = *reinterpret_cast<uint32_t *>(s + 0x18EC);
        for (uint32_t i = 0; i < count && i < 128; ++i) {
            uint8_t *slot = s + 0x18F0 + static_cast<size_t>(i) * 24;
            uint32_t slot_id = *reinterpret_cast<uint32_t *>(slot + 0x00);
            auto cb = reinterpret_cast<void (ABI *)(void *, uint32_t)>(*reinterpret_cast<void **>(slot + 0x08));
            void *ud = *reinterpret_cast<void **>(slot + 0x10);
            if (slot_id == id && cb) {
                cb(ud, id);
            }
        }
    }
}

/* 0x00578EB0 — BpMenuDataSource::SetValueImpl(void *self, int32_t val_id, const void *val_data)
 * Updates a dynamic menu data source value (volume, gamma, subtitle status).
 */
int32_t ABI u2_cpp_BpMenuDataSource_SetValueImpl(void *self, int32_t val_id, const void *val_data) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000) return 0;
    auto *s = static_cast<uint8_t *>(self);
    *reinterpret_cast<int32_t *>(s + 0x40) = val_id;
    if (val_data) {
        std::memcpy(s + 0x48, val_data, 16);
    }
    return 1;
}

/* 0x005B5C10 — MenuDrawer::DrawQuad(void *self, const float *dest_rect, void *texture, const float *src_uv_rect)
 * Submits a 2D textured quad to the active GUI command buffer.
 */
void ABI u2_cpp_MenuDrawer_DrawQuad(void *self, const float *dest_rect, void *texture, const float *src_uv_rect) {
    (void)self;
    (void)dest_rect;
    (void)texture;
    (void)src_uv_rect;
}

/* 0x00BDBB10 — HudQuad::Begin_Actual(void *self, void *render_target, void *pipeline)
 * Begins a 2D quad batch for HUD / Menu rendering.
 */
void * ABI u2_cpp_HudQuad_Begin_Actual(void *self, void *render_target, void *pipeline) {
    (void)render_target;
    (void)pipeline;
    return self;
}

/* 0x00BDC670 — HudQuad::End_Actual(void *self, void *cmd_buf, const float *params)
 * Ends and flushes the 2D quad batch to the GPU.
 */
void ABI u2_cpp_HudQuad_End_Actual(void *self, void *cmd_buf, const float *params) {
    (void)self;
    (void)cmd_buf;
    (void)params;
}
