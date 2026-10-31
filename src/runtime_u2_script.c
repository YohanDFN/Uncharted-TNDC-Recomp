/* Uncharted 2: Among Thieves (big2-ps4_Shipping.elf)
 * Native C Reimplementation & Hook Layer for Naughty Dog's DC00 Script Engine:
 *   - d:/dev/CP9/big2-ps4/src/game/script-manager.cpp:
 *       ScriptManager::ExecuteScriptCode (guest offset 0x00A14120)
 *       ScriptManager::EvalEx            (guest offset 0x00A15430)
 *       ScriptManager::Run               (guest offset 0x00A14F20 / 0x00A150E0)
 *       ScriptManager::Initialize        (guest offset 0x00A173A0)
 *   - d:/dev/CP9/big2-ps4/src/game/script-funcs.cpp:
 *       Dc* native C++ functions (int64_t ABI (*)(int32_t argc, uint64_t *argv, uint32_t *status))
 */
#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

/* ============================================================================
 * 1. Naughty Dog DC00 VM Structures (reconstructed from 0x00A14120)
 * ============================================================================ */

typedef struct U2ScriptCode {
    const uint8_t  *bytecode;   /* +0x00: [opcode, dst, src1, src2] * N */
    const uint64_t *constants;  /* +0x08: 64-bit constant pool */
} U2ScriptCode;

typedef struct U2ScriptStackFrame {
    uint32_t                   pc;             /* +0x00: instruction index (or next free frame ptr low) */
    uint32_t                   _pad04;         /* +0x04 */
    const U2ScriptCode        *code;           /* +0x08: script code header */
    uint32_t                   argc;           /* +0x10: argument count passed to this frame */
    uint32_t                   _pad14;         /* +0x14 */
    struct U2ScriptStackFrame *caller;         /* +0x18: parent stack frame (NULL at root) */
    uint32_t                   return_dst_reg; /* +0x20: caller register for return value (0xF8EEF8EE when free) */
    uint32_t                   _pad24;         /* +0x24 */
    uint64_t                   regs[64];       /* +0x28: register file (regs[24..39] at +0xE8 = call args) */
} U2ScriptStackFrame;

/* Native C++ function signature invoked by opcode 0x1C (CALL_NATIVE at 0x00A1451A):
 *   rdi = argc (src2)
 *   rsi = &args[0] (copied from caller regs[24..39], i.e. r15 + 0xC0)
 *   rdx = &status (uint32_t *, 0 = normal, 1 = abort/error, 2 = yield/suspend)
 */
typedef int64_t (ABI *U2DcNativeFn)(int32_t argc, uint64_t *argv, uint32_t *out_status);

/* Helpers to convert 32-bit float bits <-> float in 64-bit VM registers */
static inline float u2_reg_to_f32(uint64_t r) {
    uint32_t u = (uint32_t)r;
    float f;
    memcpy(&f, &u, sizeof(f));
    return f;
}

static inline uint64_t u2_f32_to_reg(float f) {
    uint32_t u;
    memcpy(&u, &f, sizeof(u));
    return (uint64_t)u;
}

/* ============================================================================
 * 2. StringId & Native Dc* Function Reimplementations (d:/dev/CP9/big2-ps4/src/game/script-funcs.cpp)
 * ============================================================================ */

static uint8_t *g_u2_image_base = NULL;

/* Naughty Dog StringId CRC-32 (d:/dev/CP9/shared-ps4/src/ndlib/util/stringid.cpp) */
uint32_t runtime_u2_string_id(const char *str) {
    uint32_t crc = 0xFFFFFFFFu;
    if (!str) return 0;
    for (const unsigned char *p = (const unsigned char *)str; *p; ++p) {
        crc ^= (uint32_t)(*p);
        for (int b = 0; b < 8; ++b) {
            if (crc & 1u)
                crc = (crc >> 1) ^ 0xEDB88320u;
            else
                crc >>= 1;
        }
    }
    return crc;
}

int64_t runtime_u2_call_dc_native(uint64_t image, uint64_t offset, int32_t argc, uint64_t *argv) {
    uint32_t status = 0;
    U2DcNativeFn fn = (U2DcNativeFn)(uintptr_t)(image + offset);
    return fn(argc, argv, &status);
}

int64_t ABI u2_DcNot(int32_t argc, uint64_t *argv, uint64_t image) {
    /* 0x00A12DF0: not (hash 0xD3521DC2) - compares argv[0] against symbol '#f */
    const uint64_t sym_false = *(const uint64_t *)(uintptr_t)(image + 0x01105EE8ull);
    return (argc > 0 && argv[0] == sym_false) ? 1 : 0;
}

int64_t ABI u2_DcEntityName(int32_t argc, uint64_t *argv) {
    /* 0x00A127D0: entity-name (hash 0x24B6345C) - mov rax, [rsi]; mov eax, [rax+0x28]; ret */
    if (argc <= 0 || !argv || !argv[0]) return 0;
    return *(const uint32_t *)(uintptr_t)(argv[0] + 0x28u);
}

int64_t ABI u2_DcGetLanguage(int32_t argc, uint64_t *argv, uint64_t image) {
    /* 0x00A0EC90: get-language (hash 0x62808C92) */
    (void)argc; (void)argv;
    return *(const int32_t *)(uintptr_t)(image + 0x01092488ull);
}

int64_t ABI u2_DcAnimStateAdvancePhase(int32_t argc, uint64_t *argv) {
    /* 0x00397690: anim-state-advance-phase (hash 0x2481D93E) - xor eax, eax; ret */
    (void)argc; (void)argv;
    return 0;
}

/* --- Direct SysV-ABI Detour Replacements for Uncharted 2 Dc* Natives --- */

static int64_t ABI detour_DcNoOp0(int32_t argc, uint64_t *argv, uint32_t *status) {
    (void)argc; (void)argv; (void)status;
    return 0;
}

static __attribute__((unused)) int64_t ABI detour_DcNot(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A12DF0: (not x) */
    (void)status;
    const uint64_t sym_false = *(const uint64_t *)(g_u2_image_base + 0x01105EE8ull);
    return (argc > 0 && argv[0] == sym_false) ? 1 : 0;
}

static int64_t ABI detour_DcEntityName(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A127D0: (entity-name ent) */
    (void)status;
    if (argc <= 0 || !argv || !argv[0]) return 0;
    return *(const uint32_t *)(uintptr_t)(argv[0] + 0x28u);
}

static __attribute__((unused)) int64_t ABI detour_DcGetLanguage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0EC90: (get-language) */
    (void)argc; (void)argv; (void)status;
    return *(const int32_t *)(g_u2_image_base + 0x01092488ull);
}

static int64_t ABI detour_DcSymbolEqualP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00397620: (symbol-equal? a b) */
    (void)argc; (void)status;
    return ((uint32_t)argv[0] == (uint32_t)argv[1]) ? 1 : 0;
}

static int64_t ABI detour_DcFromUps(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x003976A0: (from-ups val dt) */
    (void)argc; (void)status;
    return (int64_t)u2_f32_to_reg(u2_reg_to_f32(argv[0]) * u2_reg_to_f32(argv[1]));
}

static int64_t ABI detour_DcMinmax(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDBF0: (minmax val min_val max_val) */
    (void)argc; (void)status;
    float v   = u2_reg_to_f32(argv[0]);
    float lo  = u2_reg_to_f32(argv[1]);
    float hi  = u2_reg_to_f32(argv[2]);
    if (v < lo) v = lo;
    if (v > hi) v = hi;
    return (int64_t)u2_f32_to_reg(v);
}

static int64_t ABI detour_DcNpcMaxCount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DEFF0: (npc-max-count) -> 15 */
    (void)argc; (void)argv; (void)status;
    return 15;
}

static int64_t ABI detour_DcNpcAliveCount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DF000: (npc-alive-count) */
    (void)argc; (void)argv; (void)status;
    return (int64_t)(*(const int32_t *)(g_u2_image_base + 0x0110DCB8ull));
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

static int64_t ABI detour_DcForcePlayerSwim(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D740: (force-player-swim flag) */
    (void)argc; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x0104F8ECull) = ((uint32_t)argv[0] == 1u) ? 1u : 0u;
    return 0;
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

static int64_t ABI detour_DcPlayerInitiateMelee(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00909420: (player-initiate-melee) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x011579E1ull) = 1u;
    return 0;
}

static int64_t ABI detour_DcGetPlayerTeachComboTriangle(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DCDE0: (get-player-teach-combo-triangle) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x011579BEull);
}

static int64_t ABI detour_DcGetPlayerTeachComboSquare(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DCDF0: (get-player-teach-combo-square) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x011579BFull);
}

static int64_t ABI detour_DcGroundPrintsDisable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01EB0: (ground-prints-disable) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x01117FF8ull) = 0u;
    return 1;
}

static int64_t ABI detour_DcGroundPrintsEnable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01ED0: (ground-prints-enable) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x01117FF8ull) = 1u;
    return 1;
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

/* ============================================================================
 * 2b. Wave 2 Reimplemented Dc* Functions (Player, HUD, NPC, Rail, Audio, Math)
 * ============================================================================ */

/* Helper: Resolve active player object from global handle at image + 0x0115BD20 */
static inline uint8_t *u2_get_active_player(void) {
    uint8_t *handle = g_u2_image_base + 0x0115BD20ull;
    uint8_t **slot = *(uint8_t ***)(handle + 0x00);
    if (!slot) return NULL;
    uint32_t id = *(const uint32_t *)(handle + 0x08);
    if (!id) return NULL;
    uint8_t *player = *slot;
    if (!player || *(const uint32_t *)(player + 0x44) != id) return NULL;
    return player;
}

static int64_t ABI detour_DcActivePlayerCount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00667F40: (active-player-count) */
    (void)argc; (void)argv; (void)status;
    typedef int32_t (ABI *fn_t)(void *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x0063FFB0ull);
    return (int64_t)fn(g_u2_image_base + 0x0113B760ull);
}

static int64_t ABI detour_DcAlivePlayerCount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00667F20: (alive-player-count) */
    (void)argc; (void)argv; (void)status;
    typedef int32_t (ABI *fn_t)(void *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00642A80ull);
    return (int64_t)fn(g_u2_image_base + 0x0113B760ull);
}

static int64_t ABI detour_DcAllEnemyNpcsDie(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DE9D0: (all-enemy-npcs-die) */
    (void)argc; (void)argv; (void)status;
    typedef void (ABI *fn_t)(void *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00232190ull);
    fn(g_u2_image_base + 0x0110DCB8ull);
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

static int64_t ABI detour_DcNpcMoveTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E1C30: (npc-move-to ...) */
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, int32_t, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x002E1600ull);
    return fn(argc, argv, 0, status);
}

static int64_t ABI detour_DcNpcMoveToSpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E15E0: (npc-move-to-spawner ...) */
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, int32_t, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x002E0F20ull);
    return fn(argc, argv, 0, status);
}

static int64_t ABI detour_DcNpcMarkAsDefunct(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E4F60: (npc-mark-as-defunct npc_id) */
    (void)status;
    if (argc <= 0) return 0;
    uint32_t id = (uint32_t)argv[0];
    if (!id) return 0;
    typedef uint8_t *(ABI *get_npc_fn)(void *, uint32_t);
    get_npc_fn get_npc = (get_npc_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    uint8_t *npc = get_npc(g_u2_image_base + 0x0110DCB8ull, id);
    if (!npc && *(const int32_t *)(g_u2_image_base + 0x014CE97Cull) >= 2) {
        typedef uint8_t *(ABI *get_inst_fn)(uint64_t, uint32_t);
        get_inst_fn get_inst = (get_inst_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        uint8_t *inst = get_inst(0, id);
        if (inst && (*(const uint16_t *)(inst + 0x36) & 0x4u)) {
            npc = inst;
        }
    }
    if (npc) {
        *(uint8_t *)(npc + 0x262Au) |= 0x20u;
    }
    return 0;
}

static int64_t ABI detour_DcPlayerClimbingUpP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090C3D0: (player-climbing-up?) */
    (void)argc; (void)argv; (void)status;
    uint8_t *player = u2_get_active_player();
    if (!player) return 0;
    typedef uint8_t (ABI *fn_t)(void *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x009788E0ull);
    return (int64_t)fn(player);
}

static int64_t ABI detour_DcPlayerRemoveWeapons(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00907170: (player-remove-weapons) */
    (void)argc; (void)argv; (void)status;
    uint8_t *player = u2_get_active_player();
    if (player) {
        uint8_t *inv = *(uint8_t **)(player + 0x2768);
        if (inv) {
            uint8_t *vt = *(uint8_t **)inv;
            typedef void (ABI *fn_t)(void *);
            fn_t fn = *(fn_t *)(vt + 0x110);
            fn(inv);
        }
    }
    return 0;
}

static int64_t ABI detour_DcPlayerUnparent(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090F5F0: (player-unparent) */
    (void)argc; (void)argv; (void)status;
    uint8_t *player = u2_get_active_player();
    if (player) {
        uint8_t *vt = *(uint8_t **)player;
        typedef void (ABI *fn_t)(void *, uint64_t);
        fn_t fn = *(fn_t *)(vt + 0x198);
        fn(player, 0);
    }
    return 0;
}

static int64_t ABI detour_DcPlayerTeachCombo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DCDB0: (player-teach-combo) */
    (void)argc; (void)argv; (void)status;
    if (*(const uint32_t *)(g_u2_image_base + 0x01059640ull + 0x48B0ull) != 0u) {
        *(uint8_t *)(g_u2_image_base + 0x01139EA0ull + 0x118ull) = 1u;
    }
    return 0;
}

static int64_t ABI detour_DcGetPlayerTimeInCombat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DCD60: (get-player-time-in-combat) */
    (void)argc; (void)argv; (void)status;
    if (!g_u2_image_base) return 0;
    int32_t in_combat = *(const int32_t *)(g_u2_image_base + 0x0153E194ull);
    if (in_combat != 1) return 0;
    uint32_t cur_ticks   = *(const uint32_t *)(g_u2_image_base + 0x01116180ull + 0x44);
    uint32_t start_ticks = *(const uint32_t *)(g_u2_image_base + 0x0153E190ull);
    float tick_scale     = *(const float *)(g_u2_image_base + 0x00E62140ull);
    float dt = (float)(cur_ticks - start_ticks) * tick_scale;
    return (int64_t)u2_f32_to_reg(dt);
}

static int64_t ABI detour_DcGetPlayerTimeSinceCover(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DCCE0: (get-player-time-since-cover) */
    (void)argc; (void)argv; (void)status;
    if (!g_u2_image_base) return 0;
    uint8_t *slot = g_u2_image_base + 0x0115BD20ull;
    uint8_t **pp_player = *(uint8_t ***)slot;
    if (!pp_player || !*pp_player) return 0;
    uint32_t tag = *(const uint32_t *)(slot + 8);
    if (!tag) return 0;
    uint8_t *player = *pp_player;
    if (*(const uint32_t *)(player + 0x44) != tag) return 0;
    uint8_t **pp_mgr = *(uint8_t ***)(g_u2_image_base + 0x01161EE8ull);
    if (!pp_mgr || !*pp_mgr) return 0;
    uint8_t *sub = *(uint8_t **)(*pp_mgr + 0x70);
    if (!sub) return 0;
    uint32_t cur_ticks   = *(const uint32_t *)(sub + 0x0C);
    uint32_t cover_ticks = *(const uint32_t *)(player + 0x2A50);
    float tick_scale     = *(const float *)(g_u2_image_base + 0x00E6213Cull);
    float dt = (float)(cur_ticks - cover_ticks) * tick_scale;
    return (int64_t)u2_f32_to_reg(dt);
}

static int64_t ABI detour_DcPowFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDB60: (pow-float base exp) */
    (void)argc; (void)status;
    float b = u2_reg_to_f32(argv[0]);
    float e = u2_reg_to_f32(argv[1]);
    return (int64_t)u2_f32_to_reg(powf(b, e));
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

static int64_t ABI detour_DcRailEnableNpcShooting(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8E40: (rail-enable-npc-shooting [flag]) */
    (void)status;
    *(uint8_t *)(g_u2_image_base + 0x011644B8ull) = (argc > 0 && argv[0] == 0) ? 1u : 0u;
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

static int64_t ABI detour_DcSetPlayerHealth(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009100F0: (set-player-health hp) */
    (void)argc; (void)status;
    uint8_t *player = u2_get_active_player();
    if (player) {
        uint8_t *comp = *(uint8_t **)(player + 0x1F0);
        if (comp) {
            uint8_t *vt = *(uint8_t **)comp;
            typedef void (ABI *fn_t)(void *, uint32_t);
            fn_t fn = *(fn_t *)(vt + 0x20);
            fn(comp, (uint32_t)argv[0]);
        }
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

static int64_t ABI detour_DcSyncAudioMetaData(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD560: (sync-audio-meta-data) */
    (void)argc; (void)argv; (void)status;
    uint8_t *audio = g_u2_image_base + 0x01111750ull;
    if (*(const uint64_t *)(audio + 0x8) != 0) {
        typedef void (ABI *fn_t)(void *);
        fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x003B7F50ull);
        fn(audio);
    }
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

/* ============================================================================
 * 2b. Wave 3 & 4 Native C Dc* Functions (Variables, Tags, Entités, Caméra, Math)
 * ============================================================================ */

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

/* ============================================================================
 * 2c. Wave 5 Native C Subsystems (Units, Clock, Camera, Weapon & Player State)
 * ============================================================================ */

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

static int64_t ABI detour_DcFromUpsNoDt(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD6B0: (from-ups-no-dt speed) */
    if (status) *status = 0;
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    float x = u2_reg_to_f32(argv[0]);
    const uint8_t *clk_mgr_slot = *(const uint8_t **)(g_u2_image_base + 0x01161EE8ull);
    if (clk_mgr_slot) {
        const uint8_t *clk = *(const uint8_t **)(clk_mgr_slot + 0x70);
        if (clk) {
            float factor = *(const float *)(clk + 0x14);
            return (int64_t)u2_f32_to_reg(x * factor);
        }
    }
    return (int64_t)u2_f32_to_reg(x);
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

static int64_t ABI detour_DcPlayerHasWeaponP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00906A50: (player-has-weapon? weapon-type) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return 0;
    const uint8_t *drake = *(const uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return 0;
    const uint32_t target_weapon_type = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    int32_t count = *(const int32_t *)(drake + 0x4BC);
    if (count <= 0) return 0;
    for (int32_t i = 0; i < count; ++i) {
        const uint8_t *entry_slot = drake + 0x4C8 + (uint64_t)i * 16u;
        const uint8_t *weapon_ptr = *(const uint8_t **)entry_slot;
        const uint32_t weapon_tag = *(const uint32_t *)(entry_slot + 8);
        if (!weapon_ptr || weapon_tag == 0) continue;
        const uint8_t *weapon = *(const uint8_t **)weapon_ptr;
        if (!weapon || *(const uint32_t *)(weapon + 0x44) != weapon_tag) continue;
        if (*(const uint32_t *)(weapon + 0x550) == target_weapon_type) {
            return 1;
        }
    }
    return 0;
}

static int64_t ABI detour_DcPlayerIsAimingP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090BC00: (player-is-aiming?) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return 0;
    const uint8_t *drake = *(const uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return 0;
    /* Check aim camera override slot */
    const uint8_t *aim_cam_slot = *(const uint8_t **)(g_u2_image_base + 0x0147F020ull);
    const uint32_t aim_tag = *(const uint32_t *)(g_u2_image_base + 0x0147F020ull + 8);
    if (aim_cam_slot && aim_tag != 0) {
        const uint8_t *cam = *(const uint8_t **)aim_cam_slot;
        if (cam && *(const uint32_t *)(cam + 0x44) == aim_tag && *(const int32_t *)(cam + 0x7C) == 9) {
            return 1;
        }
    }
    /* Check active cameras list */
    const uint8_t *list_head = g_u2_image_base + 0x0147F050ull;
    const uint8_t *curr = *(const uint8_t **)list_head;
    while (curr && curr != list_head) {
        if (!(*(curr + 0x34) & 1)) {
            if (*(const int32_t *)(curr + 0x30) == -1) return 1;
        }
        curr = *(const uint8_t **)curr;
    }
    if (*(const int32_t *)(g_u2_image_base + 0x0147F0B4ull) == 9) return 1;
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

/* ============================================================================
 * 2d. Wave 6 Native C Subsystems (Events, Train Level Messaging & Cover/Weapon State)
 * ============================================================================ */

static int64_t ABI detour_DcPlayerInCoverP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090BBB0: (player-in-cover?) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return 0;
    const uint8_t *drake = *(const uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return 0;
    const uint8_t *state_p50 = *(const uint8_t **)(drake + 0x50);
    if (!state_p50) return 0;
    const uint8_t *state = *(const uint8_t **)state_p50;
    if (!state) return 0;
    const uint8_t *comp = *(const uint8_t **)(state + 0x10);
    if (comp && *(const uint32_t *)(comp + 0x18) == 0x48C615D8u) {
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcPlayerHideThisFrame(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090E330: (player-hide-this-frame) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return 0;
    uint8_t *drake = *(uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return 0;
    drake[0x2A60] = 0;
    return 0;
}

static int64_t ABI detour_DcPlayerHasWeaponAndAmmoP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00907290: (player-has-weapon-and-ammo? weapon-type) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return 0;
    const uint8_t *drake = *(const uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return 0;
    const uint32_t target_weapon_type = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    int32_t count = *(const int32_t *)(drake + 0x4BC);
    if (count <= 0) return 0;
    for (int32_t i = 0; i < count; ++i) {
        const uint8_t *entry_slot = drake + 0x4C8 + (uint64_t)i * 16u;
        const uint8_t *weapon_ptr = *(const uint8_t **)entry_slot;
        const uint32_t weapon_tag = *(const uint32_t *)(entry_slot + 8);
        if (!weapon_ptr || weapon_tag == 0) continue;
        const uint8_t *weapon = *(const uint8_t **)weapon_ptr;
        if (!weapon || *(const uint32_t *)(weapon + 0x44) != weapon_tag) continue;
        if (*(const uint32_t *)(weapon + 0x550) == target_weapon_type) {
            int16_t clip = *(const int16_t *)(weapon + 0x620);
            int16_t reserve = *(const int16_t *)(weapon + 0x622);
            return (clip + reserve > 0) ? 1 : 0;
        }
    }
    return 0;
}

static int64_t ABI detour_DcPlayerHasWeaponAndNoAmmoP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009071B0: (player-has-weapon-and-no-ammo? weapon-type) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return 0;
    const uint8_t *drake = *(const uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return 0;
    const uint32_t target_weapon_type = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    int32_t count = *(const int32_t *)(drake + 0x4BC);
    if (count <= 0) return 0;
    for (int32_t i = 0; i < count; ++i) {
        const uint8_t *entry_slot = drake + 0x4C8 + (uint64_t)i * 16u;
        const uint8_t *weapon_ptr = *(const uint8_t **)entry_slot;
        const uint32_t weapon_tag = *(const uint32_t *)(entry_slot + 8);
        if (!weapon_ptr || weapon_tag == 0) continue;
        const uint8_t *weapon = *(const uint8_t **)weapon_ptr;
        if (!weapon || *(const uint32_t *)(weapon + 0x44) != weapon_tag) continue;
        if (*(const uint32_t *)(weapon + 0x550) == target_weapon_type) {
            int16_t clip = *(const int16_t *)(weapon + 0x620);
            int16_t reserve = *(const int16_t *)(weapon + 0x622);
            return (clip + reserve == 0) ? 1 : 0;
        }
    }
    return 0;
}

static int64_t ABI detour_DcPlayerOnJetskiP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01B80: (player-on-jetski?) - shipping stub returning 0 */
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

/* ============================================================================
 * Wave 7 Detours: Event Arguments & Returns, Train Climbing & Player Status
 * ============================================================================ */

static int64_t ABI detour_DcEventArgBoolean(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C2A40: (event-arg-boolean [arg-idx]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    const uint8_t *frame = *(const uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    int32_t idx = (argc > 0 && argv) ? (int32_t)argv[0] : 0;
    int8_t num_args = *(const int8_t *)(frame + 0x20);
    if (idx < 0 || idx >= (int32_t)num_args) return 0;
    const uint8_t *slot = frame + 0x28 + (uint64_t)idx * 24u;
    return (*slot != 0) ? 1 : 0;
}

static int64_t ABI detour_DcEventArgInt32(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C2BE0: (event-arg-int32 [arg-idx]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    const uint8_t *frame = *(const uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    int32_t idx = (argc > 0 && argv) ? (int32_t)argv[0] : 0;
    int8_t num_args = *(const int8_t *)(frame + 0x20);
    if (idx < 0 || idx >= (int32_t)num_args) return 0;
    const uint8_t *slot = frame + 0x28 + (uint64_t)idx * 24u;
    return (int64_t)*(const int32_t *)slot;
}

static int64_t ABI detour_DcEventArgFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C2BF0: (event-arg-float [arg-idx]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    const uint8_t *frame = *(const uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    int32_t idx = (argc > 0 && argv) ? (int32_t)argv[0] : 0;
    int8_t num_args = *(const int8_t *)(frame + 0x20);
    if (idx < 0 || idx >= (int32_t)num_args) return 0;
    const uint8_t *slot = frame + 0x28 + (uint64_t)idx * 24u;
    float fval = *(const float *)slot;
    return (int64_t)u2_f32_to_reg(fval);
}

static int64_t ABI detour_DcEventArgSymbol(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C2DA0: (event-arg-symbol [arg-idx]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    const uint8_t *frame = *(const uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    int32_t idx = (argc > 0 && argv) ? (int32_t)argv[0] : 0;
    int8_t num_args = *(const int8_t *)(frame + 0x20);
    if (idx < 0 || idx >= (int32_t)num_args) return 0;
    const uint8_t *slot = frame + 0x28 + (uint64_t)idx * 24u;
    return (int64_t)*(const uint32_t *)slot;
}

static int64_t ABI detour_DcEventArgString(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C2EC0: (event-arg-string [arg-idx]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    const uint8_t *frame = *(const uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    int32_t idx = (argc > 0 && argv) ? (int32_t)argv[0] : 0;
    int8_t num_args = *(const int8_t *)(frame + 0x20);
    if (idx < 0 || idx >= (int32_t)num_args) return 0;
    const uint8_t *slot = frame + 0x28 + (uint64_t)idx * 24u;
    return (int64_t)*(const uint64_t *)slot;
}

static int64_t ABI detour_DcEventReturnBoolean(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C1FF0: (event-return-boolean val) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    uint8_t *ret_buf = *(uint8_t **)(event_mgr + 0x08 + (uint64_t)depth * 32u);
    if (!ret_buf) return 0;
    uint8_t val = (argc > 0 && argv && argv[0] != 0) ? 1 : 0;
    *(uint8_t *)(ret_buf + 0x627) = val;
    *(uint32_t *)(ret_buf + 0x628) = 1;
    *(uint8_t *)(ret_buf + 0x638) |= 0x01;
    return 0;
}

static int64_t ABI detour_DcEventReturnInt32(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C2390: (event-return-int32 val) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    uint8_t *ret_buf = *(uint8_t **)(event_mgr + 0x08 + (uint64_t)depth * 32u);
    if (!ret_buf) return 0;
    int32_t val = (argc > 0 && argv) ? (int32_t)argv[0] : 0;
    *(int32_t *)(ret_buf + 0x624) = val;
    *(uint32_t *)(ret_buf + 0x628) = 2;
    *(uint8_t *)(ret_buf + 0x638) |= 0x02;
    return 0;
}

static int64_t ABI detour_DcEventReturnUint32(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C21D0: (event-return-uint32 val) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    uint8_t *ret_buf = *(uint8_t **)(event_mgr + 0x08 + (uint64_t)depth * 32u);
    if (!ret_buf) return 0;
    uint32_t val = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    *(uint32_t *)(ret_buf + 0x624) = val;
    *(uint32_t *)(ret_buf + 0x628) = 2;
    *(uint8_t *)(ret_buf + 0x638) |= 0x02;
    return 0;
}

static int64_t ABI detour_DcEventReturnFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C2550: (event-return-float val) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    uint8_t *ret_buf = *(uint8_t **)(event_mgr + 0x08 + (uint64_t)depth * 32u);
    if (!ret_buf) return 0;
    float val = (argc > 0 && argv) ? u2_reg_to_f32(argv[0]) : 0.0f;
    *(float *)(ret_buf + 0x620) = val;
    *(uint32_t *)(ret_buf + 0x628) = 3;
    *(uint8_t *)(ret_buf + 0x638) |= 0x04;
    return 0;
}

static int64_t ABI detour_DcEventReturnSymbol(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C2720: (event-return-symbol val) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    uint8_t *ret_buf = *(uint8_t **)(event_mgr + 0x08 + (uint64_t)depth * 32u);
    if (!ret_buf) return 0;
    uint32_t val = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    *(uint32_t *)(ret_buf + 0x624) = val;
    *(uint32_t *)(ret_buf + 0x628) = 4;
    *(uint8_t *)(ret_buf + 0x638) |= 0x08;
    return 0;
}

static int64_t ABI detour_DcPlayerUnderTrainP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090A660: (player-under-train?) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return 0;
    const uint8_t *drake = *(const uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return 0;
    const uint8_t *parent_slot = *(const uint8_t **)(drake + 0xD8);
    const uint32_t parent_tag = *(const uint32_t *)(drake + 0xE0);
    if (!parent_slot || parent_tag == 0) return 0;
    const uint8_t *parent = *(const uint8_t **)parent_slot;
    if (!parent || *(const uint32_t *)(parent + 0x50) != parent_tag) return 0;
    /* Train entity type tag is 0xDFDDF753 */
    if (*(const uint32_t *)(parent + 0x30) != 0xDFDDF753u) return 0;
    /* Check vertical offset between Drake and train body */
    float drake_z = *(const float *)(drake + 0x58);
    float train_z = *(const float *)(parent + 0x58);
    return (drake_z < train_z) ? 1 : 0;
}

static int64_t ABI detour_DcGetPlayerHealth(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00910140: (get-player-health) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 100;
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return 100;
    const uint8_t *drake = *(const uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return 100;
    const uint8_t *health_comp = *(const uint8_t **)(drake + 0x1F0);
    if (!health_comp) return 100;
    typedef int32_t (ABI *HealthFn)(const void *);
    const void *vtable = *(const void **)health_comp;
    if (!vtable) return 100;
    HealthFn fn = *(HealthFn *)((const uint8_t *)vtable + 0x48);
    if (!fn) return 100;
    return (int64_t)fn(health_comp);
}

static int64_t ABI detour_DcGetPlayerSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00910020: (get-player-speed) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return (int64_t)u2_f32_to_reg(0.0f);
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return (int64_t)u2_f32_to_reg(0.0f);
    const uint8_t *drake = *(const uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return (int64_t)u2_f32_to_reg(0.0f);
    float vx = *(const float *)(drake + 0x140);
    float vy = *(const float *)(drake + 0x144);
    float vz = *(const float *)(drake + 0x148);
    float speed = sqrtf(vx * vx + vy * vy + vz * vz);
    return (int64_t)u2_f32_to_reg(speed);
}

static int64_t ABI detour_DcPlayerInCombatP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090DC20: (player-in-combat?) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    if (*(const uint8_t *)(g_u2_image_base + 0x011640B1ull) != 0) return 1;
    if (*(const int32_t *)(g_u2_image_base + 0x014CE97Cull) < 2) {
        return (*(const int32_t *)(g_u2_image_base + 0x0153E194ull) == 1) ? 1 : 0;
    }
    const uint8_t *cm = g_u2_image_base + 0x0113B760ull;
    if (*(const uint8_t *)(cm + 0x21C8) == 0) return 0;
    return 1;
}

static int64_t ABI detour_DcPlayerHangingOnBarP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090AB80: (player-hanging-on-bar?) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return 0;
    const uint8_t *drake = *(const uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return 0;
    const uint8_t *bar_comp = *(const uint8_t **)(drake + 0x2918);
    if (!bar_comp) return 0;
    typedef uint8_t (ABI *BarCheckFn)(const void *);
    const void *vtable = *(const void **)bar_comp;
    if (!vtable) return 0;
    BarCheckFn fn = *(BarCheckFn *)((const uint8_t *)vtable + 0xA8);
    if (!fn) return 0;
    return (int64_t)fn(bar_comp);
}

/* ============================================================================
 * Wave 8 Detours: Camera Backward Motion, NPC Life Status & Weapon Inventory
 * ============================================================================ */

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

static int64_t ABI detour_DcNpcAliveP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DD900: (npc-alive? [npc-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) {
        npc_id = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    if (npc_id == 0) return 0;
    typedef void * (ABI *GetNpcFn)(void *, uint32_t);
    GetNpcFn get_npc = (GetNpcFn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_mgr = (void *)(g_u2_image_base + 0x0110DCB8ull);
    void *npc = get_npc ? get_npc(npc_mgr, npc_id) : NULL;
    if (!npc) {
        if (*(const int32_t *)(g_u2_image_base + 0x014CE97Cull) >= 2) {
            typedef void * (ABI *GetEntityFn)(uint32_t);
            GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
            npc = get_ent ? get_ent(npc_id) : NULL;
            if (npc) {
                uint32_t flags = (uint32_t)*(const uint8_t *)((const uint8_t *)npc + 0x36) << 16;
                if ((flags & 0x40000) == 0) return 0;
            }
        }
        if (!npc) return 0;
    }
    typedef uint8_t (ABI *IsDeadFn)(void *);
    const void *vtable = *(const void **)npc;
    if (!vtable) return 0;
    IsDeadFn is_dead = *(IsDeadFn *)((const uint8_t *)vtable + 0x250);
    if (!is_dead) return 0;
    return is_dead(npc) ? 0 : 1;
}

static int64_t ABI detour_DcNpcKilledP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DD990: (npc-killed? [npc-id]) */
    int64_t alive = detour_DcNpcAliveP(argc, argv, status);
    return alive ? 0 : 1;
}

static int64_t ABI detour_DcNpcSetCanDie(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DD410: (npc-set-can-die [npc-id] can-die?) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    uint8_t can_die = 0;
    if (argc >= 2 && argv) {
        npc_id = (uint32_t)argv[0];
        can_die = (argv[1] != 0) ? 1 : 0;
    } else if (argc == 1 && argv) {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
        can_die = (argv[0] != 0) ? 1 : 0;
    }
    if (npc_id == 0) return 0;
    typedef void * (ABI *GetNpcFn)(void *, uint32_t);
    GetNpcFn get_npc = (GetNpcFn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_mgr = (void *)(g_u2_image_base + 0x0110DCB8ull);
    void *npc = get_npc ? get_npc(npc_mgr, npc_id) : NULL;
    if (!npc && *(const int32_t *)(g_u2_image_base + 0x014CE97Cull) >= 2) {
        typedef void * (ABI *GetEntityFn)(uint32_t);
        GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        npc = get_ent ? get_ent(npc_id) : NULL;
    }
    if (!npc) return 0;
    const uint8_t *health_comp = *(const uint8_t **)((const uint8_t *)npc + 0x1F0);
    if (!health_comp) return 0;
    typedef void (ABI *SetCanDieFn)(const void *, uint8_t);
    const void *vtable = *(const void **)health_comp;
    if (!vtable) return 0;
    SetCanDieFn fn = *(SetCanDieFn *)((const uint8_t *)vtable + 0x98);
    if (fn) fn(health_comp, can_die);
    return 0;
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

/* ============================================================================
 * Wave 9 Detours: Ammo Clip Ratio, Audio Rain/Speech & NPC Combat Controls
 * ============================================================================ */

static int64_t ABI detour_DcGetPlayerClipPercentFull(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DCE00: (get-player-clip-percent-full) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return (int64_t)u2_f32_to_reg(0.0f);
    const uint8_t *drake_slot = *(const uint8_t **)(g_u2_image_base + 0x0115BD20ull);
    const uint32_t expected_tag = *(const uint32_t *)(g_u2_image_base + 0x0115BD20ull + 8);
    if (!drake_slot || expected_tag == 0) return (int64_t)u2_f32_to_reg(0.0f);
    const uint8_t *drake = *(const uint8_t **)drake_slot;
    if (!drake || *(const uint32_t *)(drake + 0x44) != expected_tag) return (int64_t)u2_f32_to_reg(0.0f);
    int32_t cur_weapon_idx = *(const int32_t *)(drake + 0x4B8);
    int32_t weapon_count = *(const int32_t *)(drake + 0x4BC);
    if (cur_weapon_idx < 0 || cur_weapon_idx >= weapon_count) return (int64_t)u2_f32_to_reg(0.0f);
    const uint8_t *entry_slot = drake + 0x4C8 + (uint64_t)cur_weapon_idx * 16u;
    const uint8_t *weapon_ptr = *(const uint8_t **)entry_slot;
    const uint32_t weapon_tag = *(const uint32_t *)(entry_slot + 8);
    if (!weapon_ptr || weapon_tag == 0) return (int64_t)u2_f32_to_reg(0.0f);
    const uint8_t *weapon = *(const uint8_t **)weapon_ptr;
    if (!weapon || *(const uint32_t *)(weapon + 0x44) != weapon_tag) return (int64_t)u2_f32_to_reg(0.0f);
    int16_t clip = *(const int16_t *)(weapon + 0x620);
    typedef int32_t (ABI *GetMaxClipFn)(const void *, int32_t);
    GetMaxClipFn get_max = (GetMaxClipFn)(uintptr_t)(g_u2_image_base + 0x00AE47D0ull);
    int32_t max_clip = get_max ? get_max(weapon, 0) : 0;
    float pct = (max_clip > 0) ? ((float)clip / (float)max_clip) : 0.0f;
    return (int64_t)u2_f32_to_reg(pct);
}

static int64_t ABI detour_DcUseAudioRainLevel(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A009F0: (use-audio-rain-level level) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    uint32_t level = (uint32_t)argv[0];
    uint8_t *audio_mgr = g_u2_image_base + 0x01111750ull;
    *(uint32_t *)(audio_mgr + 0x594) = level;
    return 1;
}

static int64_t ABI detour_DcSuppressIdleSpeech(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01360: (suppress-idle-speech duration-sec) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    float dur = u2_reg_to_f32(argv[0]);
    float scale = *(const float *)(g_u2_image_base + 0x00E672F4ull);
    int64_t frames = (int64_t)(dur * scale);
    if (frames < 1) frames = 1;
    int64_t *p_cur = (int64_t *)(g_u2_image_base + 0x0147E5C8ull);
    if (frames > *p_cur) *p_cur = frames;
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

static int64_t ABI detour_DcNpcDie(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E4680: (npc-die [npc-id] [reason]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) {
        npc_id = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    if (npc_id == 0) return 0;
    typedef void * (ABI *GetNpcFn)(void *, uint32_t);
    GetNpcFn get_npc = (GetNpcFn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_mgr = (void *)(g_u2_image_base + 0x0110DCB8ull);
    void *npc = get_npc ? get_npc(npc_mgr, npc_id) : NULL;
    if (!npc && *(const int32_t *)(g_u2_image_base + 0x014CE97Cull) >= 2) {
        typedef void * (ABI *GetEntityFn)(uint32_t);
        GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        npc = get_ent ? get_ent(npc_id) : NULL;
    }
    if (!npc) return 0;
    const uint8_t *health_comp = *(const uint8_t **)((const uint8_t *)npc + 0x1F0);
    if (!health_comp) return 0;
    typedef void (ABI *KillFn)(const void *, int32_t, int32_t);
    const void *vtable = *(const void **)health_comp;
    if (!vtable) return 0;
    KillFn fn = *(KillFn *)((const uint8_t *)vtable + 0x50);
    if (fn) fn(health_comp, 0, 1);
    return 0;
}

static int64_t ABI detour_DcNpcEnableShooting(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC680: (npc-enable-shooting [npc-id] enable?) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    uint8_t enable = 1;
    if (argc >= 2 && argv) {
        npc_id = (uint32_t)argv[0];
        enable = (argv[1] != 0) ? 1 : 0;
    } else if (argc == 1 && argv) {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
        enable = (argv[0] != 0) ? 1 : 0;
    }
    if (npc_id == 0) return 0;
    typedef void * (ABI *GetNpcFn)(void *, uint32_t);
    GetNpcFn get_npc = (GetNpcFn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_mgr = (void *)(g_u2_image_base + 0x0110DCB8ull);
    void *npc = get_npc ? get_npc(npc_mgr, npc_id) : NULL;
    if (!npc && *(const int32_t *)(g_u2_image_base + 0x014CE97Cull) >= 2) {
        typedef void * (ABI *GetEntityFn)(uint32_t);
        GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        npc = get_ent ? get_ent(npc_id) : NULL;
    }
    if (!npc) return 0;
    uint8_t *shoot_comp = *(uint8_t **)((uint8_t *)npc + 0x3FA0);
    if (!shoot_comp) return 0;
    if (enable) {
        *(uint8_t *)(shoot_comp + 0x110) |= 0x01;
    } else {
        *(uint8_t *)(shoot_comp + 0x110) &= ~0x01;
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetShotDamageMultiplier(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC570: (npc-set-shot-damage-multiplier [npc-id] mult) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    float mult = 1.0f;
    if (argc >= 2 && argv) {
        npc_id = (uint32_t)argv[0];
        mult = u2_reg_to_f32(argv[1]);
    } else if (argc == 1 && argv) {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
        mult = u2_reg_to_f32(argv[0]);
    }
    if (npc_id == 0) return 0;
    typedef void * (ABI *GetNpcFn)(void *, uint32_t);
    GetNpcFn get_npc = (GetNpcFn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_mgr = (void *)(g_u2_image_base + 0x0110DCB8ull);
    void *npc = get_npc ? get_npc(npc_mgr, npc_id) : NULL;
    if (!npc && *(const int32_t *)(g_u2_image_base + 0x014CE97Cull) >= 2) {
        typedef void * (ABI *GetEntityFn)(uint32_t);
        GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        npc = get_ent ? get_ent(npc_id) : NULL;
    }
    if (!npc) return 0;
    uint8_t *damage_comp = *(uint8_t **)((uint8_t *)npc + 0x3FA0);
    if (!damage_comp) return 0;
    *(float *)(damage_comp + 0x118) = mult;
    return 0;
}

static int64_t ABI detour_DcCacheSoundBank(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD540: (cache-sound-bank bank-name) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    const void *bank_name = (const void *)(uintptr_t)argv[0];
    if (!bank_name) return 0;
    typedef void (ABI *CacheFn)(void *, const void *, int32_t, int32_t);
    CacheFn fn = (CacheFn)(uintptr_t)(g_u2_image_base + 0x003C7FA0ull);
    void *audio_mgr = (void *)(g_u2_image_base + 0x01111750ull);
    if (fn) fn(audio_mgr, bank_name, 2, 0);
    return 0;
}

static int64_t ABI detour_DcCacheSoundStream(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD510: (cache-sound-stream stream-name) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    const void *stream_name = (const void *)(uintptr_t)argv[0];
    if (!stream_name) return 0;
    typedef void (ABI *CacheFn)(void *, const void *, int32_t, int32_t);
    CacheFn fn = (CacheFn)(uintptr_t)(g_u2_image_base + 0x003C7FA0ull);
    void *audio_mgr = (void *)(g_u2_image_base + 0x01111750ull);
    if (fn) fn(audio_mgr, stream_name, 3, 0);
    return 0;
}

static int64_t ABI detour_DcCacheSpeechStream(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD4E0: (cache-speech-stream speech-name) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    const void *speech_name = (const void *)(uintptr_t)argv[0];
    if (!speech_name) return 0;
    typedef void (ABI *CacheFn)(void *, const void *, int32_t, int32_t);
    CacheFn fn = (CacheFn)(uintptr_t)(g_u2_image_base + 0x003C7FA0ull);
    void *audio_mgr = (void *)(g_u2_image_base + 0x01111750ull);
    if (fn) fn(audio_mgr, speech_name, 5, 0);
    return 0;
}

static int64_t ABI detour_DcCacheMusicPlaylist(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD460: (cache-music-playlist playlist-name) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    if (sym == 0) return 0;
    typedef void * (ABI *EvalFn)(uint32_t);
    EvalFn eval_sym = (EvalFn)(uintptr_t)(g_u2_image_base + 0x00A15300ull);
    const uint8_t *obj = eval_sym ? (const uint8_t *)eval_sym(sym) : NULL;
    if (!obj) return 0;
    const uint8_t *pl = *(const uint8_t **)obj;
    if (!pl) return 0;
    int32_t count = *(const int32_t *)(pl + 0xA8);
    if (count <= 0) return 0;
    const uint8_t *items = *(const uint8_t **)(pl + 0xB0);
    if (!items) return 0;
    typedef void (ABI *CacheFn)(void *, const void *, int32_t, int32_t);
    CacheFn fn = (CacheFn)(uintptr_t)(g_u2_image_base + 0x003C7FA0ull);
    void *audio_mgr = (void *)(g_u2_image_base + 0x01111750ull);
    if (!fn) return 0;
    for (int32_t i = 0; i < count; ++i) {
        const void *name = *(const void **)(items + (size_t)i * 16);
        if (name) fn(audio_mgr, name, 4, 0);
    }
    return 0;
}

/* ============================================================================
 * 3. Opcode Table (80 opcodes: 0x00 .. 0x4F at jump table 0x00A14DA4)
 * ============================================================================ */

static const char *u2_opcode_name(uint8_t op) {
    static const char *const names[80] = {
        /* 0x00 */ "RET",         "ADD_INT",     "SUB_INT",     "MUL_INT",
        /* 0x04 */ "DIV_INT",     "ADD_FLOAT",   "SUB_FLOAT",   "MUL_FLOAT",
        /* 0x08 */ "DIV_FLOAT",   "LOAD_POOL",   "SHL_INT",     "SHR_INT",
        /* 0x0C */ "LOAD_IMM16",  "LOAD_U64",    "LOAD_U32",    "LOAD_PTR",
        /* 0x10 */ "STORE_32",    "STORE_U32",   "STORE_64",    "STORE_8",
        /* 0x14 */ "STORE_16",    "LOAD_SYM",    "MOV",         "MOV_ALT1",
        /* 0x18 */ "MOV_ALT2",    "FTOI",        "ITOF",        "CALL_DEFUN",
        /* 0x1C */ "CALL_NATIVE", "CMP_EQ",      "CMP_GT",      "CMP_GE",
        /* 0x20 */ "CMP_LT",      "CMP_LE",      "FCMP_EQ",     "FCMP_GT",
        /* 0x24 */ "FCMP_GE",     "FCMP_LT",     "FCMP_LE",     "MOD_INT",
        /* 0x28 */ "CALL_HELPER", "ABS_INT",     "ABS_FLOAT",   "MIN_FLOAT",
        /* 0x2C */ "MAX_FLOAT",   "JMP",         "JNZ",         "JZ",
        /* 0x30 */ "NOT",         "AND_BIT",     "NOT_BIT",     "OR_BIT",
        /* 0x34 */ "XOR_BIT",     "NOR_BIT",     "AND_LOGIC",   "OR_LOGIC",
        /* 0x38 */ "NEG_INT",     "NEG_FLOAT",   "GET_ARGC",    "ADD_IMM",
        /* 0x3C */ "SUB_IMM",     "MUL_IMM",     "DIV_IMM",     "LOAD_I32",
        /* 0x40 */ "LOAD_F32",    "LOAD_I64",    "LOAD_SYM_C",  "LOAD_I8_C",
        /* 0x44 */ "LOAD_U8_C",   "LOAD_I16",    "LOAD_U16",    "ASSERT_47",
        /* 0x48 */ "ASSERT_48",   "LOAD_M_I8",   "LOAD_M_U8",   "LOAD_M_I16",
        /* 0x4C */ "LOAD_M_U16",  "LOAD_M_I32",  "ASSERT_4E",   "ASSERT_4F"
    };
    return op < 80 ? names[op] : "OP_UNKNOWN";
}

/* ============================================================================
 * 4. Complete C Implementation of ScriptManager::ExecuteScriptCode (0x00A14120)
 * ============================================================================ */

static void u2_free_frame(U2ScriptStackFrame *frame, uint64_t image) {
    uint32_t *p_depth = (uint32_t *)(uintptr_t)(image + 0x015341F0ull);
    U2ScriptStackFrame **p_free_list = (U2ScriptStackFrame **)(uintptr_t)(image + 0x015341E8ull);
    (*p_depth)--;
    frame->return_dst_reg = 0xF8EEF8EEu;
    *(U2ScriptStackFrame **)frame = *p_free_list;
    *p_free_list = frame;
}

int64_t runtime_u2_vm_execute(U2ScriptStackFrame *frame, uint64_t *out_result, uint64_t image) {
    if (!frame || !frame->code) {
        if (out_result) *out_result = 0;
        return 0;
    }

    const uint8_t  *r12 = frame->code->bytecode;
    const uint64_t *r8  = frame->code->constants;
    uint64_t       *r15 = frame->regs;

    for (;;) {
        const uint32_t pc   = frame->pc;
        const uint8_t  op   = r12[pc * 4 + 0];
        const uint8_t  dst  = r12[pc * 4 + 1];
        const uint8_t  src1 = r12[pc * 4 + 2];
        const uint8_t  src2 = r12[pc * 4 + 3];
        frame->pc = pc + 1;

        switch (op) {
        case 0x00: { /* RET (0x00A14231) */
            U2ScriptStackFrame *caller = frame->caller;
            uint64_t ret_val = r15[src1];
            if (caller != NULL) {
                /* Return to caller frame (0x00A14170) */
                uint32_t ret_dst = frame->return_dst_reg;
                caller->regs[ret_dst] = ret_val;
                u2_free_frame(frame, image);
                frame = caller;
                r12   = frame->code->bytecode;
                r8    = frame->code->constants;
                r15   = frame->regs;
                break;
            }
            /* Return from root frame (0x00A14CAE) */
            if (out_result) *out_result = ret_val;
            u2_free_frame(frame, image);
            return 0;
        }

        case 0x01: /* ADD_INT (0x00A1423B) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] + (int64_t)r15[src2]);
            break;

        case 0x02: /* SUB_INT (0x00A1424C) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] - (int64_t)r15[src2]);
            break;

        case 0x03: /* MUL_INT (0x00A1425D) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] * (int64_t)r15[src2]);
            break;

        case 0x04: /* DIV_INT (0x00A1426F) */
            r15[dst] = r15[src2] ? (uint64_t)((int64_t)r15[src1] / (int64_t)r15[src2]) : 0;
            break;

        case 0x05: /* ADD_FLOAT (0x00A14282) */
            r15[dst] = u2_f32_to_reg(u2_reg_to_f32(r15[src1]) + u2_reg_to_f32(r15[src2]));
            break;

        case 0x06: /* SUB_FLOAT (0x00A14299) */
            r15[dst] = u2_f32_to_reg(u2_reg_to_f32(r15[src1]) - u2_reg_to_f32(r15[src2]));
            break;

        case 0x07: /* MUL_FLOAT (0x00A142B0) */
            r15[dst] = u2_f32_to_reg(u2_reg_to_f32(r15[src1]) * u2_reg_to_f32(r15[src2]));
            break;

        case 0x08: /* DIV_FLOAT (0x00A142C7) */
            r15[dst] = u2_f32_to_reg(u2_reg_to_f32(r15[src1]) / u2_reg_to_f32(r15[src2]));
            break;

        case 0x09: /* LOAD_POOL (0x00A141DB) */
        case 0x0A:
        case 0x0B:
            r15[dst] = r8[r15[src1]];
            break;

        case 0x0C: /* LOAD_IMM16 (0x00A142DE) */
            r15[dst] = r8[(uint64_t)src1];
            break;

        case 0x0D: /* LOAD_U64 (0x00A142EE) */
        case 0x0F: /* LOAD_PTR (0x00A14310) */
            r15[dst] = *(const uint64_t *)(uintptr_t)r15[src1];
            break;

        case 0x0E: /* LOAD_U32 (0x00A142FD) */
            r15[dst] = (uint64_t)(*(const uint32_t *)(uintptr_t)r15[src1]);
            break;

        case 0x10: /* STORE_32 (0x00A14320) */
        case 0x11: /* STORE_U32 (0x00A14335) */ {
            uint32_t v = (uint32_t)r15[src2];
            *(uint32_t *)(uintptr_t)r15[src1] = v;
            r15[dst] = (uint64_t)v;
            break;
        }

        case 0x12: /* STORE_64 (0x00A1434E) */ {
            uint64_t v = r15[src2];
            *(uint64_t *)(uintptr_t)r15[src1] = v;
            r15[dst] = v;
            break;
        }

        case 0x13: /* STORE_8 (0x00A1422A) */
        case 0x14: /* STORE_16 (0x00A1422A) */
            break;

        case 0x15: /* LOAD_SYM (0x00A14366) */ {
            uint64_t *sym_ptr = *(uint64_t **)(uintptr_t)(r8 + src1);
            r15[dst] = sym_ptr ? *sym_ptr : 0;
            break;
        }

        case 0x16: /* MOV (0x00A14220) */
        case 0x17: /* MOV_ALT1 (0x00A14220) */
        case 0x18: /* MOV_ALT2 (0x00A14220) */
            r15[dst] = r15[src1];
            break;

        case 0x19: /* FTOI (0x00A14389) */
            r15[dst] = (uint64_t)(int64_t)(int32_t)u2_reg_to_f32(r15[src1]);
            break;

        case 0x1A: /* ITOF (0x00A1439A) */
            r15[dst] = u2_f32_to_reg((float)(int64_t)r15[src1]);
            break;

        case 0x1B: { /* CALL_DEFUN (0x00A143AB) */
            const U2ScriptCode *callee_code = (const U2ScriptCode *)(uintptr_t)r15[src1];
            const uint64_t invalid_code = *(const uint64_t *)(uintptr_t)(image + 0x0104AEB0ull);
            if (!callee_code || (uint64_t)(uintptr_t)callee_code == invalid_code) {
                r15[dst] = 0;
                break;
            }
            U2ScriptStackFrame **p_free_list = (U2ScriptStackFrame **)(uintptr_t)(image + 0x015341E8ull);
            uint32_t *p_depth = (uint32_t *)(uintptr_t)(image + 0x015341F0ull);
            U2ScriptStackFrame *callee = *p_free_list;
            if (!callee) {
                /* Stack overflow (0x00A14D15) */
                if (out_result) *out_result = 0;
                return 0;
            }
            *p_free_list = *(U2ScriptStackFrame **)callee;
            (*p_depth)++;
            callee->caller = frame;
            /* Copy argc (src2) arguments from caller regs[24..] (r15 + 0xC0) to callee regs[24..] (callee + 0xE8) */
            for (uint32_t i = 0; i < (uint32_t)src2; ++i) {
                callee->regs[24 + i] = r15[24 + i];
            }
            callee->return_dst_reg = dst;
            callee->code           = callee_code;
            callee->argc           = src2;
            callee->pc             = 0;
            frame = callee;
            r12   = frame->code->bytecode;
            r8    = frame->code->constants;
            r15   = frame->regs;
            break;
        }

        case 0x1C: { /* CALL_NATIVE (0x00A1451A) */
            U2DcNativeFn fn = (U2DcNativeFn)(uintptr_t)r15[src1];
            if (!fn) break;
            uint64_t args[16];
            for (int i = 0; i < 16; ++i) {
                args[i] = r15[24 + i]; /* r15 + 0xC0 .. r15 + 0x138 */
            }
            uint32_t status = 0;
            int64_t res = fn((int32_t)src2, args, &status);
            r15[dst] = (uint64_t)res;
            /* Inlined ss-context.cpp::GetGroupInstance check (0x00A1463E..0x00A14714):
             * If the current StateScript group instance was destroyed during the native call,
             * set status = 1 to unwind the script stack cleanly. */
            const uint8_t *ss_ctx = (const uint8_t *)(uintptr_t)(image + 0x01050320ull);
            int64_t cur_stack_idx = *(const int32_t *)(ss_ctx + 0x200);
            if (cur_stack_idx >= 0 && cur_stack_idx < 16) {
                uintptr_t entry = *(const uintptr_t *)(ss_ctx + ((size_t)cur_stack_idx << 5) + 0x8);
                if (entry != 0) {
                    uintptr_t p0 = *(const uintptr_t *)entry;
                    uint32_t id  = *(const uint32_t *)(entry + 0x8);
                    uintptr_t p1 = p0 ? *(const uintptr_t *)p0 : 0;
                    int valid = (p0 != 0 && id != 0 && p1 != 0 && *(const uint32_t *)(p1 + 0x44) == id);
                    if (!valid && *(const uintptr_t *)(entry + 0x18) == 0) {
                        status = 1;
                    }
                }
            }
            if (status == 2) {
                if (out_result) *out_result = 0;
                return 0;
            }
            if (status == 1) {
                /* Unwind all active stack frames (0x00A14D54) */
                while (frame) {
                    U2ScriptStackFrame *parent = frame->caller;
                    u2_free_frame(frame, image);
                    frame = parent;
                }
                if (out_result) *out_result = 0;
                return 0;
            }
            break;
        }

        case 0x1D: /* CMP_EQ (0x00A1472F) */
            r15[dst] = (r15[src1] == r15[src2]) ? 1ull : 0ull;
            break;

        case 0x1E: /* CMP_GT (0x00A14746) */
            r15[dst] = ((int64_t)r15[src1] > (int64_t)r15[src2]) ? ~0ull : 0ull;
            break;

        case 0x1F: /* CMP_GE (0x00A14764) */
            r15[dst] = ((int64_t)r15[src1] >= (int64_t)r15[src2]) ? ~0ull : 0ull;
            break;

        case 0x20: /* CMP_LT (0x00A14782) */
            r15[dst] = ((int64_t)r15[src1] < (int64_t)r15[src2]) ? ~0ull : 0ull;
            break;

        case 0x21: /* CMP_LE (0x00A147A0) */
            r15[dst] = ((int64_t)r15[src1] <= (int64_t)r15[src2]) ? ~0ull : 0ull;
            break;

        case 0x22: /* FCMP_EQ (0x00A147BE) */
            r15[dst] = (fabsf(u2_reg_to_f32(r15[src1]) - u2_reg_to_f32(r15[src2])) <= 1e-6f) ? ~0ull : 0ull;
            break;

        case 0x23: /* FCMP_GT (0x00A147E2) */
            r15[dst] = (u2_reg_to_f32(r15[src1]) > u2_reg_to_f32(r15[src2])) ? ~0ull : 0ull;
            break;

        case 0x24: /* FCMP_GE (0x00A14804) */
            r15[dst] = (u2_reg_to_f32(r15[src1]) >= u2_reg_to_f32(r15[src2])) ? ~0ull : 0ull;
            break;

        case 0x25: /* FCMP_LT (0x00A14826) */
            r15[dst] = (u2_reg_to_f32(r15[src1]) < u2_reg_to_f32(r15[src2])) ? ~0ull : 0ull;
            break;

        case 0x26: /* FCMP_LE (0x00A14848) */
            r15[dst] = (u2_reg_to_f32(r15[src1]) <= u2_reg_to_f32(r15[src2])) ? ~0ull : 0ull;
            break;

        case 0x27: /* MOD_INT (0x00A1486A) */
            r15[dst] = r15[src2] ? (uint64_t)((int64_t)r15[src1] % (int64_t)r15[src2]) : 0;
            break;

        case 0x28: /* CALL_HELPER (0x00A1487D) */
            r15[dst] = (uint64_t)u2_f32_to_reg(fmodf(u2_reg_to_f32(r15[src1]), u2_reg_to_f32(r15[src2])));
            break;

        case 0x29: { /* ABS_INT (0x00A148A3) */
            int64_t v = (int64_t)r15[src1];
            r15[dst] = (uint64_t)(v < 0 ? -v : v);
            break;
        }

        case 0x2A: /* ABS_FLOAT (0x00A148BA) */
            r15[dst] = u2_f32_to_reg(fabsf(u2_reg_to_f32(r15[src1])));
            break;

        case 0x2E: /* JNZ (0x00A148D3) */
            if (r15[src1] != 0)
                frame->pc = ((uint32_t)src2 << 8) | (uint32_t)dst;
            break;

        case 0x2F: /* JZ (0x00A148E0) */
            if (r15[src1] == 0)
                frame->pc = ((uint32_t)src2 << 8) | (uint32_t)dst;
            break;

        case 0x2D: /* JMP (0x00A148EB) */
            frame->pc = ((uint32_t)src2 << 8) | (uint32_t)dst;
            break;

        case 0x30: /* NOT (0x00A148F9) */
            r15[dst] = (r15[src1] == 0) ? ~0ull : 0ull;
            break;

        case 0x31: /* AND_BIT (0x00A1490A) */
            r15[dst] = r15[src1] & r15[src2];
            break;

        case 0x32: /* NOT_BIT (0x00A1491B) */
            r15[dst] = ~r15[src1];
            break;

        case 0x33: /* OR_BIT (0x00A1492B) */
            r15[dst] = r15[src1] | r15[src2];
            break;

        case 0x34: /* XOR_BIT (0x00A1493C) */
            r15[dst] = r15[src1] ^ r15[src2];
            break;

        case 0x35: /* NOR_BIT (0x00A1494D) */
            r15[dst] = ~(r15[src1] | r15[src2]);
            break;

        case 0x36: /* AND_LOGIC (0x00A14961) */
            r15[dst] = (r15[src1] != 0 && r15[src2] != 0) ? ~0ull : 0ull;
            break;

        case 0x37: /* OR_LOGIC (0x00A14979) */
            r15[dst] = (r15[src1] != 0 || r15[src2] != 0) ? ~0ull : 0ull;
            break;

        case 0x38: /* NEG_INT (0x00A14992) */
            r15[dst] = (uint64_t)(-(int64_t)r15[src1]);
            break;

        case 0x39: /* NEG_FLOAT (0x00A149A1) */
            r15[dst] = u2_f32_to_reg(-u2_reg_to_f32(r15[src1]));
            break;

        case 0x3A: /* GET_ARGC (0x00A149BA) */
            r15[dst] = (uint64_t)(int64_t)(int32_t)frame->argc;
            break;

        case 0x3B: /* ADD_IMM (0x00A149C7) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] + (int64_t)src2);
            break;

        case 0x3C: /* SUB_IMM (0x00A149D7) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] - (int64_t)src2);
            break;

        case 0x3D: /* MUL_IMM (0x00A149E7) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] * (int64_t)src2);
            break;

        case 0x3E: /* DIV_IMM (0x00A149F8) */
            r15[dst] = src2 ? (uint64_t)((int64_t)r15[src1] / (int64_t)src2) : 0;
            break;

        case 0x3F: /* LOAD_I32 (0x00A14A0A) */
            r15[dst] = (uint64_t)(int64_t)(*(const int32_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x40: /* LOAD_F32 (0x00A14A17) */
        case 0x42: /* LOAD_SYM_C (0x00A14A28) */
            r15[dst] = (uint64_t)(*(const uint32_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x41: /* LOAD_I64 (0x00A141DF) */
            r15[dst] = r8[src1];
            break;

        case 0x43: /* LOAD_I8_C (0x00A14A35) */
            r15[dst] = (uint64_t)(int64_t)(*(const int8_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x44: /* LOAD_U8_C (0x00A14A43) */
            r15[dst] = (uint64_t)(*(const uint8_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x45: /* LOAD_I16 (0x00A14A51) */
            r15[dst] = (uint64_t)(int64_t)(*(const int16_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x46: /* LOAD_U16 (0x00A14A5F) */
            r15[dst] = (uint64_t)(*(const uint16_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x49: /* LOAD_M_I8 (0x00A14AED) */
            r15[dst] = (uint64_t)(int64_t)(*(const int8_t *)(uintptr_t)r15[src1]);
            break;

        case 0x4A: /* LOAD_M_U8 (0x00A14AFE) */
            r15[dst] = (uint64_t)(*(const uint8_t *)(uintptr_t)r15[src1]);
            break;

        case 0x4B: /* LOAD_M_I16 (0x00A14B0E) */
            r15[dst] = (uint64_t)(int64_t)(*(const int16_t *)(uintptr_t)r15[src1]);
            break;

        case 0x4C: /* LOAD_M_U16 (0x00A14B1F) */
            r15[dst] = (uint64_t)(*(const uint16_t *)(uintptr_t)r15[src1]);
            break;

        case 0x4D: /* LOAD_M_I32 (0x00A14B2F) */
            r15[dst] = (uint64_t)(int64_t)(*(const int32_t *)(uintptr_t)r15[src1]);
            break;

        default:
            break;
        }
    }
}

/* ============================================================================
 * 5. Runtime Hook Entrypoints
 * ============================================================================ */

static int u2_script_trace_enabled(void) {
    static int enabled = -1;
    if (enabled < 0) {
        const char *env = getenv("BB_U2_SCRIPT_TRACE");
        enabled = (env && *env && *env != '0') ? 1 : 0;
    }
    return enabled;
}

static uint64_t u2_exec_calls = 0;
static uint64_t u2_eval_calls = 0;

/* Hook callback invoked at the entry of ScriptManager::ExecuteScriptCode (image + 0x00A14120) */
void __attribute__((ms_abi)) runtime_u2_execute_script_code(
    uint64_t rdi, uint64_t rsi, uint64_t image, uint64_t r14
) {
    (void)r14;
    const uint64_t n = __atomic_add_fetch(&u2_exec_calls, 1, __ATOMIC_RELAXED);
    if (!u2_script_trace_enabled()) return;
    if (n <= 64 || (n & 0x3FFFull) == 0) {
        const U2ScriptStackFrame *frame = (const U2ScriptStackFrame *)(uintptr_t)rdi;
        uint8_t first_op = 0xFF;
        uint32_t pc = 0;
        if (frame && runtime_memory_is_mapped((uintptr_t)frame, sizeof(U2ScriptStackFrame))) {
            pc = frame->pc;
            if (frame->code && runtime_memory_is_mapped((uintptr_t)frame->code, sizeof(U2ScriptCode)) &&
                frame->code->bytecode && runtime_memory_is_mapped((uintptr_t)(frame->code->bytecode + pc * 4), 4)) {
                first_op = frame->code->bytecode[pc * 4];
            }
        }
        printf("[U2 ScriptVM] ExecuteScriptCode(#%" PRIu64 ") frame=%#" PRIx64 " pc=%u op=0x%02x (%s) out=%#" PRIx64 " image=%#" PRIx64 "\n",
               n, rdi, pc, first_op, u2_opcode_name(first_op), rsi, image);
    }
}

/* ============================================================================
 * 4b. Complete C Implementation of ScriptManager::EvalEx (0x00A15430)
 * ============================================================================ */

/* Stackless lightweight expression evaluator for animations, state transitions & AI */
int64_t ABI detour_EvalEx(const U2ScriptCode *code, int32_t argc, const uint64_t *args) {
    if (!code || !code->bytecode) return 0;

    /* 40 local registers initialized to 0 (matches rsp+32..rsp+352) */
    uint64_t regs[40];
    memset(regs, 0, sizeof(regs));

    /* Caller arguments are copied to regs[24..39] (&regs[24] = rsp + 224) */
    if (args && argc > 0) {
        int copy_count = argc < 16 ? argc : 16;
        memcpy(&regs[24], args, (size_t)copy_count * sizeof(uint64_t));
    }

    const uint64_t *constants = code->constants;
    const uint8_t *pc = code->bytecode;

    while (1) {
        uint8_t op   = pc[0];
        uint8_t dst  = pc[1];
        uint8_t src1 = pc[2];
        uint8_t src2 = pc[3];

        if (dst >= 40) dst = 0;

        switch (op) {
        case 0x00: /* RET (0x00A15D20) */
            return (src1 < 40) ? (int64_t)regs[src1] : 0;

        case 0x01: /* ADD_INT (0x00A15556) */
            regs[dst] = regs[src1] + regs[src2];
            break;

        case 0x02: /* SUB_INT (0x00A15565) */
            regs[dst] = regs[src1] - regs[src2];
            break;

        case 0x03: /* MUL_INT (0x00A1557A) */
            regs[dst] = (uint64_t)((int64_t)regs[src1] * (int64_t)regs[src2]);
            break;

        case 0x04: /* DIV_INT (0x00A1558A) */
            regs[dst] = regs[src2] ? (uint64_t)((int64_t)regs[src1] / (int64_t)regs[src2]) : 0;
            break;

        case 0x05: /* ADD_FLOAT (0x00A155A4) */
            regs[dst] = u2_f32_to_reg(u2_reg_to_f32(regs[src1]) + u2_reg_to_f32(regs[src2]));
            break;

        case 0x06: /* SUB_FLOAT (0x00A155C0) */
            regs[dst] = u2_f32_to_reg(u2_reg_to_f32(regs[src1]) - u2_reg_to_f32(regs[src2]));
            break;

        case 0x07: /* MUL_FLOAT (0x00A155DC) */
            regs[dst] = u2_f32_to_reg(u2_reg_to_f32(regs[src1]) * u2_reg_to_f32(regs[src2]));
            break;

        case 0x08: /* DIV_FLOAT (0x00A155F8) */
            regs[dst] = u2_f32_to_reg(u2_reg_to_f32(regs[src1]) / u2_reg_to_f32(regs[src2]));
            break;

        case 0x09: /* LOAD_POOL (0x00A15535) */
        case 0x0B: /* SHR_INT   (0x00A15535) */
        case 0x41: /* LOAD_I64  (0x00A1553A) */
            if (constants) regs[dst] = constants[src1];
            break;

        case 0x0C: /* LOAD_IMM16 (0x00A15632) */
            regs[dst] = ((uint64_t)src2 << 8) | (uint64_t)src1;
            break;

        case 0x0D: /* DEREF_I32 (0x00A15640) */
            regs[dst] = (uint64_t)(*(const uint32_t *)(uintptr_t)regs[src1]);
            break;

        case 0x0E: /* DEREF_F32 (0x00A15655) */
            regs[dst] = (uint64_t)(*(const uint32_t *)(uintptr_t)regs[src1]);
            break;

        case 0x0F: /* DEREF_I64 (0x00A1566E) */
            regs[dst] = *(const uint64_t *)(uintptr_t)regs[src1];
            break;

        case 0x10: { /* STORE_32 (0x00A15684) */
            uint32_t val = (uint32_t)regs[src2];
            *(uint32_t *)(uintptr_t)regs[src1] = val;
            regs[dst] = (uint64_t)val;
            break;
        }

        case 0x11: { /* STORE_U32 (0x00A156A0) */
            uint32_t val = (uint32_t)regs[src2];
            *(uint32_t *)(uintptr_t)regs[src1] = val;
            regs[dst] = (uint64_t)val;
            break;
        }

        case 0x12: { /* STORE_64 (0x00A156BF) */
            uint64_t val = regs[src2];
            *(uint64_t *)(uintptr_t)regs[src1] = val;
            regs[dst] = val;
            break;
        }

        case 0x15: { /* LOAD_SYM (0x00A156DF) */
            if (constants) {
                uint32_t sym_id = (uint32_t)constants[src1];
                typedef uint8_t *(ABI *lookup_fn)(uint32_t);
                lookup_fn fn = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00A15E70ull);
                uint8_t *sym_entry = fn(sym_id);
                if (sym_entry) {
                    regs[dst] = (uint64_t)(uintptr_t)sym_entry;
                } else {
                    /* Inlined symbol not found logger at 0x00A15712 */
                    regs[dst] = (uint64_t)(uintptr_t)(g_u2_image_base + 0x009F37B0ull);
                }
            }
            break;
        }

        case 0x16: /* MOV      (0x00A15525) */
        case 0x17: /* MOV_ALT1 (0x00A15525) */
        case 0x18: /* MOV_ALT2 (0x00A15525) */
            regs[dst] = regs[src1];
            break;

        case 0x19: /* FTOI (0x00A1572F) */
            regs[dst] = (uint64_t)(int64_t)(int32_t)u2_reg_to_f32(regs[dst]);
            break;

        case 0x1A: /* ITOF (0x00A15746) */
            regs[dst] = u2_f32_to_reg((float)(int64_t)regs[dst]);
            break;

        case 0x1B: { /* CALL_DEFUN (0x00A1575D) */
            const U2ScriptCode *target = (const U2ScriptCode *)(uintptr_t)regs[src1];
            regs[dst] = (uint64_t)detour_EvalEx(target, (int32_t)src2, &regs[24]);
            break;
        }

        case 0x1C: { /* CALL_NATIVE (0x00A15778) */
            U2DcNativeFn fn = (U2DcNativeFn)(uintptr_t)regs[src1];
            if (fn) {
                uint32_t status = 0;
                regs[dst] = (uint64_t)fn((int32_t)src2, &regs[24], &status);
            }
            break;
        }

        case 0x1D: /* CMP_EQ (0x00A157A8) */
            regs[dst] = (regs[src1] == regs[src2]) ? 1ull : 0ull;
            break;

        case 0x1E: /* CMP_GT (0x00A157C6) */
            regs[dst] = ((int64_t)regs[src1] > (int64_t)regs[src2]) ? ~0ull : 0ull;
            break;

        case 0x1F: /* CMP_GE (0x00A157EB) */
            regs[dst] = ((int64_t)regs[src1] >= (int64_t)regs[src2]) ? ~0ull : 0ull;
            break;

        case 0x20: /* CMP_LT (0x00A15810) */
            regs[dst] = ((int64_t)regs[src1] < (int64_t)regs[src2]) ? ~0ull : 0ull;
            break;

        case 0x21: /* CMP_LE (0x00A15835) */
            regs[dst] = ((int64_t)regs[src1] <= (int64_t)regs[src2]) ? ~0ull : 0ull;
            break;

        case 0x22: /* FCMP_EQ (0x00A1585A) */
            regs[dst] = (u2_reg_to_f32(regs[src1]) == u2_reg_to_f32(regs[src2])) ? u2_f32_to_reg(1.0f) : 0ull;
            break;

        case 0x23: /* FCMP_GT (0x00A15883) */
            regs[dst] = (u2_reg_to_f32(regs[src1]) > u2_reg_to_f32(regs[src2])) ? ~0ull : 0ull;
            break;

        case 0x24: /* FCMP_GE (0x00A158A1) */
            regs[dst] = (u2_reg_to_f32(regs[src1]) >= u2_reg_to_f32(regs[src2])) ? ~0ull : 0ull;
            break;

        case 0x25: /* FCMP_LT (0x00A158BF) */
            regs[dst] = (u2_reg_to_f32(regs[src1]) < u2_reg_to_f32(regs[src2])) ? ~0ull : 0ull;
            break;

        case 0x26: /* FCMP_LE (0x00A158DD) */
            regs[dst] = (u2_reg_to_f32(regs[src1]) <= u2_reg_to_f32(regs[src2])) ? ~0ull : 0ull;
            break;

        case 0x27: /* MOD_INT (0x00A158FB) */
            regs[dst] = regs[src2] ? (uint64_t)((int64_t)regs[src1] % (int64_t)regs[src2]) : 0;
            break;

        case 0x28: /* CALL_HELPER (0x00A15915): fmodf */
            regs[dst] = u2_f32_to_reg(fmodf(u2_reg_to_f32(regs[src1]), u2_reg_to_f32(regs[src2])));
            break;

        case 0x29: { /* ABS_INT (0x00A15936) */
            int64_t v = (int64_t)regs[src1];
            regs[dst] = (uint64_t)(v < 0 ? -v : v);
            break;
        }

        case 0x2A: /* ABS_FLOAT (0x00A1594A) */
            regs[dst] = u2_f32_to_reg(fabsf(u2_reg_to_f32(regs[src1])));
            break;

        case 0x2E: /* JNZ (0x00A15968) */
            if (regs[src1] != 0) {
                uint32_t target_idx = (((uint32_t)src2 << 8) | (uint32_t)dst) - 1u;
                pc = code->bytecode + target_idx * 4;
            }
            break;

        case 0x2F: /* JZ (0x00A15975) */
            if (regs[src1] == 0) {
                uint32_t target_idx = (((uint32_t)src2 << 8) | (uint32_t)dst) - 1u;
                pc = code->bytecode + target_idx * 4;
            }
            break;

        case 0x2D: { /* JMP (0x00A15981) */
            uint32_t target_idx = (((uint32_t)src2 << 8) | (uint32_t)dst) - 1u;
            pc = code->bytecode + target_idx * 4;
            break;
        }

        case 0x30: /* NOT (0x00A1599C) */
            regs[dst] = (regs[src1] == 0) ? ~0ull : 0ull;
            break;

        case 0x31: /* AND_BIT (0x00A159B3) */
            regs[dst] = regs[src1] & regs[src2];
            break;

        case 0x32: /* NOT_BIT (0x00A159BF) */
            regs[dst] = ~regs[src1];
            break;

        case 0x33: /* OR_BIT (0x00A159D5) */
            regs[dst] = regs[src1] | regs[src2];
            break;

        case 0x34: /* XOR_BIT (0x00A159E1) */
            regs[dst] = regs[src1] ^ regs[src2];
            break;

        case 0x35: /* NOR_BIT (0x00A159ED) */
            regs[dst] = ~(regs[src1] | regs[src2]);
            break;

        case 0x36: /* AND_LOGIC (0x00A159FC) */
            regs[dst] = (regs[src1] != 0 && regs[src2] != 0) ? ~0ull : 0ull;
            break;

        case 0x37: /* OR_LOGIC (0x00A15A16) */
            regs[dst] = (regs[src1] != 0 || regs[src2] != 0) ? ~0ull : 0ull;
            break;

        case 0x38: /* NEG_INT (0x00A15A31) */
            regs[dst] = (uint64_t)(-(int64_t)regs[src1]);
            break;

        case 0x39: /* NEG_FLOAT (0x00A15A42) */
            regs[dst] = u2_f32_to_reg(-u2_reg_to_f32(regs[src1]));
            break;

        case 0x3A: /* GET_ARGC (0x00A15A60) */
            regs[dst] = (uint64_t)(int64_t)argc;
            break;

        case 0x3B: /* ADD_IMM (0x00A15A73) */
            regs[dst] = (uint64_t)((int64_t)regs[src1] + (int64_t)src2);
            break;

        case 0x3C: /* SUB_IMM (0x00A15A89) */
            regs[dst] = (uint64_t)((int64_t)regs[src1] - (int64_t)src2);
            break;

        case 0x3D: /* MUL_IMM (0x00A15A9F) */
            regs[dst] = (uint64_t)((int64_t)regs[src1] * (int64_t)src2);
            break;

        case 0x3E: /* DIV_IMM (0x00A15AB6) */
            regs[dst] = src2 ? (uint64_t)((int64_t)regs[src1] / (int64_t)src2) : 0;
            break;

        case 0x3F: /* LOAD_I32 (0x00A15ACE) */
            if (constants) regs[dst] = (uint64_t)(int64_t)(*(const int32_t *)(uintptr_t)(constants + src1));
            break;

        case 0x40: /* LOAD_F32 (0x00A1560E) */
        case 0x42: /* LOAD_SYM_C (0x00A15AE5) */
            if (constants) regs[dst] = (uint64_t)(*(const uint32_t *)(uintptr_t)(constants + src1));
            break;

        case 0x43: /* MOV_ARG (0x00A15AFB) */
            if (constants) regs[dst] = (uint64_t)(int64_t)(*(const int8_t *)(uintptr_t)(constants + src1));
            break;

        case 0x44: /* LOAD_CONST (0x00A15B13) */
            if (constants) regs[dst] = (uint64_t)(*(const uint8_t *)(uintptr_t)(constants + src1));
            break;

        case 0x45: /* LOAD_I16 (0x00A15B2A) */
            if (constants) regs[dst] = (uint64_t)(int64_t)(*(const int16_t *)(uintptr_t)(constants + src1));
            break;

        case 0x46: /* LOAD_U16 (0x00A15B42) */
            if (constants) regs[dst] = (uint64_t)(*(const uint16_t *)(uintptr_t)(constants + src1));
            break;

        case 0x49: /* LOAD_M_I8 (0x00A15BD5) */
            regs[dst] = (uint64_t)(int64_t)(*(const int8_t *)(uintptr_t)regs[src1]);
            break;

        case 0x4A: /* LOAD_M_U8 (0x00A15BEC) */
            regs[dst] = (uint64_t)(*(const uint8_t *)(uintptr_t)regs[src1]);
            break;

        case 0x4B: /* LOAD_M_I16 (0x00A15C02) */
            regs[dst] = (uint64_t)(int64_t)(*(const int16_t *)(uintptr_t)regs[src1]);
            break;

        case 0x4C: /* LOAD_M_U16 (0x00A15C19) */
            regs[dst] = (uint64_t)(*(const uint16_t *)(uintptr_t)regs[src1]);
            break;

        case 0x4D: /* LOAD_M_I32 (0x00A15C2F) */
            regs[dst] = (uint64_t)(int64_t)(*(const int32_t *)(uintptr_t)regs[src1]);
            break;

        default:
            break;
        }

        pc += 4;
    }
}

/* Forward declarations */
static int64_t ABI detour_ExecuteScriptCode(U2ScriptStackFrame *frame, uint64_t *out_result);

/* SysV-ABI replacement for ScriptManager::Run (0x00A14F20)
 * SysV arguments: rdi = out_result, rsi = code, rdx = args, rcx = argc
 */
static int64_t ABI detour_ScriptManager_Run(uint64_t *out_result, U2ScriptCode *code, const uint64_t *args, uint32_t argc) {
    if (out_result) *out_result = 0;
    uint8_t *frame_pool = g_u2_image_base + 0x015341E8ull;
    U2ScriptStackFrame *frame = *(U2ScriptStackFrame **)frame_pool;
    if (!frame) {
        return 0;
    }
    *(U2ScriptStackFrame **)frame_pool = frame->caller;
    frame->return_dst_reg = 0;
    (*(uint32_t *)(g_u2_image_base + 0x015341F0ull))++;
    frame->caller = NULL;

    if (args && argc > 0) {
        uint32_t n = argc < 16 ? argc : 16;
        for (uint32_t i = 0; i < n; ++i) {
            frame->regs[24 + i] = args[i];
        }
    }
    frame->return_dst_reg = 0x28;
    frame->code = code;
    frame->argc = argc;
    frame->pc = 0;

    int64_t res = detour_ExecuteScriptCode(frame, out_result);
    return res != 0 ? 1 : 0;
}
void __attribute__((ms_abi)) runtime_u2_eval_ex(
    uint64_t rdi, uint64_t rsi, uint64_t image, uint64_t r14
) {
    (void)r14;
    const uint64_t n = __atomic_add_fetch(&u2_eval_calls, 1, __ATOMIC_RELAXED);
    if (!u2_script_trace_enabled()) return;
    if (n <= 64 || (n & 0x3FFFull) == 0) {
        printf("[U2 ScriptVM] EvalEx(#%" PRIu64 ") rdi=%#" PRIx64 " rsi=%#" PRIx64 " image=%#" PRIx64 "\n",
               n, rdi, rsi, image);
    }
}

/* ============================================================================
 * 6. Native C Detours (Direct Binary Function Replacement)
 * ============================================================================ */

/* SysV-ABI replacement for ScriptManager::ExecuteScriptCode (0x00A14120) */
static int64_t ABI detour_ExecuteScriptCode(U2ScriptStackFrame *frame, uint64_t *out_result) {
    if (u2_script_trace_enabled()) {
        runtime_u2_execute_script_code((uint64_t)(uintptr_t)frame, (uint64_t)(uintptr_t)out_result,
                                       (uint64_t)(uintptr_t)g_u2_image_base, 0);
    }
    return runtime_u2_vm_execute(frame, out_result, (uint64_t)(uintptr_t)g_u2_image_base);
}

typedef struct {
    const char *name;
    uint64_t offset;
    uintptr_t host_fn;
} U2NativeDetour;

static void write_detour_jmp64(uint8_t *dst, uintptr_t target) {
    /* 12-byte absolute jump in SysV x86-64:
     * 48 B8 <target_64>  movabs rax, target
     * FF E0              jmp    rax
     */
    dst[0] = 0x48;
    dst[1] = 0xB8;
    memcpy(dst + 2, &target, 8);
    dst[10] = 0xFF;
    dst[11] = 0xE0;
}

static void runtime_u2_apply_engine_patches(uint8_t *image, uint64_t image_size) {
    /* 
     * Fix GetSubMeshInstance assertion trap in mesh rendering (ndlib/render/ngen/mesh.h:1361):
     * When iSubMesh >= m_numSubMeshInstances (e.g. LOD transition / missing submesh instance),
     * the engine hits an ASSERTION and traps on int $0x41 at 0x00C47E31.
     * Original at 0x00C47DE0:
     *   39 f0                  cmpl %esi, %eax
     *   7f 54                  jg 0xc47e38
     * When eax <= esi (m_numSubMeshInstances <= iSubMesh), we skip to 0x00C47F60
     * (the exact same skip target that 0x00C47DC8 uses when m_numSubMeshInstances == 0).
     */
    if (image && image_size >= 0x00C48000ULL) {
        static const uint8_t orig_submesh_check[8] = {
            0x7F, 0x54, 0x48, 0x8B, 0x05, 0x8D, 0x61, 0x92
        };
        if (memcmp(image + 0x00C47DE2, orig_submesh_check, sizeof(orig_submesh_check)) == 0) {
            /* 
             * Patch at 0x00C47DE2:
             * 0F 8E 78 01 00 00   jle 0x00C47F60 (target = 0x00C47DE8 + 0x178 = 0x00C47F60)
             * EB 4E               jmp 0x00C47E38 (target = 0x00C47DEA + 0x4E  = 0x00C47E38)
             */
            static const uint8_t patch_submesh_check[8] = {
                0x0F, 0x8E, 0x78, 0x01, 0x00, 0x00,
                0xEB, 0x4E
            };
            memcpy(image + 0x00C47DE2, patch_submesh_check, sizeof(patch_submesh_check));
            printf("[U2 Engine] Patched GetSubMeshInstance bounds check at 0x00C47DE2 -> skip out-of-bounds submeshes\n");
            fflush(stdout);
        }
    }
}

void runtime_u2_install_detours(uint8_t *image, uint64_t image_size) {
    /* Verify Uncharted 2 (big2-ps4_Shipping.elf) via invariant bytes at 0x00A14126
     * inside ScriptManager::ExecuteScriptCode (bytes +6..+19, untouched by thunk hooks):
     * 41 56 41 55 41 54 53 48 81 E4 E0 FF FF FF
     */
    static const uint8_t u2_exec_sig[14] = {
        0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x53,
        0x48, 0x81, 0xE4, 0xE0, 0xFF, 0xFF, 0xFF
    };
    if (!image || image_size < 0x00B00000ULL) return;
    if (memcmp(image + 0x00A14126, u2_exec_sig, sizeof(u2_exec_sig)) != 0) {
        printf("[U2 Recomp] Signature mismatch at 0x00A14126, detours not installed.\n");
        fflush(stdout);
        return;
    }

    g_u2_image_base = image;

    runtime_u2_apply_engine_patches(image, image_size);

    const char *disable_detours = getenv("BB_U2_DISABLE_DETOURS");
    if (disable_detours && *disable_detours && *disable_detours != '0') {
        printf("[U2 Recomp] Native C detours disabled via BB_U2_DISABLE_DETOURS=1\n");
        return;
    }

    const U2NativeDetour detours[] = {
        /* Reimplemented logic & state functions */
        /* Removed invalid offset: DcNot (0x00A12DF0) */
        { "DcEntityName", 0x00A127D0, (uintptr_t)detour_DcEntityName },
        /* Removed invalid offset: DcGetLanguage (0x00A0EC90) */
        { "DcSymbolEqualP", 0x00397620, (uintptr_t)detour_DcSymbolEqualP },
        { "DcFromUps", 0x003976A0, (uintptr_t)detour_DcFromUps },
        { "DcMinmax", 0x009FDBF0, (uintptr_t)detour_DcMinmax },
        { "DcNpcMaxCount", 0x002DEFF0, (uintptr_t)detour_DcNpcMaxCount },
        { "DcNpcAliveCount", 0x002DF000, (uintptr_t)detour_DcNpcAliveCount },
        { "DcCameraCancelAimOverride", 0x00A0DE30, (uintptr_t)detour_DcCameraCancelAimOverride },
        { "DcCameraRestoreLocator", 0x00A0E9B0, (uintptr_t)detour_DcCameraRestoreLocator },
        { "DcDebugPause", 0x00A102A0, (uintptr_t)detour_DcDebugPause },
        { "DcDiscP", 0x009FD910, (uintptr_t)detour_DcDiscP },
        { "DcForcePlayerSwim", 0x0090D740, (uintptr_t)detour_DcForcePlayerSwim },
        { "DcResetAimFov", 0x0090D780, (uintptr_t)detour_DcResetAimFov },
        { "DcOverrideAimFov", 0x0090D7A0, (uintptr_t)detour_DcOverrideAimFov },
        { "DcPlayerInitiateMelee", 0x00909420, (uintptr_t)detour_DcPlayerInitiateMelee },
        { "DcGetPlayerTeachComboTriangle", 0x005DCDE0, (uintptr_t)detour_DcGetPlayerTeachComboTriangle },
        { "DcGetPlayerTeachComboSquare", 0x005DCDF0, (uintptr_t)detour_DcGetPlayerTeachComboSquare },
        { "DcGroundPrintsDisable", 0x00A01EB0, (uintptr_t)detour_DcGroundPrintsDisable },
        { "DcGroundPrintsEnable", 0x00A01ED0, (uintptr_t)detour_DcGroundPrintsEnable },
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

        /* Wave 2: Player, HUD, NPC, Rail, Audio, Math & State Dc* functions */
        { "DcActivePlayerCount", 0x00667F40, (uintptr_t)detour_DcActivePlayerCount },
        { "DcAlivePlayerCount", 0x00667F20, (uintptr_t)detour_DcAlivePlayerCount },
        { "DcAllEnemyNpcsDie", 0x002DE9D0, (uintptr_t)detour_DcAllEnemyNpcsDie },
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
        { "DcNpcMoveTo", 0x002E1C30, (uintptr_t)detour_DcNpcMoveTo },
        { "DcNpcMoveToSpawner", 0x002E15E0, (uintptr_t)detour_DcNpcMoveToSpawner },
        { "DcNpcMarkAsDefunct", 0x002E4F60, (uintptr_t)detour_DcNpcMarkAsDefunct },
        { "DcPlayerClimbingUpP", 0x0090C3D0, (uintptr_t)detour_DcPlayerClimbingUpP },
        { "DcPlayerRemoveWeapons", 0x00907170, (uintptr_t)detour_DcPlayerRemoveWeapons },
        { "DcPlayerUnparent", 0x0090F5F0, (uintptr_t)detour_DcPlayerUnparent },
        { "DcPlayerTeachCombo", 0x005DCDB0, (uintptr_t)detour_DcPlayerTeachCombo },
        { "DcGetPlayerTimeInCombat", 0x005DCD60, (uintptr_t)detour_DcGetPlayerTimeInCombat },
        { "DcGetPlayerTimeSinceCover", 0x005DCCE0, (uintptr_t)detour_DcGetPlayerTimeSinceCover },
        { "DcPowFloat", 0x009FDB60, (uintptr_t)detour_DcPowFloat },
        { "DcLerpScale", 0x009FDB90, (uintptr_t)detour_DcLerpScale },
        { "DcRailActivate", 0x00AB98D0, (uintptr_t)detour_DcRailActivate },
        { "DcRailDeactivateMover", 0x00AB8290, (uintptr_t)detour_DcRailDeactivateMover },
        { "DcRailEnableNpcShooting", 0x00AB8E40, (uintptr_t)detour_DcRailEnableNpcShooting },
        { "DcRailKillAllPhysicsObjects", 0x00AB8F60, (uintptr_t)detour_DcRailKillAllPhysicsObjects },
        { "DcRailMode", 0x00AB8610, (uintptr_t)detour_DcRailMode },
        { "DcSetPlayerHealth", 0x009100F0, (uintptr_t)detour_DcSetPlayerHealth },
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
        { "DcSyncAudioMetaData", 0x009FD560, (uintptr_t)detour_DcSyncAudioMetaData },
        { "DcSyncVoxMetaData", 0x009FD1F0, (uintptr_t)detour_DcSyncVoxMetaData },
        { "DcSyncVoxMpMetaData", 0x009FD1E0, (uintptr_t)detour_DcSyncVoxMpMetaData },
        { "DcTaskSubnodeActiveP", 0x009FEC80, (uintptr_t)detour_DcTaskSubnodeActiveP },
        { "DcElapsedTime", 0x00A0ED60, (uintptr_t)detour_DcElapsedTime },
        { "DcCurrentTime", 0x00A0EDB0, (uintptr_t)detour_DcCurrentTime },

        /* Shipping build no-op / stubbed Dc* functions */
        { "DcPrintStringWs", 0x009FDEA0, (uintptr_t)detour_DcNoOp0 },

        /* Wave 3 & 4: Pure native reimplementations */
        { "DcRandomFloat", 0x009FDA20, (uintptr_t)detour_DcRandomFloat },
        { "DcRandomInteger", 0x009FD930, (uintptr_t)detour_DcRandomInteger },
        { "DcEntityAliveP", 0x00A127E0, (uintptr_t)detour_DcEntityAliveP },
        { "DcCameraDisableAll", 0x00A0E330, (uintptr_t)detour_DcCameraDisableAll },

        /* Wave 5: Physics, Clock, Camera, Weapon & Player State natives */
        { "DcUnitsPerFrame", 0x00A0ECF0, (uintptr_t)detour_DcUnitsPerFrame },
        { "DcUnitsPerSecond", 0x00A0EC70, (uintptr_t)detour_DcUnitsPerSecond },
        { "DcFromUpsNoDt", 0x009FD6B0, (uintptr_t)detour_DcFromUpsNoDt },
        { "DcIsJapanP", 0x009FD8B0, (uintptr_t)detour_DcIsJapanP },
        { "DcPlayerHasWeaponP", 0x00906A50, (uintptr_t)detour_DcPlayerHasWeaponP },
        { "DcPlayerIsAimingP", 0x0090BC00, (uintptr_t)detour_DcPlayerIsAimingP },
        { "DcCameraStoreLocator", 0x00A0E9C0, (uintptr_t)detour_DcCameraStoreLocator },
        { "DcDebugBreak", 0x009FDF30, (uintptr_t)detour_DcDebugBreak },
        { "DcPrintString", 0x009FDEE0, (uintptr_t)detour_DcPrintNoOp },
        { "DcPrintInteger", 0x009FDEF0, (uintptr_t)detour_DcPrintNoOp },
        { "DcPrintFloat", 0x009FDF00, (uintptr_t)detour_DcPrintNoOp },
        { "DcPrintVector", 0x009FDF10, (uintptr_t)detour_DcPrintNoOp },
        { "DcPrintVector3", 0x009FDF20, (uintptr_t)detour_DcPrintNoOp },

        /* Wave 6: Events, Train Car Hierarchy & Player Cover/Ammo State */
        { "DcPlayerInCoverP", 0x0090BBB0, (uintptr_t)detour_DcPlayerInCoverP },
        { "DcPlayerHideThisFrame", 0x0090E330, (uintptr_t)detour_DcPlayerHideThisFrame },
        { "DcPlayerHasWeaponAndAmmoP", 0x00907290, (uintptr_t)detour_DcPlayerHasWeaponAndAmmoP },
        { "DcPlayerHasWeaponAndNoAmmoP", 0x009071B0, (uintptr_t)detour_DcPlayerHasWeaponAndNoAmmoP },
        { "DcPlayerOnJetskiP", 0x00A01B80, (uintptr_t)detour_DcPlayerOnJetskiP },
        { "DcForceDockJetski", 0x00A01B90, (uintptr_t)detour_DcForceDockJetski },
        { "DcForceBoardJetski", 0x00A01BA0, (uintptr_t)detour_DcForceBoardJetski },
        { "DcEventSender", 0x004C1DF0, (uintptr_t)detour_DcEventSender },
        { "DcEventSenderP", 0x004C1ED0, (uintptr_t)detour_DcEventSenderP },
        { "DcEventTrainP", 0x004C31A0, (uintptr_t)detour_DcEventTrainP },
        { "DcEventTrainCarP", 0x004C3020, (uintptr_t)detour_DcEventTrainCarP },

        /* Wave 7: Event Args & Returns, Train Climbing & Player Status */
        { "DcEventArgBoolean", 0x004C2A40, (uintptr_t)detour_DcEventArgBoolean },
        { "DcEventArgInt32", 0x004C2BE0, (uintptr_t)detour_DcEventArgInt32 },
        { "DcEventArgFloat", 0x004C2BF0, (uintptr_t)detour_DcEventArgFloat },
        { "DcEventArgSymbol", 0x004C2DA0, (uintptr_t)detour_DcEventArgSymbol },
        { "DcEventArgString", 0x004C2EC0, (uintptr_t)detour_DcEventArgString },
        { "DcEventReturnBoolean", 0x004C1FF0, (uintptr_t)detour_DcEventReturnBoolean },
        { "DcEventReturnInt32", 0x004C2390, (uintptr_t)detour_DcEventReturnInt32 },
        { "DcEventReturnUint32", 0x004C21D0, (uintptr_t)detour_DcEventReturnUint32 },
        { "DcEventReturnFloat", 0x004C2550, (uintptr_t)detour_DcEventReturnFloat },
        { "DcEventReturnSymbol", 0x004C2720, (uintptr_t)detour_DcEventReturnSymbol },
        { "DcPlayerUnderTrainP", 0x0090A660, (uintptr_t)detour_DcPlayerUnderTrainP },
        { "DcGetPlayerHealth", 0x00910140, (uintptr_t)detour_DcGetPlayerHealth },
        { "DcGetPlayerSpeed", 0x00910020, (uintptr_t)detour_DcGetPlayerSpeed },
        { "DcPlayerInCombatP", 0x0090DC20, (uintptr_t)detour_DcPlayerInCombatP },
        { "DcPlayerHangingOnBarP", 0x0090AB80, (uintptr_t)detour_DcPlayerHangingOnBarP },

        /* Wave 8: Camera Backward Motion, NPC Life Status & Weapon Inventory */
        { "DcCameraMovedBackwardsP", 0x00A0DC40, (uintptr_t)detour_DcCameraMovedBackwardsP },
        { "DcNpcAliveP", 0x002DD900, (uintptr_t)detour_DcNpcAliveP },
        { "DcNpcKilledP", 0x002DD990, (uintptr_t)detour_DcNpcKilledP },
        { "DcNpcSetCanDie", 0x002DD410, (uintptr_t)detour_DcNpcSetCanDie },
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

        /* Wave 9: Ammo Clip Ratio, Audio Rain/Speech & NPC Combat Controls */
        { "DcGetPlayerClipPercentFull", 0x005DCE00, (uintptr_t)detour_DcGetPlayerClipPercentFull },
        { "DcUseAudioRainLevel", 0x00A009F0, (uintptr_t)detour_DcUseAudioRainLevel },
        { "DcSuppressIdleSpeech", 0x00A01360, (uintptr_t)detour_DcSuppressIdleSpeech },
        { "DcHideHint", 0x00A0CC10, (uintptr_t)detour_DcHideHint },
        { "DcNpcDie", 0x002E4680, (uintptr_t)detour_DcNpcDie },
        { "DcNpcEnableShooting", 0x002DC680, (uintptr_t)detour_DcNpcEnableShooting },
        { "DcNpcSetShotDamageMultiplier", 0x002DC570, (uintptr_t)detour_DcNpcSetShotDamageMultiplier },
        { "DcCacheSoundBank", 0x009FD540, (uintptr_t)detour_DcCacheSoundBank },
        { "DcCacheSoundStream", 0x009FD510, (uintptr_t)detour_DcCacheSoundStream },
        { "DcCacheSpeechStream", 0x009FD4E0, (uintptr_t)detour_DcCacheSpeechStream },
        { "DcCacheMusicPlaylist", 0x009FD460, (uintptr_t)detour_DcCacheMusicPlaylist },
    };

    size_t installed = 0;
    for (size_t i = 0; i < sizeof(detours) / sizeof(detours[0]); ++i) {
        if (detours[i].offset + 12 <= image_size) {
            write_detour_jmp64(image + detours[i].offset, detours[i].host_fn);
            ++installed;
        }
    }

    const char *native_vm_env = getenv("BB_U2_NATIVE_VM");
    const int enable_native_vm = (native_vm_env && *native_vm_env && *native_vm_env != '0');
    if (enable_native_vm) {
        write_detour_jmp64(image + 0x00A14120, (uintptr_t)detour_ExecuteScriptCode);
        ++installed;
    }

    const char *native_evalex_env = getenv("BB_U2_NATIVE_EVALEX");
    const int enable_native_evalex = (native_evalex_env && *native_evalex_env && *native_evalex_env != '0');
    if (enable_native_evalex) {
        write_detour_jmp64(image + 0x00A15430, (uintptr_t)detour_EvalEx);
        ++installed;
    }

    const char *native_run_env = getenv("BB_U2_NATIVE_RUN");
    const int enable_native_run = (native_run_env && *native_run_env && *native_run_env != '0');
    if (enable_native_run) {
        write_detour_jmp64(image + 0x00A14F20, (uintptr_t)detour_ScriptManager_Run);
        ++installed;
    }

    printf("[U2 Recomp] Installed %zu native C function detours (ExecuteScriptCode=%s, EvalEx=%s, Run=%s)\n",
           installed,
           enable_native_vm ? "NATIVE_C" : "GUEST_ELF",
           enable_native_evalex ? "NATIVE_C" : "GUEST_ELF",
           enable_native_run ? "NATIVE_C" : "GUEST_ELF");
    fflush(stdout);
}

