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

static void *u2_resolve_npc(int32_t argc, uint64_t *argv) {
    if (!g_u2_image_base) return NULL;
    uint32_t npc_id = 0;
    if (argc > 0 && argv) {
        npc_id = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) npc_id = get_self(0, 0, 1);
    }
    if (npc_id == 0) return NULL;

    typedef void * (ABI *GetNpcFn)(void *, uint32_t);
    GetNpcFn get_npc = (GetNpcFn)(uintptr_t)(g_u2_image_base + 0x00231C50ull);
    void *npc_mgr = (void *)(g_u2_image_base + 0x0110DCB8ull);
    void *npc = get_npc ? get_npc(npc_mgr, npc_id) : NULL;
    if (!npc && *(const int32_t *)(g_u2_image_base + 0x014CE97Cull) >= 2) {
        typedef void * (ABI *GetEntityFn)(uint32_t);
        GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        npc = get_ent ? get_ent(npc_id) : NULL;
    }
    return npc;
}

static int64_t ABI detour_DcNpcGroupDeadCount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DAEE0: (npc-group-dead-count group-id) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return 0;
    typedef int64_t (ABI *CountFn)(int32_t, uint64_t *, uint32_t *);
    CountFn fn = (CountFn)(uintptr_t)(g_u2_image_base + 0x002DAF00ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcNpcInMeleeWithPlayerP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E0050: (npc-in-melee-with-player? [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    const uint8_t *melee_comp = *(const uint8_t **)(npc + 0x2208);
    if (!melee_comp) return 0;
    const uint8_t *sub = *(const uint8_t **)(melee_comp + 0x18);
    if (!sub) return 0;
    const void *vtable = *(const void **)sub;
    if (!vtable) return 0;
    typedef uint8_t (ABI *MeleeFn)(const void *);
    MeleeFn fn = *(MeleeFn *)((const uint8_t *)vtable + 0x30);
    return fn ? (fn(sub) ? 1 : 0) : 0;
}

static int64_t ABI detour_DcNpcFiredRecentlyP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DEAB0: (npc-fired-recently? [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
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

static int64_t ABI detour_DcNpcReloadWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DBDA0: (npc-reload-weapon [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    const uint8_t *comp = *(const uint8_t **)(npc + 0x2208);
    if (!comp) return 0;
    const uint8_t *sub = *(const uint8_t **)(comp + 0x28);
    if (!sub) return 0;
    const void *vtable = *(const void **)sub;
    if (!vtable) return 0;
    typedef uint8_t (ABI *ReloadFn)(const void *);
    ReloadFn fn = *(ReloadFn *)((const uint8_t *)vtable + 0x30);
    return fn ? (fn(sub) ? 1 : 0) : 0;
}

static int64_t ABI detour_DcNpcLowerWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DBD00: (npc-lower-weapon [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    const uint8_t *comp = *(const uint8_t **)(npc + 0x2208);
    if (!comp) return 0;
    const uint8_t *sub = *(const uint8_t **)(comp + 0x28);
    if (!sub) return 0;
    const void *vtable = *(const void **)sub;
    if (!vtable) return 0;
    typedef void (ABI *LowerFn)(const void *);
    LowerFn fn = *(LowerFn *)((const uint8_t *)vtable + 0x88);
    if (fn) fn(sub);
    return 0;
}

static int64_t ABI detour_DcNpcSetInCombat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DA720: (npc-set-in-combat [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    void *combat_mgr = *(void **)(npc + 0x3FA8);
    if (combat_mgr) {
        typedef void (ABI *CombatStateFn)(void *, int32_t);
        CombatStateFn set_state = (CombatStateFn)(uintptr_t)(g_u2_image_base + 0x00277320ull);
        if (set_state) set_state(combat_mgr, 1);
    }
    return 0;
}

static int64_t ABI detour_DcNpcStop(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DBF50: (npc-stop [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    void *nav = *(void **)(npc + 0x21E0);
    if (nav) {
        typedef void (ABI *StopFn)(void *);
        StopFn stop_fn = (StopFn)(uintptr_t)(g_u2_image_base + 0x0023C9A0ull);
        if (stop_fn) stop_fn(nav);
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetWeaponVisible(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DB170: (npc-set-weapon-visible [npc-id] [visible]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
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

static int64_t ABI detour_DcNpcFireWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DBE80: (npc-fire-weapon [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    const uint8_t *comp = *(const uint8_t **)(npc + 0x2208);
    if (!comp) return 0;
    const uint8_t *sub = *(const uint8_t **)(comp + 0x28);
    if (!sub) return 0;
    const void *vtable = *(const void **)sub;
    if (!vtable) return 0;
    typedef void (ABI *FireFn)(const void *);
    FireFn fn = *(FireFn *)((const uint8_t *)vtable + 0x28);
    if (fn) fn(sub);
    return 0;
}

static int64_t ABI detour_DcNpcCanSeeP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DA420: (npc-can-see? [npc-id] [target-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint32_t target_id = (argc > 1 && argv) ? (uint32_t)argv[1] : 0;
    if (target_id == 0) return 0;
    void *vision = *(void **)(npc + 0x21F8);
    if (!vision) return 0;
    typedef uint8_t (ABI *CanSeeFn)(void *, uint32_t);
    CanSeeFn can_see = (CanSeeFn)(uintptr_t)(g_u2_image_base + 0x0025D890ull);
    return can_see ? (can_see(vision, target_id) ? 1 : 0) : 0;
}

static int64_t ABI detour_DcNpcSetToPlayerFaction(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC960: (npc-set-to-player-faction [npc-id]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    *(int32_t *)(npc + 0x150) = 1;
    return 0;
}

static int64_t ABI detour_DcNpcSetDemeanor(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC0D0: (npc-set-demeanor [npc-id] [demeanor]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    int32_t demeanor = (argc > 1 && argv) ? (int32_t)argv[1] : 0;
    *(int32_t *)(npc + 0x154) = demeanor;
    return 0;
}

static int64_t ABI detour_DcNpcDisableFlinch(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC1D0: (npc-disable-flinch [npc-id] [disable]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    int32_t disable = (argc > 1 && argv) ? (int32_t)argv[1] : 1;
    if (disable) {
        *(uint32_t *)(npc + 0x158) |= 0x01;
    } else {
        *(uint32_t *)(npc + 0x158) &= ~0x01;
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetPerfectAim(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC490: (npc-set-perfect-aim [npc-id] [enable]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    int32_t perfect = (argc > 1 && argv) ? (int32_t)argv[1] : 1;
    uint8_t *aim_mgr = *(uint8_t **)(npc + 0x3FA0);
    if (aim_mgr) {
        *(uint8_t *)(aim_mgr + 0x20) = perfect ? 1 : 0;
    }
    return 0;
}

static int64_t ABI detour_DcNpcIgnoreExplosionDamage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DC9C0: (npc-ignore-explosion-damage [npc-id] [ignore]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    int32_t ignore = (argc > 1 && argv) ? (int32_t)argv[1] : 1;
    uint8_t *dmg_ctrl = *(uint8_t **)(npc + 0x1F0);
    if (dmg_ctrl) {
        *(uint8_t *)(dmg_ctrl + 0x48) = ignore ? 1 : 0;
    }
    return 0;
}

static int64_t ABI detour_DcNpcMovementSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DCA40: (npc-movement-speed [npc-id] [speed]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
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

static int64_t ABI detour_DcNpcSetMovementSpeedScale(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DD030: (npc-set-movement-speed-scale [npc-id] [scale]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    float scale = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 1.0f;
    uint8_t *loco = *(uint8_t **)(npc + 0x190);
    if (loco) {
        *(float *)(loco + 0x44) = scale;
    }
    return 0;
}

static int64_t ABI detour_DcNpcEnableHitReactions(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DD4B0: (npc-enable-hit-reactions [npc-id] [enable]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    int32_t enable = (argc > 1 && argv) ? (int32_t)argv[1] : 1;
    *(uint8_t *)(npc + 0x160) = enable ? 1 : 0;
    return 0;
}

static int64_t ABI detour_DcNpcSetCanBeHit(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DD580: (npc-set-can-be-hit [npc-id] [can-be-hit]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    int32_t can_be_hit = (argc > 1 && argv) ? (int32_t)argv[1] : 1;
    *(uint8_t *)(npc + 0x161) = can_be_hit ? 1 : 0;
    return 0;
}

static int64_t ABI detour_DcNpcCombatBehaviorTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DD720: (npc-combat-behavior-time [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return (int64_t)u2_f32_to_reg(0.0f);
    const uint8_t *comp = *(const uint8_t **)(npc + 0x3FA8);
    float tm = comp ? *(const float *)(comp + 0x78) : 0.0f;
    return (int64_t)u2_f32_to_reg(tm);
}

static int64_t ABI detour_DcNpcSetZoneRadius(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3280: (npc-set-zone-radius [npc-id] [radius]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    float r = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
    uint8_t *zone = *(uint8_t **)(npc + 0x21F0);
    if (zone) *(float *)(zone + 0x24) = r;
    return 0;
}

static int64_t ABI detour_DcNpcSetHuntingZoneRadius(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E31C0: (npc-set-hunting-zone-radius [npc-id] [radius]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    float r = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
    uint8_t *zone = *(uint8_t **)(npc + 0x21F0);
    if (zone) *(float *)(zone + 0x28) = r;
    return 0;
}

static int64_t ABI detour_DcNpcSetZoneYThreshold(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3110: (npc-set-zone-y-threshold [npc-id] [threshold]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    float th = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
    uint8_t *zone = *(uint8_t **)(npc + 0x21F0);
    if (zone) *(float *)(zone + 0x2C) = th;
    return 0;
}

static int64_t ABI detour_DcNpcSetHuntingZoneYThreshold(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3050: (npc-set-hunting-zone-y-threshold [npc-id] [threshold]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    float th = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
    uint8_t *zone = *(uint8_t **)(npc + 0x21F0);
    if (zone) *(float *)(zone + 0x30) = th;
    return 0;
}

static int64_t ABI detour_DcNpcSetZoneCenter(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3450: (npc-set-zone-center [npc-id] [point-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint32_t point_id = (argc > 1 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *zone = *(uint8_t **)(npc + 0x21F0);
    if (zone) *(uint32_t *)(zone + 0x14) = point_id;
    return 0;
}

static int64_t ABI detour_DcNpcSetHuntingZoneCenter(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3330: (npc-set-hunting-zone-center [npc-id] [point-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint32_t point_id = (argc > 1 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *zone = *(uint8_t **)(npc + 0x21F0);
    if (zone) *(uint32_t *)(zone + 0x18) = point_id;
    return 0;
}

static int64_t ABI detour_DcNpcTargetInZoneP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E2F20: (npc-target-in-zone? [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    void *zone = *(void **)(npc + 0x21F0);
    if (!zone) return 0;
    typedef uint8_t (ABI *InZoneFn)(void *);
    InZoneFn fn = (InZoneFn)(uintptr_t)(g_u2_image_base + 0x0025D810ull);
    return fn ? (fn(zone) ? 1 : 0) : 0;
}

static int64_t ABI detour_DcNpcSetPatrolRoute(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DE620: (npc-set-patrol-route [npc-id] [route-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint32_t route_id = (argc > 1 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *nav = *(uint8_t **)(npc + 0x21E0);
    if (nav) *(uint32_t *)(nav + 0x58) = route_id;
    return 0;
}

static int64_t ABI detour_DcNpcSetLeadDrakeSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DFEE0: (npc-set-lead-drake-spline [npc-id] [spline-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint32_t spline_id = (argc > 1 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *nav = *(uint8_t **)(npc + 0x21E0);
    if (nav) *(uint32_t *)(nav + 0x5C) = spline_id;
    return 0;
}

static int64_t ABI detour_DcNpcIsPlayerInFrontP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002DF270: (npc-is-player-in-front? [npc-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
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


static int64_t ABI detour_DcNpcMyBattleManagerId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E4EF0: (npc-my-battle-manager-id [npc]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    void **p_mgr_slot = *(void ***)(npc + 0x2548);
    if (!p_mgr_slot) return 0;
    uint8_t *mgr = (uint8_t *)(*p_mgr_slot);
    if (!mgr) return 0;
    int32_t seq = *(const int32_t *)(mgr + 0x44);
    if (seq != *(const int32_t *)(npc + 0x2550)) return 0;
    return *(const int32_t *)(mgr + 0x88);
}

static int64_t ABI detour_DcNpcUseClosestCover(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E43B0: (npc-use-closest-cover [npc] [cover-pos] [anim]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint8_t *ctrl = *(uint8_t **)(npc + 0x2638);
    if (!ctrl) return 0;
    ctrl = *(uint8_t **)(ctrl + 0x08);
    if (!ctrl) return 0;
    uint8_t *cover_mgr = *(uint8_t **)(ctrl + 0x40);
    if (!cover_mgr) return 0;
    if (argc >= 2 && argv && argv[1]) {
        const float *cpos = (const float *)(uintptr_t)argv[1];
        memcpy(cover_mgr + 0x40, cpos, 0x40);
    }
    *(int32_t *)(cover_mgr + 0x34) = -1;
    *(int32_t *)(cover_mgr + 0x30) = 5;
    *(int32_t *)(cover_mgr + 0x1c) = 4;
    return 1;
}

static int64_t ABI detour_DcWaitNpcUseClosestCover(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E4130: (wait-npc-use-closest-cover [npc] [cover-pos]) */
    detour_DcNpcUseClosestCover(argc, argv, status);
    return 0;
}

static int64_t ABI detour_DcCoverThrowGrenadeAnim(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E4070: (cover-throw-grenade-anim [npc]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    typedef void *(ABI *get_anim_fn)(void *);
    void **vtable = *(void ***)npc;
    if (!vtable) return 0;
    get_anim_fn get_anim = (get_anim_fn)vtable[0x2a0 / 8];
    if (!get_anim) return 0;
    uint8_t *anim = (uint8_t *)get_anim(npc);
    if (anim) {
        uint8_t *slot = *(uint8_t **)(npc + 0x2208);
        if (slot) {
            void *rdi = *(void **)(slot + 8);
            if (rdi) {
                typedef void (ABI *play_anim_fn)(void *, void *);
                play_anim_fn play_fn = (play_anim_fn)(uintptr_t)(g_u2_image_base + 0x002C19D0ull);
                if (play_fn) play_fn(rdi, anim + 0x20);
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcUseCap(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3F60: (npc-use-cap [npc] cap_id) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint32_t cap_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *skill_ctrl = *(uint8_t **)(npc + 0x3fa0);
    if (skill_ctrl) {
        *(uint32_t *)(skill_ctrl + 0xc8) = cap_id;
    }
    return 0;
}

static int64_t ABI detour_DcNpcAddSkill(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3E70: (npc-add-skill [npc] skill_idx) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint32_t skill_idx = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *skill_ctrl = *(uint8_t **)(npc + 0x3fa0);
    if (skill_ctrl) {
        *(uint64_t *)(skill_ctrl + 0x20) |= (1ull << (skill_idx & 63u));
    }
    return 0;
}

static int64_t ABI detour_DcNpcRemoveSkill(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3D70: (npc-remove-skill [npc] skill_idx) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint32_t skill_idx = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    uint8_t *skill_ctrl = *(uint8_t **)(npc + 0x3fa0);
    if (skill_ctrl) {
        *(uint64_t *)(skill_ctrl + 0x20) &= ~(1ull << (skill_idx & 63u));
    }
    return 0;
}

static int64_t ABI detour_DcNpcUsingSkillP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3CB0: (npc-using-skill? [npc] skill_idx) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint32_t skill_idx = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    const uint8_t *skill_ctrl = *(const uint8_t * const *)(npc + 0x3fa0);
    if (!skill_ctrl) return 0;
    uint64_t mask = *(const uint64_t *)(skill_ctrl + 0x20);
    return ((mask & (1ull << (skill_idx & 63u))) != 0) ? 1 : 0;
}

static int64_t ABI detour_DcNpcSetAllowAdjustHeadToNavMap(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3C00: (npc-set-allow-adjust-head-to-nav-map [npc] flag) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    int32_t flag = (argc >= 2 && argv && argv[1] != 0) ? 1 : 0;
    uint8_t val = npc[0x21e0];
    if (flag) val |= 0x04;
    else val &= (uint8_t)~0x04;
    npc[0x21e0] = val;
    return 0;
}

static int64_t ABI detour_DcNpcEnableBlockFirstTapDelay(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3B50: (npc-enable-block-first-tap-delay [npc] flag) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    int32_t flag = (argc >= 2 && argv && argv[1] != 0) ? 1 : 0;
    uint8_t val = npc[0x21e0];
    if (flag) val |= 0x08;
    else val &= (uint8_t)~0x08;
    npc[0x21e0] = val;
    return 0;
}

static int64_t ABI detour_DcNpcSetZoneMarker(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E3AA0: (npc-set-zone-marker [npc] [marker]) */
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
    if (set_marker) set_marker(npc_id, 0, &marker_info);
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

static int64_t ABI detour_DcNpcThrowGrenadeAtPlayer(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E2350: (npc-throw-grenade-at-player [npc]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint8_t *ctrl = *(uint8_t **)(npc + 0x4020);
    if (!ctrl) return 0;
    void *player = *(void **)(g_u2_image_base + 0x0115BD20ull);
    if (!player) return 0;
    void *player_ent = *(void **)player;
    if (!player_ent) return 0;
    float target_pos[4];
    memcpy(target_pos, (const uint8_t *)player_ent + 0xf0, 16);
    typedef void (ABI *throw_fn)(void *, void *, const float *);
    void **vtable = *(void ***)ctrl;
    if (!vtable) return 0;
    throw_fn do_throw = (throw_fn)vtable[0x80 / 8];
    if (do_throw) do_throw(ctrl, npc, target_pos);
    return 0;
}

static int64_t ABI detour_DcNpcHasConfirmedStatusP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E2160: (npc-has-confirmed-status? [npc] [target-id]) */
    if (status) *status = 0;
    const uint8_t *npc = (const uint8_t *)u2_resolve_npc(argc, argv);
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

static int64_t ABI detour_DcWaitNpcBind(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E1E80: (wait-npc-bind [npc] ...) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint8_t *ctrl = *(uint8_t **)(npc + 0x2638);
    if (ctrl) {
        ctrl = *(uint8_t **)(ctrl + 0x08);
        if (ctrl) {
            uint8_t *cover_mgr = *(uint8_t **)(ctrl + 0x40);
            if (cover_mgr) {
                *(int32_t *)(cover_mgr + 0x30) = 0;
                *(uint8_t *)(cover_mgr + 0xc1) = 0;
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcNpcUnbind(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E1D20: (npc-unbind [npc]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint8_t *ctrl = *(uint8_t **)(npc + 0x2638);
    if (!ctrl) return 0;
    ctrl = *(uint8_t **)(ctrl + 0x08);
    if (!ctrl) return 0;
    uint8_t *cover_mgr = *(uint8_t **)(ctrl + 0x40);
    if (!cover_mgr) return 0;
    uint8_t *bound_info = *(uint8_t **)(npc + 0x4020);
    if (bound_info && *(const uint32_t *)(bound_info + 0x10) == 0xDE65C4DD) {
        *(int32_t *)(cover_mgr + 0x30) = 0;
        if (cover_mgr[0xc1] != 0) {
            typedef void (ABI *fn_a26620)(void *);
            fn_a26620 f = (fn_a26620)(uintptr_t)(g_u2_image_base + 0x00A26620ull);
            if (f) f(cover_mgr + 0x118);
        }
        cover_mgr[0xc1] = 0;
        if (*(const uint32_t *)(cover_mgr + 0x14) <= 1) {
            *(int32_t *)(npc + 0x2630) = 2;
        }
    } else {
        if (cover_mgr[0xc1] != 0) {
            typedef void (ABI *fn_a26620)(void *);
            fn_a26620 f = (fn_a26620)(uintptr_t)(g_u2_image_base + 0x00A26620ull);
            if (f) f(cover_mgr + 0x118);
        }
        cover_mgr[0xc1] = 0;
    }
    return 0;
}

static int64_t ABI detour_DcNpcTrackPlayerWhileBound(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E1C40: (npc-track-player-while-bound [npc] flag) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint8_t *ctrl = *(uint8_t **)(npc + 0x2638);
    if (!ctrl) return 0;
    ctrl = *(uint8_t **)(ctrl + 0x08);
    if (!ctrl) return 0;
    uint8_t *cover_mgr = *(uint8_t **)(ctrl + 0x40);
    if (!cover_mgr) return 0;
    int32_t flag = (argc >= 2 && argv && argv[1] != 0) ? 1 : 0;
    uint8_t val = cover_mgr[0xc0];
    if (flag) val |= 0x08;
    else val &= (uint8_t)~0x08;
    cover_mgr[0xc0] = val;
    return 0;
}

static int64_t ABI detour_DcNpcAttachToBattleManager(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E0530: (npc-attach-to-battle-manager [npc] [battle-manager-id]) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
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

static int64_t ABI detour_DcNpcSetCommGroupId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E0430: (npc-set-comm-group-id [npc] comm-group-id) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    uint32_t comm_id = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    if (*(uint32_t *)(npc + 0x40ac) != comm_id) {
        *(uint32_t *)(npc + 0x40ac) = comm_id;
        *(uint64_t *)(npc + 0x2658) = 0;
        *(uint32_t *)(npc + 0x2660) = 0;
    }
    return 0;
}

static int64_t ABI detour_DcNpcSetAccuracyMultiplier(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x002E00F0: (npc-set-accuracy-multiplier [npc] mult) */
    if (status) *status = 0;
    uint8_t *npc = (uint8_t *)u2_resolve_npc(argc, argv);
    if (!npc) return 0;
    float mult = 1.0f;
    if (argc >= 2 && argv) {
        mult = *(const float *)&argv[1];
    }
    *(float *)(npc + 0x4044) = mult;
    return 1;
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
    { "DcNpcFiredRecentlyP", 0x002DEAB0, (uintptr_t)detour_DcNpcFiredRecentlyP },
    { "DcNpcReloadWeapon", 0x002DBDA0, (uintptr_t)detour_DcNpcReloadWeapon },
    { "DcNpcLowerWeapon", 0x002DBD00, (uintptr_t)detour_DcNpcLowerWeapon },
    { "DcNpcSetInCombat", 0x002DA720, (uintptr_t)detour_DcNpcSetInCombat },
    { "DcNpcStop", 0x002DBF50, (uintptr_t)detour_DcNpcStop },
    { "DcNpcSetWeaponVisible", 0x002DB170, (uintptr_t)detour_DcNpcSetWeaponVisible },
    { "DcNpcFireWeapon", 0x002DBE80, (uintptr_t)detour_DcNpcFireWeapon },
    { "DcNpcCanSeeP", 0x002DA420, (uintptr_t)detour_DcNpcCanSeeP },
    { "DcNpcSetToPlayerFaction", 0x002DC960, (uintptr_t)detour_DcNpcSetToPlayerFaction },
    { "DcNpcSetDemeanor", 0x002DC0D0, (uintptr_t)detour_DcNpcSetDemeanor },
    { "DcNpcDisableFlinch", 0x002DC1D0, (uintptr_t)detour_DcNpcDisableFlinch },
    { "DcNpcSetPerfectAim", 0x002DC490, (uintptr_t)detour_DcNpcSetPerfectAim },
    { "DcNpcIgnoreExplosionDamage", 0x002DC9C0, (uintptr_t)detour_DcNpcIgnoreExplosionDamage },
    { "DcNpcMovementSpeed", 0x002DCA40, (uintptr_t)detour_DcNpcMovementSpeed },
    { "DcNpcSetMovementSpeedScale", 0x002DD030, (uintptr_t)detour_DcNpcSetMovementSpeedScale },
    { "DcNpcEnableHitReactions", 0x002DD4B0, (uintptr_t)detour_DcNpcEnableHitReactions },
    { "DcNpcSetCanBeHit", 0x002DD580, (uintptr_t)detour_DcNpcSetCanBeHit },
    { "DcNpcCombatBehaviorTime", 0x002DD720, (uintptr_t)detour_DcNpcCombatBehaviorTime },
    { "DcNpcSetZoneRadius", 0x002E3280, (uintptr_t)detour_DcNpcSetZoneRadius },
    { "DcNpcSetHuntingZoneRadius", 0x002E31C0, (uintptr_t)detour_DcNpcSetHuntingZoneRadius },
    { "DcNpcSetZoneYThreshold", 0x002E3110, (uintptr_t)detour_DcNpcSetZoneYThreshold },
    { "DcNpcSetHuntingZoneYThreshold", 0x002E3050, (uintptr_t)detour_DcNpcSetHuntingZoneYThreshold },
    { "DcNpcSetZoneCenter", 0x002E3450, (uintptr_t)detour_DcNpcSetZoneCenter },
    { "DcNpcSetHuntingZoneCenter", 0x002E3330, (uintptr_t)detour_DcNpcSetHuntingZoneCenter },
    { "DcNpcTargetInZoneP", 0x002E2F20, (uintptr_t)detour_DcNpcTargetInZoneP },
    { "DcNpcSetPatrolRoute", 0x002DE620, (uintptr_t)detour_DcNpcSetPatrolRoute },
    { "DcNpcSetLeadDrakeSpline", 0x002DFEE0, (uintptr_t)detour_DcNpcSetLeadDrakeSpline },
    { "DcNpcIsPlayerInFrontP", 0x002DF270, (uintptr_t)detour_DcNpcIsPlayerInFrontP },
    { "DcNpcMyBattleManagerId", 0x002E4EF0, (uintptr_t)detour_DcNpcMyBattleManagerId },
    { "DcNpcUseClosestCover", 0x002E43B0, (uintptr_t)detour_DcNpcUseClosestCover },
    { "DcWaitNpcUseClosestCover", 0x002E4130, (uintptr_t)detour_DcWaitNpcUseClosestCover },
    { "DcCoverThrowGrenadeAnim", 0x002E4070, (uintptr_t)detour_DcCoverThrowGrenadeAnim },
    { "DcNpcUseCap", 0x002E3F60, (uintptr_t)detour_DcNpcUseCap },
    { "DcNpcAddSkill", 0x002E3E70, (uintptr_t)detour_DcNpcAddSkill },
    { "DcNpcRemoveSkill", 0x002E3D70, (uintptr_t)detour_DcNpcRemoveSkill },
    { "DcNpcUsingSkillP", 0x002E3CB0, (uintptr_t)detour_DcNpcUsingSkillP },
    { "DcNpcSetAllowAdjustHeadToNavMap", 0x002E3C00, (uintptr_t)detour_DcNpcSetAllowAdjustHeadToNavMap },
    { "DcNpcEnableBlockFirstTapDelay", 0x002E3B50, (uintptr_t)detour_DcNpcEnableBlockFirstTapDelay },
    { "DcNpcSetZoneMarker", 0x002E3AA0, (uintptr_t)detour_DcNpcSetZoneMarker },
    { "DcNpcSetHuntingZoneMarker", 0x002E3570, (uintptr_t)detour_DcNpcSetHuntingZoneMarker },
    { "DcNpcThrowGrenadeAtPlayer", 0x002E2350, (uintptr_t)detour_DcNpcThrowGrenadeAtPlayer },
    { "DcNpcHasConfirmedStatusP", 0x002E2160, (uintptr_t)detour_DcNpcHasConfirmedStatusP },
    { "DcWaitNpcBind", 0x002E1E80, (uintptr_t)detour_DcWaitNpcBind },
    { "DcNpcUnbind", 0x002E1D20, (uintptr_t)detour_DcNpcUnbind },
    { "DcNpcTrackPlayerWhileBound", 0x002E1C40, (uintptr_t)detour_DcNpcTrackPlayerWhileBound },
    { "DcNpcAttachToBattleManager", 0x002E0530, (uintptr_t)detour_DcNpcAttachToBattleManager },
    { "DcNpcSetCommGroupId", 0x002E0430, (uintptr_t)detour_DcNpcSetCommGroupId },
    { "DcNpcSetAccuracyMultiplier", 0x002E00F0, (uintptr_t)detour_DcNpcSetAccuracyMultiplier },
};

size_t u2_get_npc_detours(const U2NativeDetour **out) {
    if (out) *out = s_npc_detours;
    return sizeof(s_npc_detours) / sizeof(s_npc_detours[0]);
}
