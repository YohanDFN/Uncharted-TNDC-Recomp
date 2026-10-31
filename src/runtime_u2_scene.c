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


static int64_t ABI detour_DcFmaRequestPartModule(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A00050: (fma-request-part-module [symbol-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t sym = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    if (!sym) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) sym = get_self(0, 0, 1);
    }
    if (!sym) return 0;
    uint8_t *fx_mgr = (uint8_t *)(g_u2_image_base + 0x013D1E70ull);
    typedef void (ABI *ReqFn)(void *, uint32_t);
    ReqFn req = (ReqFn)(uintptr_t)(g_u2_image_base + 0x00C54ED0ull);
    if (req) req(fx_mgr + 0x50, sym);
    return 0;
}

static int64_t ABI detour_DcKillParticles(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A00140: (kill-particles handle [immediate-p]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t handle = (uint32_t)argv[0];
    uint32_t idx = handle & 0x3FF;
    uint8_t *fx_mgr = (uint8_t *)(g_u2_image_base + 0x013D1E70ull);
    uint32_t max_count = *(uint32_t *)(fx_mgr + 0x9D8);
    if (idx >= max_count) return 0;
    uint8_t *pool = *(uint8_t **)(fx_mgr + 0x9D0);
    if (!pool) return 0;
    uint32_t cap = *(uint32_t *)(pool + 0x9C);
    if (idx >= cap) return 0;
    uint8_t *elem = (*(uint8_t **)(pool + 0x90)) + (size_t)idx * 40;
    uint32_t gen = handle >> 10;
    bool immediate = (argc >= 2 && argv[1] != 0);
    uint8_t *ptr = *(uint8_t **)elem;
    if (ptr && *(uint32_t *)ptr == gen) {
        uint16_t flag = immediate ? 2 : 1;
        *(uint16_t *)(ptr + 4) |= flag;
    }
    return 0;
}

static int64_t ABI detour_DcSpawnParticlesAtLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A002D0: (spawn-particles-at-locator fx-name locator [entity-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t fx_sym = (uint32_t)argv[0];
    uint8_t *loc = (uint8_t *)argv[1];
    uint32_t ent_handle = (argc >= 3) ? (uint32_t)argv[2] : 0;
    if (!loc || !fx_sym) return 0;
    if (!ent_handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) ent_handle = get_self(0, 0, 1);
    }
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = ent_handle ? get_ent(0, ent_handle) : NULL;
    (void)ent;

    /* Build spawn descriptor (0x220 bytes) */
    uint8_t desc[0x220];
    memset(desc, 0, sizeof(desc));
    *(uint32_t *)(desc + 0x00) = 0xF6725C04; /* magic header */
    *(uint64_t *)(desc + 0x08) = *(uint64_t *)(g_u2_image_base + 0x0115F1A0ull + 0xC98);
    *(uint64_t *)(desc + 0x18) = (uintptr_t)(desc + 0x60);

    /* FX parameters at offset 0x60 */
    *(uint32_t *)(desc + 0x60) = 0x68A84033;
    *(uint32_t *)(desc + 0x64) = fx_sym;
    *(float *)(desc + 0x94) = 1.0f;
    *(uint8_t *)(desc + 0xAC) = 1;
    *(uint8_t *)(desc + 0xAD) = 1;

    /* Copy transform matrix from locator */
    memcpy(desc + 0xF0, loc, sizeof(float) * 16);
    *(uint32_t *)(desc + 0x1B8) = 0;

    typedef uint8_t * (ABI *SpawnFxFn)(void *, uint32_t);
    SpawnFxFn spawn_fn = (SpawnFxFn)(uintptr_t)(g_u2_image_base + 0x009B5C20ull);
    uint8_t *pfx = spawn_fn ? spawn_fn(desc, 0) : NULL;
    if (pfx) {
        uint8_t *inst = *(uint8_t **)(pfx + 0xA8);
        uint32_t gen = *(uint32_t *)(pfx + 0xB0);
        if (inst && gen) {
            uint8_t *fx_mgr = (uint8_t *)(g_u2_image_base + 0x013D1E70ull);
            uint8_t *pool = *(uint8_t **)(fx_mgr + 0x9D0);
            if (pool) {
                uint8_t *base = *(uint8_t **)(pool + 0x90);
                if (inst >= base) {
                    uint32_t idx = (uint32_t)((inst - base) / 40);
                    return (int64_t)((idx & 0x3FF) | (gen << 10));
                }
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcSpawnParticlesAtJoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A00950: (spawn-particles-at-joint ...) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *SpawnPartFn)(int32_t, uint64_t *, uint32_t);
    SpawnPartFn fn = (SpawnPartFn)(uintptr_t)(g_u2_image_base + 0x009F2E80ull);
    return fn ? fn(argc, argv, 2) : 0;
}

static int64_t ABI detour_DcSpawnParticlesAttached(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A00960: (spawn-particles-attached ...) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *SpawnPartFn)(int32_t, uint64_t *, uint32_t);
    SpawnPartFn fn = (SpawnPartFn)(uintptr_t)(g_u2_image_base + 0x009F2E80ull);
    return fn ? fn(argc, argv, 1) : 0;
}

static int64_t ABI detour_DcPartModuleLoadedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A00970: (part-module-loaded? [symbol-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t sym = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    if (!sym) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) sym = get_self(0, 0, 1);
    }
    if (!sym) return 0;
    uint8_t *fx_mgr = (uint8_t *)(g_u2_image_base + 0x013D1E70ull);
    uint32_t count = *(uint32_t *)(fx_mgr + 0x8C8);
    if (!count) return 0;
    uint8_t *entry = fx_mgr + 0x298;
    for (uint32_t i = 0; i < count; ++i) {
        if (*(uint32_t *)entry == sym) {
            return 1;
        }
        entry += 0x30;
    }
    return 0;
}

static int64_t ABI detour_DcSpawnSnowDisturb(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01BB0: (spawn-snow-disturb locator-or-pos [radius]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv || !argv[0]) return 0;
    uint8_t *loc = (uint8_t *)argv[0];
    float radius = 1.0f;
    if (argc >= 2) {
        union { uint64_t u; float f; } conv;
        conv.u = argv[1];
        radius = conv.f;
    }
    (void)loc;
    (void)radius;
    return 0;
}

static int64_t ABI detour_DcGroundPrintsType(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01F00: (ground-prints-type) */
    (void)argv;
    if (status) *status = 0;
    return (argc > 0) ? 1 : 0;
}

static int64_t ABI detour_DcRailDisableCareen(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8B80: (rail-disable-careen [vehicle-id] [disabled-bool]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    bool disable = (argc >= 2 && argv && argv[1] != 0);
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
        typedef void (ABI *CareenFn)(void *, bool);
        CareenFn fn = (CareenFn)(uintptr_t)(g_u2_image_base + 0x00004EE0ull);
        if (fn) fn(veh, disable);
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcRailForceCareen(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8C40: (rail-force-careen [vehicle-id]) */
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
        typedef void (ABI *ForceCareenFn)(void *);
        ForceCareenFn fn = (ForceCareenFn)(uintptr_t)(g_u2_image_base + 0x00004EB0ull);
        if (fn) fn(veh);
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcRailFireTurretGrenade(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8CD0: (rail-fire-turret-grenade [turret-id] [target-id] [param]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t turret_id = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    uint32_t target_id = (argc > 1 && argv && argv[1]) ? (uint32_t)argv[1] : 0;
    uint32_t param = (argc > 2 && argv && argv[2]) ? (uint32_t)argv[2] : 0;
    typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
    GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
    if (!turret_id && get_self) turret_id = get_self(0, 0, 1);
    if (!target_id && get_self) target_id = get_self(0, 0, 1);
    if (!param && get_self) param = get_self(0, 0, 1);
    if (!turret_id || !target_id) return 0;

    typedef uint8_t * (ABI *FindActorFn)(void *, uint32_t);
    FindActorFn find_actor = (FindActorFn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    uint8_t *turret = find_actor ? find_actor((void *)(g_u2_image_base + 0x0110DCB8ull), turret_id) : NULL;
    if (!turret) return 0;

    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *target = get_ent ? get_ent(0, target_id) : NULL;
    if (!target) return 0;

    uint8_t *sub = *(uint8_t **)(turret + 0x4020);
    if (sub && *(uint32_t *)(sub + 0x10) == 0xCC3BC266) {
        typedef void (ABI *FireGrenadeFn)(void *, void *, uint32_t);
        FireGrenadeFn fn = (FireGrenadeFn)(uintptr_t)(g_u2_image_base + 0x00A7D320ull);
        if (fn) fn(turret, target, param);
    }
    return 0;
}

static int64_t ABI detour_DcRailEnableCollideWithObjects(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8EA0: (rail-enable-collide-with-objects [vehicle-id] [enabled-bool]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    bool enable = (argc >= 2 && argv && argv[1] != 0);
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
        typedef void (ABI *CollideFn)(void *, bool);
        CollideFn fn = (CollideFn)(uintptr_t)(g_u2_image_base + 0x00003590ull);
        if (fn) fn(veh, enable);
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcRailScrub(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8F90: (rail-scrub vehicle-id scrub-pos) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t handle = (uint32_t)argv[0];
    if (!handle) return 0;
    union { uint64_t u; float f; } conv;
    conv.u = argv[1];
    float scrub_pos = conv.f;

    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *veh = get_ent ? get_ent(0, handle) : NULL;
    if (veh) {
        typedef uint8_t (ABI *GetActiveFn)(void *);
        typedef void (ABI *SetActiveFn)(void *, uint32_t);
        typedef void (ABI *SetPosFn)(void *, float);
        GetActiveFn get_act = (GetActiveFn)(uintptr_t)(g_u2_image_base + 0x000054C0ull);
        SetActiveFn set_act = (SetActiveFn)(uintptr_t)(g_u2_image_base + 0x00005480ull);
        SetPosFn set_pos = (SetPosFn)(uintptr_t)(g_u2_image_base + 0x000057F0ull);
        uint8_t was_active = get_act ? get_act(veh) : 0;
        if (set_act) set_act(veh, 1);
        if (set_pos) set_pos(veh, scrub_pos);
        if (set_act) set_act(veh, was_active);
    }
    return 0;
}

static int64_t ABI detour_DcRailOverrideSfxState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9070: (rail-override-sfx-state [vehicle-id] [sfx-state]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    uint32_t sfx_state = (argc >= 2 && argv && argv[1]) ? (uint32_t)argv[1] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *veh = get_ent ? get_ent(0, handle) : NULL;
    if (veh && *(uint32_t *)(veh + 0x30) == 0xDC984A89) {
        typedef void (ABI *OverrideSfxFn)(void *, uint32_t);
        OverrideSfxFn fn = (OverrideSfxFn)(uintptr_t)(g_u2_image_base + 0x00013170ull);
        if (fn) fn(veh, sfx_state);
    }
    return 0;
}

static int64_t ABI detour_DcRailSetSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9230: (rail-set-speed vehicle-id speed [immediate-p]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t handle = (uint32_t)argv[0];
    if (!handle) return 0;
    union { uint64_t u; float f; } conv;
    conv.u = argv[1];
    float speed = conv.f;
    bool immediate = (argc >= 3 && argv[2] != 0);

    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *veh = get_ent ? get_ent(0, handle) : NULL;
    if (veh) {
        typedef float (ABI *GetScaleFn)(void *);
        GetScaleFn get_scale = (GetScaleFn)(uintptr_t)(g_u2_image_base + 0x00004A70ull);
        float scale = get_scale ? get_scale(veh) : 1.0f;
        float final_speed = speed * scale;
        if (immediate) {
            typedef void (ABI *SetSpdImmFn)(void *, float);
            SetSpdImmFn fn = (SetSpdImmFn)(uintptr_t)(g_u2_image_base + 0x000053C0ull);
            if (fn) fn(veh, final_speed);
        } else {
            typedef void (ABI *SetSpdFn)(void *, float);
            SetSpdFn fn = (SetSpdFn)(uintptr_t)(g_u2_image_base + 0x00005240ull);
            if (fn) fn(veh, final_speed);
        }
    }
    return 0;
}

static int64_t ABI detour_DcRailShutdown(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9340: (rail-shutdown) */
    (void)argc;
    (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint8_t *mgr = (uint8_t *)(g_u2_image_base + 0x0115F1A0ull);
    uint8_t *head = *(uint8_t **)(mgr + 0xC50);
    typedef uint8_t * (ABI *NextMoverFn)(void *, uint32_t);
    NextMoverFn next_fn = (NextMoverFn)(uintptr_t)(g_u2_image_base + 0x009BCDF0ull);
    typedef void (ABI *ShutdownMoverFn)(void *);
    ShutdownMoverFn shut_fn = (ShutdownMoverFn)(uintptr_t)(g_u2_image_base + 0x009BE760ull);
    typedef uint8_t (ABI *CheckMoverFn)(void *);
    CheckMoverFn check_fn = (CheckMoverFn)(uintptr_t)(g_u2_image_base + 0x000011D0ull);

    uint8_t *mover = head;
    while (mover && next_fn) {
        mover = next_fn(mover, 0);
        if (!mover) break;
        uint16_t flags = *(uint16_t *)(mover + 0x34);
        if ((flags & 1) && check_fn && check_fn(mover) && shut_fn) {
            shut_fn(mover);
        }
    }
    return 0;
}

static int64_t ABI detour_DcRailUseFixedAprefInIgc(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9410: (rail-use-fixed-apref-in-igc [vehicle-id] [enabled-bool]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    bool enable = (argc < 2 || (argv && argv[1] != 0));
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
        typedef void (ABI *SetFixedFn)(void *, bool);
        SetFixedFn fn = (SetFixedFn)(uintptr_t)(g_u2_image_base + 0x00005590ull);
        if (fn) fn(veh, enable);
    }
    return 0;
}

static int64_t ABI detour_DcRailHasVehicle(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB94B0: (rail-has-vehicle? vehicle-id) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv || !argv[0]) return 0;
    uint32_t handle = (uint32_t)argv[0];
    typedef int32_t (ABI *FindSlotFn)(uint32_t, uint32_t);
    FindSlotFn find_slot = (FindSlotFn)(uintptr_t)(g_u2_image_base + 0x00A91D00ull);
    int32_t slot = find_slot ? find_slot(handle, 0) : -1;
    if (slot >= 0) {
        uint8_t *table = (uint8_t *)(g_u2_image_base + 0x0153EFA8ull);
        size_t off = (size_t)slot * 96;
        uint8_t **veh_slot = *(uint8_t ***)(table + off + 0x118);
        uint32_t gen = *(uint32_t *)(table + off + 0x120);
        if (veh_slot && gen) {
            uint8_t *veh = *veh_slot;
            if (veh && *(uint32_t *)(veh + 0x44) == gen) {
                return 1;
            }
        }
    }
    return 0;
}


static int64_t ABI detour_DcTankWaitTeleport(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA4420: (tank-wait-teleport [tank-id] [locator-matrix] [speed]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t handle = (uint32_t)argv[0];
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *tank = get_ent ? get_ent(0, handle) : NULL;
    if (tank && argc >= 2 && argv[1]) {
        /* Teleport tank to target locator matrix */
        memcpy(tank + 0x50, (void *)argv[1], sizeof(float) * 16);
    }
    return 1;
}

static int64_t ABI detour_DcTankForceFireCannon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA42C0: (tank-force-fire-cannon [tank-id] [target-pos-vector]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t handle = (uint32_t)argv[0];
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *tank = get_ent ? get_ent(0, handle) : NULL;
    if (tank && *(uint32_t *)(tank + 0x30) == 0x9F81473C) {
        if (argc >= 2 && argv[1]) {
            memcpy(tank + 0x1420, (void *)argv[1], sizeof(float) * 3);
        }
        /* Trigger cannon fire state */
        *(uint32_t *)(tank + 0x1418) = 1;
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcInFrontOfTankP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3E10: (in-front-of-tank? tank-id target-id) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t tank_id = (uint32_t)argv[0];
    uint32_t target_id = (uint32_t)argv[1];
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *tank = get_ent ? get_ent(0, tank_id) : NULL;
    uint8_t *target = get_ent ? get_ent(0, target_id) : NULL;
    if (!tank || !target || *(uint32_t *)(tank + 0x30) != 0x9F81473C) return 0;
    float *tank_pos = (float *)(tank + 0x50 + 0x30);
    float *tank_fwd = (float *)(tank + 0x50 + 0x20);
    float *tgt_pos = (float *)(target + 0x50 + 0x30);
    float dx = tgt_pos[0] - tank_pos[0];
    float dy = tgt_pos[1] - tank_pos[1];
    float dz = tgt_pos[2] - tank_pos[2];
    float dot = dx * tank_fwd[0] + dy * tank_fwd[1] + dz * tank_fwd[2];
    return (dot > 0.0f) ? 1 : 0;
}

static int64_t ABI detour_DcTankGetHealth(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3D80: (tank-get-health [tank-id]) */
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
    uint8_t *tank = get_ent ? get_ent(0, handle) : NULL;
    if (tank && *(uint32_t *)(tank + 0x30) == 0x9F81473C) {
        return (int64_t)*(int32_t *)(tank + 0x1410);
    }
    return 0;
}

static int64_t ABI detour_DcTankSetHealth(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3CC0: (tank-set-health [tank-id] [health]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t handle = (uint32_t)argv[0];
    int32_t health = (int32_t)argv[1];
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *tank = get_ent ? get_ent(0, handle) : NULL;
    if (tank && *(uint32_t *)(tank + 0x30) == 0x9F81473C) {
        int32_t max_hp = *(int32_t *)(tank + 0x1414);
        if (health > max_hp) health = max_hp;
        *(int32_t *)(tank + 0x1410) = health;
        typedef void (ABI *TankUpdateFn)(void *);
        TankUpdateFn update_fn = (TankUpdateFn)(uintptr_t)(g_u2_image_base + 0x00AAFD70ull);
        if (update_fn) update_fn(tank);
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcWaitTankMoveTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3C60: (wait-tank-move-to ...) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *TankWaitFn)(int32_t, uint64_t *, uint32_t, uint32_t, uint64_t, void *, uint64_t, uint64_t);
    TankWaitFn fn = (TankWaitFn)(uintptr_t)(g_u2_image_base + 0x00AA3490ull);
    return fn ? fn(argc, argv, 2, 4, (uintptr_t)status, NULL, 0, 0) : 1;
}

static int64_t ABI detour_DcWaitTankReverseTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3BC0: (wait-tank-reverse-to ...) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *TankWaitFn)(int32_t, uint64_t *, uint32_t, uint32_t, uint64_t, void *, uint64_t, uint64_t);
    TankWaitFn fn = (TankWaitFn)(uintptr_t)(g_u2_image_base + 0x00AA3490ull);
    return fn ? fn(argc, argv, 2, 4, (uintptr_t)status, NULL, 0, 0) : 1;
}

static int64_t ABI detour_DcWaitTankFireAtPos(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3B60: (wait-tank-fire-at-pos ...) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *TankWaitFn)(int32_t, uint64_t *, uint32_t, uint32_t, uint64_t, void *, uint64_t, uint64_t);
    TankWaitFn fn = (TankWaitFn)(uintptr_t)(g_u2_image_base + 0x00AA3490ull);
    return fn ? fn(argc, argv, 2, 4, (uintptr_t)status, NULL, 0, 0) : 1;
}

static int64_t ABI detour_DcWaitTankFireAtObj(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3B00: (wait-tank-fire-at-obj ...) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *TankWaitFn)(int32_t, uint64_t *, uint32_t, uint32_t, uint64_t, void *, uint64_t, uint64_t);
    TankWaitFn fn = (TankWaitFn)(uintptr_t)(g_u2_image_base + 0x00AA3490ull);
    return fn ? fn(argc, argv, 2, 3, (uintptr_t)status, NULL, 0, 0) : 1;
}

static int64_t ABI detour_DcWaitTankAimAtPos(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3AA0: (wait-tank-aim-at-pos ...) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *TankWaitFn)(int32_t, uint64_t *, uint32_t, uint32_t, uint64_t, void *, uint64_t, uint64_t);
    TankWaitFn fn = (TankWaitFn)(uintptr_t)(g_u2_image_base + 0x00AA3490ull);
    return fn ? fn(argc, argv, 2, 2, (uintptr_t)status, NULL, 0, 0) : 1;
}

static int64_t ABI detour_DcTankFindBestTargetMP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA4950: (tank-find-best-target-mp ...) */
    (void)argc;
    (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcHeliSetMgStats(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7F6B0: (heli-set-mg-stats [heli-id] [burst-len] [fire-rate] [cooldown]) */
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
    uint8_t *heli = get_ent ? get_ent(0, handle) : NULL;
    if (heli && *(uint32_t *)(heli + 0x30) == 0x5C01F503) {
        if (argc >= 2) *(uint32_t *)(heli + 0x5D8) = (uint32_t)argv[1];
        if (argc >= 3) {
            union { uint64_t u; float f; } c; c.u = argv[2];
            *(float *)(heli + 0x5DC) = c.f;
        }
        if (argc >= 4) {
            union { uint64_t u; float f; } c; c.u = argv[3];
            *(float *)(heli + 0x5E0) = c.f;
        }
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcHeliSetFiringState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7F5C0: (heli-set-firing-state [heli-id] [firing-bool] [param]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    bool firing = (argc < 2 || (argv && argv[1] != 0));
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *heli = get_ent ? get_ent(0, handle) : NULL;
    if (heli && *(uint32_t *)(heli + 0x30) == 0x5C01F503) {
        *(uint8_t *)(heli + 0x5D6) = firing ? 1 : 0;
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcHeliSetTargetObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7F4A0: (heli-set-target-object [heli-id] [target-obj-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    uint32_t target_id = (argc >= 2 && argv && argv[1]) ? (uint32_t)argv[1] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *heli = get_ent ? get_ent(0, handle) : NULL;
    uint8_t *target = target_id ? get_ent(0, target_id) : NULL;
    if (heli && *(uint32_t *)(heli + 0x30) == 0x5C01F503) {
        *(uint8_t **)(heli + 0x5E8) = target;
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcHeliSetTargetLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7F2E0: (heli-set-target-locator [heli-id] [locator-matrix] [speed]) */
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
    uint8_t *heli = get_ent ? get_ent(0, handle) : NULL;
    if (heli && *(uint32_t *)(heli + 0x30) == 0x5C01F503) {
        if (argc >= 2 && argv[1]) {
            memcpy(heli + 0x5F0, (void *)argv[1], sizeof(float) * 16);
        }
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcHeliFireMissile(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7F140: (heli-fire-missile [heli-id] [target-matrix] [homing-bool]) */
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
    uint8_t *heli = get_ent ? get_ent(0, handle) : NULL;
    if (heli && *(uint32_t *)(heli + 0x30) == 0x5C01F503) {
        /* Set missile launch trigger on helicopter */
        *(uint32_t *)(heli + 0x630) = 1;
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcWaitHeliFireMissile(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7EE30: (wait-heli-fire-missile [heli-id] [target-matrix]) */
    if (status) *status = 0;
    return detour_DcHeliFireMissile(argc, argv, status);
}

static int64_t ABI detour_DcHeliSetHover(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7ED80: (heli-set-hover [heli-id] [hover-bool]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    bool hover = (argc < 2 || (argv && argv[1] != 0));
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *heli = get_ent ? get_ent(0, handle) : NULL;
    if (heli && *(uint32_t *)(heli + 0x30) == 0x5C01F503) {
        *(uint8_t *)(heli + 0x5D4) = hover ? 1 : 0;
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcHeliAnimateGun(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7ECD0: (heli-animate-gun [heli-id] [anim-bool]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    bool anim = (argc < 2 || (argv && argv[1] != 0));
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *heli = get_ent ? get_ent(0, handle) : NULL;
    if (heli && *(uint32_t *)(heli + 0x30) == 0x5C01F503) {
        *(uint8_t *)(heli + 0x5D5) = anim ? 1 : 0;
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcHeliSetBasePitch(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7EC00: (heli-set-base-pitch [heli-id] [pitch-angle]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    float pitch = 0.0f;
    if (argc >= 2) {
        union { uint64_t u; float f; } c; c.u = argv[1];
        pitch = c.f * 0.0174532925f; /* Deg to Rad */
    }
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    if (!handle) return 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *heli = get_ent ? get_ent(0, handle) : NULL;
    if (heli && *(uint32_t *)(heli + 0x30) == 0x5C01F503) {
        *(float *)(heli + 0x5C8) = pitch;
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcHeliEnableWind(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7EB60: (heli-enable-wind [enabled-bool]) */
    if (status) *status = 0;
    (void)argc;
    (void)argv;
    return 1;
}


static int64_t ABI detour_DcUseReflections(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02C10: (use-reflections flag) */
    (void)status;
    if (!g_u2_image_base) return 0;
    int32_t flag = (argc > 0 && argv && argv[0] != 0) ? 1 : 0;
    *(uint8_t *)(g_u2_image_base + 0x01164980ull) = (uint8_t)flag;
    return 0;
}

static int64_t ABI detour_DcUseVisibleWaterUnitForReflection(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02BC0: (use-visible-water-unit-for-reflection flag) */
    (void)status;
    if (!g_u2_image_base) return 0;
    int32_t flag = (argc > 0 && argv && argv[0] != 0) ? 1 : 0;
    *(uint8_t *)(g_u2_image_base + 0x01164981ull) = (uint8_t)flag;
    return 0;
}

static int64_t ABI detour_DcUseWaterUnitForReflection(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02C70: (use-water-unit-for-reflection unit-idx flag) */
    (void)status;
    if (!g_u2_image_base) return 0;
    if (argc >= 2 && argv) {
        int32_t idx = (int32_t)argv[0];
        int32_t flag = (argv[1] != 0) ? 1 : 0;
        if (idx >= 0 && idx < 16) {
            *(uint8_t *)(g_u2_image_base + 0x01164982ull + idx) = (uint8_t)flag;
        }
    }
    return 0;
}

static int64_t ABI detour_DcUseRainIntensityValues(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A031C0: (use-rain-intensity-values ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcUseWindIntensityValues(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03290: (use-wind-intensity-values ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcKillAllBreakables(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F260: (kill-all-breakables) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *scene_mgr = *(const uint8_t * const *)(g_u2_image_base + 0x0115F1A0ull + 0xc58);
    if (!scene_mgr) return 0;
    typedef void (ABI *kill_fn)(const void *, uint32_t);
    kill_fn fn = (kill_fn)(uintptr_t)(g_u2_image_base + 0x0088BC10ull);
    if (fn) fn(scene_mgr, 0);
    return 0;
}

static int64_t ABI detour_DcKillAllDroppedWeapons(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F1B0: (kill-all-dropped-weapons) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *weapon_mgr = *(const uint8_t * const *)(g_u2_image_base + 0x0115F1A0ull + 0xc90);
    if (!weapon_mgr) return 0;
    typedef void (ABI *kill_fn)(const void *);
    kill_fn fn = (kill_fn)(uintptr_t)(g_u2_image_base + 0x00898510ull);
    if (fn) fn(weapon_mgr);
    return 0;
}

static int64_t ABI detour_DcKillAllAttachables(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009F2BF0: (kill-all-attachables) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcKillPlayerFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0A920: (kill-player-flashlight) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcKillSullivanFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0A750: (kill-sullivan-flashlight) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}


static int64_t ABI detour_DcCalcNewPhaseFromSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FBF0: (calc-new-phase-from-speed ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return (int64_t)u2_f32_to_reg(0.0f);
}

static int64_t ABI detour_DcCalcStartPhase(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FB70: (calc-start-phase ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return (int64_t)u2_f32_to_reg(0.0f);
}

static int64_t ABI detour_DcCalcStartPhaseMaxFrames(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FAF0: (calc-start-phase-max-frames ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return (int64_t)u2_f32_to_reg(0.0f);
}

static int64_t ABI detour_DcCalcStartPhaseRange(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FA70: (calc-start-phase-range ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return (int64_t)u2_f32_to_reg(0.0f);
}

static int64_t ABI detour_DcMakeCharacterMoreWet(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A023C0: (make-character-more-wet [character] amount) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcMakeActiveCharacterMoreWet(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A020E0: (make-active-character-more-wet amount) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcMakeVector(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDCF0: (make-vector [x] [y] [z] [w]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void *(ABI *alloc_fn)(uint32_t, uint32_t);
    alloc_fn alloc_v = (alloc_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    float *v = alloc_v ? (float *)alloc_v(16, 16) : NULL;
    if (v) {
        v[0] = (argc > 0 && argv) ? u2_reg_to_f32(argv[0]) : 0.0f;
        v[1] = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
        v[2] = (argc > 2 && argv) ? u2_reg_to_f32(argv[2]) : 0.0f;
        v[3] = (argc > 3 && argv) ? u2_reg_to_f32(argv[3]) : 1.0f;
    }
    return (int64_t)(uintptr_t)v;
}

static int64_t ABI detour_DcMakeVector3(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDC20: (make-vector-3 [x] [y] [z]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void *(ABI *alloc_fn)(uint32_t, uint32_t);
    alloc_fn alloc_v = (alloc_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    float *v = alloc_v ? (float *)alloc_v(16, 16) : NULL;
    if (v) {
        v[0] = (argc > 0 && argv) ? u2_reg_to_f32(argv[0]) : 0.0f;
        v[1] = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
        v[2] = (argc > 2 && argv) ? u2_reg_to_f32(argv[2]) : 0.0f;
        v[3] = 0.0f;
    }
    return (int64_t)(uintptr_t)v;
}

static int64_t ABI detour_DcDebugDrawLine(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE090: (debug-draw-line p0 p1 [color]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcDebugDrawCross(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE070: (debug-draw-cross pt [radius] [color]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcDebugDrawLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE000: (debug-draw-locator loc [color]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcDebugDrawString(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDFB0: (debug-draw-string str pt [color]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcAddImportModule(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FF10: (add-import-module mod-name) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcAddLoadModule(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FE90: (add-load-module mod-name) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcAddDynamicLight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FF2C0: (add-dynamic-light ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}


/* --------------------------------------------------------------------------
   LOT 18: Tank & Rail Vehicle Combat Systems (26 funcs)
   -------------------------------------------------------------------------- */

static int64_t ABI detour_DcTankMoveTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA33E0: (tank-move-to [tank] [dest-vec] [speed]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA3010ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankReverseTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2F80: (tank-reverse-to [tank] [dest-vec] [speed]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA3010ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankFireAtPos(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2F30: (tank-fire-at-pos [tank] [pos-vec] [lead]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA2B60ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankFireAtObj(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2EE0: (tank-fire-at-obj [tank] [target] [lead]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA26D0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankAimAtPos(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2E90: (tank-aim-at-pos [tank] [pos-vec]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA2B60ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankAimAtPosInstant(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2B10: (tank-aim-at-pos-instant [tank] [pos-vec]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA2B60ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankAimAtObj(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2AC0: (tank-aim-at-obj [tank] [target]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA26D0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankAimForward(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2A70: (tank-aim-forward [tank]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA26D0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankFire(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2930: (tank-fire [tank]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    typedef uint8_t * (ABI *GetEntityFn)(uint32_t, int32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    uint8_t *tank = get_ent ? get_ent(handle, 0) : NULL;
    if (tank) {
        typedef void (ABI *fire_fn)(void *);
        fire_fn fn = (fire_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(tank);
    }
    return 1;
}

static int64_t ABI detour_DcTankMgFireAtObj(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2680: (tank-mg-fire-at-obj [tank] [target]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA21C0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankMgFireAtLoc(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2120: (tank-mg-fire-at-loc [tank] [loc-vec]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA21C0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankMgCeaseFire(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2000: (tank-mg-cease-fire [tank]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA20A0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankTeleport(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA1BA0: (tank-teleport [tank] [locator]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA1C40ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankMoveTrackingObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA1550: (tank-move-tracking-object [tank] [target]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA1600ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankDistToPoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA1300: (tank-dist-to-point [tank] [pos-vec]) -> float */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    typedef uint8_t * (ABI *GetEntityFn)(uint32_t, int32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    uint8_t *tank = get_ent ? get_ent(handle, 0) : NULL;
    if (tank && argc >= 2 && argv && argv[1]) {
        const float *target_pt = (const float *)(uintptr_t)argv[1];
        const float *tank_pos = (const float *)(tank + 0x30);
        float dx = tank_pos[0] - target_pt[0];
        float dy = tank_pos[1] - target_pt[1];
        float dz = tank_pos[2] - target_pt[2];
        float dist = sqrtf(dx * dx + dy * dy + dz * dz);
        uint32_t bits;
        memcpy(&bits, &dist, sizeof(bits));
        return (int64_t)bits;
    }
    return 0;
}

static int64_t ABI detour_DcRailEnableTruckJump(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9BB0: (rail-enable-truck-jump [flag]) */
    (void)argc;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    int32_t flag = (argc > 0 && argv) ? (int32_t)argv[0] : 1;
    typedef void * (ABI *prop_helper_fn)(int32_t, const char *, float);
    prop_helper_fn prop_fn = (prop_helper_fn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    void *obj = prop_fn ? prop_fn(0, "enable-truck-jump", (float)flag) : NULL;
    if (obj) {
        typedef void (ABI *post_fn)(void *, int32_t);
        post_fn pfn = (post_fn)(uintptr_t)(g_u2_image_base + 0x00DE6400ull);
        if (pfn) pfn(obj, 1);
    }
    return 1;
}

static int64_t ABI detour_DcRailSpawn(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9920: (rail-spawn [rail] [spawner-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t rail_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    uint32_t spawner_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *rail = lookup ? lookup(rail_id, 0) : NULL;
    if (rail) {
        typedef void (ABI *spawn_fn)(void *, uint32_t);
        spawn_fn fn = (spawn_fn)(uintptr_t)(g_u2_image_base + 0x00A34240ull);
        if (fn) fn(rail, spawner_id);
    }
    return 1;
}

static int64_t ABI detour_DcRailDeactivate(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB97D0: (rail-deactivate [rail]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t rail_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *rail = lookup ? lookup(rail_id, 0) : NULL;
    if (rail) {
        typedef void (ABI *deact_fn)(void *);
        deact_fn fn = (deact_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(rail);
    }
    return 1;
}

static int64_t ABI detour_DcRailEnableTributary(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB96B0: (rail-enable-tributary [rail] [tributary-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t rail_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    uint32_t trib_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *rail = lookup ? lookup(rail_id, 0) : NULL;
    if (rail) {
        typedef void (ABI *trib_fn)(void *, uint32_t);
        trib_fn fn = (trib_fn)(uintptr_t)(g_u2_image_base + 0x00A35970ull);
        if (fn) fn(rail, trib_id);
    }
    return 1;
}

static int64_t ABI detour_DcRailSetNpcArchetype(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9660: (rail-set-npc-archetype [rail] [archetype]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t rail_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    uint32_t arch = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *rail = lookup ? lookup(rail_id, 0) : NULL;
    if (rail) {
        typedef void (ABI *arch_fn)(void *, uint32_t);
        arch_fn fn = (arch_fn)(uintptr_t)(g_u2_image_base + 0x00A904E0ull);
        if (fn) fn(rail, arch);
    }
    return 1;
}

static int64_t ABI detour_DcRailSetAcceleration(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9590: (rail-set-acceleration [rail] [accel]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t rail_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    float accel = 0.0f;
    if (argc >= 2 && argv) accel = *(const float *)&argv[1];
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *rail = lookup ? lookup(rail_id, 0) : NULL;
    if (rail) {
        typedef void (ABI *accel_fn)(void *, float);
        accel_fn fn = (accel_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(rail, accel);
    }
    return 1;
}

static int64_t ABI detour_DcRailSetFollowDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8560: (rail-set-follow-distance [rail] [min-dist] [max-dist]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t rail_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    float min_dist = (argc >= 2 && argv) ? *(const float *)&argv[1] : 0.0f;
    float max_dist = (argc >= 3 && argv) ? *(const float *)&argv[2] : min_dist;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *rail = lookup ? lookup(rail_id, 0) : NULL;
    if (rail) {
        typedef void (ABI *dist_fn)(void *, float, float);
        dist_fn fn = (dist_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(rail, min_dist, max_dist);
    }
    return 1;
}

static int64_t ABI detour_DcRailSetMoverFollowDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB7FD0: (rail-set-mover-follow-distance [distance]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float dist = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *mover_dist = (float *)(uintptr_t)(g_u2_image_base + 0x0156DF78ull);
    if (mover_dist) *mover_dist = dist;
    return 1;
}

static int64_t ABI detour_DcRailSetMoverIntialDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB7E80: (rail-set-mover-initial-distance [distance]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float dist = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *mover_init = (float *)(uintptr_t)(g_u2_image_base + 0x0156DF7Cull);
    if (mover_init) *mover_init = dist;
    return 1;
}

static int64_t ABI detour_DcRailGetMoverLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB7D00: (rail-get-mover-locator [out-loc]) */
    (void)argc;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    if (argv && argv[0]) {
        float *out_loc = (float *)(uintptr_t)argv[0];
        const float *src_loc = (const float *)(uintptr_t)(g_u2_image_base + 0x01540FF0ull);
        if (src_loc) {
            memcpy(out_loc, src_loc, 16 * sizeof(float));
            return (int64_t)(uintptr_t)out_loc;
        }
    }
    return 0;
}

static int64_t ABI detour_DcRailSetAllVehiclesToMoverSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB7BF0: (rail-set-all-vehicles-to-mover-speed [speed]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float speed = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *target_speed = (float *)(uintptr_t)(g_u2_image_base + 0x0156DF80ull);
    if (target_speed) *target_speed = speed;
    return 1;
}


/* --------------------------------------------------------------------------
   LOT 19 (Part 3): Engine Wait Scheduler, Coroutines & System Enables (23 funcs)
   -------------------------------------------------------------------------- */

static int64_t ABI detour_DcWaitAnimate(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x003969A0: (wait-animate [anim-track]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcEnableMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DC8E0: (enable-message [msg-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t msg_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *msg_fn)(uint32_t, int32_t);
    msg_fn fn = (msg_fn)(uintptr_t)(g_u2_image_base + 0x005DC800ull);
    if (fn) fn(msg_id, 1);
    return 1;
}

static int64_t ABI detour_DcDisableMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DC990: (disable-message [msg-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t msg_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *msg_fn)(uint32_t, int32_t);
    msg_fn fn = (msg_fn)(uintptr_t)(g_u2_image_base + 0x005DC800ull);
    if (fn) fn(msg_id, 0);
    return 1;
}

static int64_t ABI detour_DcEnableActionPack(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0A060: (enable-action-pack [ap-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t ap_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *ap_fn)(uint32_t);
    ap_fn fn = (ap_fn)(uintptr_t)(g_u2_image_base + 0x00A09F80ull);
    if (fn) fn(ap_id);
    return 1;
}

static int64_t ABI detour_DcWaitSpawnEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09E90: (wait-spawn-entity [archetype] [name]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcWaitSpawnEntityAt(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09DC0: (wait-spawn-entity-at [archetype] [pos]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcEnableNavBlockerObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04F60: (enable-nav-blocker-object [obj-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t obj_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *nav_fn)(uint32_t, int32_t);
    nav_fn fn = (nav_fn)(uintptr_t)(g_u2_image_base + 0x00A04D00ull);
    if (fn) fn(obj_id, 1);
    return 1;
}

static int64_t ABI detour_DcDisableNavBlockerObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04EF0: (disable-nav-blocker-object [obj-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t obj_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *nav_fn)(uint32_t, int32_t);
    nav_fn fn = (nav_fn)(uintptr_t)(g_u2_image_base + 0x00A04D00ull);
    if (fn) fn(obj_id, 0);
    return 1;
}

static int64_t ABI detour_DcDisablePlatformControlOnObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04E80: (disable-platform-control-on-object [obj-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t obj_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *plat_fn)(uint32_t);
    plat_fn fn = (plat_fn)(uintptr_t)(g_u2_image_base + 0x00A04D80ull);
    if (fn) fn(obj_id);
    return 1;
}

static int64_t ABI detour_DcDisableGadgetry(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03A70: (disable-gadgetry [obj-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t obj_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *gadget_fn)(uint32_t, int32_t);
    gadget_fn fn = (gadget_fn)(uintptr_t)(g_u2_image_base + 0x00A03900ull);
    if (fn) fn(obj_id, 0);
    return 1;
}

static int64_t ABI detour_DcEnableGadgetry(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A039C0: (enable-gadgetry [obj-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t obj_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *gadget_fn)(uint32_t, int32_t);
    gadget_fn fn = (gadget_fn)(uintptr_t)(g_u2_image_base + 0x00A03900ull);
    if (fn) fn(obj_id, 1);
    return 1;
}

static int64_t ABI detour_DcDisableFrameForward(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A038A0: (disable-frame-forward) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint8_t *flag = (uint8_t *)(uintptr_t)(g_u2_image_base + 0x01161EEAull);
    if (flag) *flag = 0;
    return 1;
}

static int64_t ABI detour_DcWaitInt32(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A41D40: (wait-int32 [target-val]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcWaitBoolean(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A41610: (wait-boolean [condition]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcWait(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A3E690: (wait [seconds]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcWaitFrames(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A41270: (wait-frames [num-frames]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcWaitOneFrame(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A40F70: (wait-one-frame) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcWaitUntil(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A40E60: (wait-until [predicate]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcWaitUntilFrame(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A40D40: (wait-until-frame [target-frame]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcWaitUntilNetTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A40870: (wait-until-net-time [target-time]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcWaitFor(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A3FC50: (wait-for [coroutine-task]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcWaitTankAimAtObj(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3A40: (wait-tank-aim-at-obj [tank] [target]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA26D0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcWaitTankAimForward(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3430: (wait-tank-aim-forward [tank]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA26D0ull);
    return fn ? fn(argc, argv, status) : 0;
}


/* --------------------------------------------------------------------------
   LOT 20 (Part 4): Scene Shaders, Water, Weather, Lighting & Renders (32 funcs)
   -------------------------------------------------------------------------- */

static int64_t ABI detour_DcSetAnimationSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00397880: (set-animation-speed [anim] [speed]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetLookatStopPhase(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00397320: (set-lookat-stop-phase [val]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetTableShaderMaterialValuesParam(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009F3B70: (set-table-shader-material-values-param ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetShaderMaterialParam(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10DD0: (set-shader-material-param ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetSingleShaderInstanceParam(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10A50: (set-single-shader-instance-param ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetShaderInstanceParam(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A107B0: (set-shader-instance-param ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetBlackFade(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10700: (set-black-fade [fade-amount]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float fade = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *p_fade = (float *)(uintptr_t)(g_u2_image_base + 0x01150494ull);
    if (p_fade) *p_fade = fade;
    return 1;
}

static int64_t ABI detour_DcSetWhiteFade(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A106A0: (set-white-fade [fade-amount]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float fade = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *p_fade = (float *)(uintptr_t)(g_u2_image_base + 0x01150498ull);
    if (p_fade) *p_fade = fade;
    return 1;
}

static int64_t ABI detour_DcSetFocusableIgnore(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F3C0: (set-focusable-ignore [obj-id] [flag]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t obj_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    int32_t flag = (argc >= 2 && argv) ? (int32_t)argv[1] : 1;
    typedef void (ABI *focus_fn)(uint32_t, int32_t);
    focus_fn fn = (focus_fn)(uintptr_t)(g_u2_image_base + 0x00A0F300ull);
    if (fn) fn(obj_id, flag);
    return 1;
}

static int64_t ABI detour_DcSetFocusableGrappled(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F2C0: (set-focusable-grappled [obj-id] [flag]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t obj_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    int32_t flag = (argc >= 2 && argv) ? (int32_t)argv[1] : 1;
    typedef void (ABI *focus_fn)(uint32_t, int32_t);
    focus_fn fn = (focus_fn)(uintptr_t)(g_u2_image_base + 0x00A0F300ull);
    if (fn) fn(obj_id, flag);
    return 1;
}

static int64_t ABI detour_DcSetClockMultiplier(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0EE60: (set-clock-multiplier [speed]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float mult = (argc > 0 && argv) ? *(const float *)&argv[0] : 1.0f;
    float *p_clock = (float *)(uintptr_t)(g_u2_image_base + 0x01161EE0ull);
    if (p_clock) *p_clock = mult;
    return 1;
}

static int64_t ABI detour_DcSetVisible(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0BD30: (set-visible [entity] [visible-p]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t entity_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    int32_t vis = (argc >= 2 && argv) ? (int32_t)argv[1] : 1;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(entity_id, 0) : NULL;
    if (ent) {
        typedef void (ABI *vis_fn)(void *, int32_t);
        vis_fn fn = (vis_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(ent, vis);
    }
    return 1;
}

static int64_t ABI detour_DcSetMeshVisible(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0BA20: (set-mesh-visible [entity] [mesh-idx] [visible-p]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t entity_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    uint32_t mesh = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    int32_t vis = (argc >= 3 && argv) ? (int32_t)argv[2] : 1;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(entity_id, 0) : NULL;
    if (ent) {
        typedef void (ABI *mesh_fn)(void *, uint32_t, int32_t);
        mesh_fn fn = (mesh_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(ent, mesh, vis);
    }
    return 1;
}

static int64_t ABI detour_DcSetObjectHealth(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04A20: (set-object-health [entity] [hp]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t entity_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    float hp = (argc >= 2 && argv) ? *(const float *)&argv[1] : 100.0f;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(entity_id, 0) : NULL;
    if (ent) {
        typedef void (ABI *hp_fn)(void *, float);
        hp_fn fn = (hp_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(ent, hp);
    }
    return 1;
}

static int64_t ABI detour_DcSetCollisionLayer(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03CD0: (set-collision-layer [entity] [layer]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t entity_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    uint32_t layer = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(entity_id, 0) : NULL;
    if (ent) {
        typedef void (ABI *layer_fn)(void *, uint32_t);
        layer_fn fn = (layer_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(ent, layer);
    }
    return 1;
}

static int64_t ABI detour_DcSetWaterOffset(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02FF0: (set-water-offset [offset-vec]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetWaterState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02E20: (set-water-state [state]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetDeformationOfWaterUnit(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02CC0: (set-deformation-of-water-unit [unit] [deform]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetRainState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02830: (set-rain-state [state]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetRainRipplesState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A027F0: (set-rain-ripples-state [state]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetTimeToDryPeriod(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02780: (set-time-to-dry-period [time-sec]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetCharacterWetMaskValue(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A026E0: (set-character-wet-mask-value [character] [val]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetCharacterWetMaskState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02690: (set-character-wet-mask-state [character] [state]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetCharacterWetMask(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02550: (set-character-wet-mask [character] [mask]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetMaxCharacterWetness(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02510: (set-max-character-wetness [character] [max-wet]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetRenderSettings(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FFF80: (set-render-settings [setting-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetLightFov(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FFC80: (set-light-fov [light-id] [fov-deg]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetDynamicLightColor(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FEE20: (set-dynamic-light-color [light-id] [color-vec]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetCurrentBySpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD710: (set-current-by-spawner [spawner-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetRagdollFlow(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD5C0: (set-ragdoll-flow [entity] [flow-vec]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSetObjectFaction(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FCF40: (set-object-faction [entity] [faction-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t entity_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    uint32_t faction = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(entity_id, 0) : NULL;
    if (ent) {
        typedef void (ABI *faction_fn)(void *, uint32_t);
        faction_fn fn = (faction_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(ent, faction);
    }
    return 1;
}

static int64_t ABI detour_DcSetIceCaveStupidGearsHack(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FCC50: (set-ice-cave-stupid-gears-hack [flag]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}


/* --------------------------------------------------------------------------
   LOT 21 (Part 3): Scene Object, Locator, Joint & Physics Getters (27 funcs)
   -------------------------------------------------------------------------- */

static int64_t ABI detour_DcGetAnimationPhase(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x003976C0: (get-animation-phase [anim]) -> float */
    (void)argc; (void)argv;
    if (status) *status = 0;
    float phase = 0.0f;
    uint32_t bits;
    memcpy(&bits, &phase, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetObjectPosition(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A12610: (get-object-position [entity] [out-vec]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(ent_id, 0) : NULL;
    if (ent && argc >= 2 && argv && argv[1]) {
        float *out = (float *)(uintptr_t)argv[1];
        const float *pos = (const float *)((uint8_t *)ent + 0x30);
        memcpy(out, pos, 4 * sizeof(float));
        return (int64_t)(uintptr_t)out;
    }
    return 0;
}

static int64_t ABI detour_DcGetObjectOffsetPosition(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A12390: (get-object-offset-position [entity] [offset-vec] [out-vec]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(ent_id, 0) : NULL;
    if (ent && argc >= 3 && argv && argv[1] && argv[2]) {
        const float *offset = (const float *)(uintptr_t)argv[1];
        float *out = (float *)(uintptr_t)argv[2];
        const float *pos = (const float *)((uint8_t *)ent + 0x30);
        out[0] = pos[0] + offset[0];
        out[1] = pos[1] + offset[1];
        out[2] = pos[2] + offset[2];
        out[3] = 1.0f;
        return (int64_t)(uintptr_t)out;
    }
    return 0;
}

static int64_t ABI detour_DcGetObjectOffsetLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A121E0: (get-object-offset-locator [entity] [offset] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(ent_id, 0) : NULL;
    if (ent && argc >= 3 && argv && argv[2]) {
        float *out = (float *)(uintptr_t)argv[2];
        const float *mat = (const float *)((uint8_t *)ent + 0x40);
        memcpy(out, mat, 16 * sizeof(float));
        return (int64_t)(uintptr_t)out;
    }
    return 0;
}

static int64_t ABI detour_DcGetJointPosition(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A120C0: (get-joint-position [entity] [joint-idx] [out-pos]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(ent_id, 0) : NULL;
    if (ent && argc >= 3 && argv && argv[2]) {
        float *out = (float *)(uintptr_t)argv[2];
        const float *pos = (const float *)((uint8_t *)ent + 0x30);
        memcpy(out, pos, 4 * sizeof(float));
        return (int64_t)(uintptr_t)out;
    }
    return 0;
}

static int64_t ABI detour_DcGetQuaternionPitch(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009F37C0: (get-quaternion-pitch [quat]) -> float */
    if (status) *status = 0;
    float pitch = 0.0f;
    if (argc > 0 && argv && argv[0]) {
        const float *q = (const float *)(uintptr_t)argv[0];
        pitch = asinf(2.0f * (q[0] * q[2] - q[3] * q[1]));
    }
    uint32_t bits;
    memcpy(&bits, &pitch, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetLocatorDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0EF50: (get-locator-distance [loc1] [loc2]) -> float */
    if (status) *status = 0;
    float dist = 0.0f;
    if (argc >= 2 && argv && argv[0] && argv[1]) {
        const float *p1 = (const float *)(uintptr_t)argv[0];
        const float *p2 = (const float *)(uintptr_t)argv[1];
        float dx = p1[12] - p2[12];
        float dy = p1[13] - p2[13];
        float dz = p1[14] - p2[14];
        dist = sqrtf(dx * dx + dy * dy + dz * dz);
    }
    uint32_t bits;
    memcpy(&bits, &dist, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetObjectId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0BEE0: (get-object-id [entity]) -> id */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    return (int64_t)(uint32_t)argv[0];
}

static int64_t ABI detour_DcGetSpawnerPosition(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A094B0: (get-spawner-position [spawner-id] [out-pos]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    out[0] = 0.0f; out[1] = 0.0f; out[2] = 0.0f; out[3] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetSpawnerLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09330: (get-spawner-locator [spawner-id] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetSpawnerLocatorProjected(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A08D10: (get-spawner-locator-projected [spawner-id] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetSpawnerLocatorRadial(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A088C0: (get-spawner-locator-radial [spawner-id] [radius] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 3 || !argv || !argv[2]) return 0;
    float *out = (float *)(uintptr_t)argv[2];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetObjectLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A08630: (get-object-locator [entity] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(ent_id, 0) : NULL;
    if (ent && argc >= 2 && argv && argv[1]) {
        float *out = (float *)(uintptr_t)argv[1];
        const float *mat = (const float *)((uint8_t *)ent + 0x40);
        memcpy(out, mat, 16 * sizeof(float));
        return (int64_t)(uintptr_t)out;
    }
    return 0;
}

static int64_t ABI detour_DcGetObjectLocatorRadial(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A081A0: (get-object-locator-radial [entity] [radius] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(ent_id, 0) : NULL;
    if (ent && argc >= 3 && argv && argv[2]) {
        float *out = (float *)(uintptr_t)argv[2];
        const float *mat = (const float *)((uint8_t *)ent + 0x40);
        memcpy(out, mat, 16 * sizeof(float));
        return (int64_t)(uintptr_t)out;
    }
    return 0;
}

static int64_t ABI detour_DcGetJointLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A07E80: (get-joint-locator [entity] [joint] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(ent_id, 0) : NULL;
    if (ent && argc >= 3 && argv && argv[2]) {
        float *out = (float *)(uintptr_t)argv[2];
        const float *mat = (const float *)((uint8_t *)ent + 0x40);
        memcpy(out, mat, 16 * sizeof(float));
        return (int64_t)(uintptr_t)out;
    }
    return 0;
}

static int64_t ABI detour_DcGetApReferenceLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A07990: (get-ap-reference-locator [ap-ref] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetApReferenceEffectLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A07340: (get-ap-reference-effect-locator [ap-ref] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetApReferenceLocatorRadial(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A06C20: (get-ap-reference-locator-radial [ap-ref] [radius] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 3 || !argv || !argv[2]) return 0;
    float *out = (float *)(uintptr_t)argv[2];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetAlignLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A066F0: (get-align-locator [entity] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetJointAngleDiffFromBindpose(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A06180: (get-joint-angle-diff-from-bindpose [entity] [joint]) -> float */
    (void)argc; (void)argv;
    if (status) *status = 0;
    float diff = 0.0f;
    uint32_t bits;
    memcpy(&bits, &diff, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetActionPackLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A060C0: (get-action-pack-locator [ap] [out-matrix]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetOriginLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A05F60: (get-origin-locator [out-matrix]) */
    if (status) *status = 0;
    if (argc < 1 || !argv || !argv[0]) return 0;
    float *out = (float *)(uintptr_t)argv[0];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetObjectState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04C10: (get-object-state [entity]) -> state */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(ent_id, 0) : NULL;
    return ent ? (int64_t)(*(const uint32_t *)((uint8_t *)ent + 0x20)) : 0;
}

static int64_t ABI detour_DcGetObjectHealth(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04B10: (get-object-health [entity]) -> float */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(ent_id, 0) : NULL;
    float hp = ent ? *(const float *)((uint8_t *)ent + 0x28) : 100.0f;
    uint32_t bits;
    memcpy(&bits, &hp, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetWaterOffset(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02F20: (get-water-offset [out-vec]) */
    if (status) *status = 0;
    if (argc < 1 || !argv || !argv[0]) return 0;
    float *out = (float *)(uintptr_t)argv[0];
    out[0] = 0.0f; out[1] = 0.0f; out[2] = 0.0f; out[3] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetWaterState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02D90: (get-water-state) -> int */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcGetDda(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD010: (get-dda) -> float */
    (void)argc; (void)argv;
    if (status) *status = 0;
    float dda = 1.0f;
    uint32_t bits;
    memcpy(&bits, &dda, sizeof(bits));
    return (int64_t)bits;
}


/* --------------------------------------------------------------------------
   LOT 22 (Part 4): Scene Spawners, Entities, Ropes & Nav Polys (27 funcs)
   -------------------------------------------------------------------------- */

static int64_t ABI detour_DcEntityAutoSpawnOnce(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A12830: (entity-auto-spawn-once [spawner]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t spawner_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *spawn_fn)(uint32_t);
    spawn_fn fn = (spawn_fn)(uintptr_t)(g_u2_image_base + 0x00A127A0ull);
    if (fn) fn(spawner_id);
    return 1;
}

static int64_t ABI detour_DcObjectSpawnedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A12780: (object-spawned? [entity]) -> bool */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t ent_id = (uint32_t)argv[0];
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    return lookup && lookup(ent_id, 0) != NULL ? 1 : 0;
}

static int64_t ABI detour_DcBattleSelectBestSpawnerFromGroup(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A11190: (battle-select-best-spawner-from-group [group]) -> spawner-id */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t group_id = (uint32_t)argv[0];
    typedef uint32_t (ABI *select_fn)(uint32_t);
    select_fn fn = (select_fn)(uintptr_t)(g_u2_image_base + 0x00A10F70ull);
    return fn ? (int64_t)fn(group_id) : 0;
}

static int64_t ABI detour_DcSelectBestSchemaSpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10EB0: (select-best-schema-spawner [schema]) -> spawner-id */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t schema_id = (uint32_t)argv[0];
    typedef uint32_t (ABI *select_fn)(uint32_t);
    select_fn fn = (select_fn)(uintptr_t)(g_u2_image_base + 0x00A10E00ull);
    return fn ? (int64_t)fn(schema_id) : 0;
}

static int64_t ABI detour_DcObjectInRegionP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FCA0: (object-in-region? [entity] [region]) -> bool */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t ent_id = (uint32_t)argv[0];
    uint32_t reg_id = (uint32_t)argv[1];
    typedef int32_t (ABI *in_reg_fn)(uint32_t, uint32_t);
    in_reg_fn fn = (in_reg_fn)(uintptr_t)(g_u2_image_base + 0x00A0FBC0ull);
    return fn ? (fn(ent_id, reg_id) != 0 ? 1 : 0) : 0;
}

static int64_t ABI detour_DcSpawnAttachable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F540: (spawn-attachable [archetype] [parent] [joint]) -> ent-id */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 3 || !argv) return 0;
    typedef uint32_t (ABI *attach_fn)(uint32_t, uint32_t, uint32_t);
    attach_fn fn = (attach_fn)(uintptr_t)(g_u2_image_base + 0x00A0F4C0ull);
    return fn ? (int64_t)fn((uint32_t)argv[0], (uint32_t)argv[1], (uint32_t)argv[2]) : 0;
}

static int64_t ABI detour_DcSlaForceTackle(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F4A0: (sla-force-tackle [entity]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcCreateWeaponDrawables(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0C410: (create-weapon-drawables [entity]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcEmitBulletFromSpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0BF40: (emit-bullet-from-spawner [spawner] [target]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcForceStealthOff(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0BC40: (force-stealth-off) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint8_t *stealth = (uint8_t *)(uintptr_t)(g_u2_image_base + 0x01161EE6ull);
    if (stealth) *stealth = 0;
    return 1;
}

static int64_t ABI detour_DcStickySpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0B8F0: (sticky-spawner [spawner]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcUnstickySpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0B7C0: (unsticky-spawner [spawner]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcAttachFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0A4C0: (attach-flashlight [entity]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcKillFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0A130: (kill-flashlight [entity]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSpawnEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09F40: (spawn-entity [archetype] [name]) -> ent-id */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    typedef uint32_t (ABI *spawn_fn)(uint32_t, uint64_t);
    spawn_fn fn = (spawn_fn)(uintptr_t)(g_u2_image_base + 0x00A096E0ull);
    return fn ? (int64_t)fn((uint32_t)argv[0], argc >= 2 ? argv[1] : 0) : 0;
}

static int64_t ABI detour_DcSpawnEntityAt(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009F3900: (spawn-entity-at [archetype] [pos-vec]) -> ent-id */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    typedef uint32_t (ABI *spawn_fn)(uint32_t, uint64_t);
    spawn_fn fn = (spawn_fn)(uintptr_t)(g_u2_image_base + 0x00A096E0ull);
    return fn ? (int64_t)fn((uint32_t)argv[0], argc >= 2 ? argv[1] : 0) : 0;
}

static int64_t ABI detour_DcKillEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09D60: (kill-entity [entity]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t ent_id = (uint32_t)argv[0];
    typedef void (ABI *kill_fn)(uint32_t);
    kill_fn fn = (kill_fn)(uintptr_t)(g_u2_image_base + 0x00A09BF0ull);
    if (fn) fn(ent_id);
    return 1;
}

static int64_t ABI detour_DcKillDroppedWeaponsInSphere(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09B10: (kill-dropped-weapons-in-sphere [center] [radius]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcKillPartSpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A099B0: (kill-part-spawner [spawner]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcSwapEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09590: (swap-entity [old-ent] [new-archetype]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcBlockNavPolyUnderObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A05030: (block-nav-poly-under-object [entity]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    typedef void (ABI *nav_fn)(uint32_t, int32_t);
    nav_fn fn = (nav_fn)(uintptr_t)(g_u2_image_base + 0x00A04FA0ull);
    if (fn) fn((uint32_t)argv[0], 1);
    return 1;
}

static int64_t ABI detour_DcUnblockNavPolyUnderObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04FC0: (unblock-nav-poly-under-object [entity]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    typedef void (ABI *nav_fn)(uint32_t, int32_t);
    nav_fn fn = (nav_fn)(uintptr_t)(g_u2_image_base + 0x00A04FA0ull);
    if (fn) fn((uint32_t)argv[0], 0);
    return 1;
}

static int64_t ABI detour_DcRegenerateCoverActionPacksFromObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04E20: (regenerate-cover-action-packs-from-object [entity]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcRemoveCoverActionPacksFromObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04DC0: (remove-cover-action-packs-from-object [entity]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcRenameObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04D10: (rename-object [entity] [new-name]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcGrabRope(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04740: (grab-rope [entity] [rope-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcDropRope(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A045E0: (drop-rope [entity]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
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
    { "DcFmaRequestPartModule", 0x00A00050, (uintptr_t)detour_DcFmaRequestPartModule },
    { "DcKillParticles", 0x00A00140, (uintptr_t)detour_DcKillParticles },
    { "DcSpawnParticlesAtLocator", 0x00A002D0, (uintptr_t)detour_DcSpawnParticlesAtLocator },
    { "DcSpawnParticlesAtJoint", 0x00A00950, (uintptr_t)detour_DcSpawnParticlesAtJoint },
    { "DcSpawnParticlesAttached", 0x00A00960, (uintptr_t)detour_DcSpawnParticlesAttached },
    { "DcPartModuleLoadedP", 0x00A00970, (uintptr_t)detour_DcPartModuleLoadedP },
    { "DcSpawnSnowDisturb", 0x00A01BB0, (uintptr_t)detour_DcSpawnSnowDisturb },
    { "DcGroundPrintsType", 0x00A01F00, (uintptr_t)detour_DcGroundPrintsType },
    { "DcRailDisableCareen", 0x00AB8B80, (uintptr_t)detour_DcRailDisableCareen },
    { "DcRailForceCareen", 0x00AB8C40, (uintptr_t)detour_DcRailForceCareen },
    { "DcRailFireTurretGrenade", 0x00AB8CD0, (uintptr_t)detour_DcRailFireTurretGrenade },
    { "DcRailEnableCollideWithObjects", 0x00AB8EA0, (uintptr_t)detour_DcRailEnableCollideWithObjects },
    { "DcRailScrub", 0x00AB8F90, (uintptr_t)detour_DcRailScrub },
    { "DcRailOverrideSfxState", 0x00AB9070, (uintptr_t)detour_DcRailOverrideSfxState },
    { "DcRailSetSpeed", 0x00AB9230, (uintptr_t)detour_DcRailSetSpeed },
    { "DcRailShutdown", 0x00AB9340, (uintptr_t)detour_DcRailShutdown },
    { "DcRailUseFixedAprefInIgc", 0x00AB9410, (uintptr_t)detour_DcRailUseFixedAprefInIgc },
    { "DcRailHasVehicle", 0x00AB94B0, (uintptr_t)detour_DcRailHasVehicle },
    { "DcTankWaitTeleport", 0x00AA4420, (uintptr_t)detour_DcTankWaitTeleport },
    { "DcTankForceFireCannon", 0x00AA42C0, (uintptr_t)detour_DcTankForceFireCannon },
    { "DcInFrontOfTankP", 0x00AA3E10, (uintptr_t)detour_DcInFrontOfTankP },
    { "DcTankGetHealth", 0x00AA3D80, (uintptr_t)detour_DcTankGetHealth },
    { "DcTankSetHealth", 0x00AA3CC0, (uintptr_t)detour_DcTankSetHealth },
    { "DcWaitTankMoveTo", 0x00AA3C60, (uintptr_t)detour_DcWaitTankMoveTo },
    { "DcWaitTankReverseTo", 0x00AA3BC0, (uintptr_t)detour_DcWaitTankReverseTo },
    { "DcWaitTankFireAtPos", 0x00AA3B60, (uintptr_t)detour_DcWaitTankFireAtPos },
    { "DcWaitTankFireAtObj", 0x00AA3B00, (uintptr_t)detour_DcWaitTankFireAtObj },
    { "DcWaitTankAimAtPos", 0x00AA3AA0, (uintptr_t)detour_DcWaitTankAimAtPos },
    { "DcTankFindBestTargetMP", 0x00AA4950, (uintptr_t)detour_DcTankFindBestTargetMP },
    { "DcHeliSetMgStats", 0x00A7F6B0, (uintptr_t)detour_DcHeliSetMgStats },
    { "DcHeliSetFiringState", 0x00A7F5C0, (uintptr_t)detour_DcHeliSetFiringState },
    { "DcHeliSetTargetObject", 0x00A7F4A0, (uintptr_t)detour_DcHeliSetTargetObject },
    { "DcHeliSetTargetLocator", 0x00A7F2E0, (uintptr_t)detour_DcHeliSetTargetLocator },
    { "DcHeliFireMissile", 0x00A7F140, (uintptr_t)detour_DcHeliFireMissile },
    { "DcWaitHeliFireMissile", 0x00A7EE30, (uintptr_t)detour_DcWaitHeliFireMissile },
    { "DcHeliSetHover", 0x00A7ED80, (uintptr_t)detour_DcHeliSetHover },
    { "DcHeliAnimateGun", 0x00A7ECD0, (uintptr_t)detour_DcHeliAnimateGun },
    { "DcHeliSetBasePitch", 0x00A7EC00, (uintptr_t)detour_DcHeliSetBasePitch },
    { "DcHeliEnableWind", 0x00A7EB60, (uintptr_t)detour_DcHeliEnableWind },
    { "DcUseReflections", 0x00A02C10, (uintptr_t)detour_DcUseReflections },
    { "DcUseVisibleWaterUnitForReflection", 0x00A02BC0, (uintptr_t)detour_DcUseVisibleWaterUnitForReflection },
    { "DcUseWaterUnitForReflection", 0x00A02C70, (uintptr_t)detour_DcUseWaterUnitForReflection },
    { "DcUseRainIntensityValues", 0x00A031C0, (uintptr_t)detour_DcUseRainIntensityValues },
    { "DcUseWindIntensityValues", 0x00A03290, (uintptr_t)detour_DcUseWindIntensityValues },
    { "DcKillAllBreakables", 0x00A0F260, (uintptr_t)detour_DcKillAllBreakables },
    { "DcKillAllDroppedWeapons", 0x00A0F1B0, (uintptr_t)detour_DcKillAllDroppedWeapons },
    { "DcKillAllAttachables", 0x009F2BF0, (uintptr_t)detour_DcKillAllAttachables },
    { "DcKillPlayerFlashlight", 0x00A0A920, (uintptr_t)detour_DcKillPlayerFlashlight },
    { "DcKillSullivanFlashlight", 0x00A0A750, (uintptr_t)detour_DcKillSullivanFlashlight },
    { "DcCalcNewPhaseFromSpeed", 0x00A0FBF0, (uintptr_t)detour_DcCalcNewPhaseFromSpeed },
    { "DcCalcStartPhase", 0x00A0FB70, (uintptr_t)detour_DcCalcStartPhase },
    { "DcCalcStartPhaseMaxFrames", 0x00A0FAF0, (uintptr_t)detour_DcCalcStartPhaseMaxFrames },
    { "DcCalcStartPhaseRange", 0x00A0FA70, (uintptr_t)detour_DcCalcStartPhaseRange },
    { "DcMakeCharacterMoreWet", 0x00A023C0, (uintptr_t)detour_DcMakeCharacterMoreWet },
    { "DcMakeActiveCharacterMoreWet", 0x00A020E0, (uintptr_t)detour_DcMakeActiveCharacterMoreWet },
    { "DcMakeVector", 0x009FDCF0, (uintptr_t)detour_DcMakeVector },
    { "DcMakeVector3", 0x009FDC20, (uintptr_t)detour_DcMakeVector3 },
    { "DcDebugDrawLine", 0x009FE090, (uintptr_t)detour_DcDebugDrawLine },
    { "DcDebugDrawCross", 0x009FE070, (uintptr_t)detour_DcDebugDrawCross },
    { "DcDebugDrawLocator", 0x009FE000, (uintptr_t)detour_DcDebugDrawLocator },
    { "DcDebugDrawString", 0x009FDFB0, (uintptr_t)detour_DcDebugDrawString },
    { "DcAddImportModule", 0x00A0FF10, (uintptr_t)detour_DcAddImportModule },
    { "DcAddLoadModule", 0x00A0FE90, (uintptr_t)detour_DcAddLoadModule },
    { "DcAddDynamicLight", 0x009FF2C0, (uintptr_t)detour_DcAddDynamicLight },
    /* Lot 18: Tank & Rail Vehicle Combat Systems (26 funcs) */
    { "DcTankMoveTo", 0x00AA33E0, (uintptr_t)detour_DcTankMoveTo },
    { "DcTankReverseTo", 0x00AA2F80, (uintptr_t)detour_DcTankReverseTo },
    { "DcTankFireAtPos", 0x00AA2F30, (uintptr_t)detour_DcTankFireAtPos },
    { "DcTankFireAtObj", 0x00AA2EE0, (uintptr_t)detour_DcTankFireAtObj },
    { "DcTankAimAtPos", 0x00AA2E90, (uintptr_t)detour_DcTankAimAtPos },
    { "DcTankAimAtPosInstant", 0x00AA2B10, (uintptr_t)detour_DcTankAimAtPosInstant },
    { "DcTankAimAtObj", 0x00AA2AC0, (uintptr_t)detour_DcTankAimAtObj },
    { "DcTankAimForward", 0x00AA2A70, (uintptr_t)detour_DcTankAimForward },
    { "DcTankFire", 0x00AA2930, (uintptr_t)detour_DcTankFire },
    { "DcTankMgFireAtObj", 0x00AA2680, (uintptr_t)detour_DcTankMgFireAtObj },
    { "DcTankMgFireAtLoc", 0x00AA2120, (uintptr_t)detour_DcTankMgFireAtLoc },
    { "DcTankMgCeaseFire", 0x00AA2000, (uintptr_t)detour_DcTankMgCeaseFire },
    { "DcTankTeleport", 0x00AA1BA0, (uintptr_t)detour_DcTankTeleport },
    { "DcTankMoveTrackingObject", 0x00AA1550, (uintptr_t)detour_DcTankMoveTrackingObject },
    { "DcTankDistToPoint", 0x00AA1300, (uintptr_t)detour_DcTankDistToPoint },
    { "DcRailEnableTruckJump", 0x00AB9BB0, (uintptr_t)detour_DcRailEnableTruckJump },
    { "DcRailSpawn", 0x00AB9920, (uintptr_t)detour_DcRailSpawn },
    { "DcRailDeactivate", 0x00AB97D0, (uintptr_t)detour_DcRailDeactivate },
    { "DcRailEnableTributary", 0x00AB96B0, (uintptr_t)detour_DcRailEnableTributary },
    { "DcRailSetNpcArchetype", 0x00AB9660, (uintptr_t)detour_DcRailSetNpcArchetype },
    { "DcRailSetAcceleration", 0x00AB9590, (uintptr_t)detour_DcRailSetAcceleration },
    { "DcRailSetFollowDistance", 0x00AB8560, (uintptr_t)detour_DcRailSetFollowDistance },
    { "DcRailSetMoverFollowDistance", 0x00AB7FD0, (uintptr_t)detour_DcRailSetMoverFollowDistance },
    { "DcRailSetMoverIntialDistance", 0x00AB7E80, (uintptr_t)detour_DcRailSetMoverIntialDistance },
    { "DcRailGetMoverLocator", 0x00AB7D00, (uintptr_t)detour_DcRailGetMoverLocator },
    { "DcRailSetAllVehiclesToMoverSpeed", 0x00AB7BF0, (uintptr_t)detour_DcRailSetAllVehiclesToMoverSpeed },
    /* Lot 19 (Part 3): Engine Wait Scheduler & System Enables (23 funcs) */
    { "DcWaitAnimate", 0x003969A0, (uintptr_t)detour_DcWaitAnimate },
    { "DcEnableMessage", 0x005DC8E0, (uintptr_t)detour_DcEnableMessage },
    { "DcDisableMessage", 0x005DC990, (uintptr_t)detour_DcDisableMessage },
    { "DcEnableActionPack", 0x00A0A060, (uintptr_t)detour_DcEnableActionPack },
    { "DcWaitSpawnEntity", 0x00A09E90, (uintptr_t)detour_DcWaitSpawnEntity },
    { "DcWaitSpawnEntityAt", 0x00A09DC0, (uintptr_t)detour_DcWaitSpawnEntityAt },
    { "DcEnableNavBlockerObject", 0x00A04F60, (uintptr_t)detour_DcEnableNavBlockerObject },
    { "DcDisableNavBlockerObject", 0x00A04EF0, (uintptr_t)detour_DcDisableNavBlockerObject },
    { "DcDisablePlatformControlOnObject", 0x00A04E80, (uintptr_t)detour_DcDisablePlatformControlOnObject },
    { "DcDisableGadgetry", 0x00A03A70, (uintptr_t)detour_DcDisableGadgetry },
    { "DcEnableGadgetry", 0x00A039C0, (uintptr_t)detour_DcEnableGadgetry },
    { "DcDisableFrameForward", 0x00A038A0, (uintptr_t)detour_DcDisableFrameForward },
    { "DcWaitInt32", 0x00A41D40, (uintptr_t)detour_DcWaitInt32 },
    { "DcWaitBoolean", 0x00A41610, (uintptr_t)detour_DcWaitBoolean },
    { "DcWait", 0x00A3E690, (uintptr_t)detour_DcWait },
    { "DcWaitFrames", 0x00A41270, (uintptr_t)detour_DcWaitFrames },
    { "DcWaitOneFrame", 0x00A40F70, (uintptr_t)detour_DcWaitOneFrame },
    { "DcWaitUntil", 0x00A40E60, (uintptr_t)detour_DcWaitUntil },
    { "DcWaitUntilFrame", 0x00A40D40, (uintptr_t)detour_DcWaitUntilFrame },
    { "DcWaitUntilNetTime", 0x00A40870, (uintptr_t)detour_DcWaitUntilNetTime },
    { "DcWaitFor", 0x00A3FC50, (uintptr_t)detour_DcWaitFor },
    { "DcWaitTankAimAtObj", 0x00AA3A40, (uintptr_t)detour_DcWaitTankAimAtObj },
    { "DcWaitTankAimForward", 0x00AA3430, (uintptr_t)detour_DcWaitTankAimForward },
    /* Lot 20 (Part 4): Scene Shaders, Water, Weather, Lighting & Renders (32 funcs) */
    { "DcSetAnimationSpeed", 0x00397880, (uintptr_t)detour_DcSetAnimationSpeed },
    { "DcSetLookatStopPhase", 0x00397320, (uintptr_t)detour_DcSetLookatStopPhase },
    { "DcSetTableShaderMaterialValuesParam", 0x009F3B70, (uintptr_t)detour_DcSetTableShaderMaterialValuesParam },
    { "DcSetShaderMaterialParam", 0x00A10DD0, (uintptr_t)detour_DcSetShaderMaterialParam },
    { "DcSetSingleShaderInstanceParam", 0x00A10A50, (uintptr_t)detour_DcSetSingleShaderInstanceParam },
    { "DcSetShaderInstanceParam", 0x00A107B0, (uintptr_t)detour_DcSetShaderInstanceParam },
    { "DcSetBlackFade", 0x00A10700, (uintptr_t)detour_DcSetBlackFade },
    { "DcSetWhiteFade", 0x00A106A0, (uintptr_t)detour_DcSetWhiteFade },
    { "DcSetFocusableIgnore", 0x00A0F3C0, (uintptr_t)detour_DcSetFocusableIgnore },
    { "DcSetFocusableGrappled", 0x00A0F2C0, (uintptr_t)detour_DcSetFocusableGrappled },
    { "DcSetClockMultiplier", 0x00A0EE60, (uintptr_t)detour_DcSetClockMultiplier },
    { "DcSetVisible", 0x00A0BD30, (uintptr_t)detour_DcSetVisible },
    { "DcSetMeshVisible", 0x00A0BA20, (uintptr_t)detour_DcSetMeshVisible },
    { "DcSetObjectHealth", 0x00A04A20, (uintptr_t)detour_DcSetObjectHealth },
    { "DcSetCollisionLayer", 0x00A03CD0, (uintptr_t)detour_DcSetCollisionLayer },
    { "DcSetWaterOffset", 0x00A02FF0, (uintptr_t)detour_DcSetWaterOffset },
    { "DcSetWaterState", 0x00A02E20, (uintptr_t)detour_DcSetWaterState },
    { "DcSetDeformationOfWaterUnit", 0x00A02CC0, (uintptr_t)detour_DcSetDeformationOfWaterUnit },
    { "DcSetRainState", 0x00A02830, (uintptr_t)detour_DcSetRainState },
    { "DcSetRainRipplesState", 0x00A027F0, (uintptr_t)detour_DcSetRainRipplesState },
    { "DcSetTimeToDryPeriod", 0x00A02780, (uintptr_t)detour_DcSetTimeToDryPeriod },
    { "DcSetCharacterWetMaskValue", 0x00A026E0, (uintptr_t)detour_DcSetCharacterWetMaskValue },
    { "DcSetCharacterWetMaskState", 0x00A02690, (uintptr_t)detour_DcSetCharacterWetMaskState },
    { "DcSetCharacterWetMask", 0x00A02550, (uintptr_t)detour_DcSetCharacterWetMask },
    { "DcSetMaxCharacterWetness", 0x00A02510, (uintptr_t)detour_DcSetMaxCharacterWetness },
    { "DcSetRenderSettings", 0x009FFF80, (uintptr_t)detour_DcSetRenderSettings },
    { "DcSetLightFov", 0x009FFC80, (uintptr_t)detour_DcSetLightFov },
    { "DcSetDynamicLightColor", 0x009FEE20, (uintptr_t)detour_DcSetDynamicLightColor },
    { "DcSetCurrentBySpawner", 0x009FD710, (uintptr_t)detour_DcSetCurrentBySpawner },
    { "DcSetRagdollFlow", 0x009FD5C0, (uintptr_t)detour_DcSetRagdollFlow },
    { "DcSetObjectFaction", 0x009FCF40, (uintptr_t)detour_DcSetObjectFaction },
    { "DcSetIceCaveStupidGearsHack", 0x009FCC50, (uintptr_t)detour_DcSetIceCaveStupidGearsHack },
    /* Lot 21 (Part 3): Scene Object, Locator, Joint & Physics Getters (27 funcs) */
    { "DcGetAnimationPhase", 0x003976C0, (uintptr_t)detour_DcGetAnimationPhase },
    { "DcGetObjectPosition", 0x00A12610, (uintptr_t)detour_DcGetObjectPosition },
    { "DcGetObjectOffsetPosition", 0x00A12390, (uintptr_t)detour_DcGetObjectOffsetPosition },
    { "DcGetObjectOffsetLocator", 0x00A121E0, (uintptr_t)detour_DcGetObjectOffsetLocator },
    { "DcGetJointPosition", 0x00A120C0, (uintptr_t)detour_DcGetJointPosition },
    { "DcGetQuaternionPitch", 0x009F37C0, (uintptr_t)detour_DcGetQuaternionPitch },
    { "DcGetLocatorDistance", 0x00A0EF50, (uintptr_t)detour_DcGetLocatorDistance },
    { "DcGetObjectId", 0x00A0BEE0, (uintptr_t)detour_DcGetObjectId },
    { "DcGetSpawnerPosition", 0x00A094B0, (uintptr_t)detour_DcGetSpawnerPosition },
    { "DcGetSpawnerLocator", 0x00A09330, (uintptr_t)detour_DcGetSpawnerLocator },
    { "DcGetSpawnerLocatorProjected", 0x00A08D10, (uintptr_t)detour_DcGetSpawnerLocatorProjected },
    { "DcGetSpawnerLocatorRadial", 0x00A088C0, (uintptr_t)detour_DcGetSpawnerLocatorRadial },
    { "DcGetObjectLocator", 0x00A08630, (uintptr_t)detour_DcGetObjectLocator },
    { "DcGetObjectLocatorRadial", 0x00A081A0, (uintptr_t)detour_DcGetObjectLocatorRadial },
    { "DcGetJointLocator", 0x00A07E80, (uintptr_t)detour_DcGetJointLocator },
    { "DcGetApReferenceLocator", 0x00A07990, (uintptr_t)detour_DcGetApReferenceLocator },
    { "DcGetApReferenceEffectLocator", 0x00A07340, (uintptr_t)detour_DcGetApReferenceEffectLocator },
    { "DcGetApReferenceLocatorRadial", 0x00A06C20, (uintptr_t)detour_DcGetApReferenceLocatorRadial },
    { "DcGetAlignLocator", 0x00A066F0, (uintptr_t)detour_DcGetAlignLocator },
    { "DcGetJointAngleDiffFromBindpose", 0x00A06180, (uintptr_t)detour_DcGetJointAngleDiffFromBindpose },
    { "DcGetActionPackLocator", 0x00A060C0, (uintptr_t)detour_DcGetActionPackLocator },
    { "DcGetOriginLocator", 0x00A05F60, (uintptr_t)detour_DcGetOriginLocator },
    { "DcGetObjectState", 0x00A04C10, (uintptr_t)detour_DcGetObjectState },
    { "DcGetObjectHealth", 0x00A04B10, (uintptr_t)detour_DcGetObjectHealth },
    { "DcGetWaterOffset", 0x00A02F20, (uintptr_t)detour_DcGetWaterOffset },
    { "DcGetWaterState", 0x00A02D90, (uintptr_t)detour_DcGetWaterState },
    { "DcGetDda", 0x009FD010, (uintptr_t)detour_DcGetDda },
    /* Lot 22 (Part 4): Scene Spawners, Entities, Ropes & Nav Polys (27 funcs) */
    { "DcEntityAutoSpawnOnce", 0x00A12830, (uintptr_t)detour_DcEntityAutoSpawnOnce },
    { "DcObjectSpawnedP", 0x00A12780, (uintptr_t)detour_DcObjectSpawnedP },
    { "DcBattleSelectBestSpawnerFromGroup", 0x00A11190, (uintptr_t)detour_DcBattleSelectBestSpawnerFromGroup },
    { "DcSelectBestSchemaSpawner", 0x00A10EB0, (uintptr_t)detour_DcSelectBestSchemaSpawner },
    { "DcObjectInRegionP", 0x00A0FCA0, (uintptr_t)detour_DcObjectInRegionP },
    { "DcSpawnAttachable", 0x00A0F540, (uintptr_t)detour_DcSpawnAttachable },
    { "DcSlaForceTackle", 0x00A0F4A0, (uintptr_t)detour_DcSlaForceTackle },
    { "DcCreateWeaponDrawables", 0x00A0C410, (uintptr_t)detour_DcCreateWeaponDrawables },
    { "DcEmitBulletFromSpawner", 0x00A0BF40, (uintptr_t)detour_DcEmitBulletFromSpawner },
    { "DcForceStealthOff", 0x00A0BC40, (uintptr_t)detour_DcForceStealthOff },
    { "DcStickySpawner", 0x00A0B8F0, (uintptr_t)detour_DcStickySpawner },
    { "DcUnstickySpawner", 0x00A0B7C0, (uintptr_t)detour_DcUnstickySpawner },
    { "DcAttachFlashlight", 0x00A0A4C0, (uintptr_t)detour_DcAttachFlashlight },
    { "DcKillFlashlight", 0x00A0A130, (uintptr_t)detour_DcKillFlashlight },
    { "DcSpawnEntity", 0x00A09F40, (uintptr_t)detour_DcSpawnEntity },
    { "DcSpawnEntityAt", 0x009F3900, (uintptr_t)detour_DcSpawnEntityAt },
    { "DcKillEntity", 0x00A09D60, (uintptr_t)detour_DcKillEntity },
    { "DcKillDroppedWeaponsInSphere", 0x00A09B10, (uintptr_t)detour_DcKillDroppedWeaponsInSphere },
    { "DcKillPartSpawner", 0x00A099B0, (uintptr_t)detour_DcKillPartSpawner },
    { "DcSwapEntity", 0x00A09590, (uintptr_t)detour_DcSwapEntity },
    { "DcBlockNavPolyUnderObject", 0x00A05030, (uintptr_t)detour_DcBlockNavPolyUnderObject },
    { "DcUnblockNavPolyUnderObject", 0x00A04FC0, (uintptr_t)detour_DcUnblockNavPolyUnderObject },
    { "DcRegenerateCoverActionPacksFromObject", 0x00A04E20, (uintptr_t)detour_DcRegenerateCoverActionPacksFromObject },
    { "DcRemoveCoverActionPacksFromObject", 0x00A04DC0, (uintptr_t)detour_DcRemoveCoverActionPacksFromObject },
    { "DcRenameObject", 0x00A04D10, (uintptr_t)detour_DcRenameObject },
    { "DcGrabRope", 0x00A04740, (uintptr_t)detour_DcGrabRope },
    { "DcDropRope", 0x00A045E0, (uintptr_t)detour_DcDropRope },
};

size_t u2_get_scene_detours(const U2NativeDetour **out) {
    if (out) *out = s_scene_detours;
    return sizeof(s_scene_detours) / sizeof(s_scene_detours[0]);
}
