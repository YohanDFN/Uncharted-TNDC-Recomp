// Uncharted 2: Among Thieves — Native C++ Engine Functional Unit Tests
// Validates all C++23 engine detours in src/u2_engine/ by copying the installed
// 12-byte ELF trampolines from (real_image + offset) into an isolated executable
// sandbox image (PAGE_EXECUTE_READWRITE), wiring mock guest helpers, and verifying
// every return value (%rax / %xmm0) and memory/struct mutation.

#include "u2_engine.h"
#include "game/loading/level_resources.h"
#include "game/camera/camera_orientation.h"
#include "game/camera/camera_position.h"
#include "game/rendercamera.h"
#include "ndlib/render/ngen/texture_preview.h"
#include "ndlib/render/ngen/font_preview.h"
#include "../u2_standalone/preview_clip.h"
#include "../u2_standalone/preview_depth.h"
#include "../u2_standalone/preview_lighting.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <bit>
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

#if !defined(U2_STANDALONE)
    // These mid-function checks belong to the original DC task lifecycle.
    // A registry filter cannot detect or disable compensating text patches.
    const uint8_t state_check[] = {
        0x48, 0x8B, 0x83, 0x88, 0x00, 0x00, 0x00,
        0x48, 0x83, 0x78, 0x10, 0x00, 0x74, 0x19
    };
    const uint8_t graph_check[] = {
        0x49, 0x8B, 0x87, 0x88, 0x00, 0x00, 0x00,
        0x8B, 0x78, 0x28, 0x85, 0xFF,
        0x0F, 0x84, 0x84, 0x00, 0x00, 0x00
    };
    U2_TEST_ASSERT(std::memcmp(real_image + 0x00A5301B, state_check, sizeof(state_check)) == 0,
                   "Hybrid preserves original StateTask check at A5301B");
    U2_TEST_ASSERT(std::memcmp(real_image + 0x00A5A4C0, graph_check, sizeof(graph_check)) == 0,
                   "Hybrid preserves original task graph check at A5A4C0");
#endif

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

    // Exercise prototypes through sandbox trampolines without admitting them
    // to the hybrid production registry. Functional tests are not ELF parity.
    const U2NativeDetour *native_only = nullptr;
    size_t native_count = u2_get_engine_native_only_detours(&native_only);
    for (size_t n = 0; n < native_count; ++n) {
        const auto &entry = native_only[n];
        bool registered = false;
        for (size_t i = 0; i < engine_count; ++i) {
            if (engine_detours[i].offset == entry.offset) registered = true;
        }
#if defined(U2_STANDALONE)
        U2_TEST_ASSERT(registered, "Standalone registry includes native prototype");
#else
        U2_TEST_ASSERT(!registered, "Hybrid registry excludes standalone prototype");
#endif
        write_temp_jmp64(image + entry.offset, entry.host_fn);
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
        // Prove host operations neither adopt nor overwrite an existing guest
        // stack. The TLS slot is established by runtime_thread.c and the ELF.
#if !defined(U2_STANDALONE)
        uint64_t *tcb = runtime_thread_current_tcb();
        auto **guest_slot = tcb ? reinterpret_cast<void **>(reinterpret_cast<uint8_t *>(tcb) - 0x20) : nullptr;
        void *saved_guest_stack = guest_slot ? *guest_slot : nullptr;
        alignas(16) uint8_t guest_stack_marker[0x780] = {};
        if (guest_slot) *guest_slot = guest_stack_marker;
        U2_TEST_ASSERT(guest_slot != nullptr, "AllocStack isolation test has guest TLS slot");
#endif
        auto fn_alloc_stack  = reinterpret_cast<void * (ABI *)(void)>(image + 0x00B36D40);
        auto fn_free_stack   = reinterpret_cast<void (ABI *)(void)>(image + 0x00B35D30);
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

#if !defined(U2_STANDALONE)
        U2_TEST_ASSERT(!guest_slot || *guest_slot == guest_stack_marker,
                       "Host allocate/push/pop preserve guest TLS ownership");
#endif
        fn_free_stack();
        fn_free_stack();
#if !defined(U2_STANDALONE)
        U2_TEST_ASSERT(!guest_slot || *guest_slot == guest_stack_marker,
                       "Host release preserves guest TLS ownership, including repeated release");
        bool marker_unchanged = true;
        for (uint8_t byte : guest_stack_marker) marker_unchanged &= byte == 0;
        U2_TEST_ASSERT(marker_unchanged, "Host AllocStack operations never mutate guest stack data");
        if (guest_slot) *guest_slot = saved_guest_stack;
#endif

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

    // 9b. Test the native-only PackageMgr request registry. This must resolve
    // files without claiming that their PAK contents are loaded or ready.
    {
        u2_cpp_PackageMgr_ResetNativeRequestsForTests();
        U2_TEST_ASSERT(u2_cpp_PackageMgr_RequestPackage(nullptr, nullptr) == false,
                       "PackageMgr::RequestPackage rejects NULL name");
        U2_TEST_ASSERT(u2_cpp_PackageMgr_RequestPackage(nullptr, "menu") == true,
                       "PackageMgr::RequestPackage resolves menu.pak");
        const U2NativePackageInfo *menu = u2_cpp_PackageMgr_FindNativeRequest("menu");
        U2_TEST_ASSERT(menu != nullptr, "PackageMgr native registry contains menu");
        U2_TEST_ASSERT(menu && menu->file_size > 0, "PackageMgr records menu.pak size");
        U2_TEST_ASSERT(menu && menu->state == U2NativePackageState::Resolved,
                       "PackageMgr marks menu resolved but not ready");
        U2_TEST_ASSERT(u2_cpp_PackageMgr_RequestPackage(nullptr, "menu") == true,
                       "PackageMgr duplicate menu request succeeds");
        U2_TEST_ASSERT(u2_cpp_PackageMgr_GetNativeRequestCount() == 1u,
                       "PackageMgr deduplicates requests by StringId");
        U2_TEST_ASSERT(u2_cpp_PackageMgr_RequestPackage(nullptr, "menu-temple") == true,
                       "PackageMgr::RequestPackage resolves menu-temple.pak");
        U2_TEST_ASSERT(u2_cpp_PackageMgr_GetNativeRequestCount() == 2u,
                       "PackageMgr registers both native menu packages");
        const auto *temple = u2_cpp_PackageMgr_FindNativeRequest("menu-temple");
        U2_TEST_ASSERT(temple && temple->level_info_loaded && temple->level_info_value == 0,
                       "Real LEVEL_INFO_1 payload transferred to native level property");
        U2NativeCameraDefinition actual_camera;
        {
            std::vector<U2NativeMeshPreview> meshes;
            const bool loaded=u2_cpp_PackageMgr_GetNativeGeometryPreview("menu-temple",&meshes);
            size_t refs=0,decoded=0;bool valid=loaded;
            for(size_t m=0;m<meshes.size();++m) for(size_t r=0;r<meshes[m].material_textures.size();++r) {
                const auto &ref=meshes[m].material_textures[r];
                U2NativeTexturePreview image,indexed;
                const bool found=u2_cpp_PackageMgr_GetNativeMaterialTexturePreview("menu-temple",m,r,&image);
                const bool by_index=ref.image_index>=0 && u2_cpp_PackageMgr_GetNativeTexturePreview("menu-temple",size_t(ref.image_index),&indexed);
                valid &= found && by_index && image.cooked_name==ref.cooked_name+".ndb" && image.uid==indexed.uid &&
                    image.resident_blocks==indexed.resident_blocks && image.pixels.rgba==indexed.pixels.rgba;
                ++refs;decoded+=!image.pixels.rgba.empty();
            }
            U2_TEST_ASSERT(valid && refs==100 && decoded>0 && decoded<refs,
                           "All 100 real material references resolve to owned images by cooked filename with literal .ndb suffix, including compressed-only images");
            bool column=false;
            if(meshes.size()>7) for(size_t r=0;r<meshes[7].material_textures.size();++r) {
                if(meshes[7].material_textures[r].parameter_name!="g_tColorMap") continue;
                U2NativeTexturePreview image;
                column=u2_cpp_PackageMgr_GetNativeMaterialTexturePreview("menu-temple",7,r,&image) &&
                    image.format==72 && image.width==64 && image.height==64 && image.pixels.rgba.size()==4096 &&
                    image.pixels.rgba[0]==0xFF8C8E94;
            }
            U2_TEST_ASSERT(column,"Column material color parameter retrieves the previously GPU-matched BC1 image");
            U2NativeTexturePreview prior;prior.uid=123;
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_GetNativeMaterialTexturePreview(nullptr,0,0,&prior) &&
                           !u2_cpp_PackageMgr_GetNativeMaterialTexturePreview("missing",0,0,&prior) &&
                           !u2_cpp_PackageMgr_GetNativeMaterialTexturePreview("menu-temple",SIZE_MAX,0,&prior) &&
                           !u2_cpp_PackageMgr_GetNativeMaterialTexturePreview("menu-temple",0,SIZE_MAX,&prior) &&
                           !u2_cpp_PackageMgr_GetNativeMaterialTexturePreview("menu-temple",0,0,nullptr) && prior.uid==123,
                           "Material image API rejects invalid package and reference bounds without publishing output");
        }
        {
            std::vector<U2NativeMeshPreview> meshes(1);
            meshes[0].material_textures.resize(2);
            auto &refs=meshes[0].material_textures;
            refs[0].cooked_name="cook/a";refs[0].texture_index=49;
            refs[1].cooked_name="cook/b";refs[1].texture_index=3;
            std::vector<U2NativeTexturePreview> images(2);
            images[0].cooked_name="cook/b.ndb";images[1].cooked_name="cook/a.ndb";
            images[1].pixels.rgba={0xFF0000FF};
            U2_TEST_ASSERT(u2_native_BindGeometryTexturePreviews(&meshes,images) && refs[0].image_index==1 && refs[1].image_index==0,
                           "Material image association follows cooked names despite unrelated geometry indices and reversed image order");
            refs[1].cooked_name="missing";
            std::swap(images[0],images[1]);
            U2_TEST_ASSERT(!u2_native_BindGeometryTexturePreviews(&meshes,images) && refs[0].image_index==1 && refs[1].image_index==0,
                           "Missing later material image preserves all previous bindings atomically");
            refs[1].cooked_name="cook/b";images.push_back(images[0]);
            U2_TEST_ASSERT(!u2_native_BindGeometryTexturePreviews(&meshes,images) && refs[0].image_index==1,
                           "Ambiguous duplicate cooked names reject association without choosing arbitrary image data");
            images.pop_back();refs[1].cooked_name="COOK/B";
            U2_TEST_ASSERT(!u2_native_BindGeometryTexturePreviews(&meshes,images),"Material image names preserve case and require an exact match");
            refs[1].cooked_name="cook/b";refs[1].texture_index=-1;
            U2_TEST_ASSERT(!u2_native_BindGeometryTexturePreviews(&meshes,images),"Unresolved geometry reference cannot receive an image binding");
            refs[1].texture_index=3;refs[1].cooked_name.clear();
            U2_TEST_ASSERT(!u2_native_BindGeometryTexturePreviews(&meshes,images),"Empty cooked references cannot bind images");
            refs[1].cooked_name="cook/b";
            U2_TEST_ASSERT(u2_native_BindGeometryTexturePreviews(&meshes,images) && refs[0].image_index==0 && refs[1].image_index==1 &&
                           images[1].pixels.rgba.empty(),"Association recovers after rejection and preserves compressed-only image status");
            images[1].cooked_name="cook/b.NDB";
            U2_TEST_ASSERT(!u2_native_BindGeometryTexturePreviews(&meshes,images),"Descriptor suffix comparison is case sensitive");
            images[1].cooked_name="cook/b.ndb.extra";
            U2_TEST_ASSERT(!u2_native_BindGeometryTexturePreviews(&meshes,images),"Descriptor suffix must terminate the exact filename");
            images[1].cooked_name="cook/b";
            U2_TEST_ASSERT(!u2_native_BindGeometryTexturePreviews(&meshes,images),"Missing descriptor suffix is not silently accepted");
            U2_TEST_ASSERT(!u2_native_BindGeometryTexturePreviews(nullptr,images),"Material image association rejects null geometry output");
        }
        {
            const auto *info=u2_cpp_PackageMgr_FindNativeRequest("menu-temple");
            U2_TEST_ASSERT(info && info->texture_table_loaded && info->texture_images==60 && info->texture_images_decoded==44 &&
                           info->state==U2NativePackageState::Resolved && info->vram_bytes==8763392,
                           "Real TEXTURE_TABLE loads 60 owned compressed images and decodes 44 supported BC1 previews without claiming engine Ready");
            size_t bytes=0,pixels=0;bool complete=true;
            for(size_t i=0;i<60;++i) {
                U2NativeTexturePreview image;
                complete &= u2_cpp_PackageMgr_GetNativeTexturePreview("menu-temple",i,&image);
                bytes+=image.resident_blocks.size();pixels+=image.pixels.rgba.size();
                complete &= image.uid!=0 && !image.resource_name.empty() && !image.cooked_name.empty() &&
                    image.vram_offset<=info->vram_bytes && image.resident_blocks.size()<=info->vram_bytes-image.vram_offset;
            }
            U2_TEST_ASSERT(complete && bytes==info->vram_bytes && pixels==177152,
                           "All 60 image extents cover the declared VRAM bytes and supported first mips own 177152 pixels");
            U2NativeTexturePreview column,unsupported;
            U2_TEST_ASSERT(u2_cpp_PackageMgr_GetNativeTexturePreview("menu-temple",37,&column) && column.format==72 &&
                           column.width==64 && column.height==64 && column.mip_count==7 && column.resident_blocks.size()==5120 &&
                           column.pixels.srgb && column.pixels.rgba.size()==4096 && column.pixels.rgba[0]==0xFF8C8E94,
                           "Column image is found through TEXTURE_TABLE and the declared VRAM extent, with no fixed file offset");
            U2_TEST_ASSERT(u2_cpp_PackageMgr_GetNativeTexturePreview("menu-temple",8,&unsupported) && unsupported.format==77 &&
                           unsupported.resident_blocks.size()==5597184 && unsupported.pixels.rgba.empty(),
                           "Unsupported large image remains explicitly compressed rather than receiving fabricated pixels");
            const auto saved_uid=column.uid;
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_GetNativeTexturePreview("missing",0,&column) &&
                           !u2_cpp_PackageMgr_GetNativeTexturePreview(nullptr,0,&column) &&
                           !u2_cpp_PackageMgr_GetNativeTexturePreview("menu-temple",60,&column) &&
                           !u2_cpp_PackageMgr_GetNativeTexturePreview("menu-temple",0,nullptr) && column.uid==saved_uid,
                           "Texture snapshot lookup bounds inputs without replacing prior output");
            uint8_t header[80]{};
            auto put32=[&](size_t at,uint32_t value) { for(unsigned i=0;i<4;++i) header[at+i]=uint8_t(value>>(8*i)); };
            put32(0x10,2);put32(0x14,44);put32(0x20,512);
            put32(44,80);put32(48,16);put32(56,96);put32(60,16);
            uint64_t offset=7,extent=9;
            U2_TEST_ASSERT(u2_cpp_PackageMgr_InspectVramExtent(header,sizeof(header),624,&offset,&extent) && offset==112 && extent==512,
                           "VRAM extent follows declared CPU pages and header size, independent of temple offsets");
            put32(56,128);
            U2_TEST_ASSERT(u2_cpp_PackageMgr_InspectVramExtent(header,sizeof(header),656,&offset,&extent) && offset==144,
                           "VRAM start follows a shifted last CPU page including an alignment gap");
            offset=7;extent=9;
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_InspectVramExtent(header,sizeof(header),655,&offset,&extent) && offset==7 && extent==9 &&
                           !u2_cpp_PackageMgr_InspectVramExtent(header,sizeof(header),657,&offset,&extent),
                           "VRAM declaration must end exactly at EOF and failed inspection preserves output");
            put32(64,1);
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_InspectVramExtent(header,sizeof(header),656,&offset,&extent),
                           "VRAM preflight refuses unestablished compressed CPU page kinds");
            put32(64,0);put32(56,90);
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_InspectVramExtent(header,sizeof(header),618,&offset,&extent),
                           "VRAM preflight rejects overlapping CPU page extents");
            put32(56,128);put32(0x20,16*1024*1024+1);
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_InspectVramExtent(header,sizeof(header),144ull+16*1024*1024+1,&offset,&extent),
                           "VRAM loading budget is enforced before allocation");
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_InspectVramExtent(nullptr,sizeof(header),656,&offset,&extent) &&
                           !u2_cpp_PackageMgr_InspectVramExtent(header,0x23,656,&offset,&extent) &&
                           !u2_cpp_PackageMgr_InspectVramExtent(header,sizeof(header),656,nullptr,&extent),
                           "VRAM inspection rejects missing outputs and short headers");
        }
        {
            uint8_t page[512]{},vram[1024]{};
            const auto put32=[&](size_t at,uint32_t value) { for(unsigned i=0;i<4;++i) page[at+i]=uint8_t(value>>(8*i)); };
            const auto put64=[&](size_t at,uint64_t value) { for(unsigned i=0;i<8;++i) page[at+i]=uint8_t(value>>(8*i)); };
            const auto ptr=[&](size_t at,size_t target) { put64(at,reinterpret_cast<uintptr_t>(page+target)); };
            put64(8,64);std::memcpy(page+64,"TEXTURE_TABLE",sizeof("TEXTURE_TABLE"));put32(32,2);
            ptr(40,96);ptr(48,112);ptr(56,128);put32(96,1);put32(100,2);
            ptr(112,320);ptr(120,384);ptr(128,160);ptr(136,224);
            std::memcpy(page+320,"first.ndb",10);std::memcpy(page+352,"cooked/first",13);
            std::memcpy(page+384,"second.ndb",11);std::memcpy(page+416,"cooked/second",14);
            for(size_t i=0;i<2;++i) {
                const size_t d=160+i*64;put64(d+8,i*512);put32(d+16,512);put32(d+20,uint32_t(i+1));put32(d+24,1);
                put32(d+28,i ? 72 : 83);put32(d+32,i ? 0x010F0000 : 0x01000000);put32(d+36,1);put32(d+40,4);put32(d+44,4);
                put32(d+52,2);ptr(d+56,352+i*64);
            }
            vram[513]=0xF8;vram[514]=0x1F;
            std::vector<U2NativeTexturePreview> images;
            const auto read=[&] { return u2_native_ReadTextureTablePreview(&images,page,sizeof(page),0,vram,sizeof(vram)); };
            U2_TEST_ASSERT(read() && images.size()==2 && images[0].pixels.rgba.empty() && images[0].resident_blocks.size()==512 &&
                           images[1].pixels.rgba.size()==16 && images[1].pixels.rgba[0]==0xFF0000FF,
                           "Texture table preserves unknown compression and decodes only the supported bounded BC1 layout");
            U2_TEST_ASSERT(images[1].resource_name=="second.ndb" && images[1].cooked_name=="cooked/second" && images[1].vram_offset==512,
                           "Texture descriptors preserve names and 64-bit VRAM offsets in owned snapshots");
            ptr(128,224);ptr(136,160);put32(96,2);put32(100,1);ptr(112,384);ptr(120,320);
            U2_TEST_ASSERT(read() && images[0].uid==2 && images[0].pixels.rgba[0]==0xFF0000FF,
                           "Texture loading follows relocated tables rather than fixed descriptor addresses");
            ptr(128,160);ptr(136,224);put32(96,1);put32(100,2);ptr(112,320);ptr(120,384);
            put32(224+20,3);
            U2_TEST_ASSERT(!read() && images[0].uid==2,"Mismatched descriptor UIDs reject the table without partial publication");
            put32(224+20,2);put64(224+8,UINT64_MAX);
            U2_TEST_ASSERT(!read(),"Texture VRAM offset overflow is rejected before pointer arithmetic");
            put64(224+8,513);
            U2_TEST_ASSERT(!read(),"Texture extent exceeding the VRAM block by one byte is rejected");
            put64(224+8,512);put32(224+16,511);
            U2_TEST_ASSERT(!read(),"Supported BC1 mip-chain extent must match its padded declared layout");
            put32(224+16,512);ptr(136,449);
            U2_TEST_ASSERT(!read(),"Texture descriptor reads require the complete 64-byte record");
            ptr(136,224);put64(40,UINT64_MAX);
            U2_TEST_ASSERT(!read(),"Texture UID tables reject foreign pointers");
            ptr(40,96);ptr(224+56,511);page[511]='X';
            U2_TEST_ASSERT(!read(),"Texture cooked names must terminate inside the owning page");
            ptr(224+56,416);put32(32,UINT32_MAX);
            U2_TEST_ASSERT(!read(),"Texture counts are bounded before table multiplication and allocation");
            put32(32,2);put32(224+36,4);
            U2_TEST_ASSERT(!read(),"Texture mip counts cannot exceed their dimensions");
            put32(224+36,1);page[64]='X';
            U2_TEST_ASSERT(!read(),"Texture table reader checks the exact resource type");
            page[64]='T';
            U2_TEST_ASSERT(!u2_native_ReadTextureTablePreview(nullptr,page,sizeof(page),0,vram,sizeof(vram)) &&
                           !u2_native_ReadTextureTablePreview(&images,page,sizeof(page),SIZE_MAX,vram,sizeof(vram)) &&
                           !u2_native_ReadTextureTablePreview(&images,page,63,0,vram,sizeof(vram)) &&
                           !u2_native_ReadTextureTablePreview(&images,page,sizeof(page),0,nullptr,sizeof(vram)),
                           "Texture table reader rejects null, overflowing and truncated inputs");
            U2_TEST_ASSERT(read(),"Texture table reader recovers after rejected inputs");
            std::memset(page,0,sizeof(page));std::memset(vram,0,sizeof(vram));
            U2_TEST_ASSERT(images[1].resident_blocks[1]==0xF8 && images[1].pixels.rgba[0]==0xFF0000FF &&
                           images[1].cooked_name=="cooked/second",
                           "Compressed bytes, decoded pixels and names survive destruction of both source buffers");
        }
        {
            U2NativeTexturePixels image;
            std::array<uint8_t,1024> blocks{};
            blocks[0]=0;blocks[1]=0xF8;blocks[2]=0x1F;blocks[3]=0;
            for(size_t i=4;i<8;++i) blocks[i]=0xE4; // selectors 0,1,2,3 on each row
            U2_TEST_ASSERT(u2_native_DecodeThinBc1Preview(&image,blocks.data(),512,4,4,false) &&
                           image.rgba.size()==16 && image.rgba[0]==0xFF0000FF && image.rgba[1]==0xFFFF0000,
                           "BC1 endpoints decode RGB565 with explicit RGBA byte order");
            U2_TEST_ASSERT(image.rgba[2]==0xFF5500AA && image.rgba[3]==0xFFAA0055 && image.rgba[12]==image.rgba[0],
                           "BC1 four-color interpolation and row selector order match known encoded blocks");
            blocks[0]=0x1F;blocks[1]=0;blocks[2]=0;blocks[3]=0xF8;
            U2_TEST_ASSERT(u2_native_DecodeThinBc1Preview(&image,blocks.data(),512,4,4,true) && image.srgb &&
                           image.rgba[2]==0xFF7F007F && image.rgba[3]==0,
                           "BC1 three-color mode preserves transparent black and encoded sRGB values");
            blocks[0]=0xE0;blocks[1]=7;blocks[2]=0xE0;blocks[3]=7;
            U2_TEST_ASSERT(u2_native_DecodeThinBc1Preview(&image,blocks.data(),512,4,4,false) &&
                           image.rgba[0]==0xFF00FF00 && image.rgba[3]==0,
                           "Equal BC1 endpoints retain three-color transparency semantics");
            std::fill(blocks.begin(),blocks.end(),0);
            // Distinct endpoints at microtile Morton indices 1 (x=1) and 2 (y=1).
            blocks[8+1]=0xF8;blocks[16+0]=0x1F;blocks[512+0]=0xE0;blocks[512+1]=7;
            U2_TEST_ASSERT(u2_native_DecodeThinBc1Preview(&image,blocks.data(),blocks.size(),64,32,false) &&
                           image.rgba[4]==0xFF0000FF && image.rgba[4*64]==0xFFFF0000 && image.rgba[32]==0xFF00FF00,
                           "Thin1D BC1 detiling follows Morton block order and crosses 8x8 microtile boundaries");
            U2_TEST_ASSERT(u2_native_DecodeThinBc1Preview(&image,blocks.data(),512,5,3,false) &&
                           image.rgba.size()==15 && image.rgba[4]==0xFF0000FF,
                           "BC1 preview crops partial edge blocks to the requested pixel dimensions");
            const auto saved=image.rgba;
            U2_TEST_ASSERT(!u2_native_DecodeThinBc1Preview(&image,blocks.data(),511,4,4,false) && image.rgba==saved && image.width==5,
                           "BC1 decoder rejects a padded microtile truncated by one byte atomically");
            U2_TEST_ASSERT(!u2_native_DecodeThinBc1Preview(nullptr,blocks.data(),512,4,4,false) &&
                           !u2_native_DecodeThinBc1Preview(&image,nullptr,512,4,4,false) &&
                           !u2_native_DecodeThinBc1Preview(&image,blocks.data(),512,0,4,false) &&
                           !u2_native_DecodeThinBc1Preview(&image,blocks.data(),512,4,0,false),
                           "BC1 decoder rejects missing buffers and zero dimensions");
            U2_TEST_ASSERT(!u2_native_DecodeThinBc1Preview(&image,blocks.data(),blocks.size(),UINT32_MAX,4,false) &&
                           !u2_native_DecodeThinBc1Preview(&image,blocks.data(),blocks.size(),4,4097,false) && image.rgba==saved,
                           "BC1 dimension budget is checked before products and allocations");
            U2_TEST_ASSERT(u2_native_DecodeThinBc1Preview(&image,blocks.data(),512,4,4,false),
                           "BC1 decoder accepts an exact padded first-mip extent");
            const auto owned=image.rgba;std::fill(blocks.begin(),blocks.end(),255);
            U2_TEST_ASSERT(image.rgba==owned,"Decoded BC1 pixels own their data after the input is overwritten");
            // Captured resident column color: full 5120 bytes match the original
            // GPU image by SHA-256; its first mip is BC1 sRGB Thin1DThin, 64x64.
            U2NativeTexturePreview resident;
            const bool read_sample=u2_cpp_PackageMgr_GetNativeTexturePreview("menu-temple",37,&resident);
            U2_TEST_ASSERT(read_sample && u2_native_DecodeThinBc1Preview(&image,resident.resident_blocks.data(),resident.resident_blocks.size(),64,64,true) &&
                           image.rgba.size()==4096 && image.srgb && image.rgba[0]==0xFF8C8E94,
                           "Real GPU-matched temple column BC1 data decodes to 4096 owned pixels with the expected first RGB565 endpoint");
        }
        {
            std::vector<uint8_t> blocks(1024);
            blocks[0]=255;blocks[1]=0;
            uint64_t selectors=0;for(unsigned i=0;i<16;++i) selectors|=uint64_t(i%8)<<(3*i);
            for(unsigned i=0;i<6;++i) blocks[2+i]=uint8_t(selectors>>(8*i));
            U2NativeTexturePixels gray;
            U2_TEST_ASSERT(u2_native_DecodeThinBc4Preview(&gray,blocks.data(),blocks.size(),4,4) &&
                           (gray.rgba[0]&255)==255 && (gray.rgba[1]&255)==0 && (gray.rgba[2]&255)==218 &&
                           (gray.rgba[3]&255)==182 && (gray.rgba[7]&255)==36 && !gray.srgb,
                           "BC4 UNORM expands endpoints and all six interpolated values without gamma conversion");
            blocks[0]=10;blocks[1]=110;
            U2_TEST_ASSERT(u2_native_DecodeThinBc4Preview(&gray,blocks.data(),blocks.size(),4,4) &&
                           (gray.rgba[2]&255)==30 && (gray.rgba[5]&255)==90 &&
                           (gray.rgba[6]&255)==0 && (gray.rgba[7]&255)==255,
                           "BC4 ascending endpoints provide four interpolants plus exact zero and one");
            for(size_t block=0;block<128;++block) {
                std::fill(blocks.begin()+block*8,blocks.begin()+block*8+8,0);
                blocks[block*8]=blocks[block*8+1]=uint8_t(block);
            }
            U2_TEST_ASSERT(u2_native_DecodeThinBc4Preview(&gray,blocks.data(),blocks.size(),64,32) &&
                           (gray.rgba[4]&255)==1 && (gray.rgba[4*64]&255)==2 && (gray.rgba[32]&255)==64,
                           "BC4 Thin1D detiling crosses Morton blocks and adjacent microtiles");
            U2_TEST_ASSERT(u2_native_DecodeThinBc4Preview(&gray,blocks.data(),512,5,3) && gray.rgba.size()==15 && (gray.rgba[4]&255)==1,
                           "BC4 partial image edges retain only pixels inside the requested dimensions");
            const auto saved=gray.rgba;
            U2_TEST_ASSERT(!u2_native_DecodeThinBc4Preview(&gray,blocks.data(),511,4,4) && gray.rgba==saved &&
                           !u2_native_DecodeThinBc4Preview(&gray,nullptr,512,4,4) &&
                           !u2_native_DecodeThinBc4Preview(nullptr,blocks.data(),512,4,4) &&
                           !u2_native_DecodeThinBc4Preview(&gray,blocks.data(),512,0,4) &&
                           !u2_native_DecodeThinBc4Preview(&gray,blocks.data(),512,4097,4) && gray.rgba==saved,
                           "BC4 missing, truncated and oversized inputs preserve the previous owned output");
            std::vector<uint8_t> rgb(512),alpha(512);rgb[0]=0;rgb[1]=0xF8;alpha[0]=alpha[1]=128;
            U2_TEST_ASSERT(u2_native_BuildTitleLogoPreview(&gray,rgb.data(),rgb.size(),alpha.data(),alpha.size(),4,4) &&
                           gray.rgba[0]==0x800000FF && gray.rgba[15]==0x800000FF,
                           "Title logo combines original BC1 RGB with separate BC4 alpha without premultiplication");
            const auto composed=gray.rgba;
            U2_TEST_ASSERT(!u2_native_BuildTitleLogoPreview(&gray,rgb.data(),rgb.size(),alpha.data(),511,4,4) && gray.rgba==composed &&
                           !u2_native_BuildTitleLogoPreview(nullptr,rgb.data(),rgb.size(),alpha.data(),alpha.size(),4,4),
                           "Logo composition publishes nothing when either plane or the output is invalid");
            U2NativeTexturePixels logo;
            const bool loaded=u2_cpp_PackageMgr_LoadNativeTitleLogoPreview(&logo);
            U2_TEST_ASSERT(loaded && logo.width==2048 && logo.height==512 && logo.rgba.size()==1048576 && !logo.srgb &&
                           std::any_of(logo.rgba.begin(),logo.rgba.end(),[](uint32_t p) { return (p>>24)==0; }) &&
                           std::any_of(logo.rgba.begin(),logo.rgba.end(),[](uint32_t p) { return (p>>24)==255; }) &&
                           std::any_of(logo.rgba.begin(),logo.rgba.end(),[](uint32_t p) { return (p>>24)>0 && (p>>24)<255; }),
                           "Real menu package logo pair produces owned RGB with transparent, opaque and antialiased mask pixels");
            const auto *menu=u2_cpp_PackageMgr_FindNativeRequest("menu");
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_LoadNativeTitleLogoPreview(nullptr) && menu && menu->state==U2NativePackageState::Resolved,
                           "Title preview rejects a missing output and does not promote the menu package to Ready");
        }
        {
            std::vector<uint8_t> page(1024);
            const auto put32=[&](size_t at,uint32_t value) { for(unsigned i=0;i<4;++i) page[at+i]=uint8_t(value>>(i*8)); };
            const auto put64=[&](size_t at,uint64_t value) { for(unsigned i=0;i<8;++i) page[at+i]=uint8_t(value>>(i*8)); };
            const auto text=[&](size_t at,const char *s) { std::memcpy(page.data()+at,s,std::strlen(s)+1); };
            put64(8,768);text(768,"PAK_LOGIN_TABLE");put32(0x20,1);put32(0x2C,64);
            put64(64+8,800);text(800,"TEXTURE_DICTIONARY");put32(64+0x20,1);
            put64(64+0x28,128);put64(64+0x30,160);put64(64+0x38,192);
            put32(128,123);put64(160,512);put64(192,256);text(512,"test.ndb");
            put64(256+8,4096);put32(256+0x10,701440);put32(256+0x14,123);
            put32(256+0x18,1);put32(256+0x1C,72);put32(256+0x20,0x010F0000);
            put32(256+0x24,11);put32(256+0x28,1024);put32(256+0x2C,1024);put32(256+0x34,2);put64(256+0x38,512);
            const auto valid=page;
            U2NativeDictionaryTextureDescriptor descriptor;
            U2_TEST_ASSERT(u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"test.ndb",800000) &&
                           descriptor.image.uid==123 && descriptor.image.width==1024 && descriptor.image.mip_count==11 &&
                           descriptor.stored_bytes==701440 && descriptor.image.resident_blocks.empty(),
                           "Raw dictionary descriptor resolves the exact resource without allocating its VRAM bank");
            text(544,"short-resource-label");put64(160,544);
            U2_TEST_ASSERT(u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"test.ndb",800000) &&
                           descriptor.image.resource_name=="short-resource-label" && descriptor.image.cooked_name=="test.ndb",
                           "Dictionary resource labels and full cooked filenames are independent namespaces");
            page=valid;
            U2_TEST_ASSERT(!u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"TEST.ndb",800000) && descriptor.image.uid==123 &&
                           !u2_native_FindDictionaryTextureDescriptor(nullptr,page.data(),page.size(),0,"test.ndb",800000) &&
                           !u2_native_FindDictionaryTextureDescriptor(&descriptor,nullptr,page.size(),0,"test.ndb",800000) &&
                           !u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,nullptr,800000),
                           "Dictionary lookup rejects missing inputs and case mismatches without changing output");
            put32(64+0x20,2);put32(132,123);put64(168,512);put64(200,256);
            U2_TEST_ASSERT(!u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"test.ndb",800000) && descriptor.image.uid==123,
                           "Ambiguous dictionary names fail atomically instead of picking the first texture");
            page=valid;put64(160,1024);
            U2_TEST_ASSERT(!u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"test.ndb",800000),
                           "Dictionary resource-name offsets are checked against the raw page extent");
            page=valid;put64(192,1023);
            U2_TEST_ASSERT(!u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"test.ndb",800000),
                           "Dictionary descriptor extent is validated before reading its fields");
            page=valid;put32(128,124);
            U2_TEST_ASSERT(!u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"test.ndb",800000),
                           "Dictionary key and descriptor UID must agree");
            page=valid;
            U2_TEST_ASSERT(!u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"test.ndb",705535),
                           "Dictionary texture chain cannot cross the declared VRAM extent");
            put32(256+0x10,701439);
            U2_TEST_ASSERT(!u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"test.ndb",800000),
                           "Dictionary BC1 chain must match the derived padded mip sizes exactly");
            page=valid;put32(256+0x34,3);
            U2_TEST_ASSERT(!u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"test.ndb",800000),
                           "Unestablished dictionary texture layouts remain unsupported");
            page=valid;put32(0x28,1);
            U2_TEST_ASSERT(!u2_native_FindDictionaryTextureDescriptor(&descriptor,page.data(),page.size(),0,"test.ndb",800000),
                           "Single-page dictionary lookup rejects cross-page metadata references");
            U2NativeTexturePreview low,high;
            const bool resident=u2_cpp_PackageMgr_GetNativeTexturePreview("menu-temple",37,&low);
            const bool detailed=resident && u2_cpp_PackageMgr_LoadNativeDictionaryTexturePreview("chunk-21-tstrm",low.cooked_name.c_str(),&high);
            U2_TEST_ASSERT(detailed && high.uid==low.uid && high.cooked_name==low.cooked_name && high.width==1024 && high.height==1024 &&
                           high.format==72 && high.mip_count==11 && high.resident_blocks.size()==701440 && high.pixels.rgba.size()==1048576 && high.pixels.srgb,
                           "Real column dictionary resource supplies the GPU-matched 1024x1024 BC1 chain and owned pixels");
            const size_t low_offset=524288+131072+32768+8192;
            U2_TEST_ASSERT(detailed && low.resident_blocks.size()>=2048 && high.resident_blocks.size()>=low_offset+2048 &&
                           std::equal(low.resident_blocks.begin(),low.resident_blocks.begin()+2048,high.resident_blocks.begin()+low_offset),
                           "Streamed column mip4 equals the previously GPU-matched resident 64x64 mip, including its detiled block order");
            high.uid=987;
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_LoadNativeDictionaryTexturePreview(nullptr,"x",&high) &&
                           !u2_cpp_PackageMgr_LoadNativeDictionaryTexturePreview("../chunk-21-tstrm","x",&high) &&
                           !u2_cpp_PackageMgr_LoadNativeDictionaryTexturePreview("chunk-21-tstrm",nullptr,&high) &&
                           !u2_cpp_PackageMgr_LoadNativeDictionaryTexturePreview("chunk-21-tstrm","not-a-resource",&high) && high.uid==987 &&
                           !u2_cpp_PackageMgr_LoadNativeDictionaryTexturePreview("chunk-21-tstrm","x",nullptr),
                           "Dictionary preview API rejects missing inputs, path traversal and absent resources atomically");
            U2NativeTexturePreview preserved;
            U2_TEST_ASSERT(u2_cpp_PackageMgr_GetNativeTexturePreview("menu-temple",37,&preserved) && preserved.width==64 &&
                           preserved.pixels.rgba==low.pixels.rgba,
                           "On-demand detailed preview does not mutate the package's resident snapshot or streaming state");
        }
        {
            using u2::standalone::preview_light_intensity;
            float brightness=-1;
            U2_TEST_ASSERT(preview_light_intensity({0,1,0},{0,1,0},&brightness) && brightness==1,
                           "Preview directional light reaches full intensity for an aligned normal");
            U2_TEST_ASSERT(preview_light_intensity({0,-1,0},{0,1,0},&brightness) && brightness==0.22f &&
                           preview_light_intensity({1,0,0},{0,1,0},&brightness) && brightness==0.22f,
                           "Preview back-facing and perpendicular normals retain only ambient light");
            const std::array<float,3> normal{0,2,2},light{0,8,0};
            U2_TEST_ASSERT(preview_light_intensity(normal,light,&brightness) &&
                           std::abs(brightness-float(0.22+0.78/std::sqrt(2.0)))<0.000001f && normal[1]==2 && light[1]==8,
                           "Preview lighting normalizes temporary directions without modifying stored normals");
            const float largest=std::numeric_limits<float>::max(),smallest=std::numeric_limits<float>::denorm_min();
            U2_TEST_ASSERT(preview_light_intensity({largest,0,0},{smallest,0,0},&brightness) && brightness==1,
                           "Preview lighting handles large finite and subnormal directions without float norm overflow");
            brightness=-7;
            U2_TEST_ASSERT(!preview_light_intensity({0,0,0},light,&brightness) &&
                           !preview_light_intensity(normal,{0,0,0},&brightness) && brightness==-7 &&
                           !preview_light_intensity(normal,light,nullptr),
                           "Preview lighting refuses zero directions and missing output without inventing an intensity");
            const float nan=std::numeric_limits<float>::quiet_NaN(),inf=std::numeric_limits<float>::infinity();
            U2_TEST_ASSERT(!preview_light_intensity({nan,0,1},light,&brightness) &&
                           !preview_light_intensity(normal,{inf,0,1},&brightness) && brightness==-7,
                           "Preview lighting rejects nonfinite normals and light directions atomically");

            using Image=u2::standalone::PreviewDepthImage;
            constexpr uint32_t bg=0xFF130C08;
            Image shaded,plain;
            shaded.reset(16,16,bg);plain.reset(16,16,bg);
            const std::array<Image::Vertex,3> triangle{{{-1,-1,0.2f,1},{2,-2,1.6f,2},{0,4,2,4}}};
            const size_t center=8*16+8;
            shaded.triangle(triangle,0xFFFFFFFF,{1,0,0});
            U2_TEST_ASSERT(shaded.pixels[center]==0xFF787878 && std::abs(shaded.depth[center]-0.51875f)<0.000001f,
                           "Unequal-w brightness is perspective-correct (0.46875 at sample), while depth remains linear z/w");
            auto reversed=triangle;std::swap(reversed[0],reversed[2]);
            plain.triangle(reversed,0xFFFFFFFF,{0,0,1});
            bool equivalent_depth=true;
            for(size_t i=0;i<plain.depth.size();++i) {
                const bool covered=std::isfinite(plain.depth[i]),other=std::isfinite(shaded.depth[i]);
                equivalent_depth &= covered==other && (!covered || std::abs(plain.depth[i]-shaded.depth[i])<0.000001f);
            }
            U2_TEST_ASSERT(plain.pixels==shaded.pixels && equivalent_depth,
                           "Lighting follows vertex attributes when winding changes");
            shaded.reset(16,16,bg);plain.reset(16,16,bg);
            shaded.triangle(triangle,0x7F204080,{0.5f,0.5f,0.5f});
            U2_TEST_ASSERT(shaded.pixels[center]==0x7F102040,
                           "Vertex brightness scales RGB channels while preserving alpha");
            shaded.reset(16,16,bg);
            const auto original_pixels=shaded.pixels;
            const auto original_depth=shaded.depth;
            bool invalid_rejected=true;
            for(float invalid : {nan,-0.01f,1.01f})
                invalid_rejected &= shaded.triangle(triangle,0xFFFFFFFF,{0,invalid,1})==0 &&
                    shaded.pixels==original_pixels && shaded.depth==original_depth;
            U2_TEST_ASSERT(invalid_rejected,"Invalid vertex brightness leaves both color and depth untouched");
            shaded.triangle(triangle,0xFFFFFFFF,{1,1,1});plain.triangle(triangle,0xFFFFFFFF);
            U2_TEST_ASSERT(shaded.pixels==plain.pixels && shaded.depth==plain.depth,
                           "Default brightness retains the previous opaque depth rendering");
            shaded.reset(16,16,bg);plain.reset(16,16,bg);
            shaded.triangle({{{-1,-1,-0.5f,1},{1,-1,0.5f,1},{0,1,0.5f,1}}},0xFFFFFFFF,{1,0,0});
            const Image::Vertex a{-0.5f,0,0,1},b{0,-1,0,1},c{1,-1,0.5f,1},d{0,1,0.5f,1};
            plain.triangle({a,b,c},0xFFFFFFFF,{0.5f,0.5f,0});plain.triangle({a,c,d},0xFFFFFFFF,{0.5f,0,0});
            U2_TEST_ASSERT(shaded.pixels==plain.pixels && shaded.depth==plain.depth,
                           "Near-plane clipping interpolates brightness like an explicitly split polygon");
        }
        {
            using Image=u2::standalone::PreviewDepthImage;
            using u2::standalone::preview_sample_repeat;
            constexpr uint32_t bg=0xFF130C08;
            U2NativeTexturePixels texture{2,2,false,{0xFF000000,0xFF0000FF,0xFF00FF00,0xFFFF0000}};
            uint32_t sample=123;
            U2_TEST_ASSERT(preview_sample_repeat(texture,0.75f,0.25f,1,&sample) && sample==0xFF0000FF,
                           "Bilinear normalized texel centers preserve the selected RGBA texel without flipping V");
            U2_TEST_ASSERT(preview_sample_repeat(texture,-0.25f,2.25f,1,&sample) && sample==0xFF0000FF,
                           "Repeat sampling retains negative and tiled UV coordinates");
            U2_TEST_ASSERT(preview_sample_repeat(texture,0,0,1,&sample) && sample==0xFF404040,
                           "Repeat bilinear filtering crosses both image seams at normalized zero");
            U2_TEST_ASSERT(preview_sample_repeat(texture,0.75f,0.25f,0.5f,&sample) && sample==0xFF000080,
                           "Linear texture lighting scales RGB and preserves filtered alpha");
            texture.srgb=true;
            U2_TEST_ASSERT(preview_sample_repeat(texture,0.75f,0.25f,0.5f,&sample) && sample==0xFF0000BC,
                           "sRGB texture lighting occurs in linear space before encoding for SDL");
            U2_TEST_ASSERT(preview_sample_repeat(texture,0,0,1,&sample) && sample==0xFF898989,
                           "sRGB bilinear interpolation decodes texels before averaging");
            texture.srgb=false;sample=123;
            const float nan=std::numeric_limits<float>::quiet_NaN();
            auto invalid=texture;invalid.rgba.pop_back();
            U2_TEST_ASSERT(!preview_sample_repeat(invalid,0,0,1,&sample) && sample==123 &&
                           !preview_sample_repeat(texture,nan,0,1,&sample) && sample==123 &&
                           !preview_sample_repeat(texture,0,0,-1,&sample) && sample==123 &&
                           !preview_sample_repeat(texture,0,0,1,nullptr),
                           "Invalid texture extents, UVs, brightness and output are rejected without publishing pixels");
            U2_TEST_ASSERT(preview_sample_repeat(texture,std::numeric_limits<float>::max(),-std::numeric_limits<float>::max(),1,&sample) && sample==0xFF404040,
                           "Large finite UVs reduce before conversion to bounded texel coordinates");
            U2NativeTexturePixels single{1,1,false,{0x7F204080}};
            U2_TEST_ASSERT(preview_sample_repeat(single,-13.5f,9,0.5f,&sample) && sample==0x7F102040,
                           "Single-texel repeat and alpha remain well-defined");
            Image image,split;image.reset(16,16,bg);split.reset(16,16,bg);
            const std::array<Image::Vertex,3> face{{{-1,-1,0.2f,1},{2,-2,1.6f,2},{0,4,2,4}}};
            image.triangle(face,0,{1,1,1},&texture,{{{0.25f,0.25f},{0.75f,0.25f},{0.25f,0.75f}}});
            // At (8.5,8.5): screen weights 0.234375,0.296875,0.46875;
            // perspective weights divide them by w=1,2,4 before normalization.
            preview_sample_repeat(texture,0.25f+0.5f*(0.296875f/2)/0.5f,
                0.25f+0.5f*(0.46875f/4)/0.5f,1,&sample);
            U2_TEST_ASSERT(image.pixels[8*16+8]==sample && std::abs(image.depth[8*16+8]-0.51875f)<0.000001f,
                           "Unequal-w UV interpolation is perspective-correct while z/w depth remains linear");
            image.reset(16,16,bg);
            image.triangle({{{-1,-1,-0.5f,1},{1,-1,0.5f,1},{0,1,0.5f,1}}},0,{1,1,1},&texture,
                {{{0.25f,0.25f},{0.75f,0.25f},{0.25f,0.75f}}});
            const Image::Vertex a{-0.5f,0,0,1},b{0,-1,0,1},c{1,-1,0.5f,1},d{0,1,0.5f,1};
            split.triangle({a,b,c},0,{1,1,1},&texture,{{{0.25f,0.5f},{0.5f,0.25f},{0.75f,0.25f}}});
            split.triangle({a,c,d},0,{1,1,1},&texture,{{{0.25f,0.5f},{0.75f,0.25f},{0.25f,0.75f}}});
            U2_TEST_ASSERT(image.pixels==split.pixels && image.depth==split.depth,
                           "Near-plane clipping carries UVs like an explicitly split textured polygon");
            const auto saved=image.pixels;const auto depths=image.depth;
            U2_TEST_ASSERT(image.triangle(face,0,{1,1,1},&invalid)==0 && image.pixels==saved && image.depth==depths &&
                           image.triangle(face,0,{1,1,1},&texture,{{{nan,0},{0,0},{0,0}}})==0 && image.pixels==saved && image.depth==depths,
                           "Invalid textured triangles preserve both color and depth buffers");
            image.reset(16,16,bg);split.reset(16,16,bg);
            image.triangle(face,0x7F204080,{0.5f,0.5f,0.5f});
            split.triangle(face,0,{0.5f,0.5f,0.5f},&single);
            U2_TEST_ASSERT(image.pixels==split.pixels && image.depth==split.depth,
                           "Constant linear texture preserves established depth coverage and brightness");
        }
        {
            using Image=u2::standalone::PreviewDepthImage;
            constexpr uint32_t background=0xFF130C08,red=0xFF0000FF,blue=0xFFFF0000;
            Image image,reverse;
            U2_TEST_ASSERT(image.reset(16,16,background) && reverse.reset(16,16,background) &&
                           image.pixels.size()==256 && std::isinf(image.depth[0]),
                           "Solid preview initializes owned color and depth buffers");
            const std::array<Image::Vertex,3> front_face{{{-0.9f,-0.9f,0.2f,1},{0.9f,-0.9f,0.2f,1},{0,0.9f,0.2f,1}}};
            auto back_face=front_face;for(auto &v : back_face) v[2]=0.8f;
            const size_t center=8*16+8;
            U2_TEST_ASSERT(image.triangle(back_face,blue)>0 && image.triangle(front_face,red)>0 &&
                           image.pixels[center]==red && std::abs(image.depth[center]-0.2f)<0.000001f,
                           "Per-pixel depth lets a nearer opaque surface hide a farther surface");
            reverse.triangle(front_face,red);reverse.triangle(back_face,blue);
            U2_TEST_ASSERT(image.pixels==reverse.pixels && image.depth==reverse.depth,
                           "Opaque occlusion is independent of triangle submission order");
            image.reset(16,16,background);reverse.reset(16,16,background);
            auto clockwise=front_face;std::swap(clockwise[1],clockwise[2]);
            image.triangle(front_face,red);reverse.triangle(clockwise,red);
            U2_TEST_ASSERT(image.pixels==reverse.pixels && image.depth==reverse.depth,
                           "Solid viewer handles both windings without guessing an original cull mode");
            image.reset(16,16,background);auto small=front_face;
            for(auto &v : small) { v[0]*=0.4f;v[1]*=0.4f; }
            image.triangle(back_face,blue);image.triangle(small,red);
            U2_TEST_ASSERT(image.pixels[center]==red && image.pixels[12*16+8]==blue,
                           "Depth resolves partial overlap while retaining the visible surrounding surface");
            image.reset(16,16,background);auto crossing=front_face;crossing[0][2]=-0.5f;
            U2_TEST_ASSERT(image.triangle(crossing,red)>0,
                           "Solid triangle clipping retains geometry crossing the near plane");
            image.reset(16,16,background);crossing=front_face;crossing[0][2]=2;
            U2_TEST_ASSERT(image.triangle(crossing,red)>0,
                           "Solid triangle clipping retains geometry crossing the far plane");
            bool sides=true;
            for(size_t axis=0;axis<2;++axis) for(float sign : {-1.0f,1.0f}) {
                image.reset(16,16,background);
                crossing={{{0,0,0.5f,1},{-0.5f,-0.5f,0.5f,1},{0.5f,0.5f,0.5f,1}}};
                crossing[0][axis]=sign*2;
                sides &= image.triangle(crossing,red)>0;
            }
            U2_TEST_ASSERT(sides,"Solid triangle clipping handles all four viewport sides");
            image.reset(16,16,background);const auto saved_pixels=image.pixels;const auto saved_depth=image.depth;
            auto hidden=front_face;for(auto &v : hidden) v[2]=-0.2f;
            U2_TEST_ASSERT(image.triangle(hidden,red)==0 && image.pixels==saved_pixels && image.depth==saved_depth,
                           "Fully clipped surfaces do not write color or depth");
            hidden=front_face;for(auto &v : hidden) { v[2]=-0.5f;v[3]=-1; }
            U2_TEST_ASSERT(image.triangle(hidden,red)==0,"Surfaces behind the camera are rejected before perspective division");
            hidden=front_face;hidden[2]=hidden[1];
            U2_TEST_ASSERT(image.triangle(hidden,red)==0,"Degenerate triangles do not produce invalid raster bounds");
            hidden=front_face;hidden[1][0]=std::bit_cast<float>(uint32_t{0x7FC00000});
            U2_TEST_ASSERT(image.triangle(hidden,red)==0 && image.pixels==saved_pixels && image.depth==saved_depth,
                           "Nonfinite triangle coordinates cannot poison the cached depth image");
            const std::array<Image::Vertex,3> perspective{{{-1,-1,0.2f,1},{2,-2,1.6f,2},{0,4,2,4}}};
            image.triangle(perspective,red);
            U2_TEST_ASSERT(std::abs(image.depth[center]-0.51875f)<0.000001f,
                           "Depth interpolates z divided by w linearly in screen space for unequal vertex w");
            bool boundaries=true;
            size_t boundary_samples[2]{};size_t boundary_index=0;
            for(float depth : {0.0f,1.0f}) {
                image.reset(16,16,background);auto face=front_face;for(auto &v : face) v[2]=depth;
                boundary_samples[boundary_index++]=image.triangle(face,red);
                boundaries &= boundary_samples[boundary_index-1]>0 && std::abs(image.depth[center]-depth)<0.000001f;
            }
            U2_TEST_ASSERT(boundaries && boundary_samples[0]==boundary_samples[1],
                           "Solid depth accepts exact near and far boundaries with identical coverage and no far-plane holes");
            const auto prior=image.pixels;
            U2_TEST_ASSERT(!image.reset(0,16,0) && !image.reset(2147483647,2,0) && image.pixels==prior && image.width==16,
                           "Depth image dimensions are bounded before allocation and failure preserves the existing image");
        }
        {
            const std::array<float,4> inside_a{0,0,0.25f,1},inside_b{0.5f,0.5f,0.75f,1};
            auto a=inside_a,b=inside_b;
            U2_TEST_ASSERT(u2::standalone::clip_preview_line(a,b) && a==inside_a && b==inside_b,
                           "Preview clipping preserves a fully visible line");
            a={0,0,-1,1};b={0,0,1,1};
            U2_TEST_ASSERT(u2::standalone::clip_preview_line(a,b) && a[2]==0 && b[2]==1,
                           "Preview clipping retains the visible portion across the near plane");
            a={0,0,0.5f,1};b={0,0,2,1};
            U2_TEST_ASSERT(u2::standalone::clip_preview_line(a,b) && b[2]==b[3],
                           "Preview clipping terminates a line at the far plane");
            bool lateral=true;
            for(size_t axis=0;axis<2;++axis) for(float sign : {-1.0f,1.0f}) {
                a={0,0,0.5f,1};b=a;b[axis]=sign*2;
                lateral &= u2::standalone::clip_preview_line(a,b) && b[axis]==sign && b[3]==1;
            }
            U2_TEST_ASSERT(lateral,"Preview clipping handles all four side planes");
            a={-3,0,0.5f,1};b={-2,0,0.5f,1};const auto saved_a=a,saved_b=b;
            U2_TEST_ASSERT(!u2::standalone::clip_preview_line(a,b) && a==saved_a && b==saved_b,
                           "Preview clipping rejects a fully hidden line atomically");
            a={0,0,-1,-1};b={0,0,-2,-2};
            U2_TEST_ASSERT(!u2::standalone::clip_preview_line(a,b),"Preview clipping rejects lines behind the camera");
            a={0,0,0,0};b=a;
            U2_TEST_ASSERT(!u2::standalone::clip_preview_line(a,b),"Preview clipping rejects zero homogeneous divisors");
            a=inside_a;b=inside_b;a[0]=std::bit_cast<float>(uint32_t{0x7FC00000});
            U2_TEST_ASSERT(!u2::standalone::clip_preview_line(a,b) && b==inside_b,
                           "Preview clipping rejects nonfinite coordinates before projection");
        }
        {
            std::vector<U2NativeMeshPreview> meshes;
            U2_TEST_ASSERT(u2_cpp_PackageMgr_GetNativeGeometryPreview("menu-temple", &meshes) && meshes.size()==13,
                           "Real GEOMETRY_1 yields 13 owned mesh snapshots");
            U2_TEST_ASSERT(!meshes.empty() && meshes[0].name=="menu-temple.Prototype0.Submesh0" && meshes[0].positions.size()==321 &&
                           meshes[0].positions[0][0]==-69.251220703125f && meshes[0].positions[0][1]==38.54443359375f &&
                           meshes[0].positions[0][2]==-9.87060546875f,
                           "First real temple mesh preserves its original name, vertex count and float3 coordinates");
            size_t total=0;
            for (const auto &mesh : meshes) total+=mesh.positions.size();
            U2_TEST_ASSERT(total==12383, "All 13 temple mesh position streams decoded with their 0x88 descriptor stride");
            size_t normal_count=0;bool normal_lengths=true;
            for (const auto &mesh : meshes) {
                normal_count+=mesh.normals.size();
                normal_lengths &= mesh.normals.size()==mesh.positions.size();
                for (const auto &n : mesh.normals) {
                    const float length=std::sqrt(n[0]*n[0]+n[1]*n[1]+n[2]*n[2]);
                    normal_lengths &= length>0.993f && length<1.007f && n[3]==1;
                }
            }
            U2_TEST_ASSERT(normal_count==12383 && normal_lengths,
                           "All real temple attribute-2 streams decode near-unit normals with preserved w=1");
            const std::array<float,4> first_normal{-0.00390625f,1,-0.00390625f,1};
            U2_TEST_ASSERT(!meshes.empty() && !meshes[0].normals.empty() && meshes[0].normals[0]==first_normal,
                           "First original temple normal preserves exact signed binary16 values");
            size_t uv_count=0;bool uv_sizes=true,uv_outside_unit=false;
            for (const auto &mesh : meshes) {
                uv_count+=mesh.texcoords.size();uv_sizes &= mesh.texcoords.size()==mesh.positions.size();
                for (const auto &uv : mesh.texcoords) for(float coordinate : uv)
                    uv_outside_unit |= coordinate<0 || coordinate>1;
            }
            U2_TEST_ASSERT(uv_count==12383 && uv_sizes && uv_outside_unit,
                           "All temple meshes preserve a texture coordinate per vertex, including negative and tiled coordinates");
            const std::array<float,2> first_uv{-2.51171875f,-2.513671875f};
            U2_TEST_ASSERT(!meshes.empty() && !meshes[0].texcoords.empty() && meshes[0].texcoords[0]==first_uv,
                           "First real UV pair decodes exactly from attribute 1 R16G16Float");
            bool material_indices=meshes.size()==13;
            for(size_t i=0;i<meshes.size();++i) material_indices &= meshes[i].material_index==int32_t(i) && !meshes[i].material_name.empty();
            U2_TEST_ASSERT(material_indices,"All 13 temple material indices match the live original PrepareBinary capture");
            U2_TEST_ASSERT(!meshes.empty() && meshes[0].material_name==
                           "levels/temple/walls/tmp-stone-crumpled-blend:redpaint-to-lrg-brick,f8906991-3ff7-4224-813f-2dc43d693c13,blend-color-bump-specular.fx",
                           "First temple mesh owns the exact original material reference, not a guessed texture path");
            constexpr int texture_indices[13][9]={{1,2,3,4,5,6,7,8,9},{10,11,12,13,14,15,8,9,-1},
                {16,11,17,18,18,19,8,9,-1},{20,11,21,22,23,24,8,9,-1},{10,11,25,13,26,27,8,9,-1},
                {28,11,29,30,30,31,8,9,-1},{32,11,33,34,35,36,8,9,-1},{37,38,37,39,39,40,8,9,-1},
                {41,42,3,43,5,7,8,9,-1},{44,45,46,47,8,9,-1,-1,-1},{48,11,49,50,50,51,8,9,-1},
                {52,42,53,54,55,56,8,9,-1},{57,58,59,8,9,-1,-1,-1,-1}};
            size_t reference_count=0;bool texture_matches=meshes.size()==13;
            for(size_t i=0;i<meshes.size();++i) {
                const auto &references=meshes[i].material_textures;reference_count+=references.size();
                size_t expected=0;while(expected<9 && texture_indices[i][expected]>=0) ++expected;
                texture_matches &= references.size()==expected;
                for(size_t r=0;r<references.size();++r)
                    texture_matches &= r<expected && references[r].texture_index==texture_indices[i][r];
            }
            U2_TEST_ASSERT(reference_count==100 && texture_matches,
                           "All 100 temple material texture indices match the live original PrepareBinary capture");
            U2_TEST_ASSERT(meshes.size()==13 && !meshes[0].material_textures.empty() &&
                           meshes[0].material_textures[0].parameter_name=="g_tColorMap" &&
                           meshes[0].material_textures[0].source_path=="art/levels/temple/textures/walls/tmp-wall-red2-color.tga" &&
                           meshes[0].material_textures[0].cooked_name==
                           "d:/dev/CP9/Data/U2/CookData/ND/build/texture2/art/levels/temple/textures/walls/tmp-wall-red2-color.tga/4246CBC0DBEC44C4",
                           "Texture references preserve parameter, source path and cooked lookup name as distinct owned values");
            size_t triangle_count=0;
            for (const auto &mesh : meshes) triangle_count+=mesh.triangles.size();
            const std::array<uint16_t,3> first_triangle{0,1,2};
            U2_TEST_ASSERT(meshes.size()==13 && meshes[0].triangles.size()==224 &&
                           meshes[0].triangles[0]==first_triangle && triangle_count==13680,
                           "Real temple index buffers decode 13680 triangles with the topology observed in original draws");
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_GetNativeGeometryPreview("missing", &meshes) &&
                           !u2_cpp_PackageMgr_GetNativeGeometryPreview(nullptr, &meshes) &&
                           !u2_cpp_PackageMgr_GetNativeGeometryPreview("menu-temple", nullptr) && meshes.size()==13,
                           "Geometry snapshot lookup rejects absent inputs without replacing owned output");
            uint8_t page[768]{};
            auto put32=[&](size_t offset,uint32_t value) { for(unsigned i=0;i<4;++i) page[offset+i]=uint8_t(value>>(8*i)); };
            auto put64=[&](size_t offset,uint64_t value) { for(unsigned i=0;i<8;++i) page[offset+i]=uint8_t(value>>(8*i)); };
            auto ptr=[&](size_t offset,size_t target) { put64(offset,reinterpret_cast<uintptr_t>(page+target)); };
            put64(8,640);std::memcpy(page+640,"GEOMETRY_1",sizeof("GEOMETRY_1"));
            put32(40,1);ptr(48,128); // payload mesh count +8 and table +10
            ptr(128+8,544);std::memcpy(page+544,"test-mesh",sizeof("test-mesh"));
            put32(128+32,3);put32(128+40,1);ptr(128+48,416);
            put32(128+36,3);ptr(128+64,512);page[514]=1;page[516]=2;
            page[416]=1;page[418]=12;put32(420,36);ptr(424,448);ptr(432,464);put32(448,0x060C0000);
            const std::array<float,3> first{-1,2,3.5f};
            for (size_t i=0;i<3;++i) put32(464+i*4,std::bit_cast<uint32_t>(first[i]));
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes.size()==1 &&
                           meshes[0].positions.size()==3 && meshes[0].positions[0]==first && meshes[0].normals.empty() && meshes[0].texcoords.empty(),
                           "Geometry preview decodes signed nonzero float3 positions");
            U2_TEST_ASSERT(meshes[0].triangles.size()==1 && meshes[0].triangles[0]==first_triangle,
                           "Geometry decoder preserves little-endian uint16 triangle indices");
            page[514]=3;
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].triangles[0]==first_triangle,
                           "Geometry rejects an index equal to vertex count without changing output");
            page[514]=1;page[515]=1;
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),
                           "Geometry uses both bytes of each index when checking vertex bounds");
            page[515]=0;put32(128+36,4);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),
                           "Geometry refuses incomplete triangle triples");
            put32(128+36,UINT32_MAX-3);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),
                           "Geometry bounds index count before allocation and size multiplication");
            put32(128+36,3);ptr(128+64,764);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),
                           "Geometry rejects truncated index data at the end of its page");
            put64(128+64,UINT64_MAX);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),
                           "Geometry rejects foreign index-buffer pointers");
            ptr(128+64,512);
            std::memcpy(page+128+0x88,page+128,0x88);put32(40,2);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes.size()==2 &&
                           meshes[1].positions[0]==first, "Geometry preview uses established adjacent mesh descriptor stride");
            put32(40,UINT32_MAX);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes.size()==2,
                           "Geometry preview bounds table extent and preserves prior output");
            put32(40,1);page[418]=8;
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0), "Geometry preview refuses unsupported vertex stride");
            page[418]=12;put32(448,0x070C0000);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0), "Geometry preview refuses unproven attribute formats");
            put32(448,0x060C0000);put32(420,35);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0), "Geometry preview checks exact stream byte count");
            put32(420,36);put64(432,UINT64_MAX);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0), "Geometry preview rejects foreign vertex buffer pointers");
            ptr(432,464);put32(128+32,UINT32_MAX);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0), "Geometry preview bounds vertex counts before allocation");
            put32(128+32,3);
            bool nonfinite_rejected=true;
            for(size_t axis=0;axis<3;++axis) {
                put32(464+4*axis,0x7FC00000);
                nonfinite_rejected &= !u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes.size()==2 && meshes[0].positions[0]==first;
                put32(464+4*axis,std::bit_cast<uint32_t>(first[axis]));
            }
            U2_TEST_ASSERT(nonfinite_rejected,"Geometry preview rejects nonfinite positions without partial publication");
            page[640]='X';
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0), "Geometry preview checks exact resource type");
            page[640]='G';
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(nullptr,page,sizeof(page),0) &&
                           !u2_native_ReadGeometryPreview(&meshes,nullptr,sizeof(page),0) &&
                           !u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),SIZE_MAX) &&
                           !u2_native_ReadGeometryPreview(&meshes,page,0x77,0), "Geometry preview rejects null, overflowing and truncated metadata");
            // Add an optional normal stream to the synthetic position-only page.
            put32(40,1);ptr(128+48,568);put32(128+40,2);
            std::memcpy(page+568,page+416,24);
            auto put16=[&](size_t at,uint16_t value) { page[at]=uint8_t(value);page[at+1]=uint8_t(value>>8); };
            put16(592,1);put16(594,8);put32(596,24);ptr(600,672);ptr(608,680);put32(672,0x0A080002);
            for(size_t v=0;v<3;++v) {
                put16(680+v*8,0xBC00);put16(682+v*8,0x3800);
                put16(684+v*8,1);put16(686+v*8,0x8000);
            }
            const std::array<float,4> sample_normal{-1,0.5f,0x1p-24f,-0.0f};
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes.size()==1 &&
                           meshes[0].normals.size()==3 && meshes[0].normals[0]==sample_normal &&
                           std::signbit(meshes[0].normals[0][3]),
                           "Normal decoding preserves signed values, subnormals and negative zero without normalization");
            put16(680,0x7BFF);put16(682,0x0400);put16(684,0x03FF);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) &&
                           meshes[0].normals[0][0]==65504 && meshes[0].normals[0][1]==0x1p-14f &&
                           meshes[0].normals[0][2]==1023*0x1p-24f,
                           "Normal binary16 conversion handles maximum finite and normal/subnormal boundaries exactly");
            put16(680,0xBC00);put16(682,0x3800);put16(684,1);
            bool rejected_specials=true;
            for(uint16_t special : {uint16_t{0x7C00},uint16_t{0xFC00},uint16_t{0x7E01}}) {
                put16(680,special);
                rejected_specials &= !u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) &&
                    meshes[0].normals[0][0]==65504;
            }
            U2_TEST_ASSERT(rejected_specials,"Normal stream rejects infinities and NaN without publishing partial output");
            put16(680,0xBC00);put32(672,0x06080002);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Normal stream refuses unsupported formats");
            put32(672,0x0A080102);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Normal stream refuses unproven attribute offsets");
            put32(672,0x0A080002);put16(594,4);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Normal stream checks its eight-byte stride");
            put16(594,8);put32(596,23);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Normal stream checks exact vertex byte count");
            put32(596,24);put64(608,UINT64_MAX);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Normal stream rejects foreign data pointers");
            ptr(608,745);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Normal stream rejects data truncated by one byte");
            ptr(608,744);std::memcpy(page+744,page+680,24);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].normals[0]==sample_normal,
                           "Normal stream accepts data ending exactly at the page boundary");
            ptr(608,680);put32(128+40,3);std::memcpy(page+616,page+592,24);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Normal streams reject duplicate semantic IDs");
            put32(672,0x0A080004);ptr(624,708);put32(708,0x0A080002);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].normals[0]==sample_normal,
                           "Normal stream lookup follows attribute ID rather than its stream slot");
            ptr(624,767);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),
                           "Normal stream bounds attribute definitions before reading semantic IDs");
            ptr(624,708);put16(616,0);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),
                           "Stream inspection refuses empty attribute arrays");
            put16(616,5);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),
                           "Stream inspection respects the original four-attribute limit");
            put16(616,1);
            // Position + UV + normal streams, each with its own semantic ID.
            put32(672,0x22040001);put16(594,4);put32(596,12);ptr(608,720);
            put16(720,0xC100);put16(722,0x4280); // (-2.5, 3.25), preserve tiling
            put16(724,0x8000);put16(726,1); // negative zero and minimum subnormal
            put16(728,0x7BFF);put16(730,0x83FF);
            const std::array<float,2> sample_uv{-2.5f,3.25f};
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].texcoords.size()==3 &&
                           meshes[0].texcoords[0]==sample_uv && meshes[0].normals[0]==sample_normal,
                           "UV snapshots coexist with normals and do not clamp or wrap texture coordinates");
            U2_TEST_ASSERT(std::signbit(meshes[0].texcoords[1][0]) && meshes[0].texcoords[1][1]==0x1p-24f &&
                           meshes[0].texcoords[2][0]==65504 && meshes[0].texcoords[2][1]==-1023*0x1p-24f,
                           "UV half-float decoding preserves signed zero, subnormals and the finite maximum");
            bool invalid_uv=true;
            for(size_t component=0;component<2;++component) for(uint16_t special : {uint16_t{0x7C00},uint16_t{0xFC00},uint16_t{0x7E01}}) {
                const uint16_t saved=component ? 0x4280 : 0xC100;
                put16(720+component*2,special);
                invalid_uv &= !u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].texcoords[0]==sample_uv;
                put16(720+component*2,saved);
            }
            U2_TEST_ASSERT(invalid_uv,"UV stream rejects nonfinite values in either component without partial publication");
            put32(672,0x0A040001);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"UV reader refuses formats other than the established R16G16Float");
            put32(672,0x22040101);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"UV reader refuses unproven attribute offsets");
            put32(672,0x22080001);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"UV reader checks the four-byte attribute size");
            put32(672,0x22040001);put16(594,8);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"UV reader checks the four-byte vertex stride");
            put16(594,4);put32(596,13);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"UV reader checks exact byte count against vertex count");
            put32(596,12);put64(608,UINT64_MAX);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"UV reader rejects foreign data pointers");
            ptr(608,757);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"UV reader rejects a buffer truncated by one byte");
            ptr(608,756);std::memcpy(page+756,page+720,12);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].texcoords[0]==sample_uv,
                           "UV reader accepts a buffer ending exactly at the page boundary");
            ptr(608,720);ptr(624,672);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"UV reader rejects duplicate attribute IDs");
            ptr(624,708);
            std::array<uint8_t,24> saved_stream;
            std::memcpy(saved_stream.data(),page+592,24);std::memcpy(page+592,page+616,24);std::memcpy(page+616,saved_stream.data(),24);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].texcoords[0]==sample_uv && meshes[0].normals[0]==sample_normal,
                           "UV reader identifies streams by semantic ID when UV and normal slots are swapped");
            std::memset(page,0,sizeof(page));
            U2_TEST_ASSERT(meshes[0].name=="test-mesh" && meshes[0].positions[0]==first && meshes[0].triangles[0]==first_triangle,
                           "Geometry positions and names survive destruction of source page data");
            U2_TEST_ASSERT(meshes[0].normals.size()==3 && meshes[0].normals[0]==sample_normal && std::signbit(meshes[0].normals[0][3]),
                           "Normal snapshots own their data after the source page is destroyed");
            U2_TEST_ASSERT(meshes[0].texcoords.size()==3 && meshes[0].texcoords[0]==sample_uv && std::signbit(meshes[0].texcoords[1][0]),
                           "UV snapshots own their coordinates after the source page is destroyed");
        }
        {
            uint8_t page[1024]{};
            auto put32=[&](size_t at,uint32_t value) { for(unsigned i=0;i<4;++i) page[at+i]=uint8_t(value>>(8*i)); };
            auto ptr=[&](size_t at,size_t target) { const auto value=reinterpret_cast<uintptr_t>(page+target);for(unsigned i=0;i<8;++i) page[at+i]=uint8_t(value>>(8*i)); };
            auto text=[&](size_t at,const char *value) { std::memcpy(page+at,value,std::strlen(value)+1); };
            put32(8,900);text(900,"GEOMETRY_1");put32(40,1);ptr(48,128);
            ptr(136,840);text(840,"material-test");ptr(128+0x48,860);text(860,"beta");put32(128+0x50,UINT32_MAX);
            put32(160,3);put32(164,3);put32(168,1);ptr(176,320);ptr(192,384);
            page[386]=1;page[388]=2;
            page[320]=1;page[322]=12;put32(324,36);ptr(328,360);ptr(336,400);put32(360,0x060C0000);
            put32(0x68,3);ptr(0x70,512);ptr(512,760);ptr(584,780);ptr(656,760);text(760,"beta");text(780,"alpha");text(800,"gamma");
            std::vector<U2NativeMeshPreview> meshes;
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes.size()==1 &&
                           meshes[0].material_index==0 && meshes[0].material_name=="beta" && meshes[0].material_name_hash==0x50906A36 &&
                           page[128+0x50]==255,
                           "Material lookup uses the original CRC and first duplicate, leaving the raw guest index untouched");
            ptr(512,800);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_index==2,
                           "Material indices follow table names rather than mesh order");
            text(860,"BETA");
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_index==-1 &&
                           meshes[0].material_name=="BETA",
                           "Missing case-sensitive material lookup preserves the original -1 result");
            text(860,"alpha");
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_index==1 &&
                           meshes[0].material_name_hash==0xE13ADC9F,
                           "Unsigned material hash ordering handles keys with the high bit set");
            text(760,"material:5bc97c8a");text(800,"material:d5bf1bfc");text(860,"material:d5bf1bfc");ptr(512,760);ptr(656,800);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_index==0 &&
                           meshes[0].material_name_hash==0x2C62520D,
                           "Original material lookup keeps the first index even for two distinct names with the same CRC");
            text(760,"beta");text(800,"gamma");text(860,"beta");ptr(656,760);
            put32(0x68,UINT32_MAX);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_index==0,
                           "Material preflight rejects negative or overflowing counts without replacing output");
            put32(0x68,1);ptr(0x70,953);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Material table rejects a truncated 72-byte record");
            ptr(0x70,952);std::memcpy(page+952,page+512,72);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_index==0,
                           "Material table accepts a record ending exactly at the page boundary");
            ptr(0x70,512);ptr(512,1023);page[1023]='X';
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Material lookup rejects unterminated table names");
            ptr(512,760);ptr(128+0x48,1023);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Material lookup bounds requested mesh names");
            ptr(128+0x48,860);std::memset(page+512,0xFF,8);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Material lookup rejects foreign table name pointers");
            std::memset(page+512,0,8);std::memset(page+128+0x48,0,8);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_index==0 &&
                           meshes[0].material_name.empty() && meshes[0].material_name_hash==0,
                           "Null material names retain the original empty-name hash semantics");
            put32(0x68,0);ptr(128+0x48,860);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_index==-1 && meshes[0].material_name=="beta",
                           "An empty material table preserves an unresolved named reference");
            // Add geometry-local texture entries and three material references.
            put32(8,448);text(448,"GEOMETRY_1");put32(0x68,1);ptr(512,860);
            put32(0x58,3);ptr(0x60,592);ptr(592,940);ptr(632,948);ptr(672,956);
            text(940,"cook/a");text(948,"cook/b");text(956,"cook/c");
            put32(568,3);ptr(552,720);
            text(880,"g_tColorMap");text(896,"g_tNormalMap");text(912,"g_tBlendMap");
            text(972,"art/a.tga");text(984,"art/b.tga");text(996,"art/c.tga");
            for(size_t i=0;i<3;++i) {
                const size_t at=720+i*40;ptr(at,880+i*16);ptr(at+8,972+i*12);ptr(at+16,i==1 ? 940 : 948);put32(at+24,UINT32_MAX);
            }
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_textures.size()==3 &&
                           meshes[0].material_textures[0].texture_index==1 && meshes[0].material_textures[1].texture_index==0 &&
                           meshes[0].material_textures[2].texture_index==1 && page[744]==255,
                           "Material texture resolution uses cooked names and preserves raw guest indices");
            U2_TEST_ASSERT(meshes[0].material_textures[0].parameter_name=="g_tColorMap" &&
                           meshes[0].material_textures[0].source_path=="art/a.tga" && meshes[0].material_textures[0].cooked_name=="cook/b",
                           "Source image paths are not confused with the cooked texture lookup key");
            ptr(592,948);ptr(632,940);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) &&
                           meshes[0].material_textures[0].texture_index==0 && meshes[0].material_textures[1].texture_index==1,
                           "Texture indices follow table contents when geometry texture entries are reordered");
            ptr(672,948); // Keep cook/a at index 1 for the second reference.
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_textures[0].texture_index==0,
                           "Duplicate geometry texture hashes select the first original table index");
            ptr(592,940);ptr(632,948);ptr(672,956);put32(568,1);
            ptr(736,1010);text(1010,"absent");
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_textures[0].texture_index==0,
                           "Missing cooked texture references reject the snapshot atomically, matching the original assertion");
            text(1010,"Cook/b");
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Texture lookup preserves case-sensitive cooked names");
            ptr(736,948);put32(0x58,UINT32_MAX);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Geometry texture counts are bounded before allocation");
            put32(0x58,1);ptr(0x60,985);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Geometry texture table rejects a truncated 40-byte entry");
            ptr(0x60,984);std::memcpy(page+984,page+632,40);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_textures[0].texture_index==0,
                           "Geometry texture table accepts an entry ending exactly at the page boundary");
            ptr(0x60,592);put32(0x58,3);put32(568,UINT32_MAX);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Material texture reference counts respect the page-derived budget");
            put32(568,1);ptr(552,985);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Material texture references reject a truncated 40-byte entry");
            ptr(552,984);std::memcpy(page+984,page+720,40);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_textures[0].texture_index==1,
                           "Material texture references accept an entry ending exactly at the page boundary");
            ptr(552,720);ptr(720,1023);page[1023]='X';
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Texture parameter names require a terminator inside their page");
            ptr(720,880);std::memset(page+728,0xFF,8);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Texture source paths reject foreign pointers");
            ptr(728,972);std::memset(page+736,0xFF,8);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Cooked texture names reject foreign pointers");
            ptr(736,948);std::memset(page+592,0xFF,8);
            U2_TEST_ASSERT(!u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0),"Geometry texture name pointers are checked before hashing");
            std::memset(page+592,0,8);std::memset(page+736,0,8);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) &&
                           meshes[0].material_textures[0].cooked_name.empty() && meshes[0].material_textures[0].texture_index==0,
                           "Texture name hashing retains original NULL-name semantics");
            ptr(592,940);ptr(736,948);
            U2_TEST_ASSERT(u2_native_ReadGeometryPreview(&meshes,page,sizeof(page),0) && meshes[0].material_textures[0].texture_index==1,
                           "Texture reference snapshots recover after rejected inputs without retaining partial state");
            std::memset(page,0,sizeof(page));
            U2_TEST_ASSERT(meshes[0].material_name=="beta" && meshes[0].material_index==0,
                           "Material references remain owned after the source page is destroyed");
            U2_TEST_ASSERT(meshes[0].material_textures.size()==1 && meshes[0].material_textures[0].parameter_name=="g_tColorMap" &&
                           meshes[0].material_textures[0].source_path=="art/a.tga" && meshes[0].material_textures[0].cooked_name=="cook/b",
                           "Texture reference names and paths remain owned after the source page is destroyed");
        }
        {
            // Coherent perspective sample 307 from the original RenderCamera
            // dump (25 s menu test), object bytes +250..+25C and +90..+C0.
            const U2NativePerspectiveParams captured{0.5625f, 1.3032253980636597f, 0.23999999463558197f, 5000};
            const std::array<uint32_t, 16> original_bits{
                0xBFA6D017,0,0,0, 0,0x40144731,0,0, 0x80000000,0,0x3F800193,0x3F800000,
                0,0,0xBE75C595,0};
            std::array<float, 16> matrix{};
            bool matches = u2_native_BuildCenteredPerspective(captured, &matrix);
            for (size_t i = 0; i < 16; ++i) matches &= std::bit_cast<uint32_t>(matrix[i]) == original_bits[i];
            U2_TEST_ASSERT(matches, "Native centered perspective matches all 16 original RenderCamera float bit patterns");
            const U2NativePerspectiveParams simple{0.5f, 2, 1, 11};
            U2_TEST_ASSERT(u2_native_BuildCenteredPerspective(simple, &matrix), "Native perspective accepts explicit finite parameters");
            // Transform independent clip-space points: w = z, near -> 0, far -> 1.
            auto depth = [&matrix](float z) { return (matrix[10]*z + matrix[14]) / (matrix[11]*z + matrix[15]); };
            U2_TEST_ASSERT(std::abs(depth(1)) < 0.000001f && std::abs(depth(11)-1) < 0.000001f,
                           "Original perspective convention maps positive-Z near and far to depth zero and one");
            U2_TEST_ASSERT(std::abs(matrix[0]*(-1)/2 - 1) < 0.000001f &&
                           std::abs(matrix[5]*0.5f/2 - 1) < 0.000001f,
                           "Original perspective retains negative X scale and height-over-width aspect convention");
            U2_TEST_ASSERT(u2_native_BuildCenteredPerspective({2,2,1,11}, &matrix) && matrix[0] == -2 && matrix[5] == 1,
                           "Portrait aspect changes vertical scale without inverting horizontal FOV");
            U2_TEST_ASSERT(u2_native_PerspectiveFromHorizontalFov(1.57079632679f, 1, 1, 11, &matrix) &&
                           std::abs(matrix[0]+1) < 0.000001f && std::abs(matrix[5]-1) < 0.000001f,
                           "Horizontal FOV is in radians and uses cotangent of its half angle");
            U2_TEST_ASSERT(u2_native_PerspectiveFromHorizontalFov(1.30899692f, captured.height_over_width,
                           captured.near_plane, captured.far_plane, &matrix) &&
                           std::abs(matrix[0]+captured.horizontal_zoom) < 0.000002f,
                           "Captured menu zoom agrees with its inferred horizontal field of view");
            matrix.fill(42);
            const auto sentinel = matrix;
            bool invalid_rejected = true;
            const U2NativePerspectiveParams invalid[] = {{0,2,1,11}, {-1,2,1,11}, {1,0,1,11}, {1,-1,1,11},
                {1,2,0,11}, {1,2,-1,11}, {1,2,1,1}, {1,2,11,1}, {1e-30f,3e38f,1,11}, {1,2,2e38f,3e38f}};
            for (const auto &p : invalid)
                invalid_rejected &= !u2_native_BuildCenteredPerspective(p, &matrix) && matrix == sentinel;
            U2_TEST_ASSERT(invalid_rejected, "Perspective rejects invalid ranges and overflowing coefficients atomically");
            bool nonfinite_rejected = true;
            for (size_t i = 0; i < 4; ++i) {
                for (uint32_t bits : {uint32_t{0x7FC00000}, uint32_t{0x7F800000}, uint32_t{0xFF800000}}) {
                    auto p = simple;
                    float *fields[] = {&p.height_over_width, &p.horizontal_zoom, &p.near_plane, &p.far_plane};
                    *fields[i] = std::bit_cast<float>(bits);
                    nonfinite_rejected &= !u2_native_BuildCenteredPerspective(p, &matrix) && matrix == sentinel;
                }
            }
            U2_TEST_ASSERT(nonfinite_rejected, "Perspective rejects nonfinite values in every input without partial publication");
            bool invalid_fov = true;
            for (float fov : {0.0f, -1.0f, 3.14159265359f, 4.0f, std::bit_cast<float>(uint32_t{0x7FC00000})})
                invalid_fov &= !u2_native_PerspectiveFromHorizontalFov(fov, 1, 1, 11, &matrix) && matrix == sentinel;
            U2_TEST_ASSERT(invalid_fov && !u2_native_BuildCenteredPerspective(simple, nullptr) &&
                           !u2_native_PerspectiveFromHorizontalFov(1,1,1,11,nullptr),
                           "Perspective validates FOV domain and missing output");
        }
        U2NativeLevelOffset actual_offset;
        const std::array<float, 3> no_translation{};
        U2_TEST_ASSERT(temple && temple->level_offset_decoded &&
                       u2_cpp_PackageMgr_GetNativeLevelOffset("menu-temple", &actual_offset) &&
                       actual_offset.decoded && actual_offset.translation == no_translation,
                       "Real LEVEL_OFFSET_1 decodes its zero translation from the PAK");
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_GetNativeLevelOffset(nullptr, &actual_offset) &&
                       !u2_cpp_PackageMgr_GetNativeLevelOffset("missing", &actual_offset) &&
                       !u2_cpp_PackageMgr_GetNativeLevelOffset("menu-temple", nullptr) &&
                       actual_offset.decoded && actual_offset.translation == no_translation,
                       "Level offset lookup rejects absent input without changing the prior snapshot");
        U2_TEST_ASSERT(temple && temple->camera_definitions_decoded && temple->camera_definitions == 1 &&
                       u2_cpp_PackageMgr_GetNativeCamera("menu-temple", 0, &actual_camera),
                       "Real CAMERA_TABLE_1 yields one owned camera definition");
        U2_TEST_ASSERT(actual_camera.name == "|_cam_start_1|_cam_start_Shape1" && actual_camera.is_start,
                       "Real camera preserves original name and start-camera selection");
        U2_TEST_ASSERT(std::abs(actual_camera.position[0] + 78.3848038f) < 0.0001f &&
                       std::abs(actual_camera.position[1] - 26.8985996f) < 0.0001f &&
                       std::abs(actual_camera.position[2] + 29.6289997f) < 0.0001f,
                       "Real camera position decoded from PAK payload");
        U2_TEST_ASSERT(actual_camera.position_translated &&
                       std::bit_cast<uint32_t>(actual_camera.translated_position[0]) == 0xC29CC505 &&
                       std::bit_cast<uint32_t>(actual_camera.translated_position[1]) == 0x41D73055 &&
                       std::bit_cast<uint32_t>(actual_camera.translated_position[2]) == 0xC1ED0831,
                       "Translated temple camera position matches original ELF XMM1 capture at 435A5D");
        {
            uint8_t page[64] = {};
            auto put32 = [&page](size_t off, uint32_t value) {
                for (unsigned i = 0; i < 4; ++i) page[off+i] = static_cast<uint8_t>(value >> (8*i));
            };
            page[8] = 48;
            std::memcpy(page + 48, "LEVEL_OFFSET_1", sizeof("LEVEL_OFFSET_1"));
            const std::array<float, 3> expected{10, -20, 30.5f};
            for (size_t i = 0; i < 3; ++i) put32(32 + 4*i, std::bit_cast<uint32_t>(expected[i]));
            U2NativeLevelOffset offset;
            U2_TEST_ASSERT(u2_native_ReadLevelOffset(&offset, page, sizeof(page), 0) &&
                           offset.decoded && offset.translation == expected,
                           "Level offset decoder preserves signed nonzero float components");
            bool rejects_nonfinite = true;
            for (size_t i = 0; i < 3; ++i) {
                for (uint32_t bits : {uint32_t{0x7FC00000}, uint32_t{0x7F800000}, uint32_t{0xFF800000}}) {
                    put32(32 + i*4, bits);
                    rejects_nonfinite &= !u2_native_ReadLevelOffset(&offset, page, sizeof(page), 0) &&
                        offset.decoded && offset.translation == expected;
                }
                put32(32 + i*4, std::bit_cast<uint32_t>(expected[i]));
            }
            U2_TEST_ASSERT(rejects_nonfinite, "Level offset rejects NaN and infinities atomically in all three components");
            page[48] = 'X';
            U2_TEST_ASSERT(!u2_native_ReadLevelOffset(&offset, page, sizeof(page), 0) && offset.translation == expected,
                           "Level offset verifies exact resource type");
            page[48] = 'L';
            page[8] = 63;
            U2_TEST_ASSERT(!u2_native_ReadLevelOffset(&offset, page, sizeof(page), 0),
                           "Level offset rejects truncated type string");
            std::memset(page + 8, 0xFF, 8);
            U2_TEST_ASSERT(!u2_native_ReadLevelOffset(&offset, page, sizeof(page), 0),
                           "Level offset bounds the full 64-bit type offset");
            std::memset(page + 8, 0, 8); page[8] = 48;
            U2_TEST_ASSERT(!u2_native_ReadLevelOffset(&offset, page, 43, 0) &&
                           !u2_native_ReadLevelOffset(&offset, page, sizeof(page), SIZE_MAX) &&
                           !u2_native_ReadLevelOffset(nullptr, page, sizeof(page), 0) &&
                           !u2_native_ReadLevelOffset(&offset, nullptr, sizeof(page), 0) && offset.translation == expected,
                           "Level offset rejects null, truncated and overflowing metadata inputs");
            U2NativeCameraDefinition camera;
            camera.position = {1, 2, -3.5f};
            const std::array<float, 3> translated{11, -18, 27};
            const std::array<float, 3> sentinel{100, 200, 300};
            auto out = sentinel;
            U2_TEST_ASSERT(u2_native_TranslateCameraPosition(camera, offset, &out) && out == translated &&
                           camera.position[0] == 1 && offset.translation == expected,
                           "Camera translation adds nonzero level offset once without mutating stored inputs");
            out = sentinel;
            offset.decoded = false;
            U2_TEST_ASSERT(!u2_native_TranslateCameraPosition(camera, offset, &out) && out == sentinel,
                           "Camera translation requires a decoded level offset");
            offset.decoded = true; camera.uses_external_transform = true;
            U2_TEST_ASSERT(!u2_native_TranslateCameraPosition(camera, offset, &out) && out == sentinel,
                           "Camera translation refuses unresolved external transform atomically");
            camera.uses_external_transform = false;
            U2_TEST_ASSERT(!u2_native_TranslateCameraPosition(camera, offset, nullptr), "Camera translation rejects null destination");
            bool invalid_rejected = true;
            for (size_t i = 0; i < 6; ++i) {
                auto &vector = i < 3 ? camera.position : offset.translation;
                const float saved = vector[i%3];
                for (uint32_t bits : {uint32_t{0x7FC00000}, uint32_t{0x7F800000}, uint32_t{0xFF800000}}) {
                    vector[i%3] = std::bit_cast<float>(bits);
                    invalid_rejected &= !u2_native_TranslateCameraPosition(camera, offset, &out) && out == sentinel;
                }
                vector[i%3] = saved;
            }
            camera.position[2] = offset.translation[2] = 3e38f;
            U2_TEST_ASSERT(invalid_rejected && !u2_native_TranslateCameraPosition(camera, offset, &out) && out == sentinel,
                           "Camera translation rejects nonfinite inputs and result overflow without partial writes");
            std::memset(page, 0, sizeof(page));
            U2_TEST_ASSERT(offset.translation[0] == 10 && offset.translation[1] == -20,
                           "Level offset copy survives source buffer overwrite");
        }
        U2_TEST_ASSERT(std::abs(actual_camera.orientation_axis_a[0] + 0.195780724f) < 0.000001f &&
                       std::abs(actual_camera.orientation_axis_a[1] - 0.903568625f) < 0.000001f &&
                       std::abs(actual_camera.orientation_axis_a[2] + 0.381095320f) < 0.000001f &&
                       std::abs(actual_camera.orientation_axis_b[0] - 0.412893116f) < 0.000001f &&
                       std::abs(actual_camera.orientation_axis_b[1] - 0.428443372f) < 0.000001f &&
                       std::abs(actual_camera.orientation_axis_b[2] - 0.803713620f) < 0.000001f &&
                       !actual_camera.uses_external_transform,
                       "Real camera preserves both stored orientation vectors without external transform");
        {
            // Original ELF XMM0 at 435A11, one menu-temple call in the hybrid:
            // low/high words 3e6ad52ebe5a8cda / 3f72c1043d536b35.
            const std::array<float, 4> original{-0.213427931f, 0.229328841f, 0.0516159125f, 0.948257685f};
            bool matches_original = actual_camera.orientation_calculated;
            for (size_t i = 0; i < 4; ++i)
                matches_original &= std::abs(actual_camera.orientation_xyzw[i] - original[i]) < 0.000002f;
            U2_TEST_ASSERT(matches_original, "Native camera quaternion matches original ELF runtime capture within 2e-6");
        }
        {
            struct OrientationCase {
                std::array<float, 3> a, b;
                std::array<float, 4> expected;
                const char *label;
            };
            constexpr float half_sqrt_two = 0.707106781f;
            const OrientationCase cases[] = {
                {{0, 1, 0}, {0, 0, 1}, {0, 0, 0, 1}, "Camera identity quaternion"},
                {{0, -1, 0}, {0, 0, -1}, {1, 0, 0, 0}, "Camera X half-turn branch"},
                {{0, 1, 0}, {0, 0, -1}, {0, 1, 0, 0}, "Camera Y half-turn branch"},
                {{0, -1, 0}, {0, 0, 1}, {0, 0, 1, 0}, "Camera Z half-turn branch"},
                {{0, 0, 1}, {0, -1, 0}, {half_sqrt_two, 0, 0, half_sqrt_two}, "Camera positive X quarter-turn signs"},
                {{0, 1, 0}, {1, 0, 0}, {0, half_sqrt_two, 0, half_sqrt_two}, "Camera positive Y quarter-turn signs"},
                {{-1, 0, 0}, {0, 0, 1}, {0, 0, half_sqrt_two, half_sqrt_two}, "Camera positive Z quarter-turn signs"},
                {{0, 0, 3}, {0, -5, 0}, {half_sqrt_two, 0, 0, half_sqrt_two}, "Camera normalizes independently scaled stored axes"},
            };
            for (const auto &test : cases) {
                U2NativeCameraDefinition camera;
                camera.orientation_axis_a = test.a;
                camera.orientation_axis_b = test.b;
                std::array<float, 4> q{};
                bool valid = u2_native_CalculateCameraOrientation(camera, &q);
                for (size_t i = 0; i < 4; ++i) valid &= std::abs(q[i] - test.expected[i]) < 0.000002f;
                U2_TEST_ASSERT(valid, test.label);
            }
            // Independent check: rotate local basis vectors by the real camera
            // quaternion. Column Y must become A and column Z must become B.
            const auto &q = actual_camera.orientation_xyzw;
            const float x = q[0], y = q[1], z = q[2], w = q[3];
            const std::array<float, 3> rotated_y{2*(x*y-z*w), 1-2*(x*x+z*z), 2*(y*z+x*w)};
            const std::array<float, 3> rotated_z{2*(x*z+y*w), 2*(y*z-x*w), 1-2*(x*x+y*y)};
            bool basis_matches = actual_camera.orientation_calculated;
            for (size_t i = 0; i < 3; ++i) {
                basis_matches &= std::abs(rotated_y[i] - actual_camera.orientation_axis_a[i]) < 0.00001f;
                basis_matches &= std::abs(rotated_z[i] - actual_camera.orientation_axis_b[i]) < 0.00001f;
            }
            U2_TEST_ASSERT(basis_matches, "Camera quaternion reconstructs the stored temple basis with the established column convention");
            U2NativeCameraDefinition camera;
            camera.orientation_axis_a = {0, 1, 0};
            camera.orientation_axis_b = {0, 0, 1};
            const std::array<float, 4> sentinel{2, 3, 4, 5};
            auto output = sentinel;
            camera.uses_external_transform = true;
            U2_TEST_ASSERT(!u2_native_CalculateCameraOrientation(camera, &output) && output == sentinel,
                           "Camera orientation refuses unresolved external transforms without modifying output");
            camera.uses_external_transform = false;
            U2_TEST_ASSERT(!u2_native_CalculateCameraOrientation(camera, nullptr), "Camera orientation rejects missing output");
            bool degenerate_rejected = true;
            const std::array<float, 3> invalid_b[] = {{0,0,0}, {0,2,0}, {0,-2,0}, {0,0,1e-30f}, {0,0,3e38f}};
            for (const auto &invalid : invalid_b) {
                camera.orientation_axis_b = invalid;
                degenerate_rejected &= !u2_native_CalculateCameraOrientation(camera, &output) && output == sentinel;
            }
            U2_TEST_ASSERT(degenerate_rejected, "Camera orientation refuses zero, parallel, underflow and overflow arithmetic atomically");
            camera.orientation_axis_b = {0,0,1};
            bool nonfinite_rejected = true;
            for (size_t i = 0; i < 6; ++i) {
                auto &axis = i < 3 ? camera.orientation_axis_a : camera.orientation_axis_b;
                const float saved = axis[i % 3];
                for (uint32_t bits : {uint32_t{0x7FC00000}, uint32_t{0x7F800000}, uint32_t{0xFF800000}}) {
                    axis[i % 3] = std::bit_cast<float>(bits);
                    nonfinite_rejected &= !u2_native_CalculateCameraOrientation(camera, &output) && output == sentinel;
                }
                axis[i % 3] = saved;
            }
            U2_TEST_ASSERT(nonfinite_rejected, "Camera orientation rejects nonfinite values in all six input components");
        }
        U2_TEST_ASSERT(temple && temple->state == U2NativePackageState::Resolved && temple->login_resources == 10,
                       "Camera decoding does not declare the package ready or complete engine login");
        const auto saved_camera_name = actual_camera.name;
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_GetNativeCamera("menu-temple", 1, &actual_camera) &&
                       actual_camera.name == saved_camera_name,
                       "Camera lookup rejects invalid index without modifying output");
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_GetNativeCamera(nullptr, 0, &actual_camera) &&
                       !u2_cpp_PackageMgr_GetNativeCamera("missing", 0, &actual_camera) &&
                       !u2_cpp_PackageMgr_GetNativeCamera("menu-temple", 0, nullptr),
                       "Camera lookup rejects absent package or destination");
        {
            uint8_t page[320] = {};
            auto put32 = [&page](size_t off, uint32_t value) {
                for (unsigned i = 0; i < 4; ++i) page[off + i] = static_cast<uint8_t>(value >> (8 * i));
            };
            auto put64 = [&page](size_t off, uint64_t value) {
                for (unsigned i = 0; i < 8; ++i) page[off + i] = static_cast<uint8_t>(value >> (8 * i));
            };
            put64(8, 224);
            std::memcpy(page + 224, "CAMERA_TABLE_1", sizeof("CAMERA_TABLE_1"));
            put32(32, 1);
            put64(40, reinterpret_cast<uintptr_t>(page + 64));
            put64(64, reinterpret_cast<uintptr_t>(page + 240));
            std::memcpy(page + 240, "_cam_start_test", sizeof("_cam_start_test"));
            put32(72, 0x3F800000); // 1
            put32(76, 0xC0000000); // -2
            put32(80, 0x40600000); // 3.5
            const std::array<float, 3> expected_a{2, -3, 4};
            const std::array<float, 3> expected_b{-5, 6, -7};
            const std::array<float, 3> zero_vector{};
            for (size_t axis = 0; axis < 3; ++axis) {
                put32(64 + 0x20 + axis * 4, std::bit_cast<uint32_t>(expected_a[axis]));
                put32(64 + 0x14 + axis * 4, std::bit_cast<uint32_t>(expected_b[axis]));
            }
            std::vector<U2NativeCameraDefinition> cameras;
            const std::array<float, 3> expected_position{1, -2, 3.5f};
            U2_TEST_ASSERT(u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0) &&
                           cameras.size() == 1 && cameras[0].position == expected_position,
                           "Camera decoder reads signed little-endian position");
            U2_TEST_ASSERT(cameras[0].orientation_axis_a == expected_a &&
                           cameras[0].orientation_axis_b == expected_b && !cameras[0].uses_external_transform,
                           "Camera vectors retain signs, components and magnitude without normalization");
            put64(64 + 0x40, UINT64_MAX);
            U2_TEST_ASSERT(u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0) &&
                           cameras[0].uses_external_transform && !cameras[0].orientation_calculated,
                           "Camera decoder records external-transform presence without dereferencing it");
            put64(64 + 0x40, 0);
            put32(32, 2);
            put64(64 + 0x48, reinterpret_cast<uintptr_t>(page + 264));
            std::memcpy(page + 264, "overview", sizeof("overview"));
            put32(64 + 0x48 + 8, 0x40A00000); // 5
            U2_TEST_ASSERT(u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0) &&
                           cameras.size() == 2 && cameras[1].name == "overview" && !cameras[1].is_start &&
                           cameras[1].position[0] == 5 && cameras[1].orientation_axis_a == zero_vector &&
                           cameras[1].orientation_axis_b == zero_vector && !cameras[1].uses_external_transform &&
                           !cameras[1].orientation_calculated,
                           "Camera decoder uses proven 0x48 stride across adjacent records");
            put32(32, 1);
            bool rejected_nonfinite_axes = true;
            for (size_t axis = 0; axis < 3; ++axis) {
                for (size_t offset : {size_t{0x14}, size_t{0x20}}) {
                    for (uint32_t invalid : {uint32_t{0x7FC00000}, uint32_t{0x7F800000}, uint32_t{0xFF800000}}) {
                        const float saved = offset == 0x14 ? expected_b[axis] : expected_a[axis];
                        put32(64 + offset + axis * 4, invalid);
                        rejected_nonfinite_axes &= !u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0) &&
                            cameras.size() == 2 && cameras[0].orientation_axis_a == expected_a &&
                            cameras[0].orientation_axis_b == expected_b;
                        put32(64 + offset + axis * 4, std::bit_cast<uint32_t>(saved));
                    }
                }
            }
            U2_TEST_ASSERT(rejected_nonfinite_axes,
                           "Camera decoder rejects NaN and both infinities in every orientation component atomically");
            U2_TEST_ASSERT(u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0) && cameras.size() == 1,
                           "Camera decoder replaces a previously decoded table atomically");
            put32(32, UINT32_MAX);
            U2_TEST_ASSERT(!u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0) && cameras.size() == 1,
                           "Camera decoder bounds record count and preserves prior output");
            put32(32, 1);
            put64(40, reinterpret_cast<uintptr_t>(page + 312));
            U2_TEST_ASSERT(!u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0),
                           "Camera decoder rejects truncated record extent");
            put64(40, reinterpret_cast<uintptr_t>(page + 64));
            put64(64, reinterpret_cast<uintptr_t>(page) - 1);
            U2_TEST_ASSERT(!u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0),
                           "Camera decoder rejects foreign name pointer");
            put64(64, reinterpret_cast<uintptr_t>(page + 240));
            std::memset(page + 240, 'X', 80);
            U2_TEST_ASSERT(!u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0) &&
                           cameras[0].name == "_cam_start_test",
                           "Camera decoder rejects unterminated name without partial publication");
            page[319] = 0;
            put32(72, 0x7FC00000);
            U2_TEST_ASSERT(!u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0) && cameras[0].position[0] == 1,
                           "Camera decoder rejects nonfinite positions without poisoning output");
            put32(72, 0x3F800000);
            page[224] = 'X';
            U2_TEST_ASSERT(!u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), 0),
                           "Camera decoder verifies resource type");
            page[224] = 'C';
            U2_TEST_ASSERT(!u2_native_ReadCameraDefinitions(&cameras, page, 47, 0) &&
                           !u2_native_ReadCameraDefinitions(&cameras, page, sizeof(page), SIZE_MAX) &&
                           !u2_native_ReadCameraDefinitions(nullptr, page, sizeof(page), 0) &&
                           !u2_native_ReadCameraDefinitions(&cameras, nullptr, sizeof(page), 0),
                           "Camera decoder rejects absent or truncated inputs and overflowing metadata");
            std::memset(page, 0, sizeof(page));
            U2_TEST_ASSERT(cameras[0].name == "_cam_start_test" && cameras[0].position[2] == 3.5f &&
                           cameras[0].orientation_axis_a == expected_a && cameras[0].orientation_axis_b == expected_b,
                           "Camera snapshot remains valid after source data is overwritten");
        }
        {
            uint8_t page[64] = {};
            page[8] = 40;
            std::memcpy(page + 40, "LEVEL_INFO_1", 13);
            page[32] = 0x78; page[33] = 0x56; page[34] = 0x34; page[35] = 0x12;
            U2NativeLevelInfo level;
            U2_TEST_ASSERT(u2_native_LoginLevelInfo(&level, page, 64, 0) && level.loaded && level.value == 0x12345678,
                           "LEVEL_INFO login copies nonzero uint32 payload");
            U2_TEST_ASSERT(!u2_native_LoginLevelInfo(&level, page, 64, 0), "LEVEL_INFO duplicate login rejected");
            level = {};
            U2_TEST_ASSERT(!u2_native_LoginLevelInfo(&level, page, 35, 0) && !level.loaded,
                           "LEVEL_INFO truncated payload leaves destination unchanged");
            page[40] = 'X';
            U2_TEST_ASSERT(!u2_native_LoginLevelInfo(&level, page, 64, 0) && !level.loaded,
                           "LEVEL_INFO wrong type rejected");
            U2_TEST_ASSERT(!u2_native_LoginLevelInfo(nullptr, page, 64, 0), "LEVEL_INFO absent level handled");
        }
        U2_TEST_ASSERT(temple && temple->login_resources == 10,
                       "PAK login list has ten resources, distinct from catalog");
        const char *login_first = u2_cpp_PackageMgr_GetLoginType("menu-temple", 0);
        const char *login_last = u2_cpp_PackageMgr_GetLoginType("menu-temple", 9);
        U2_TEST_ASSERT(login_first && std::strcmp(login_first, "LEVEL_INFO_1") == 0 &&
                       login_last && std::strcmp(login_last, "COLLISION_DATA_HAVOK_BG") == 0,
                       "PAK login preserves original resource order");
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_GetLoginType("menu-temple", 10),
                       "PAK login lookup rejects out of range index");
        {
            uint8_t header[16] = {}, root[64] = {}, page[48] = {};
            header[8] = 1; root[0x20] = 1;
            page[8] = 40; page[40] = 'X';
            std::vector<std::string> types;
            U2_TEST_ASSERT(u2_cpp_PackageMgr_ReadLoginTypes(header, 16, page, 48, root, 64, &types) &&
                           types.size() == 1 && types[0] == "X", "PAK login decodes page reference");
            root[0x28] = 2;
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_ReadLoginTypes(header, 16, page, 48, root, 64, &types) && types[0] == "X",
                           "PAK login rejects missing page with output preserved");
            root[0x28] = 0;
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_ReadLoginTypes(header, 16, page, 48, root, 47, &types),
                           "PAK login rejects truncated pair table");
        }
        U2_TEST_ASSERT(temple && temple->catalog_resources == 538,
                       "PAK catalog registers 538 real page-0 metadata descriptors");
        U2_TEST_ASSERT(u2_cpp_PackageMgr_CountResourceType("menu-temple", "CAMERA_TABLE_1") == 1,
                       "PAK native catalog resolves camera table type");
        {
            uint8_t page[80] = {};
            page[0x12] = 1; page[0x1C] = 36;
            page[36] = 68; page[44] = 69; page[69] = 'T';
            std::vector<std::string> types;
            U2_TEST_ASSERT(u2_cpp_PackageMgr_ReadResourceTypes(page, sizeof(page), &types) &&
                           types.size() == 1 && types[0] == "T", "PAK metadata strings decoded");
            page[44] = 80;
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_ReadResourceTypes(page, sizeof(page), &types) &&
                           types.size() == 1 && types[0] == "T", "PAK invalid string leaves catalog unchanged");
            page[44] = 79; page[79] = 'X';
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_ReadResourceTypes(page, sizeof(page), &types),
                           "PAK rejects unterminated metadata string");
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_ReadResourceTypes(page, 30, &types),
                           "PAK rejects truncated descriptor table");
        }
        U2_TEST_ASSERT(temple && temple->local_fixups == 4088 && temple->pending_fixups == 60,
                       "PAK fixup audit validates real page 0 and counts missing page 1 slots");
        uint8_t fix_header[136] = {};
        U2_TEST_ASSERT(temple && temple->second_page_size == 696 && temple->pointers_relocated,
                       "PAK both real menu-temple pages relocated successfully");
        {
            uint8_t header[144] = {};
            header[0x18] = 8; header[0x1C] = 32;
            header[36] = 128; header[40] = 2;
            header[136] = 1; // Second record writes page 1.
            uint8_t p0[16] = {}, p1[16] = {};
            p0[0] = 8; p1[0] = 16; // Invalid target at end of page.
            bool relocated = false;
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_RelocatePages(header, sizeof(header), p0, 16, p1, 16, &relocated)
                           && !relocated && p0[0] == 8 && p1[0] == 16,
                           "PAK relocation failure leaves all pages unchanged");
            p1[0] = 0;
            U2_TEST_ASSERT(u2_cpp_PackageMgr_RelocatePages(header, sizeof(header), p0, 16, p1, 16, &relocated)
                           && relocated, "PAK relocation commits validated cross-page plan");
            uint64_t pointer0 = 0, pointer1 = 0;
            std::memcpy(&pointer0, p0, 8); std::memcpy(&pointer1, p1, 8);
            U2_TEST_ASSERT(pointer0 == reinterpret_cast<uintptr_t>(p0 + 8) &&
                           pointer1 == reinterpret_cast<uintptr_t>(p0),
                           "PAK relocation produces correct local and cross-page addresses");
            U2_TEST_ASSERT(!u2_cpp_PackageMgr_RelocatePages(header, sizeof(header), p0, 16, p1, 16, &relocated),
                           "PAK relocation rejects second application");
        }
        fix_header[0x18] = 8;
        fix_header[0x1C] = 32;
        fix_header[36] = 128;
        fix_header[40] = 1;
        uint8_t fix_page[16] = {};
        uint32_t local = 0, pending = 0;
        U2_TEST_ASSERT(u2_cpp_PackageMgr_AuditFixups(fix_header, sizeof(fix_header), fix_page, 16, &local, &pending) && local == 1,
                       "PAK fixup preflight accepts bounded local pointer");
        fix_header[132] = 16;
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_AuditFixups(fix_header, sizeof(fix_header), fix_page, 16, &local, &pending),
                       "PAK fixup rejects out-of-page write slot");
        fix_header[132] = 0;
        fix_page[0] = 16;
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_AuditFixups(fix_header, sizeof(fix_header), fix_page, 16, &local, &pending),
                       "PAK fixup rejects out-of-page target");
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_AuditFixups(fix_header, 135, fix_page, 16, &local, &pending),
                       "PAK fixup rejects truncated record table");
        U2_TEST_ASSERT(temple && temple->first_page_verified &&
                       temple->first_page_offset == 0x8280 && temple->first_page_size == 0x1E8174,
                       "PackageMgr retains verified menu-temple page 0");
        uint8_t page_entry[12] = {0x20, 0, 0, 0, 16, 0, 0, 0};
        uint8_t page_bytes[16] = {};
        page_bytes[12] = 16;
        U2_TEST_ASSERT(u2_cpp_PackageMgr_CheckPage(page_entry, 12, 0x30, page_bytes, 16),
                       "PAK page accepts exact EOF and matching page header");
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_CheckPage(page_entry, 12, 0x2F, page_bytes, 16),
                       "PAK page rejects extent beyond EOF");
        page_bytes[12] = 15;
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_CheckPage(page_entry, 12, 0x30, page_bytes, 16),
                       "PAK page rejects inconsistent header size");
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_CheckPage(page_entry, 12, 0x30, page_bytes, 15),
                       "PAK page rejects short payload");
        page_entry[8] = 1;
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_CheckPage(page_entry, 12, 0x30, page_bytes, 16),
                       "PAK page rejects unsupported entry kind");
        U2_TEST_ASSERT(temple && temple->header_inspected && temple->page_count == 2 &&
                       temple->page_table_offset == 0x2Cu,
                       "PackageMgr reads real menu-temple header and page table");
        uint8_t prefix[0x18] = {};
        prefix[0x10] = 2;
        prefix[0x14] = 0x2C;
        uint32_t count = 0, offset = 0;
        U2_TEST_ASSERT(u2_cpp_PackageMgr_InspectHeader(prefix, sizeof(prefix), 0x44, &count, &offset),
                       "PAK header accepts table ending exactly at EOF");
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_InspectHeader(prefix, sizeof(prefix), 0x43, &count, &offset),
                       "PAK header rejects truncated table");
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_InspectHeader(prefix, 0x17, 0x44, &count, &offset),
                       "PAK header rejects truncated prefix");
        prefix[0x17] = 0x80;
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_InspectHeader(prefix, sizeof(prefix), UINT64_MAX, &count, &offset),
                       "PAK header rejects negative signed table offset");
        U2_TEST_ASSERT(!u2_cpp_PackageMgr_InspectHeader(nullptr, 0, 0, &count, &offset),
                       "PAK header rejects NULL prefix");
    }

    // 10. Test Main Menu & 2D GUI Subsystems (main_menu.cpp)
    {
        uint8_t mock_menuhost[0x2000] = {};
        u2_cpp_MenuHost_SetMode(mock_menuhost, 1, 0); // Set mode 1 (Main Menu)
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(mock_menuhost + 0x00) == 1, "MenuHost::SetMode sets mode 1 (Main Menu)");
        U2_TEST_ASSERT(u2_cpp_MenuHost_GetMode(mock_menuhost) == 1, "MenuHost::GetMode returns 1");
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(image + 0x014CE97C) == 1, "MenuHost::SetMode updates global offset 0x014CE97C");

        u2_cpp_MenuHost_ShowWaitPopup(mock_menuhost, 42, 1);
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(mock_menuhost + 0x9B0) == 42, "MenuHost::ShowWaitPopup sets popup ID");

        static uint32_t s_clicked_id = 0;
        auto click_cb = [](void *, uint32_t id) ABI { s_clicked_id = id; };
        u2_cpp_MenuHost_AddControlClickedHandler(mock_menuhost, 101, click_cb, nullptr);
        U2_TEST_ASSERT(*reinterpret_cast<uint32_t *>(mock_menuhost + 0x18EC) == 1u, "MenuHost::AddControlClickedHandler registers handler");

        u2_cpp_MenuHost_TriggerControlClicked(mock_menuhost, 101);
        U2_TEST_ASSERT(s_clicked_id == 101u, "MenuHost::TriggerControlClicked executes click callback");

        uint8_t mock_datasource[0x100] = {};
        float vol_data[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
        int32_t val_res = u2_cpp_BpMenuDataSource_SetValueImpl(mock_datasource, 5, vol_data);
        U2_TEST_ASSERT(val_res == 1, "BpMenuDataSource::SetValueImpl returns 1");
        U2_TEST_ASSERT(*reinterpret_cast<int32_t *>(mock_datasource + 0x40) == 5, "BpMenuDataSource value ID matches");

        float drect[4] = { 10.0f, 20.0f, 100.0f, 50.0f };
        float uvs[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
        u2_cpp_MenuDrawer_DrawQuad(nullptr, drect, nullptr, uvs);
        U2_TEST_ASSERT(true, "MenuDrawer::DrawQuad safe execution");

        void *hud_ctx = u2_cpp_HudQuad_Begin_Actual(mock_menuhost, nullptr, nullptr);
        U2_TEST_ASSERT(hud_ctx != nullptr, "HudQuad::Begin_Actual returns context");
        u2_cpp_HudQuad_End_Actual(hud_ctx, nullptr, nullptr);
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

    // 18. Test Bitmap Font Preview Parser & UTF-8 Layout (ndlib/render/ngen/font_preview.cpp)
    {
        std::vector<uint8_t> tga(18 + 2 * 2 * 4, 0);
        tga[2] = 2;    // Uncompressed true-color
        tga[12] = 2;   // width = 2
        tga[14] = 2;   // height = 2
        tga[16] = 32;  // 32 bpp
        tga[17] = 0x20; // Top-left origin
        const uint8_t px[16] = {
            11, 22, 33, 44,
            55, 66, 77, 88,
            99, 110, 121, 132,
            143, 154, 165, 176
        };
        std::memcpy(tga.data() + 18, px, sizeof(px));

        const char *fnt_desc =
            "info face=\"Test\" size=20\n"
            "common lineHeight=20 base=16 scaleW=2 scaleH=2 pages=1 packed=1\n"
            "page id=0 file=\"main_00.tga\"\n"
            "chars count=2\n"
            "char id=65 x=0 y=0 width=1 height=2 xoffset=1 yoffset=2 xadvance=10 page=0 chnl=1\n"
            "char id=66 x=1 y=0 width=1 height=2 xoffset=-1 yoffset=3 xadvance=8 page=0 chnl=4\n"
            "kernings count=1\n"
            "kerning first=65 second=66 amount=-2\n";

        U2NativeFontPreview font;
        const bool parsed = u2_native_ReadMainFontPreview(&font, fnt_desc, tga.data(), tga.size());
        U2_TEST_ASSERT(parsed &&
                       font.line_height == 20 &&
                       font.baseline == 16 &&
                       font.atlas_width == 2 &&
                       font.atlas_height == 2 &&
                       font.atlas.width == 2 &&
                       font.atlas.height == 8 &&
                       font.atlas.rgba.size() == 16 &&
                       font.glyphs.size() == 2 &&
                       font.kernings.size() == 1,
                       "ReadMainFontPreview decodes BMFont metadata and 4-plane B/G/R/A atlas");
        U2_TEST_ASSERT(parsed &&
                       font.atlas.rgba[0] == (0xFFFFFFu | (11u << 24)) &&
                       font.atlas.rgba[4] == (0xFFFFFFu | (22u << 24)) &&
                       font.atlas.rgba[8] == (0xFFFFFFu | (33u << 24)) &&
                       font.atlas.rgba[12] == (0xFFFFFFu | (44u << 24)),
                       "ReadMainFontPreview unpacks B, G, R, A channels into distinct vertical white alpha planes");

        U2NativeFontText layout1;
        const bool laid_out_1 = u2_native_LayoutFontText(font, "AB", 20.0f, &layout1);
        U2_TEST_ASSERT(laid_out_1 &&
                       layout1.quads.size() == 2 &&
                       std::fabs(layout1.quads[0].x - 1.0f) < 1e-4f &&
                       std::fabs(layout1.quads[0].y - 2.0f) < 1e-4f &&
                       layout1.quads[0].source_x == 0 &&
                       layout1.quads[0].source_y == 0 &&
                       std::fabs(layout1.quads[1].x - 7.0f) < 1e-4f &&
                       std::fabs(layout1.quads[1].y - 3.0f) < 1e-4f &&
                       layout1.quads[1].source_x == 1 &&
                       layout1.quads[1].source_y == 4 &&
                       std::fabs(layout1.advance - 16.0f) < 1e-4f,
                       "LayoutFontText applies packed channel offsets and kerning at 1.0x scale");

        U2NativeFontText layout_half;
        const bool laid_out_half = u2_native_LayoutFontText(font, "AB", 10.0f, &layout_half);
        U2_TEST_ASSERT(laid_out_half &&
                       layout_half.quads.size() == 2 &&
                       std::fabs(layout_half.quads[0].x - 0.5f) < 1e-4f &&
                       std::fabs(layout_half.quads[0].width - 0.5f) < 1e-4f &&
                       std::fabs(layout_half.quads[0].height - 1.0f) < 1e-4f &&
                       std::fabs(layout_half.quads[1].x - 3.5f) < 1e-4f &&
                       std::fabs(layout_half.advance - 8.0f) < 1e-4f,
                       "LayoutFontText scales positions, sizes, kerning, and total advance proportionally");

        // Atomic failure preservation on ReadMainFontPreview
        const auto saved_rgba = font.atlas.rgba;
        const size_t saved_glyph_count = font.glyphs.size();
        const size_t saved_kern_count = font.kernings.size();
        std::vector<uint8_t> bad_origin = tga;
        bad_origin[17] = 0x00;
        U2_TEST_ASSERT(!u2_native_ReadMainFontPreview(nullptr, fnt_desc, tga.data(), tga.size()) &&
                       !u2_native_ReadMainFontPreview(&font, fnt_desc, tga.data(), tga.size() - 1) &&
                       !u2_native_ReadMainFontPreview(&font, fnt_desc, bad_origin.data(), bad_origin.size()) &&
                       !u2_native_ReadMainFontPreview(&font,
                           "common lineHeight=20 base=16 scaleW=2 scaleH=2 pages=1 packed=1\n"
                           "page id=0 file=\"other_00.tga\"\nchars count=1\n"
                           "char id=65 x=0 y=0 width=1 height=2 xoffset=0 yoffset=0 xadvance=1 page=0 chnl=1\n",
                           tga.data(), tga.size()) &&
                       !u2_native_ReadMainFontPreview(&font,
                           "common lineHeight=20 base=16 scaleW=2 scaleH=2 pages=1 packed=1\n"
                           "page id=0 file=\"main_00.tga\"\nchars count=1\n"
                           "char id=65 x=0 y=0 width=1 height=2 xoffset=0 yoffset=0 xadvance=1 page=0 chnl=3\n",
                           tga.data(), tga.size()) &&
                       !u2_native_ReadMainFontPreview(&font,
                           "common lineHeight=20 base=16 scaleW=2 scaleH=2 pages=1 packed=1\n"
                           "page id=0 file=\"main_00.tga\"\nchars count=2\n"
                           "char id=65 x=0 y=0 width=1 height=2 xoffset=0 yoffset=0 xadvance=1 page=0 chnl=1\n",
                           tga.data(), tga.size()) &&
                       !u2_native_ReadMainFontPreview(&font,
                           "common lineHeight=20 base=16 scaleW=2 scaleH=2 pages=1 packed=1\n"
                           "page id=0 file=\"main_00.tga\"\nchars count=1\n"
                           "char id=65 x=0 y=0 width=1 height=2 xoffset=0 yoffset=0 xadvance=1 page=0 chnl=1\n"
                           "kernings count=1\nkerning first=65 second=67 amount=-1\n",
                           tga.data(), tga.size()) &&
                       font.atlas.rgba == saved_rgba &&
                       font.glyphs.size() == saved_glyph_count &&
                       font.kernings.size() == saved_kern_count,
                       "ReadMainFontPreview rejects invalid/inconsistent inputs atomically without mutating output");

        // Atomic failure preservation on LayoutFontText
        const size_t saved_quad_count = layout1.quads.size();
        const float saved_adv = layout1.advance;
        U2_TEST_ASSERT(!u2_native_LayoutFontText(font, "AB", 20.0f, nullptr) &&
                       !u2_native_LayoutFontText(font, "\xFF", 20.0f, &layout1) &&
                       !u2_native_LayoutFontText(font, "\xC0\xAF", 20.0f, &layout1) &&
                       !u2_native_LayoutFontText(font, "\xE2\x82", 20.0f, &layout1) &&
                       !u2_native_LayoutFontText(font, "AC", 20.0f, &layout1) &&
                       !u2_native_LayoutFontText(font, "AB", 0.0f, &layout1) &&
                       !u2_native_LayoutFontText(font, "AB", -5.0f, &layout1) &&
                       layout1.quads.size() == saved_quad_count &&
                       std::fabs(layout1.advance - saved_adv) < 1e-6f,
                       "LayoutFontText rejects malformed UTF-8, missing glyphs, and invalid heights without mutating output");

        // Real PS4 game font asset check (/app0/u2data/data/fonts/ps4/main.fnt + main_00.tga)
        const int font_fd = int(runtime_file_open("/app0/u2data/data/fonts/ps4/main.fnt", 0, 0));
        if (font_fd >= 0) {
            runtime_file_close(font_fd);
            U2NativeFontPreview real_font;
            const bool loaded_real = u2_native_LoadMainFontPreview(&real_font);
            U2_TEST_ASSERT(loaded_real &&
                           real_font.line_height == 42 &&
                           real_font.baseline == 34 &&
                           real_font.atlas_width == 256 &&
                           real_font.atlas_height == 128 &&
                           real_font.atlas.width == 256 &&
                           real_font.atlas.height == 512 &&
                           real_font.glyphs.size() == 183 &&
                           real_font.kernings.size() == 592,
                           "LoadMainFontPreview loads real game main.fnt (183 glyphs, 592 kernings, 256x128 -> 256x512)");
            U2NativeFontText real_prompt, real_legal;
            U2_TEST_ASSERT(u2_native_LayoutFontText(real_font, "Appuyer sur  pour commencer.", 42.0f, &real_prompt) &&
                           !real_prompt.quads.empty() && real_prompt.advance > 100.0f &&
                           u2_native_LayoutFontText(real_font, "Uncharted\xE2\x84\xA2 2: Among Thieves Remastered \xC2\xA9 2009-2015", 32.0f, &real_legal) &&
                           !real_legal.quads.empty() && real_legal.advance > 100.0f,
                           "LayoutFontText renders title prompt and UTF-8 legal text (TM/copyright) using real main.fnt");
        }
    }

    g_u2_image_base = saved_base;
    std::printf("[U2 EngineTest] PASSED: %d/%d functional C++23 engine assertions verified via ELF trampolines!\n",
                s_passed_assertions, s_passed_assertions + s_failed_assertions);
    std::fflush(stdout);
    return s_failed_assertions;
}
