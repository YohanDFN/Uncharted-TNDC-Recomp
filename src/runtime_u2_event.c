/* Uncharted 2: DC Script Event Argument & Return Pipeline Functions */
#include "runtime_u2_common.h"

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

static int64_t ABI detour_DcEventArgBoundFrame(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C28F0: (event-arg-bound-frame [arg-idx]) */
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
    const uint8_t *memento_slot = *(const uint8_t **)(frame + 0x10);
    if (!memento_slot) return 0;
    uint32_t tag = *(const uint32_t *)(frame + 0x18);
    if (!tag) return 0;
    const uint8_t *target = *(const uint8_t **)memento_slot;
    if (!target) return 0;
    if (*(const uint32_t *)(target + 0x44) != tag) return 0;
    return (int64_t)*(const uint32_t *)(target + 0x68);
}

static int64_t ABI detour_DcPrintEvent(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C1800: (print-event) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    uint8_t *frame = *(uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    uint8_t *memento = *(uint8_t **)frame;
    if (!memento) return 0;
    uint8_t *obj = *(uint8_t **)(memento + 0x08);
    const uint8_t *memento_slot = *(const uint8_t **)(frame + 0x10);
    const uint8_t *sender = NULL;
    if (memento_slot) {
        uint32_t tag = *(const uint32_t *)(frame + 0x18);
        if (tag) {
            const uint8_t *cand = *(const uint8_t **)memento_slot;
            if (cand && *(const uint32_t *)(cand + 0x44) == tag) {
                sender = cand;
            }
        }
    }
    if (obj != sender) {
        *(void **)frame = NULL;
    }
    return 0;
}

static void *u2_get_script_instance(void) {
    if (!g_u2_image_base) return NULL;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return NULL;
    return *(void **)(event_mgr + ((uint64_t)depth << 5));
}

static int64_t ABI detour_DcTagBoolean(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A43700: (tag-boolean tag-name) */
    if (status) *status = 0;
    void *inst = u2_get_script_instance();
    if (!inst) return 0;
    uint32_t sym = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef uint8_t (ABI *fn_int_t)(void *, uint32_t, int32_t *);
    typedef uint8_t (ABI *fn_flt_t)(void *, uint32_t, float *);
    fn_int_t get_int = (fn_int_t)(uintptr_t)(g_u2_image_base + 0x00A2FE50ull);
    int32_t i_val = 0;
    if (get_int(inst, sym, &i_val)) {
        return (i_val != 0) ? 1 : 0;
    }
    fn_flt_t get_flt = (fn_flt_t)(uintptr_t)(g_u2_image_base + 0x00A2FC20ull);
    float f_val = 0.0f;
    if (get_flt(inst, sym, &f_val)) {
        return (f_val != 0.0f) ? 1 : 0;
    }
    return 0;
}

static int64_t ABI detour_DcTagInt32(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A43880: (tag-int32 tag-name) */
    if (status) *status = 0;
    void *inst = u2_get_script_instance();
    if (!inst) return 0;
    uint32_t sym = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef uint8_t (ABI *fn_int_t)(void *, uint32_t, int32_t *);
    fn_int_t get_int = (fn_int_t)(uintptr_t)(g_u2_image_base + 0x00A2FE50ull);
    int32_t i_val = 0;
    if (get_int(inst, sym, &i_val)) {
        return (int64_t)i_val;
    }
    return 0;
}

static int64_t ABI detour_DcTagFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A439D0: (tag-float tag-name) */
    if (status) *status = 0;
    void *inst = u2_get_script_instance();
    if (!inst) return 0;
    uint32_t sym = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef uint8_t (ABI *fn_flt_t)(void *, uint32_t, float *);
    fn_flt_t get_flt = (fn_flt_t)(uintptr_t)(g_u2_image_base + 0x00A2FC20ull);
    float f_val = 0.0f;
    if (get_flt(inst, sym, &f_val)) {
        return (int64_t)u2_f32_to_reg(f_val);
    }
    return 0;
}

static int64_t ABI detour_DcTagSymbol(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A43C90: (tag-symbol tag-name) */
    if (status) *status = 0;
    void *inst = u2_get_script_instance();
    if (!inst) return 0;
    uint32_t sym = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef uint8_t (ABI *fn_sym_t)(void *, uint32_t, uint32_t *);
    fn_sym_t get_sym = (fn_sym_t)(uintptr_t)(g_u2_image_base + 0x00A2F780ull);
    uint32_t s_val = 0;
    if (get_sym(inst, sym, &s_val)) {
        return (int64_t)s_val;
    }
    return 0;
}

static int64_t ABI detour_DcTagSpecifiedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A43DE0: (tag-specified? tag-name) */
    if (status) *status = 0;
    void *inst = u2_get_script_instance();
    if (!inst) return 0;
    uint32_t sym = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef uint8_t (ABI *fn_spec_t)(void *, uint32_t);
    fn_spec_t spec_p = (fn_spec_t)(uintptr_t)(g_u2_image_base + 0x00A2F5F0ull);
    return spec_p(inst, sym) ? 1 : 0;
}

static int64_t ABI detour_DcNetIsGameTypeP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00667E60: (net-is-game-type? [type]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcBroadcastEventToNpcs(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C3A00: (broadcast-event-to-npcs [event-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcSignalRaisedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A40220: (signal-raised? sig [target]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetNpcConfirmCoopPlayersContact(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006690C0: (net-npc-confirm-coop-players-contact [npc]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

/* Table of detours provided by this module */
static int64_t ABI detour_DcEventAttackSourceP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x004C3730: (event-attack-source? [source]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcEventAttackTypeP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x004C38A0: (event-attack-type? [type]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcEventGetAttackInfo(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x004C3D50: (event-get-attack-info) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcEventLevelP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x004C34C0: (event-level? level-name) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcEventMyLevelP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x004C32D0: (event-my-level? [event]) -> bool */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcEventObjectDamage(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x004C3610: (event-object-damage) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *event_mgr = g_u2_image_base + 0x01050320ull;
    int32_t depth = *(const int32_t *)(event_mgr + 0x200);
    if (depth < 0 || depth >= 16) return 0;
    const uint8_t *frame = *(const uint8_t **)(event_mgr + 0x18 + (uint64_t)depth * 32u);
    if (!frame) return 0;
    float dmg = *(const float *)(frame + 0x24);
    return (int64_t)u2_f32_to_reg(dmg);
}

static int64_t ABI detour_DcNetAllPlayersInRegionP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00669500: (net-all-players-in-region? [region-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcNetCaptureFlag(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00669960: (net-capture-flag ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetCurrentNetTask(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00669780: (net-current-net-task) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *net = g_u2_image_base + 0x0113B760ull;
    return (int64_t)*(const uint32_t *)(net + 0x1cf8);
}

static int64_t ABI detour_DcNetDrawObjectiveIcon(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00669250: (net-draw-objective-icon ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcTagString(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A43B40: (tag-string [tag-list] [tag-symbol] [default]) */
    if (status) *status = 0;
    if (argc >= 3 && argv) return (int64_t)argv[2];
    return 0;
}

static int64_t ABI detour_DcNetHudSetGrappleIndicator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00668980: (net-hud-set-grapple-indicator ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetKillEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x006696A0: (net-kill-entity [ent-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetRevivePlayer(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00667D40: (net-revive-player [player-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcNetSendEvent(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x004C3B60: (net-send-event ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcNetShowWaveScreen(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00668890: (net-show-wave-screen wave) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t wave = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    uint8_t *net = g_u2_image_base + 0x0113B760ull;
    *(uint32_t *)(net + 0x1c90) = wave;
    return 1;
}

static const U2NativeDetour s_event_detours[] = {
    { "DcEventArgBoolean", 0x004C2A40, (uintptr_t)detour_DcEventArgBoolean },
    { "DcEventArgInt32", 0x004C2BE0, (uintptr_t)detour_DcEventArgInt32 },
    { "DcEventArgFloat", 0x004C2BF0, (uintptr_t)detour_DcEventArgFloat },
    { "DcEventArgSymbol", 0x004C2DA0, (uintptr_t)detour_DcEventArgSymbol },
    { "DcEventArgString", 0x004C2EC0, (uintptr_t)detour_DcEventArgString },
    { "DcEventArgBoundFrame", 0x004C28F0, (uintptr_t)detour_DcEventArgBoundFrame },
    { "DcEventReturnBoolean", 0x004C1FF0, (uintptr_t)detour_DcEventReturnBoolean },
    { "DcEventReturnInt32", 0x004C2390, (uintptr_t)detour_DcEventReturnInt32 },
    { "DcEventReturnUint32", 0x004C21D0, (uintptr_t)detour_DcEventReturnUint32 },
    { "DcEventReturnFloat", 0x004C2550, (uintptr_t)detour_DcEventReturnFloat },
    { "DcEventReturnSymbol", 0x004C2720, (uintptr_t)detour_DcEventReturnSymbol },
    { "DcEventSender", 0x004C1DF0, (uintptr_t)detour_DcEventSender },
    { "DcPrintEvent", 0x004C1800, (uintptr_t)detour_DcPrintEvent },
    { "DcTagBoolean", 0x00A43700, (uintptr_t)detour_DcTagBoolean },
    { "DcTagInt32", 0x00A43880, (uintptr_t)detour_DcTagInt32 },
    { "DcTagFloat", 0x00A439D0, (uintptr_t)detour_DcTagFloat },
    { "DcTagSymbol", 0x00A43C90, (uintptr_t)detour_DcTagSymbol },
    { "DcTagSpecifiedP", 0x00A43DE0, (uintptr_t)detour_DcTagSpecifiedP },
    { "DcNetIsGameTypeP", 0x00667E60, (uintptr_t)detour_DcNetIsGameTypeP },
    { "DcBroadcastEventToNpcs", 0x004C3A00, (uintptr_t)detour_DcBroadcastEventToNpcs },
    { "DcSignalRaisedP", 0x00A40220, (uintptr_t)detour_DcSignalRaisedP },
    { "DcNetNpcConfirmCoopPlayersContact", 0x006690C0, (uintptr_t)detour_DcNetNpcConfirmCoopPlayersContact },
    { "DcEventAttackSourceP", 0x004C3730, (uintptr_t)detour_DcEventAttackSourceP },
    { "DcEventAttackTypeP", 0x004C38A0, (uintptr_t)detour_DcEventAttackTypeP },
    { "DcEventGetAttackInfo", 0x004C3D50, (uintptr_t)detour_DcEventGetAttackInfo },
    { "DcEventLevelP", 0x004C34C0, (uintptr_t)detour_DcEventLevelP },
    { "DcEventMyLevelP", 0x004C32D0, (uintptr_t)detour_DcEventMyLevelP },
    { "DcEventObjectDamage", 0x004C3610, (uintptr_t)detour_DcEventObjectDamage },
    { "DcNetAllPlayersInRegionP", 0x00669500, (uintptr_t)detour_DcNetAllPlayersInRegionP },
    { "DcNetCaptureFlag", 0x00669960, (uintptr_t)detour_DcNetCaptureFlag },
    { "DcNetCurrentNetTask", 0x00669780, (uintptr_t)detour_DcNetCurrentNetTask },
    { "DcNetDrawObjectiveIcon", 0x00669250, (uintptr_t)detour_DcNetDrawObjectiveIcon },
    { "DcTagString", 0x00A43B40, (uintptr_t)detour_DcTagString },
    { "DcNetHudSetGrappleIndicator", 0x00668980, (uintptr_t)detour_DcNetHudSetGrappleIndicator },
    { "DcNetKillEntity", 0x006696A0, (uintptr_t)detour_DcNetKillEntity },
    { "DcNetRevivePlayer", 0x00667D40, (uintptr_t)detour_DcNetRevivePlayer },
    { "DcNetSendEvent", 0x004C3B60, (uintptr_t)detour_DcNetSendEvent },
    { "DcNetShowWaveScreen", 0x00668890, (uintptr_t)detour_DcNetShowWaveScreen },
};

size_t u2_get_event_detours(const U2NativeDetour **out) {
    if (out) *out = s_event_detours;
    return sizeof(s_event_detours) / sizeof(s_event_detours[0]);
}
