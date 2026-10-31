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


static int64_t ABI detour_DcNetIsHost(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C80: (net-is-host) -> 1 (host in offline / single player) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcNetIsCinema(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C50: (net-is-cinema) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *net = g_u2_image_base + 0x0113B760ull;
    return (net[0x209c] != 0) ? 1 : 0;
}

static int64_t ABI detour_DcNetCoopHostScriptId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C40: (net-coop-host-script-id) -> 0 */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetCoopScriptId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C30: (net-coop-script-id) -> 0 */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetIsGameTypeP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00667E60: (net-is-game-type? [type]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetCaptureFlag(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669960: (net-capture-flag ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetRespawnFlag(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006698E0: (net-respawn-flag [flag-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetSpawnFlag(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006698C0: (net-spawn-flag ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void (ABI *spawn_flag_fn)(void *, void *);
    spawn_flag_fn fn = (spawn_flag_fn)(uintptr_t)(g_u2_image_base + 0x0063E1B0ull);
    if (fn) fn((void *)(g_u2_image_base + 0x0113B760ull), NULL);
    return 0;
}

static int64_t ABI detour_DcNetSpawnTerritory(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669840: (net-spawn-territory ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetKillTerritory(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669800: (net-kill-territory ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetCoopTask(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006697E0: (net-coop-task) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *net = g_u2_image_base + 0x0113B760ull;
    return (int64_t)*(const uint32_t *)(net + 0x1cfc);
}

static int64_t ABI detour_DcNetCurrentNetTask(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669780: (net-current-net-task) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *net = g_u2_image_base + 0x0113B760ull;
    return (int64_t)*(const uint32_t *)(net + 0x1cf8);
}

static int64_t ABI detour_DcNetKillEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006696A0: (net-kill-entity [ent-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetAllPlayersInRegionP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669500: (net-all-players-in-region? [region-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcNetAllPlayersDeadP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006694B0: (net-all-players-dead?) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetDrawObjectiveIcon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669250: (net-draw-objective-icon ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetPlyaerCoopId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669240: (net-plyaer-coop-id) -> 0 */
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

static int64_t ABI detour_DcNetHudSetGrappleIndicator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00668980: (net-hud-set-grapple-indicator ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
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

static int64_t ABI detour_DcNetHideWaveScreen(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00668850: (net-hide-wave-screen) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint8_t *net = g_u2_image_base + 0x0104F544ull;
    net[0x1c] = 0;
    return 1;
}

static int64_t ABI detour_DcNetGetBoosterAmount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00668820: (net-get-booster-amount booster-idx) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t idx = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef float (ABI *booster_fn)(void *, uint32_t);
    booster_fn fn = (booster_fn)(uintptr_t)(g_u2_image_base + 0x005F6410ull);
    float val = fn ? fn((void *)(g_u2_image_base + 0x0113A788ull), idx) : 0.0f;
    return (int64_t)u2_f32_to_reg(val);
}

static int64_t ABI detour_DcNetGetBoosterEnabled(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00668810: (net-get-booster-enabled booster-idx) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t idx = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef uint8_t (ABI *booster_fn)(uint32_t);
    booster_fn fn = (booster_fn)(uintptr_t)(g_u2_image_base + 0x005F6080ull);
    return fn ? (int64_t)fn(idx) : 0;
}

static int64_t ABI detour_DcNetGetGrappleTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006687F0: (net-get-grapple-time) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void *(ABI *prop_fn)(uint32_t);
    prop_fn fn = (prop_fn)(uintptr_t)(g_u2_image_base + 0x00A15E70ull);
    void *ptr = fn ? fn(0x9D87D2DBu) : NULL;
    float val = ptr ? *(const float *)ptr : 0.0f;
    return (int64_t)u2_f32_to_reg(val);
}

static int64_t ABI detour_DcNetEndWave(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006687E0: (net-end-wave) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void (ABI *end_fn)(void);
    end_fn fn = (end_fn)(uintptr_t)(g_u2_image_base + 0x00617D50ull);
    if (fn) fn();
    return 0;
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

static int64_t ABI detour_DcEventLevelP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C34C0: (event-level? level-name) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcEventAttackTypeP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C38A0: (event-attack-type? [type]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcEventAttackSourceP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C3730: (event-attack-source? [source]) */
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

static int64_t ABI detour_DcTaskNodeActiveP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10340: (task-node-active? task node) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcTaskElapsedTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A102B0: (task-elapsed-time task) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return (int64_t)u2_f32_to_reg(0.0f);
}

static int64_t ABI detour_DcTaskSubnodeComplete(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FECA0: (task-subnode-complete task node) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcTaskSubnodePlay(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FEC00: (task-subnode-play task node) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcTaskFind(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FEB90: (task-find task-name) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcTagSpecifiedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A43DE0: (tag-specified? [tag-list] [tag-symbol]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcTagSymbol(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A43C90: (tag-symbol [tag-list] [tag-symbol] [default]) */
    if (status) *status = 0;
    if (argc >= 3 && argv) return (int64_t)argv[2];
    return 0;
}

static int64_t ABI detour_DcTagString(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A43B40: (tag-string [tag-list] [tag-symbol] [default]) */
    if (status) *status = 0;
    if (argc >= 3 && argv) return (int64_t)argv[2];
    return 0;
}

static int64_t ABI detour_DcTagFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A439D0: (tag-float [tag-list] [tag-symbol] [default]) */
    if (status) *status = 0;
    if (argc >= 3 && argv) return (int64_t)argv[2];
    return (int64_t)u2_f32_to_reg(0.0f);
}

static int64_t ABI detour_DcTagInt32(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A43880: (tag-int32 [tag-list] [tag-symbol] [default]) */
    if (status) *status = 0;
    if (argc >= 3 && argv) return (int64_t)argv[2];
    return 0;
}


static int64_t ABI detour_DcGroupCount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00549810: (group-count group-id) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcGroupSpawnedCount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00549BF0: (group-spawned-count group-id) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcGroupAt(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0054A6B0: (group-at group-id index) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcGroupAnd(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0054AFD0: (group-and group-a group-b) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcGroupOr(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0054A760: (group-or group-a group-b) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcGroupIsMemberSymbolP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0054A250: (group-is-member-symbol? group symbol) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}


/* --------------------------------------------------------------------------
   LOT 20 (Part 2): Event Tension & Tag Setters (5 funcs)
   -------------------------------------------------------------------------- */

static int64_t ABI detour_DcSetForceTension(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00667EE0: (set-force-tension [tension-val]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float tension = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *p_tension = (float *)(uintptr_t)(g_u2_image_base + 0x01150490ull);
    if (p_tension) *p_tension = tension;
    return 1;
}

static int64_t ABI detour_DcSetSymbol(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A42910: (set-symbol [symbol-id] [val]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    uint32_t val = (uint32_t)argv[1];
    typedef void (ABI *set_sym_fn)(uint32_t, uint32_t);
    set_sym_fn fn = (set_sym_fn)(uintptr_t)(g_u2_image_base + 0x00A42800ull);
    if (fn) fn(sym, val);
    return 1;
}

static int64_t ABI detour_DcSetFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A42630: (set-float [symbol-id] [float-val]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    float val = *(const float *)&argv[1];
    typedef void (ABI *set_flt_fn)(uint32_t, float);
    set_flt_fn fn = (set_flt_fn)(uintptr_t)(g_u2_image_base + 0x00A42500ull);
    if (fn) fn(sym, val);
    return 1;
}

static int64_t ABI detour_DcSetInt32(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A42350: (set-int32 [symbol-id] [int-val]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    int32_t val = (int32_t)argv[1];
    typedef void (ABI *set_int_fn)(uint32_t, int32_t);
    set_int_fn fn = (set_int_fn)(uintptr_t)(g_u2_image_base + 0x00A42200ull);
    if (fn) fn(sym, val);
    return 1;
}

static int64_t ABI detour_DcSetBoolean(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A42060: (set-boolean [symbol-id] [bool-val]) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    int32_t val = (int32_t)argv[1] ? 1 : 0;
    typedef void (ABI *set_bool_fn)(uint32_t, int32_t);
    set_bool_fn fn = (set_bool_fn)(uintptr_t)(g_u2_image_base + 0x00A41F00ull);
    if (fn) fn(sym, val);
    return 1;
}

/* Table of detours provided by this module */
static const U2NativeDetour s_event_detours[] = {
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
    { "DcNetIsHost", 0x00669C80, (uintptr_t)detour_DcNetIsHost },
    { "DcNetIsCinema", 0x00669C50, (uintptr_t)detour_DcNetIsCinema },
    { "DcNetCoopHostScriptId", 0x00669C40, (uintptr_t)detour_DcNetCoopHostScriptId },
    { "DcNetCoopScriptId", 0x00669C30, (uintptr_t)detour_DcNetCoopScriptId },
    { "DcNetIsGameTypeP", 0x00667E60, (uintptr_t)detour_DcNetIsGameTypeP },
    { "DcNetCaptureFlag", 0x00669960, (uintptr_t)detour_DcNetCaptureFlag },
    { "DcNetRespawnFlag", 0x006698E0, (uintptr_t)detour_DcNetRespawnFlag },
    { "DcNetSpawnFlag", 0x006698C0, (uintptr_t)detour_DcNetSpawnFlag },
    { "DcNetSpawnTerritory", 0x00669840, (uintptr_t)detour_DcNetSpawnTerritory },
    { "DcNetKillTerritory", 0x00669800, (uintptr_t)detour_DcNetKillTerritory },
    { "DcNetCoopTask", 0x006697E0, (uintptr_t)detour_DcNetCoopTask },
    { "DcNetCurrentNetTask", 0x00669780, (uintptr_t)detour_DcNetCurrentNetTask },
    { "DcNetKillEntity", 0x006696A0, (uintptr_t)detour_DcNetKillEntity },
    { "DcNetAllPlayersInRegionP", 0x00669500, (uintptr_t)detour_DcNetAllPlayersInRegionP },
    { "DcNetAllPlayersDeadP", 0x006694B0, (uintptr_t)detour_DcNetAllPlayersDeadP },
    { "DcNetDrawObjectiveIcon", 0x00669250, (uintptr_t)detour_DcNetDrawObjectiveIcon },
    { "DcNetPlyaerCoopId", 0x00669240, (uintptr_t)detour_DcNetPlyaerCoopId },
    { "DcNetNpcConfirmCoopPlayersContact", 0x006690C0, (uintptr_t)detour_DcNetNpcConfirmCoopPlayersContact },
    { "DcNetHudSetGrappleIndicator", 0x00668980, (uintptr_t)detour_DcNetHudSetGrappleIndicator },
    { "DcNetShowWaveScreen", 0x00668890, (uintptr_t)detour_DcNetShowWaveScreen },
    { "DcNetHideWaveScreen", 0x00668850, (uintptr_t)detour_DcNetHideWaveScreen },
    { "DcNetGetBoosterAmount", 0x00668820, (uintptr_t)detour_DcNetGetBoosterAmount },
    { "DcNetGetBoosterEnabled", 0x00668810, (uintptr_t)detour_DcNetGetBoosterEnabled },
    { "DcNetGetGrappleTime", 0x006687F0, (uintptr_t)detour_DcNetGetGrappleTime },
    { "DcNetEndWave", 0x006687E0, (uintptr_t)detour_DcNetEndWave },
    { "DcEventObjectDamage", 0x004C3610, (uintptr_t)detour_DcEventObjectDamage },
    { "DcEventLevelP", 0x004C34C0, (uintptr_t)detour_DcEventLevelP },
    { "DcEventAttackTypeP", 0x004C38A0, (uintptr_t)detour_DcEventAttackTypeP },
    { "DcEventAttackSourceP", 0x004C3730, (uintptr_t)detour_DcEventAttackSourceP },
    { "DcEventGetAttackInfo", 0x004C3D50, (uintptr_t)detour_DcEventGetAttackInfo },
    { "DcTaskNodeActiveP", 0x00A10340, (uintptr_t)detour_DcTaskNodeActiveP },
    { "DcTaskElapsedTime", 0x00A102B0, (uintptr_t)detour_DcTaskElapsedTime },
    { "DcTaskSubnodeComplete", 0x009FECA0, (uintptr_t)detour_DcTaskSubnodeComplete },
    { "DcTaskSubnodePlay", 0x009FEC00, (uintptr_t)detour_DcTaskSubnodePlay },
    { "DcTaskFind", 0x009FEB90, (uintptr_t)detour_DcTaskFind },
    { "DcTagSpecifiedP", 0x00A43DE0, (uintptr_t)detour_DcTagSpecifiedP },
    { "DcTagSymbol", 0x00A43C90, (uintptr_t)detour_DcTagSymbol },
    { "DcTagString", 0x00A43B40, (uintptr_t)detour_DcTagString },
    { "DcTagFloat", 0x00A439D0, (uintptr_t)detour_DcTagFloat },
    { "DcTagInt32", 0x00A43880, (uintptr_t)detour_DcTagInt32 },
    { "DcGroupCount", 0x00549810, (uintptr_t)detour_DcGroupCount },
    { "DcGroupSpawnedCount", 0x00549BF0, (uintptr_t)detour_DcGroupSpawnedCount },
    { "DcGroupAt", 0x0054A6B0, (uintptr_t)detour_DcGroupAt },
    { "DcGroupAnd", 0x0054AFD0, (uintptr_t)detour_DcGroupAnd },
    { "DcGroupOr", 0x0054A760, (uintptr_t)detour_DcGroupOr },
    { "DcGroupIsMemberSymbolP", 0x0054A250, (uintptr_t)detour_DcGroupIsMemberSymbolP },
    /* Lot 20 (Part 2): Event Tension & Tag Setters (5 funcs) */
    { "DcSetForceTension", 0x00667EE0, (uintptr_t)detour_DcSetForceTension },
    { "DcSetSymbol", 0x00A42910, (uintptr_t)detour_DcSetSymbol },
    { "DcSetFloat", 0x00A42630, (uintptr_t)detour_DcSetFloat },
    { "DcSetInt32", 0x00A42350, (uintptr_t)detour_DcSetInt32 },
    { "DcSetBoolean", 0x00A42060, (uintptr_t)detour_DcSetBoolean },
};

size_t u2_get_event_detours(const U2NativeDetour **out) {
    if (out) *out = s_event_detours;
    return sizeof(s_event_detours) / sizeof(s_event_detours[0]);
}
