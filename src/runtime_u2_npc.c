/* Uncharted 2: NPC & Enemy AI, Combat, Groups & Behavior Functions */
#include "runtime_u2_common.h"

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

static int64_t ABI detour_DcAllEnemyNpcsDie(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DE9D0: (all-enemy-npcs-die) */
    (void)argc; (void)argv; (void)status;
    typedef void (ABI *fn_t)(void *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00232190ull);
    fn(g_u2_image_base + 0x0110DCB8ull);
    return 1;
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

static int64_t ABI detour_DcRailEnableNpcShooting(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8E40: (rail-enable-npc-shooting [flag]) */
    (void)status;
    *(uint8_t *)(g_u2_image_base + 0x011644B8ull) = (argc > 0 && argv[0] == 0) ? 1u : 0u;
    return 0;
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

static int64_t ABI detour_DcNpcInCombatP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DD830: (npc-in-combat? [npc-id]) */
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
    const uint8_t *alert_comp = *(const uint8_t **)((const uint8_t *)npc + 0x3FA8);
    if (!alert_comp) return 0;
    int32_t alert_state = *(const int32_t *)(alert_comp + 0x64);
    return (alert_state >= 3) ? 1 : 0;
}

static int64_t ABI detour_DcNpcGroupAliveCount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DB160: (npc-group-alive-count group-id) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    typedef int64_t (ABI *CountFn)(int32_t, uint64_t *, uint32_t *);
    CountFn fn = (CountFn)(uintptr_t)(g_u2_image_base + 0x002DAF00ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcNpcGroupDeadCount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DAEE0: (npc-group-dead-count group-id) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    typedef int64_t (ABI *CountFn)(int32_t, uint64_t *, uint32_t *);
    CountFn fn = (CountFn)(uintptr_t)(g_u2_image_base + 0x002DAEE0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcNpcInMeleeWithPlayerP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E0050: (npc-in-melee-with-player? [npc-id]) */
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
    const uint8_t *melee_comp = *(const uint8_t **)((const uint8_t *)npc + 0x2208);
    if (!melee_comp) return 0;
    const uint8_t *sub = *(const uint8_t **)(melee_comp + 0x18);
    if (!sub) return 0;
    const void *vtable = *(const void **)sub;
    if (!vtable) return 0;
    typedef uint8_t (ABI *MeleeFn)(const void *);
    MeleeFn fn = *(MeleeFn *)((const uint8_t *)vtable + 0x30);
    return fn ? (fn(sub) ? 1 : 0) : 0;
}

static uint8_t *u2_resolve_npc_from_args(int32_t argc, uint64_t *argv) {
    if (!g_u2_image_base) return NULL;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) {
        npc_id = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    if (!npc_id) return NULL;
    typedef uint8_t *(ABI *get_npc_fn)(void *, uint32_t);
    get_npc_fn get_npc = (get_npc_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    uint8_t *npc = get_npc(g_u2_image_base + 0x0110DCB8ull, npc_id);
    if (!npc && *(const int32_t *)(g_u2_image_base + 0x014CE97Cull) >= 2) {
        typedef uint8_t *(ABI *get_inst_fn)(uint64_t, uint32_t);
        get_inst_fn get_inst = (get_inst_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        uint8_t *inst = get_inst(0, npc_id);
        if (inst && (*(const uint16_t *)(inst + 0x36) & 0x4u)) {
            npc = inst;
        }
    }
    return npc;
}

static int64_t ABI detour_DcNpcDisableFlinch(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC1D0: (npc-disable-flinch [npc]) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint8_t *config = *(uint8_t **)(npc + 0x3FA0);
    if (config) {
        *config &= ~0x20u;
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetPerfectAim(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC490: (npc-set-perfect-aim npc flag) */
    if (status) *status = 0;
    if (argc < 2 || !argv) return 0;
    uint8_t *npc = u2_resolve_npc_from_args(1, argv);
    if (!npc) return 0;
    uint8_t *config = *(uint8_t **)(npc + 0x3FA0);
    if (config) {
        uint32_t flag = (argv[1] != 0) ? 0x400u : 0u;
        *(uint32_t *)config = (*(uint32_t *)config & ~0x400u) | flag;
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetMovementSpeedScale(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DD030: (npc-set-movement-speed-scale [npc] scale) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    float scale = (argc >= 2 && argv) ? *(const float *)&argv[1] : 0.0f;
    uint8_t *p1 = *(uint8_t **)(npc + 0x190);
    if (p1) {
        uint8_t *p2 = *(uint8_t **)(p1 + 0x140);
        if (p2) {
            *(float *)(p2 + 0x40) = scale;
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcTriggerAmbush(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DD0D0: (npc-trigger-ambush [npc]) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint8_t *config = *(uint8_t **)(npc + 0x3FA0);
    if (config) {
        uint32_t val = *(uint32_t *)config;
        if (val & 4u) {
            *(uint32_t *)config = val & ~4u;
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetLeadDrakeSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DFEE0: (npc-set-lead-drake-spline [npc] spline) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint32_t spline_id = 0;
    if (argc > 1 && argv) {
        spline_id = (uint32_t)argv[1];
    } else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) spline_id = get_self(0, 0, 1);
    }
    uint8_t *p1 = *(uint8_t **)(npc + 0x2638);
    if (p1) {
        uint8_t *p2 = *(uint8_t **)(p1 + 0x08);
        if (p2) {
            uint8_t *p3 = *(uint8_t **)(p2 + 0x370);
            if (p3) {
                *(uint32_t *)(p3 + 0x24) = spline_id;
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetAccuracyMultiplier(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E00F0: (npc-set-accuracy-multiplier [npc] mult) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    float mult = (argc >= 2 && argv) ? *(const float *)&argv[1] : 1.0f;
    *(float *)(npc + 0x4044) = mult;
    return 1;
}

static int64_t ABI detour_DcNpcSetCommGroupId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E0430: (npc-set-comm-group-id [npc] group-id) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint32_t group_id = (argc > 1 && argv) ? (uint32_t)argv[1] : 0;
    if (*(uint32_t *)(npc + 0x40AC) != group_id) {
        *(uint32_t *)(npc + 0x40AC) = group_id;
        *(uint64_t *)(npc + 0x2658) = 0;
        *(uint32_t *)(npc + 0x2660) = 0;
    }
    return 0;
}

static int64_t ABI detour_DcNpcMyBattleManagerId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E4EF0: (npc-my-battle-manager-id [npc]) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint8_t *slot = *(uint8_t **)(npc + 0x2548);
    if (!slot) return 0;
    uint8_t *mgr = *(uint8_t **)slot;
    if (!mgr) return 0;
    if (*(const uint32_t *)(mgr + 0x44) != *(const uint32_t *)(npc + 0x2550)) return 0;
    return (int64_t)*(const uint32_t *)(mgr + 0x88);
}

static int64_t ABI detour_DcNpcEnteredActionPackP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DFFB0: (npc-entered-action-pack? [npc]) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    const void *const *vtable = *(const void *const **)npc;
    if (!vtable) return 0;
    typedef void * (ABI *ActionPackFn)(void *);
    ActionPackFn fn = (ActionPackFn)vtable[0x2A0 / 8];
    if (!fn) return 0;
    return fn(npc) != NULL ? 1 : 0;
}

static int64_t ABI detour_DcNpcLowerWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DBD00: (npc-lower-weapon [npc]) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint8_t *w1 = *(uint8_t **)(npc + 0x2208);
    if (w1) {
        uint8_t *w2 = *(uint8_t **)(w1 + 0x28);
        if (w2) {
            const void *const *vtable = *(const void *const **)w2;
            if (vtable) {
                typedef void (ABI *LowerFn)(void *);
                LowerFn fn = (LowerFn)vtable[0x88 / 8];
                if (fn) fn(w2);
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcReloadWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DBDA0: (npc-reload-weapon [npc]) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint8_t *w1 = *(uint8_t **)(npc + 0x2208);
    if (w1) {
        uint8_t *w2 = *(uint8_t **)(w1 + 0x28);
        if (w2) {
            const void *const *vtable = *(const void *const **)w2;
            if (vtable) {
                typedef uint8_t (ABI *IsBusyFn)(void *);
                typedef void (ABI *ReloadFn)(void *);
                IsBusyFn is_busy = (IsBusyFn)vtable[0x30 / 8];
                ReloadFn reload_fn = (ReloadFn)vtable[0xE0 / 8];
                if (is_busy && !is_busy(w2) && reload_fn) {
                    reload_fn(w2);
                    return 1;
                }
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcFireWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DBE80: (npc-fire-weapon [npc]) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    const void *const *vtable = *(const void *const **)npc;
    if (vtable) {
        typedef void (ABI *FireFn)(void *, int32_t, int32_t);
        FireFn fn = (FireFn)vtable[0x438 / 8];
        if (fn) {
            fn(npc, 0, 0);
            return 1;
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetToPlayerFaction(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC960: (npc-set-to-player-faction [npc]) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (npc) {
        *(uint32_t *)(npc + 0x150) = 1;
    }
    return 0;
}

static int64_t ABI detour_DcNpcIgnoreExplosionDamage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC9C0: (npc-ignore-explosion-damage [npc-id] [ignore]) */
    if (status) *status = 0;
    uint8_t *npc = u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    int32_t ignore = (argc > 1 && argv) ? (int32_t)argv[1] : 1;
    uint8_t *dmg_ctrl = *(uint8_t **)(npc + 0x1F0);
    if (dmg_ctrl) {
        void **vt = *(void ***)dmg_ctrl;
        if (vt && vt[0x68 / 8]) {
            typedef void (ABI *ExplosionDmgFn)(void *, int32_t);
            ExplosionDmgFn fn = (ExplosionDmgFn)vt[0x68 / 8];
            fn(dmg_ctrl, ignore ? 0 : -1);
        } else {
            *(uint8_t *)(dmg_ctrl + 0x48) = ignore ? 1 : 0;
        }
    }
    return 0;
}

static int64_t ABI detour_DcGrenadeDirectorDisable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0069BA40: (grenade-director-disable) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    uint8_t *drake = (uint8_t *)u2_get_active_player();
    if (drake) {
        *(uint8_t *)(drake + 0x3120) = 0;
    }
    return 0;
}

static int64_t ABI detour_DcGrenadeDirectorIsEnabledP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0069B9C0: (grenade-director-is-enabled?) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    uint8_t *drake = (uint8_t *)u2_get_active_player();
    return drake ? (*(const uint8_t *)(drake + 0x3120) != 0) : 1;
}

static int64_t ABI detour_DcAllowLegIkByOthers(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FCDF0: (allow-leg-ik-by-others [npc]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcDisallowLegIkByOthers(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FCCF0: (disallow-leg-ik-by-others [npc]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcGrenadeDirectorEnable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0069BAB0: (grenade-director-enable) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    uint8_t *drake = (uint8_t *)u2_get_active_player();
    if (drake) {
        *(uint8_t *)(drake + 0x3120) = 1;
    }
    return 1;
}

static int64_t ABI detour_DcDontCollideAgainstAllies(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FCBB0: (dont-collide-against-allies [npc]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcNpcTurnTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DFD20: (npc-turn-to [npc] target) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNpcUnbind(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E1D20: (npc-unbind [npc]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNpcSetInCombat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DA720: (npc-set-in-combat [npc]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNpcSetPreoccupied(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DA830: (npc-set-preoccupied [npc] [flag]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

/* Table of detours provided by this module */
static int64_t ABI detour_DcNpcAddSkill(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E3E70: (npc-add-skill [npc] skill_idx) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint32_t skill_idx = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *skill_ctrl = *(uint8_t **)(npc + 0x3fa0);
    if (skill_ctrl) {
        *(uint64_t *)(skill_ctrl + 0x20) |= (1ull << (skill_idx & 63u));
    }
    return 0;
}

static int64_t ABI detour_DcNpcAddTraversalSkill(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DE790: (npc-add-traversal-skill [npc] [skill-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t skill_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *skill_fn)(void *, uint32_t);
        skill_fn fn = (skill_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, skill_id);
    }
    return 0;
}

static int64_t ABI detour_DcNpcAimAt(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E0A50: (npc-aim-at [npc] [target-or-pos]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor && argc >= 2 && argv) {
        typedef void (ABI *aim_fn)(void *, uint64_t);
        aim_fn fn = (aim_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, argv[1]);
    }
    return 0;
}

static int64_t ABI detour_DcNpcAttachToBattleManager(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E0530: (npc-attach-to-battle-manager [npc] [battle-manager-id]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint32_t battle_mgr_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void *(ABI *get_bm_fn)(void *, uint32_t);
    get_bm_fn get_bm = (get_bm_fn)(uintptr_t)(g_u2_image_base + 0x0056C7B0ull);
    uint8_t *bm_ent = get_bm ? (uint8_t *)get_bm((void *)(g_u2_image_base + 0x01130390ull), battle_mgr_id) : NULL;
    if (bm_ent) {
        void **p_slot = *(void ***)(bm_ent + 0x30);
        uint32_t seq = *(const uint32_t *)(bm_ent + 0x38);
        if (p_slot && *p_slot) {
            uint8_t *real_mgr = (uint8_t *)(*p_slot);
            if (*(const uint32_t *)(real_mgr + 0x44) == seq) {
                *(void **)(npc + 0x2548) = *(void **)(real_mgr + 0x38);
                *(uint64_t *)(npc + 0x2550) = (uint64_t)seq;
                return 1;
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcCanSeeP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DA420: (npc-can-see? [npc-id] [target-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint32_t target_id = (argc > 1 && argv) ? (uint32_t)argv[1] : 0;
    if (target_id == 0) return 0;
    void *vision = *(void **)(npc + 0x21F8);
    if (!vision) return 0;
    typedef uint8_t (ABI *CanSeeFn)(void *, uint32_t);
    CanSeeFn can_see = (CanSeeFn)(uintptr_t)(g_u2_image_base + 0x0025D890ull);
    return can_see ? (can_see(vision, target_id) ? 1 : 0) : 0;
}

static int64_t ABI detour_DcNpcChangeSniperParams(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DED80: (npc-change-sniper-params [npc] [params]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t params = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *param_fn)(void *, uint32_t);
        param_fn fn = (param_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, params);
    }
    return 0;
}

static int64_t ABI detour_DcNpcChangeVisionSettings(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DA600: (npc-change-vision-settings [npc] [settings-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t vision_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *set_vis_fn)(void *, uint32_t);
        set_vis_fn fn = (set_vis_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, vision_id);
    }
    return 0;
}

static int64_t ABI detour_DcNpcCollectOne(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E4FB0: (npc-collect-one [filter]) -> npc-id */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef uint32_t (ABI *collect_fn)(void);
    collect_fn fn = (collect_fn)(uintptr_t)(g_u2_image_base + 0x002E4C00ull);
    return fn ? (int64_t)fn() : 0;
}

static int64_t ABI detour_DcNpcCombatBehaviorTime(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DD720: (npc-combat-behavior-time [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return (int64_t)u2_f32_to_reg(0.0f);
    const uint8_t *comp = *(const uint8_t **)(npc + 0x3FA8);
    float tm = comp ? *(const float *)(comp + 0x78) : 0.0f;
    return (int64_t)u2_f32_to_reg(tm);
}

static int64_t ABI detour_DcNpcDestroyHeldObject(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DCB10: (npc-destroy-held-object [npc]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *destroy_fn)(void *);
        destroy_fn fn = (destroy_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor);
    }
    return 0;
}

static int64_t ABI detour_DcNpcDisableDealNoDamageWhenOffscreen(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DEF60: (npc-disable-deal-no-damage-when-offscreen) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef void * (ABI *prop_helper_fn)(int32_t, const char *, float);
    prop_helper_fn prop_fn = (prop_helper_fn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    void *obj = prop_fn ? prop_fn(0, "deal-no-damage-when-offscreen", 0.0f) : NULL;
    if (obj) {
        typedef void (ABI *post_fn)(void *, int32_t);
        post_fn pfn = (post_fn)(uintptr_t)(g_u2_image_base + 0x00DE6400ull);
        if (pfn) pfn(obj, 1);
    }
    return 1;
}

static int64_t ABI detour_DcNpcDropObject(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DCC50: (npc-drop-object [npc]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *drop_fn)(void *);
        drop_fn fn = (drop_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor);
    }
    return 0;
}

static int64_t ABI detour_DcNpcDropWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DB680: (npc-drop-weapon [npc]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *drop_fn)(void *);
        drop_fn fn = (drop_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor);
    }
    return 0;
}

static int64_t ABI detour_DcNpcEnableBlockFirstTapDelay(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E3B50: (npc-enable-block-first-tap-delay [npc] flag) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    int32_t flag = (argc >= 2 && argv && argv[1] != 0) ? 1 : 0;
    uint8_t val = npc[0x21e0];
    if (flag) val |= 0x08;
    else val &= (uint8_t)~0x08;
    npc[0x21e0] = val;
    return 0;
}

static int64_t ABI detour_DcNpcEnableHitReactions(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DD4B0: (npc-enable-hit-reactions [npc-id] [enable]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    int32_t enable = (argc > 1 && argv) ? (int32_t)argv[1] : 1;
    *(uint8_t *)(npc + 0x160) = enable ? 1 : 0;
    return 0;
}

static int64_t ABI detour_DcNpcFace(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DBB30: (npc-face [npc] [target-or-dir]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *face_fn)(void *, uint64_t);
        face_fn fn = (face_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, (argc >= 2 && argv) ? argv[1] : 0);
    }
    return 0;
}

static int64_t ABI detour_DcNpcFiredRecentlyP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DEAB0: (npc-fired-recently? [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint32_t flags = ((uint32_t)*(const uint8_t *)(npc + 0x36)) << 16;
    if (flags & 0x40000) {
        const uint8_t *time_ctx = *(const uint8_t **)(npc + 0x70);
        if (time_ctx) {
            uint32_t cur_time = *(const uint32_t *)(time_ctx + 0x0C);
            uint32_t last_fire = *(const uint32_t *)(npc + 0x29C);
            return (cur_time - last_fire < 0x4B) ? 1 : 0;
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcGrabObject(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DCD90: (npc-grab-object [npc] [object]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t obj_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *grab_fn)(void *, uint32_t);
        grab_fn fn = (grab_fn)(uintptr_t)(g_u2_image_base + 0x0056C7B0ull);
        if (fn) fn(actor, obj_id);
    }
    return 0;
}

static int64_t ABI detour_DcNpcGroupFindFirstAlive(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002D7160: (npc-group-find-first-alive [group-id]) -> npc-id */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t group_id = 0;
    if (argc > 0 && argv) group_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) group_id = get_self(0, 0, 1);
    }
    typedef uint32_t (ABI *find_fn)(uint32_t);
    find_fn fn = (find_fn)(uintptr_t)(g_u2_image_base + 0x002D6E20ull);
    return fn ? (int64_t)fn(group_id) : 0;
}

static int64_t ABI detour_DcNpcHasConfirmedStatusP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E2160: (npc-has-confirmed-status? [npc] [target-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    const uint8_t *status_mgr = *(const uint8_t * const *)(npc + 0x3fa8);
    if (!status_mgr) return 0;
    uint32_t target_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    if (target_id == 0) {
        for (int i = 0; i < 16; i++) {
            const uint8_t *slot = status_mgr + 0x38 + i * 0x110;
            if (*(const int32_t *)slot == 3) return 1;
        }
        return 0;
    }
    typedef void *(ABI *get_ent_fn)(uint32_t);
    get_ent_fn get_ent = (get_ent_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    const uint8_t *target_ent = get_ent ? (const uint8_t *)get_ent(target_id) : NULL;
    if (!target_ent || !(*(const uint16_t *)(target_ent + 0x36) & 0x01)) return 0;
    for (int i = 0; i < 16; i++) {
        const uint8_t *slot = status_mgr + 0x38 + i * 0x110;
        void *slot_target = *(void **)(slot - 0x18);
        if (slot_target) {
            uint32_t seq = *(const uint32_t *)(slot - 0x10);
            void *real_ent = *(void **)slot_target;
            if (real_ent && *(const uint32_t *)((const uint8_t *)real_ent + 0x44) == seq) {
                if (real_ent == target_ent && *(const int32_t *)slot == 3) return 1;
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcHunt(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DDA30: (npc-hunt [npc] [target]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t target_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void (ABI *hunt_fn)(uint32_t, uint32_t);
    hunt_fn fn = (hunt_fn)(uintptr_t)(g_u2_image_base + 0x002DD0E0ull);
    if (fn) fn(npc_id, target_id);
    return 0;
}

static int64_t ABI detour_DcNpcIsInScriptedAnimP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DEEC0: (npc-is-in-scripted-anim? [npc]) -> bool */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (!actor) return 0;
    typedef int32_t (ABI *check_fn)(void *);
    check_fn fn = (check_fn)(uintptr_t)(g_u2_image_base + 0x00A35970ull);
    return fn ? (fn(actor) != 0 ? 1 : 0) : 0;
}

static int64_t ABI detour_DcNpcIsNavMapPositionBlockedP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E07A0: (npc-is-nav-map-position-blocked? [npc] [pos-vec]) -> bool */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (!actor) return 0;
    typedef int32_t (ABI *blocked_fn)(void *, const void *);
    blocked_fn fn = (blocked_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    const void *pos = (argc >= 2 && argv) ? (const void *)(uintptr_t)argv[1] : NULL;
    return fn ? (fn(actor, pos) != 0 ? 1 : 0) : 0;
}

static int64_t ABI detour_DcNpcIsPlayerInFrontP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DF270: (npc-is-player-in-front? [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    const uint8_t *drake = (const uint8_t *)u2_get_active_player();
    if (!drake) return 0;

    const float *npc_pos = (const float *)(npc + 0x80);
    const float *drake_pos = (const float *)(drake + 0x80);
    const float *npc_fwd = (const float *)(npc + 0xB0);

    float dx = drake_pos[0] - npc_pos[0];
    float dz = drake_pos[2] - npc_pos[2];
    float dot = dx * npc_fwd[0] + dz * npc_fwd[2];
    return (dot > 0.0f) ? 1 : 0;
}

static int64_t ABI detour_DcNpcLazSetParam(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DA290: (npc-laz-set-param [npc] [param] [val]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    float val = 0.0f;
    if (argc >= 2 && argv) {
        val = *(const float *)&argv[1];
    }
    typedef void (ABI *set_param_fn)(uint32_t, float);
    set_param_fn fn = (set_param_fn)(uintptr_t)(g_u2_image_base + 0x00285EA0ull);
    if (fn) fn(npc_id, val);
    return 0;
}

static int64_t ABI detour_DcNpcMakeOthersAwareOf(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DC780: (npc-make-others-aware-of [npc] [target]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t target_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *aware_fn)(void *, uint32_t);
        aware_fn fn = (aware_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, target_id);
    }
    return 0;
}

static int64_t ABI detour_DcNpcMovementSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DCA40: (npc-movement-speed [npc-id] [speed]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return (int64_t)u2_f32_to_reg(0.0f);
    if (argc > 1 && argv) {
        float spd = u2_reg_to_f32(argv[1]);
        uint8_t *loco = *(uint8_t **)(npc + 0x190);
        if (loco) {
            uint8_t *sub = *(uint8_t **)(loco + 0x140);
            if (sub) *(float *)(sub + 0x20) = spd;
        }
    }
    const uint8_t *loco = *(const uint8_t **)(npc + 0x190);
    float cur_spd = 0.0f;
    if (loco) {
        const uint8_t *sub = *(const uint8_t **)(loco + 0x140);
        if (sub) cur_spd = *(const float *)(sub + 0x20);
    }
    return (int64_t)u2_f32_to_reg(cur_spd);
}

static int64_t ABI detour_DcNpcOverrideContactStatus(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DC2D0: (npc-override-contact-status [npc] [status]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t cstatus = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *status_fn)(void *, uint32_t);
        status_fn fn = (status_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, cstatus);
    }
    return 0;
}

static int64_t ABI detour_DcNpcPlayFacialAnim(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DF010: (npc-play-facial-anim [npc] [anim-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t anim_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *anim_fn)(void *, uint32_t);
        anim_fn fn = (anim_fn)(uintptr_t)(g_u2_image_base + 0x002DE980ull);
        if (fn) fn(actor, anim_id);
    }
    return 0;
}

static int64_t ABI detour_DcNpcPlayingScriptedAnimationP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DEA10: (npc-playing-scripted-animation? [npc]) -> bool */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (!actor) return 0;
    typedef int32_t (ABI *check_fn)(void *);
    check_fn fn = (check_fn)(uintptr_t)(g_u2_image_base + 0x00A35970ull);
    return fn ? (fn(actor) != 0 ? 1 : 0) : 0;
}

static int64_t ABI detour_DcNpcPreferTarget(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DAD80: (npc-prefer-target [npc] [target]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t target_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *prefer_fn)(void *, uint32_t);
        prefer_fn fn = (prefer_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, target_id);
    }
    return 0;
}

static int64_t ABI detour_DcNpcRemoveSkill(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E3D70: (npc-remove-skill [npc] skill_idx) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint32_t skill_idx = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *skill_ctrl = *(uint8_t **)(npc + 0x3fa0);
    if (skill_ctrl) {
        *(uint64_t *)(skill_ctrl + 0x20) &= ~(1ull << (skill_idx & 63u));
    }
    return 0;
}

static int64_t ABI detour_DcNpcRemoveTraversalSkill(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DE6F0: (npc-remove-traversal-skill [npc] [skill-id]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t skill_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *skill_fn)(void *, uint32_t);
        skill_fn fn = (skill_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, skill_id);
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetAllowAdjustHeadToNavMap(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E3C00: (npc-set-allow-adjust-head-to-nav-map [npc] flag) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    int32_t flag = (argc >= 2 && argv && argv[1] != 0) ? 1 : 0;
    uint8_t val = npc[0x21e0];
    if (flag) val |= 0x04;
    else val &= (uint8_t)~0x04;
    npc[0x21e0] = val;
    return 0;
}

static int64_t ABI detour_DcNpcSetAmbient(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DA960: (npc-set-ambient [npc] [ambient-p]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    int32_t flag = (argc >= 2 && argv) ? (int32_t)argv[1] : 1;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *set_ambient_fn)(void *, int32_t);
        set_ambient_fn fn = (set_ambient_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, flag);
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetBuddySniperHideTime(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DEC70: (npc-set-buddy-sniper-hide-time [npc] [time-sec]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    float hide_time = 0.0f;
    if (argc >= 2 && argv) hide_time = *(const float *)&argv[1];
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *hide_fn)(void *, float);
        hide_fn fn = (hide_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, hide_time);
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetCanBeHit(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DD580: (npc-set-can-be-hit [npc-id] [can-be-hit]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    int32_t can_be_hit = (argc > 1 && argv) ? (int32_t)argv[1] : 1;
    *(uint8_t *)(npc + 0x161) = can_be_hit ? 1 : 0;
    return 0;
}

static int64_t ABI detour_DcNpcSetCombatBehavior(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DE560: (npc-set-combat-behavior [npc] [behavior-type]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t behavior = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *beh_fn)(void *, uint32_t);
        beh_fn fn = (beh_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, behavior);
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetCoverAdvanceSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DA300: (npc-set-cover-advance-spline [npc] [spline]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    uint32_t spline_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *set_spline_fn)(void *, uint32_t);
        set_spline_fn fn = (set_spline_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, spline_id);
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetDemeanor(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DC0D0: (npc-set-demeanor [npc-id] [demeanor]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    int32_t demeanor = (argc > 1 && argv) ? (int32_t)argv[1] : 0;
    *(int32_t *)(npc + 0x154) = demeanor;
    return 0;
}

static int64_t ABI detour_DcNpcSetGuardRadioDelay(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DEB60: (npc-set-guard-radio-delay [npc] [delay-sec]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    float delay = 0.0f;
    if (argc >= 2 && argv) delay = *(const float *)&argv[1];
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *delay_fn)(void *, float);
        delay_fn fn = (delay_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, delay);
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetHuntingZoneCenter(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E3330: (npc-set-hunting-zone-center [npc-id] [point-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint32_t point_id = (argc > 1 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *zone = *(uint8_t **)(npc + 0x21F0);
    if (zone) *(uint32_t *)(zone + 0x18) = point_id;
    return 0;
}

static int64_t ABI detour_DcNpcSetHuntingZoneMarker(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E3570: (npc-set-hunting-zone-marker [npc] [marker]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    void *marker_info = NULL;
    if (argc >= 2 && argv) marker_info = (void *)(uintptr_t)argv[1];
    typedef void (ABI *helper_fn)(uint32_t, int32_t, void **);
    helper_fn set_marker = (helper_fn)(uintptr_t)(g_u2_image_base + 0x002E3620ull);
    if (set_marker) set_marker(npc_id, 1, &marker_info);
    return 0;
}

static int64_t ABI detour_DcNpcSetHuntingZoneRadius(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E31C0: (npc-set-hunting-zone-radius [npc-id] [radius]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    float r = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
    uint8_t *zone = *(uint8_t **)(npc + 0x21F0);
    if (zone) *(float *)(zone + 0x28) = r;
    return 0;
}

static int64_t ABI detour_DcNpcSetHuntingZoneYThreshold(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E3050: (npc-set-hunting-zone-y-threshold [npc-id] [threshold]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    float th = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
    uint8_t *zone = *(uint8_t **)(npc + 0x21F0);
    if (zone) *(float *)(zone + 0x30) = th;
    return 0;
}

static int64_t ABI detour_DcNpcSetIgnore(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DD350: (npc-set-ignore [npc] [ignore-p]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) npc_id = (uint32_t)argv[0];
    else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    int32_t ignore_val = (argc >= 2 && argv) ? (int32_t)argv[1] : 1;
    typedef void * (ABI *lookup_fn)(void *, uint32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_pool = (void *)(uintptr_t)(g_u2_image_base + 0x0110DCB8ull);
    void *actor = lookup ? lookup(npc_pool, npc_id) : NULL;
    if (actor) {
        typedef void (ABI *ignore_fn)(void *, int32_t);
        ignore_fn fn = (ignore_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        if (fn) fn(actor, ignore_val);
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetPatrolRoute(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DE620: (npc-set-patrol-route [npc-id] [route-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint32_t route_id = (argc > 1 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *nav = *(uint8_t **)(npc + 0x21E0);
    if (nav) *(uint32_t *)(nav + 0x58) = route_id;
    return 0;
}

static int64_t ABI detour_DcNpcSetWeaponVisible(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002DB170: (npc-set-weapon-visible [npc-id] [visible]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    int32_t visible = (argc > 1 && argv) ? (int32_t)argv[1] : 1;
    void **vtable = *(void ***)npc;
    if (vtable && vtable[0x260 / 8]) {
        typedef void * (ABI *GetWeaponDrawableFn)(void *);
        GetWeaponDrawableFn get_draw = (GetWeaponDrawableFn)vtable[0x260 / 8];
        uint8_t *draw = (uint8_t *)get_draw(npc);
        if (draw) {
            *(uint8_t *)(draw + 0x180) = visible ? 1 : 0;
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetZoneCenter(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x002E3450: (npc-set-zone-center [npc-id] [point-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc_from_args(argc, argv);
    if (!npc) return 0;
    uint32_t point_id = (argc > 1 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *zone = *(uint8_t **)(npc + 0x21F0);
    if (zone) *(uint32_t *)(zone + 0x14) = point_id;
    return 0;
}

static const U2NativeDetour s_npc_detours[] = {
    { "DcNpcMaxCount", 0x002DEFF0, (uintptr_t)detour_DcNpcMaxCount },
    { "DcNpcAliveCount", 0x002DF000, (uintptr_t)detour_DcNpcAliveCount },
    { "DcAllEnemyNpcsDie", 0x002DE9D0, (uintptr_t)detour_DcAllEnemyNpcsDie },
    { "DcNpcMoveTo", 0x002E1C30, (uintptr_t)detour_DcNpcMoveTo },
    { "DcNpcMoveToSpawner", 0x002E15E0, (uintptr_t)detour_DcNpcMoveToSpawner },
    { "DcNpcMarkAsDefunct", 0x002E4F60, (uintptr_t)detour_DcNpcMarkAsDefunct },
    { "DcRailEnableNpcShooting", 0x00AB8E40, (uintptr_t)detour_DcRailEnableNpcShooting },
    { "DcNpcAliveP", 0x002DD900, (uintptr_t)detour_DcNpcAliveP },
    { "DcNpcKilledP", 0x002DD990, (uintptr_t)detour_DcNpcKilledP },
    { "DcNpcSetCanDie", 0x002DD410, (uintptr_t)detour_DcNpcSetCanDie },
    { "DcNpcDie", 0x002E4680, (uintptr_t)detour_DcNpcDie },
    { "DcNpcEnableShooting", 0x002DC680, (uintptr_t)detour_DcNpcEnableShooting },
    { "DcNpcSetShotDamageMultiplier", 0x002DC570, (uintptr_t)detour_DcNpcSetShotDamageMultiplier },
    { "DcNpcInCombatP", 0x002DD830, (uintptr_t)detour_DcNpcInCombatP },
    { "DcNpcGroupAliveCount", 0x002DB160, (uintptr_t)detour_DcNpcGroupAliveCount },
    { "DcNpcGroupDeadCount", 0x002DAEE0, (uintptr_t)detour_DcNpcGroupDeadCount },
    { "DcNpcInMeleeWithPlayerP", 0x002E0050, (uintptr_t)detour_DcNpcInMeleeWithPlayerP },
    { "DcNpcDisableFlinch", 0x002DC1D0, (uintptr_t)detour_DcNpcDisableFlinch },
    { "DcNpcSetPerfectAim", 0x002DC490, (uintptr_t)detour_DcNpcSetPerfectAim },
    { "DcNpcSetMovementSpeedScale", 0x002DD030, (uintptr_t)detour_DcNpcSetMovementSpeedScale },
    { "DcNpcTriggerAmbush", 0x002DD0D0, (uintptr_t)detour_DcNpcTriggerAmbush },
    { "DcNpcSetLeadDrakeSpline", 0x002DFEE0, (uintptr_t)detour_DcNpcSetLeadDrakeSpline },
    { "DcNpcSetAccuracyMultiplier", 0x002E00F0, (uintptr_t)detour_DcNpcSetAccuracyMultiplier },
    { "DcNpcSetCommGroupId", 0x002E0430, (uintptr_t)detour_DcNpcSetCommGroupId },
    { "DcNpcMyBattleManagerId", 0x002E4EF0, (uintptr_t)detour_DcNpcMyBattleManagerId },
    { "DcNpcEnteredActionPackP", 0x002DFFB0, (uintptr_t)detour_DcNpcEnteredActionPackP },
    { "DcNpcLowerWeapon", 0x002DBD00, (uintptr_t)detour_DcNpcLowerWeapon },
    { "DcNpcReloadWeapon", 0x002DBDA0, (uintptr_t)detour_DcNpcReloadWeapon },
    { "DcNpcFireWeapon", 0x002DBE80, (uintptr_t)detour_DcNpcFireWeapon },
    { "DcNpcSetToPlayerFaction", 0x002DC960, (uintptr_t)detour_DcNpcSetToPlayerFaction },
    { "DcNpcIgnoreExplosionDamage", 0x002DC9C0, (uintptr_t)detour_DcNpcIgnoreExplosionDamage },
    { "DcGrenadeDirectorDisable", 0x0069BA40, (uintptr_t)detour_DcGrenadeDirectorDisable },
    { "DcGrenadeDirectorIsEnabledP", 0x0069B9C0, (uintptr_t)detour_DcGrenadeDirectorIsEnabledP },
    { "DcAllowLegIkByOthers", 0x009FCDF0, (uintptr_t)detour_DcAllowLegIkByOthers },
    { "DcDisallowLegIkByOthers", 0x009FCCF0, (uintptr_t)detour_DcDisallowLegIkByOthers },
    { "DcGrenadeDirectorEnable", 0x0069BAB0, (uintptr_t)detour_DcGrenadeDirectorEnable },
    { "DcDontCollideAgainstAllies", 0x009FCBB0, (uintptr_t)detour_DcDontCollideAgainstAllies },
    { "DcNpcTurnTo", 0x002DFD20, (uintptr_t)detour_DcNpcTurnTo },
    { "DcNpcUnbind", 0x002E1D20, (uintptr_t)detour_DcNpcUnbind },
    { "DcNpcSetInCombat", 0x002DA720, (uintptr_t)detour_DcNpcSetInCombat },
    { "DcNpcSetPreoccupied", 0x002DA830, (uintptr_t)detour_DcNpcSetPreoccupied },
    { "DcNpcAddSkill", 0x002E3E70, (uintptr_t)detour_DcNpcAddSkill },
    { "DcNpcAddTraversalSkill", 0x002DE790, (uintptr_t)detour_DcNpcAddTraversalSkill },
    { "DcNpcAimAt", 0x002E0A50, (uintptr_t)detour_DcNpcAimAt },
    { "DcNpcAttachToBattleManager", 0x002E0530, (uintptr_t)detour_DcNpcAttachToBattleManager },
    { "DcNpcCanSeeP", 0x002DA420, (uintptr_t)detour_DcNpcCanSeeP },
    { "DcNpcChangeSniperParams", 0x002DED80, (uintptr_t)detour_DcNpcChangeSniperParams },
    { "DcNpcChangeVisionSettings", 0x002DA600, (uintptr_t)detour_DcNpcChangeVisionSettings },
    { "DcNpcCollectOne", 0x002E4FB0, (uintptr_t)detour_DcNpcCollectOne },
    { "DcNpcCombatBehaviorTime", 0x002DD720, (uintptr_t)detour_DcNpcCombatBehaviorTime },
    { "DcNpcDestroyHeldObject", 0x002DCB10, (uintptr_t)detour_DcNpcDestroyHeldObject },
    { "DcNpcDisableDealNoDamageWhenOffscreen", 0x002DEF60, (uintptr_t)detour_DcNpcDisableDealNoDamageWhenOffscreen },
    { "DcNpcDropObject", 0x002DCC50, (uintptr_t)detour_DcNpcDropObject },
    { "DcNpcDropWeapon", 0x002DB680, (uintptr_t)detour_DcNpcDropWeapon },
    { "DcNpcEnableBlockFirstTapDelay", 0x002E3B50, (uintptr_t)detour_DcNpcEnableBlockFirstTapDelay },
    { "DcNpcEnableHitReactions", 0x002DD4B0, (uintptr_t)detour_DcNpcEnableHitReactions },
    { "DcNpcFace", 0x002DBB30, (uintptr_t)detour_DcNpcFace },
    { "DcNpcFiredRecentlyP", 0x002DEAB0, (uintptr_t)detour_DcNpcFiredRecentlyP },
    { "DcNpcGrabObject", 0x002DCD90, (uintptr_t)detour_DcNpcGrabObject },
    { "DcNpcGroupFindFirstAlive", 0x002D7160, (uintptr_t)detour_DcNpcGroupFindFirstAlive },
    { "DcNpcHasConfirmedStatusP", 0x002E2160, (uintptr_t)detour_DcNpcHasConfirmedStatusP },
    { "DcNpcHunt", 0x002DDA30, (uintptr_t)detour_DcNpcHunt },
    { "DcNpcIsInScriptedAnimP", 0x002DEEC0, (uintptr_t)detour_DcNpcIsInScriptedAnimP },
    { "DcNpcIsNavMapPositionBlockedP", 0x002E07A0, (uintptr_t)detour_DcNpcIsNavMapPositionBlockedP },
    { "DcNpcIsPlayerInFrontP", 0x002DF270, (uintptr_t)detour_DcNpcIsPlayerInFrontP },
    { "DcNpcLazSetParam", 0x002DA290, (uintptr_t)detour_DcNpcLazSetParam },
    { "DcNpcMakeOthersAwareOf", 0x002DC780, (uintptr_t)detour_DcNpcMakeOthersAwareOf },
    { "DcNpcMovementSpeed", 0x002DCA40, (uintptr_t)detour_DcNpcMovementSpeed },
    { "DcNpcOverrideContactStatus", 0x002DC2D0, (uintptr_t)detour_DcNpcOverrideContactStatus },
    { "DcNpcPlayFacialAnim", 0x002DF010, (uintptr_t)detour_DcNpcPlayFacialAnim },
    { "DcNpcPlayingScriptedAnimationP", 0x002DEA10, (uintptr_t)detour_DcNpcPlayingScriptedAnimationP },
    { "DcNpcPreferTarget", 0x002DAD80, (uintptr_t)detour_DcNpcPreferTarget },
    { "DcNpcRemoveSkill", 0x002E3D70, (uintptr_t)detour_DcNpcRemoveSkill },
    { "DcNpcRemoveTraversalSkill", 0x002DE6F0, (uintptr_t)detour_DcNpcRemoveTraversalSkill },
    { "DcNpcSetAllowAdjustHeadToNavMap", 0x002E3C00, (uintptr_t)detour_DcNpcSetAllowAdjustHeadToNavMap },
    { "DcNpcSetAmbient", 0x002DA960, (uintptr_t)detour_DcNpcSetAmbient },
    { "DcNpcSetBuddySniperHideTime", 0x002DEC70, (uintptr_t)detour_DcNpcSetBuddySniperHideTime },
    { "DcNpcSetCanBeHit", 0x002DD580, (uintptr_t)detour_DcNpcSetCanBeHit },
    { "DcNpcSetCombatBehavior", 0x002DE560, (uintptr_t)detour_DcNpcSetCombatBehavior },
    { "DcNpcSetCoverAdvanceSpline", 0x002DA300, (uintptr_t)detour_DcNpcSetCoverAdvanceSpline },
    { "DcNpcSetDemeanor", 0x002DC0D0, (uintptr_t)detour_DcNpcSetDemeanor },
    { "DcNpcSetGuardRadioDelay", 0x002DEB60, (uintptr_t)detour_DcNpcSetGuardRadioDelay },
    { "DcNpcSetHuntingZoneCenter", 0x002E3330, (uintptr_t)detour_DcNpcSetHuntingZoneCenter },
    { "DcNpcSetHuntingZoneMarker", 0x002E3570, (uintptr_t)detour_DcNpcSetHuntingZoneMarker },
    { "DcNpcSetHuntingZoneRadius", 0x002E31C0, (uintptr_t)detour_DcNpcSetHuntingZoneRadius },
    { "DcNpcSetHuntingZoneYThreshold", 0x002E3050, (uintptr_t)detour_DcNpcSetHuntingZoneYThreshold },
    { "DcNpcSetIgnore", 0x002DD350, (uintptr_t)detour_DcNpcSetIgnore },
    { "DcNpcSetPatrolRoute", 0x002DE620, (uintptr_t)detour_DcNpcSetPatrolRoute },
    { "DcNpcSetWeaponVisible", 0x002DB170, (uintptr_t)detour_DcNpcSetWeaponVisible },
    { "DcNpcSetZoneCenter", 0x002E3450, (uintptr_t)detour_DcNpcSetZoneCenter },
};

size_t u2_get_npc_detours(const U2NativeDetour **out) {
    if (out) *out = s_npc_detours;
    return sizeof(s_npc_detours) / sizeof(s_npc_detours[0]);
}
