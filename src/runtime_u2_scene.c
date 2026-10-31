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

static int64_t ABI detour_DcNetCoopTeamFailedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C20: (net-coop-team-failed?) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x0113D910ull);
}

static int64_t ABI detour_DcPsnDemoShowUpsell(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD590: (psn-demo-show-upsell) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x0113769Dull) = 1u;
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

static int64_t ABI detour_DcSwimDisallowStandOnBottom(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D680: (swim-disallow-stand-on-bottom) */
    (void)argc; (void)argv; (void)status;
    typedef void (ABI *fn_t)(uint64_t, void *, uint64_t);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00A1D2D0ull);
    uint64_t ctx = *(const uint64_t *)(g_u2_image_base + 0x01161EE8ull);
    fn(0, g_u2_image_base + 0x0104F810ull + 0x5Eull, ctx);
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

static int64_t ABI detour_DcCameraEnable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E680: (camera-enable cam-sym [target] [fade_time] [priority] ...) */
    if (status) *status = 0;
    if (argc < 1 || !g_u2_image_base) return 0;
    uint32_t cam_id = (uint32_t)argv[0];

    uint8_t *head = (uint8_t *)(g_u2_image_base + 0x0147F050ull);
    uint8_t *cur = *(uint8_t **)head;
    while (cur && cur != head) {
        if (*(const uint32_t *)(cur + 0x14) == cam_id) {
            cur[0x34] &= (uint8_t)~0x01; /* clear abandon */
            cur[0x34] |= 0x04;          /* set active / enabled */
            break;
        }
        cur = *(uint8_t **)cur;
    }
    return 0;
}

static int64_t ABI detour_DcCameraAbandon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E5E0: (camera-abandon [cam-symbol]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t cam_id = 0;
    if (argc > 0) {
        cam_id = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *GetSelfFn)(int, int, int);
        GetSelfFn get_self = *(GetSelfFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) cam_id = get_self(0, 0, 1);
    }
    if (!cam_id) return 0;

    uint8_t *head = (uint8_t *)(g_u2_image_base + 0x0147F050ull);
    uint8_t *cur = *(uint8_t **)head;
    while (cur && cur != head) {
        if (*(const uint32_t *)(cur + 0x14) == cam_id) {
            cur[0x34] |= 0x01; /* abandon flag */
            break;
        }
        cur = *(uint8_t **)cur;
    }
    return 0;
}

static int64_t ABI detour_DcCameraFadeOut(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E380: (camera-fade-out [cam-symbol] [duration]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t cam_id = 0;
    float duration = 1.0f;
    if (argc > 0) {
        cam_id = (uint32_t)argv[0];
        if (argc > 1) duration = u2_reg_to_f32(argv[1]);
    } else {
        typedef uint32_t (ABI *GetSelfFn)(int, int, int);
        GetSelfFn get_self = *(GetSelfFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) cam_id = get_self(0, 0, 1);
        duration = *(const float *)(g_u2_image_base + 0x00E673E0ull);
    }
    if (!cam_id) return 0;

    uint8_t *head = (uint8_t *)(g_u2_image_base + 0x0147F050ull);
    uint8_t *cur = *(uint8_t **)head;
    while (cur && cur != head) {
        if (*(const uint32_t *)(cur + 0x14) == cam_id) {
            *(float *)(cur + 0x38) = duration;
            cur[0x34] |= 0x02; /* fade-out flag */
            break;
        }
        cur = *(uint8_t **)cur;
    }
    return 0;
}

static int64_t ABI detour_DcCameraLocalSpace(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0EA20: (camera-local-space) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    int32_t val = 0x4CBEBC20;
    void *str_arg = (void *)(uintptr_t)(g_u2_image_base + 0x0104F810ull + 0x40);
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1D290ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1D2A0ull);
    typedef void * (ABI *PropHelperFn)(uint32_t, void *, void *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    void *obj = prop_fn ? prop_fn(0xBD729FA7u, str_arg, &val, 4, t1, t2, NULL) : NULL;
    if (obj) {
        uint8_t *b = (uint8_t *)obj;
        b[0x61] = (b[0x61] & (uint8_t)~0x27) | 0x04;
        *(uint32_t *)(b + 0x20) = 0x65;
        uint32_t ticks = *(const uint32_t *)(g_u2_image_base + 0x01116180ull + 0x84);
        *(uint32_t *)(b + 0x10) = ticks + 9u;
    }
    return 0;
}

static int64_t ABI detour_DcDisableCoverCamera(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E290: (disable-cover-camera) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    uint8_t flag = 0;
    void *str_arg = (void *)(uintptr_t)(g_u2_image_base + 0x0104F693ull);
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD30ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD40ull);
    typedef void * (ABI *PropHelperFn)(uint32_t, void *, uint8_t *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    void *obj = prop_fn ? prop_fn(0x17DDF893u, str_arg, &flag, 1, t1, t2, NULL) : NULL;
    if (obj) {
        uint8_t *b = (uint8_t *)obj;
        b[0x61] = (b[0x61] & (uint8_t)~0x27) | 0x04;
        *(uint32_t *)(b + 0x20) = 0x65;
        uint32_t ticks = *(const uint32_t *)(g_u2_image_base + 0x01116180ull + 0x84);
        *(uint32_t *)(b + 0x10) = ticks + 9u;
    }
    return 0;
}

static int64_t ABI detour_DcAllowCameraCutCompensate(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0DD40: (allow-camera-cut-compensate) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    uint8_t flag = 0;
    void *str_arg = (void *)(uintptr_t)(g_u2_image_base + 0x01157738ull);
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD30ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1D2C0ull);
    typedef void * (ABI *PropHelperFn)(uint32_t, void *, uint8_t *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    void *obj = prop_fn ? prop_fn(0x309861E2u, str_arg, &flag, 1, t1, t2, NULL) : NULL;
    if (obj) {
        uint8_t *b = (uint8_t *)obj;
        b[0x61] = (b[0x61] & (uint8_t)~0x27) | 0x04;
        *(uint32_t *)(b + 0x20) = 0x65;
        uint32_t ticks = *(const uint32_t *)(g_u2_image_base + 0x01116180ull + 0x84);
        *(uint32_t *)(b + 0x10) = ticks + 9u;
    }
    return 0;
}

static int64_t ABI detour_DcCameraAimOverridePoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0DF40: (camera-aim-override-point point) */
    if (status) *status = 0;
    if (argc < 1 || !argv || !argv[0] || !g_u2_image_base) return 0;
    const float *pt = (const float *)(uintptr_t)argv[0];
    *(uint32_t *)(g_u2_image_base + 0x0147F0BCull) = 1u;
    float *dest = (float *)(g_u2_image_base + 0x0147F080ull);
    dest[0] = pt[0];
    dest[1] = pt[1];
    dest[2] = pt[2];
    dest[3] = 1.0f;
    return 1;
}

static int64_t ABI detour_DcCameraAimOverrideDirection(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0DE40: (camera-aim-override-direction pitch yaw) */
    if (status) *status = 0;
    if (argc < 2 || !g_u2_image_base) return 0;
    float pitch = u2_reg_to_f32(argv[0]);
    float yaw   = u2_reg_to_f32(argv[1]);
    *(uint32_t *)(g_u2_image_base + 0x0147F0BCull) = 2u;
    float *dest = (float *)(g_u2_image_base + 0x0147F090ull);
    dest[0] = pitch;
    dest[1] = yaw;
    return 1;
}

static int64_t ABI detour_DcCameraAnimationDisable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A033B0: (camera-animation-disable track-id [blend-time]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcCameraAnimationEnable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03480: (camera-animation-enable track-id [blend-time]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcCameraShakeInZoomCamera(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0DCA0: (camera-shake-in-zoom-camera) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    uint8_t flag = 1;
    void *str_arg = (void *)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x0104AD78ull) + 0x110);
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD30ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD40ull);
    typedef void * (ABI *PropHelperFn)(uint32_t, void *, uint8_t *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    void *obj = prop_fn ? prop_fn(0x92950534u, str_arg, &flag, 1, t1, t2, NULL) : NULL;
    if (obj) {
        uint8_t *b = (uint8_t *)obj;
        b[0x61] = (b[0x61] & (uint8_t)~0x27) | 0x04;
        *(uint32_t *)(b + 0x20) = 0x65;
        uint32_t ticks = *(const uint32_t *)(g_u2_image_base + 0x01116180ull + 0x84);
        *(uint32_t *)(b + 0x10) = ticks + 9u;
    }
    return 0;
}

static int64_t ABI detour_DcCameraShake(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E040: (camera-shake type intensity [duration]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcIsCameraStickPressed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0D610: (is-camera-stick-pressed) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float dx = *(const float *)(g_u2_image_base + 0x01116180ull + 0xA80);
    float dy = *(const float *)(g_u2_image_base + 0x01116180ull + 0xA84);
    float mag = sqrtf(dx * dx + dy * dy);
    float threshold = *(const float *)(g_u2_image_base + 0x00E67390ull);
    return (mag > threshold) ? 1 : 0;
}

static int64_t ABI detour_DcCameraLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A128B0: (camera-locator) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void * (ABI *AllocFn)(uint64_t, void *, uint64_t);
    AllocFn alloc_fn = (AllocFn)(uintptr_t)(g_u2_image_base + 0x00B332A0ull);
    uint32_t type_tag = 0x45;
    void *ptr = alloc_fn ? alloc_fn(0x60, &type_tag, 0x10) : NULL;
    if (ptr) {
        uint32_t *p32 = (uint32_t *)ptr;
        p32[0] = 0x7FCCCCCC;
        p32[1] = 0x7FCCCCCC;
        memset((uint8_t *)ptr + 0x20, 0, 16);
        memcpy((uint8_t *)ptr + 0x30, (const void *)(g_u2_image_base + 0x00E50B80ull), 16);
        *(uint64_t *)((uint8_t *)ptr + 0x08) = 0;
        *(uint32_t *)((uint8_t *)ptr + 0x10) = 0;
    }
    return (int64_t)(uintptr_t)ptr;
}

static int64_t ABI detour_DcSetDefaultFollowCameraSettings(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0EBF0: (set-default-follow-camera-settings [setting-ptr]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    void *arg_val = (argc > 0) ? (void *)(uintptr_t)argv[0] : NULL;
    void *str_arg = (void *)(uintptr_t)(g_u2_image_base + 0x0104F810ull + 0x50);
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1D270ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1D280ull);
    typedef void * (ABI *PropHelperFn)(uint32_t, void *, void *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    void *obj = prop_fn ? prop_fn(0xF46E3BABu, str_arg, arg_val, 4, t1, t2, NULL) : NULL;
    if (obj) {
        uint8_t *b = (uint8_t *)obj;
        b[0x61] = (b[0x61] & (uint8_t)~0x27) | 0x04;
        *(uint32_t *)(b + 0x20) = 0x65;
        uint32_t ticks = *(const uint32_t *)(g_u2_image_base + 0x01116180ull + 0x84);
        *(uint32_t *)(b + 0x10) = ticks + 0x27u;
    }
    return 0;
}

static int64_t ABI detour_DcForceNoShiftCameraZoomMode(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A035E0: (force-no-shift-camera-zoom-mode) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    uint8_t flag = 1;
    void *str_arg = (void *)(uintptr_t)(g_u2_image_base + 0x01114200ull);
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD30ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD40ull);
    typedef void * (ABI *PropHelperFn)(uint32_t, void *, uint8_t *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    if (prop_fn) prop_fn(0, str_arg, &flag, 1, t1, t2, NULL);
    return 0;
}

static int64_t ABI detour_DcForceAimingCameraUltraTight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03670: (force-aiming-camera-ultra-tight) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    uint8_t flag = 1;
    void *str_arg = (void *)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x0104AD78ull) + 0xE1);
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD30ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD40ull);
    typedef void * (ABI *PropHelperFn)(uint32_t, void *, uint8_t *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    if (prop_fn) prop_fn(0, str_arg, &flag, 1, t1, t2, NULL);
    return 0;
}

/* --- Train & Helicopter Setpiece Subsystem --- */

static inline uint8_t *u2_resolve_train(int32_t argc, uint64_t *argv) {
    if (!g_u2_image_base) return NULL;
    uint32_t handle = 0;
    if (argc > 0 && argv && argv[0]) {
        handle = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) {
            handle = get_self(0, 0, 1);
        }
    }
    uint8_t *ent = NULL;
    if (handle) {
        typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
        GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        ent = get_ent ? get_ent(0, handle) : NULL;
    }
    if (!ent) {
        uint8_t *mgr = g_u2_image_base + 0x01050320ull;
        int32_t count = *(const int32_t *)(mgr + 0x200);
        if (count > 0 && count <= 16) {
            ent = *(uint8_t **)(mgr + 8);
        }
    }
    if (ent && *(const uint32_t *)(ent + 0x30) == 0xDFDDF753u) {
        return ent;
    }
    return NULL;
}

static inline uint8_t *u2_resolve_heli(int32_t argc, uint64_t *argv) {
    if (!g_u2_image_base) return NULL;
    uint32_t handle = 0;
    if (argc > 0 && argv && argv[0]) {
        handle = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) {
            handle = get_self(0, (const void *)(g_u2_image_base + 0x00DF4374ull), 1);
        }
    }
    if (!handle) return NULL;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = get_ent ? get_ent(0, handle) : NULL;
    if (ent && *(const uint32_t *)(ent + 0x30) == 0x5C01F503u) {
        return ent;
    }
    return NULL;
}

static int64_t ABI detour_DcTrainGetSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A67AE0: (train-get-spline [train]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    int32_t spline_idx = *(const int32_t *)(train + 0x290);
    if (spline_idx < 0) return 0;
    uint8_t *table = *(uint8_t **)(g_u2_image_base + 0x0104EB30ull + 8);
    if (!table) return 0;
    uint8_t *entry = *(uint8_t **)(table + (size_t)spline_idx * 0x110 + 0x20);
    if (entry && *(uint8_t **)(train + 0x298) == entry) {
        return (int64_t)*(const int32_t *)entry;
    }
    return 0;
}

static int64_t ABI detour_DcTrainGetDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A67B90: (train-get-distance [train]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    typedef float (ABI *SplineDistFn)(void *, int32_t);
    SplineDistFn dist_fn = (SplineDistFn)(uintptr_t)(g_u2_image_base + 0x00A23EC0ull);
    float dist = dist_fn ? dist_fn(train + 0x3B0, 1) : *(const float *)(train + 0x3B0);
    return (int64_t)u2_f32_to_reg(dist);
}

static int64_t ABI detour_DcTrainGetHeadCar(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A67C40: (train-get-head-car [train]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    typedef uint8_t * (ABI *GetTrainMoverFn)(void *);
    GetTrainMoverFn mover_fn = (GetTrainMoverFn)(uintptr_t)(g_u2_image_base + 0x00A6CCF0ull);
    uint8_t *mover = mover_fn ? mover_fn(train) : NULL;
    if (mover) {
        return (int64_t)*(const uint32_t *)(mover + 0x68);
    }
    return 0;
}

static int64_t ABI detour_DcTrainTeleport(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A67CD0: (train-teleport [train] spline-id [dist]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    uint32_t target_spline = (argc >= 2) ? (uint32_t)argv[1] : (argc == 1 ? (uint32_t)argv[0] : 0);
    float dist = (argc >= 3) ? u2_reg_to_f32(argv[2]) : 0.0f;
    uint8_t *table = *(uint8_t **)(g_u2_image_base + 0x0104EB30ull + 8);
    if (!table) return 0;
    for (uint32_t i = 0; i < 0x180u; ++i) {
        uint8_t *entry = table + (size_t)i * 0x110 + 0x10;
        if (*(uint8_t **)(entry - 0x10) != NULL) {
            uint32_t *id_ptr = *(uint32_t **)(entry + 0x10);
            if (id_ptr && *id_ptr == target_spline) {
                uint8_t flags = *entry;
                if ((flags & 0x05) == 0) {
                    typedef void (ABI *TeleportFn)(void *, uint8_t *, float);
                    TeleportFn fn = (TeleportFn)(uintptr_t)(g_u2_image_base + 0x00A6CBC0ull);
                    if (fn) fn(train, entry, dist);
                    return 1;
                }
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcTrainDecouple(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A67E40: (train-decouple [train] [car-idx] [front-or-back] [flag]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    int32_t car_idx = (argc >= 2) ? (int32_t)argv[1] : 0;
    int32_t side = (argc >= 3) ? (int32_t)argv[2] : 1;
    int32_t flag = (argc >= 4) ? (int32_t)argv[3] : 0;
    typedef void (ABI *DecoupleFn)(void *, int32_t, int32_t, int32_t);
    DecoupleFn fn = (DecoupleFn)(uintptr_t)(g_u2_image_base + 0x00A6D3D0ull);
    if (fn) fn(train, car_idx, side, flag);
    return 0;
}

static int64_t ABI detour_DcTrainSetSpeedMph(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A67F60: (train-set-speed-mph [train] speed-mph [trans-time]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    float speed_mph = (argc >= 2) ? u2_reg_to_f32(argv[1]) : (argc == 1 ? u2_reg_to_f32(argv[0]) : 0.0f);
    float trans_time = (argc >= 3) ? u2_reg_to_f32(argv[2]) : 0.0f;
    float speed_ms = speed_mph * 0.44704f;
    typedef uint8_t * (ABI *GetTrainMoverFn)(void *);
    GetTrainMoverFn mover_fn = (GetTrainMoverFn)(uintptr_t)(g_u2_image_base + 0x00A6CCF0ull);
    uint8_t *mover = mover_fn ? mover_fn(train) : NULL;
    if (mover) {
        *(float *)(mover + 0x534) = speed_ms;
        if (trans_time <= 0.0f) {
            *(float *)(mover + 0x538) = speed_ms;
            *(float *)(mover + 0x53C) = 0.0f;
        } else {
            float cur_speed = *(const float *)(mover + 0x538);
            *(float *)(mover + 0x53C) = (speed_ms - cur_speed) / trans_time;
        }
    }
    return 0;
}

static int64_t ABI detour_DcTrainSetAcceleration(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A68140: (train-set-acceleration [train] accel) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    float accel = (argc >= 2) ? u2_reg_to_f32(argv[1]) : (argc == 1 ? u2_reg_to_f32(argv[0]) : 10.0f);
    typedef uint8_t * (ABI *GetTrainMoverFn)(void *);
    GetTrainMoverFn mover_fn = (GetTrainMoverFn)(uintptr_t)(g_u2_image_base + 0x00A6CCF0ull);
    uint8_t *mover = mover_fn ? mover_fn(train) : NULL;
    if (mover) {
        *(float *)(mover + 0x540) = accel;
    }
    *(float *)(train + 0x540) = accel;
    return 0;
}

static int64_t ABI detour_DcTrainGetAcceleration(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A68230: (train-get-acceleration [train]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    float accel = train ? *(const float *)(train + 0x540) : 10.0f;
    return (int64_t)u2_f32_to_reg(accel);
}

static int64_t ABI detour_DcTrainTrackPlayer(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A68300: (train-track-player [train] [enable-p]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    int32_t enable = (argc >= 2) ? (argv[1] != 0) : 1;
    typedef uint8_t * (ABI *GetTrainMoverFn)(void *);
    GetTrainMoverFn mover_fn = (GetTrainMoverFn)(uintptr_t)(g_u2_image_base + 0x00A6CCF0ull);
    uint8_t *mover = mover_fn ? mover_fn(train) : NULL;
    if (mover) {
        uint32_t flags = *(const uint32_t *)(mover + 0x558);
        if (enable) flags |= 0x2000u;
        else flags &= ~0x2000u;
        *(uint32_t *)(mover + 0x558) = flags;
        *(uint64_t *)(mover + 0x548) = *(const uint64_t *)(train + 0x38);
        *(uint32_t *)(mover + 0x550) = *(const uint32_t *)(train + 0x44);
    }
    return 0;
}

static int64_t ABI detour_DcTrainAllowUnphysicalize(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A68430: (train-allow-unphysicalize [train] [allow-p]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    int32_t allow = (argc >= 2) ? (argv[1] != 0) : 1;
    typedef uint8_t * (ABI *GetTrainMoverFn)(void *);
    GetTrainMoverFn mover_fn = (GetTrainMoverFn)(uintptr_t)(g_u2_image_base + 0x00A6CCF0ull);
    uint8_t *mover = mover_fn ? mover_fn(train) : NULL;
    while (mover) {
        uint32_t flags = *(const uint32_t *)(mover + 0x558);
        if (allow) flags |= 0x800000u;
        else flags &= ~0x800000u;
        *(uint32_t *)(mover + 0x558) = flags;
        uint8_t *chain = *(uint8_t **)(mover + 0x848);
        int32_t count = *(const int32_t *)(mover + 0x850);
        if (chain && count > 0 && *(uint8_t **)chain) {
            uint8_t *next_car = *(uint8_t **)chain;
            if (*(const int32_t *)(next_car + 0x44) == count) {
                mover = next_car;
                continue;
            }
        }
        break;
    }
    return 0;
}

static int64_t ABI detour_DcTrainKillAllRiders(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A68530: (train-kill-all-riders [train] [kill-type]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    int32_t kill_type = (argc >= 2) ? (int32_t)argv[1] : 0;
    typedef void (ABI *KillRidersFn)(void *, int32_t);
    KillRidersFn fn = (KillRidersFn)(uintptr_t)(g_u2_image_base + 0x00A70BE0ull);
    if (fn) fn(train, kill_type);
    return 0;
}

static int64_t ABI detour_DcTrainTransferPendingP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A685E0: (train-transfer-pending? [train]) */
    if (status) *status = 0;
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    typedef void (ABI *EventQueryFn)(uint8_t *, uint32_t, void *, void *, void *, void *);
    EventQueryFn fn = (EventQueryFn)(uintptr_t)(g_u2_image_base + 0x004C4090ull);
    uint8_t result[64] = {0};
    uint8_t dummy[128] = {0};
    if (fn) {
        fn(result, 0xC14382CEu, train, dummy, dummy + 32, dummy + 64);
        return (result[0] != 0) ? 1 : 0;
    }
    return 0;
}

static int64_t ABI detour_DcTrainSpawn(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A67960: (train-spawn type-symbol) */
    if (status) *status = 0;
    if (argc <= 0 || !argv || !argv[0] || !g_u2_image_base) return 0;
    typedef void * (ABI *SpawnFn)(uint64_t);
    SpawnFn spawn_fn = (SpawnFn)(uintptr_t)(g_u2_image_base + 0x00A69AC0ull);
    if (spawn_fn) spawn_fn(argv[0]);

    uint8_t *mgr = g_u2_image_base + 0x01050320ull;
    int32_t count = *(const int32_t *)(mgr + 0x200);
    /* Safe bounds clamping: prevents int $0x41 debug crash trap from original binary */
    if (count >= 0 && count < 16) {
        uint8_t *slot = mgr + 8 + (size_t)count * 32;
        void *r12 = *(void **)slot;
        void *rdi = *(void **)(slot + 8);
        if (r12 && rdi) {
            typedef void (ABI *AttachFn)(void *, void *);
            AttachFn attach_fn = (AttachFn)(uintptr_t)(g_u2_image_base + 0x00A49300ull);
            if (attach_fn) attach_fn(rdi, r12);
        }
    }
    if (status) *status = 2;
    return 0;
}

static int64_t ABI detour_DcTrainSpawnOnTrack(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A68740: (train-spawn-on-track train-id spline-id [dist]) */
    if (status) *status = 0;
    if (argc < 2 || !argv || !g_u2_image_base) return 0;
    uint8_t *train = u2_resolve_train(1, argv);
    if (!train) return 0;
    uint32_t spline_id = (uint32_t)argv[1];
    float dist = (argc >= 3) ? u2_reg_to_f32(argv[2]) : 0.0f;
    uint8_t *table = *(uint8_t **)(g_u2_image_base + 0x0104EB30ull + 8);
    if (table) {
        for (uint32_t i = 0; i < 0x180u; ++i) {
            uint8_t *entry = table + (size_t)i * 0x110 + 0x10;
            if (*(uint8_t **)(entry - 0x10) != NULL) {
                uint32_t *id_ptr = *(uint32_t **)(entry + 0x10);
                if (id_ptr && *id_ptr == spline_id) {
                    typedef void (ABI *TeleportFn)(void *, uint8_t *, float);
                    TeleportFn fn = (TeleportFn)(uintptr_t)(g_u2_image_base + 0x00A6CBC0ull);
                    if (fn) fn(train, entry, dist);
                    break;
                }
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcHeliMoveTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7FEE0: (heli-move-to [heli] target-locator [speed]) */
    if (status) *status = 0;
    uint8_t *heli = u2_resolve_heli(argc, argv);
    if (!heli) return 0;
    int32_t arg_idx = (argc >= 2) ? 1 : 0;
    void *target = (argc > arg_idx) ? (void *)(uintptr_t)argv[arg_idx] : NULL;
    float speed = (argc > arg_idx + 1) ? u2_reg_to_f32(argv[arg_idx + 1]) : 15.0f;
    if (target) {
        memcpy(heli + 0x270, target, 0x50);
        memcpy(heli + 0x2D0, (uint8_t *)target + 0x20, 0x10);
    }
    *(float *)(heli + 0x240) = speed;
    *(float *)(heli + 0x244) = speed;
    *(uint32_t *)(heli + 0x340) = 0;
    return 0;
}

static int64_t ABI detour_DcHeliMoveToSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7FDD0: (heli-move-to-spline [heli] spline-id [speed]) */
    if (status) *status = 0;
    uint8_t *heli = u2_resolve_heli(argc, argv);
    if (!heli) return 0;
    int32_t arg_idx = (argc >= 2) ? 1 : 0;
    uint32_t spline_id = (argc > arg_idx) ? (uint32_t)argv[arg_idx] : 0;
    float speed = (argc > arg_idx + 1) ? u2_reg_to_f32(argv[arg_idx + 1]) : 15.0f;
    typedef void (ABI *HeliMoveSplineFn)(void *, uint32_t, float, uint32_t, uint32_t);
    HeliMoveSplineFn fn = (HeliMoveSplineFn)(uintptr_t)(g_u2_image_base + 0x00A87990ull);
    if (fn) fn(heli, spline_id, speed, 0, 0);
    return 0;
}

static int64_t ABI detour_DcNetHeliMoveOnSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7FCC0: (net-heli-move-on-spline [heli] spline-id [speed]) */
    if (status) *status = 0;
    uint8_t *heli = u2_resolve_heli(argc, argv);
    if (!heli) return 0;
    int32_t arg_idx = (argc >= 2) ? 1 : 0;
    uint32_t spline_id = (argc > arg_idx) ? (uint32_t)argv[arg_idx] : 0;
    float speed = (argc > arg_idx + 1) ? u2_reg_to_f32(argv[arg_idx + 1]) : 15.0f;
    typedef void (ABI *NetHeliMoveFn)(void *, uint32_t, float);
    NetHeliMoveFn fn = (NetHeliMoveFn)(uintptr_t)(g_u2_image_base + 0x00A87B60ull);
    if (fn) fn(heli, spline_id, speed);
    return 0;
}

static int64_t ABI detour_DcWaitHeliMoveTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7FA70: (wait-heli-move-to [heli] [target] [speed]) */
    uint8_t *heli = u2_resolve_heli(argc, argv);
    if (!heli) {
        if (status) *status = 0;
        return 0;
    }
    int32_t arg_idx = (argc >= 2) ? 1 : 0;
    void *target = (argc > arg_idx) ? (void *)(uintptr_t)argv[arg_idx] : NULL;
    float speed = (argc > arg_idx + 1) ? u2_reg_to_f32(argv[arg_idx + 1]) : 15.0f;

    uint8_t *mgr = g_u2_image_base + 0x01050320ull;
    int32_t count = *(const int32_t *)(mgr + 0x200);
    /* Safe bounds check: clamps < 16 to avoid int $0x41 debug trap from original binary */
    void *train_car = NULL;
    if (count >= 0 && count < 16) {
        uint8_t *slot = mgr + 8 + (size_t)count * 32;
        train_car = *(void **)slot;
    }

    if (status) *status = 2; /* pending wait status */
    typedef void (ABI *WaitFn)(void *, void *, float, void *, uint32_t);
    WaitFn wait_fn = (WaitFn)(uintptr_t)(g_u2_image_base + 0x00A877A0ull);
    if (wait_fn) {
        wait_fn(heli, target, speed, train_car, 0);
    }
    return 0;
}

/* --- Spline Navigation & Rail Vehicles Subsystem --- */

static int64_t ABI detour_DcSplineLengthP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A256B0: (spline-length? spline-id) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv || !argv[0]) return 0;
    uint32_t spline_id = (uint32_t)argv[0];
    uint8_t *table = *(uint8_t **)(g_u2_image_base + 0x0104EB30ull + 8);
    if (!table) return 0;
    for (uint32_t i = 0; i < 0x180u; ++i) {
        uint8_t *entry = table + (size_t)i * 0x110 + 0x10;
        if (*(uint8_t **)(entry - 0x10) != NULL) {
            uint32_t *id_ptr = *(uint32_t **)(entry + 0x10);
            if (id_ptr && *id_ptr == spline_id) {
                uint8_t flags = *entry;
                if ((flags & 0x05) == 0) {
                    float length = *(const float *)(entry + 0x14);
                    return (int64_t)u2_f32_to_reg(length);
                }
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcWaitSplineDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25740: (wait-spline-distance entity target-dist) */
    if (!g_u2_image_base || argc < 2 || !argv) {
        if (status) *status = 0;
        return 0;
    }
    uint32_t ent_id = (uint32_t)argv[0];
    float target_dist = u2_reg_to_f32(argv[1]);
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = get_ent ? get_ent(0, ent_id) : NULL;
    if (!ent) {
        if (status) *status = 0;
        return 0;
    }

    /* Safe train check bounds clamped to avoid int $0x41 debug trap */
    uint8_t *mgr = g_u2_image_base + 0x01050320ull;
    int32_t count = *(const int32_t *)(mgr + 0x200);
    void *train_car = NULL;
    if (count >= 0 && count < 16) {
        uint8_t *slot = mgr + 8 + (size_t)count * 32;
        train_car = *(void **)slot;
    }
    (void)train_car;

    typedef float (ABI *GetCurDistFn)(void *);
    GetCurDistFn dist_fn = (GetCurDistFn)(uintptr_t)(g_u2_image_base + 0x00A23EC0ull);
    float cur_dist = dist_fn ? dist_fn(ent + 0x3B0) : *(const float *)(ent + 0x3B0);

    if (cur_dist >= target_dist) {
        if (status) *status = 0;
        return 1;
    }
    if (status) *status = 2; /* pending wait */
    return 0;
}

static int64_t ABI detour_DcSplineTransferPendingP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25A20: (spline-transfer-pending? [entity]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = get_ent ? get_ent(0, handle) : NULL;
    if (ent && (*(const uint32_t *)(ent + 0x34) & 0x10000u) != 0) {
        uint8_t *comp = *(uint8_t **)(ent + 0x1B8);
        if (comp && *(const uint32_t *)(comp + 0x20) != 0) {
            return 1;
        }
    }
    return 0;
}

static int64_t ABI detour_DcTransferToSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25A90: (transfer-to-spline [entity] target-spline [speed]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    int32_t arg_idx = (argc >= 2) ? 1 : 0;
    uint32_t handle = (argc >= 2) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    uint32_t target_spline = (uint32_t)argv[arg_idx];
    float speed = (argc > arg_idx + 1) ? u2_reg_to_f32(argv[arg_idx + 1]) : 10.0f;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = handle ? get_ent(0, handle) : NULL;
    if (ent) {
        typedef void (ABI *TransferFn)(void *, uint32_t, float);
        TransferFn fn = (TransferFn)(uintptr_t)(g_u2_image_base + 0x00A23F50ull);
        if (fn) fn(ent, target_spline, speed);
    }
    return 0;
}

static int64_t ABI detour_DcSetSplineFollowDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25C00: (set-spline-follow-distance [entity] dist) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    int32_t arg_idx = (argc >= 2) ? 1 : 0;
    uint32_t handle = (argc >= 2) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    float dist = u2_reg_to_f32(argv[arg_idx]);
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = handle ? get_ent(0, handle) : NULL;
    if (ent) {
        *(float *)(ent + 0x3B0) = dist;
    }
    return 0;
}

static int64_t ABI detour_DcSetSplineFollowAcceleration(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25CB0: (set-spline-follow-acceleration [entity] accel) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    int32_t arg_idx = (argc >= 2) ? 1 : 0;
    uint32_t handle = (argc >= 2) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    float accel = u2_reg_to_f32(argv[arg_idx]);
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = handle ? get_ent(0, handle) : NULL;
    if (ent) {
        *(float *)(ent + 0x540) = accel;
    }
    return 0;
}

static int64_t ABI detour_DcSetSplineFollowSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25D60: (set-spline-follow-speed [entity] speed [trans-time]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    int32_t arg_idx = (argc >= 2) ? 1 : 0;
    uint32_t handle = (argc >= 2) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    float speed = u2_reg_to_f32(argv[arg_idx]);
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = handle ? get_ent(0, handle) : NULL;
    if (ent) {
        *(float *)(ent + 0x534) = speed;
        *(float *)(ent + 0x538) = speed;
    }
    return 0;
}

static int64_t ABI detour_DcFollowSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25E50: (follow-spline [entity] spline-id [speed] [accel]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    int32_t arg_idx = (argc >= 2) ? 1 : 0;
    uint32_t handle = (argc >= 2) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    uint32_t spline_id = (uint32_t)argv[arg_idx];
    float speed = (argc > arg_idx + 1) ? u2_reg_to_f32(argv[arg_idx + 1]) : 10.0f;
    float accel = (argc > arg_idx + 2) ? u2_reg_to_f32(argv[arg_idx + 2]) : 5.0f;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = handle ? get_ent(0, handle) : NULL;
    if (ent) {
        typedef void (ABI *FollowFn)(void *, uint32_t, float, float);
        FollowFn fn = (FollowFn)(uintptr_t)(g_u2_image_base + 0x00A241B0ull);
        if (fn) fn(ent, spline_id, speed, accel);
    }
    return 0;
}

static int64_t ABI detour_DcSplineDistanceLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25F60: (spline-distance-locator spline-id locator) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv || !argv[0] || !argv[1]) return 0;
    uint32_t spline_id = (uint32_t)argv[0];
    const float *locator = (const float *)(uintptr_t)argv[1];
    uint8_t *table = *(uint8_t **)(g_u2_image_base + 0x0104EB30ull + 8);
    if (!table) return 0;
    for (uint32_t i = 0; i < 0x180u; ++i) {
        uint8_t *entry = table + (size_t)i * 0x110 + 0x10;
        if (*(uint8_t **)(entry - 0x10) != NULL) {
            uint32_t *id_ptr = *(uint32_t **)(entry + 0x10);
            if (id_ptr && *id_ptr == spline_id) {
                typedef float (ABI *LocDistFn)(uint8_t *, const float *);
                LocDistFn fn = (LocDistFn)(uintptr_t)(g_u2_image_base + 0x0046B0A0ull);
                float dist = fn ? fn(entry, locator) : 0.0f;
                return (int64_t)u2_f32_to_reg(dist);
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcSplineDistanceSpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A260E0: (spline-distance-spawner spline-id spawner-id) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t spline_id = (uint32_t)argv[0];
    uint32_t spawner_id = (uint32_t)argv[1];
    float out_dist = 0.0f;
    typedef uint8_t (ABI *SpawnerDistFn)(float *, uint32_t, uint32_t, const void *);
    SpawnerDistFn fn = (SpawnerDistFn)(uintptr_t)(g_u2_image_base + 0x0046B0A0ull);
    if (fn && fn(&out_dist, spawner_id, spline_id, (const void *)(g_u2_image_base + 0x00EA236Full))) {
        return (int64_t)u2_f32_to_reg(out_dist);
    }
    return 0;
}

static int64_t ABI detour_DcSplineActivate(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A26180: (spline-activate spline-id [active-p]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv || !argv[0]) return 0;
    uint32_t spline_id = (uint32_t)argv[0];
    int32_t active = (argc >= 2) ? (argv[1] != 0) : 1;
    uint8_t *table = *(uint8_t **)(g_u2_image_base + 0x0104EB30ull + 8);
    if (!table) return 0;
    for (uint32_t i = 0; i < 0x180u; ++i) {
        uint8_t *entry = table + (size_t)i * 0x110 + 0x10;
        if (*(uint8_t **)(entry - 0x10) != NULL) {
            uint32_t *id_ptr = *(uint32_t **)(entry + 0x10);
            if (id_ptr && *id_ptr == spline_id) {
                uint8_t flags = *entry;
                if (active) flags &= (uint8_t)~0x04;
                else flags |= 0x04;
                *entry = flags;
                return 1;
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcWaitHeliMoveToSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7F800: (wait-heli-move-to-spline [heli] spline-id [speed]) */
    uint8_t *heli = u2_resolve_heli(argc, argv);
    if (!heli) {
        if (status) *status = 0;
        return 0;
    }
    int32_t arg_idx = (argc >= 2) ? 1 : 0;
    uint32_t spline_id = (argc > arg_idx) ? (uint32_t)argv[arg_idx] : 0;
    float speed = (argc > arg_idx + 1) ? u2_reg_to_f32(argv[arg_idx + 1]) : 15.0f;

    /* Clamps train bounds safely to avoid int $0x41 debug trap from original binary */
    uint8_t *mgr = g_u2_image_base + 0x01050320ull;
    int32_t count = *(const int32_t *)(mgr + 0x200);
    void *train_car = NULL;
    if (count >= 0 && count < 16) {
        uint8_t *slot = mgr + 8 + (size_t)count * 32;
        train_car = *(void **)slot;
    }

    if (status) *status = 2; /* pending wait status */
    typedef void (ABI *WaitSplineFn)(void *, uint32_t, float, void *);
    WaitSplineFn fn = (WaitSplineFn)(uintptr_t)(g_u2_image_base + 0x00A87850ull);
    if (fn) fn(heli, spline_id, speed, train_car);
    return 0;
}

static int64_t ABI detour_DcRailSetMoverSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB80A0: (rail-set-mover-speed speed [mover-idx]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    float speed = u2_reg_to_f32(argv[0]);
    uint32_t mover_idx = (argc >= 2) ? (uint32_t)argv[1] : 0;
    if (mover_idx >= 2) return 0;
    uint8_t *rail_mgr = g_u2_image_base + 0x010508B0ull;
    if (rail_mgr) {
        *(float *)(rail_mgr + 0x48 + (size_t)mover_idx * 0x30) = speed;
    }
    return 0;
}

static int64_t ABI detour_DcRailAnimateToMover(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8130: (rail-animate-to-mover vehicle-id [mover-idx]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv || !argv[0]) return 0;
    uint32_t veh_id = (uint32_t)argv[0];
    uint32_t mover_idx = (argc >= 2) ? (uint32_t)argv[1] : 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *veh = get_ent ? get_ent(0, veh_id) : NULL;
    if (veh) {
        typedef void (ABI *AnimToMoverFn)(void *, uint32_t);
        AnimToMoverFn fn = (AnimToMoverFn)(uintptr_t)(g_u2_image_base + 0x00AB6B20ull);
        if (fn) fn(veh, mover_idx);
    }
    return 0;
}

static int64_t ABI detour_DcRailFollowMover(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB81E0: (rail-follow-mover vehicle-id [mover-idx]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv || !argv[0]) return 0;
    uint32_t veh_id = (uint32_t)argv[0];
    uint32_t mover_idx = (argc >= 2) ? (uint32_t)argv[1] : 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *veh = get_ent ? get_ent(0, veh_id) : NULL;
    if (veh) {
        typedef void (ABI *FollowMoverFn)(void *, uint32_t);
        FollowMoverFn fn = (FollowMoverFn)(uintptr_t)(g_u2_image_base + 0x00AB6BD0ull);
        if (fn) fn(veh, mover_idx);
    }
    return 0;
}

static int64_t ABI detour_DcRailActivateMover(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8340: (rail-activate-mover [mover-idx] [active-p]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t mover_idx = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    int32_t active = (argc >= 2 && argv) ? (argv[1] != 0) : 1;
    uint8_t *rail_mgr = g_u2_image_base + 0x010508B0ull;
    if (rail_mgr && mover_idx < 2) {
        *(uint32_t *)(rail_mgr + 0x54 + (size_t)mover_idx * 0x30) = active ? 1 : 0;
    }
    return 0;
}

static int64_t ABI detour_DcRailEnableGroundHug(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8430: (rail-enable-ground-hug vehicle-id [enable-p]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv || !argv[0]) return 0;
    uint32_t veh_id = (uint32_t)argv[0];
    int32_t enable = (argc >= 2) ? (argv[1] != 0) : 1;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *veh = get_ent ? get_ent(0, veh_id) : NULL;
    if (veh) {
        uint32_t flags = *(const uint32_t *)(veh + 0x138);
        if (enable) flags |= 0x02u;
        else flags &= ~0x02u;
        *(uint32_t *)(veh + 0x138) = flags;
    }
    return 0;
}

static int64_t ABI detour_DcRailGetSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB84E0: (rail-get-spline [vehicle-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *veh = get_ent ? get_ent(0, handle) : NULL;
    if (veh) {
        typedef uint32_t (ABI *GetRailSplineFn)(void *);
        GetRailSplineFn fn = (GetRailSplineFn)(uintptr_t)(g_u2_image_base + 0x000015B0ull);
        return fn ? (int64_t)fn(veh) : 0;
    }
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
    { "DcNetCoopTeamFailedP", 0x00669C20, (uintptr_t)detour_DcNetCoopTeamFailedP },
    { "DcPsnDemoShowUpsell", 0x009FD590, (uintptr_t)detour_DcPsnDemoShowUpsell },
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
    { "DcSwimDisallowStandOnBottom", 0x0090D680, (uintptr_t)detour_DcSwimDisallowStandOnBottom },
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
    { "DcSetE3LoadingScreen", 0x009FD5B0, (uintptr_t)detour_DcNoOp0 },
    { "DcEntitySetLevelB", 0x00A64C30, (uintptr_t)detour_DcNoOp0 },
    { "DcFmaResetPartModules", 0x00A00130, (uintptr_t)detour_DcNoOp0 },
    { "DcCameraEnable", 0x00A0E680, (uintptr_t)detour_DcCameraEnable },
    { "DcCameraAbandon", 0x00A0E5E0, (uintptr_t)detour_DcCameraAbandon },
    { "DcCameraFadeOut", 0x00A0E380, (uintptr_t)detour_DcCameraFadeOut },
    { "DcCameraLocalSpace", 0x00A0EA20, (uintptr_t)detour_DcCameraLocalSpace },
    { "DcDisableCoverCamera", 0x00A0E290, (uintptr_t)detour_DcDisableCoverCamera },
    { "DcAllowCameraCutCompensate", 0x00A0DD40, (uintptr_t)detour_DcAllowCameraCutCompensate },
    { "DcCameraAimOverridePoint", 0x00A0DF40, (uintptr_t)detour_DcCameraAimOverridePoint },
    { "DcCameraAimOverrideDirection", 0x00A0DE40, (uintptr_t)detour_DcCameraAimOverrideDirection },
    { "DcCameraAnimationDisable", 0x00A033B0, (uintptr_t)detour_DcCameraAnimationDisable },
    { "DcCameraAnimationEnable", 0x00A03480, (uintptr_t)detour_DcCameraAnimationEnable },
    { "DcCameraShakeInZoomCamera", 0x00A0DCA0, (uintptr_t)detour_DcCameraShakeInZoomCamera },
    { "DcCameraShake", 0x00A0E040, (uintptr_t)detour_DcCameraShake },
    { "DcIsCameraStickPressed", 0x00A0D610, (uintptr_t)detour_DcIsCameraStickPressed },
    { "DcCameraLocator", 0x00A128B0, (uintptr_t)detour_DcCameraLocator },
    { "DcSetDefaultFollowCameraSettings", 0x00A0EBF0, (uintptr_t)detour_DcSetDefaultFollowCameraSettings },
    { "DcForceNoShiftCameraZoomMode", 0x00A035E0, (uintptr_t)detour_DcForceNoShiftCameraZoomMode },
    { "DcForceAimingCameraUltraTight", 0x00A03670, (uintptr_t)detour_DcForceAimingCameraUltraTight },
    { "DcTrainSpawn", 0x00A67960, (uintptr_t)detour_DcTrainSpawn },
    { "DcTrainGetSpline", 0x00A67AE0, (uintptr_t)detour_DcTrainGetSpline },
    { "DcTrainGetDistance", 0x00A67B90, (uintptr_t)detour_DcTrainGetDistance },
    { "DcTrainGetHeadCar", 0x00A67C40, (uintptr_t)detour_DcTrainGetHeadCar },
    { "DcTrainTeleport", 0x00A67CD0, (uintptr_t)detour_DcTrainTeleport },
    { "DcTrainDecouple", 0x00A67E40, (uintptr_t)detour_DcTrainDecouple },
    { "DcTrainSetSpeedMph", 0x00A67F60, (uintptr_t)detour_DcTrainSetSpeedMph },
    { "DcTrainSetAcceleration", 0x00A68140, (uintptr_t)detour_DcTrainSetAcceleration },
    { "DcTrainGetAcceleration", 0x00A68230, (uintptr_t)detour_DcTrainGetAcceleration },
    { "DcTrainTrackPlayer", 0x00A68300, (uintptr_t)detour_DcTrainTrackPlayer },
    { "DcTrainAllowUnphysicalize", 0x00A68430, (uintptr_t)detour_DcTrainAllowUnphysicalize },
    { "DcTrainKillAllRiders", 0x00A68530, (uintptr_t)detour_DcTrainKillAllRiders },
    { "DcTrainTransferPendingP", 0x00A685E0, (uintptr_t)detour_DcTrainTransferPendingP },
    { "DcTrainSpawnOnTrack", 0x00A68740, (uintptr_t)detour_DcTrainSpawnOnTrack },
    { "DcHeliMoveTo", 0x00A7FEE0, (uintptr_t)detour_DcHeliMoveTo },
    { "DcHeliMoveToSpline", 0x00A7FDD0, (uintptr_t)detour_DcHeliMoveToSpline },
    { "DcNetHeliMoveOnSpline", 0x00A7FCC0, (uintptr_t)detour_DcNetHeliMoveOnSpline },
    { "DcWaitHeliMoveTo", 0x00A7FA70, (uintptr_t)detour_DcWaitHeliMoveTo },
    { "DcSplineLengthP", 0x00A256B0, (uintptr_t)detour_DcSplineLengthP },
    { "DcWaitSplineDistance", 0x00A25740, (uintptr_t)detour_DcWaitSplineDistance },
    { "DcSplineTransferPendingP", 0x00A25A20, (uintptr_t)detour_DcSplineTransferPendingP },
    { "DcTransferToSpline", 0x00A25A90, (uintptr_t)detour_DcTransferToSpline },
    { "DcSetSplineFollowDistance", 0x00A25C00, (uintptr_t)detour_DcSetSplineFollowDistance },
    { "DcSetSplineFollowAcceleration", 0x00A25CB0, (uintptr_t)detour_DcSetSplineFollowAcceleration },
    { "DcSetSplineFollowSpeed", 0x00A25D60, (uintptr_t)detour_DcSetSplineFollowSpeed },
    { "DcFollowSpline", 0x00A25E50, (uintptr_t)detour_DcFollowSpline },
    { "DcSplineDistanceLocator", 0x00A25F60, (uintptr_t)detour_DcSplineDistanceLocator },
    { "DcSplineDistanceSpawner", 0x00A260E0, (uintptr_t)detour_DcSplineDistanceSpawner },
    { "DcSplineActivate", 0x00A26180, (uintptr_t)detour_DcSplineActivate },
    { "DcWaitHeliMoveToSpline", 0x00A7F800, (uintptr_t)detour_DcWaitHeliMoveToSpline },
    { "DcRailSetMoverSpeed", 0x00AB80A0, (uintptr_t)detour_DcRailSetMoverSpeed },
    { "DcRailAnimateToMover", 0x00AB8130, (uintptr_t)detour_DcRailAnimateToMover },
    { "DcRailFollowMover", 0x00AB81E0, (uintptr_t)detour_DcRailFollowMover },
    { "DcRailActivateMover", 0x00AB8340, (uintptr_t)detour_DcRailActivateMover },
    { "DcRailEnableGroundHug", 0x00AB8430, (uintptr_t)detour_DcRailEnableGroundHug },
    { "DcRailGetSpline", 0x00AB84E0, (uintptr_t)detour_DcRailGetSpline },
};

size_t u2_get_scene_detours(const U2NativeDetour **out) {
    if (out) *out = s_scene_detours;
    return sizeof(s_scene_detours) / sizeof(s_scene_detours[0]);
}
