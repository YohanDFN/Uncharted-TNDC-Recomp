// Uncharted 2: Among Thieves — Native C++ Engine Umbrella Header
// Aggregates modular subsystem headers (ndlib/ and game/)
#pragma once

#include "../runtime_u2_common.h"
#include <cstdint>
#include <cstddef>
#include <cstring>

// Subsystem Headers: ndlib/ (Core Engine)
#include "ndlib/memory/alloc_stack.h"
#include "ndlib/io/streaming.h"
#include "ndlib/io/package_manager.h"
#include "ndlib/anim/spline.h"
#include "ndlib/audio/audio_driver.h"

// Subsystem Headers: game/ (Gameplay & UI)
#include "game/camera/camera_zoom.h"
#include "game/vehicle/rail_vehicle.h"
#include "game/menu/main_menu.h"
#include "game/loading/game_loading.h"
#include "game/hud/save_hud.h"
#include "game/weapon/tranquilizer.h"
#include "game/weapon/unlockable_manager.h"
#include "game/script/process_system.h"
#include "game/script/script_manager.h"

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

} // namespace u2::engine
