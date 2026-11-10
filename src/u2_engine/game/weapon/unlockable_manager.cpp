// Uncharted 2: Among Thieves — UnlockableManager Subsystem
// Clean C++23 Reimplementation of big2-ps4/src/game/unlockablemanager.cpp
#include "u2_engine.h"
#include "game/weapon/unlockable_manager.h"

extern "C" {

/* 0x00A75450 — UnlockableManager::SetDefaultWeapon(void *self, uint8_t flag)
 * In original PS4 code, if ScriptManager::LookupPointer(0x74BFA7D0) returns NULL,
 * line 0x00A75494 crashes by dereferencing NULL: movl (%r13), %eax without check.
 * Our detour adds the missing NULL check and safely completes the setup.
 */
void ABI u2_cpp_UnlockableManager_SetDefaultWeapon(void *self, uint8_t flag) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000) return;
    auto *mgr = static_cast<uint8_t *>(self);

    if (flag != 0) {
        *reinterpret_cast<void **>(mgr + 0x00) = nullptr;
        *reinterpret_cast<uint32_t *>(mgr + 0x28) = 1;

        // Lookup symbol 0x74BFA7D0 in ScriptManager
        void *weapons_table = nullptr;
        if (g_u2_image_base) {
            auto lookup_fn = u2::engine::elf_fn<void *(ABI *)(uint32_t)>(0x00A15E70);
            if (lookup_fn) {
                weapons_table = lookup_fn(0x74BFA7D0u);
            }
        }

        if (weapons_table && reinterpret_cast<uintptr_t>(weapons_table) >= 0x10000) {
            auto *wt = static_cast<uint8_t *>(weapons_table);
            uint32_t count = *reinterpret_cast<const uint32_t *>(wt + 0x00);
            uint8_t *items_array = *reinterpret_cast<uint8_t **>(wt + 0x08);
            if (count > 0 && items_array && reinterpret_cast<uintptr_t>(items_array) >= 0x10000) {
                auto add_fn = u2::engine::elf_fn<void (ABI *)(void *, void *, uint32_t)>(0x0056D430);
                uint8_t *weapon_dict = u2::engine::elf_ptr<uint8_t>(0x01130390);
                for (uint32_t i = 0; i < count; ++i) {
                    uint8_t *item = items_array + (i * 0x20);
                    if (*reinterpret_cast<uint32_t *>(item + 0x10) == 0) {
                        void *str_or_entry = *reinterpret_cast<void **>(item + 0x00);
                        if (str_or_entry && add_fn) {
                            add_fn(weapon_dict, str_or_entry, 0);
                        }
                    }
                }
            }
        }
    }

    // Common epilogue (0x00A754DC)
    *reinterpret_cast<uint32_t *>(mgr + 0x4C) = 0;
    if (g_u2_image_base) {
        uint8_t *p1248 = u2::engine::elf_ptr<uint8_t>(0x012483F4);
        if (p1248) {
            p1248[0x04] = 0;
            p1248[0x06] = 0;
        }
        uint8_t *p1111 = u2::engine::elf_ptr<uint8_t>(0x01111750);
        if (p1111 && p1111[0x08] != 0) {
            p1111[0x0F] = 0;
        }
    }
}

} // extern "C"
