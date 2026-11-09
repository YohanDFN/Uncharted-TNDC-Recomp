// Uncharted 2: Among Thieves — Process, ProcessMgr & ProcessList Subsystem (C++23)
// Clean C++23 reimplementation of:
//   - src/game/process/process.cpp
//   - src/game/process/process-mgr.cpp
//   - src/game/processlist.cpp

#include "u2_engine.h"

namespace u2::engine {

namespace {

inline uint8_t *lookup_process_in_tree(void *root, uint32_t pid) noexcept {
    if (!g_u2_image_base || !root || reinterpret_cast<uintptr_t>(root) < 0x10000 || pid == 0) {
        return nullptr;
    }
    auto fn_next = elf_fn<uint8_t *(ABI *)(void *, void *)>(0x009BCDF0);
    uint8_t *cur = static_cast<uint8_t *>(root);
    while (cur && reinterpret_cast<uintptr_t>(cur) >= 0x10000) {
        uint16_t flags = *reinterpret_cast<const uint16_t *>(cur + 0x34);
        if ((flags & 1u) != 0 && *reinterpret_cast<const uint32_t *>(cur + 0x44) == pid) {
            return cur;
        }
        cur = fn_next(cur, root);
    }
    return nullptr;
}

inline uint8_t *lookup_process_in_mgr(void *mgr, uint32_t pid) noexcept {
    if (!g_u2_image_base || !mgr || reinterpret_cast<uintptr_t>(mgr) < 0x10000 || pid == 0) {
        return nullptr;
    }
    void *head = *reinterpret_cast<void * const *>(static_cast<const uint8_t *>(mgr) + 0xC40);
    return lookup_process_in_tree(head, pid);
}

} // namespace

} // namespace u2::engine

extern "C" {

/* 0x009BBCF0 — ProcessMgr::LookupProcessByPid(void *mgr, uint32_t pid) */
void * ABI u2_cpp_ProcessMgr_LookupProcessByPid(void *mgr, uint32_t pid) {
    return u2::engine::lookup_process_in_mgr(mgr, pid);
}

/* 0x009BEF30 — Process::LookupProcessByPid(void *root, uint32_t pid) */
void * ABI u2_cpp_Process_LookupProcessByPid(void *root, uint32_t pid) {
    return u2::engine::lookup_process_in_tree(root, pid);
}

/* 0x009BE660 — Process::ValidateInternalPointer(void *self)
 * Replaces debug trap int $0x41 with safe pointer validation on Process::m_pState (+0x50).
 */
void ABI u2_cpp_Process_ValidateInternalPointer(void *self) {
    if (!g_u2_image_base || !self || reinterpret_cast<uintptr_t>(self) < 0x10000) return;
    const auto *s = static_cast<const uint8_t *>(self);
    const uint8_t *state = *reinterpret_cast<const uint8_t * const *>(s + 0x50);
    if (!state || reinterpret_cast<uintptr_t>(state) < 0x10000) return;

    uintptr_t internal_ptr = *reinterpret_cast<const uintptr_t *>(state + 0x20);
    if (!internal_ptr) return;

    const uint8_t *default_heap = *u2::engine::elf_ptr<const uint8_t *>(0x0115F198);
    if (default_heap && reinterpret_cast<uintptr_t>(default_heap) >= 0x10000) {
        uintptr_t heap_begin = *reinterpret_cast<const uintptr_t *>(default_heap + 0xD8);
        uintptr_t heap_end   = *reinterpret_cast<const uintptr_t *>(default_heap + 0xE0);
        if (internal_ptr >= heap_begin && internal_ptr < heap_end) {
            return;
        }
    }
}

/* 0x009B8CD0 — ProcessMgr::AddProcessForScriptUpdate(void *mgr, void *proc) */
void ABI u2_cpp_ProcessMgr_AddProcessForScriptUpdate(void *mgr, void *proc) {
    if (!mgr || reinterpret_cast<uintptr_t>(mgr) < 0x10000) return;
    auto *m = static_cast<uint8_t *>(mgr);
    uint32_t count = *reinterpret_cast<const uint32_t *>(m + 0xC10);
    if (count < 0x180u) {
        *reinterpret_cast<void **>(m + 0x10 + static_cast<size_t>(count) * 8) = proc;
        *reinterpret_cast<uint32_t *>(m + 0xC10) = count + 1u;
    }
}

/* 0x009B0D70 — ProcessList::Alloc(void *proc) */
void * ABI u2_cpp_ProcessList_Alloc(void *proc) {
    if (!g_u2_image_base) return nullptr;
    uint8_t *pool = *u2::engine::elf_ptr<uint8_t *>(0x0151FF70);
    if (!pool || reinterpret_cast<uintptr_t>(pool) < 0x10000) return nullptr;

    uint8_t *node = *reinterpret_cast<uint8_t **>(pool + 0x20);
    if (!node || reinterpret_cast<uintptr_t>(node) < 0x10000) return nullptr;

    uint8_t *next = *reinterpret_cast<uint8_t **>(node + 0x00);
    uint8_t *prev = *reinterpret_cast<uint8_t **>(node + 0x08);
    if (prev && reinterpret_cast<uintptr_t>(prev) >= 0x10000) {
        *reinterpret_cast<uint8_t **>(prev + 0x00) = next;
    }
    if (next && reinterpret_cast<uintptr_t>(next) >= 0x10000) {
        *reinterpret_cast<uint8_t **>(next + 0x08) = prev;
    }
    if (*reinterpret_cast<uint8_t **>(pool + 0x20) == node) {
        *reinterpret_cast<uint8_t **>(pool + 0x20) = next;
    }

    *reinterpret_cast<void **>(node + 0x00) = nullptr;
    *reinterpret_cast<void **>(node + 0x08) = nullptr;

    if (proc && reinterpret_cast<uintptr_t>(proc) >= 0x10000) {
        const auto *p = static_cast<const uint8_t *>(proc);
        *reinterpret_cast<void **>(node + 0x10) = *reinterpret_cast<void * const *>(p + 0x38);
        *reinterpret_cast<uint32_t *>(node + 0x18) = *reinterpret_cast<const uint32_t *>(p + 0x44);
    } else {
        *reinterpret_cast<void **>(node + 0x10) = nullptr;
        *reinterpret_cast<uint32_t *>(node + 0x18) = 0u;
    }
    return node;
}

/* 0x001FE420 — Process::DMenuStateProp(void *self, int32_t cmd) */
uint8_t ABI u2_cpp_Process_DMenuStateProp(void *self, int32_t cmd) {
    if (!g_u2_image_base || !self || reinterpret_cast<uintptr_t>(self) < 0x10000) return 0;
    auto *s = static_cast<uint8_t *>(self);
    uint32_t pid = *reinterpret_cast<const uint32_t *>(s + 0x38);
    uint8_t *proc = u2::engine::lookup_process_in_mgr(u2::engine::elf_ptr<void>(0x0115F1A0), pid);
    if (proc) {
        if (cmd == 3) {
            void *vtbl = *reinterpret_cast<void **>(proc);
            if (vtbl && reinterpret_cast<uintptr_t>(vtbl) >= 0x10000) {
                auto fn_get_ctrl = *reinterpret_cast<void *(ABI **)(void *)>(static_cast<uint8_t *>(vtbl) + 0x188);
                void *ctrl = fn_get_ctrl ? fn_get_ctrl(proc) : nullptr;
                if (ctrl && reinterpret_cast<uintptr_t>(ctrl) >= 0x10000) {
                    void *ctrl_vtbl = *reinterpret_cast<void **>(ctrl);
                    auto fn_get_state = *reinterpret_cast<uint32_t (ABI **)(void *)>(static_cast<uint8_t *>(ctrl_vtbl) + 0x48);
                    if (fn_get_state) {
                        uint32_t cur_state = fn_get_state(ctrl);
                        s[0x78] = (cur_state == *reinterpret_cast<const uint32_t *>(s + 0x30)) ? 1u : 0u;
                    }
                }
            }
        } else if (cmd == 4) {
            void *vtbl = *reinterpret_cast<void **>(proc);
            if (vtbl && reinterpret_cast<uintptr_t>(vtbl) >= 0x10000) {
                auto fn_get_ctrl = *reinterpret_cast<void *(ABI **)(void *)>(static_cast<uint8_t *>(vtbl) + 0x188);
                void *ctrl = fn_get_ctrl ? fn_get_ctrl(proc) : nullptr;
                if (ctrl && reinterpret_cast<uintptr_t>(ctrl) >= 0x10000) {
                    void *ctrl_vtbl = *reinterpret_cast<void **>(ctrl);
                    auto fn_set_state = *reinterpret_cast<void (ABI **)(void *, uint32_t)>(static_cast<uint8_t *>(ctrl_vtbl) + 0x38);
                    if (fn_set_state) {
                        fn_set_state(ctrl, *reinterpret_cast<const uint32_t *>(s + 0x30));
                    }
                }
            }
        }
    }
    return s[0x78];
}

/* 0x001FE7E0 — Process::DMenuFloatProp_354(void *self, int32_t cmd, float val) */
float ABI u2_cpp_Process_DMenuFloatProp_354(void *self, int32_t cmd, float val) {
    if (!g_u2_image_base || !self || reinterpret_cast<uintptr_t>(self) < 0x10000) return 0.0f;
    uint32_t pid = *reinterpret_cast<const uint32_t *>(static_cast<const uint8_t *>(self) + 0x38);
    uint8_t *proc = u2::engine::lookup_process_in_mgr(u2::engine::elf_ptr<void>(0x0115F1A0), pid);
    if (!proc) return 0.0f;
    if (cmd == 4) {
        *reinterpret_cast<float *>(proc + 0x354) = val;
        return val;
    }
    return *reinterpret_cast<const float *>(proc + 0x354);
}

/* 0x001FE8C0 — Process::DMenuBoolProp_43A(void *self, int32_t cmd) */
uint8_t ABI u2_cpp_Process_DMenuBoolProp_43A(void *self, int32_t cmd) {
    if (!g_u2_image_base || !self || reinterpret_cast<uintptr_t>(self) < 0x10000) return 0;
    uint32_t pid = *reinterpret_cast<const uint32_t *>(static_cast<const uint8_t *>(self) + 0x38);
    uint8_t *proc = u2::engine::lookup_process_in_mgr(u2::engine::elf_ptr<void>(0x0115F1A0), pid);
    if (!proc) return 0;
    uint8_t v = proc[0x43A];
    if (cmd == 4) {
        v ^= 1u;
        proc[0x43A] = v;
    }
    return (v != 0) ? 1u : 0u;
}

/* 0x001FE980 — Process::DMenuBoolProp_43F(void *self, int32_t cmd) */
uint8_t ABI u2_cpp_Process_DMenuBoolProp_43F(void *self, int32_t cmd) {
    if (!g_u2_image_base || !self || reinterpret_cast<uintptr_t>(self) < 0x10000) return 0;
    uint32_t pid = *reinterpret_cast<const uint32_t *>(static_cast<const uint8_t *>(self) + 0x38);
    uint8_t *proc = u2::engine::lookup_process_in_mgr(u2::engine::elf_ptr<void>(0x0115F1A0), pid);
    if (!proc) return 0;
    if (cmd == 4) {
        proc[0x43F] ^= 1u;
        auto fn_sync = u2::engine::elf_fn<void (ABI *)(void *, uint32_t)>(0x001FEA50);
        fn_sync(proc, 0u);
    }
    return (proc[0x43F] != 0) ? 1u : 0u;
}

/* 0x001FECA0 — Process::DMenuBoolProp_439(void *self, int32_t cmd) */
uint8_t ABI u2_cpp_Process_DMenuBoolProp_439(void *self, int32_t cmd) {
    if (!g_u2_image_base || !self || reinterpret_cast<uintptr_t>(self) < 0x10000) return 0;
    uint32_t pid = *reinterpret_cast<const uint32_t *>(static_cast<const uint8_t *>(self) + 0x38);
    uint8_t *proc = u2::engine::lookup_process_in_mgr(u2::engine::elf_ptr<void>(0x0115F1A0), pid);
    if (!proc) return 0;
    if (cmd == 4) {
        proc[0x439] ^= 1u;
        auto fn_sync = u2::engine::elf_fn<void (ABI *)(void *, uint32_t)>(0x001FEA50);
        fn_sync(proc, 0u);
    }
    return (proc[0x439] != 0) ? 1u : 0u;
}

/* 0x001FED70 — Process::DMenuBoolProp_438(void *self, int32_t cmd) */
uint8_t ABI u2_cpp_Process_DMenuBoolProp_438(void *self, int32_t cmd) {
    if (!g_u2_image_base || !self || reinterpret_cast<uintptr_t>(self) < 0x10000) return 0;
    uint32_t pid = *reinterpret_cast<const uint32_t *>(static_cast<const uint8_t *>(self) + 0x38);
    uint8_t *proc = u2::engine::lookup_process_in_mgr(u2::engine::elf_ptr<void>(0x0115F1A0), pid);
    if (!proc) return 0;
    if (cmd == 4) {
        proc[0x438] ^= 1u;
        auto fn_sync = u2::engine::elf_fn<void (ABI *)(void *, uint32_t)>(0x001FEA50);
        fn_sync(proc, 0u);
    }
    return (proc[0x438] != 0) ? 1u : 0u;
}

/* 0x001FEE40 — Process::DMenuBoolProp_43B(void *self, int32_t cmd) */
uint8_t ABI u2_cpp_Process_DMenuBoolProp_43B(void *self, int32_t cmd) {
    if (!g_u2_image_base || !self || reinterpret_cast<uintptr_t>(self) < 0x10000) return 0;
    uint32_t pid = *reinterpret_cast<const uint32_t *>(static_cast<const uint8_t *>(self) + 0x38);
    uint8_t *proc = u2::engine::lookup_process_in_mgr(u2::engine::elf_ptr<void>(0x0115F1A0), pid);
    if (!proc) return 0;
    uint8_t v = proc[0x43B];
    if (cmd == 4) {
        v ^= 1u;
        proc[0x43B] = v;
    }
    return (v != 0) ? 1u : 0u;
}

} // extern "C"
