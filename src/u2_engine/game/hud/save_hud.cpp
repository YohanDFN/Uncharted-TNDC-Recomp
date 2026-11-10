// Uncharted 2: Among Thieves — Boot, SaveBuffer, Settings, TaskGraph, PlayGo & HUD Subsystems (C++23)
// Clean C++23 reimplementation of:
//   - src/ndlib/save/savebuffer.cpp
//   - src/game/hud/hud.cpp
//   - src/game/tasks/task.cpp
//   - src/game/settings.cpp
//   - src/ndlib/level/game-loading.cpp

#include "u2_engine.h"

extern "C" {

/* 0x00CD0E10 — SaveBuffer::SaveBuffer(void *self, uint32_t magic, uint32_t size, void *buf) */
void ABI u2_cpp_SaveBuffer_InitExternal(void *self, uint32_t magic, uint32_t size, void *buf) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000) return;
    auto *s = static_cast<uint8_t *>(self);
    *reinterpret_cast<uint32_t *>(s + 0x00) = magic;
    *reinterpret_cast<uint32_t *>(s + 0x04) = size;
    *reinterpret_cast<void **>(s + 0x08)    = buf;
    s[0x18]                                 = 0u;
}

/* 0x00CD0E80 — SaveBuffer::DebugSetBuffer(void *self, void *buf) */
void ABI u2_cpp_SaveBuffer_DebugSetBuffer(void *self, void *buf) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000) return;
    auto *s = static_cast<uint8_t *>(self);
    *reinterpret_cast<void **>(s + 0x08) = buf;
    *reinterpret_cast<void **>(s + 0x10) = buf;
}

/* 0x00CD0EE0 — SaveBuffer::CopyFrom(void *self, const void *src) */
void ABI u2_cpp_SaveBuffer_CopyFrom(void *self, const void *src) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000 ||
        !src  || reinterpret_cast<uintptr_t>(src)  < 0x10000) {
        return;
    }
    auto *s = static_cast<uint8_t *>(self);
    const auto *other = static_cast<const uint8_t *>(src);

    uint8_t *dst_buf = *reinterpret_cast<uint8_t **>(s + 0x08);
    uint32_t dst_size = *reinterpret_cast<const uint32_t *>(s + 0x04);
    if (!dst_buf || reinterpret_cast<uintptr_t>(dst_buf) < 0x10000) return;

    std::memset(dst_buf, 0, dst_size);
    *reinterpret_cast<uint8_t **>(s + 0x10) = dst_buf;
    *reinterpret_cast<uint8_t **>(s + 0x98) = dst_buf + 0x414;

    if (dst_size >= 0x0C) {
        *reinterpret_cast<uint32_t *>(dst_buf + 0x08) = *reinterpret_cast<const uint32_t *>(s + 0x00);
        *reinterpret_cast<uint32_t *>(dst_buf + 0x04) = 0u;
        *reinterpret_cast<uint32_t *>(dst_buf + 0x00) = 0u;
    }

    *reinterpret_cast<uint32_t *>(s + 0x00) = *reinterpret_cast<const uint32_t *>(other + 0x00);
    const void *src_buf = *reinterpret_cast<void * const *>(other + 0x08);
    if (src_buf && reinterpret_cast<uintptr_t>(src_buf) >= 0x10000 && dst_size > 0) {
        std::memcpy(dst_buf, src_buf, dst_size);
    }
}

/* 0x00CD16A0 — SaveBuffer::Write(void *self, const void *data, uint32_t size) */
void ABI u2_cpp_SaveBuffer_WriteBytes(void *self, const void *data, uint32_t size) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000 ||
        !data || reinterpret_cast<uintptr_t>(data) < 0x10000) {
        return;
    }
    auto *s = static_cast<uint8_t *>(self);
    uint8_t *base = *reinterpret_cast<uint8_t **>(s + 0x08);
    uint8_t *cur  = *reinterpret_cast<uint8_t **>(s + 0x98);
    uint32_t cap  = *reinterpret_cast<const uint32_t *>(s + 0x04);
    if (!base || !cur || cur < base) return;

    size_t used = static_cast<size_t>(cur - base);
    if (used + size <= static_cast<size_t>(cap)) {
        std::memcpy(cur, data, size);
        *reinterpret_cast<uint8_t **>(s + 0x98) = cur + size;
    }
}

/* 0x00CD1730 — SaveBuffer::Write(void *self, const uint64_t *val) [Count0 / StringId] */
void ABI u2_cpp_SaveBuffer_WriteU64Count0(void *self, const uint64_t *val) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000 ||
        !val  || reinterpret_cast<uintptr_t>(val)  < 0x10000) {
        return;
    }
    auto *s = static_cast<uint8_t *>(self);
    if (g_u2_image_base) {
        uint32_t sid = static_cast<uint32_t>(*val);
        if (sid != 0) {
            auto fn_sid_to_str = u2::engine::elf_fn<const char *(ABI *)(uint32_t, int32_t)>(0x00CE33A0);
            (void)fn_sid_to_str(sid, 1);
        }
    }
    uint8_t *base = *reinterpret_cast<uint8_t **>(s + 0x08);
    uint8_t *cur  = *reinterpret_cast<uint8_t **>(s + 0x98);
    uint8_t *hdr  = *reinterpret_cast<uint8_t **>(s + 0x10);
    uint32_t cap  = *reinterpret_cast<const uint32_t *>(s + 0x04);
    if (!base || !cur || cur < base) return;

    size_t used = static_cast<size_t>(cur - base);
    if (used + 8u <= static_cast<size_t>(cap)) {
        *reinterpret_cast<uint64_t *>(cur) = *val;
        *reinterpret_cast<uint8_t **>(s + 0x98) = cur + 8;
        if (hdr && reinterpret_cast<uintptr_t>(hdr) >= 0x10000) {
            *reinterpret_cast<uint32_t *>(hdr + 0x0C) += 1u;
        }
    }
}

/* 0x00CD17C0 — SaveBuffer::Write(void *self, const uint64_t *val) [Count1] */
void ABI u2_cpp_SaveBuffer_WriteU64Count1(void *self, const uint64_t *val) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000 ||
        !val  || reinterpret_cast<uintptr_t>(val)  < 0x10000) {
        return;
    }
    auto *s = static_cast<uint8_t *>(self);
    uint8_t *base = *reinterpret_cast<uint8_t **>(s + 0x08);
    uint8_t *cur  = *reinterpret_cast<uint8_t **>(s + 0x98);
    uint8_t *hdr  = *reinterpret_cast<uint8_t **>(s + 0x10);
    uint32_t cap  = *reinterpret_cast<const uint32_t *>(s + 0x04);
    if (!base || !cur || cur < base) return;

    size_t used = static_cast<size_t>(cur - base);
    if (used + 8u <= static_cast<size_t>(cap)) {
        *reinterpret_cast<uint64_t *>(cur) = *val;
        *reinterpret_cast<uint8_t **>(s + 0x98) = cur + 8;
        if (hdr && reinterpret_cast<uintptr_t>(hdr) >= 0x10000) {
            *reinterpret_cast<uint32_t *>(hdr + 0x10) += 1u;
        }
    }
}

/* 0x00554F70 — Hud::FreeGrenadeIndicator(void *self, uint32_t idx) */
void ABI u2_cpp_Hud_FreeGrenadeIndicator(void *self, uint32_t idx) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000 || idx >= 8u) return;
    auto *s = static_cast<uint8_t *>(self);
    uint8_t *ind = *reinterpret_cast<uint8_t **>(s + 0x18 + static_cast<size_t>(idx) * 8);
    if (ind && reinterpret_cast<uintptr_t>(ind) >= 0x10000) {
        ind[0x41] = 0u;
    }
}

/* 0x00554FD0 — Hud::ShowGrenadeIndicator(void *self, uint32_t idx, uint8_t show) */
void ABI u2_cpp_Hud_ShowGrenadeIndicator(void *self, uint32_t idx, uint8_t show) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000 || idx >= 8u) return;
    auto *s = static_cast<uint8_t *>(self);
    uint8_t *ind = *reinterpret_cast<uint8_t **>(s + 0x18 + static_cast<size_t>(idx) * 8);
    if (ind && reinterpret_cast<uintptr_t>(ind) >= 0x10000) {
        *reinterpret_cast<uint32_t *>(ind + 0x0C) = static_cast<uint32_t>(show);
    }
}

/* 0x00A5F970 — cbCollectTaskNode(void *task, uint32_t key, void *collection)
 * Binary searches task->m_subnodeTable (+0x30) for key and appends matching subnode to collection.
 */
int32_t ABI u2_cpp_GameTask_cbCollectTaskNode(void *task, uint32_t key, void *collection) {
    if (!task || reinterpret_cast<uintptr_t>(task) < 0x10000 ||
        !collection || reinterpret_cast<uintptr_t>(collection) < 0x10000) {
        return 0;
    }
    const auto *t = static_cast<const uint8_t *>(task);
    const uint8_t *table_hdr = *reinterpret_cast<const uint8_t * const *>(t + 0x30);
    if (!table_hdr || reinterpret_cast<uintptr_t>(table_hdr) < 0x10000) return 0;

    int32_t count = *reinterpret_cast<const int32_t *>(table_hdr + 0x00);
    if (count <= 0) return 0;

    const uint8_t *entries = *reinterpret_cast<const uint8_t * const *>(table_hdr + 0x08);
    if (!entries || reinterpret_cast<uintptr_t>(entries) < 0x10000) return 0;

    int32_t lo = 0;
    int32_t hi = count - 1;
    while (lo <= hi) {
        int32_t mid = (lo + hi) / 2;
        const uint8_t *entry = entries + static_cast<size_t>(mid) * 16;
        uint32_t entry_key = *reinterpret_cast<const uint32_t *>(entry + 0x00);
        if (entry_key < key) {
            if (mid >= hi) return 0;
            lo = mid + 1;
        } else if (entry_key > key) {
            hi = mid - 1;
        } else {
            void *subnode = *reinterpret_cast<void * const *>(entry + 0x08);
            if (subnode) {
                auto *col = static_cast<uint8_t *>(collection);
                uint32_t col_count = *reinterpret_cast<const uint32_t *>(col + 0x00);
                if (col_count < 0x280u) {
                    *reinterpret_cast<uint32_t *>(col + 0x00) = col_count + 1u;
                    *reinterpret_cast<void **>(col + 0x08 + static_cast<size_t>(col_count) * 8) = subnode;
                }
            }
            return 0;
        }
    }
    return 0;
}

/* 0x00A1D600 — FreeSetting(void *setting)
 * Removes setting from m_settingHashTable, returns node to free list, clears slot bit in bitmap.
 */
void ABI u2_cpp_Settings_FreeSetting(void *setting) {
    if (!g_u2_image_base || !setting || reinterpret_cast<uintptr_t>(setting) < 0x10000) return;
    auto *s = static_cast<uint8_t *>(setting);
    uint8_t *settings_base = *u2::engine::elf_ptr<uint8_t *>(0x015350E0);
    if (!settings_base) return;

    intptr_t diff = s - settings_base;
    uint64_t slot_idx = static_cast<uint64_t>((diff >> 4) * 0x6DB6DB6DB6DB6DB7ULL);
    if (static_cast<uint32_t>(slot_idx) >= 0x2000u) return;

    uint64_t setting_key = *reinterpret_cast<const uint64_t *>(s + 0x18);
    int32_t mod = *u2::engine::elf_ptr<const int32_t>(0x0153513C);
    uint8_t **buckets = *u2::engine::elf_ptr<uint8_t **>(0x01535100);

    if (mod > 0 && buckets && reinterpret_cast<uintptr_t>(buckets) >= 0x10000) {
        uint32_t bucket_idx = static_cast<uint32_t>(setting_key) % static_cast<uint32_t>(mod);
        uint8_t **bucket_slot = &buckets[bucket_idx];
        uint8_t *node = *bucket_slot;
        uint8_t *end_sentinel = buckets[mod];

        auto resolve_owner = [](const uint8_t *st) noexcept -> const void * {
            const uint8_t *h = *reinterpret_cast<const uint8_t * const *>(st + 0x00);
            uint32_t pid = *reinterpret_cast<const uint32_t *>(st + 0x08);
            if (!h || reinterpret_cast<uintptr_t>(h) < 0x10000 || pid == 0) return nullptr;
            const uint8_t *proc = *reinterpret_cast<const uint8_t * const *>(h);
            if (!proc || reinterpret_cast<uintptr_t>(proc) < 0x10000) return nullptr;
            return (*reinterpret_cast<const uint32_t *>(proc + 0x44) == pid) ? proc : nullptr;
        };

        const void *target_owner = resolve_owner(s);
        while (node && reinterpret_cast<uintptr_t>(node) >= 0x10000 && node != end_sentinel) {
            const uint8_t *cand = *reinterpret_cast<const uint8_t * const *>(node + 0x10);
            if (cand && reinterpret_cast<uintptr_t>(cand) >= 0x10000 &&
                *reinterpret_cast<const uint64_t *>(cand + 0x18) == setting_key &&
                *reinterpret_cast<const uint32_t *>(cand + 0x64) == *reinterpret_cast<const uint32_t *>(s + 0x64) &&
                resolve_owner(cand) == target_owner) {
                break;
            }
            node = *reinterpret_cast<uint8_t **>(node + 0x00);
        }

        if (node && reinterpret_cast<uintptr_t>(node) >= 0x10000 && node != end_sentinel) {
            uint8_t *next = *reinterpret_cast<uint8_t **>(node + 0x00);
            uint8_t *prev = *reinterpret_cast<uint8_t **>(node + 0x08);
            if (prev && reinterpret_cast<uintptr_t>(prev) >= 0x10000) {
                *reinterpret_cast<uint8_t **>(prev + 0x00) = next;
            }
            if (next && reinterpret_cast<uintptr_t>(next) >= 0x10000) {
                *reinterpret_cast<uint8_t **>(next + 0x08) = prev;
            }
            if (*bucket_slot == node) {
                *bucket_slot = next;
            }
            uint8_t **free_head = u2::engine::elf_ptr<uint8_t *>(0x01535108);
            *reinterpret_cast<uint8_t **>(node + 0x00) = *free_head;
            *free_head = node;
        }
    }

    uint32_t idx32 = static_cast<uint32_t>(slot_idx);
    if (idx32 <= 0x1FFFu) {
        s[0x61] &= static_cast<uint8_t>(~1u);
        uint64_t mask = ~(1ULL << (idx32 & 63u));
        uint64_t *bitmap = u2::engine::elf_ptr<uint64_t>(0x01534CE0);
        bitmap[idx32 >> 6] &= mask;
    }
    *reinterpret_cast<uint32_t *>(s + 0x68) = 0u;
}

/* 0x00B77AA0 — IsChunkSetEntirelyWithin(const char *chunk_set_name, const uint64_t *target_mask) */
uint8_t ABI u2_cpp_GameLoading_IsChunkSetEntirelyWithin(const char *chunk_set_name, const uint64_t *target_mask) {
    if (!chunk_set_name) return 1u;
    if (!g_u2_image_base || reinterpret_cast<uintptr_t>(chunk_set_name) < 0x10000 ||
        !target_mask || reinterpret_cast<uintptr_t>(target_mask) < 0x10000) {
        return 1u;
    }

    // FNV-1a 32-bit hash
    uint32_t hash = 0x811C9DC5u;
    for (const uint8_t *p = reinterpret_cast<const uint8_t *>(chunk_set_name); *p != 0; ++p) {
        hash = (hash ^ static_cast<uint32_t>(*p)) * 0x01000193u;
    }

    int32_t mod = *u2::engine::elf_ptr<const int32_t>(0x015AFA3C);
    uint8_t **buckets = *u2::engine::elf_ptr<uint8_t **>(0x015AFA00);
    if (mod <= 0 || !buckets || reinterpret_cast<uintptr_t>(buckets) < 0x10000) {
        return 1u;
    }

    uint32_t bucket_idx = hash % static_cast<uint32_t>(mod);
    uint8_t *node = buckets[bucket_idx];
    uint8_t *end_sentinel = buckets[mod];

    const uint64_t *chunk_mask = nullptr;
    while (node && reinterpret_cast<uintptr_t>(node) >= 0x10000 && node != end_sentinel) {
        const char *cand_name = *reinterpret_cast<const char * const *>(node + 0x10);
        if (cand_name && reinterpret_cast<uintptr_t>(cand_name) >= 0x10000 &&
            std::strcmp(cand_name, chunk_set_name) == 0) {
            chunk_mask = reinterpret_cast<const uint64_t *>(node + 0x18);
            break;
        }
        node = *reinterpret_cast<uint8_t **>(node + 0x00);
    }

    if (!chunk_mask) return 1u;

    uint64_t missing0 = (~target_mask[0]) & chunk_mask[0];
    uint64_t missing1 = ((~target_mask[1]) & chunk_mask[1]) & 0xFFFFFFFFFULL;
    return ((missing0 | missing1) == 0ULL) ? 1u : 0u;
}

/* 0x00B770D0 — GetPlayGoSpeed(void) */
uint32_t ABI u2_cpp_GameLoading_GetPlayGoSpeed(void) {
    if (!g_u2_image_base) return 0u;
    int32_t suspend_count = *u2::engine::elf_ptr<const int32_t>(0x015AF7C0);
    if (suspend_count > 0) return 0u;
    return *u2::engine::elf_ptr<const uint32_t>(0x01050A30);
}

/* 0x00552C70 — Hud::ReadConfigFile(void *self)
 * Reads HUD settings and materials from script.
 * In original PS4 code, if ScriptManager::LookupPointerInModule(0x9353ED16, 0x863652C5)
 * returns NULL (or uninitialized table), it crashes at 0x00552E48 dereferencing 0x70(%rbx).
 * Our detour cleanly initializes the material and button arrays on the Hud instance:
 *   +0x80: uint32_t numMaterials = 0
 *   +0x88: void *materials[16] = { nullptr }
 *   +0x108: uint32_t numButtons = 0
 *   +0x110: void *buttons[64] = { nullptr }
 */
void ABI u2_cpp_Hud_ReadConfigFile(void *self) {
    if (!self || reinterpret_cast<uintptr_t>(self) < 0x10000) return;
    auto *hud = static_cast<uint8_t *>(self);

    // 1. Initialize material slots
    *reinterpret_cast<uint32_t *>(hud + 0x80) = 0;
    std::memset(hud + 0x88, 0, 16 * sizeof(void *));

    // 2. Initialize button slots
    *reinterpret_cast<uint32_t *>(hud + 0x108) = 0;
    std::memset(hud + 0x110, 0, 64 * sizeof(void *));

    // 3. Attempt symbol lookup via ScriptManager::LookupPointerInModule(0x9353ED16, 0x863652C5)
    if (g_u2_image_base) {
        auto lookup_fn = u2::engine::elf_fn<void *(ABI *)(uint32_t, uint32_t)>(0x00A17280);
        if (lookup_fn) {
            void *tbl = lookup_fn(0x9353ED16, 0x863652C5);
            if (tbl && reinterpret_cast<uintptr_t>(tbl) >= 0x10000) {
                auto *tbl_bytes = static_cast<uint8_t *>(tbl);
                void *mat_array = *reinterpret_cast<void **>(tbl_bytes + 0x70);
                if (mat_array && reinterpret_cast<uintptr_t>(mat_array) >= 0x10000) {
                    // Script material table is present and populated
                }
            }
        }
    }
}

} // extern "C"
