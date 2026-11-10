/* Uncharted 2: Among Thieves — Native C++ Engine Subsystems
 * Source units:
 *   - src/game/vehicle/rail-vehicle.cpp (originally compiled -O0 in PS4 shipping ELF)
 *   - src/game/vehicle/rail-vehicle-behavior.cpp
 *   - src/game/vehicle/rail-vehicle-manager.cpp
 */
#include "u2_engine.h"

using namespace u2::engine;

extern "C" {

void ABI u2_cpp_RailVehicle_InitBehavior(void *self, void *dc_behavior) {
    // 0x000013D0: RailVehicle::InitBehavior(DcRailVehicleBehavior const*)
    if (!self || !dc_behavior || !g_u2_image_base) return;
    using ContainerInitFn = void (ABI *)(void *, void *, void *);
    auto fn = elf_fn<ContainerInitFn>(0x00A8EAA0ull);
    fn(static_cast<uint8_t *>(self) + 0xEB0, self, dc_behavior);
}

void ABI u2_cpp_RailVehicle_SetFollowDistance(void *self, float begin_dist, float end_dist) {
    /* 0x00001460: RailVehicle::SetFollowDistance(float, float) */
    if (!self) return;
    auto *bytes = static_cast<uint8_t *>(self);
    *reinterpret_cast<float *>(bytes + 0x560) = begin_dist;
    *reinterpret_cast<float *>(bytes + 0x564) = end_dist;
}

void * ABI u2_cpp_RailVehicle_GetVehicleClass(void *self) {
    /* 0x00001DD0: RailVehicle::GetVehicleClass() */
    if (!self || !g_u2_image_base) return nullptr;
    auto *bytes = static_cast<uint8_t *>(self);
    void *cls = *reinterpret_cast<void **>(bytes + 0x260);
    if (!cls) {
        uint32_t sid = *reinterpret_cast<const uint32_t *>(bytes + 0x25C);
        using LookupTypeFn = void * (ABI *)(uint32_t);
        auto fn = elf_fn<LookupTypeFn>(0x00D75840ull);
        cls = fn(sid);
        *reinterpret_cast<void **>(bytes + 0x260) = cls;
    }
    return cls;
}

void ABI u2_cpp_RailVehicle_ResetWheels(void *self) {
    /* 0x00001ED0: RailVehicle::ResetWheels() */
    if (!self) return;
    auto *bytes = static_cast<uint8_t *>(self);
    uint32_t count = *reinterpret_cast<const uint32_t *>(bytes + 0x750);
    for (uint32_t i = 0; i < count; ++i) {
        *(bytes + 0x820 + static_cast<size_t>(i) * 0xD0 + 0xC8) = 0;
    }
}

void * ABI u2_cpp_RailVehicle_GetDamageable(void *self, uint32_t index) {
    /* 0x00002430: RailVehicle::GetDamageable(unsigned int) */
    if (!self || !g_u2_image_base) return nullptr;
    auto *cls = static_cast<uint8_t *>(u2_cpp_RailVehicle_GetVehicleClass(self));
    if (!cls) return nullptr;
    uint32_t count = *reinterpret_cast<const uint32_t *>(cls + 0xD0);
    if (index >= count) return nullptr;
    uint8_t *arr = *reinterpret_cast<uint8_t **>(cls + 0xD8);
    return arr ? (arr + static_cast<size_t>(index) * 0x30) : nullptr;
}

void ABI u2_cpp_RailVehicle_OnIgcBind(void *self) {
    /* 0x00005630: RailVehicle::OnIgcBind() */
    if (!self || !g_u2_image_base) return;
    auto *bytes = static_cast<uint8_t *>(self);
    int32_t cnt = *reinterpret_cast<const int32_t *>(bytes + 0xDF0);
    if (cnt == 0) {
        *elf_ptr<uint8_t>(0x011362F8ull + 0x31) = 1;
    }
    *reinterpret_cast<int32_t *>(bytes + 0xDF0) = cnt + 1;
    *reinterpret_cast<uint32_t *>(bytes + 0xE08) = 0xFFFFFFFFu;
}

void ABI u2_cpp_RailVehicle_OnIgcUnbind(void *self) {
    /* 0x000056E0: RailVehicle::OnIgcUnbind() */
    if (!self || !g_u2_image_base) return;
    auto *bytes = static_cast<uint8_t *>(self);
    int32_t cnt = *reinterpret_cast<const int32_t *>(bytes + 0xDF0);
    if (cnt > 0) {
        cnt--;
        *reinterpret_cast<int32_t *>(bytes + 0xDF0) = cnt;
        if (cnt == 0) {
            uint32_t flags = *reinterpret_cast<const uint32_t *>(bytes + 0x5A0);
            flags = (flags & ~0x2020u) | 0x2020u;
            *reinterpret_cast<uint32_t *>(bytes + 0x5A0) = flags;
            *reinterpret_cast<uint32_t *>(bytes + 0xE0C) = 0;
            *elf_ptr<uint8_t>(0x011362F8ull + 0x31) = 0;
            using GetTimeFn = float (ABI *)(void *);
            auto get_time = elf_fn<GetTimeFn>(0x00A90590ull);
            float t = get_time(elf_ptr(0x0116454Bull));
            *reinterpret_cast<float *>(bytes + 0xDFC) = t + *elf_ptr<const float>(0x00DF440Cull);
        }
    }
}

void * ABI u2_cpp_RailVehicleBehavior_GetDcBehavior(void *self) {
    /* 0x00A8DE50: RailVehicleBehavior::GetDcBehavior() const */
    if (!self) return nullptr;
    return *reinterpret_cast<void **>(static_cast<uint8_t *>(self) + 0x18);
}

void * ABI u2_cpp_RailVehicleBehaviorContainer_GetCurrentBehavior(void *self) {
    /* 0x00A8EA30: RailVehicleBehaviorContainer::GetCurrentBehavior() */
    if (!self) return nullptr;
    uint32_t type = *static_cast<const uint32_t *>(self);
    if (type <= 1u) {
        return static_cast<uint8_t *>(self) + 0x70;
    }
    if (type != 2u) {
        *static_cast<uint32_t *>(self) = 2u;
    }
    return static_cast<uint8_t *>(self) + 0x10;
}

void * ABI u2_cpp_RailVehicleManager_GetMover(void *self, uint32_t idx) {
    /* 0x00A8FCE0: RailVehicleManager::GetMover(unsigned int) */
    (void)self;
    if (!g_u2_image_base) return nullptr;
    return elf_ptr(0x0153EFA8ull + 0xD18ull + static_cast<size_t>(idx) * 0x30);
}

void ABI u2_cpp_RailVehicleManager_DetachFromRail(void *self, void *vehicle, int32_t rail_idx) {
    /* 0x00A903E0: RailVehicleManager::DetachFromRail(RailVehicle *, int) */
    (void)self;
    (void)vehicle;
    if (!g_u2_image_base || rail_idx < 0) return;
    uint8_t *rail = elf_ptr(0x0153EFA8ull + static_cast<size_t>(static_cast<uint32_t>(rail_idx)) * 0x60);
    rail[0x12C] &= static_cast<uint8_t>(~0x2u);
    *reinterpret_cast<uint64_t *>(rail + 0x118) = 0;
    *reinterpret_cast<uint32_t *>(rail + 0x120) = 0;
    *reinterpret_cast<float *>(rail + 0x128) = *reinterpret_cast<const float *>(rail + 0xF8);
}

} // extern "C"
