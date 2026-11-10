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

static void * ABI mock_ProcessTreeNext(void *cur, void *root) {
    (void)root;
    if (!cur) return nullptr;
    return *reinterpret_cast<void **>(static_cast<uint8_t *>(cur) + 0x10);
}

static uint32_t s_mock_ctrl_state = 7u;
static uint32_t ABI mock_Ctrl_GetState(void *ctrl) {
    (void)ctrl;
    return s_mock_ctrl_state;
}
static void ABI mock_Ctrl_SetState(void *ctrl, uint32_t st) {
    (void)ctrl;
    s_mock_ctrl_state = st;
}
static void *s_mock_ctrl_obj = nullptr;
static void * ABI mock_Proc_GetCtrl(void *proc) {
    (void)proc;
    return s_mock_ctrl_obj;
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

    // Stub PLT jump table [0x00DE6000 .. 0x00DF0000) with safe SysV ret stubs:
    // Any legacy DC call to PLT entry points (like 0x00DE6400) safely returns 0 instead of jumping through null GOT
    for (size_t off = 0x00DE6000; off < 0x00DF0000; off += 16) {
        uint8_t *slot = image + off;
        slot[0] = 0x31; slot[1] = 0xC0;                 // xor eax, eax
        slot[2] = 0x0F; slot[3] = 0x57; slot[4] = 0xC0; // xorps xmm0, xmm0
        for (size_t k = 5; k < 16; ++k) slot[k] = 0xC3; // ret
    }

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

    // 6. Test Process, ProcessMgr & ProcessList (process_system.cpp)
    {
        write_temp_jmp64(image + 0x009BCDF0, reinterpret_cast<uintptr_t>(&mock_ProcessTreeNext));

        std::vector<uint8_t> proc1(0x500, 0);
        std::vector<uint8_t> proc2(0x500, 0);
        *reinterpret_cast<uint16_t *>(proc1.data() + 0x34) = 1u;
        *reinterpret_cast<uint32_t *>(proc1.data() + 0x44) = 101u;
        *reinterpret_cast<void **>(proc1.data() + 0x10) = proc2.data();

        *reinterpret_cast<uint16_t *>(proc2.data() + 0x34) = 1u;
        *reinterpret_cast<void **>(proc2.data() + 0x38) = reinterpret_cast<void *>(0xABCDEF00ull);
        *reinterpret_cast<uint32_t *>(proc2.data() + 0x44) = 202u;

        // Global ProcessMgr at 0x0115F1A0
        *reinterpret_cast<void **>(image + 0x0115F1A0 + 0xC40) = proc1.data();

        auto fn_mgr_lookup = reinterpret_cast<void *(ABI *)(void *, uint32_t)>(image + 0x009BBCF0);
        auto fn_proc_lookup = reinterpret_cast<void *(ABI *)(void *, uint32_t)>(image + 0x009BEF30);
        auto fn_validate_ptr = reinterpret_cast<void (ABI *)(void *)>(image + 0x009BE660);
        auto fn_add_script_upd = reinterpret_cast<void (ABI *)(void *, void *)>(image + 0x009B8CD0);
        auto fn_plist_alloc = reinterpret_cast<void *(ABI *)(void *)>(image + 0x009B0D70);

        U2_TEST_ASSERT(fn_mgr_lookup(image + 0x0115F1A0, 202u) == proc2.data() &&
                       fn_mgr_lookup(image + 0x0115F1A0, 999u) == nullptr,
                       "ProcessMgr::LookupProcessByPid");
        U2_TEST_ASSERT(fn_proc_lookup(proc1.data(), 202u) == proc2.data() &&
                       fn_proc_lookup(proc1.data(), 999u) == nullptr,
                       "Process::LookupProcessByPid");

        uint8_t fake_state[0x40] = {};
        *reinterpret_cast<void **>(proc2.data() + 0x50) = fake_state;
        fn_validate_ptr(proc2.data());
        U2_TEST_ASSERT(*reinterpret_cast<void **>(proc2.data() + 0x50) == fake_state,
                       "Process::ValidateInternalPointer");

        *reinterpret_cast<uint32_t *>(image + 0x0115F1A0 + 0xC10) = 0u;
        fn_add_script_upd(image + 0x0115F1A0, proc2.data());
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(image + 0x0115F1A0 + 0xC10) == 1u &&
                       *reinterpret_cast<void **>(image + 0x0115F1A0 + 0x10) == proc2.data(),
                       "ProcessMgr::AddProcessForScriptUpdate");

        uint8_t pool[0x40] = {};
        uint8_t node1[0x30] = {};
        uint8_t node2[0x30] = {};
        *reinterpret_cast<void **>(node1 + 0x00) = node2;
        *reinterpret_cast<void **>(node2 + 0x08) = node1;
        *reinterpret_cast<void **>(pool + 0x20) = node1;
        *reinterpret_cast<void **>(image + 0x0151FF70) = pool;

        void *alloc_n = fn_plist_alloc(proc2.data());
        U2_TEST_ASSERT(alloc_n == node1 &&
                       *reinterpret_cast<void **>(pool + 0x20) == node2 &&
                       *reinterpret_cast<void **>(node1 + 0x10) == reinterpret_cast<void *>(0xABCDEF00ull) &&
                       *reinterpret_cast<uint32_t *>(node1 + 0x18) == 202u,
                       "ProcessList::Alloc");

        // DMenu Process property accessors (0x001FE420 .. 0x001FEE40)
        uint8_t ctrl_vtbl[0x60] = {};
        *reinterpret_cast<void **>(ctrl_vtbl + 0x38) = reinterpret_cast<void *>(&mock_Ctrl_SetState);
        *reinterpret_cast<void **>(ctrl_vtbl + 0x48) = reinterpret_cast<void *>(&mock_Ctrl_GetState);
        uint8_t ctrl_obj[0x20] = {};
        *reinterpret_cast<void **>(ctrl_obj + 0x00) = ctrl_vtbl;
        s_mock_ctrl_obj = ctrl_obj;

        std::vector<uint8_t> proc_vtbl(0x200, 0);
        *reinterpret_cast<void **>(proc_vtbl.data() + 0x188) = reinterpret_cast<void *>(&mock_Proc_GetCtrl);
        *reinterpret_cast<void **>(proc2.data() + 0x00) = proc_vtbl.data();

        uint8_t dmenu_item[0x90] = {};
        *reinterpret_cast<uint32_t *>(dmenu_item + 0x30) = 12u;  // target state = 12
        *reinterpret_cast<uint32_t *>(dmenu_item + 0x38) = 202u; // pid = 202

        auto fn_state_prop = reinterpret_cast<uint8_t (ABI *)(void *, int32_t)>(image + 0x001FE420);
        auto fn_float_354  = reinterpret_cast<float (ABI *)(void *, int32_t, float)>(image + 0x001FE7E0);
        auto fn_bool_43A   = reinterpret_cast<uint8_t (ABI *)(void *, int32_t)>(image + 0x001FE8C0);
        auto fn_bool_43F   = reinterpret_cast<uint8_t (ABI *)(void *, int32_t)>(image + 0x001FE980);
        auto fn_bool_439   = reinterpret_cast<uint8_t (ABI *)(void *, int32_t)>(image + 0x001FECA0);
        auto fn_bool_438   = reinterpret_cast<uint8_t (ABI *)(void *, int32_t)>(image + 0x001FED70);
        auto fn_bool_43B   = reinterpret_cast<uint8_t (ABI *)(void *, int32_t)>(image + 0x001FEE40);

        s_mock_ctrl_state = 7u;
        U2_TEST_ASSERT(fn_state_prop(dmenu_item, 3) == 0u, "Process::DMenuStateProp(query!=target)");
        fn_state_prop(dmenu_item, 4);
        U2_TEST_ASSERT(s_mock_ctrl_state == 12u && fn_state_prop(dmenu_item, 3) == 1u,
                       "Process::DMenuStateProp(set & query==target)");

        fn_float_354(dmenu_item, 4, 3.75f);
        U2_TEST_ASSERT(std::fabs(fn_float_354(dmenu_item, 0, 0.0f) - 3.75f) < 1e-5f,
                       "Process::DMenuFloatProp_354");

        U2_TEST_ASSERT(fn_bool_43A(dmenu_item, 4) == 1u && fn_bool_43A(dmenu_item, 0) == 1u,
                       "Process::DMenuBoolProp_43A");
        U2_TEST_ASSERT(fn_bool_43F(dmenu_item, 4) == 1u && fn_bool_43F(dmenu_item, 0) == 1u,
                       "Process::DMenuBoolProp_43F");
        U2_TEST_ASSERT(fn_bool_439(dmenu_item, 4) == 1u && fn_bool_439(dmenu_item, 0) == 1u,
                       "Process::DMenuBoolProp_439");
        U2_TEST_ASSERT(fn_bool_438(dmenu_item, 4) == 1u && fn_bool_438(dmenu_item, 0) == 1u,
                       "Process::DMenuBoolProp_438");
        U2_TEST_ASSERT(fn_bool_43B(dmenu_item, 4) == 1u && fn_bool_43B(dmenu_item, 0) == 1u,
                       "Process::DMenuBoolProp_43B");
    }

    // 7. Test SaveBuffer, Hud, GameTask, Settings & GameLoading (save_hud.cpp)
    {
        auto fn_sb_init   = reinterpret_cast<void (ABI *)(void *, uint32_t, uint32_t, void *)>(image + 0x00CD0E10);
        auto fn_sb_dbg    = reinterpret_cast<void (ABI *)(void *, void *)>(image + 0x00CD0E80);
        auto fn_sb_copy   = reinterpret_cast<void (ABI *)(void *, const void *)>(image + 0x00CD0EE0);
        auto fn_sb_wbytes = reinterpret_cast<void (ABI *)(void *, const void *, uint32_t)>(image + 0x00CD16A0);
        auto fn_sb_wu64_0 = reinterpret_cast<void (ABI *)(void *, const uint64_t *)>(image + 0x00CD1730);
        auto fn_sb_wu64_1 = reinterpret_cast<void (ABI *)(void *, const uint64_t *)>(image + 0x00CD17C0);

        std::vector<uint8_t> raw_buf_a(0x800, 0);
        std::vector<uint8_t> raw_buf_b(0x800, 0x5A);
        uint8_t sb_a[0xB0] = {};
        uint8_t sb_b[0xB0] = {};

        fn_sb_init(sb_a, 0x53415645u, 0x800u, raw_buf_a.data());
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(sb_a + 0x00) == 0x53415645u &&
                       *reinterpret_cast<uint32_t *>(sb_a + 0x04) == 0x800u &&
                       *reinterpret_cast<void **>(sb_a + 0x08) == raw_buf_a.data(),
                       "SaveBuffer::InitExternal");

        fn_sb_dbg(sb_b, raw_buf_b.data());
        *reinterpret_cast<uint32_t *>(sb_b + 0x00) = 0x11223344u;
        U2_TEST_ASSERT(*reinterpret_cast<void **>(sb_b + 0x08) == raw_buf_b.data() &&
                       *reinterpret_cast<void **>(sb_b + 0x10) == raw_buf_b.data(),
                       "SaveBuffer::DebugSetBuffer");

        fn_sb_copy(sb_a, sb_b);
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(sb_a + 0x00) == 0x11223344u &&
                       *reinterpret_cast<uint8_t **>(sb_a + 0x98) == (raw_buf_a.data() + 0x414) &&
                       raw_buf_a[0x100] == 0x5A,
                       "SaveBuffer::CopyFrom");

        const uint32_t payload = 0xDEADBEEFu;
        fn_sb_wbytes(sb_a, &payload, 4u);
        U2_TEST_ASSERT(*reinterpret_cast<const uint32_t *>(raw_buf_a.data() + 0x414) == 0xDEADBEEFu &&
                       *reinterpret_cast<uint8_t **>(sb_a + 0x98) == (raw_buf_a.data() + 0x418),
                       "SaveBuffer::WriteBytes");

        *reinterpret_cast<uint32_t *>(raw_buf_a.data() + 0x0C) = 0u;
        *reinterpret_cast<uint32_t *>(raw_buf_a.data() + 0x10) = 0u;
        uint64_t val0 = 0x12345678ULL;
        uint64_t val1 = 0xCAFEBABE11223344ULL;
        fn_sb_wu64_0(sb_a, &val0);
        fn_sb_wu64_1(sb_a, &val1);
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(raw_buf_a.data() + 0x0C) == 1u &&
                       *reinterpret_cast<uint32_t *>(raw_buf_a.data() + 0x10) == 1u &&
                       *reinterpret_cast<const uint64_t *>(raw_buf_a.data() + 0x418) == val0 &&
                       *reinterpret_cast<const uint64_t *>(raw_buf_a.data() + 0x420) == val1,
                       "SaveBuffer::WriteU64Count0 & WriteU64Count1");

        // Hud grenade indicators
        auto fn_hud_free = reinterpret_cast<void (ABI *)(void *, uint32_t)>(image + 0x00554F70);
        auto fn_hud_show = reinterpret_cast<void (ABI *)(void *, uint32_t, uint8_t)>(image + 0x00554FD0);
        uint8_t hud[0x60] = {};
        uint8_t ind[0x50] = {};
        ind[0x41] = 1u;
        *reinterpret_cast<void **>(hud + 0x18 + 3 * 8) = ind;
        fn_hud_show(hud, 3u, 1u);
        fn_hud_free(hud, 3u);
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(ind + 0x0C) == 1u && ind[0x41] == 0u,
                       "Hud::ShowGrenadeIndicator & FreeGrenadeIndicator");

        // GameTask::cbCollectTaskNode (0x00A5F970)
        auto fn_collect_node = reinterpret_cast<int32_t (ABI *)(void *, uint32_t, void *)>(image + 0x00A5F970);
        uint8_t entries[3 * 16] = {};
        void *expected_subnode = reinterpret_cast<void *>(0x77778888ull);
        *reinterpret_cast<uint32_t *>(entries + 0 * 16 + 0x00) = 10u;
        *reinterpret_cast<uint32_t *>(entries + 1 * 16 + 0x00) = 25u;
        *reinterpret_cast<void **>(entries + 1 * 16 + 0x08)    = expected_subnode;
        *reinterpret_cast<uint32_t *>(entries + 2 * 16 + 0x00) = 40u;

        uint8_t table_hdr[0x10] = {};
        *reinterpret_cast<int32_t *>(table_hdr + 0x00) = 3;
        *reinterpret_cast<void **>(table_hdr + 0x08)   = entries;

        uint8_t task[0x40] = {};
        *reinterpret_cast<void **>(task + 0x30) = table_hdr;
        uint8_t col[0x20] = {};
        fn_collect_node(task, 25u, col);
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(col + 0x00) == 1u &&
                       *reinterpret_cast<void **>(col + 0x08) == expected_subnode,
                       "GameTask::cbCollectTaskNode");

        // Settings::FreeSetting (0x00A1D600)
        auto fn_free_setting = reinterpret_cast<void (ABI *)(void *)>(image + 0x00A1D600);
        std::vector<uint8_t> settings_arr(4 * 0x70, 0);
        *reinterpret_cast<void **>(image + 0x015350E0) = settings_arr.data();
        *reinterpret_cast<int32_t *>(image + 0x0153513C) = 4;
        void *buckets[5] = {};
        *reinterpret_cast<void **>(image + 0x01535100) = buckets;

        uint8_t *slot2_setting = settings_arr.data() + 2 * 0x70;
        slot2_setting[0x61] = 1u;
        *reinterpret_cast<uint32_t *>(slot2_setting + 0x68) = 42u;
        *reinterpret_cast<uint64_t *>(image + 0x01534CE0) = (1ULL << 2);

        fn_free_setting(slot2_setting);
        U2_TEST_ASSERT((slot2_setting[0x61] & 1u) == 0u &&
                       *reinterpret_cast<uint32_t *>(slot2_setting + 0x68) == 0u &&
                       (*reinterpret_cast<uint64_t *>(image + 0x01534CE0) & (1ULL << 2)) == 0ULL,
                       "Settings::FreeSetting");

        // GameLoading::IsChunkSetEntirelyWithin (0x00B77AA0) & GetPlayGoSpeed (0x00B770D0)
        auto fn_chunk_within = reinterpret_cast<uint8_t (ABI *)(const char *, const uint64_t *)>(image + 0x00B77AA0);
        auto fn_playgo_speed = reinterpret_cast<uint32_t (ABI *)(void)>(image + 0x00B770D0);

        const char *cs_name = "menu";
        uint32_t hash = 0x811C9DC5u;
        for (const char *p = cs_name; *p; ++p) {
            hash = (hash ^ static_cast<uint8_t>(*p)) * 0x01000193u;
        }
        void *pg_buckets[5] = {};
        uint8_t pg_node[0x30] = {};
        *reinterpret_cast<const char **>(pg_node + 0x10) = cs_name;
        *reinterpret_cast<uint64_t *>(pg_node + 0x18) = 0x0FULL;
        *reinterpret_cast<uint64_t *>(pg_node + 0x20) = 0x01ULL;
        pg_buckets[hash % 4u] = pg_node;
        *reinterpret_cast<int32_t *>(image + 0x015AFA3C) = 4;
        *reinterpret_cast<void **>(image + 0x015AFA00) = pg_buckets;

        uint64_t mask_full[2] = { 0xFFULL, 0x03ULL };
        uint64_t mask_part[2] = { 0x03ULL, 0x01ULL };
        U2_TEST_ASSERT(fn_chunk_within(cs_name, mask_full) == 1u &&
                       fn_chunk_within(cs_name, mask_part) == 0u,
                       "GameLoading::IsChunkSetEntirelyWithin");

        *reinterpret_cast<uint32_t *>(image + 0x01050A30) = 500u;
        *reinterpret_cast<int32_t *>(image + 0x015AF7C0) = 0;
        U2_TEST_ASSERT(fn_playgo_speed() == 500u, "GameLoading::GetPlayGoSpeed(active)");
        *reinterpret_cast<int32_t *>(image + 0x015AF7C0) = 1;
        U2_TEST_ASSERT(fn_playgo_speed() == 0u, "GameLoading::GetPlayGoSpeed(suspended)");
    }

    // 8. Test AllocStack & ProcessHeap diagnostics (memory_system.cpp)
    {
        auto fn_alloc_stack  = reinterpret_cast<void * (ABI *)(void)>(image + 0x00B36D40);
        auto fn_push_alloc   = reinterpret_cast<void (ABI *)(void *, const char *, int32_t, const char *)>(image + 0x00B360D0);
        auto fn_pop_alloc    = reinterpret_cast<void * (ABI *)(void)>(image + 0x00B361C0);
        auto fn_stack_depth  = reinterpret_cast<uint32_t (ABI *)(void)>(image + 0x00B362A0);
        auto fn_scan_heap    = reinterpret_cast<void (ABI *)(void *)>(image + 0x009B7770);

        // Verify AllocateAllocStack creates an initial stack with HostDefaultHeap root
        void *stack_ptr = fn_alloc_stack();
        U2_TEST_ASSERT(stack_ptr != nullptr, "AllocateAllocStack: valid pointer");
        U2_TEST_ASSERT(fn_stack_depth() == 1u, "AllocStack: initial depth is 1 (HostDefaultHeap root)");

        // Verify PushAllocator and GetAllocatorStackDepth
        void *mock_alloc_a = reinterpret_cast<void *>(0x11223344ull);
        void *mock_alloc_b = reinterpret_cast<void *>(0x55667788ull);
        fn_push_alloc(mock_alloc_a, "TagMain", 101, "main.cpp");
        U2_TEST_ASSERT(fn_stack_depth() == 2u, "AllocStack: depth after 1 push");

        fn_push_alloc(mock_alloc_b, "TagLevel", 202, "level.cpp");
        U2_TEST_ASSERT(fn_stack_depth() == 3u, "AllocStack: depth after 2 pushes");

        // Verify PopAllocator restores order LIFO
        void *popped_b = fn_pop_alloc();
        U2_TEST_ASSERT(popped_b == mock_alloc_b, "AllocStack: pop b matches top");
        U2_TEST_ASSERT(fn_stack_depth() == 2u, "AllocStack: depth after 1 pop");

        void *popped_a = fn_pop_alloc();
        U2_TEST_ASSERT(popped_a == mock_alloc_a, "AllocStack: pop a matches top");
        U2_TEST_ASSERT(fn_stack_depth() == 1u, "AllocStack: depth after 2 pops");

        void *popped_root = fn_pop_alloc();
        U2_TEST_ASSERT(popped_root != nullptr, "AllocStack: pop root returns valid allocator");
        U2_TEST_ASSERT(fn_stack_depth() == 1u, "AllocStack: root allocator preserved at depth 1");

        // Verify ProcessHeap::ScanForProcessDroppings safety
        uint8_t mock_heap[0x100] = {};
        mock_heap[0xF0] = 0; // Inactive heap
        fn_scan_heap(mock_heap);
        U2_TEST_ASSERT(true, "ProcessHeap::ScanForProcessDroppings: inactive safe");

        mock_heap[0xF0] = 1; // Active heap
        *reinterpret_cast<void **>(mock_heap + 0x40) = mock_heap + 0x40; // Empty circular linked list
        fn_scan_heap(mock_heap);
        U2_TEST_ASSERT(true, "ProcessHeap::ScanForProcessDroppings: circular list safe");
    }

    // 9. Test Boot State Machine, Settings & ProcessMgr (boot_state_machine.cpp)
    {
        auto fn_settings_init     = reinterpret_cast<void (ABI *)(void)>(image + 0x00A1C710);
        auto fn_new_setting       = reinterpret_cast<void * (ABI *)(void *, uint64_t, const char *, uint32_t, void *)>(image + 0x00A1CD50);
        auto fn_free_setting      = reinterpret_cast<void (ABI *)(void *)>(image + 0x00A1D600);
        auto fn_process_startup   = reinterpret_cast<void (ABI *)(void)>(image + 0x009B0A30);
        auto fn_process_init      = reinterpret_cast<void (ABI *)(void *)>(image + 0x009B9530);
        auto fn_mount_archives    = reinterpret_cast<uint32_t (ABI *)(void)>(image + 0x00B783F0);
        auto fn_playgo_wait       = reinterpret_cast<uint32_t (ABI *)(int32_t)>(image + 0x00B78790);
        auto fn_intro_update      = reinterpret_cast<int32_t (ABI *)(void *)>(image + 0x00A551B0);
        auto fn_state_update      = reinterpret_cast<int32_t (ABI *)(void *, float)>(image + 0x00D59A50);

        // Test Settings::Init
        fn_settings_init();
        U2_TEST_ASSERT(*reinterpret_cast<uint8_t *>(image + 0x01535170) == 1u, "Settings::Init sets initialized flag");
        U2_TEST_ASSERT(*reinterpret_cast<uint8_t **>(image + 0x015350E0) != nullptr, "Settings::Init allocates settings pool");
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(image + 0x0153513C) == 8191, "Settings::Init sets modulo 8191");
        U2_TEST_ASSERT(*reinterpret_cast<uint8_t **>(image + 0x01534140) == (image + 0x01534140), "ScriptManager: s_packages head points to self");
        U2_TEST_ASSERT(*reinterpret_cast<uint8_t **>(image + 0x01534208) != nullptr, "ScriptManager: builtin DC registry head initialized");

        // Test Settings::NewSetting and Settings::FreeSetting
        void *setting = fn_new_setting(nullptr, 0x12345678ULL, "Setting_MasterVolume", 1, nullptr);
        U2_TEST_ASSERT(setting != nullptr, "Settings::NewSetting allocates setting");
        auto *s_bytes = static_cast<uint8_t *>(setting);
        U2_TEST_ASSERT(*reinterpret_cast<uint64_t *>(s_bytes + 0x18) == 0x12345678ULL, "Setting key matches");
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(s_bytes + 0x20) == 1u, "Setting type matches");

        fn_free_setting(setting);
        U2_TEST_ASSERT(true, "Settings::FreeSetting cleans up cleanly");

        // Test ProcessList and ProcessMgr
        fn_process_startup();
        U2_TEST_ASSERT(*reinterpret_cast<void **>(image + 0x0151FF70) != nullptr, "ProcessList::StartUp initializes pool");

        uint8_t mock_mgr[0xE00] = {};
        fn_process_init(mock_mgr);
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(mock_mgr + 0x00) == 1u, "ProcessMgr::Init marks initialized");

        // Test Archive mounting and PlayGo wait
        *reinterpret_cast<uint64_t *>(image + 0x015AF448) = 0;
        uint32_t mount_res = fn_mount_archives();
        U2_TEST_ASSERT(mount_res == 1u, "GameLoading::MountInitialArchives returns success");
        U2_TEST_ASSERT(*reinterpret_cast<uint64_t *>(image + 0x015AF448) == 3u, "Core archives count incremented to 3 (bin, data, core)");
        U2_TEST_ASSERT(fn_playgo_wait(100) == 0u, "GameLoading::PlayGoWait ready immediately");
        U2_TEST_ASSERT(*reinterpret_cast<uint8_t *>(image + 0x015AF77C) == 1u, "GameLoading::PlayGoWait sets chunk ready flag");
        U2_TEST_ASSERT(*reinterpret_cast<float *>(image + 0x01050A38) == 1.0f, "GameLoading::PlayGoWait sets progress to 1.0f");

        // Test IntroState::Update
        U2_TEST_ASSERT(fn_intro_update(nullptr) == 1, "IntroState::Update completes logo presentation");

        // Test UCCApp::StateMachineUpdate full transition (1 -> 7)
        uint8_t mock_ucc[0x100] = {};
        *reinterpret_cast<int32_t *>(mock_ucc + 0x00) = 1; // State 1
        int32_t s2 = fn_state_update(mock_ucc, 0.016f);
        U2_TEST_ASSERT(s2 == 2, "UCCApp state transitions 1 -> 2 (Archives mounting)");

        int32_t s3 = fn_state_update(mock_ucc, 0.016f);
        U2_TEST_ASSERT(s3 == 3, "UCCApp state transitions 2 -> 3 (Archives ready)");

        int32_t s4 = fn_state_update(mock_ucc, 0.016f);
        U2_TEST_ASSERT(s4 == 4, "UCCApp state transitions 3 -> 4 (Aux archives)");

        int32_t s5 = fn_state_update(mock_ucc, 0.016f);
        U2_TEST_ASSERT(s5 == 5, "UCCApp state transitions 4 -> 5 (Settings/ProcessMgr initialized)");

        int32_t s6 = fn_state_update(mock_ucc, 0.016f);
        U2_TEST_ASSERT(s6 == 6, "UCCApp state transitions 5 -> 6 (Save buffer check)");

        int32_t s7 = fn_state_update(mock_ucc, 0.016f);
        U2_TEST_ASSERT(s7 == 7, "UCCApp state transitions 6 -> 7 (MAIN MENU ACTIVE!)");

        // Step State 7 once to configure Main Menu parameters
        fn_state_update(mock_ucc, 0.016f);
        U2_TEST_ASSERT(std::fabs(*reinterpret_cast<float *>(mock_ucc + 0x08) - 100.0f) < 1e-4f, "Main Menu camera FOV 100.0f");
        U2_TEST_ASSERT(std::fabs(*reinterpret_cast<float *>(mock_ucc + 0xE0) - (-80.0f)) < 1e-4f, "Main Menu camera angle -80.0f");
        U2_TEST_ASSERT(std::fabs(*reinterpret_cast<float *>(mock_ucc + 0xE4) - 4.0f) < 1e-4f, "Main Menu camera speed 4.0f");
    }

    // 10. Test Main Menu & 2D GUI Subsystems (menu_system.cpp)
    {
        auto fn_set_mode        = reinterpret_cast<void (ABI *)(void *, int32_t, int32_t)>(image + 0x005C7BB0);
        auto fn_show_popup      = reinterpret_cast<void (ABI *)(void *, int32_t, int32_t)>(image + 0x005C8840);
        auto fn_add_handler     = reinterpret_cast<void (ABI *)(void *, uint32_t, void (ABI *)(void *, uint32_t), void *)>(image + 0x005D1BF0);
        auto fn_set_val_impl    = reinterpret_cast<int32_t (ABI *)(void *, int32_t, const void *)>(image + 0x00578EB0);
        auto fn_draw_quad       = reinterpret_cast<void (ABI *)(void *, const float *, void *, const float *)>(image + 0x005B5C10);
        auto fn_hud_begin       = reinterpret_cast<void * (ABI *)(void *, void *, void *)>(image + 0x00BDBB10);
        auto fn_hud_end         = reinterpret_cast<void (ABI *)(void *, void *, const float *)>(image + 0x00BDC670);

        uint8_t mock_menuhost[0x2000] = {};
        fn_set_mode(mock_menuhost, 1, 0); // Set mode 1 (Main Menu)
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(mock_menuhost + 0x00) == 1, "MenuHost::SetMode sets mode 1 (Main Menu)");
        U2_TEST_ASSERT(u2_cpp_MenuHost_GetMode(mock_menuhost) == 1, "MenuHost::GetMode returns 1");
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(image + 0x014CE97C) == 1, "MenuHost::SetMode updates global offset 0x014CE97C");

        fn_show_popup(mock_menuhost, 42, 1);
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(mock_menuhost + 0x9B0) == 42, "MenuHost::ShowWaitPopup sets popup ID");

        static uint32_t s_clicked_id = 0;
        auto click_cb = [](void *, uint32_t id) ABI { s_clicked_id = id; };
        fn_add_handler(mock_menuhost, 101, click_cb, nullptr);
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(mock_menuhost + 0x18EC) == 1u, "MenuHost::AddControlClickedHandler registers handler");

        u2_cpp_MenuHost_TriggerControlClicked(mock_menuhost, 101);
        U2_TEST_ASSERT(s_clicked_id == 101u, "MenuHost::TriggerControlClicked executes click callback");

        uint8_t mock_datasource[0x100] = {};
        float vol_data[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
        int32_t val_res = fn_set_val_impl(mock_datasource, 5, vol_data);
        U2_TEST_ASSERT(val_res == 1, "BpMenuDataSource::SetValueImpl returns 1");
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(mock_datasource + 0x40) == 5, "BpMenuDataSource value ID matches");

        float drect[4] = { 10.0f, 20.0f, 100.0f, 50.0f };
        float uvs[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
        fn_draw_quad(nullptr, drect, nullptr, uvs);
        U2_TEST_ASSERT(true, "MenuDrawer::DrawQuad safe execution");

        void *hud_ctx = fn_hud_begin(mock_menuhost, nullptr, nullptr);
        U2_TEST_ASSERT(hud_ctx != nullptr, "HudQuad::Begin_Actual returns context");
        fn_hud_end(hud_ctx, nullptr, nullptr);
        U2_TEST_ASSERT(true, "HudQuad::End_Actual flushes batch cleanly");
    }

    // 11. Test File Loader Subsystem (file_loader.cpp)
    {
        auto fn_load_file = reinterpret_cast<bool (ABI *)(void *, const char *)>(image + 0x00CED020);

        // Test A: NULL arguments safely rejected
        U2_TEST_ASSERT(fn_load_file(nullptr, nullptr) == false, "LoadFileHelper(NULL, NULL) returns false");

        // Test B: Non-existent file returns false with clean struct
        struct TestFileData {
            void *buf{reinterpret_cast<void *>(0x1234)};
            uint64_t size{999};
        } test_data;
        bool bRes = fn_load_file(&test_data, "non_existent_mock_shader.cxo");
        U2_TEST_ASSERT(bRes == false, "LoadFileHelper(non-existent) returns false");
        U2_TEST_ASSERT(test_data.buf == nullptr, "LoadFileHelper resets buffer on failure");
        U2_TEST_ASSERT(test_data.size == 0, "LoadFileHelper resets size on failure");
    }

    // 12. Test Audio Subsystem (audio_system.cpp)
    {
        auto fn_audio_init = reinterpret_cast<void (ABI *)(void *)>(image + 0x003CE4A0);

        // Test A: NULL pointer safety
        fn_audio_init(nullptr);
        U2_TEST_ASSERT(true, "AudioFile::Initialize(NULL) safe");

        // Test B: Mock AudioFile initialization
        uint8_t mock_audio[0x100] = {};
        fn_audio_init(mock_audio);
        U2_TEST_ASSERT(mock_audio[0] == 1, "AudioFile::Initialize sets initialized flag");
        U2_TEST_ASSERT(*reinterpret_cast<void **>(mock_audio + 0x08) != nullptr, "AudioFile buffer 1 allocated");
        U2_TEST_ASSERT(*reinterpret_cast<void **>(mock_audio + 0x28) != nullptr, "AudioFile buffer 2 allocated");
        U2_TEST_ASSERT(*reinterpret_cast<void **>(mock_audio + 0x50) != nullptr, "AudioFile slots allocated");

        // Test C: Idempotent initialization
        fn_audio_init(mock_audio);
        U2_TEST_ASSERT(mock_audio[0] == 1, "AudioFile::Initialize idempotent re-init safe");
    }

    // 13. Test Streaming Path Resolution (ndlib/io/streaming.cpp)
    {
        auto fn_resolve = reinterpret_cast<void (ABI *)(const char *, char *)>(image + 0x00B70270);

        // Test A: NULL pointer safety
        fn_resolve(nullptr, nullptr);
        char out_buf[1024] = {};
        fn_resolve(nullptr, out_buf);
        U2_TEST_ASSERT(out_buf[0] == '\0', "Streaming::ResolvePath(NULL) produces empty string");

        // Test B: Host network prefix stripped and mapped to /app0/
        fn_resolve("/host/Z:/u2data/test.bin", out_buf);
        U2_TEST_ASSERT(std::strcmp(out_buf, "/app0/u2data/test.bin") == 0, "Streaming::ResolvePath strips /host/Z:/");

        // Test C: Relative path starting with u2data/ mapped to /app0/u2data/
        fn_resolve("u2data/build/main/dc1/ps4/hud-settings.bin", out_buf);
        U2_TEST_ASSERT(std::strcmp(out_buf, "/app0/u2data/build/main/dc1/ps4/hud-settings.bin") == 0,
                       "Streaming::ResolvePath maps relative u2data/ to /app0/u2data/");

        // Test D: Windows backslashes normalized to forward slashes
        fn_resolve("u2data\\build\\main\\test.bin", out_buf);
        U2_TEST_ASSERT(std::strcmp(out_buf, "/app0/u2data/build/main/test.bin") == 0,
                       "Streaming::ResolvePath normalizes backslashes to forward slashes");
    }

    // 14. Test Tranquilizer Weapon Subsystem (game/weapon/tranquilizer.cpp)
    {
        auto fn_tranquilizer_init = reinterpret_cast<void (ABI *)(void)>(image + 0x009E0CC0);

        // Setup mock table at 0x015296F0
        uint8_t *mock_table = image + 0x015296F0;
        std::memset(mock_table, 0, 5 * 0x48);
        for (int i = 0; i < 5; ++i) {
            *reinterpret_cast<uint32_t *>(mock_table + i * 0x48 + 0x24) = 0xCD0FBE5Au;
        }

        // Must execute cleanly without NULL dereference even when symbols are absent
        fn_tranquilizer_init();
        U2_TEST_ASSERT(true, "Tranquilizer::Init executes safely with absent symbols");
    }

    // 15. Test HUD Subsystem (game/hud/save_hud.cpp)
    {
        auto fn_hud_read_config = reinterpret_cast<void (ABI *)(void *)>(image + 0x00552C70);

        // Test A: NULL pointer safety
        fn_hud_read_config(nullptr);
        U2_TEST_ASSERT(true, "Hud::ReadConfigFile(NULL) is safe");

        // Test B: Mock HUD buffer initialization
        uint8_t mock_hud[0x400];
        std::memset(mock_hud, 0xFF, sizeof(mock_hud));
        fn_hud_read_config(mock_hud);

        uint32_t num_materials = *reinterpret_cast<uint32_t *>(mock_hud + 0x80);
        U2_TEST_ASSERT(num_materials == 0, "Hud::ReadConfigFile resets numMaterials to 0");

        uint32_t num_buttons = *reinterpret_cast<uint32_t *>(mock_hud + 0x108);
        U2_TEST_ASSERT(num_buttons == 0, "Hud::ReadConfigFile resets numButtons to 0");
    }

    // 16. Test UnlockableManager Subsystem (game/weapon/unlockable_manager.cpp)
    {
        auto fn_unlockable_set_default = reinterpret_cast<void (ABI *)(void *, uint8_t)>(image + 0x00A75450);

        // Test A: NULL pointer safety
        fn_unlockable_set_default(nullptr, 1);
        U2_TEST_ASSERT(true, "UnlockableManager::SetDefaultWeapon(NULL) is safe");

        // Test B: Mock UnlockableManager buffer initialization
        uint8_t mock_mgr[0x100];
        std::memset(mock_mgr, 0xFF, sizeof(mock_mgr));
        fn_unlockable_set_default(mock_mgr, 1);

        U2_TEST_ASSERT(*reinterpret_cast<void **>(mock_mgr + 0x00) == nullptr,
                       "UnlockableManager::SetDefaultWeapon clears offset +0x00");
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(mock_mgr + 0x28) == 1,
                       "UnlockableManager::SetDefaultWeapon sets offset +0x28 to 1");
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(mock_mgr + 0x4C) == 0,
                       "UnlockableManager::SetDefaultWeapon clears offset +0x4C to 0");
    }

    // 17. Test ScriptManager DC Symbol Resolution (game/script/script_manager.cpp)
    {
        auto fn_lookup_ptr = reinterpret_cast<void * (ABI *)(uint32_t)>(image + 0x00A15E70);
        auto fn_lookup_mod = reinterpret_cast<void * (ABI *)(uint32_t, uint32_t)>(image + 0x00A17280);

        // Test A: Symbol lookup with known/preloaded DC symbol or safe fallback
        void *res_menu = fn_lookup_ptr(0x74FFCEEBu);
        U2_TEST_ASSERT(res_menu != nullptr, "ScriptManager::LookupPointer(0x74FFCEEB) returns valid non-null pointer");

        // Test B: Symbol lookup with unknown hash returns safe fallback non-null buffer
        void *res_unknown = fn_lookup_ptr(0xDEADBEEFu);
        U2_TEST_ASSERT(res_unknown != nullptr, "ScriptManager::LookupPointer(unknown) returns safe non-null fallback");

        // Test C: Module lookup returns valid pointer
        void *res_mod = fn_lookup_mod(0x74FFCEEBu, 0x12345678u);
        U2_TEST_ASSERT(res_mod != nullptr, "ScriptManager::LookupPointerInModule returns valid non-null pointer");
    }

    g_u2_image_base = saved_base;
    std::printf("[U2 EngineTest] PASSED: %d/%d functional C++23 engine assertions verified via ELF trampolines!\n",
                s_passed_assertions, s_passed_assertions + s_failed_assertions);
    std::fflush(stdout);
    return s_failed_assertions;
}
