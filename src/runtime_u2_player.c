/* Uncharted 2: Drake Player Locomotion, Health, Weapons & State Functions */
#include "runtime_u2_common.h"

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

static int64_t ABI detour_DcForcePlayerSwim(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D740: (force-player-swim flag) */
    (void)argc; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x0104F8ECull) = ((uint32_t)argv[0] == 1u) ? 1u : 0u;
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

/* Table of detours provided by this module */
static const U2NativeDetour s_player_detours[] = {
    { "DcFromUps", 0x003976A0, (uintptr_t)detour_DcFromUps },
    { "DcMinmax", 0x009FDBF0, (uintptr_t)detour_DcMinmax },
    { "DcForcePlayerSwim", 0x0090D740, (uintptr_t)detour_DcForcePlayerSwim },
    { "DcPlayerInitiateMelee", 0x00909420, (uintptr_t)detour_DcPlayerInitiateMelee },
    { "DcGetPlayerTeachComboTriangle", 0x005DCDE0, (uintptr_t)detour_DcGetPlayerTeachComboTriangle },
    { "DcGetPlayerTeachComboSquare", 0x005DCDF0, (uintptr_t)detour_DcGetPlayerTeachComboSquare },
    { "DcGroundPrintsDisable", 0x00A01EB0, (uintptr_t)detour_DcGroundPrintsDisable },
    { "DcGroundPrintsEnable", 0x00A01ED0, (uintptr_t)detour_DcGroundPrintsEnable },
    { "DcActivePlayerCount", 0x00667F40, (uintptr_t)detour_DcActivePlayerCount },
    { "DcAlivePlayerCount", 0x00667F20, (uintptr_t)detour_DcAlivePlayerCount },
    { "DcPlayerClimbingUpP", 0x0090C3D0, (uintptr_t)detour_DcPlayerClimbingUpP },
    { "DcPlayerRemoveWeapons", 0x00907170, (uintptr_t)detour_DcPlayerRemoveWeapons },
    { "DcPlayerUnparent", 0x0090F5F0, (uintptr_t)detour_DcPlayerUnparent },
    { "DcPlayerTeachCombo", 0x005DCDB0, (uintptr_t)detour_DcPlayerTeachCombo },
    { "DcGetPlayerTimeInCombat", 0x005DCD60, (uintptr_t)detour_DcGetPlayerTimeInCombat },
    { "DcGetPlayerTimeSinceCover", 0x005DCCE0, (uintptr_t)detour_DcGetPlayerTimeSinceCover },
    { "DcSetPlayerHealth", 0x009100F0, (uintptr_t)detour_DcSetPlayerHealth },
    { "DcFromUpsNoDt", 0x009FD6B0, (uintptr_t)detour_DcFromUpsNoDt },
    { "DcPlayerHasWeaponP", 0x00906A50, (uintptr_t)detour_DcPlayerHasWeaponP },
    { "DcPlayerIsAimingP", 0x0090BC00, (uintptr_t)detour_DcPlayerIsAimingP },
    { "DcPlayerInCoverP", 0x0090BBB0, (uintptr_t)detour_DcPlayerInCoverP },
    { "DcPlayerHideThisFrame", 0x0090E330, (uintptr_t)detour_DcPlayerHideThisFrame },
    { "DcPlayerHasWeaponAndAmmoP", 0x00907290, (uintptr_t)detour_DcPlayerHasWeaponAndAmmoP },
    { "DcPlayerHasWeaponAndNoAmmoP", 0x009071B0, (uintptr_t)detour_DcPlayerHasWeaponAndNoAmmoP },
    { "DcPlayerOnJetskiP", 0x00A01B80, (uintptr_t)detour_DcPlayerOnJetskiP },
    { "DcPlayerUnderTrainP", 0x0090A660, (uintptr_t)detour_DcPlayerUnderTrainP },
    { "DcGetPlayerHealth", 0x00910140, (uintptr_t)detour_DcGetPlayerHealth },
    { "DcGetPlayerSpeed", 0x00910020, (uintptr_t)detour_DcGetPlayerSpeed },
    { "DcPlayerInCombatP", 0x0090DC20, (uintptr_t)detour_DcPlayerInCombatP },
    { "DcPlayerHangingOnBarP", 0x0090AB80, (uintptr_t)detour_DcPlayerHangingOnBarP },
    { "DcGetPlayerClipPercentFull", 0x005DCE00, (uintptr_t)detour_DcGetPlayerClipPercentFull },
    { "DcNpcInMeleeWithPlayerP", 0x002E0050, (uintptr_t)detour_DcNpcInMeleeWithPlayerP },
};

size_t u2_get_player_detours(const U2NativeDetour **out) {
    if (out) *out = s_player_detours;
    return sizeof(s_player_detours) / sizeof(s_player_detours[0]);
}
