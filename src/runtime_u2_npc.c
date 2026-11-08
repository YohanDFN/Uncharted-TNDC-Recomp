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

/* Table of detours provided by this module */
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
};

size_t u2_get_npc_detours(const U2NativeDetour **out) {
    if (out) *out = s_npc_detours;
    return sizeof(s_npc_detours) / sizeof(s_npc_detours[0]);
}
