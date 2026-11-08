/* Uncharted 2: Scene Actors, Entity Queries, Shipping Stubs & Utilities */
#include "runtime_u2_common.h"

static int64_t ABI detour_DcEntityName(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A127D0: (entity-name ent) */
    (void)status;
    if (argc <= 0 || !argv || !argv[0]) return 0;
    return *(const uint32_t *)(uintptr_t)(argv[0] + 0x28u);
}

static int64_t ABI detour_DcSymbolEqualP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00397620: (symbol-equal? a b) */
    (void)argc; (void)status;
    return ((uint32_t)argv[0] == (uint32_t)argv[1]) ? 1 : 0;
}

static int64_t ABI detour_DcCameraCancelAimOverride(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0DE30: (camera-cancel-aim-override) */
    (void)argc; (void)argv; (void)status;
    *(uint32_t *)(g_u2_image_base + 0x0147F0BCull) = 0u;
    return 1;
}

static int64_t ABI detour_DcCameraRestoreLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E9B0: (camera-restore-locator) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x0147F0B0ull) = 1u;
    return 0;
}

static int64_t ABI detour_DcDebugPause(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A102A0: (debug-pause) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x010595E4ull) = 1u;
    return 0;
}

static int64_t ABI detour_DcDiscP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD910: (disc? n) */
    (void)argc; (void)status;
    uint32_t cur_disc = *(const uint32_t *)(g_u2_image_base + 0x0105CFE8ull);
    return ((uint32_t)argv[0] == cur_disc) ? 1 : 0;
}

static int64_t ABI detour_DcResetAimFov(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D780: (reset-aim-fov) */
    (void)argc; (void)argv; (void)status;
    *(uint32_t *)(g_u2_image_base + 0x0104F8E0ull) = 0xBF800000u; /* -1.0f */
    return 0;
}

static int64_t ABI detour_DcOverrideAimFov(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D7A0: (override-aim-fov fov) */
    (void)argc; (void)status;
    *(uint32_t *)(g_u2_image_base + 0x0104F8E0ull) = (uint32_t)argv[0];
    return 0;
}

static int64_t ABI detour_DcGetHarvestingPriority(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01AB0: (get-harvesting-priority) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x0110DCB0ull);
}

static int64_t ABI detour_DcSetHarvestingPriority(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01AC0: (set-harvesting-priority prio) */
    (void)status;
    if (argc > 0) {
        uint8_t v = (uint8_t)argv[0];
        if (v > 0xFDu) v = 0xFDu;
        *(uint8_t *)(g_u2_image_base + 0x0110DCB0ull) = v;
    }
    return 0;
}

static int64_t ABI detour_DcResetHarvestingPriority(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01AE0: (reset-harvesting-priority) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x0110DCB0ull) = 0x0Au;
    return 0;
}

static int64_t ABI detour_DcHarvestingEnabledP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01AF0: (harvesting-enabled?) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x0110DCA0ull);
}

static int64_t ABI detour_DcSetHarvestingEnabled(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01B00: (set-harvesting-enabled flag) */
    (void)status;
    if (argc > 0) {
        *(uint8_t *)(g_u2_image_base + 0x0110DCA0ull) = ((uint32_t)argv[0] != 0u) ? 1u : 0u;
    }
    return 0;
}

static int64_t ABI detour_DcHideHud(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0CD00: (hide-hud) */
    (void)argc; (void)argv; (void)status;
    uint8_t *hud = *(uint8_t **)(g_u2_image_base + 0x01130068ull);
    if (hud) *(hud + 0x40Du) = 1u;
    return 0;
}

static int64_t ABI detour_DcIsFma(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FE80: (is-fma) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x0105DECDull);
}

static int64_t ABI detour_DcJournalClearPage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD090: (journal-clear-page) */
    (void)argc; (void)argv; (void)status;
    *(int32_t *)(g_u2_image_base + 0x0105DFC0ull) = -1;
    return 0;
}

static int64_t ABI detour_DcJournalGetPage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD110: (journal-get-page) */
    (void)argc; (void)argv; (void)status;
    return (int64_t)(*(const int32_t *)(g_u2_image_base + 0x0105DFC0ull) + 1);
}

static int64_t ABI detour_DcKillAllParticles(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FE60: (kill-all-particles) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x013C2DC4ull) = 1u;
    return 0;
}

static int64_t ABI detour_DcGetCaptureZone(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006687D0: (get-capture-zone) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetPlyaerCoopId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669240: (net-plyaer-coop-id) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetCoopTeamFailedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C20: (net-coop-team-failed?) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x0113D910ull);
}

static int64_t ABI detour_DcNetCoopScriptId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C30: (net-coop-script-id) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetCoopHostScriptId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C40: (net-coop-host-script-id) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetIsCinema(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C50: (net-is-cinema) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    int32_t net_mode = *(const int32_t *)(g_u2_image_base + 0x014CE97Cull);
    if (net_mode < 2) return 0;
    return (*(const uint8_t *)(g_u2_image_base + 0x0113B760ull + 0x209C) != 0) ? 1 : 0;
}

static int64_t ABI detour_DcPsnDemoShowUpsell(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD590: (psn-demo-show-upsell) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x0113769Dull) = 1u;
    return 0;
}

static int64_t ABI detour_DcAutoSaveGame(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE820: (auto-save-game) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint8_t enabled = *(const uint8_t *)(g_u2_image_base + 0x0105053Dull);
    if (!enabled) return 0;
    void *save_mgr = (void *)(g_u2_image_base + 0x01050E68ull);
    int32_t slot = *(const int32_t *)((const uint8_t *)save_mgr + 0x1FC);
    if (slot < 0) return 0;
    typedef void (ABI *SaveReqFn)(void *mgr, uint32_t action, int32_t slot, uint32_t mode, uint32_t flags, int32_t extra);
    SaveReqFn save_fn = (SaveReqFn)(uintptr_t)(g_u2_image_base + 0x00CD2800ull);
    if (save_fn) {
        save_fn(save_mgr, 1, 10, 0, 4, -1);
        save_fn(save_mgr, 1, slot, 0, 0, -1);
    }
    return 0;
}

static int64_t ABI detour_DcGameTaskGraphLogin(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE8F0: (game-task-graph-login task) */
    (void)status;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    if (argc > 0 && argv) {
        typedef void (ABI *GameTaskGraphLoginFn)(uint64_t task);
        GameTaskGraphLoginFn login_fn = (GameTaskGraphLoginFn)(uintptr_t)(g_u2_image_base + 0x00A59960ull);
        if (login_fn) {
            login_fn(argv[0]);
        }
    }
    return 0;
}

static int64_t ABI detour_DcRailSetIgcFadeOutTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB93F0: (rail-set-igc-fade-out-time t) */
    (void)status;
    *(uint32_t *)(g_u2_image_base + 0x0153FCB4ull) = (argc > 0) ? (uint32_t)argv[0] : 0u;
    return 0;
}

static int64_t ABI detour_DcTrainGetDefaultAcceleration(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A682E0: (train-get-default-acceleration) -> 2.0f (0x40000000) */
    (void)argc; (void)argv; (void)status;
    return 0x40000000ll;
}

static int64_t ABI detour_DcTutorialHintsOnP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F490: (tutorial-hints-on?) */
    (void)argc; (void)argv; (void)status;
    return (int64_t)(*(const int32_t *)(g_u2_image_base + 0x0105DEF0ull));
}

static int64_t ABI detour_DcUseWaterUnitForReflectionIfVisible(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02C50: (use-water-unit-for-reflection-if-visible) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x011649DDull) = 1u;
    return 1;
}

static int64_t ABI detour_DcAngleMod(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00397630: (angle-mod rad) -> wraps angle into [-pi, pi] */
    (void)argc; (void)status;
    const float two_pi = 6.28318530717958647692f;
    const float pi     = 3.14159265358979323846f;
    float x = u2_reg_to_f32(argv[0]);
    if (x < -pi || x > pi) {
        x = fmodf(x, two_pi);
        if (x > pi) x -= two_pi;
        else if (x < -pi) x += two_pi;
    }
    return (int64_t)u2_f32_to_reg(x);
}

static int64_t ABI detour_DcAnimate(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00397C50: (animate ...) */
    (void)status;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, int32_t, const char *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00396180ull);
    return fn(argc, argv, 0, "animate");
}

static int64_t ABI detour_DcActionPackEnabledP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09FF0: (action-pack-enabled? ...) */
    (void)status;
    typedef uint8_t *(ABI *fn_t)(int32_t *, int32_t, uint64_t *, const char *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x009F2D40ull);
    int32_t err = 0;
    uint8_t *obj = fn(&err, argc, argv, "action-pack-enabled?");
    if (!obj) return 0;
    return ((~(*(const uint8_t *)(obj + 0xB8))) >> 6) & 1;
}

static int64_t ABI detour_DcHudSetCounter(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10470: (hud-set-counter cur max) */
    (void)argc; (void)status;
    uint32_t v0 = (uint32_t)argv[0];
    uint32_t v1 = (uint32_t)argv[1];
    uint8_t *base = g_u2_image_base + 0x01130008ull;
    uint8_t *counter = *(uint8_t **)(base + 0x328);
    if (counter) {
        *(uint32_t *)(counter + 0x40) = v0;
        *(uint32_t *)(counter + 0x44) = v1;
    }
    uint32_t *flags_ptr = *(uint32_t **)(base + 0x60);
    if (flags_ptr) {
        if (v0 == 0) *flags_ptr &= ~0x4000u;
        else         *flags_ptr |=  0x4000u;
    }
    return 0;
}

static int64_t ABI detour_DcHudSetTimer(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A104B0: (hud-set-timer time-seconds) */
    (void)argc; (void)status;
    if (!g_u2_image_base) return 0;
    float time_val = u2_reg_to_f32(argv[0]);
    float scale = *(const float *)(g_u2_image_base + 0x00E67418ull);
    int32_t ticks = (int32_t)(time_val * scale);
    uint8_t *base = g_u2_image_base + 0x01130008ull;
    uint8_t *timer = *(uint8_t **)(base + 0x318);
    if (timer) {
        *(int32_t *)(timer + 0x40) = ticks;
    }
    uint32_t *flags_ptr = *(uint32_t **)(base + 0x60);
    if (flags_ptr) {
        float inv_scale = *(const float *)(g_u2_image_base + 0x00E6741Cull);
        float computed = (float)ticks * inv_scale;
        if (computed > 0.0f) {
            *flags_ptr |= 0x1000u;
        } else {
            *flags_ptr &= ~0x1000u;
        }
    }
    return 0;
}

static int64_t ABI detour_DcIncrementStat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0097BDF0: (increment-stat stat amount) */
    (void)status;
    if (argc == 2) {
        typedef uint8_t (ABI *fn_t)(void *, uint32_t, uint32_t, uint32_t, uint32_t);
        fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x0097A630ull);
        return (int64_t)fn(g_u2_image_base + 0x0115BD40ull, (uint32_t)argv[1], (uint32_t)argv[0], 0u, 0u);
    }
    return 0;
}

static int64_t ABI detour_DcIncrementTaskStat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0097BE20: (increment-task-stat stat sub amount) */
    (void)status;
    if (argc == 3) {
        typedef uint8_t (ABI *fn_t)(void *, uint32_t, uint32_t, uint32_t, uint32_t);
        fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x0097A630ull);
        return (int64_t)fn(g_u2_image_base + 0x0115BD40ull, (uint32_t)argv[2], (uint32_t)argv[0], (uint32_t)argv[1], 0u);
    }
    return 0;
}

static int64_t ABI detour_DcIsDifficultyVeryEasy(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FCFC0: (is-difficulty-very-easy) */
    (void)argc; (void)argv; (void)status;
    uint32_t diff = *(const uint32_t *)(g_u2_image_base + 0x01059640ull + 0x48A4ull);
    int32_t mode  = *(const int32_t *)(g_u2_image_base + 0x014CE97Cull);
    return (diff == 0u && mode < 2) ? 1 : 0;
}

static int64_t ABI detour_DcGetDifficulty(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD040: (get-difficulty) */
    (void)argc; (void)argv; (void)status;
    if (!g_u2_image_base) return 2;
    int32_t mode = *(const int32_t *)(g_u2_image_base + 0x014CE97Cull);
    if (mode > 1) {
        return 2;
    }
    return (int64_t)*(const int32_t *)(g_u2_image_base + 0x01059640ull + 0x48A4ull);
}

static int64_t ABI detour_DcJournalOpenP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD0B0: (journal-open?) */
    (void)argc; (void)argv; (void)status;
    uint8_t *player = u2_get_active_player();
    if (!player) return 0;
    uint8_t *p50 = *(uint8_t **)(player + 0x50);
    if (!p50) return 0;
    uint8_t *p0 = *(uint8_t **)p50;
    if (!p0) return 0;
    uint8_t *p10 = *(uint8_t **)(p0 + 0x10);
    if (!p10) return 0;
    return (*(const uint32_t *)(p10 + 0x18) == 0xAB4D428Cu) ? 1 : 0;
}

static int64_t ABI detour_DcJournalSetAmbientScale(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD060: (journal-set-ambient-scale [scale]) */
    (void)status;
    uint32_t bits = (argc > 0) ? (uint32_t)argv[0] : 0x3F800000u; /* 1.0f */
    *(uint32_t *)(g_u2_image_base + 0x01059640ull + 0x4984ull) = bits;
    return 0;
}

static int64_t ABI detour_DcKillMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DC630: (kill-message id) */
    (void)argc; (void)status;
    typedef void (ABI *fn_t)(void *, uint32_t);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x005D9D80ull);
    fn(g_u2_image_base + 0x01139EA0ull, (uint32_t)argv[0]);
    return 0;
}

static int64_t ABI detour_DcPowFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDB60: (pow-float base exp) */
    (void)argc; (void)status;
    float b = u2_reg_to_f32(argv[0]);
    float e = u2_reg_to_f32(argv[1]);
    return (int64_t)u2_f32_to_reg(powf(b, e));
}

static int64_t ABI detour_DcLerpScale(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDB90: (lerp-scale min-in max-in min-out max-out val) */
    (void)status;
    if (argc < 5) return 0;
    float min_in  = u2_reg_to_f32(argv[0]);
    float max_in  = u2_reg_to_f32(argv[1]);
    float min_out = u2_reg_to_f32(argv[2]);
    float max_out = u2_reg_to_f32(argv[3]);
    float val     = u2_reg_to_f32(argv[4]);
    float denom   = max_in - min_in;
    float t       = (denom != 0.0f) ? ((val - min_in) / denom) : 0.0f;
    if (t < 0.0f) t = 0.0f;
    else if (t > 1.0f) t = 1.0f;
    float res = min_out * (1.0f - t) + max_out * t;
    return (int64_t)u2_f32_to_reg(res);
}

static int64_t ABI detour_DcRailActivate(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB98D0: (rail-activate ...) */
    (void)status;
    if (argc > 0 && argv[0] != 0) {
        typedef void (ABI *fn_t)(int32_t, uint64_t);
        fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00A8FD50ull);
        fn(argc, argv[0]);
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcRailDeactivateMover(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8290: (rail-deactivate-mover [idx]) */
    (void)status;
    uint32_t idx = (argc > 0) ? (uint32_t)argv[0] : 0u;
    if (idx < 2u) {
        uint8_t *m = g_u2_image_base + 0x0153EFA8ull + (size_t)idx * 0x30u;
        *(uint32_t *)(m + 0xD18) = 0u;
        *(uint64_t *)(m + 0xD30) = 0ull;
        *(uint32_t *)(m + 0xD40) = 0xFFFFFFFFu;
        *(uint32_t *)(m + 0xD3C) = 0u;
        *(uint32_t *)(m + 0xD38) = 0u;
        *(uint32_t *)(m + 0xD20) = 0xFFFFFFFFu;
        *(uint64_t *)(m + 0xD28) = 0ull;
    }
    return 0;
}

static int64_t ABI detour_DcRailKillAllPhysicsObjects(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8F60: (rail-kill-all-physics-objects) */
    (void)argc; (void)argv; (void)status;
    typedef void (ABI *fn_t)(void);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x0000EB10ull);
    fn();
    return 0;
}

static int64_t ABI detour_DcRailMode(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8610: (rail-mode mode) */
    (void)status;
    if (argc > 0) {
        *(uint32_t *)(g_u2_image_base + 0x0153FCB8ull) = (uint32_t)argv[0];
    }
    return 0;
}

static int64_t ABI detour_DcSetSwimDragMultiplier(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D650: (set-swim-drag-multiplier mult) */
    (void)status;
    if (argc == 1) {
        /* vmovss [rip+12650275] @ 0x0090D65D (8 bytes) -> 0x0090D665 + 0xC10823 = 0x0151DE88 */
        *(uint32_t *)(g_u2_image_base + 0x0151DE88ull) = (uint32_t)argv[0];
    }
    return 0;
}

static int64_t ABI detour_DcStateCharacterAccumulatesSnowFromGround(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01FE0: (state-character-accumulates-snow-from-ground flag) */
    (void)status;
    if (argc <= 0) return 0;
    *(uint8_t *)(g_u2_image_base + 0x01117570ull + 0x1C8ull) = (argv[0] != 0) ? 1u : 0u;
    return 1;
}

static int64_t ABI detour_DcStateCharacterAccumulatesSnowFromSky(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01F70: (state-character-accumulates-snow-from-sky flag rate) */
    (void)status;
    if (argc < 2) return 0;
    uint8_t *base = g_u2_image_base + 0x01117570ull;
    uint32_t rate = (uint32_t)argv[1];
    *(uint8_t *)(base + 0x1C9) = (argv[0] != 0) ? 1u : 0u;
    *(uint32_t *)(base + 0x64)  = rate;
    *(uint32_t *)(base + 0xA4)  = rate;
    *(uint32_t *)(base + 0xE4)  = rate;
    *(uint32_t *)(base + 0x124) = rate;
    *(uint32_t *)(base + 0x164) = rate;
    *(uint32_t *)(base + 0x1A4) = rate;
    return 1;
}

static int64_t ABI detour_DcStateCharacterAccumulatesWetnessFromWater(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01F30: (state-character-accumulates-wetness-from-water flag) */
    (void)status;
    if (argc <= 0) return 0;
    *(uint8_t *)(g_u2_image_base + 0x01117570ull + 0x1CAull) = (argv[0] != 0) ? 1u : 0u;
    return 1;
}

static int64_t ABI detour_DcStopScriptRumbles(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A105B0: (stop-script-rumbles) */
    (void)argc; (void)argv; (void)status;
    uint8_t *base = g_u2_image_base + 0x0152FBD0ull;
    for (size_t off = 0x35; off != 0x4B5; off += 0x48) {
        if (base[off - 1] != 0 && base[off] != 0) {
            base[off - 1] = 0;
        }
    }
    return 0;
}

static int64_t ABI detour_DcStringFromStringId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDE30: (string-from-string-id sid) */
    (void)status;
    if (argc > 0) {
        typedef const char *(ABI *fn_t)(uint32_t, uint64_t);
        fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        return (int64_t)(uintptr_t)fn((uint32_t)argv[0], 0);
    }
    return (int64_t)(uintptr_t)(g_u2_image_base + 0x00DF4374ull);
}

static int64_t ABI detour_DcStringIdFromString(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDE00: (string-id-from-string str) */
    (void)status;
    if (argc > 0) {
        typedef uint32_t (ABI *fn_t)(uint64_t, uint64_t, uint32_t);
        fn_t fn = *(fn_t *)(g_u2_image_base + 0x010666C8ull);
        return (int64_t)fn(0, argv[0], 1u);
    }
    return 0;
}

static int64_t ABI detour_DcSwimDisable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D760: (swim-disable) */
    typedef void (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00924040ull);
    fn(argc, argv, status);
    return 0;
}

static int64_t ABI detour_DcSwimDisallowStandOnBottom(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D680: (swim-disallow-stand-on-bottom) */
    (void)argc; (void)argv; (void)status;
    typedef void (ABI *fn_t)(uint64_t, void *, uint64_t);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00A1D2D0ull);
    uint64_t ctx = *(const uint64_t *)(g_u2_image_base + 0x01161EE8ull);
    fn(0, g_u2_image_base + 0x0104F810ull + 0x5Eull, ctx);
    return 0;
}

static int64_t ABI detour_DcSwimEnable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D770: (swim-enable) */
    typedef void (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00924140ull);
    fn(argc, argv, status);
    return 0;
}

static int64_t ABI detour_DcSyncVoxMetaData(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD1F0: (sync-vox-meta-data) */
    (void)argc; (void)argv; (void)status;
    typedef void (ABI *fn_t)(uint64_t);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x003E2AC0ull);
    fn(0);
    return 0;
}

static int64_t ABI detour_DcSyncVoxMpMetaData(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD1E0: (sync-vox-mp-meta-data) */
    typedef void (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x003E3320ull);
    fn(argc, argv, status);
    return 0;
}

static int64_t ABI detour_DcTaskSubnodeActiveP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FEC80: (task-subnode-active? subnode) */
    (void)argc; (void)status;
    typedef uint8_t (ABI *fn_t)(uint64_t);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00A5D6E0ull);
    return (int64_t)fn(argv[0]);
}

static int64_t ABI detour_DcElapsedTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0ED60: (elapsed-time [clock-id]) */
    (void)status;
    if (!g_u2_image_base) return 0;
    int64_t idx = (argc > 0) ? (int64_t)(int32_t)argv[0] : 0;
    int32_t slot = 1;
    if (argc > 0 && idx >= 0 && idx <= 9) {
        slot = *(const int32_t *)(g_u2_image_base + 0x00DEFA10ull + idx * 4);
    }
    uint8_t *clock_ptr = g_u2_image_base + 0x01116180ull + ((size_t)slot << 6);
    float elapsed = *(const float *)(clock_ptr + 0x4C);
    return (int64_t)u2_f32_to_reg(elapsed);
}

static int64_t ABI detour_DcCurrentTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0EDB0: (current-time [clock-id]) */
    (void)status;
    if (!g_u2_image_base) return 0;
    int64_t idx = (argc > 0) ? (int64_t)(int32_t)argv[0] : 0;
    int32_t slot = 1;
    if (argc > 0 && idx >= 0 && idx <= 9) {
        slot = *(const int32_t *)(g_u2_image_base + 0x00DEFA10ull + idx * 4);
    }
    uint8_t *clock_ptr = g_u2_image_base + 0x01116180ull + ((size_t)slot << 6);
    int32_t ticks = *(const int32_t *)(clock_ptr + 0x44);
    float sec_scale = *(const float *)(g_u2_image_base + 0x00E67400ull);
    float cur_sec = (float)ticks * sec_scale;
    return (int64_t)u2_f32_to_reg(cur_sec);
}

static int64_t ABI detour_DcNoOp0(int32_t argc, uint64_t *argv, uint32_t *status) {
    (void)argc; (void)argv; (void)status;
    return 0;
}

static int64_t ABI detour_DcRandomFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDA20: (random-float) */
    (void)argc; (void)argv; (void)status;
    uint32_t *p_seed1 = (uint32_t *)(g_u2_image_base + 0x01542F38ull);
    uint32_t *p_seed0 = (uint32_t *)(g_u2_image_base + 0x01542F34ull);
    uint32_t *p_c     = (uint32_t *)(g_u2_image_base + 0x01542F2Cull);
    uint32_t *p_d     = (uint32_t *)(g_u2_image_base + 0x01542F28ull);
    uint64_t rcx = (uint64_t)(*p_seed0) * 0x7DD4FFC7ull + (uint64_t)(*p_seed1);
    uint64_t rdx = (uint64_t)(*p_c) * 0x5D4ull + rcx;
    uint64_t rsi = (uint64_t)(*p_d) * 0x6F0ull + rdx;
    *p_seed1 = *p_c;
    *p_seed0 = *p_d;
    *p_c = (uint32_t)(rsi >> 32);
    *p_d = (uint32_t)rsi;
    float f = (float)(uint32_t)rsi * (1.0f / 4294967296.0f);
    return (int64_t)u2_f32_to_reg(f);
}

static int64_t ABI detour_DcRandomInteger(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD930: (random-integer [min] max) */
    (void)status;
    if (argc <= 0 || !argv) return 0;
    int32_t max_val = (int32_t)argv[0];
    int32_t min_val = 0;
    if (argc >= 2) {
        min_val = (int32_t)argv[1];
    }
    if (max_val <= min_val) return min_val;
    uint32_t range = (uint32_t)(max_val - min_val);
    uint32_t rnd = (uint32_t)rand();
    return (int64_t)(min_val + (int32_t)(rnd % range));
}

static int64_t ABI detour_DcEntityAliveP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A127E0: (entity-alive? handle) */
    (void)status;
    if (argc <= 0 || !argv || !argv[0]) return 0;
    typedef uint8_t *(ABI *resolve_fn)(uint32_t, uint64_t);
    resolve_fn resolve = (resolve_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    uint8_t *ent = resolve((uint32_t)argv[0], 0);
    if (!ent) return 0;
    uint32_t flags = *(const uint32_t *)(ent + 0x2Cu);
    return (flags & 1u) ? 0 : 1;
}

static int64_t ABI detour_DcCameraDisableAll(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E330: (camera-disable-all) */
    (void)argc; (void)argv; (void)status;
    uint8_t *head = *(uint8_t **)(g_u2_image_base + 0x0147F050ull);
    uint8_t *sentinel = g_u2_image_base + 0x0147F050ull;
    while (head && head != sentinel) {
        head[52] |= 1; /* disable flag */
        head = *(uint8_t **)head;
    }
    return 0;
}

static int64_t ABI detour_DcUnitsPerFrame(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0ECF0: (units-per-frame units [clock-id]) */
    if (status) *status = 0;
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    float units = u2_reg_to_f32(argv[0]);
    int32_t clock_id = (argc >= 2) ? (int32_t)argv[1] : 1;
    uint32_t clock_index = 1;
    if (clock_id >= 0 && clock_id <= 9) {
        clock_index = *(const uint32_t *)(g_u2_image_base + 0x00DEFA10ull + (uint64_t)clock_id * 4u);
    }
    const uint8_t *clock_ptr = g_u2_image_base + 0x01116180ull + (uint64_t)clock_index * 64u;
    float dt = *(const float *)(clock_ptr + 0x4C);
    return (int64_t)u2_f32_to_reg(units * dt);
}

static int64_t ABI detour_DcUnitsPerSecond(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0EC70: (units-per-second units [clock-id]) */
    if (status) *status = 0;
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    float units = u2_reg_to_f32(argv[0]);
    int32_t clock_id = (argc >= 2) ? (int32_t)argv[1] : 1;
    uint32_t clock_index = 1;
    if (clock_id >= 0 && clock_id <= 9) {
        clock_index = *(const uint32_t *)(g_u2_image_base + 0x00DEFA10ull + (uint64_t)clock_id * 4u);
    }
    const uint8_t *clock_ptr = g_u2_image_base + 0x01116180ull + (uint64_t)clock_index * 64u;
    float dt = *(const float *)(clock_ptr + 0x4C);
    float epsilon = *(const float *)(g_u2_image_base + 0x00E673FCull);
    if (dt > epsilon && dt > 0.0f) {
        return (int64_t)u2_f32_to_reg(units / dt);
    }
    return 0;
}

static int64_t ABI detour_DcIsJapanP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD8B0: (is-japan?) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *settings = g_u2_image_base + 0x01059640ull;
    if (*(const int32_t *)(settings + 0x3994) == 0) return 1;
    const uint8_t *lang_mgr = g_u2_image_base + 0x01450D18ull;
    uint32_t slot0 = *(const uint32_t *)(lang_mgr + 0x604);
    if (*(const int32_t *)(lang_mgr + (uint64_t)slot0 * 0x38ull + 8) == 8) return 1;
    uint64_t slot12 = *(const uint64_t *)(lang_mgr + 0x608);
    uint32_t s1 = (uint32_t)slot12;
    if (*(const int32_t *)(lang_mgr + (uint64_t)s1 * 0x38ull + 8) == 8) return 1;
    uint32_t s2 = (uint32_t)(slot12 >> 32);
    if (*(const int32_t *)(lang_mgr + (uint64_t)s2 * 0x38ull + 8) == 8) return 1;
    return 0;
}

static int64_t ABI detour_DcCameraStoreLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E9C0: (camera-store-locator) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *tracker_slot = *(const uint8_t **)(g_u2_image_base + 0x01114208ull);
    const uint32_t tracker_tag = *(const uint32_t *)(g_u2_image_base + 0x01114208ull + 8);
    float pos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    float rot[4] = { 2.0f, 2.0f, 2.0f, 2.0f }; /* default from 0x00E509E0 */
    if (tracker_slot && tracker_tag != 0) {
        const uint8_t *tracker = *(const uint8_t **)tracker_slot;
        if (tracker && *(const uint32_t *)(tracker + 0x44) == tracker_tag) {
            memcpy(pos, tracker + 0x370, sizeof(pos));
            memcpy(rot, tracker + 0x380, sizeof(rot));
        }
    }
    memcpy(g_u2_image_base + 0x0147F030ull, pos, sizeof(pos));
    memcpy(g_u2_image_base + 0x0147F040ull, rot, sizeof(rot));
    return 0;
}

static int64_t ABI detour_DcDebugBreak(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDF30: (debug-break) - safe no-op replacing int $0x41 crash */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcPrintNoOp(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* Shipping build 16-byte no-op prints (0x009FDEE0..0x009FDF20) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcForceDockJetski(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01B90: (force-dock-jetski) - shipping stub returning 0 */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcForceBoardJetski(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01BA0: (force-board-jetski) - shipping stub returning 0 */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcEventSender(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C1DF0: (event-sender) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    const uint8_t *frame = *(const uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    const uint8_t *sender_slot = *(const uint8_t **)(frame + 0x10);
    const uint32_t sender_tag = *(const uint32_t *)(frame + 0x18);
    if (!sender_slot || sender_tag == 0) return 0;
    const uint8_t *sender = *(const uint8_t **)sender_slot;
    if (!sender || *(const uint32_t *)(sender + 0x44) != sender_tag) return 0;
    return (int64_t)*(const uint32_t *)(sender + 0x68);
}

static int64_t ABI detour_DcEventSenderP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C1ED0: (event-sender? [sender-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    const uint8_t *frame = *(const uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    uint32_t target_sender_id = 0;
    if (argc > 0 && argv) {
        target_sender_id = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) target_sender_id = get_self(0, 0, 1);
    }
    if (target_sender_id == 0) return 0;
    const uint8_t *sender_slot = *(const uint8_t **)(frame + 0x10);
    const uint32_t sender_tag = *(const uint32_t *)(frame + 0x18);
    if (!sender_slot || sender_tag == 0) return 0;
    const uint8_t *sender = *(const uint8_t **)sender_slot;
    if (!sender || *(const uint32_t *)(sender + 0x44) != sender_tag) return 0;
    return (*(const uint32_t *)(sender + 0x68) == target_sender_id) ? 1 : 0;
}

static int64_t ABI detour_DcEventTrainP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C31A0: (event-train? [train-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    const uint8_t *frame = *(const uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    const uint8_t *sender_slot = *(const uint8_t **)(frame + 0x10);
    const uint32_t sender_tag = *(const uint32_t *)(frame + 0x18);
    if (!sender_slot || sender_tag == 0) return 0;
    const uint8_t *sender = *(const uint8_t **)sender_slot;
    if (!sender || *(const uint32_t *)(sender + 0x44) != sender_tag) return 0;
    if (*(const uint32_t *)(sender + 0x30) != 0xDFDDF753u) return 0;
    uint32_t target_train_id = 0;
    if (argc > 0 && argv) {
        target_train_id = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *self_fn)(uint32_t, const char *, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) target_train_id = get_self(0, (const char *)(g_u2_image_base + 0x00DF4374ull), 1);
    }
    return (*(const uint32_t *)(sender + 0x240) == target_train_id) ? 1 : 0;
}

static int64_t ABI detour_DcEventTrainCarP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C3020: (event-train-car? [car-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    const uint8_t *frame = *(const uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    const uint8_t *sender_slot = *(const uint8_t **)(frame + 0x10);
    const uint32_t sender_tag = *(const uint32_t *)(frame + 0x18);
    if (!sender_slot || sender_tag == 0) return 0;
    const uint8_t *sender = *(const uint8_t **)sender_slot;
    if (!sender || *(const uint32_t *)(sender + 0x44) != sender_tag) return 0;
    if (*(const uint32_t *)(sender + 0x30) != 0xDFDDF753u) return 0;
    uint32_t target_car_id = 0;
    if (argc > 0 && argv) {
        target_car_id = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *self_fn)(uint32_t, const char *, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) target_car_id = get_self(0, (const char *)(g_u2_image_base + 0x00DF4374ull), 1);
    }
    return (*(const uint32_t *)(sender + 0x244) == target_car_id) ? 1 : 0;
}

static int64_t ABI detour_DcCameraMovedBackwardsP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0DC40: (camera-moved-backwards? [threshold]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float threshold = (argc > 0 && argv) ? u2_reg_to_f32(argv[0]) : *(const float *)(g_u2_image_base + 0x00E673B4ull);
    const uint8_t *cam_ptr = *(const uint8_t **)(g_u2_image_base + 0x0147F020ull);
    if (!cam_ptr) return 0;
    const uint8_t *cam = *(const uint8_t **)cam_ptr;
    if (!cam) return 0;
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0147F028ull);
    if (*(const uint32_t *)(cam + 0x44) != expected_tag) return 0;
    if (*(const uint32_t *)(cam + 0x7C) != 0x58D01CCEu) return 0;
    float back_dist = *(const float *)(cam + 0x0A14);
    return (back_dist > threshold) ? 1 : 0;
}

static int64_t ABI detour_DcWeaponSetReset(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090AD00: (weapon-set-reset) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void * (ABI *EvalSymFn)(uint32_t);
    EvalSymFn eval_sym = (EvalSymFn)(uintptr_t)(g_u2_image_base + 0x00A15E70ull);
    if (eval_sym) {
        const void *src = eval_sym(0x459D0E4Eu);
        if (src) {
            memcpy((void *)(g_u2_image_base + 0x0153E1A0ull), src, 15 * sizeof(uint64_t));
        }
    }
    return 0;
}

static int64_t ABI detour_DcInvalidStringId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDE50: (invalid-string-id) -> 0 */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcHideHint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0CC10: (hide-hint) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    *(uint8_t *)(g_u2_image_base + 0x0104F1B8ull) = 0;
    return 0;
}

static int64_t ABI detour_DcGetSelfId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04A00: (get-self-id) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void * (ABI *GetSelfFn)(uint32_t);
    GetSelfFn get_self = (GetSelfFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    const uint8_t *obj = get_self ? (const uint8_t *)get_self(0xFFB75566u) : NULL;
    if (!obj) return 0;
    return (int64_t)*(const uint32_t *)(obj + 0x68);
}

static int64_t ABI detour_DcClearHarvestedBodies(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01A90: (clear-harvested-bodies) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void (ABI *ClearFn)(void *);
    ClearFn fn = (ClearFn)(uintptr_t)(g_u2_image_base + 0x00212680ull);
    void *mgr = (void *)(g_u2_image_base + 0x0110DCA0ull);
    if (fn) fn(mgr);
    return 0;
}

static int64_t ABI detour_DcResetActors(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD690: (reset-actors) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void (ABI *VoidFn)(void);
    VoidFn f1 = (VoidFn)(uintptr_t)(g_u2_image_base + 0x009691D0ull);
    VoidFn f2 = (VoidFn)(uintptr_t)(g_u2_image_base + 0x003F4CA0ull);
    if (f1) f1();
    if (f2) f2();
    return 0;
}

/* Table of detours provided by this module */
static const U2NativeDetour s_scene_detours[] = {
    { "DcEntityName", 0x00A127D0, (uintptr_t)detour_DcEntityName },
    { "DcSymbolEqualP", 0x00397620, (uintptr_t)detour_DcSymbolEqualP },
    { "DcCameraCancelAimOverride", 0x00A0DE30, (uintptr_t)detour_DcCameraCancelAimOverride },
    { "DcCameraRestoreLocator", 0x00A0E9B0, (uintptr_t)detour_DcCameraRestoreLocator },
    { "DcDebugPause", 0x00A102A0, (uintptr_t)detour_DcDebugPause },
    { "DcDiscP", 0x009FD910, (uintptr_t)detour_DcDiscP },
    { "DcResetAimFov", 0x0090D780, (uintptr_t)detour_DcResetAimFov },
    { "DcOverrideAimFov", 0x0090D7A0, (uintptr_t)detour_DcOverrideAimFov },
    { "DcGetHarvestingPriority", 0x00A01AB0, (uintptr_t)detour_DcGetHarvestingPriority },
    { "DcSetHarvestingPriority", 0x00A01AC0, (uintptr_t)detour_DcSetHarvestingPriority },
    { "DcResetHarvestingPriority", 0x00A01AE0, (uintptr_t)detour_DcResetHarvestingPriority },
    { "DcHarvestingEnabledP", 0x00A01AF0, (uintptr_t)detour_DcHarvestingEnabledP },
    { "DcSetHarvestingEnabled", 0x00A01B00, (uintptr_t)detour_DcSetHarvestingEnabled },
    { "DcHideHud", 0x00A0CD00, (uintptr_t)detour_DcHideHud },
    { "DcIsFma", 0x00A0FE80, (uintptr_t)detour_DcIsFma },
    { "DcJournalClearPage", 0x009FD090, (uintptr_t)detour_DcJournalClearPage },
    { "DcJournalGetPage", 0x009FD110, (uintptr_t)detour_DcJournalGetPage },
    { "DcKillAllParticles", 0x00A0FE60, (uintptr_t)detour_DcKillAllParticles },
    { "DcGetCaptureZone", 0x006687D0, (uintptr_t)detour_DcGetCaptureZone },
    { "DcNetPlyaerCoopId", 0x00669240, (uintptr_t)detour_DcNetPlyaerCoopId },
    { "DcNetCoopTeamFailedP", 0x00669C20, (uintptr_t)detour_DcNetCoopTeamFailedP },
    { "DcNetCoopScriptId", 0x00669C30, (uintptr_t)detour_DcNetCoopScriptId },
    { "DcNetCoopHostScriptId", 0x00669C40, (uintptr_t)detour_DcNetCoopHostScriptId },
    { "DcNetIsCinema", 0x00669C50, (uintptr_t)detour_DcNetIsCinema },
    { "DcPsnDemoShowUpsell", 0x009FD590, (uintptr_t)detour_DcPsnDemoShowUpsell },
    { "DcAutoSaveGame", 0x009FE820, (uintptr_t)detour_DcAutoSaveGame },
    { "DcGameTaskGraphLogin", 0x009FE8F0, (uintptr_t)detour_DcGameTaskGraphLogin },
    { "DcRailSetIgcFadeOutTime", 0x00AB93F0, (uintptr_t)detour_DcRailSetIgcFadeOutTime },
    { "DcTrainGetDefaultAcceleration", 0x00A682E0, (uintptr_t)detour_DcTrainGetDefaultAcceleration },
    { "DcTutorialHintsOnP", 0x00A0F490, (uintptr_t)detour_DcTutorialHintsOnP },
    { "DcUseWaterUnitForReflectionIfVisible", 0x00A02C50, (uintptr_t)detour_DcUseWaterUnitForReflectionIfVisible },
    { "DcAngleMod", 0x00397630, (uintptr_t)detour_DcAngleMod },
    { "DcAnimate", 0x00397C50, (uintptr_t)detour_DcAnimate },
    { "DcActionPackEnabledP", 0x00A09FF0, (uintptr_t)detour_DcActionPackEnabledP },
    { "DcHudSetCounter", 0x00A10470, (uintptr_t)detour_DcHudSetCounter },
    { "DcHudSetTimer", 0x00A104B0, (uintptr_t)detour_DcHudSetTimer },
    { "DcIncrementStat", 0x0097BDF0, (uintptr_t)detour_DcIncrementStat },
    { "DcIncrementTaskStat", 0x0097BE20, (uintptr_t)detour_DcIncrementTaskStat },
    { "DcIsDifficultyVeryEasy", 0x009FCFC0, (uintptr_t)detour_DcIsDifficultyVeryEasy },
    { "DcGetDifficulty", 0x009FD040, (uintptr_t)detour_DcGetDifficulty },
    { "DcJournalOpenP", 0x009FD0B0, (uintptr_t)detour_DcJournalOpenP },
    { "DcJournalSetAmbientScale", 0x009FD060, (uintptr_t)detour_DcJournalSetAmbientScale },
    { "DcKillMessage", 0x005DC630, (uintptr_t)detour_DcKillMessage },
    { "DcPowFloat", 0x009FDB60, (uintptr_t)detour_DcPowFloat },
    { "DcLerpScale", 0x009FDB90, (uintptr_t)detour_DcLerpScale },
    { "DcRailActivate", 0x00AB98D0, (uintptr_t)detour_DcRailActivate },
    { "DcRailDeactivateMover", 0x00AB8290, (uintptr_t)detour_DcRailDeactivateMover },
    { "DcRailKillAllPhysicsObjects", 0x00AB8F60, (uintptr_t)detour_DcRailKillAllPhysicsObjects },
    { "DcRailMode", 0x00AB8610, (uintptr_t)detour_DcRailMode },
    { "DcSetSwimDragMultiplier", 0x0090D650, (uintptr_t)detour_DcSetSwimDragMultiplier },
    { "DcStateCharacterAccumulatesSnowFromGround", 0x00A01FE0, (uintptr_t)detour_DcStateCharacterAccumulatesSnowFromGround },
    { "DcStateCharacterAccumulatesSnowFromSky", 0x00A01F70, (uintptr_t)detour_DcStateCharacterAccumulatesSnowFromSky },
    { "DcStateCharacterAccumulatesWetnessFromWater", 0x00A01F30, (uintptr_t)detour_DcStateCharacterAccumulatesWetnessFromWater },
    { "DcStopScriptRumbles", 0x00A105B0, (uintptr_t)detour_DcStopScriptRumbles },
    { "DcStringFromStringId", 0x009FDE30, (uintptr_t)detour_DcStringFromStringId },
    { "DcStringIdFromString", 0x009FDE00, (uintptr_t)detour_DcStringIdFromString },
    { "DcSwimDisable", 0x0090D760, (uintptr_t)detour_DcSwimDisable },
    { "DcSwimDisallowStandOnBottom", 0x0090D680, (uintptr_t)detour_DcSwimDisallowStandOnBottom },
    { "DcSwimEnable", 0x0090D770, (uintptr_t)detour_DcSwimEnable },
    { "DcSyncVoxMetaData", 0x009FD1F0, (uintptr_t)detour_DcSyncVoxMetaData },
    { "DcSyncVoxMpMetaData", 0x009FD1E0, (uintptr_t)detour_DcSyncVoxMpMetaData },
    { "DcTaskSubnodeActiveP", 0x009FEC80, (uintptr_t)detour_DcTaskSubnodeActiveP },
    { "DcElapsedTime", 0x00A0ED60, (uintptr_t)detour_DcElapsedTime },
    { "DcCurrentTime", 0x00A0EDB0, (uintptr_t)detour_DcCurrentTime },
    { "DcPrintStringWs", 0x009FDEA0, (uintptr_t)detour_DcNoOp0 },
    { "DcRandomFloat", 0x009FDA20, (uintptr_t)detour_DcRandomFloat },
    { "DcRandomInteger", 0x009FD930, (uintptr_t)detour_DcRandomInteger },
    { "DcEntityAliveP", 0x00A127E0, (uintptr_t)detour_DcEntityAliveP },
    { "DcCameraDisableAll", 0x00A0E330, (uintptr_t)detour_DcCameraDisableAll },
    { "DcUnitsPerFrame", 0x00A0ECF0, (uintptr_t)detour_DcUnitsPerFrame },
    { "DcUnitsPerSecond", 0x00A0EC70, (uintptr_t)detour_DcUnitsPerSecond },
    { "DcIsJapanP", 0x009FD8B0, (uintptr_t)detour_DcIsJapanP },
    { "DcCameraStoreLocator", 0x00A0E9C0, (uintptr_t)detour_DcCameraStoreLocator },
    { "DcDebugBreak", 0x009FDF30, (uintptr_t)detour_DcDebugBreak },
    { "DcPrintString", 0x009FDEE0, (uintptr_t)detour_DcPrintNoOp },
    { "DcPrintInteger", 0x009FDEF0, (uintptr_t)detour_DcPrintNoOp },
    { "DcPrintFloat", 0x009FDF00, (uintptr_t)detour_DcPrintNoOp },
    { "DcPrintVector", 0x009FDF10, (uintptr_t)detour_DcPrintNoOp },
    { "DcPrintVector3", 0x009FDF20, (uintptr_t)detour_DcPrintNoOp },
    { "DcForceDockJetski", 0x00A01B90, (uintptr_t)detour_DcForceDockJetski },
    { "DcForceBoardJetski", 0x00A01BA0, (uintptr_t)detour_DcForceBoardJetski },
    { "DcEventSender", 0x004C1DF0, (uintptr_t)detour_DcEventSender },
    { "DcEventSenderP", 0x004C1ED0, (uintptr_t)detour_DcEventSenderP },
    { "DcEventTrainP", 0x004C31A0, (uintptr_t)detour_DcEventTrainP },
    { "DcEventTrainCarP", 0x004C3020, (uintptr_t)detour_DcEventTrainCarP },
    { "DcCameraMovedBackwardsP", 0x00A0DC40, (uintptr_t)detour_DcCameraMovedBackwardsP },
    { "DcWeaponSetReset", 0x0090AD00, (uintptr_t)detour_DcWeaponSetReset },
    { "DcInvalidStringId", 0x009FDE50, (uintptr_t)detour_DcInvalidStringId },
    { "DcPrintPoint", 0x00A10290, (uintptr_t)detour_DcPrintNoOp },
    { "DcPrintSymbol", 0x00A10280, (uintptr_t)detour_DcPrintNoOp },
    { "DcOceanBobEnable", 0x00A033A0, (uintptr_t)detour_DcNoOp0 },
    { "DcOceanBobDisable", 0x00A03390, (uintptr_t)detour_DcNoOp0 },
    { "DcDisplay", 0x009FD000, (uintptr_t)detour_DcNoOp0 },
    { "DcDisplayInt", 0x009FCFF0, (uintptr_t)detour_DcNoOp0 },
    { "DcMessage", 0x005DC620, (uintptr_t)detour_DcNoOp0 },
    { "DcAnimStateAdvancePhase", 0x00397690, (uintptr_t)detour_DcNoOp0 },
    { "DcHideHint", 0x00A0CC10, (uintptr_t)detour_DcHideHint },
    { "DcGetSelfId", 0x00A04A00, (uintptr_t)detour_DcGetSelfId },
    { "DcClearHarvestedBodies", 0x00A01A90, (uintptr_t)detour_DcClearHarvestedBodies },
    { "DcResetActors", 0x009FD690, (uintptr_t)detour_DcResetActors },
    { "DcSetBarrelFlowMultiplier", 0x00A01EF0, (uintptr_t)detour_DcNoOp0 },
    { "DcSetJetskiFlowMultiplier", 0x00A01B70, (uintptr_t)detour_DcNoOp0 },
    { "DcSetJetskiMaxThrottle", 0x00A01B60, (uintptr_t)detour_DcNoOp0 },
    { "DcSetJetskiNearStupidRampInRuins2", 0x00A01B50, (uintptr_t)detour_DcNoOp0 },
    { "DcJetskiClearEngineSoundOverride", 0x00A01B20, (uintptr_t)detour_DcNoOp0 },
    { "DcSetE3LoadingScreen", 0x009FD5B0, (uintptr_t)detour_DcNoOp0 },
    { "DcEntitySetLevelB", 0x00A64C30, (uintptr_t)detour_DcNoOp0 },
    { "DcFmaResetPartModules", 0x00A00130, (uintptr_t)detour_DcNoOp0 },
};

size_t u2_get_scene_detours(const U2NativeDetour **out) {
    if (out) *out = s_scene_detours;
    return sizeof(s_scene_detours) / sizeof(s_scene_detours[0]);
}
