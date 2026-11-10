/* Uncharted 2: Among Thieves — Native C++ Engine Subsystems
 * Source units:
 *   - src/game/math/catmull-rom.cpp
 *   - src/game/math/catmull-rom-manager.cpp
 *   - src/game/spline/spline-tracker.cpp
 *   - src/game/vehicle/vehicle-spline-tracker.cpp
 */
#include "u2_engine.h"

namespace u2::engine {

struct CatmullRomSplineData {
    uint32_t m_nameId;          // +0x00
    uint32_t m_pad04;           // +0x04
    void    *m_spawnerData;     // +0x08
    uint32_t m_typeId;          // +0x10
    uint8_t  m_flags;           // +0x14 (bit 0 = isLooped)
    uint8_t  m_pad15[0x13];     // +0x15..+0x27
    uint32_t m_controlPoints;   // +0x28
    uint8_t  m_pad2C[0x1C];     // +0x2C..+0x47
    float   *m_cumulArcLengths; // +0x48
};

struct CatmullRomSpline {
    uint8_t               m_header[0x10]; // +0x00
    CatmullRomSplineData *m_data;         // +0x10
};

} // namespace u2::engine

using namespace u2::engine;

extern "C" {

const char * ABI u2_cpp_CatmullRomSpline_GetName(void *self) {
    /* 0x00468260: CatmullRomSpline::GetName() const */
    if (!self || !g_u2_image_base) return nullptr;
    auto *spline = static_cast<const CatmullRomSpline *>(self);
    if (!spline->m_data) return nullptr;
    using StringIdToStringFn = const char * (ABI *)(uint32_t, int32_t);
    auto fn = elf_fn<StringIdToStringFn>(0x00CE33A0ull);
    return fn(spline->m_data->m_nameId, 0);
}

uint32_t ABI u2_cpp_CatmullRomSpline_IsLooped(void *self) {
    /* 0x004684C0: CatmullRomSpline::IsLooped() const */
    if (!self) return 0;
    auto *spline = static_cast<const CatmullRomSpline *>(self);
    if (!spline->m_data) return 0;
    return static_cast<uint32_t>(spline->m_data->m_flags & 1u);
}

uint32_t ABI u2_cpp_CatmullRomSpline_GetControlPointCount(void *self) {
    /* 0x00468520: CatmullRomSpline::GetControlPointCount() const */
    if (!self) return 0;
    auto *spline = static_cast<const CatmullRomSpline *>(self);
    if (!spline->m_data) return 0;
    return static_cast<uint32_t>(spline->m_data->m_flags & 1u) + spline->m_data->m_controlPoints;
}

float ABI u2_cpp_CatmullRomSpline_GetTotalArcLength(void *self) {
    /* 0x00468D90: CatmullRomSpline::GetTotalArcLength() const */
    if (!self) return 0.0f;
    auto *spline = static_cast<const CatmullRomSpline *>(self);
    const CatmullRomSplineData *data = spline->m_data;
    if (!data || !data->m_cumulArcLengths) return 0.0f;
    uint32_t total = static_cast<uint32_t>(data->m_flags & 1u) + data->m_controlPoints;
    uint32_t segs = (total == 0) ? 0u : (total - 1u);
    return (segs == 0) ? 0.0f : data->m_cumulArcLengths[segs - 1u];
}

float ABI u2_cpp_CatmullRomSpline_GetCumulSegArcLength(void *self, int32_t seg_idx) {
    /* 0x00469F60: CatmullRomSpline::GetCumulSegArcLength(int) const */
    if (!self || seg_idx <= 0) return 0.0f;
    auto *spline = static_cast<const CatmullRomSpline *>(self);
    const CatmullRomSplineData *data = spline->m_data;
    if (!data || !data->m_cumulArcLengths) return 0.0f;
    return data->m_cumulArcLengths[seg_idx - 1];
}

void ABI u2_cpp_CatmullRomManager_OnSpawnGameObject(void *mgr, void *game_obj) {
    /* 0x004643E0: CatmullRomManager::OnSpawnGameObject(GameObject *) */
    if (!mgr || !game_obj || !g_u2_image_base) return;
    uintptr_t vtbl = *static_cast<const uintptr_t *>(game_obj);
    if (!vtbl) return;
    using GetSpawnerDataFn = void * (ABI *)(void *);
    auto get_id = *reinterpret_cast<const GetSpawnerDataFn *>(vtbl + 0xA0);
    if (!get_id) return;
    void *id = get_id(game_obj);
    if (!id) return;

    using BindGameObjectFn = void (ABI *)(void *, void *);
    auto bind_fn = elf_fn<BindGameObjectFn>(0x004678E0ull);

    for (size_t offset = 0x20; offset != 0x19820; offset += 0x110) {
        uint8_t *base = *reinterpret_cast<uint8_t **>(static_cast<uint8_t *>(mgr) + 0x8);
        if (!base) break;
        if (*reinterpret_cast<const uint64_t *>(base + offset - 0x20) != 0) {
            const auto *spline_data = *reinterpret_cast<const CatmullRomSplineData * const *>(base + offset);
            if (spline_data && spline_data->m_spawnerData == id) {
                bind_fn(base + offset - 0x10, game_obj);
            }
        }
    }
}

void ABI u2_cpp_CatmullRomManager_OnDestroyGameObject(void *mgr, void *game_obj) {
    /* 0x00464490: CatmullRomManager::OnDestroyGameObject(GameObject *) */
    if (!mgr || !game_obj || !g_u2_image_base) return;
    uintptr_t vtbl = *static_cast<const uintptr_t *>(game_obj);
    if (!vtbl) return;
    using GetSpawnerDataFn = void * (ABI *)(void *);
    auto get_id = *reinterpret_cast<const GetSpawnerDataFn *>(vtbl + 0xA0);
    if (!get_id) return;
    void *id = get_id(game_obj);
    if (!id) return;

    using BindGameObjectFn = void (ABI *)(void *, void *);
    auto bind_fn = elf_fn<BindGameObjectFn>(0x004678E0ull);

    for (size_t offset = 0x20; offset != 0x19820; offset += 0x110) {
        uint8_t *base = *reinterpret_cast<uint8_t **>(static_cast<uint8_t *>(mgr) + 0x8);
        if (!base) break;
        if (*reinterpret_cast<const uint64_t *>(base + offset - 0x20) != 0) {
            const auto *spline_data = *reinterpret_cast<const CatmullRomSplineData * const *>(base + offset);
            if (spline_data && spline_data->m_spawnerData == id) {
                bind_fn(base + offset - 0x10, nullptr);
            }
        }
    }
}

void ABI u2_cpp_CatmullRomManager_RemoveSpline(void *mgr, void *spline_data) {
    /* 0x00465240: CatmullRomManager::RemoveSpline(CatmullRomSplineData *) */
    if (!mgr || !g_u2_image_base) return;
    using RemoveCallbackFn = void (ABI *)(void *, void *);
    using UnregisterSpecialSplineFn = void (ABI *)(void *, void *);
    auto unreg_fn = elf_fn<UnregisterSpecialSplineFn>(0x003D1EA0ull);

    for (size_t offset = 0; offset != 0x19800; offset += 0x110) {
        uint8_t *base = *reinterpret_cast<uint8_t **>(static_cast<uint8_t *>(mgr) + 0x8);
        if (!base) break;
        if (*reinterpret_cast<void **>(base + offset) == spline_data) {
            const auto *data = static_cast<const CatmullRomSplineData *>(spline_data);
            uint32_t type = data ? data->m_typeId : 0;
            bool is_special = (type == 0x0E3532C9u || type == 0x642C65B1u || type == 0x6749641Eu);
            auto cb = *reinterpret_cast<const RemoveCallbackFn *>(static_cast<const uint8_t *>(mgr) + 0x68);
            if (cb) {
                cb(base + offset + 0x10, *reinterpret_cast<void **>(static_cast<uint8_t *>(mgr) + 0x70));
            }
            if (is_special) {
                unreg_fn(elf_ptr(0x01112430ull), base + offset + 0x10);
            }
            (*static_cast<uint32_t *>(mgr))--;
            base = *reinterpret_cast<uint8_t **>(static_cast<uint8_t *>(mgr) + 0x8);
            if (base) {
                *reinterpret_cast<uint64_t *>(base + offset) = 0;
                *reinterpret_cast<uint64_t *>(base + offset + 0x20) = 0;
            }
        }
    }
}

void ABI u2_cpp_CatmullRomManager_WalkSplines(void *mgr, void (ABI *cb)(void *, void *), void *user_data, int32_t all_flag) {
    /* 0x00466320: CatmullRomManager::WalkSplines(callback, user_data, bool) */
    if (!mgr || !cb) return;
    for (size_t offset = 0x10; offset != 0x19810; offset += 0x110) {
        uint8_t *base = *reinterpret_cast<uint8_t **>(static_cast<uint8_t *>(mgr) + 0x8);
        if (!base) break;
        if (*reinterpret_cast<const uint64_t *>(base + offset - 0x10) != 0) {
            if (all_flag == 1 || (base[offset] & 0x5u) == 0) {
                cb(base + offset, user_data);
            }
        }
    }
}

void * ABI u2_cpp_SplineTracker_GetSpline0(void *self) {
    /* 0x00A24040: SplineTracker::GetSpline() */
    if (!self || !g_u2_image_base) return nullptr;
    int64_t idx = *reinterpret_cast<const int32_t *>(static_cast<const uint8_t *>(self) + 0x30);
    if (idx < 0) return nullptr;
    uint8_t *arr = *elf_ptr<uint8_t *>(0x0104EB30ull + 0x8);
    if (!arr) return nullptr;
    uint8_t *entry = arr + static_cast<size_t>(idx) * 0x110;
    if (*reinterpret_cast<void **>(entry + 0x20) != *reinterpret_cast<void * const *>(static_cast<const uint8_t *>(self) + 0x38)) {
        return nullptr;
    }
    return entry + 0x10;
}

void * ABI u2_cpp_SplineTracker_GetSpline1(void *self) {
    /* 0x00A24080: SplineTracker::GetSpline() const */
    return u2_cpp_SplineTracker_GetSpline0(self);
}

uint32_t ABI u2_cpp_SplineTracker_GetSplineNameId(void *self) {
    /* 0x00A240C0: SplineTracker::GetSplineNameId() const */
    if (!self || !g_u2_image_base) return 0;
    int64_t idx = *reinterpret_cast<const int32_t *>(static_cast<const uint8_t *>(self) + 0x30);
    if (idx < 0) return 0;
    uint8_t *arr = *elf_ptr<uint8_t *>(0x0104EB30ull + 0x8);
    if (!arr) return 0;
    uint8_t *entry = arr + static_cast<size_t>(idx) * 0x110;
    const uint32_t *data = *reinterpret_cast<const uint32_t * const *>(entry + 0x20);
    if (static_cast<const void *>(data) != *reinterpret_cast<const void * const *>(static_cast<const uint8_t *>(self) + 0x38) || !data) {
        return 0;
    }
    return *data;
}

void ABI u2_cpp_SplineTracker_FinalizeDistance(void *self) {
    /* 0x00A24110: SplineTracker::FinalizeDistance() */
    if (!self || !g_u2_image_base) return;
    using GetRealDistFn = float (ABI *)(void *, uint32_t);
    auto fn = elf_fn<GetRealDistFn>(0x00A23EC0ull);
    float d = fn(self, 1);
    auto *bytes = static_cast<uint8_t *>(self);
    *reinterpret_cast<float *>(bytes + 0x40) = d;
    *reinterpret_cast<uint32_t *>(bytes + 0x44) = 0;
    *reinterpret_cast<uint32_t *>(bytes + 0x50) = 0;
}

float ABI u2_cpp_VehicleSplineTracker_GetCurrentVirtualDistance_m(void *self, int32_t mode) {
    /* 0x00ABA7B0: VehicleSplineTracker::GetCurrentVirtualDistance_m(int) const */
    if (!self) return 0.0f;
    const auto *bytes = static_cast<const uint8_t *>(self);
    if (mode == 0) return *reinterpret_cast<const float *>(bytes + 0x40);
    if (mode == 1) return *reinterpret_cast<const float *>(bytes + 0x190);
    return 0.0f;
}

float ABI u2_cpp_VehicleSplineTracker_GetCurrentRealDistance_m(void *self, int32_t mode, uint32_t wrap) {
    /* 0x00ABA840: VehicleSplineTracker::GetCurrentRealDistance_m(int, bool) const */
    if (!self || !g_u2_image_base) return 0.0f;
    using GetRealDistFn = float (ABI *)(void *, uint32_t);
    auto fn = elf_fn<GetRealDistFn>(0x00A23EC0ull);
    if (mode == 0) return fn(self, wrap & 0xFFu);
    if (mode == 1) return fn(static_cast<uint8_t *>(self) + 0x150, wrap & 0xFFu);
    return 0.0f;
}

} // extern "C"
