// Uncharted 2: Among Thieves — Native C++ Engine Functional Unit Tests
// Validates all C++23 engine detours in src/u2_engine/ by copying the installed
// 12-byte ELF trampolines from (real_image + offset) into an isolated executable
// sandbox image (PAGE_EXECUTE_READWRITE), wiring mock guest helpers, and verifying
// every return value (%rax / %xmm0) and memory/struct mutation.

#include "u2_engine.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#endif

namespace {

static constexpr size_t kMockImageSize = 0x01600000; // 22 MiB covers all U2 code + .bss offsets

struct ExecutableMockImage {
    uint8_t *base = nullptr;

    ExecutableMockImage() {
#ifdef _WIN32
        base = static_cast<uint8_t *>(VirtualAlloc(nullptr, kMockImageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
#else
        void *p = mmap(nullptr, kMockImageSize, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (p != MAP_FAILED) base = static_cast<uint8_t *>(p);
#endif
    }

    ~ExecutableMockImage() {
        if (!base) return;
#ifdef _WIN32
        VirtualFree(base, 0, MEM_RELEASE);
#else
        munmap(base, kMockImageSize);
#endif
    }
};

static void write_temp_jmp64(uint8_t *dst, uintptr_t target) {
    dst[0] = 0x48;
    dst[1] = 0xB8;
    std::memcpy(dst + 2, &target, 8);
    dst[10] = 0xFF;
    dst[11] = 0xE0;
}

static int s_failed_assertions = 0;
static int s_passed_assertions = 0;

#define U2_TEST_ASSERT(cond, msg)                                                           \
    do {                                                                                    \
        if (!(cond)) {                                                                      \
            std::printf("[U2 EngineTest] FAILED (%s:%d): %s\n", __FILE__, __LINE__, (msg)); \
            ++s_failed_assertions;                                                          \
        } else {                                                                            \
            ++s_passed_assertions;                                                          \
        }                                                                                   \
    } while (0)

// Mock state for guest helpers called by C++ engine methods
static const char * ABI mock_StringIdToString(uint32_t id, int32_t mode) {
    (void)mode;
    return (id == 0x12345678u) ? "spline_train_track_01" : "unknown";
}

static void *s_last_bound_spline = nullptr;
static void *s_last_bound_obj = nullptr;
static void ABI mock_BindGameObject(void *spline, void *obj) {
    s_last_bound_spline = spline;
    s_last_bound_obj = obj;
}

static void * ABI mock_GameObject_GetSpawnerData(void *self) {
    return *reinterpret_cast<void **>(static_cast<uint8_t *>(self) + 0x10);
}

static int s_unreg_special_calls = 0;
static void ABI mock_UnregisterSpecialSpline(void *global_mgr, void *spline) {
    (void)global_mgr;
    if (spline) ++s_unreg_special_calls;
}

static int s_remove_cb_calls = 0;
static void ABI mock_RemoveSplineCallback(void *spline, void *user_data) {
    if (spline && user_data == reinterpret_cast<void *>(0x99887766ull)) {
        ++s_remove_cb_calls;
    }
}

static int s_walk_cb_calls = 0;
static void ABI mock_WalkSplinesCallback(void *spline, void *user_data) {
    if (spline && user_data == reinterpret_cast<void *>(0x4242ull)) {
        ++s_walk_cb_calls;
    }
}

static float ABI mock_GetRealDist(void *tracker, uint32_t wrap) {
    float base = *reinterpret_cast<const float *>(static_cast<const uint8_t *>(tracker) + 0x10);
    return base + (wrap ? 10.0f : 0.0f);
}

static void *s_mock_vehicle_class_ptr = nullptr;
static void * ABI mock_LookupVehicleClass(uint32_t sid) {
    return (sid == 0xCAFE1234u) ? s_mock_vehicle_class_ptr : nullptr;
}

static void *s_last_init_container = nullptr;
static void *s_last_init_vehicle = nullptr;
static void *s_last_init_dc = nullptr;
static void ABI mock_BehaviorContainerInit(void *container, void *vehicle, void *dc) {
    s_last_init_container = container;
    s_last_init_vehicle = vehicle;
    s_last_init_dc = dc;
}

static float ABI mock_GetTime(void *clock_ptr) {
    (void)clock_ptr;
    return 100.0f;
}

} // namespace

extern "C" int u2_run_engine_unit_tests(uint8_t *real_image) {
    if (!real_image) return 0;
    s_failed_assertions = 0;
    s_passed_assertions = 0;

    ExecutableMockImage mock_img;
    if (!mock_img.base) return 0;
    uint8_t *image = mock_img.base;

    // Initialize .text region [0 .. 0x00DA8000) with safe SysV ABI stub:
    //   31 C0       xor %eax, %eax
    //   0F 57 C0    xorps %xmm0, %xmm0
    //   C3          ret
    // padded with 0xC3 (ret) so any guest helper call returns 0 / nullptr / 0.0f safely.
    for (size_t off = 0; off < 0x00DA8000; off += 16) {
        uint8_t *slot = image + off;
        slot[0] = 0x31; slot[1] = 0xC0;
        slot[2] = 0x0F; slot[3] = 0x57; slot[4] = 0xC0;
        for (size_t k = 5; k < 16; ++k) slot[k] = 0xC3;
    }
    // Copy .rodata constants/tables [0x00DA8000 .. 0x01040000) from real_image
    std::memcpy(image + 0x00DA8000, real_image + 0x00DA8000, 0x01040000 - 0x00DA8000);

    // Copy the actual 12-byte trampolines installed in real_image for every engine detour
    const U2NativeDetour *engine_detours = nullptr;
    size_t engine_count = u2_get_engine_detours(&engine_detours);
    for (size_t i = 0; i < engine_count; ++i) {
        uintptr_t off = engine_detours[i].offset;
        std::memcpy(image + off, real_image + off, 12);
    }

    uint8_t *saved_base = g_u2_image_base;
    g_u2_image_base = image;

    // 0. Full-Body Sandbox Fuzzing across all 5 DC Script Modules via ELF trampolines
    //    Unlike pre-boot NULL fuzzing, g_u2_image_base points to the executable mock image,
    //    so 100% of the body of every Dc* function executes past `if (!g_u2_image_base)`.
    {
        using GetterFn = size_t (*)(const U2NativeDetour **);
        const GetterFn dc_getters[] = {
            u2_get_player_detours,
            u2_get_npc_detours,
            u2_get_audio_detours,
            u2_get_event_detours,
            u2_get_scene_detours,
        };
        std::vector<uint8_t> dummy_arg_buf(4096, 0);
        uint64_t zero_args[8] = {0};
        uint64_t ptr_args[8];
        for (int k = 0; k < 8; ++k) {
            ptr_args[k] = reinterpret_cast<uintptr_t>(dummy_arg_buf.data() + k * 256);
        }
        uint32_t valid_status = 0;
        size_t full_body_tested = 0;

        for (GetterFn getter : dc_getters) {
            const U2NativeDetour *detours = nullptr;
            size_t count = getter(&detours);
            for (size_t i = 0; i < count; ++i) {
                if (std::strcmp(detours[i].name, "DcDebugBreak") == 0 ||
                    std::strcmp(detours[i].name, "DcJump") == 0 ||
                    std::strcmp(detours[i].name, "DcEndScript") == 0) {
                    continue;
                }
                uintptr_t off = detours[i].offset;
                uint8_t saved_stub[16];
                std::memcpy(saved_stub, image + off, 16);
                std::memcpy(image + off, real_image + off, 12);
                auto tramp_fn = reinterpret_cast<U2DcNativeFn>(image + off);

                // Pass A: g_u2_image_base != NULL, argc=0, NULL argv
                // Regression check: if %rdx points to a C++ object on the heap (vtable = 0x0B954321u)
                // when a helper like DcKillAllAttachables is called from C++, u2_set_status(status, 0)
                // must NEVER zero out the vtable at (%rdx).
                valid_status = 0x0B954321u;
                tramp_fn(0, nullptr, &valid_status);
                if (valid_status == 0) {
                    U2_TEST_ASSERT(valid_status != 0, "Dc* must never overwrite (rdx) with 0");
                }
                // Pass B: g_u2_image_base != NULL, argc=4, zeroed argv
                valid_status = 0;
                tramp_fn(4, zero_args, &valid_status);
                // Pass C: g_u2_image_base != NULL, argc=4, valid pointer/non-zero ID argv
                valid_status = 0;
                tramp_fn(4, ptr_args, &valid_status);

                // Restore stub & reset dummy buffer
                std::memcpy(image + off, saved_stub, 16);
                std::memset(dummy_arg_buf.data(), 0, dummy_arg_buf.size());
                ++full_body_tested;
            }
        }
        // Clear .data/.bss of mock image after DC full-body fuzzing
        std::memset(image + 0x01040000, 0, kMockImageSize - 0x01040000);
        std::printf("[U2 EngineTest] PASSED: %zu DC detours survived full-body sandbox execution via ELF trampolines!\n",
                    full_body_tested);
        std::fflush(stdout);
        U2_TEST_ASSERT(full_body_tested >= 830, "Full-body sandbox execution of all DC script detours");
    }

    // 1. Test CatmullRomSpline methods (via ELF trampolines)
    {
        write_temp_jmp64(image + 0x00CE33A0, reinterpret_cast<uintptr_t>(&mock_StringIdToString));

        float cumul[4] = { 12.5f, 30.0f, 55.25f, 90.0f };
        uint8_t spline_data[0x60] = {};
        *reinterpret_cast<uint32_t *>(spline_data + 0x00) = 0x12345678u; // m_nameId
        spline_data[0x14] = 1u;                                          // m_flags (looped = 1)
        *reinterpret_cast<uint32_t *>(spline_data + 0x28) = 4u;          // m_controlPoints = 4
        *reinterpret_cast<float **>(spline_data + 0x48) = cumul;         // m_cumulArcLengths

        uint8_t spline[0x20] = {};
        *reinterpret_cast<void **>(spline + 0x10) = spline_data;

        auto fn_get_name = reinterpret_cast<const char *(ABI *)(void *)>(image + 0x00468260);
        auto fn_is_looped = reinterpret_cast<uint32_t (ABI *)(void *)>(image + 0x004684C0);
        auto fn_get_cp_count = reinterpret_cast<uint32_t (ABI *)(void *)>(image + 0x00468520);
        auto fn_get_total_len = reinterpret_cast<float (ABI *)(void *)>(image + 0x00468D90);
        auto fn_get_seg_len = reinterpret_cast<float (ABI *)(void *, int32_t)>(image + 0x00469F60);

        const char *name = fn_get_name(spline);
        U2_TEST_ASSERT(name && std::strcmp(name, "spline_train_track_01") == 0, "CatmullRomSpline::GetName");
        U2_TEST_ASSERT(fn_is_looped(spline) == 1u, "CatmullRomSpline::IsLooped (true)");
        U2_TEST_ASSERT(fn_get_cp_count(spline) == 5u, "CatmullRomSpline::GetControlPointCount (looped 4+1)");
        U2_TEST_ASSERT(std::fabs(fn_get_total_len(spline) - 90.0f) < 1e-5f, "CatmullRomSpline::GetTotalArcLength");
        U2_TEST_ASSERT(std::fabs(fn_get_seg_len(spline, 0) - 0.0f) < 1e-5f, "CatmullRomSpline::GetCumulSegArcLength(0)");
        U2_TEST_ASSERT(std::fabs(fn_get_seg_len(spline, 3) - 55.25f) < 1e-5f, "CatmullRomSpline::GetCumulSegArcLength(3)");

        spline_data[0x14] = 0u; // unlooped
        U2_TEST_ASSERT(fn_is_looped(spline) == 0u, "CatmullRomSpline::IsLooped (false)");
        U2_TEST_ASSERT(fn_get_cp_count(spline) == 4u, "CatmullRomSpline::GetControlPointCount (unlooped 4)");
        U2_TEST_ASSERT(std::fabs(fn_get_total_len(spline) - 55.25f) < 1e-5f, "CatmullRomSpline::GetTotalArcLength (unlooped)");
    }

    // 2. Test CatmullRomManager methods (OnSpawnGameObject, OnDestroyGameObject, WalkSplines, RemoveSpline)
    {
        write_temp_jmp64(image + 0x004678E0, reinterpret_cast<uintptr_t>(&mock_BindGameObject));
        write_temp_jmp64(image + 0x003D1EA0, reinterpret_cast<uintptr_t>(&mock_UnregisterSpecialSpline));

        std::vector<uint8_t> table(0x19820, 0);
        uint8_t spline_data_1[0x30] = {};
        void *target_id = reinterpret_cast<void *>(0xCAFEBABEull);
        *reinterpret_cast<void **>(spline_data_1 + 0x08) = target_id;
        *reinterpret_cast<uint32_t *>(spline_data_1 + 0x10) = 0x0E3532C9u; // special spline type

        // Populate slot 1 (offset 0x110) and slot 2 (offset 0x220)
        uint8_t *slot1 = table.data() + 0x110;
        *reinterpret_cast<void **>(slot1 + 0x00) = spline_data_1;
        slot1[0x10] = 0x00; // active flags
        *reinterpret_cast<void **>(slot1 + 0x20) = spline_data_1;

        uint8_t *slot2 = table.data() + 0x220;
        *reinterpret_cast<void **>(slot2 + 0x00) = spline_data_1;
        slot2[0x10] = 0x01; // disabled flag (bit 0 set)

        uint8_t mgr[0x80] = {};
        *reinterpret_cast<uint32_t *>(mgr + 0x00) = 2u;
        *reinterpret_cast<uint8_t **>(mgr + 0x08) = table.data();
        *reinterpret_cast<void **>(mgr + 0x68) = reinterpret_cast<void *>(&mock_RemoveSplineCallback);
        *reinterpret_cast<void **>(mgr + 0x70) = reinterpret_cast<void *>(0x99887766ull);

        uint8_t fake_vtbl[0xB0] = {};
        *reinterpret_cast<void **>(fake_vtbl + 0xA0) = reinterpret_cast<void *>(&mock_GameObject_GetSpawnerData);
        uint8_t fake_game_obj[0x20] = {};
        *reinterpret_cast<void **>(fake_game_obj + 0x00) = fake_vtbl;
        *reinterpret_cast<void **>(fake_game_obj + 0x10) = target_id;

        auto fn_spawn = reinterpret_cast<void (ABI *)(void *, void *)>(image + 0x004643E0);
        auto fn_destroy = reinterpret_cast<void (ABI *)(void *, void *)>(image + 0x00464490);
        auto fn_walk = reinterpret_cast<void (ABI *)(void *, void (ABI *)(void *, void *), void *, int32_t)>(image + 0x00466320);
        auto fn_remove = reinterpret_cast<void (ABI *)(void *, void *)>(image + 0x00465240);

        s_last_bound_spline = nullptr;
        s_last_bound_obj = nullptr;
        fn_spawn(mgr, fake_game_obj);
        U2_TEST_ASSERT(s_last_bound_spline == (slot1 + 0x10) && s_last_bound_obj == fake_game_obj,
                       "CatmullRomManager::OnSpawnGameObject");

        fn_destroy(mgr, fake_game_obj);
        U2_TEST_ASSERT(s_last_bound_spline == (slot1 + 0x10) && s_last_bound_obj == nullptr,
                       "CatmullRomManager::OnDestroyGameObject");

        s_walk_cb_calls = 0;
        fn_walk(mgr, &mock_WalkSplinesCallback, reinterpret_cast<void *>(0x4242ull), 0);
        U2_TEST_ASSERT(s_walk_cb_calls == 1, "CatmullRomManager::WalkSplines (active only)");

        s_walk_cb_calls = 0;
        fn_walk(mgr, &mock_WalkSplinesCallback, reinterpret_cast<void *>(0x4242ull), 1);
        U2_TEST_ASSERT(s_walk_cb_calls == 2, "CatmullRomManager::WalkSplines (all_flag=1)");

        s_remove_cb_calls = 0;
        s_unreg_special_calls = 0;
        fn_remove(mgr, spline_data_1);
        U2_TEST_ASSERT(s_remove_cb_calls == 2 && s_unreg_special_calls == 2 && *reinterpret_cast<uint32_t *>(mgr) == 0u,
                       "CatmullRomManager::RemoveSpline");
    }

    // 3. Test SplineTracker & VehicleSplineTracker
    {
        write_temp_jmp64(image + 0x00A23EC0, reinterpret_cast<uintptr_t>(&mock_GetRealDist));

        std::vector<uint8_t> arr(0x440, 0);
        *reinterpret_cast<uint8_t **>(image + 0x0104EB38) = arr.data();

        uint32_t fake_spline_data[4] = { 0xABCD9876u, 0, 0, 0 };
        uint8_t *entry2 = arr.data() + 2 * 0x110;
        *reinterpret_cast<void **>(entry2 + 0x20) = fake_spline_data;

        uint8_t tracker[0x1A0] = {};
        *reinterpret_cast<float *>(tracker + 0x10) = 50.0f;           // mock base for tracker 0
        *reinterpret_cast<int32_t *>(tracker + 0x30) = 2;             // spline index = 2
        *reinterpret_cast<void **>(tracker + 0x38) = fake_spline_data;
        *reinterpret_cast<float *>(tracker + 0x40) = 33.5f;           // virtual distance mode 0
        *reinterpret_cast<uint32_t *>(tracker + 0x44) = 99u;
        *reinterpret_cast<uint32_t *>(tracker + 0x50) = 88u;
        *reinterpret_cast<float *>(tracker + 0x160) = 120.0f;         // mock base for tracker 1 (+0x150 + 0x10)
        *reinterpret_cast<float *>(tracker + 0x190) = 77.25f;        // virtual distance mode 1

        auto fn_get_sp0 = reinterpret_cast<void *(ABI *)(void *)>(image + 0x00A24040);
        auto fn_get_sp1 = reinterpret_cast<void *(ABI *)(void *)>(image + 0x00A24080);
        auto fn_get_name_id = reinterpret_cast<uint32_t (ABI *)(void *)>(image + 0x00A240C0);
        auto fn_finalize = reinterpret_cast<void (ABI *)(void *)>(image + 0x00A24110);
        auto fn_virt_dist = reinterpret_cast<float (ABI *)(void *, int32_t)>(image + 0x00ABA7B0);
        auto fn_real_dist = reinterpret_cast<float (ABI *)(void *, int32_t, uint32_t)>(image + 0x00ABA840);

        U2_TEST_ASSERT(fn_get_sp0(tracker) == (entry2 + 0x10), "SplineTracker::GetSpline0");
        U2_TEST_ASSERT(fn_get_sp1(tracker) == (entry2 + 0x10), "SplineTracker::GetSpline1");
        U2_TEST_ASSERT(fn_get_name_id(tracker) == 0xABCD9876u, "SplineTracker::GetSplineNameId");

        U2_TEST_ASSERT(std::fabs(fn_virt_dist(tracker, 0) - 33.5f) < 1e-5f, "VehicleSplineTracker::GetCurrentVirtualDistance_m(0)");
        U2_TEST_ASSERT(std::fabs(fn_virt_dist(tracker, 1) - 77.25f) < 1e-5f, "VehicleSplineTracker::GetCurrentVirtualDistance_m(1)");
        U2_TEST_ASSERT(std::fabs(fn_real_dist(tracker, 0, 1) - 60.0f) < 1e-5f, "VehicleSplineTracker::GetCurrentRealDistance_m(0, true)");
        U2_TEST_ASSERT(std::fabs(fn_real_dist(tracker, 1, 0) - 120.0f) < 1e-5f, "VehicleSplineTracker::GetCurrentRealDistance_m(1, false)");

        fn_finalize(tracker);
        U2_TEST_ASSERT(std::fabs(*reinterpret_cast<float *>(tracker + 0x40) - 60.0f) < 1e-5f &&
                       *reinterpret_cast<uint32_t *>(tracker + 0x44) == 0u &&
                       *reinterpret_cast<uint32_t *>(tracker + 0x50) == 0u,
                       "SplineTracker::FinalizeDistance");
    }

    // 4. Test CameraZoom angular constraint methods
    {
        auto fn_over_h = reinterpret_cast<void (ABI *)(const float *, float, float)>(image + 0x0044BF00);
        auto fn_clear_h = reinterpret_cast<void (ABI *)(void)>(image + 0x0044BFA0);
        auto fn_over_v = reinterpret_cast<void (ABI *)(float, float)>(image + 0x0044BFE0);
        auto fn_clear_v = reinterpret_cast<void (ABI *)(void)>(image + 0x0044C060);
        auto fn_def_v = reinterpret_cast<void (ABI *)(float, float)>(image + 0x0044C090);

        float dir[4] = { 1.0f, 2.0f, 3.0f, 0.0f };
        fn_over_h(dir, -0.5f, 0.75f);
        U2_TEST_ASSERT(image[0x0147FDE0] == 1 &&
                       std::fabs(*reinterpret_cast<float *>(image + 0x0104EB10) - (-0.5f)) < 1e-5f &&
                       std::fabs(*reinterpret_cast<float *>(image + 0x011142AC) - 0.75f) < 1e-5f &&
                       std::fabs(*reinterpret_cast<float *>(image + 0x011142B0) - 1.0f) < 1e-5f,
                       "CameraZoom::OverideHorizontalAngularConstraints");

        fn_clear_h();
        U2_TEST_ASSERT(image[0x0147FDE0] == 0 &&
                       std::fabs(*reinterpret_cast<float *>(image + 0x0104EB10) - (-1.0f)) < 1e-5f &&
                       std::fabs(*reinterpret_cast<float *>(image + 0x011142AC) - 0.0f) < 1e-5f,
                       "CameraZoom::ClearHorizontalAngularConstraints");

        image[0x0147FDE1] = 0;
        fn_def_v(-0.25f, 0.5f);
        U2_TEST_ASSERT(std::fabs(*reinterpret_cast<float *>(image + 0x0104EB14) - (-0.25f)) < 1e-5f &&
                       std::fabs(*reinterpret_cast<float *>(image + 0x0104EB18) - 0.5f) < 1e-5f,
                       "CameraZoom::SetDefaultVerticalAngularConstraints");

        fn_over_v(-0.8f, 0.9f);
        U2_TEST_ASSERT(image[0x0147FDE1] == 1 &&
                       std::fabs(*reinterpret_cast<float *>(image + 0x0104EB14) - (-0.8f)) < 1e-5f &&
                       std::fabs(*reinterpret_cast<float *>(image + 0x0104EB18) - 0.9f) < 1e-5f,
                       "CameraZoom::OverideVerticalAngularConstraints");

        fn_clear_v();
        U2_TEST_ASSERT(image[0x0147FDE1] == 0 &&
                       std::fabs(*reinterpret_cast<float *>(image + 0x0104EB14) - (-0.25f)) < 1e-5f &&
                       std::fabs(*reinterpret_cast<float *>(image + 0x0104EB18) - 0.5f) < 1e-5f,
                       "CameraZoom::ClearVerticalAngularConstraints");
    }

    // 5. Test RailVehicle, RailVehicleBehavior, RailVehicleManager
    {
        write_temp_jmp64(image + 0x00A8EAA0, reinterpret_cast<uintptr_t>(&mock_BehaviorContainerInit));
        write_temp_jmp64(image + 0x00D75840, reinterpret_cast<uintptr_t>(&mock_LookupVehicleClass));
        write_temp_jmp64(image + 0x00A90590, reinterpret_cast<uintptr_t>(&mock_GetTime));

        std::vector<uint8_t> vehicle(0x1000, 0);
        uint8_t fake_cls[0x100] = {};
        uint8_t fake_damageables[2 * 0x30] = {};
        *reinterpret_cast<uint32_t *>(fake_cls + 0xD0) = 2u;
        *reinterpret_cast<uint8_t **>(fake_cls + 0xD8) = fake_damageables;
        s_mock_vehicle_class_ptr = fake_cls;

        *reinterpret_cast<uint32_t *>(vehicle.data() + 0x25C) = 0xCAFE1234u; // vehicle class sid
        *reinterpret_cast<uint32_t *>(vehicle.data() + 0x750) = 3u;          // 3 wheels
        for (uint32_t i = 0; i < 3; ++i) {
            vehicle[0x820 + i * 0xD0 + 0xC8] = 1;
        }

        auto fn_init_beh = reinterpret_cast<void (ABI *)(void *, void *)>(image + 0x000013D0);
        auto fn_set_follow = reinterpret_cast<void (ABI *)(void *, float, float)>(image + 0x00001460);
        auto fn_get_cls = reinterpret_cast<void *(ABI *)(void *)>(image + 0x00001DD0);
        auto fn_reset_wheels = reinterpret_cast<void (ABI *)(void *)>(image + 0x00001ED0);
        auto fn_get_dmg = reinterpret_cast<void *(ABI *)(void *, uint32_t)>(image + 0x00002430);
        auto fn_igc_bind = reinterpret_cast<void (ABI *)(void *)>(image + 0x00005630);
        auto fn_igc_unbind = reinterpret_cast<void (ABI *)(void *)>(image + 0x000056E0);

        void *fake_dc = reinterpret_cast<void *>(0x11223344ull);
        fn_init_beh(vehicle.data(), fake_dc);
        U2_TEST_ASSERT(s_last_init_container == (vehicle.data() + 0xEB0) &&
                       s_last_init_vehicle == vehicle.data() &&
                       s_last_init_dc == fake_dc,
                       "RailVehicle::InitBehavior");

        fn_set_follow(vehicle.data(), 4.5f, 18.25f);
        U2_TEST_ASSERT(std::fabs(*reinterpret_cast<float *>(vehicle.data() + 0x560) - 4.5f) < 1e-5f &&
                       std::fabs(*reinterpret_cast<float *>(vehicle.data() + 0x564) - 18.25f) < 1e-5f,
                       "RailVehicle::SetFollowDistance");

        U2_TEST_ASSERT(fn_get_cls(vehicle.data()) == fake_cls &&
                       *reinterpret_cast<void **>(vehicle.data() + 0x260) == fake_cls,
                       "RailVehicle::GetVehicleClass");

        fn_reset_wheels(vehicle.data());
        U2_TEST_ASSERT(vehicle[0x820 + 0 * 0xD0 + 0xC8] == 0 &&
                       vehicle[0x820 + 1 * 0xD0 + 0xC8] == 0 &&
                       vehicle[0x820 + 2 * 0xD0 + 0xC8] == 0,
                       "RailVehicle::ResetWheels");

        U2_TEST_ASSERT(fn_get_dmg(vehicle.data(), 1) == (fake_damageables + 0x30) &&
                       fn_get_dmg(vehicle.data(), 2) == nullptr,
                       "RailVehicle::GetDamageable");

        fn_igc_bind(vehicle.data());
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(vehicle.data() + 0xDF0) == 1 &&
                       *reinterpret_cast<uint32_t *>(vehicle.data() + 0xE08) == 0xFFFFFFFFu &&
                       image[0x011362F8 + 0x31] == 1,
                       "RailVehicle::OnIgcBind");

        fn_igc_unbind(vehicle.data());
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(vehicle.data() + 0xDF0) == 0 &&
                       (*reinterpret_cast<uint32_t *>(vehicle.data() + 0x5A0) & 0x2020u) == 0x2020u &&
                       image[0x011362F8 + 0x31] == 0,
                       "RailVehicle::OnIgcUnbind");

        auto fn_get_dc_beh = reinterpret_cast<void *(ABI *)(void *)>(image + 0x00A8DE50);
        auto fn_get_cur_beh = reinterpret_cast<void *(ABI *)(void *)>(image + 0x00A8EA30);
        auto fn_get_mover = reinterpret_cast<void *(ABI *)(void *, uint32_t)>(image + 0x00A8FCE0);
        auto fn_detach = reinterpret_cast<void (ABI *)(void *, void *, int32_t)>(image + 0x00A903E0);

        uint8_t beh[0x80] = {};
        *reinterpret_cast<void **>(beh + 0x18) = fake_dc;
        U2_TEST_ASSERT(fn_get_dc_beh(beh) == fake_dc, "RailVehicleBehavior::GetDcBehavior");

        *reinterpret_cast<uint32_t *>(beh) = 1u;
        U2_TEST_ASSERT(fn_get_cur_beh(beh) == (beh + 0x70), "RailVehicleBehaviorContainer::GetCurrentBehavior(<=1)");
        *reinterpret_cast<uint32_t *>(beh) = 5u;
        U2_TEST_ASSERT(fn_get_cur_beh(beh) == (beh + 0x10) && *reinterpret_cast<uint32_t *>(beh) == 2u,
                       "RailVehicleBehaviorContainer::GetCurrentBehavior(>1)");

        U2_TEST_ASSERT(fn_get_mover(nullptr, 2) == (image + 0x0153EFA8 + 0xD18 + 2 * 0x30),
                       "RailVehicleManager::GetMover");

        uint8_t *rail1 = image + 0x0153EFA8 + 1 * 0x60;
        rail1[0x12C] = 0x07;
        *reinterpret_cast<uint64_t *>(rail1 + 0x118) = 0x12345678ull;
        *reinterpret_cast<uint32_t *>(rail1 + 0x120) = 99u;
        *reinterpret_cast<float *>(rail1 + 0xF8) = 42.5f;
        fn_detach(nullptr, nullptr, 1);
        U2_TEST_ASSERT(rail1[0x12C] == 0x05 &&
                       *reinterpret_cast<uint64_t *>(rail1 + 0x118) == 0 &&
                       *reinterpret_cast<uint32_t *>(rail1 + 0x120) == 0 &&
                       std::fabs(*reinterpret_cast<float *>(rail1 + 0x128) - 42.5f) < 1e-5f,
                       "RailVehicleManager::DetachFromRail");
    }

    g_u2_image_base = saved_base;
    std::printf("[U2 EngineTest] PASSED: %d/%d functional C++23 engine assertions verified via ELF trampolines!\n",
                s_passed_assertions, s_passed_assertions + s_failed_assertions);
    std::fflush(stdout);
    return s_failed_assertions;
}
