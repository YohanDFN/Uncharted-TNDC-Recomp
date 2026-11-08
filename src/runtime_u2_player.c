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

static int64_t ABI detour_DcRemovePlayerWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00906E10: (remove-player-weapon [weapon-type]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t target_weapon_type = 0;
    if (argc > 0 && argv) {
        target_weapon_type = (uint32_t)argv[0];
    }
    const uint8_t *drake = (const uint8_t *)u2_get_active_player();
    if (!drake) return 0;

    int32_t weapon_count = *(const int32_t *)(drake + 0x4BC);
    if (weapon_count <= 0) return 0;

    for (int32_t i = 0; i < weapon_count; ++i) {
        const uint8_t *slot = drake + 0x4C8 + (uint64_t)i * 16u;
        const uint8_t *weapon_ptr = *(const uint8_t **)slot;
        uint32_t tag = *(const uint32_t *)(slot + 8);
        if (!weapon_ptr || tag == 0) continue;

        const uint8_t *weapon = *(const uint8_t **)weapon_ptr;
        if (!weapon || *(const uint32_t *)(weapon + 0x44) != tag) continue;

        uint32_t wtype = *(const uint32_t *)(weapon + 0x550);
        if (wtype == target_weapon_type) {
            void *inv = *(void **)(drake + 0x2768);
            if (inv) {
                void **vtable = *(void ***)inv;
                if (vtable && vtable[0x118 / 8]) {
                    typedef void (ABI *RemoveWeaponFn)(void *, const void *);
                    RemoveWeaponFn remove_fn = (RemoveWeaponFn)vtable[0x118 / 8];
                    remove_fn(inv, weapon);
                }
            }
            break;
        }
    }
    return 0;
}

typedef struct {
    void *ptr;
    uint64_t tag;
} U2WeaponHandle;

static int64_t ABI detour_DcPlayerInstantDrawWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090DDE0: (player-instant-draw-weapon [draw-slot]) ; 0 = sidearm, != 0 = long gun */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    if (argc <= 0 || !argv) return 0;

    int32_t slot_type = (int32_t)argv[0];
    const uint8_t *drake = (const uint8_t *)u2_get_active_player();
    if (!drake) return 0;

    void *inv = *(void **)(drake + 0x2768);
    if (!inv) return 0;

    void **vtable = *(void ***)inv;
    if (!vtable) return 0;

    int32_t draw_mode = 2; /* 2 = sidearm */
    size_t get_fn_idx = 0x70 / 8; /* primary */
    if (slot_type != 0) {
        draw_mode = 3; /* 3 = long gun */
        get_fn_idx = 0x78 / 8; /* secondary */
    }

    typedef U2WeaponHandle (ABI *GetWeaponHandleFn)(void *);
    GetWeaponHandleFn get_handle = (GetWeaponHandleFn)vtable[get_fn_idx];
    if (!get_handle) return 0;

    U2WeaponHandle h = get_handle(inv);
    if (!h.ptr || (uint32_t)h.tag == 0) return 0;

    const uint8_t *weapon = *(const uint8_t **)h.ptr;
    if (!weapon || *(const uint32_t *)(weapon + 0x44) != (uint32_t)h.tag) return 0;

    typedef void (ABI *DrawWeaponFn)(void *, int32_t);
    DrawWeaponFn draw_fn = (DrawWeaponFn)vtable[0xA0 / 8];
    if (draw_fn) {
        draw_fn(inv, draw_mode);
    }
    return 0;
}

static int64_t ABI detour_DcSetPlayerInfiniteAmmo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090BA80: (set-player-infinite-ammo [bool]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    uint8_t enabled = 0;
    if (argc > 0 && argv) {
        enabled = (argv[0] != 0) ? 1 : 0;
    }

    void **mgr_ptr = (void **)(uintptr_t)(g_u2_image_base + 0x01161EE8ull);
    if (!mgr_ptr || !*mgr_ptr) return 0;
    void *mgr = *mgr_ptr;

    typedef void * (ABI *PropHelperFn)(uint32_t, void *, uint8_t *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CDC8ull);
    void *str_arg = (void *)(uintptr_t)(g_u2_image_base + 0x0104F84Aull);
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD30ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD40ull);
    void *obj = prop_fn ? prop_fn(0, str_arg, &enabled, 1, t1, t2, mgr) : NULL;
    if (obj) {
        uint8_t *b = (uint8_t *)obj;
        b[0x61] |= 0x02;
        *(uint32_t *)(b + 0x20) = 2;
    }
    return 0;
}

static int64_t ABI detour_DcWeaponSetReset(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090AD00: (weapon-set-reset) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    typedef void * (ABI *GetWeaponSetFn)(uint32_t hash);
    GetWeaponSetFn get_ws = (GetWeaponSetFn)(uintptr_t)(g_u2_image_base + 0x00A15E70ull);
    void *src = get_ws ? get_ws(0x459D0E4Eu) : NULL;
    if (src) {
        void *dst = (void *)(uintptr_t)(g_u2_image_base + 0x0153E1A0ull);
        memcpy(dst, src, 120);
    }
    return 0;
}

static int64_t ABI detour_DcHolsterPlayerWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D7C0: (holster-player-weapon [mode]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    int32_t holster_mode = 1;
    if (argc > 0 && argv) {
        holster_mode = (int32_t)argv[0];
    }

    void *drake = u2_get_active_player();
    if (!drake) return 0;

    /* Build DC event payload for 0x2E0A4317 ('holster-weapon') */
    struct {
        int32_t mode_type;
        int32_t pad1;
        int32_t mode_val;
        int32_t pad2;
    } arg1 = { 2, 0, holster_mode, 0 };

    struct {
        int32_t obj_type;
        int32_t pad1;
        void *obj;
    } arg2 = { 10, 0, NULL };

    struct {
        int32_t count_type;
        int32_t pad1;
        int32_t count;
        int32_t pad2;
    } arg3 = { 3, 0, 0, 0 };

    struct {
        int32_t zero_type;
        int32_t pad1;
        int32_t zero_val;
        int32_t pad2;
    } arg4 = { 0, 0, 0, 0 };

    typedef void * (ABI *SendEventFn)(void *out_res, uint32_t event_id, void *target, void *a1, void *a2, void *a3, void *a4);
    SendEventFn send_evt = (SendEventFn)(uintptr_t)(g_u2_image_base + 0x004C4090ull);
    uint8_t out_res[32] = {0};
    if (send_evt) {
        send_evt(out_res, 0x2E0A4317u, drake, &arg1, &arg2, &arg3, &arg4);
    }
    return 0;
}

static int64_t ABI detour_DcPlayerHideWeapons(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090E4A0: (player-hide-weapons) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    uint8_t flag = 1;
    void *str_arg = (void *)(uintptr_t)(g_u2_image_base + 0x0104AD78ull + 0x172);
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD30ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD40ull);

    typedef void * (ABI *PropHelperFn)(uint32_t, void *, uint8_t *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    void *obj = prop_fn ? prop_fn(0x15B08523u, str_arg, &flag, 1, t1, t2, NULL) : NULL;
    if (obj) {
        uint8_t *b = (uint8_t *)obj;
        b[0x61] = (b[0x61] & (uint8_t)~0x27) | 0x04;
        *(uint32_t *)(b + 0x20) = 0x65;
    }
    return 0;
}

static int64_t ABI detour_DcPlayerNoReload(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090EC40: (player-no-reload) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;

    const uint8_t *drake = (const uint8_t *)u2_get_active_player();
    if (!drake) return 0;

    uint8_t *inv = *(uint8_t **)(drake + 0x2768);
    if (!inv) return 0;

    void **vtable = *(void ***)inv;
    if (!vtable) return 0;

    typedef int32_t (ABI *GetActiveWeaponIdxFn)(void *);
    GetActiveWeaponIdxFn get_active = (GetActiveWeaponIdxFn)vtable[0x58 / 8];
    int32_t cur_slot = get_active ? get_active(inv) : -1;
    if (cur_slot >= 0) {
        const uint8_t *slot = inv + 0x130 + (uint64_t)cur_slot * 16u;
        const uint8_t *weapon_ptr = *(const uint8_t **)slot;
        uint32_t tag = *(const uint32_t *)(slot + 8);
        if (weapon_ptr && tag != 0) {
            uint8_t *weapon = *(uint8_t **)weapon_ptr;
            if (weapon && *(const uint32_t *)(weapon + 0x44) == tag) {
                int16_t clip = *(const int16_t *)(weapon + 0x620);
                int16_t reserve = *(const int16_t *)(weapon + 0x622);
                if (clip == 0) {
                    if (reserve == 0) {
                        typedef void (ABI *NoAmmoFn)(void *, int32_t, int32_t);
                        NoAmmoFn no_ammo = (NoAmmoFn)(uintptr_t)(g_u2_image_base + 0x00AE4930ull);
                        if (no_ammo) no_ammo(weapon, 1, 0);
                    }
                    typedef void (ABI *DisableReloadFn)(void *, int32_t);
                    DisableReloadFn dis_reload = (DisableReloadFn)(uintptr_t)(g_u2_image_base + 0x00AE98B0ull);
                    if (dis_reload) dis_reload(weapon, 0);
                }
            }
        }
    }

    uint8_t flag = 1;
    void *str_arg = (void *)(uintptr_t)(g_u2_image_base + 0x0104F810ull + 0x3C);
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD30ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1CD40ull);
    typedef void * (ABI *PropHelperFn)(uint32_t, void *, uint8_t *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    void *obj = prop_fn ? prop_fn(0xEBFE460Du, str_arg, &flag, 1, t1, t2, NULL) : NULL;
    if (obj) {
        uint8_t *b = (uint8_t *)obj;
        b[0x61] = (b[0x61] & (uint8_t)~0x27) | 0x04;
        *(uint32_t *)(b + 0x20) = 0x65;
    }
    return 0;
}

static int64_t ABI detour_DcGivePlayerWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00906B20: (give-player-weapon [weapon-type] [ammo]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    if (argc <= 0 || !argv) return 0;

    uint32_t target_weapon_type = (uint32_t)argv[0];
    int32_t ammo = (argc > 1) ? (int32_t)argv[1] : -1;

    const uint8_t *drake = (const uint8_t *)u2_get_active_player();
    if (!drake) return 0;

    /* Check if player already possesses this weapon */
    int32_t weapon_count = *(const int32_t *)(drake + 0x4BC);
    for (int32_t i = 0; i < weapon_count; ++i) {
        const uint8_t *slot = drake + 0x4C8 + (uint64_t)i * 16u;
        const uint8_t *weapon_ptr = *(const uint8_t **)slot;
        uint32_t tag = *(const uint32_t *)(slot + 8);
        if (!weapon_ptr || tag == 0) continue;

        uint8_t *weapon = *(uint8_t **)weapon_ptr;
        if (!weapon || *(const uint32_t *)(weapon + 0x44) != tag) continue;

        if (*(const uint32_t *)(weapon + 0x550) == target_weapon_type) {
            /* Weapon already equipped: replenish ammo if requested */
            if (ammo >= 0) {
                typedef void (ABI *AddAmmoFn)(void *, int32_t, int32_t);
                AddAmmoFn add_ammo = (AddAmmoFn)(uintptr_t)(g_u2_image_base + 0x00AE4670ull);
                if (add_ammo) add_ammo(weapon, ammo, 0);
            }
            return 0;
        }
    }

    /* Spawn new weapon entity via engine spawner (0x00ADA780) */
    uint8_t spawn_params[176] = {0};
    *(uint32_t *)(spawn_params + 0x70) = target_weapon_type;
    *(uint32_t *)(spawn_params + 0x6C) = 0xFFFFFFFFu;
    *(const void **)(spawn_params + 0x40) = *(const void **)(drake + 0x38);
    *(uint32_t *)(spawn_params + 0x48) = *(const uint32_t *)(drake + 0x44);
    *(void **)(spawn_params + 0x58) = (void *)(uintptr_t)(g_u2_image_base + 0x00EABE0Aull);

    typedef void * (ABI *SpawnWeaponFn)(void *);
    SpawnWeaponFn spawn_fn = (SpawnWeaponFn)(uintptr_t)(g_u2_image_base + 0x00ADA780ull);
    if (spawn_fn) {
        spawn_fn(spawn_params);
    }
    return 0;
}

static int64_t ABI detour_DcPlayerCheckForGotHit(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0EF20: (player-check-for-got-hit) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    const uint8_t *ctx = g_u2_image_base + 0x0115BD20ull;
    void **p_target = *(void ***)ctx;
    if (!p_target) return 0;
    uint32_t tag = *(const uint32_t *)(ctx + 8);
    if (!tag) return 0;
    uint8_t *obj = (uint8_t *)(*p_target);
    if (!obj || *(const uint32_t *)(obj + 0x44) != tag) return 0;
    typedef void (ABI *CheckHitFn)(void *player_obj);
    CheckHitFn check_fn = (CheckHitFn)(uintptr_t)(g_u2_image_base + 0x00976A30ull);
    if (check_fn) check_fn(obj);
    return 0;
}

static int64_t ABI detour_DcCalcNewPhaseFromSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FBF0: (calc-new-phase-from-speed) */
    if (status) *status = 0;
    if (!g_u2_image_base || !argv || argc < 4) return (int64_t)u2_f32_to_reg(0.0f);

    float arg0 = u2_reg_to_f32(argv[0]);
    float arg1 = u2_reg_to_f32(argv[1]);
    int32_t arg2 = (int32_t)argv[2];
    float arg3 = u2_reg_to_f32(argv[3]);
    float arg4 = (argc >= 5) ? u2_reg_to_f32(argv[4]) : 0.0f;

    const uint8_t *ctx = g_u2_image_base + 0x0115BD20ull;
    void **p_target = *(void ***)ctx;
    if (!p_target || !*p_target) return (int64_t)u2_f32_to_reg(0.0f);

    uint8_t *player = (uint8_t *)(*p_target);
    uint32_t tag = *(const uint32_t *)(player + 0x44);
    uint32_t expected_tag = *(const uint32_t *)(ctx + 8);
    if (tag != expected_tag) return (int64_t)u2_f32_to_reg(0.0f);

    uint8_t *comp = *(uint8_t **)(player + 0x190);
    if (!comp) return (int64_t)u2_f32_to_reg(0.0f);

    void *rdi = (void *)(comp + 0x150);
    float xmm1 = arg1 * arg3;
    float xmm2 = arg4 * arg3;

    typedef float (ABI *CalcPhaseFn)(void *comp, int32_t anim_idx, float f0, float f1, float f2);
    CalcPhaseFn fn = (CalcPhaseFn)(uintptr_t)(g_u2_image_base + 0x003A7360ull);
    if (!fn) return (int64_t)u2_f32_to_reg(0.0f);

    float result = fn(rdi, arg2, arg0, xmm1, xmm2);
    return (int64_t)u2_f32_to_reg(result);
}

static int64_t ABI detour_DcDrawReticle(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090E6E0: (draw-reticle) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    void *hud_mgr = (void *)(g_u2_image_base + 0x011579F0ull);
    typedef void (ABI *DrawReticleInternalFn)(void *hud_mgr, float x, float y, float z);
    DrawReticleInternalFn fn = (DrawReticleInternalFn)(uintptr_t)(g_u2_image_base + 0x00936780ull);
    if (fn) fn(hud_mgr, -1.0f, -1.0f, -1.0f);
    return 0;
}

static int64_t ABI detour_DcCalcStartPhase(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FB70: (calc-start-phase ent) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 1 || !argv) return (int64_t)u2_f32_to_reg(0.0f);
    typedef float (ABI *CalcPhaseFn)(uint32_t ent_id, int32_t max_frames, float min_phase, float max_phase);
    CalcPhaseFn fn = (CalcPhaseFn)(uintptr_t)(g_u2_image_base + 0x0067E600ull);
    if (!fn) return (int64_t)u2_f32_to_reg(0.0f);
    float max_p = *(const float *)(g_u2_image_base + 0x00E67410ull);
    float res = fn((uint32_t)argv[0], -1, 0.0f, max_p);
    return (int64_t)u2_f32_to_reg(res);
}

static int64_t ABI detour_DcCalcStartPhaseMaxFrames(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FAF0: (calc-start-phase-max-frames ent max-frames) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 2 || !argv) return (int64_t)u2_f32_to_reg(0.0f);
    typedef float (ABI *CalcPhaseFn)(uint32_t ent_id, int32_t max_frames, float min_phase, float max_phase);
    CalcPhaseFn fn = (CalcPhaseFn)(uintptr_t)(g_u2_image_base + 0x0067E600ull);
    if (!fn) return (int64_t)u2_f32_to_reg(0.0f);
    float max_p = *(const float *)(g_u2_image_base + 0x00E6740Cull);
    float res = fn((uint32_t)argv[0], (int32_t)argv[1], 0.0f, max_p);
    return (int64_t)u2_f32_to_reg(res);
}

static int64_t ABI detour_DcCalcStartPhaseRange(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FA70: (calc-start-phase-range ent min-phase max-phase) */
    if (status) *status = 0;
    if (!g_u2_image_base || argc < 3 || !argv) return (int64_t)u2_f32_to_reg(0.0f);
    typedef float (ABI *CalcPhaseFn)(uint32_t ent_id, int32_t max_frames, float min_phase, float max_phase);
    CalcPhaseFn fn = (CalcPhaseFn)(uintptr_t)(g_u2_image_base + 0x0067E600ull);
    if (!fn) return (int64_t)u2_f32_to_reg(0.0f);
    float min_p = u2_reg_to_f32(argv[1]);
    float max_p = u2_reg_to_f32(argv[2]);
    float res = fn((uint32_t)argv[0], -1, min_p, max_p);
    return (int64_t)u2_f32_to_reg(res);
}

static int64_t ABI detour_DcSetWalkMode(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090D4B0: (set-walk-mode ...) - deprecated stub ("This is no longer supported") */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcPlayerRunSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090B180: (player-run-speed [min] [max]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcGuarantee1Bullet(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090C8F0: (guarantee-1-bullet) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    uint8_t *drake = (uint8_t *)u2_get_active_player();
    if (!drake || !g_u2_image_base) return 0;

    uint8_t *inv = *(uint8_t **)(drake + 0x2768);
    if (!inv) return 0;

    uint8_t *vt = *(uint8_t **)inv;
    typedef U2WeaponHandle (ABI *GetActiveWeaponFn)(void *);
    GetActiveWeaponFn get_active = *(GetActiveWeaponFn *)(vt + 0x70);
    U2WeaponHandle h = get_active ? get_active(inv) : (U2WeaponHandle){0, 0};
    if (h.ptr && h.tag != 0) {
        uint8_t *weapon = *(uint8_t **)h.ptr;
        if (weapon && *(const uint32_t *)(weapon + 0x44) == (uint32_t)h.tag) {
            int16_t clip = *(const int16_t *)(weapon + 0x620);
            if (clip == 0) {
                typedef void (ABI *AddAmmoFn)(void *, int32_t, int32_t);
                AddAmmoFn add_ammo = (AddAmmoFn)(uintptr_t)(g_u2_image_base + 0x00AE4930ull);
                if (add_ammo) add_ammo(weapon, 1, 0);
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcForceHairAnim(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00906840: (force-hair-anim [anim-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcPlayerSetCornerClimbDistScale(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090DE90: (player-set-corner-climb-dist-scale scale) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float scale = (argc > 0 && argv) ? u2_reg_to_f32(argv[0]) : 1.0f;
    uint8_t **str_base = (uint8_t **)(g_u2_image_base + 0x0104AD78ull);
    void *str_arg = (str_base && *str_base) ? (void *)(*str_base + 0x12c) : NULL;
    void *t1 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1D290ull);
    void *t2 = (void *)(uintptr_t)(g_u2_image_base + 0x00A1D2A0ull);
    typedef void * (ABI *PropHelperFn)(uint32_t, void *, float *, uint32_t, void *, void *, void *);
    PropHelperFn prop_fn = (PropHelperFn)(uintptr_t)(g_u2_image_base + 0x00A1CD50ull);
    void *obj = prop_fn ? prop_fn(0x9B47D004u, str_arg, &scale, 4, t1, t2, NULL) : NULL;
    if (obj) {
        uint8_t *p = (uint8_t *)obj;
        p[0x61] = (p[0x61] & ~0x27) | 0x04;
        *(uint32_t *)(p + 0x20) = 0x65;
        uint8_t *mgr = g_u2_image_base + 0x01116180ull;
        if (mgr) *(uint32_t *)(p + 0x10) = *(uint32_t *)(mgr + 0x84) + 9;
    }
    return 0;
}

static int64_t ABI detour_DcChooseStumbleAnim(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00907370: (choose-stumble-anim [hit-dir]) -> anim-id */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcCreateBeltAttachment(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090AD30: (create-belt-attachment [item-id]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcDemoResetDefaults(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090DD50: (demo-reset-defaults) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    *(float *)(g_u2_image_base + 0x0104F5A4ull) = 0.08f;
    uint8_t *cfg = g_u2_image_base + 0x01059640ull;
    *(uint32_t *)(cfg + 0x4888) = 0;
    *(uint32_t *)(cfg + 0x4880) = 0;
    *(uint32_t *)(cfg + 0x4884) = 0;
    *(uint64_t *)(cfg + 0x4874) = 0;
    *(uint64_t *)(cfg + 0x486C) = 0;
    *(uint64_t *)(cfg + 0x4864) = 0;
    *(uint64_t *)(cfg + 0x485C) = 0;
    *(uint32_t *)(cfg + 0x48A4) = 2;
    uint8_t *s2 = g_u2_image_base + 0x01136640ull;
    *(uint32_t *)(s2 + 0x1054) = 2;
    uint8_t *s3 = g_u2_image_base + 0x0124A0B4ull;
    *(uint32_t *)(s3 + 0x140) = 1;
    return 0;
}

static int64_t ABI detour_DcPlayerPushForwardSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00908750: (player-push-forward-speed [speed]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcPlayerDisableMelee(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090E9C0: (player-disable-melee) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcEnableDynoJumpFailLooks(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090CA00: (enable-dyno-jump-fail-looks) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcDisableDynoJumpFailLooks(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090CA90: (disable-dyno-jump-fail-looks) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

/* Table of detours provided by this module */
static const U2NativeDetour s_player_detours[] = {
    { "DcFromUps", 0x003976A0, (uintptr_t)detour_DcFromUps },
    { "DcMinmax", 0x009FDBF0, (uintptr_t)detour_DcMinmax },
    { "DcCalcNewPhaseFromSpeed", 0x00A0FBF0, (uintptr_t)detour_DcCalcNewPhaseFromSpeed },
    { "DcCalcStartPhase", 0x00A0FB70, (uintptr_t)detour_DcCalcStartPhase },
    { "DcCalcStartPhaseMaxFrames", 0x00A0FAF0, (uintptr_t)detour_DcCalcStartPhaseMaxFrames },
    { "DcCalcStartPhaseRange", 0x00A0FA70, (uintptr_t)detour_DcCalcStartPhaseRange },
    { "DcDrawReticle", 0x0090E6E0, (uintptr_t)detour_DcDrawReticle },
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
    { "DcRemovePlayerWeapon", 0x00906E10, (uintptr_t)detour_DcRemovePlayerWeapon },
    { "DcPlayerInstantDrawWeapon", 0x0090DDE0, (uintptr_t)detour_DcPlayerInstantDrawWeapon },
    { "DcSetPlayerInfiniteAmmo", 0x0090BA80, (uintptr_t)detour_DcSetPlayerInfiniteAmmo },
    { "DcWeaponSetReset", 0x0090AD00, (uintptr_t)detour_DcWeaponSetReset },
    { "DcHolsterPlayerWeapon", 0x0090D7C0, (uintptr_t)detour_DcHolsterPlayerWeapon },
    { "DcPlayerHideWeapons", 0x0090E4A0, (uintptr_t)detour_DcPlayerHideWeapons },
    { "DcPlayerNoReload", 0x0090EC40, (uintptr_t)detour_DcPlayerNoReload },
    { "DcGivePlayerWeapon", 0x00906B20, (uintptr_t)detour_DcGivePlayerWeapon },
    { "DcPlayerCheckForGotHit", 0x00A0EF20, (uintptr_t)detour_DcPlayerCheckForGotHit },
    { "DcSetWalkMode", 0x0090D4B0, (uintptr_t)detour_DcSetWalkMode },
    { "DcPlayerRunSpeed", 0x0090B180, (uintptr_t)detour_DcPlayerRunSpeed },
    { "DcGuarantee1Bullet", 0x0090C8F0, (uintptr_t)detour_DcGuarantee1Bullet },
    { "DcForceHairAnim", 0x00906840, (uintptr_t)detour_DcForceHairAnim },
    { "DcPlayerSetCornerClimbDistScale", 0x0090DE90, (uintptr_t)detour_DcPlayerSetCornerClimbDistScale },
    { "DcChooseStumbleAnim", 0x00907370, (uintptr_t)detour_DcChooseStumbleAnim },
    { "DcCreateBeltAttachment", 0x0090AD30, (uintptr_t)detour_DcCreateBeltAttachment },
    { "DcDemoResetDefaults", 0x0090DD50, (uintptr_t)detour_DcDemoResetDefaults },
    { "DcPlayerPushForwardSpeed", 0x00908750, (uintptr_t)detour_DcPlayerPushForwardSpeed },
    { "DcPlayerDisableMelee", 0x0090E9C0, (uintptr_t)detour_DcPlayerDisableMelee },
    { "DcEnableDynoJumpFailLooks", 0x0090CA00, (uintptr_t)detour_DcEnableDynoJumpFailLooks },
    { "DcDisableDynoJumpFailLooks", 0x0090CA90, (uintptr_t)detour_DcDisableDynoJumpFailLooks },
};

size_t u2_get_player_detours(const U2NativeDetour **out) {
    if (out) *out = s_player_detours;
    return sizeof(s_player_detours) / sizeof(s_player_detours[0]);
}
