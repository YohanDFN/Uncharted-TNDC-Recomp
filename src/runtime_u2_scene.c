/* Uncharted 2: Scene Actors, Entity Queries, Shipping Stubs & Utilities */
#include "runtime_u2_common.h"

static int64_t ABI detour_DcObjectSpawnedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A12780: (object-spawned? [name-id]) */
    (void)status;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t id = 0;
    if (argc > 0 && argv) {
        id = (uint32_t)argv[0];
        typedef void (ABI *StringIdToStringFn)(uint32_t id, const void *unused);
        StringIdToStringFn str_fn = (StringIdToStringFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_fn) str_fn(id, NULL);
    } else {
        typedef uint32_t (ABI *GetDefaultTargetFn)(void *, void *, int32_t);
        GetDefaultTargetFn *p_fn = (GetDefaultTargetFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_fn && *p_fn) id = (*p_fn)(NULL, NULL, 1);
    }
    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    return (find_fn && find_fn(0, id) != NULL) ? 1 : 0;
}

static int64_t ABI detour_DcEntityName(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A127D0: (entity-name ent) */
    (void)status;
    if (argc <= 0 || !argv || !argv[0]) return 0;
    return *(const uint32_t *)(uintptr_t)(argv[0] + 0x28u);
}

static int64_t ABI detour_DcEntityAutoSpawnOnce(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A12830: (entity-auto-spawn-once) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;

    typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
    typedef uint8_t * (ABI *GetScriptInstanceFn)(void *ctx, uint32_t name_id);

    uint32_t name_id = 0;
    if (argc > 0 && argv) {
        name_id = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(name_id, NULL);
    } else {
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) {
            name_id = (*p_make_sym)(0, NULL, 1);
        }
    }

    void *ss_context = (void *)(g_u2_image_base + 0x01130390ull);
    GetScriptInstanceFn get_inst = (GetScriptInstanceFn)(uintptr_t)(g_u2_image_base + 0x0056C7B0ull);
    if (!get_inst) return 0;

    uint8_t *inst = get_inst(ss_context, name_id);
    if (!inst) return 0;

    uint32_t val = (uint32_t)*(const uint16_t *)(inst + 0x50) | ((uint32_t)*(const uint8_t *)(inst + 0x52) << 16);
    val = (val & 0xFFBFFDu) | 0x4000u;
    *(uint16_t *)(inst + 0x50) = (uint16_t)(val & 0xFFFF);
    *(uint8_t *)(inst + 0x52) = (uint8_t)((val >> 16) & 0xFF);

    return 1;
}

static int64_t ABI detour_DcPartModuleLoadedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A00970: (part-module-loaded? [symbol-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    if (argc > 0 && argv) {
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const void *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) {
            sym = (*p_make_sym)(0, NULL, 1);
        }
    }
    const uint8_t *fx_mgr = g_u2_image_base + 0x013D1E70ull;
    uint32_t count = *(const uint32_t *)(fx_mgr + 0x8C8);
    if (!count) return 0;
    const uint8_t *entry = fx_mgr + 0x298;
    for (uint32_t i = 0; i < count; ++i) {
        if (*(const uint32_t *)entry == sym) {
            return 1;
        }
        entry += 0x30;
    }
    return 0;
}

static int64_t ABI detour_DcDisableFrameForward(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A038A0: (disable-frame-forward [obj-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }
    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    if (!find_fn) return 0;
    uint8_t *ent = (uint8_t *)find_fn(0, sym);
    if (ent && (*(const uint8_t *)(ent + 0x36) & 1)) {
        uint8_t *holder = *(uint8_t **)(ent + 0xB8);
        if (holder) {
            uint32_t count = *(const uint32_t *)holder;
            if (count >= 2) {
                uint8_t *base = *(uint8_t **)(holder + 0x28);
                if (base) {
                    uint8_t *p = base + 0x1DE;
                    for (uint32_t i = count - 1; i > 0; i--) {
                        *p = 1;
                        p += 0x110;
                    }
                }
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcUnparent(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A03940: (unparent [obj-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }
    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    if (!find_fn) return 0;
    uint8_t *ent = (uint8_t *)find_fn(0, sym);
    if (ent && (*(const uint8_t *)(ent + 0x36) & 1)) {
        void **vtable = *(void ***)ent;
        if (vtable) {
            typedef void (ABI *UnparentFn)(void *ent, int32_t zero);
            UnparentFn fn = (UnparentFn)vtable[0x198 / 8];
            if (fn) fn(ent, 0);
        }
    }
    return 0;
}

static int64_t ABI detour_DcEnableGadgetry(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A039C0: (enable-gadgetry [obj-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }
    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    if (!find_fn) return 0;
    uint8_t *ent = (uint8_t *)find_fn(0, sym);
    if (ent && (*(const uint8_t *)(ent + 0x36) & 1)) {
        uint8_t *gadget_holder = *(uint8_t **)(ent + 0xB8);
        if (gadget_holder) {
            uint32_t count = *(const uint32_t *)(gadget_holder + 0x18);
            if (count >= 2) {
                void **gadgets = *(void ***)(gadget_holder + 0x48);
                typedef void (ABI *SetPhysicsDrivenFn)(void *gadget, int32_t state);
                SetPhysicsDrivenFn set_phys = (SetPhysicsDrivenFn)(uintptr_t)(g_u2_image_base + 0x00519370ull);
                if (set_phys) {
                    for (uint32_t i = 1; i < count; i++) {
                        void *gadget = gadgets ? gadgets[i] : NULL;
                        set_phys(gadget, 2);
                    }
                }
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcDisableGadgetry(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A03A70: (disable-gadgetry [obj-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }
    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    if (!find_fn) return 0;
    uint8_t *ent = (uint8_t *)find_fn(0, sym);
    if (ent && (*(const uint8_t *)(ent + 0x36) & 1)) {
        uint8_t *gadget_holder = *(uint8_t **)(ent + 0xB8);
        if (gadget_holder) {
            uint32_t count = *(const uint32_t *)(gadget_holder + 0x18);
            if (count >= 2) {
                void **gadgets = *(void ***)(gadget_holder + 0x48);
                typedef void (ABI *SetPhysicsDrivenFn)(void *gadget, int32_t state);
                SetPhysicsDrivenFn set_phys = (SetPhysicsDrivenFn)(uintptr_t)(g_u2_image_base + 0x00519370ull);
                if (set_phys) {
                    for (uint32_t i = 1; i < count; i++) {
                        void *gadget = gadgets ? gadgets[i] : NULL;
                        set_phys(gadget, 0);
                    }
                }
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcSymbolEqualP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00397620: (symbol-equal? a b) */
    (void)argc; (void)status;
    return ((uint32_t)argv[0] == (uint32_t)argv[1]) ? 1 : 0;
}


static int64_t ABI detour_DcCameraAimOverrideDirection(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0DE40: (camera-aim-override-direction pitch yaw) */
    u2_set_status(status, 0);
    if (argc < 2 || !argv || !g_u2_image_base) return 0;
    float pitch = u2_reg_to_f32(argv[0]);
    float yaw   = u2_reg_to_f32(argv[1]);
    *(uint32_t *)(g_u2_image_base + 0x0147F0BCull) = 2u;
    float *dest = (float *)(g_u2_image_base + 0x0147F090ull);
    dest[0] = pitch;
    dest[1] = yaw;
    return 1;
}

static int64_t ABI detour_DcCameraCancelAimOverride(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A0DE30: (camera-cancel-aim-override) */
    (void)argc; (void)argv; (void)status;
    *(uint32_t *)(g_u2_image_base + 0x0147F0BCull) = 0u;
    return 1;
}

static int64_t ABI detour_DcCameraRestoreLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A0E9B0: (camera-restore-locator) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x0147F0B0ull) = 1u;
    return 0;
}

static int64_t ABI detour_DcCameraAbandon(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A0E5E0: (camera-abandon [name-or-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }

    uint32_t target_id = 0;
    typedef uint8_t * (ABI *GetScriptInstanceFn)(void *ctx, uint32_t name_id);
    GetScriptInstanceFn get_inst = (GetScriptInstanceFn)(uintptr_t)(g_u2_image_base + 0x0056C7B0ull);
    uint8_t *inst = get_inst ? get_inst((void *)(g_u2_image_base + 0x01130390ull), sym) : NULL;
    if (inst) {
        target_id = *(const uint32_t *)(inst + 0x28);
    } else {
        typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
        FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        uint8_t *ent = find_fn ? (uint8_t *)find_fn(0, sym) : NULL;
        if (ent) target_id = *(const uint32_t *)(ent + 0x68);
    }

    if (target_id != 0) {
        const uint8_t *head = g_u2_image_base + 0x0147F050ull;
        const uint8_t *cur = *(const uint8_t * const *)head;
        while (cur && cur != head) {
            if (*(const uint32_t *)(cur + 0x14) == target_id) {
                *(uint8_t *)(cur + 0x34) |= 1;
                break;
            }
            cur = *(const uint8_t * const *)cur;
        }
    }
    return 0;
}

static int64_t ABI detour_DcCameraDisable(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A0E500: (camera-disable [name-or-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }

    uint32_t target_id = 0;
    typedef uint8_t * (ABI *GetScriptInstanceFn)(void *ctx, uint32_t name_id);
    GetScriptInstanceFn get_inst = (GetScriptInstanceFn)(uintptr_t)(g_u2_image_base + 0x0056C7B0ull);
    uint8_t *inst = get_inst ? get_inst((void *)(g_u2_image_base + 0x01130390ull), sym) : NULL;
    if (inst) {
        target_id = *(const uint32_t *)(inst + 0x28);
    } else {
        typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
        FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
        uint8_t *ent = find_fn ? (uint8_t *)find_fn(0, sym) : NULL;
        if (ent) target_id = *(const uint32_t *)(ent + 0x68);
    }
    if (target_id == 0) return 0;

    const uint8_t *head = g_u2_image_base + 0x0147F050ull;
    const uint8_t *cur = *(const uint8_t * const *)(head + 8);
    uint8_t *match = NULL;
    while (cur && cur != head) {
        if (!(*(const uint8_t *)(cur + 0x34) & 1)) {
            if (*(const uint32_t *)(cur + 0x14) == target_id) {
                match = (uint8_t *)cur;
                break;
            }
        }
        cur = *(const uint8_t * const *)(cur + 8);
    }
    if (!match) return 0;

    const uint8_t *fwd = *(const uint8_t * const *)head;
    const uint8_t *active = NULL;
    while (fwd && fwd != head) {
        if (!(*(const uint8_t *)(fwd + 0x34) & 1)) {
            active = fwd;
            break;
        }
        fwd = *(const uint8_t * const *)fwd;
    }

    match[0x34] |= 1;

    if (match == active) {
        typedef void (ABI *CamActivateFn)(uint32_t, int32_t, int32_t);
        CamActivateFn act_fn = (CamActivateFn)(uintptr_t)(g_u2_image_base + 0x00404860ull);
        uint32_t cam_mgr_id = *(const uint32_t *)(g_u2_image_base + 0x0147F0B4ull);
        if (act_fn) act_fn(cam_mgr_id, 1, 0);
    }
    return 0;
}

static int64_t ABI detour_DcSetFocusableGrappled(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F2C0: (set-focusable-grappled [ent] [flag]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }

    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = find_fn ? (uint8_t *)find_fn(0, sym) : NULL;
    if (!ent) return 0;

    typedef uint8_t * (ABI *GetCompFn)(uint8_t *);
    uint8_t **vtable = *(uint8_t ***)ent;
    if (!vtable) return 0;
    GetCompFn get_comp = (GetCompFn)(uintptr_t)vtable[0x88 / 8];
    uint8_t *comp = get_comp ? get_comp(ent) : NULL;
    if (!comp) return 0;

    int32_t flag = (argc >= 2 && argv && argv[1] != 0) ? 1 : 0;
    uint16_t w = *(uint16_t *)(comp + 0x3C);
    w = (uint16_t)((w & ~0x100u) | (flag ? 0x100u : 0u));
    *(uint16_t *)(comp + 0x3C) = w;

    if (sym == 0xCDE4B742u) {
        int32_t game_mode = *(const int32_t *)(g_u2_image_base + 0x014CE97Cull);
        if (game_mode >= 2) {
            uint8_t *net_mgr = *(uint8_t **)(g_u2_image_base + 0x0113B760ull);
            if (net_mgr) {
                *(uint32_t *)(net_mgr + 0x184) = *(const uint32_t *)(g_u2_image_base + 0x01116180ull + 0x204);
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcSplineTransferPendingP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A25A20: (spline-transfer-pending? [ent]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }
    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    if (!find_fn) return 0;
    uint8_t *ent = (uint8_t *)find_fn(0, sym);
    if (!ent || !(*(const uint8_t *)(ent + 0x36) & 1)) return 0;
    const uint8_t *comp = *(const uint8_t * const *)(ent + 0x1B8);
    if (!comp) return 0;
    return (*(const uint32_t *)(comp + 0x60) != 0) ? 1 : 0;
}

static int64_t ABI detour_DcDebugPause(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A102A0: (debug-pause) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x010595E4ull) = 1u;
    return 0;
}

static int64_t ABI detour_DcDiscP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FD910: (disc? n) */
    (void)argc; (void)status;
    uint32_t cur_disc = *(const uint32_t *)(g_u2_image_base + 0x0105CFE8ull);
    return ((uint32_t)argv[0] == cur_disc) ? 1 : 0;
}

static int64_t ABI detour_DcResetAimFov(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x0090D780: (reset-aim-fov) */
    (void)argc; (void)argv; (void)status;
    *(uint32_t *)(g_u2_image_base + 0x0104F8E0ull) = 0xBF800000u; /* -1.0f */
    return 0;
}

static int64_t ABI detour_DcOverrideAimFov(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x0090D7A0: (override-aim-fov fov) */
    (void)argc; (void)status;
    *(uint32_t *)(g_u2_image_base + 0x0104F8E0ull) = (uint32_t)argv[0];
    return 0;
}

static int64_t ABI detour_DcGetHarvestingPriority(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A01AB0: (get-harvesting-priority) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x0110DCB0ull);
}

static int64_t ABI detour_DcSetHarvestingPriority(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A01AE0: (reset-harvesting-priority) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x0110DCB0ull) = 0x0Au;
    return 0;
}

static int64_t ABI detour_DcHarvestingEnabledP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A01AF0: (harvesting-enabled?) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x0110DCA0ull);
}

static int64_t ABI detour_DcSetHarvestingEnabled(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A01B00: (set-harvesting-enabled flag) */
    (void)status;
    if (argc > 0) {
        *(uint8_t *)(g_u2_image_base + 0x0110DCA0ull) = ((uint32_t)argv[0] != 0u) ? 1u : 0u;
    }
    return 0;
}

static int64_t ABI detour_DcGroundPrintsType(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01F00: (ground-prints-type ...) */
    (void)argv;
    u2_set_status(status, 0);
    return (argc > 0) ? 1 : 0;
}

static int64_t ABI detour_DcHideHud(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A0CD00: (hide-hud) */
    (void)argc; (void)argv; (void)status;
    uint8_t *hud = *(uint8_t **)(g_u2_image_base + 0x01130068ull);
    if (hud) *(hud + 0x40Du) = 1u;
    return 0;
}

static int64_t ABI detour_DcIsFma(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A0FE80: (is-fma) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x0105DECDull);
}

static int64_t ABI detour_DcGetDda(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD010: (get-dda) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef float (ABI *GetDdaFn)(void);
    GetDdaFn get_dda = (GetDdaFn)(uintptr_t)(g_u2_image_base + 0x004ABA00ull);
    float dda = get_dda ? get_dda() : 0.0f;
    return (int64_t)u2_f32_to_reg(dda);
}

static int64_t ABI detour_DcJournalClearPage(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x009FD090: (journal-clear-page) */
    (void)argc; (void)argv; (void)status;
    *(int32_t *)(g_u2_image_base + 0x0105DFC0ull) = -1;
    return 0;
}

static int64_t ABI detour_DcJournalGetPage(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x009FD110: (journal-get-page) */
    (void)argc; (void)argv; (void)status;
    return (int64_t)(*(const int32_t *)(g_u2_image_base + 0x0105DFC0ull) + 1);
}

static int64_t ABI detour_DcKillAllParticles(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A0FE60: (kill-all-particles) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x013C2DC4ull) = 1u;
    return 0;
}

static int64_t ABI detour_DcDespawnMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x005DC8C0: (despawn-message id) */
    (void)status;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    if (argc > 0 && argv) {
        void *mgr = (void *)(g_u2_image_base + 0x01139EA0ull);
        typedef void (ABI *DespawnMsgFn)(void *mgr, uint32_t msg_id);
        DespawnMsgFn despawn_fn = (DespawnMsgFn)(uintptr_t)(g_u2_image_base + 0x005D9D80ull);
        if (despawn_fn) {
            despawn_fn(mgr, (uint32_t)argv[0]);
        }
    }
    return 0;
}

static int64_t ABI detour_DcPauseCoopCombo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00667F60: (pause-coop-combo) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    void *mgr = (void *)(g_u2_image_base + 0x01139FE0ull);
    typedef void (ABI *PauseCoopComboFn)(void *mgr);
    PauseCoopComboFn pause_fn = (PauseCoopComboFn)(uintptr_t)(g_u2_image_base + 0x005DE7D0ull);
    if (pause_fn) {
        pause_fn(mgr);
    }
    return 0;
}

static int64_t ABI detour_DcGetCaptureZone(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006687D0: (get-capture-zone) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcNetEndWave(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006687E0: (net-end-wave) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef void (ABI *NetEndWaveFn)(void);
    NetEndWaveFn end_wave = (NetEndWaveFn)(uintptr_t)(g_u2_image_base + 0x00617D50ull);
    if (end_wave) {
        end_wave();
    }
    return 0;
}

static int64_t ABI detour_DcNetGetGrappleTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006687F0: (net-get-grapple-time) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef uint64_t* (ABI *LookupSymbolValueFn)(uint32_t hash);
    LookupSymbolValueFn lookup_fn = (LookupSymbolValueFn)(uintptr_t)(g_u2_image_base + 0x00A15E70ull);
    if (lookup_fn) {
        uint64_t *val_ptr = lookup_fn(0x9D87D2DBu);
        if (val_ptr) {
            return (int64_t)(*val_ptr);
        }
    }
    return 0;
}

static int64_t ABI detour_DcNetGetBoosterEnabled(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00668810: (net-get-booster-enabled booster-id) */
    (void)status;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t booster_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0u;
    typedef uint8_t (ABI *NetIsBoosterEnabledFn)(uint64_t dummy, uint32_t id);
    NetIsBoosterEnabledFn is_enabled = (NetIsBoosterEnabledFn)(uintptr_t)(g_u2_image_base + 0x005F6080ull);
    return is_enabled ? (int64_t)is_enabled(0, booster_id) : 0;
}

static int64_t ABI detour_DcNetGetBoosterAmount(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00668820: (net-get-booster-amount booster-id) */
    (void)status;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t booster_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0u;
    void *mgr = (void *)(g_u2_image_base + 0x0113A788ull);
    typedef float (ABI *GetBoosterAmountFn)(void *mgr, uint32_t id);
    GetBoosterAmountFn get_amount = (GetBoosterAmountFn)(uintptr_t)(g_u2_image_base + 0x005F6410ull);
    float amount = get_amount ? get_amount(mgr, booster_id) : 0.0f;
    return (int64_t)u2_f32_to_reg(amount);
}

static int64_t ABI detour_DcNetCoopTask(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006697E0: (net-coop-task) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef uint64_t* (ABI *LookupSymbolValueFn)(uint32_t hash);
    LookupSymbolValueFn lookup_fn = (LookupSymbolValueFn)(uintptr_t)(g_u2_image_base + 0x00A15E70ull);
    if (lookup_fn) {
        lookup_fn(0x3B9A067Du);
    }
    return 0;
}

static int64_t ABI detour_DcNetPlyaerCoopId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669240: (net-plyaer-coop-id) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcSetForceTension(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00667EE0: (set-force-tension flag) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    int32_t mode = *(const int32_t *)(g_u2_image_base + 0x014CE97Cull);
    if (mode > 1) {
        uint8_t flag = (argc > 0 && argv && argv[0] != 0) ? 1 : 0;
        *(uint8_t *)(g_u2_image_base + 0x0113B760ull + 0x21C8) = flag;
    }
    return 0;
}

static int64_t ABI detour_DcNetKillTerritory(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669800: (net-kill-territory) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;

    int32_t game_mode = *(const int32_t *)(g_u2_image_base + 0x014CE97Cull);
    if (game_mode < 2) return 0;

    const uint8_t *cfg = g_u2_image_base + 0x01065B08ull;
    if (*(void * const *)(cfg + 0x10) != NULL) {
        uint64_t val = *(const uint64_t *)(cfg + 0xB00);
        uint32_t lo = (uint32_t)val;
        uint32_t hi = (uint32_t)(val >> 32);
        if (lo != hi) return 0;
    }

    void *net_mgr = (void *)(g_u2_image_base + 0x0113B760ull);
    typedef void (ABI *KillTerritoryFn)(void *mgr);
    KillTerritoryFn fn = (KillTerritoryFn)(uintptr_t)(g_u2_image_base + 0x0064C330ull);
    if (fn) fn(net_mgr);

    return 0;
}

static int64_t ABI detour_DcNetSpawnTerritory(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669840: (net-spawn-territory) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;

    void *net_mgr = (void *)(g_u2_image_base + 0x0113B760ull);
    uint32_t count = *(const uint32_t *)((const uint8_t *)net_mgr + 0x1DD0);
    const uint8_t *entry = (const uint8_t *)net_mgr + 0x1D88;
    void *found_ent = NULL;

    for (uint32_t i = 0; i < count; i++, entry += 0x10) {
        void **p_target = *(void ***)(entry - 8);
        if (!p_target) continue;
        uint32_t expected_tag = *(const uint32_t *)entry;
        if (!expected_tag) continue;
        uint8_t *obj = (uint8_t *)(*p_target);
        if (!obj || *(const uint32_t *)(obj + 0x44) != expected_tag) continue;
        if (*(const int32_t *)(obj + 0x250) == -1) {
            found_ent = obj;
            break;
        }
    }

    typedef void (ABI *SpawnTerritoryFn)(void *mgr, void *ent);
    SpawnTerritoryFn fn = (SpawnTerritoryFn)(uintptr_t)(g_u2_image_base + 0x0064BF20ull);
    if (fn) fn(net_mgr, found_ent);

    return 0;
}

static int64_t ABI detour_DcNetSpawnFlag(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006698C0: (net-spawn-flag ...) */
    (void)argc;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    void *mgr = (void *)(g_u2_image_base + 0x0113B760ull);
    typedef void (ABI *NetSpawnFlagFn)(void *mgr, uint64_t *argv, uint32_t flag);
    NetSpawnFlagFn spawn_fn = (NetSpawnFlagFn)(uintptr_t)(g_u2_image_base + 0x0063E1B0ull);
    if (spawn_fn) {
        spawn_fn(mgr, argv, 0u);
    }
    return 0;
}

static int64_t ABI detour_DcNetRespawnFlag(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x006698E0: (net-respawn-flag [flag-name]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }
    if (!sym) return 0;
    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    if (!find_fn) return 0;
    uint8_t *ent = (uint8_t *)find_fn(0, sym);
    if (ent && *(const uint32_t *)(ent + 0x30) == 0x310F465Eu && (*(const uint8_t *)(ent + 0x428) & 2)) {
        void *mgr = (void *)(g_u2_image_base + 0x0113B760ull);
        typedef void (ABI *RespawnFn)(void *mgr, void *ent);
        RespawnFn respawn_fn = (RespawnFn)(uintptr_t)(g_u2_image_base + 0x00649700ull);
        if (respawn_fn) respawn_fn(mgr, ent);
    }
    return 0;
}

static int64_t ABI detour_DcNetCoopTeamFailedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00669C20: (net-coop-team-failed?) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x0113D910ull);
}

static int64_t ABI detour_DcNetCoopScriptId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C30: (net-coop-script-id) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcNetCoopHostScriptId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C40: (net-coop-host-script-id) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcNetIsCinema(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C50: (net-is-cinema) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    int32_t net_mode = *(const int32_t *)(g_u2_image_base + 0x014CE97Cull);
    if (net_mode < 2) return 0;
    return (*(const uint8_t *)(g_u2_image_base + 0x0113B760ull + 0x209C) != 0) ? 1 : 0;
}

static int64_t ABI detour_DcNetIsHost(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C80: (net-is-host) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    int32_t net_mode = *(const int32_t *)(g_u2_image_base + 0x014CE97Cull);
    if (net_mode < 2) return 0;
    const uint8_t *net_ctx = g_u2_image_base + 0x01065B08ull;
    if (*(const uint64_t *)(net_ctx + 0x10) == 0) return 1;
    uint64_t ids = *(const uint64_t *)(net_ctx + 0xB00);
    uint32_t low = (uint32_t)ids;
    uint32_t high = (uint32_t)(ids >> 32);
    return (low == high) ? 1 : 0;
}

static int64_t ABI detour_DcPsnDemoShowUpsell(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x009FD590: (psn-demo-show-upsell) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x0113769Dull) = 1u;
    return 0;
}

static int64_t ABI detour_DcDebugDrawCross(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE070: (debug-draw-cross ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcAutoSaveGame(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE820: (auto-save-game) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint8_t enabled = *(const uint8_t *)(g_u2_image_base + 0x0105053Dull);
    if (!enabled) return 0;
    void *save_mgr = (void *)(g_u2_image_base + 0x01050E68ull);
    int32_t slot = *(const int32_t *)((const uint8_t *)save_mgr + 0x1FC);
    if (slot < 0) return 0;
    typedef void (ABI *SaveReqFn)(void *mgr, uint32_t action, int32_t slot, uint32_t mode, uint32_t flags, int32_t extra);
    SaveReqFn save_fn = (SaveReqFn)(uintptr_t)(g_u2_image_base + 0x00CD2800ull);
    if (save_fn) {
        save_fn(save_mgr, 1, 10, 0, 4, -1);
        save_fn(save_mgr, 1, slot, 0, 0, -1);
    }
    return 0;
}

static int64_t ABI detour_DcUpdateTaskMenu(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE8C0: (update-task-menu) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    void **task_menu_ptr = (void **)(g_u2_image_base + 0x0153BD78ull);
    if (task_menu_ptr && *task_menu_ptr) {
        void *menu = *task_menu_ptr;
        void **vtable = *(void ***)menu;
        if (vtable) {
            typedef void (ABI *MenuFn)(void *this_ptr);
            MenuFn fn20 = (MenuFn)vtable[4]; /* 0x20 / 8 = 4 */
            if (fn20) fn20(menu);
            menu = *task_menu_ptr;
            if (menu) {
                vtable = *(void ***)menu;
                if (vtable) {
                    MenuFn fn38 = (MenuFn)vtable[7]; /* 0x38 / 8 = 7 */
                    if (fn38) fn38(menu);
                }
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcGameTaskGraphLogin(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FE8F0: (game-task-graph-login task) */
    (void)status;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    if (argc > 0 && argv) {
        typedef void (ABI *GameTaskGraphLoginFn)(uint64_t task);
        GameTaskGraphLoginFn login_fn = (GameTaskGraphLoginFn)(uintptr_t)(g_u2_image_base + 0x00A59960ull);
        if (login_fn) {
            login_fn(argv[0]);
        }
    }
    return 0;
}

static int64_t ABI detour_DcRemoveDynamicLight(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FEDF0: (remove-dynamic-light id) */
    (void)status;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    if (argc > 0 && argv) {
        uint32_t id = (uint32_t)argv[0];
        typedef void (ABI *StringIdToStringFn)(uint32_t id, const void *unused);
        StringIdToStringFn str_fn = (StringIdToStringFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_fn) str_fn(id, NULL);
        typedef void (ABI *RemoveLightFn)(uint32_t id);
        RemoveLightFn remove_fn = (RemoveLightFn)(uintptr_t)(g_u2_image_base + 0x00B7A470ull);
        if (remove_fn) remove_fn(id);
    }
    return 0;
}

static int64_t ABI detour_DcRestoreBlur(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FFF20: (restore-blur) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    const uint8_t *ctx = g_u2_image_base + 0x0115BD20ull;
    void **p_target = *(void ***)ctx;
    if (!p_target) return 0;
    uint32_t tag = *(const uint32_t *)(ctx + 8);
    if (!tag) return 0;
    uint8_t *obj = (uint8_t *)(*p_target);
    if (!obj || *(const uint32_t *)(obj + 0x44) != tag) return 0;
    typedef void (ABI *BlurFn)(void *blur_obj);
    BlurFn restore_fn = (BlurFn)(uintptr_t)(g_u2_image_base + 0x00974DE0ull);
    if (restore_fn) restore_fn(obj);
    return 0;
}

static int64_t ABI detour_DcDeactivateBlur(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FFF50: (deactivate-blur) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    const uint8_t *ctx = g_u2_image_base + 0x0115BD20ull;
    void **p_target = *(void ***)ctx;
    if (!p_target) return 0;
    uint32_t tag = *(const uint32_t *)(ctx + 8);
    if (!tag) return 0;
    uint8_t *obj = (uint8_t *)(*p_target);
    if (!obj || *(const uint32_t *)(obj + 0x44) != tag) return 0;
    typedef void (ABI *BlurFn)(void *blur_obj);
    BlurFn deact_fn = (BlurFn)(uintptr_t)(g_u2_image_base + 0x00974AB0ull);
    if (deact_fn) deact_fn(obj);
    return 0;
}

static int64_t ABI detour_DcRailSetIgcFadeOutTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
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

static int64_t ABI detour_DcKillAllDroppedWeapons(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F1B0: (kill-all-dropped-weapons) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    const uint8_t *entity_mgr = g_u2_image_base + 0x0115F1A0ull;
    const void *head = *(const void * const *)(entity_mgr + 0xC90);
    typedef void * (ABI *GetNextEntityFn)(const void *cur, int32_t mode);
    typedef void (ABI *KillEntityFn)(void *ent);
    typedef uint32_t (ABI *MakeSymbolFn)(uint32_t rdi, const char *name, int32_t flag);

    GetNextEntityFn get_next = (GetNextEntityFn)(uintptr_t)(g_u2_image_base + 0x009BCDF0ull);
    KillEntityFn kill_ent = (KillEntityFn)(uintptr_t)(g_u2_image_base + 0x009BE760ull);
    MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);

    if (!get_next || !kill_ent || !p_make_sym || !*p_make_sym) return 0;

    MakeSymbolFn make_sym = *p_make_sym;
    const char *dropped_str = (const char *)(g_u2_image_base + 0x00EAB13Full);

    void *cur = get_next(head, 0);
    while (cur) {
        uint16_t flags = *(const uint16_t *)((const uint8_t *)cur + 0x34);
        if ((flags & 1) && *(const uint32_t *)((const uint8_t *)cur + 0x30) == 0x9DF97F47u) {
            uint32_t sym = make_sym(0, dropped_str, 1);
            const uint8_t *c50 = *(const uint8_t * const *)((const uint8_t *)cur + 0x50);
            if (c50) {
                const uint8_t *sub = *(const uint8_t * const *)c50;
                if (sub) {
                    const uint8_t *p10 = *(const uint8_t * const *)(sub + 0x10);
                    if (p10 && *(const uint32_t *)(p10 + 0x18) == sym) {
                        kill_ent(cur);
                    }
                }
            }
        }
        cur = get_next(cur, 0);
    }
    return 0;
}

static int64_t ABI detour_DcKillAllBreakables(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F260: (kill-all-breakables) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    const uint8_t *entity_mgr = g_u2_image_base + 0x0115F1A0ull;
    const void *head = *(const void * const *)(entity_mgr + 0xC58);
    typedef void * (ABI *GetNextEntityFn)(const void *cur, int32_t mode);
    typedef void (ABI *KillEntityFn)(void *ent);
    GetNextEntityFn get_next = (GetNextEntityFn)(uintptr_t)(g_u2_image_base + 0x009BCDF0ull);
    KillEntityFn kill_ent = (KillEntityFn)(uintptr_t)(g_u2_image_base + 0x009BE760ull);
    if (!get_next || !kill_ent) return 0;
    void *cur = get_next(head, 0);
    while (cur) {
        uint16_t flags = *(const uint16_t *)((const uint8_t *)cur + 0x34);
        if ((flags & 1) && *(const uint32_t *)((const uint8_t *)cur + 0x30) == 0x8B7D6F1Au) {
            kill_ent(cur);
        }
        cur = get_next(cur, 0);
    }
    return 0;
}

static int64_t ABI detour_DcTutorialHintsOnP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A0F490: (tutorial-hints-on?) */
    (void)argc; (void)argv; (void)status;
    return (int64_t)(*(const int32_t *)(g_u2_image_base + 0x0105DEF0ull));
}

static int64_t ABI detour_DcUseWaterUnitForReflectionIfVisible(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A02C50: (use-water-unit-for-reflection-if-visible) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x011649DDull) = 1u;
    return 1;
}

static int64_t ABI detour_DcAngleMod(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00397C50: (animate ...) */
    (void)status;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, int32_t, const char *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00396180ull);
    return fn(argc, argv, 0, "animate");
}

static int64_t ABI detour_DcActionPackEnabledP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FD060: (journal-set-ambient-scale [scale]) */
    (void)status;
    uint32_t bits = (argc > 0) ? (uint32_t)argv[0] : 0x3F800000u; /* 1.0f */
    *(uint32_t *)(g_u2_image_base + 0x01059640ull + 0x4984ull) = bits;
    return 0;
}

static int64_t ABI detour_DcKillMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x005DC630: (kill-message id) */
    (void)argc; (void)status;
    typedef void (ABI *fn_t)(void *, uint32_t);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x005D9D80ull);
    fn(g_u2_image_base + 0x01139EA0ull, (uint32_t)argv[0]);
    return 0;
}

static int64_t ABI detour_DcPowFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FDB60: (pow-float base exp) */
    (void)argc; (void)status;
    float b = u2_reg_to_f32(argv[0]);
    float e = u2_reg_to_f32(argv[1]);
    return (int64_t)u2_f32_to_reg(powf(b, e));
}

static int64_t ABI detour_DcLerpScale(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00AB8F60: (rail-kill-all-physics-objects) */
    (void)argc; (void)argv; (void)status;
    typedef void (ABI *fn_t)(void);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x0000EB10ull);
    fn();
    return 0;
}

static int64_t ABI detour_DcRailMode(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB8610: (rail-mode mode) */
    (void)status;
    if (argc > 0) {
        *(uint32_t *)(g_u2_image_base + 0x0153FCB8ull) = (uint32_t)argv[0];
    }
    return 0;
}

static int64_t ABI detour_DcSetSwimDragMultiplier(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x0090D650: (set-swim-drag-multiplier mult) */
    (void)status;
    if (argc == 1) {
        /* vmovss [rip+12650275] @ 0x0090D65D (8 bytes) -> 0x0090D665 + 0xC10823 = 0x0151DE88 */
        *(uint32_t *)(g_u2_image_base + 0x0151DE88ull) = (uint32_t)argv[0];
    }
    return 0;
}

static int64_t ABI detour_DcStateCharacterAccumulatesSnowFromGround(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A01FE0: (state-character-accumulates-snow-from-ground flag) */
    (void)status;
    if (argc <= 0) return 0;
    *(uint8_t *)(g_u2_image_base + 0x01117570ull + 0x1C8ull) = (argv[0] != 0) ? 1u : 0u;
    return 1;
}

static int64_t ABI detour_DcStateCharacterAccumulatesSnowFromSky(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A01F30: (state-character-accumulates-wetness-from-water flag) */
    (void)status;
    if (argc <= 0) return 0;
    *(uint8_t *)(g_u2_image_base + 0x01117570ull + 0x1CAull) = (argv[0] != 0) ? 1u : 0u;
    return 1;
}

static int64_t ABI detour_DcSetMaxCharacterWetness(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02510: (set-max-character-wetness val) */
    (void)status;
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    *(float *)(g_u2_image_base + 0x01117570ull + 0x1CCull) = u2_reg_to_f32(argv[0]);
    return 1;
}

static int64_t ABI detour_DcSetTimeToDryPeriod(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02780: (set-time-to-dry-period val) */
    (void)status;
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    float val = u2_reg_to_f32(argv[0]);
    uint8_t *mgr = g_u2_image_base + 0x01117570ull;
    *(float *)(mgr + 0x58ull) = val;
    *(float *)(mgr + 0x98ull) = val;
    *(float *)(mgr + 0xD8ull) = val;
    *(float *)(mgr + 0x118ull) = val;
    *(float *)(mgr + 0x158ull) = val;
    *(float *)(mgr + 0x198ull) = val;
    return 1;
}

static int64_t ABI detour_DcForceReflections(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02960: (force-reflections flag) */
    (void)status;
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    *(uint8_t *)(g_u2_image_base + 0x01164980ull + 0x5Cull) = (argv[0] != 0) ? 1u : 0u;
    return 1;
}

static int64_t ABI detour_DcUseReflectionPlane(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A029A0: (use-reflection-plane nx ny nz d) */
    (void)status;
    if (argc < 4 || !argv || !g_u2_image_base) return 0;
    uint8_t *refl = g_u2_image_base + 0x01164980ull;
    refl[0x14] = 1;
    *(uint32_t *)(refl + 0x18) = 5;
    float *plane = (float *)(refl + 0x30);
    plane[0] = u2_reg_to_f32(argv[0]);
    plane[1] = u2_reg_to_f32(argv[1]);
    plane[2] = u2_reg_to_f32(argv[2]);
    plane[3] = u2_reg_to_f32(argv[3]);
    return 1;
}

static int64_t ABI detour_DcUseVisibleWaterUnitForReflectionWithTolerance(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02B70: (use-visible-water-unit-for-reflection-with-tolerance flag tol) */
    (void)status;
    if (argc < 2 || !argv || !g_u2_image_base) return 0;
    if (argv[0] != 0) {
        uint8_t *refl = g_u2_image_base + 0x01164980ull;
        refl[0x14] = 1;
        *(uint32_t *)(refl + 0x18) = 2;
    }
    return 1;
}

static int64_t ABI detour_DcUseVisibleWaterUnitForReflection(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02BC0: (use-visible-water-unit-for-reflection flag) */
    (void)status;
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    if (argv[0] != 0) {
        uint8_t *refl = g_u2_image_base + 0x01164980ull;
        refl[0x14] = 1;
        *(uint32_t *)(refl + 0x18) = 2;
    }
    return 1;
}

static int64_t ABI detour_DcUseReflections(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02C10: (use-reflections flag) */
    (void)status;
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    *(uint8_t *)(g_u2_image_base + 0x01164980ull + 0x14ull) = (argv[0] != 0) ? 1u : 0u;
    return 1;
}

static int64_t ABI detour_DcSetRainRipplesState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A027F0: (set-rain-ripples-state flag) */
    (void)status;
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    *(uint8_t *)(g_u2_image_base + 0x01164980ull + 0x5Full) = (argv[0] != 0) ? 1u : 0u;
    return 1;
}

static int64_t ABI detour_DcStopScriptRumbles(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x0090D760: (swim-disable) */
    typedef void (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00924040ull);
    fn(argc, argv, status);
    return 0;
}

static int64_t ABI detour_DcSwimDisallowStandOnBottom(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x0090D680: (swim-disallow-stand-on-bottom) */
    (void)argc; (void)argv; (void)status;
    typedef void (ABI *fn_t)(uint64_t, void *, uint64_t);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00A1D2D0ull);
    uint64_t ctx = *(const uint64_t *)(g_u2_image_base + 0x01161EE8ull);
    fn(0, g_u2_image_base + 0x0104F810ull + 0x5Eull, ctx);
    return 0;
}

static int64_t ABI detour_DcSwimEnable(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x0090D770: (swim-enable) */
    typedef void (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00924140ull);
    fn(argc, argv, status);
    return 0;
}

static int64_t ABI detour_DcSyncVoxMetaData(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x009FD1F0: (sync-vox-meta-data) */
    (void)argc; (void)argv; (void)status;
    typedef void (ABI *fn_t)(uint64_t);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x003E2AC0ull);
    fn(0);
    return 0;
}

static int64_t ABI detour_DcSyncVoxMpMetaData(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x009FD1E0: (sync-vox-mp-meta-data) */
    typedef void (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x003E3320ull);
    fn(argc, argv, status);
    return 0;
}

static int64_t ABI detour_DcTaskSubnodeActiveP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FEC80: (task-subnode-active? subnode) */
    (void)argc; (void)status;
    typedef uint8_t (ABI *fn_t)(uint64_t);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00A5D6E0ull);
    return (int64_t)fn(argv[0]);
}

static int64_t ABI detour_DcElapsedTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!argv) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
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
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcRenderSettingsApplyDefaults(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10760: (render-settings-apply-defaults [id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t id = (uint32_t)argv[0];
    *(uint32_t *)(g_u2_image_base + 0x0104FAECull) = id;
    uint8_t flag = *(const uint8_t *)(g_u2_image_base + 0x01059640ull + 0x488Dull);
    uint32_t sym = (flag != 0) ? 0x30DE052Du : 0x9EC73B22u;
    typedef void * (ABI *GetSettingsFn)(uint32_t, uint32_t);
    GetSettingsFn get_fn = (GetSettingsFn)(uintptr_t)(g_u2_image_base + 0x00A17020ull);
    void *src = get_fn ? get_fn(id, sym) : NULL;
    if (src) {
        memcpy(g_u2_image_base + 0x01162820ull, src, 0x910);
        typedef void (ABI *VoidFn)(void);
        VoidFn update_fn = (VoidFn)(uintptr_t)(g_u2_image_base + 0x009D7150ull);
        if (update_fn) update_fn();
    }
    return 1;
}

static int64_t ABI detour_DcSetObjectHealth(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A04A20: (set-object-health [ent] [f_health]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }
    if (sym == 0) return 0;

    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = find_fn ? (uint8_t *)find_fn(0, sym) : NULL;
    if (!ent) return 0;

    if (ent[0x36] & 4) {
        uint8_t *comp = *(uint8_t **)(ent + 0x1F0);
        if (comp) {
            uint8_t **vtable = *(uint8_t ***)comp;
            if (vtable) {
                typedef int32_t (ABI *GetMaxHpFn)(uint8_t *);
                GetMaxHpFn get_max = (GetMaxHpFn)(uintptr_t)vtable[0x50 / 8];
                int32_t max_hp = get_max ? get_max(comp) : 0;
                float f_health = (argc >= 2 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
                int32_t new_hp = (int32_t)((float)max_hp * f_health);
                typedef void (ABI *SetHpFn)(uint8_t *, int32_t);
                SetHpFn set_hp = (SetHpFn)(uintptr_t)vtable[0x20 / 8];
                if (set_hp) set_hp(comp, new_hp);
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcGetObjectHealth(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A04B10: (get-object-health [ent]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return (int64_t)u2_f32_to_reg(0.0f);
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }
    if (sym == 0) return (int64_t)u2_f32_to_reg(0.0f);

    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = find_fn ? (uint8_t *)find_fn(0, sym) : NULL;
    if (!ent) return (int64_t)u2_f32_to_reg(0.0f);

    float health = 0.0f;
    if (*(const uint32_t *)(ent + 0x30) == 0x8B7D6F1Au) {
        float max_hp = (float)*(const int32_t *)(ent + 0x22C);
        float cur_hp = (float)*(const int32_t *)(ent + 0x228);
        health = (max_hp != 0.0f) ? (cur_hp / max_hp) : 0.0f;
    } else if (ent[0x36] & 4) {
        uint8_t *comp = *(uint8_t **)(ent + 0x1F0);
        if (comp) {
            uint8_t **vtable = *(uint8_t ***)comp;
            if (vtable) {
                typedef float (ABI *GetHealthFn)(uint8_t *);
                GetHealthFn get_h = (GetHealthFn)(uintptr_t)vtable[0x78 / 8];
                if (get_h) health = get_h(comp);
            }
        }
    }
    return (int64_t)u2_f32_to_reg(health);
}

static int64_t ABI detour_DcSpawnParticlesAtJoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A00950: (spawn-particles-at-joint ...) */
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, int32_t, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x009F2E80ull);
    return fn(argc, argv, 2, status);
}

static int64_t ABI detour_DcSpawnParticlesAttached(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A00960: (spawn-particles-attached ...) */
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, int32_t, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x009F2E80ull);
    return fn(argc, argv, 1, status);
}

static int64_t ABI detour_DcNetHideWaveScreen(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00668850: (net-hide-wave-screen) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    if (*(const int32_t *)(g_u2_image_base + 0x014CE97Cull) > 1) {
        *(uint8_t *)(g_u2_image_base + 0x0104F544ull + 0x1C) = 0;
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcStringFromFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FDE60: (string-from-float f) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    double val = 0.0;
    if (argc > 0 && argv) {
        val = (double)u2_reg_to_f32(argv[0]);
    }
    char *buf = (char *)(g_u2_image_base + 0x01533EA0ull);
    snprintf(buf, 0x1FF, "%g", val);
    return (int64_t)(uintptr_t)buf;
}

static int64_t ABI detour_DcDebugDrawLine(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE090: (debug-draw-line ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcPlayMovie(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FE100: (play-movie movie-name) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint8_t *mgr = *(uint8_t **)(g_u2_image_base + 0x010608E0ull);
    if (mgr) {
        const void *const *vtable = *(const void *const **)mgr;
        if (vtable) {
            typedef uint8_t (ABI *IsPlayingFn)(void *);
            typedef void (ABI *PlayFn)(void *, const char *, int32_t, float);
            IsPlayingFn is_playing = (IsPlayingFn)vtable[0x38 / 8];
            PlayFn play_fn = (PlayFn)vtable[0x10 / 8];
            if (is_playing && !is_playing(mgr) && play_fn) {
                const char *name = (argc > 0 && argv) ? (const char *)argv[0] : NULL;
                play_fn(mgr, name, 0, 0.0f);
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcStartRumble(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x009FF3D0: (start-rumble [symbol]) */
    u2_set_status(status, 0);
    if (argc > 0 && argv && g_u2_image_base) {
        uint32_t sym = (uint32_t)argv[0];
        typedef void (ABI *RumbleFn)(uint32_t, int32_t, float, float);
        RumbleFn fn = (RumbleFn)(uintptr_t)(g_u2_image_base + 0x009EAFB0ull);
        if (fn) fn(sym, 1, 1.0f, 1.0f);
    }
    return 0;
}

static int64_t ABI detour_DcStopRumble(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FF370: (stop-rumble [symbol]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcNetAllPlayersDeadP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006694B0: (net-all-players-dead?) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t out_val = 0;
    typedef uint8_t (ABI *fn_t)(void *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x0063E040ull);
    return fn ? (fn((void *)(g_u2_image_base + 0x0113B760ull), &out_val) ? 1 : 0) : 0;
}

static int64_t ABI detour_DcDebugDrawString(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDFB0: (debug-draw-string ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcSetCharacterWetMaskState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02690: (set-character-wet-mask-state idx state) */
    u2_set_status(status, 0);
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    uint32_t idx = (uint32_t)argv[0];
    if (idx <= 5) {
        uint8_t state = (argc > 1 && argv[1] != 0) ? 1 : 0;
        *(uint8_t *)(g_u2_image_base + 0x01117570ull + (uint64_t)idx * 64u + 0x6C) = state;
    }
    return 1;
}

static int64_t ABI detour_DcUseWaterUnitForReflection(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02C70: (use-water-unit-for-reflection unit flag) */
    u2_set_status(status, 0);
    if (argc < 2 || !argv || !g_u2_image_base) return 0;
    if (argv[1] != 0) {
        uint32_t unit = (uint32_t)argv[0];
        uint8_t *slot = g_u2_image_base + 0x01164980ull;
        slot[0x14] = 1;
        *(uint32_t *)(slot + 0x18) = 3;
        *(uint32_t *)(slot + 0x1C) = unit;
    }
    return 1;
}

static int64_t ABI detour_DcLerpReflectionPlaneOfWaterUnits(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02B10: (lerp-reflection-plane-of-water-units u1 u2 t) */
    u2_set_status(status, 0);
    if (argc < 2 || !argv || !g_u2_image_base) return 0;
    if (argv[0] != 0) {
        uint8_t *slot = g_u2_image_base + 0x01164980ull;
        slot[0x14] = 1;
        *(uint32_t *)(slot + 0x18) = 4;
        *(uint32_t *)(slot + 0x1C) = (argc > 1) ? (uint32_t)argv[1] : 0;
        *(uint32_t *)(slot + 0x20) = (argc > 2) ? (uint32_t)argv[2] : 0;
        *(float *)(slot + 0x24) = (argc > 3) ? u2_reg_to_f32(argv[3]) : 0.0f;
    }
    return 1;
}

static int64_t ABI detour_DcRemoveCoverActionPacksFromObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04DC0: (remove-cover-action-packs-from-object obj-id) */
    u2_set_status(status, 0);
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    typedef void *(ABI *get_ent_fn)(uint32_t);
    get_ent_fn get_ent = (get_ent_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *obj = (uint8_t *)get_ent((uint32_t)argv[0]);
    if (obj && (*(const uint32_t *)(obj + 0x36) & 0x100u)) {
        typedef void (ABI *remove_fn)(void *);
        remove_fn fn = (remove_fn)(uintptr_t)(g_u2_image_base + 0x00998BD0ull);
        if (fn) fn(obj);
    }
    return 0;
}

static int64_t ABI detour_DcRegenerateCoverActionPacksFromObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04E20: (regenerate-cover-action-packs-from-object obj-id) */
    u2_set_status(status, 0);
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    typedef void *(ABI *get_ent_fn)(uint32_t);
    get_ent_fn get_ent = (get_ent_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *obj = (uint8_t *)get_ent((uint32_t)argv[0]);
    if (obj && (*(const uint32_t *)(obj + 0x36) & 0x100u)) {
        typedef void (ABI *regen_fn)(void *);
        regen_fn fn = (regen_fn)(uintptr_t)(g_u2_image_base + 0x009986C0ull);
        if (fn) fn(obj);
    }
    return 0;
}

static int64_t ABI detour_DcEnableNavBlockerObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04F60: (enable-nav-blocker-object obj-id) */
    u2_set_status(status, 0);
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    typedef void *(ABI *get_ent_fn)(uint32_t);
    get_ent_fn get_ent = (get_ent_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *obj = (uint8_t *)get_ent((uint32_t)argv[0]);
    if (obj && (*(const uint32_t *)(obj + 0x36) & 0x100u)) {
        *(uint8_t *)(obj + 0x154) |= 0x4u;
    }
    return 0;
}

static int64_t ABI detour_DcBlockNavPolyUnderObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A05030: (block-nav-poly-under-object obj-id) */
    u2_set_status(status, 0);
    if (argc <= 0 || !argv || !g_u2_image_base) return 0;
    typedef void *(ABI *get_ent_fn)(uint32_t);
    get_ent_fn get_ent = (get_ent_fn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *obj = (uint8_t *)get_ent((uint32_t)argv[0]);
    if (obj && (*(const uint32_t *)(obj + 0x36) & 0x100u)) {
        typedef void (ABI *block_fn)(void *, int32_t);
        block_fn fn = (block_fn)(uintptr_t)(g_u2_image_base + 0x00990A80ull);
        if (fn) fn(obj, 1);
    }
    return 0;
}

static int64_t ABI detour_DcKillEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A09D60: (kill-entity [ent-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = 0;
    if (argc > 0 && argv) {
        ent_id = (uint32_t)argv[0];
    } else {
        typedef uint32_t (ABI *self_fn)(uint32_t, uint32_t, uint32_t);
        self_fn get_self = *(self_fn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (get_self) ent_id = get_self(0, 0, 1);
    }
    typedef uint8_t (ABI *kill_fn)(uint32_t, uint64_t *, uint32_t *);
    kill_fn fn = (kill_fn)(uintptr_t)(g_u2_image_base + 0x009F39B0ull);
    return fn ? (fn(ent_id, argv, status) ? 1 : 0) : 0;
}

static int64_t ABI detour_DcPrintNoOp(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* Shipping build 16-byte no-op prints (0x009FDEE0..0x009FDF20) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcForceDockJetski(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01B90: (force-dock-jetski) - shipping stub returning 0 */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcForceBoardJetski(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01BA0: (force-board-jetski) - shipping stub returning 0 */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcEventSender(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x004C1DF0: (event-sender) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
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
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x004C1ED0: (event-sender? [sender-id]) */
    u2_set_status(status, 0);
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
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x004C31A0: (event-train? [train-id]) */
    u2_set_status(status, 0);
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
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x004C3020: (event-train-car? [car-id]) */
    u2_set_status(status, 0);
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
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A0DC40: (camera-moved-backwards? [threshold]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcWeaponSetReset(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x0090AD00: (weapon-set-reset) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcHideHint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0CC10: (hide-hint) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    *(uint8_t *)(g_u2_image_base + 0x0104F1B8ull) = 0;
    return 0;
}

static int64_t ABI detour_DcGetObjectId(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A0BEE0: (get-object-id [obj]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t sym = 0;
    if (argc > 0 && argv) {
        sym = (uint32_t)argv[0];
        typedef void (ABI *StringLookupFn)(uint32_t, const void *);
        StringLookupFn str_lookup = (StringLookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
        if (str_lookup) str_lookup(sym, NULL);
    } else {
        typedef uint32_t (ABI *MakeSymbolFn)(uint32_t, const char *, int32_t);
        MakeSymbolFn *p_make_sym = (MakeSymbolFn *)(uintptr_t)(g_u2_image_base + 0x010666C8ull);
        if (p_make_sym && *p_make_sym) sym = (*p_make_sym)(0, NULL, 1);
    }
    typedef void * (ABI *FindEntityByNameFn)(uint64_t dummy, uint32_t name_id);
    FindEntityByNameFn find_fn = (FindEntityByNameFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    if (!find_fn) return 0;
    uint8_t *ent = (uint8_t *)find_fn(0, sym);
    if (!ent) return 0;
    return (int64_t)*(const uint32_t *)(ent + 0x68);
}

static int64_t ABI detour_DcGetSelfId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04A00: (get-self-id) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef void (ABI *VoidFn)(void);
    VoidFn f1 = (VoidFn)(uintptr_t)(g_u2_image_base + 0x009691D0ull);
    VoidFn f2 = (VoidFn)(uintptr_t)(g_u2_image_base + 0x003F4CA0ull);
    if (f1) f1();
    if (f2) f2();
    return 0;
}

static int64_t ABI detour_DcSetWhiteFade(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A106A0: (set-white-fade [fade-amount]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float fade = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *p_fade = (float *)(uintptr_t)(g_u2_image_base + 0x01150498ull);
    if (p_fade) *p_fade = fade;
    return 0;
}

static int64_t ABI detour_DcSetBlackFade(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A10700: (set-black-fade [fade-amount]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float fade = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *p_fade = (float *)(uintptr_t)(g_u2_image_base + 0x01150494ull);
    if (p_fade) *p_fade = fade;
    return 0;
}

static int64_t ABI detour_DcMakeInert(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD850: (make-inert entity) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcDisablePlatformControlOnObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04E80: (disable-platform-control-on-object [obj-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t obj_id = (uint32_t)argv[0];
    typedef void * (ABI *LookupFn)(uint32_t, int32_t);
    LookupFn lookup = (LookupFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *obj = lookup ? (uint8_t *)lookup(obj_id, 0) : NULL;
    if (obj && (obj[0x36] & 0x01)) {
        uint8_t *plat = *(uint8_t **)(obj + 0x1A8);
        if (plat) {
            typedef void (ABI *DisableFn)(void *, void *);
            DisableFn disable = (DisableFn)(uintptr_t)(g_u2_image_base + 0x0067DA10ull);
            if (disable) disable(plat, obj);
            *(uint8_t **)(obj + 0x1A8) = NULL;
        }
    }
    return 0;
}

static int64_t ABI detour_DcDisableNavBlockerObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04EF0: (disable-nav-blocker-object [obj-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t obj_id = (uint32_t)argv[0];
    typedef void * (ABI *LookupFn)(uint32_t, int32_t);
    LookupFn lookup = (LookupFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *obj = lookup ? (uint8_t *)lookup(obj_id, 0) : NULL;
    if (obj && (obj[0x36] & 0x01)) {
        uint8_t *nav = *(uint8_t **)(obj + 0x170);
        if (nav) {
            *(uint64_t *)(nav + 0x10) = 0;
            obj[0x154] &= ~0x04;
        }
    }
    return 0;
}

static int64_t ABI detour_DcUnblockNavPolyUnderObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04FC0: (unblock-nav-poly-under-object [entity]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t obj_id = (uint32_t)argv[0];
    typedef void * (ABI *LookupFn)(uint32_t, int32_t);
    LookupFn lookup = (LookupFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *obj = lookup ? (uint8_t *)lookup(obj_id, 0) : NULL;
    if (obj && (obj[0x36] & 0x01)) {
        uint8_t *nav = *(uint8_t **)(obj + 0x178);
        if (nav) {
            nav[0x0A] &= ~0x40;
            *(uint64_t *)(obj + 0x178) = 0;
        }
    }
    return 0;
}

static int64_t ABI detour_DcDumpLevelsInfo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04900: (dump-levels-info) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcLerpReflectionPlanes(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02AA0: (lerp-reflection-planes p1 p2 t) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    if (argc >= 3 && argv && argv[0] && argv[1]) {
        uint8_t *slot = g_u2_image_base + 0x01164980ull;
        slot[0x14] = 1;
        *(uint32_t *)(slot + 0x18) = 6;
        memcpy(slot + 0x30, (const void *)argv[0], 16);
        memcpy(slot + 0x40, (const void *)argv[1], 16);
        *(float *)(slot + 0x24) = *(const float *)&argv[2];
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcSlowMoEffect(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0EE10: (slow-mo-effect [speed] [duration]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcRailSetNpcArchetype(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB9660: (rail-set-npc-archetype [rail] [archetype]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t rail_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    uint32_t arch = (argc >= 2 && argv) ? (uint32_t)argv[1] : 0;
    typedef void * (ABI *LookupFn)(uint32_t, int32_t);
    LookupFn lookup = (LookupFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *rail = lookup ? lookup(rail_id, 0) : NULL;
    if (rail) {
        typedef void (ABI *ArchFn)(void *, uint32_t);
        ArchFn fn = (ArchFn)(uintptr_t)(g_u2_image_base + 0x00A904E0ull);
        if (fn) fn(rail, arch);
    }
    return 0;
}

static int64_t ABI detour_DcDebugDrawLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE000: (debug-draw-locator loc [color]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcAddDynamicLight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FF2C0: (add-dynamic-light ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcAddDynamicLightJoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FF010: (add-dynamic-light-joint light entity joint) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcApplyExplosionImpulse(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03B20: (apply-explosion-impulse pos force radius) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcAttachFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0A4C0: (attach-flashlight [entity]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcCameraAnimationDisable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A033B0: (camera-animation-disable track-id [blend-time]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcCameraAnimationEnable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03480: (camera-animation-enable track-id [blend-time]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcCameraShake(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E040: (camera-shake type intensity [duration]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcConstrainRopeAtJoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04430: (constrain-rope-at-joint rope joint) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcCreateWeaponDrawables(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0C410: (create-weapon-drawables [entity]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcDetachRope(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04530: (detach-rope rope) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcDropRope(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A045E0: (drop-rope rope) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcDoRumblePtr(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A105F0: (do-rumble-ptr [motor] [intensity] [duration]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcJournalSetPage(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FD130: (journal-set-page page) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    if (argc > 0 && argv) {
        int32_t page = (int32_t)argv[0];
        *(int32_t *)(g_u2_image_base + 0x0105DFC0ull) = page - 1;
        *(uint8_t *)(g_u2_image_base + 0x0105DFC8ull) = 0;
    }
    return 0;
}

static int64_t ABI detour_DcLetterboxOff(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FFD60: (letterbox-off [time]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcLetterboxOn(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FFE00: (letterbox-on [time]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcRopeEnableCharacterCollision(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04270: (rope-enable-character-collision rope) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcRopeDisableCharacterCollision(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04340: (rope-disable-character-collision rope) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcUseRainIntensityValues(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A031C0: (use-rain-intensity-values ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcUseWindIntensityValues(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03290: (use-wind-intensity-values ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcKillFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0A130: (kill-flashlight [entity]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

/* Table of detours provided by this module */
static int64_t ABI detour_DcCameraFadeOut(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x00A0E380: (camera-fade-out [cam-symbol] [duration]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcCameraShakeInZoomCamera(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0DCA0: (camera-shake-in-zoom-camera) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcDisplayButtonHud(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0CD20: (display-button-hud [btn-id] [action-str]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcFadeToBlack(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FFBA0: (fade-to-black [time] [hold]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcFadeToBlackPers(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FFA40: (fade-to-black-pers [time]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcFadeToWhite(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FFAC0: (fade-to-white [time] [hold]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcGetWaterOffset(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A02F20: (get-water-offset [out-vec]) */
    u2_set_status(status, 0);
    if (argc < 1 || !argv || !argv[0]) return 0;
    float *out = (float *)(uintptr_t)argv[0];
    out[0] = 0.0f; out[1] = 0.0f; out[2] = 0.0f; out[3] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetWaterState(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A02D90: (get-water-state) -> int */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcGivePlayerFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0ADC0: (give-player-flashlight [enable] [slot]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcGiveSullivanFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0ABC0: (give-sullivan-flashlight [enable] [slot]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcKillPlayerFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0A920: (kill-player-flashlight) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcKillSullivanFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0A750: (kill-sullivan-flashlight) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcHeliEnableWind(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A7EB60: (heli-enable-wind [enabled-bool]) */
    u2_set_status(status, 0);
    (void)argc;
    (void)argv;
    return 1;
}

static int64_t ABI detour_DcKillParticles(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A00140: (kill-particles handle [immediate-p]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcScanParticleDirectories(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A19360: (scan-particle-directories) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetDeformationOfWaterUnit(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A02CC0: (set-deformation-of-water-unit [unit] [deform]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetDynamicLightColor(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FEE20: (set-dynamic-light-color [light-id] [color-vec]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetLightFov(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FFC80: (set-light-fov [light-id] [fov-deg]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetMeshVisible(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x00A0BA20: (set-mesh-visible [entity] [mesh-idx] [visible-p]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSetRainState(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A02830: (set-rain-state [state]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetWaterOffset(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A02FF0: (set-water-offset [offset-vec]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetWaterState(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A02E20: (set-water-state [state]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSpawnSnowDisturb(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A01BB0: (spawn-snow-disturb locator-or-pos [radius]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSullivanFlashlightToHand(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0A6A0: (sullivan-flashlight-to-hand) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSullivanFlashlightToWaist(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0A5F0: (sullivan-flashlight-to-waist) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcTurnOnFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0A250: (turn-on-flashlight [entity] [enable]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcUpdateDynamicLight(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FEEE0: (update-dynamic-light light) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcUseMaterialFromWaterUnit(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A02020: (use-material-from-water-unit unit mat) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetVisible(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x00A0BD30: (set-visible [entity] [visible-p]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcDifficultyInterp(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FDAD0: (difficulty-interp easy med hard crushing) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcPadGetButtonPressInterval(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0D060: (pad-get-button-press-interval [btn-id]) -> float */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    float dt = 0.0f;
    uint32_t bits;
    memcpy(&bits, &dt, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcAllowCameraCutCompensate(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0DD40: (allow-camera-cut-compensate) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcDisableCoverCamera(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0E290: (disable-cover-camera) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcDisableMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x005DC990: (disable-message [msg-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t msg_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *msg_fn)(uint32_t, int32_t);
    msg_fn fn = (msg_fn)(uintptr_t)(g_u2_image_base + 0x005DC800ull);
    if (fn) fn(msg_id, 0);
    return 1;
}

static int64_t ABI detour_DcDisplayMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FE140: (display-message msg [time] [pos]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcDumpEntityInfo(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A04970: (dump-entity-info entity) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcEnableActionPack(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x00A0A060: (enable-action-pack [ap-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t ap_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *ap_fn)(uint32_t);
    ap_fn fn = (ap_fn)(uintptr_t)(g_u2_image_base + 0x00A09F80ull);
    if (fn) fn(ap_id);
    return 1;
}

static int64_t ABI detour_DcEnableMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x005DC8E0: (enable-message [msg-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t msg_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *msg_fn)(uint32_t, int32_t);
    msg_fn fn = (msg_fn)(uintptr_t)(g_u2_image_base + 0x005DC800ull);
    if (fn) fn(msg_id, 1);
    return 1;
}

static int64_t ABI detour_DcEndScript(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A44190: (end-script script) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcFacingPointP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0F6D0: (facing-point? [entity] [pt-vec]) -> bool */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcFireWeapon(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0C930: (fire-weapon [entity] [target]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcFireWeaponAtTarget(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0C4C0: (fire-weapon-at-target entity target) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcFollowSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A25E50: (follow-spline [entity] spline-id [speed] [accel]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcForGroup(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x0054B830: (for-group [group-id] [lambda]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcCameraLocalSpace(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0EA20: (camera-local-space) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcForceAimingCameraUltraTight(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A03670: (force-aiming-camera-ultra-tight) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcForceNoShiftCameraZoomMode(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A035E0: (force-no-shift-camera-zoom-mode) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcForceStealthOff(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0BC40: (force-stealth-off) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint8_t *stealth = (uint8_t *)(uintptr_t)(g_u2_image_base + 0x01161EE6ull);
    if (stealth) *stealth = 0;
    return 1;
}

static int64_t ABI detour_DcForgetSignal(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A3FF40: (forget-signal sig [target]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcFreezeBindingWhenNonPhysical(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A03700: (freeze-binding-when-non-physical [enable]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcGetActionPackLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A060C0: (get-action-pack-locator [ap] [out-matrix]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetAlignLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A066F0: (get-align-locator [entity] [out-matrix]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetApReferenceEffectLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A07340: (get-ap-reference-effect-locator [ap-ref] [out-matrix]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetApReferenceLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A07990: (get-ap-reference-locator [ap-ref] [out-matrix]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetApReferenceLocatorRadial(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A06C20: (get-ap-reference-locator-radial [ap-ref] [radius] [out-matrix]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 3 || !argv || !argv[2]) return 0;
    float *out = (float *)(uintptr_t)argv[2];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetBoolean(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A42BF0: (get-boolean [sym-id]) -> bool */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    typedef int32_t (ABI *get_bool_fn)(uint32_t);
    get_bool_fn fn = (get_bool_fn)(uintptr_t)(g_u2_image_base + 0x00A42A00ull);
    return fn ? (fn(sym) != 0 ? 1 : 0) : 0;
}

static int64_t ABI detour_DcGetCurrentTime(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A44800: (get-current-time) -> float */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float t = *(const float *)(g_u2_image_base + 0x01161EE4ull);
    uint32_t bits;
    memcpy(&bits, &t, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A43170: (get-float [sym-id]) -> float */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    typedef float (ABI *get_flt_fn)(uint32_t);
    get_flt_fn fn = (get_flt_fn)(uintptr_t)(g_u2_image_base + 0x00A43000ull);
    float val = fn ? fn(sym) : 0.0f;
    uint32_t bits;
    memcpy(&bits, &val, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetInt32(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A42EB0: (get-int32 [sym-id]) -> int */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    typedef int32_t (ABI *get_int_fn)(uint32_t);
    get_int_fn fn = (get_int_fn)(uintptr_t)(g_u2_image_base + 0x00A42D00ull);
    return fn ? (int64_t)fn(sym) : 0;
}

static int64_t ABI detour_DcGetJointAngleDiffFromBindpose(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A06180: (get-joint-angle-diff-from-bindpose [entity] [joint]) -> float */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    float diff = 0.0f;
    uint32_t bits;
    memcpy(&bits, &diff, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetJointLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A07E80: (get-joint-locator [entity] [joint] [out-matrix]) */
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcGetLocatorDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0EF50: (get-locator-distance [loc1] [loc2]) -> float */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcGetObjectLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A08630: (get-object-locator [entity] [out-matrix]) */
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcGetObjectOffsetLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A121E0: (get-object-offset-locator [entity] [offset] [out-matrix]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcGetObjectOffsetPosition(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A12390: (get-object-offset-position [entity] [offset-vec] [out-vec]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcGetObjectPosition(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A12610: (get-object-position [entity] [out-vec]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcGetObjectState(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x00A04C10: (get-object-state [entity]) -> state */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t ent_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void * (ABI *lookup_fn)(uint32_t, int32_t);
    lookup_fn lookup = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    void *ent = lookup ? lookup(ent_id, 0) : NULL;
    return ent ? (int64_t)(*(const uint32_t *)((uint8_t *)ent + 0x20)) : 0;
}

static int64_t ABI detour_DcGetOriginLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A05F60: (get-origin-locator [out-matrix]) */
    u2_set_status(status, 0);
    if (argc < 1 || !argv || !argv[0]) return 0;
    float *out = (float *)(uintptr_t)argv[0];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetPlayerAimTime(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x005DCBA0: (get-player-aim-time) -> float */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint8_t *player = (uint8_t *)u2_get_active_player();
    if (!player) return 0;
    float aim_time = *(const float *)(player + 0x2A58);
    uint32_t bits;
    memcpy(&bits, &aim_time, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetQuaternionPitch(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009F37C0: (get-quaternion-pitch [quat]) -> float */
    u2_set_status(status, 0);
    float pitch = 0.0f;
    if (argc > 0 && argv && argv[0]) {
        const float *q = (const float *)(uintptr_t)argv[0];
        pitch = asinf(2.0f * (q[0] * q[2] - q[3] * q[1]));
    }
    uint32_t bits;
    memcpy(&bits, &pitch, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetScriptState(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A44290: (get-script-state) -> int */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    return (int64_t)(*(const uint32_t *)(g_u2_image_base + 0x01161EE8ull));
}

static int64_t ABI detour_DcGetStatValue(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x0097BD10: (get-stat-value [stat-id]) -> float */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t stat_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef float (ABI *stat_fn)(uint32_t);
    stat_fn fn = (stat_fn)(uintptr_t)(g_u2_image_base + 0x0097BC00ull);
    float val = fn ? fn(stat_id) : 0.0f;
    uint32_t bits;
    memcpy(&bits, &val, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetStateTime(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A44960: (get-state-time) -> float */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float t = *(const float *)(g_u2_image_base + 0x01161EE0ull);
    uint32_t bits;
    memcpy(&bits, &t, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcGetSymbol(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A43440: (get-symbol [sym-id]) -> val */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    typedef uint32_t (ABI *get_sym_fn)(uint32_t);
    get_sym_fn fn = (get_sym_fn)(uintptr_t)(g_u2_image_base + 0x00A43300ull);
    return fn ? (int64_t)fn(sym) : 0;
}

static int64_t ABI detour_DcGrabRope(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A04740: (grab-rope [entity] [rope-id]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcGrenadeDirectorChangeSettings(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x0069B910: (grenade-director-change-settings ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcGroupAnd(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x0054AFD0: (group-and group-a group-b) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcGroupAt(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x0054A6B0: (group-at group-id index) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcGroupCount(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00549810: (group-count group-id) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcGroupIsMemberSymbolP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x0054A250: (group-is-member-symbol? group symbol) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcGroupOr(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x0054A760: (group-or group-a group-b) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcHeliAnimateGun(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A7ECD0: (heli-animate-gun [heli-id] [anim-bool]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcHeliFireMissile(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A7F140: (heli-fire-missile [heli-id] [target-matrix] [homing-bool]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcHeliSetBasePitch(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A7EC00: (heli-set-base-pitch [heli-id] [pitch-angle]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcHeliSetFiringState(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A7F5C0: (heli-set-firing-state [heli-id] [firing-bool] [param]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcHeliSetHover(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A7ED80: (heli-set-hover [heli-id] [hover-bool]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcHeliSetMgStats(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A7F6B0: (heli-set-mg-stats [heli-id] [burst-len] [fire-rate] [cooldown]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcHeliSetTargetLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A7F2E0: (heli-set-target-locator [heli-id] [locator-matrix] [speed]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcHeliSetTargetObject(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A7F4A0: (heli-set-target-object [heli-id] [target-obj-id]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcHideMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x005DCA40: (hide-message [msg-id]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}


static int64_t ABI detour_DcUnhideMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DCAF0: (unhide-message [msg-id]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcUnhideMessageDeprecated(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DC700: (unhide-message-deprecated [msg-id]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcHideMessageDeprecated(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x005DC650: (hide-message-deprecated [msg-id]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcInFrontOfTankP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00AA3E10: (in-front-of-tank? tank-id target-id) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcIncrementStatDifficulty(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x0097C010: (increment-stat-difficulty [stat-id]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcIsCameraStickPressed(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0D610: (is-camera-stick-pressed) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float dx = *(const float *)(g_u2_image_base + 0x01116180ull + 0xA80);
    float dy = *(const float *)(g_u2_image_base + 0x01116180ull + 0xA84);
    float mag = sqrtf(dx * dx + dy * dy);
    float threshold = *(const float *)(g_u2_image_base + 0x00E67390ull);
    return (mag > threshold) ? 1 : 0;
}

static int64_t ABI detour_DcIsOnScreen(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FD200: (is-on-screen entity) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcIsSpeaking(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x00A013F0: (is-speaking [speaker]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t speaker_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *speaker = speaker_id ? get_ent(0, speaker_id) : NULL;
    if (speaker) {
        uint32_t comp_flags = *(const uint32_t *)(speaker + 0x34);
        if ((comp_flags & 0x10000u) != 0) {
            uint8_t *speech_comp = *(uint8_t **)(speaker + 0x90);
            if (speech_comp && *(const uint32_t *)(speech_comp + 0x18) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcIsStickPressedTowards(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0D6B0: (is-stick-pressed-towards [stick-id] [dir-deg]) -> bool */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcJournalDisable(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A10510: (journal-disable) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}


static int64_t ABI detour_DcKillDroppedWeaponsInSphere(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A09B10: (kill-dropped-weapons-in-sphere [center] [radius]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcLayerToScriptControllerIndices(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00EA2F7E: (layer-to-script-controller-indices layer) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcLerpReflectionPlanesByRotation(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A02A20: (lerp-reflection-planes-by-rotation p1 p2 t) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcMakeActiveCharacterMoreWet(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A020E0: (make-active-character-more-wet amount) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcMakeCharacterMoreWet(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A023C0: (make-character-more-wet [character] amount) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcMirrorAimingAt(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FC990: (mirror-aiming-at target [source]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcObjectInRegionP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0FCA0: (object-in-region? [entity] [region]) -> bool */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t ent_id = (uint32_t)argv[0];
    uint32_t reg_id = (uint32_t)argv[1];
    typedef int32_t (ABI *in_reg_fn)(uint32_t, uint32_t);
    in_reg_fn fn = (in_reg_fn)(uintptr_t)(g_u2_image_base + 0x00A0FBC0ull);
    return fn ? (fn(ent_id, reg_id) != 0 ? 1 : 0) : 0;
}

static int64_t ABI detour_DcObjectOnObjectP(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A04040: (object-on-object? obj1 obj2) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcOfferContextualEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A3FB60: (offer-contextual-entity entity) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcOfferContextualProcess(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A3FA50: (offer-contextual-process proc) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcPadGetGestureInterval(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0CEF0: (pad-get-gesture-interval [gesture-id]) -> float */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    float dt = 0.0f;
    uint32_t bits;
    memcpy(&bits, &dt, sizeof(bits));
    return (int64_t)bits;
}

static int64_t ABI detour_DcPadGetInfo(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0D210: (pad-get-info [pad-id] [out-info]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcPredictOnPhysicsDriven(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A037C0: (predict-on-physics-driven [enable]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcPrintf(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A10110: (printf [fmt] ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcRemoveContextualEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A3F960: (remove-contextual-entity entity) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcRenameObject(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A04D10: (rename-object [entity] [new-name]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcResetStat(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x0097BE50: (reset-stat [stat-id]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcReverseLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A05CB0: (reverse-locator locator) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcRotateLocatorAngle(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A05090: (rotate-locator-angle locator angle axis) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcRotateLocatorLookat(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A05620: (rotate-locator-lookat locator target) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSayPreRoll(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A019F0: (say-pre-roll speaker line-id) */
    u2_set_status(status, 0);
    if (argc < 2 || !argv || !argv[0] || !argv[1] || !g_u2_image_base) return 1;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *speaker = get_ent ? get_ent(0, (uint32_t)argv[0]) : NULL;
    if (speaker) {
        uint32_t comp_flags = *(const uint32_t *)(speaker + 0x34);
        if ((comp_flags & 0x10000u) != 0) {
            typedef void (ABI *PreRollFn)(void *, uint32_t);
            PreRollFn fn = (PreRollFn)(uintptr_t)(g_u2_image_base + 0x003BE370ull);
            if (fn) fn(speaker, (uint32_t)argv[1]);
        }
    }
    return 1;
}

static int64_t ABI detour_DcSetBoolean(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A42060: (set-boolean [symbol-id] [bool-val]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    int32_t val = (int32_t)argv[1] ? 1 : 0;
    typedef void (ABI *set_bool_fn)(uint32_t, int32_t);
    set_bool_fn fn = (set_bool_fn)(uintptr_t)(g_u2_image_base + 0x00A41F00ull);
    if (fn) fn(sym, val);
    return 1;
}

static int64_t ABI detour_DcSetCharacterWetMask(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A02550: (set-character-wet-mask [character] [mask]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetCharacterWetMaskValue(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A026E0: (set-character-wet-mask-value [character] [val]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetClockMultiplier(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x00A0EE60: (set-clock-multiplier [speed]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float mult = (argc > 0 && argv) ? *(const float *)&argv[0] : 1.0f;
    float *p_clock = (float *)(uintptr_t)(g_u2_image_base + 0x01161EE0ull);
    if (p_clock) *p_clock = mult;
    return 1;
}

static int64_t ABI detour_DcSetFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A42630: (set-float [symbol-id] [float-val]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    float val = *(const float *)&argv[1];
    typedef void (ABI *set_flt_fn)(uint32_t, float);
    set_flt_fn fn = (set_flt_fn)(uintptr_t)(g_u2_image_base + 0x00A42500ull);
    if (fn) fn(sym, val);
    return 1;
}

static int64_t ABI detour_DcSetFocusableIgnore(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x00A0F3C0: (set-focusable-ignore [obj-id] [flag]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t obj_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    int32_t flag = (argc >= 2 && argv) ? (int32_t)argv[1] : 1;
    typedef void (ABI *focus_fn)(uint32_t, int32_t);
    focus_fn fn = (focus_fn)(uintptr_t)(g_u2_image_base + 0x00A0F300ull);
    if (fn) fn(obj_id, flag);
    return 1;
}

static int64_t ABI detour_DcSetIceCaveStupidGearsHack(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FCC50: (set-ice-cave-stupid-gears-hack [flag]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetInt32(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A42350: (set-int32 [symbol-id] [int-val]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    int32_t val = (int32_t)argv[1];
    typedef void (ABI *set_int_fn)(uint32_t, int32_t);
    set_int_fn fn = (set_int_fn)(uintptr_t)(g_u2_image_base + 0x00A42200ull);
    if (fn) fn(sym, val);
    return 1;
}

static int64_t ABI detour_DcSetRagdollFlow(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FD5C0: (set-ragdoll-flow [entity] [flow-vec]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetRenderSettings(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FFF80: (set-render-settings [setting-id]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetShaderInstanceParam(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A107B0: (set-shader-instance-param ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetShaderMaterialParam(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A10DD0: (set-shader-material-param ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetSingleShaderInstanceParam(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A10A50: (set-single-shader-instance-param ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetSplineFollowAcceleration(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A25CB0: (set-spline-follow-acceleration [entity] accel) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSetSplineFollowDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A25C00: (set-spline-follow-distance [entity] dist) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSetSplineFollowSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A25D60: (set-spline-follow-speed [entity] speed [trans-time]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSetSymbol(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A42910: (set-symbol [symbol-id] [val]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    uint32_t val = (uint32_t)argv[1];
    typedef void (ABI *set_sym_fn)(uint32_t, uint32_t);
    set_sym_fn fn = (set_sym_fn)(uintptr_t)(g_u2_image_base + 0x00A42800ull);
    if (fn) fn(sym, val);
    return 1;
}

static int64_t ABI detour_DcSetTableShaderMaterialValuesParam(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009F3B70: (set-table-shader-material-values-param ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcShowBinoculars(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0CB90: (show-binoculars [flag]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcShutUp(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
/* 0x00A011E0: (shut-up [speaker] [fade-time]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t speaker_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    float fade_time = (argc >= 2 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *speaker = speaker_id ? get_ent(0, speaker_id) : NULL;
    if (speaker) {
        typedef void (ABI *ShutUpFn)(uint32_t, void *, float);
        ShutUpFn fn = (ShutUpFn)(uintptr_t)(g_u2_image_base + 0x00A34240ull);
        if (fn) fn(1, speaker, fade_time);
    }
    return 0;
}

static int64_t ABI detour_DcSignal(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A40480: (signal sig [target] [val]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSlaForceTackle(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0F4A0: (sla-force-tackle [entity]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}






static int64_t ABI detour_DcBattleSelectBestSpawnerFromGroup(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A11190: (battle-select-best-spawner-from-group [group]) -> spawner-id */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t group_id = (uint32_t)argv[0];
    typedef uint32_t (ABI *select_fn)(uint32_t);
    select_fn fn = (select_fn)(uintptr_t)(g_u2_image_base + 0x00A10F70ull);
    return fn ? (int64_t)fn(group_id) : 0;
}

static int64_t ABI detour_DcCameraAimOverridePoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0DF40: (camera-aim-override-point point) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcCameraEnable(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A0E680: (camera-enable cam-sym [target] [fade_time] [priority] ...) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcCameraLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A128B0: (camera-locator) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcEmitBulletFromSpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0BF40: (emit-bullet-from-spawner [spawner] [target]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcFmaRequestPartModule(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A00050: (fma-request-part-module [symbol-id]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcGetSpawnerLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09330: (get-spawner-locator [spawner-id] [out-matrix]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetSpawnerLocatorProjected(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A08D10: (get-spawner-locator-projected [spawner-id] [out-matrix]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetSpawnerLocatorRadial(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A088C0: (get-spawner-locator-radial [spawner-id] [radius] [out-matrix]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 3 || !argv || !argv[2]) return 0;
    float *out = (float *)(uintptr_t)argv[2];
    memset(out, 0, 16 * sizeof(float));
    out[0] = out[5] = out[10] = out[15] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGetSpawnerPosition(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A094B0: (get-spawner-position [spawner-id] [out-pos]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv || !argv[1]) return 0;
    float *out = (float *)(uintptr_t)argv[1];
    out[0] = 0.0f; out[1] = 0.0f; out[2] = 0.0f; out[3] = 1.0f;
    return (int64_t)(uintptr_t)out;
}

static int64_t ABI detour_DcGroupSpawnedCount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00549BF0: (group-spawned-count group-id) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}



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
    uint32_t handle = (argc > 0 && argv && argv[0]) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, NULL, 0);
    }
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    return handle ? get_ent(0, handle) : NULL;
}

static int64_t ABI detour_DcHeliMoveTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A7FEE0: (heli-move-to [heli] target-locator [speed]) */
    u2_set_status(status, 0);
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
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A7FDD0: (heli-move-to-spline [heli] spline-id [speed]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcKillPartSpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A099B0: (kill-part-spawner [spawner]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcLevelsFullySpawnedIgnorePlayerP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE400: (levels-fully-spawned-ignore-player?) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcLevelsFullySpawnedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE5E0: (levels-fully-spawned?) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcMakeVector(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FDCF0: (make-vector [x] [y] [z] [w]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float x = (argc > 0 && argv) ? u2_reg_to_f32(argv[0]) : 0.0f;
    float y = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
    float z = (argc > 2 && argv) ? u2_reg_to_f32(argv[2]) : 0.0f;
    float w = (argc > 3 && argv) ? u2_reg_to_f32(argv[3]) : 1.0f;
    typedef void * (ABI *AllocFn)(uint64_t size, uint32_t *tag);
    AllocFn alloc_fn = (AllocFn)(uintptr_t)(g_u2_image_base + 0x00B33020ull);
    uint32_t tag = 0x45;
    float *vec = (float *)alloc_fn(0x10, &tag);
    if (vec) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
        vec[3] = w;
    }
    return (uintptr_t)vec;
}

static int64_t ABI detour_DcMakeVector3(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FDC20: (make-vector-3 [x] [y] [z]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float x = (argc > 0 && argv) ? u2_reg_to_f32(argv[0]) : 0.0f;
    float y = (argc > 1 && argv) ? u2_reg_to_f32(argv[1]) : 0.0f;
    float z = (argc > 2 && argv) ? u2_reg_to_f32(argv[2]) : 0.0f;
    typedef void * (ABI *AllocFn)(uint64_t size, uint32_t *tag);
    AllocFn alloc_fn = (AllocFn)(uintptr_t)(g_u2_image_base + 0x00B33020ull);
    uint32_t tag = 0x45;
    float *vec = (float *)alloc_fn(0xc, &tag);
    if (vec) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
    }
    return (uintptr_t)vec;
}

static int64_t ABI detour_DcReloadModule(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A19200: (reload-module mod) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcReloadParticleModule(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A19180: (reload-particle-module mod) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSelectBestSchemaSpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10EB0: (select-best-schema-spawner [schema]) -> spawner-id */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    uint32_t schema_id = (uint32_t)argv[0];
    typedef uint32_t (ABI *select_fn)(uint32_t);
    select_fn fn = (select_fn)(uintptr_t)(g_u2_image_base + 0x00A10E00ull);
    return fn ? (int64_t)fn(schema_id) : 0;
}

static int64_t ABI detour_DcSetCollisionLayer(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A03CD0: (set-collision-layer [entity] [layer]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSetCurrentBySpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD710: (set-current-by-spawner [spawner-id]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSetDefaultFollowCameraSettings(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A0EBF0: (set-default-follow-camera-settings [setting-ptr]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSetObjectFaction(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x009FCF40: (set-object-faction [entity] [faction-id]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSetOffBalanceBySpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x008E9160: (set-off-balance-by-spawner [spawner-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t spawner_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *spawner_fn)(uint32_t);
    spawner_fn fn = (spawner_fn)(uintptr_t)(g_u2_image_base + 0x008E90B0ull);
    if (fn) fn(spawner_id);
    return 1;
}

static int64_t ABI detour_DcSpawnEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09F40: (spawn-entity [archetype] [name]) -> ent-id */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    typedef uint32_t (ABI *spawn_fn)(uint32_t, uint64_t);
    spawn_fn fn = (spawn_fn)(uintptr_t)(g_u2_image_base + 0x00A096E0ull);
    return fn ? (int64_t)fn((uint32_t)argv[0], argc >= 2 ? argv[1] : 0) : 0;
}

static int64_t ABI detour_DcSpawnEntityAt(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009F3900: (spawn-entity-at [archetype] [pos-vec]) -> ent-id */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;
    typedef uint32_t (ABI *spawn_fn)(uint32_t, uint64_t);
    spawn_fn fn = (spawn_fn)(uintptr_t)(g_u2_image_base + 0x00A096E0ull);
    return fn ? (int64_t)fn((uint32_t)argv[0], argc >= 2 ? argv[1] : 0) : 0;
}

static int64_t ABI detour_DcSpawnExplosion(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FF610: (spawn-explosion pos type radius damage) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSpawnExplosionNpcOnly(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FF410: (spawn-explosion-npc-only pos type radius damage) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSpawnFloatingBarrelAtPhase(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03E20: (spawn-floating-barrel-at-phase pos phase) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSpawnMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x005DC7B0: (spawn-message [msg-id]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t msg_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef void (ABI *msg_fn)(uint32_t);
    msg_fn fn = (msg_fn)(uintptr_t)(g_u2_image_base + 0x005DC700ull);
    if (fn) fn(msg_id);
    return 1;
}

static int64_t ABI detour_DcSpawnParticlesAtLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A002D0: (spawn-particles-at-locator fx-name locator [entity-id]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSpawnScript(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A44460: (spawn-script script ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSplineActivate(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A26180: (spline-activate spline-id [active-p]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSplineDistanceLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25F60: (spline-distance-locator spline-id locator) */
    u2_set_status(status, 0);
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
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSplineDistanceVehicleAnimation(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8640: (spline-distance-vehicle-animation veh dist) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSplineLengthP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A256B0: (spline-length? spline-id) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcSplineLocatorAtDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00EA2315: (spline-locator-at-distance spline dist) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcSsError(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A44670: (ss-error msg ...) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcStickySpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0B8F0: (sticky-spawner [spawner]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcStopSpeaking(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A01570: (stop-speaking [speaker]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t speaker_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *speaker = speaker_id ? get_ent(0, speaker_id) : NULL;
    if (speaker) {
        typedef void (ABI *StopSpeechFn)(uint32_t, void *);
        StopSpeechFn fn = (StopSpeechFn)(uintptr_t)(g_u2_image_base + 0x00A34240ull);
        if (fn) fn(1, speaker);
    }
    return 0;
}

static int64_t ABI detour_DcSwapEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09590: (swap-entity [old-ent] [new-archetype]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcTankAimAtObj(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2AC0: (tank-aim-at-obj [tank] [target]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA26D0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankAimAtPos(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2E90: (tank-aim-at-pos [tank] [pos-vec]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA2B60ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankAimAtPosInstant(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2B10: (tank-aim-at-pos-instant [tank] [pos-vec]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA2B60ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankAimForward(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2A70: (tank-aim-forward [tank]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA26D0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcSpawnAttachable(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0F540: (spawn-attachable [archetype] [parent] [joint]) -> ent-id */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 3 || !argv) return 0;
    typedef uint32_t (ABI *attach_fn)(uint32_t, uint32_t, uint32_t);
    attach_fn fn = (attach_fn)(uintptr_t)(g_u2_image_base + 0x00A0F4C0ull);
    return fn ? (int64_t)fn((uint32_t)argv[0], (uint32_t)argv[1], (uint32_t)argv[2]) : 0;
}

static int64_t ABI detour_DcTankDistToPoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA1300: (tank-dist-to-point [tank] [pos-vec]) -> float */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t handle = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    if (!handle) {
        typedef uint32_t (ABI *GetSelfFn)(uint32_t, const void *, uint32_t);
        GetSelfFn get_self = (GetSelfFn)(uintptr_t)(*(uint8_t **)(g_u2_image_base + 0x010666C8ull));
        if (get_self) handle = get_self(0, 0, 1);
    }
    typedef uint8_t * (ABI *GetEntityFn)(uint32_t, int32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
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

static int64_t ABI detour_DcTankFindBestTargetMP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA4950: (tank-find-best-target-mp ...) */
    (void)argc;
    (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcTankFire(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AA2930: (tank-fire [tank]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTankFireAtObj(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2EE0: (tank-fire-at-obj [tank] [target] [lead]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA26D0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankFireAtPos(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2F30: (tank-fire-at-pos [tank] [pos-vec] [lead]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA2B60ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankForceFireCannon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA42C0: (tank-force-fire-cannon [tank-id] [target-pos-vector]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTankGetHealth(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3D80: (tank-get-health [tank-id]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTankMgCeaseFire(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2000: (tank-mg-cease-fire [tank]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA20A0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankMgFireAtLoc(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2120: (tank-mg-fire-at-loc [tank] [loc-vec]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA21C0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankMgFireAtObj(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2680: (tank-mg-fire-at-obj [tank] [target]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA21C0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankMoveTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA33E0: (tank-move-to [tank] [dest-vec] [speed]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA3010ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankMoveTrackingObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA1550: (tank-move-tracking-object [tank] [target]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA1600ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankReverseTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA2F80: (tank-reverse-to [tank] [dest-vec] [speed]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA3010ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankSetHealth(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3CC0: (tank-set-health [tank-id] [health]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTankTeleport(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA1BA0: (tank-teleport [tank] [locator]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA1C40ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcTankWaitTeleport(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA4420: (tank-wait-teleport [tank-id] [locator-matrix] [speed]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTaskElapsedTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A102B0: (task-elapsed-time task) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return (int64_t)u2_f32_to_reg(0.0f);
}

static int64_t ABI detour_DcTaskFailed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE900: (task-failed [task] [reason]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcTaskFind(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FEB90: (task-find task-name) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcTaskNodeActiveP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10340: (task-node-active? task node) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcTaskSubnodeComplete(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FECA0: (task-subnode-complete task node) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcTaskSubnodePlay(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FEC00: (task-subnode-play task node) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 0;
}

static int64_t ABI detour_DcTransferToSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25A90: (transfer-to-spline [entity] target-spline [speed]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTranslateYLoop(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FC590: (translate-y-loop entity min max speed) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcUnstickySpawner(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0B7C0: (unsticky-spawner [spawner]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWait(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A3E690: (wait [seconds]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWaitBoolean(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A41610: (wait-boolean [condition]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWaitFor(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A3FC50: (wait-for [coroutine-task]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWaitFrames(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A41270: (wait-frames [num-frames]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWaitHeliFireMissile(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A7EE30: (wait-heli-fire-missile [heli-id] [target-matrix]) */
    u2_set_status(status, 0);
    return detour_DcHeliFireMissile(argc, argv, status);
}

static int64_t ABI detour_DcWaitHeliMoveTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A7FA70: (wait-heli-move-to [heli] [target] [speed]) */
    uint8_t *heli = u2_resolve_heli(argc, argv);
    if (!heli) {
        u2_set_status(status, 0);
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

    u2_set_status(status, 2); /* pending wait status */
    typedef void (ABI *WaitFn)(void *, void *, float, void *, uint32_t);
    WaitFn wait_fn = (WaitFn)(uintptr_t)(g_u2_image_base + 0x00A877A0ull);
    if (wait_fn) {
        wait_fn(heli, target, speed, train_car, 0);
    }
    return 0;
}

static int64_t ABI detour_DcWaitHeliMoveToSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A7F800: (wait-heli-move-to-spline [heli] spline-id [speed]) */
    uint8_t *heli = u2_resolve_heli(argc, argv);
    if (!heli) {
        u2_set_status(status, 0);
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

    u2_set_status(status, 2); /* pending wait status */
    typedef void (ABI *WaitSplineFn)(void *, uint32_t, float, void *);
    WaitSplineFn fn = (WaitSplineFn)(uintptr_t)(g_u2_image_base + 0x00A87850ull);
    if (fn) fn(heli, spline_id, speed, train_car);
    return 0;
}

static int64_t ABI detour_DcWaitInt32(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A41D40: (wait-int32 [target-val]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWaitOneFrame(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A40F70: (wait-one-frame) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWaitPlayerMoveTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0B350: (wait-player-move-to [dest-vec] [speed]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint8_t *player = (uint8_t *)u2_get_active_player();
    if (player && argc > 0 && argv && argv[0]) {
        typedef void (ABI *move_fn)(void *, const void *, int32_t);
        move_fn fn = (move_fn)(uintptr_t)(g_u2_image_base + 0x00A0B100ull);
        if (fn) fn(player, (const void *)(uintptr_t)argv[0], 1);
    }
    return 1;
}

static int64_t ABI detour_DcWaitPlayerTurnTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A0AFC0: (wait-player-turn-to [target-or-dir]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint8_t *player = (uint8_t *)u2_get_active_player();
    if (player && argc > 0 && argv) {
        typedef void (ABI *turn_fn)(void *, uint64_t, int32_t);
        turn_fn fn = (turn_fn)(uintptr_t)(g_u2_image_base + 0x00A0AE00ull);
        if (fn) fn(player, argv[0], 1);
    }
    return 1;
}

static int64_t ABI detour_DcWaitSay(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A016D0: (wait-say speaker line-id) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc <= 0 || !argv) return 0;

    /* Clamps active train check bounds safely to prevent int $0x41 debug trap */
    uint8_t *mgr = g_u2_image_base + 0x01050320ull;
    int32_t count = *(const int32_t *)(mgr + 0x200);
    int32_t extra = -1;
    if (count >= 0 && count < 16) {
        uint8_t *slot = mgr + 8 + (size_t)count * 32;
        void *train_car = *(void **)slot;
        if (train_car) {
            uint8_t *p = *(uint8_t **)((uint8_t *)train_car + 0x10);
            if (p) extra = *(const int32_t *)(p + 8);
        }
    }

    typedef int64_t (ABI *SayFn)(int32_t, uint64_t *, int32_t);
    SayFn say_fn = (SayFn)(uintptr_t)(g_u2_image_base + 0x009F3220ull);
    int64_t res = say_fn ? say_fn(argc, argv, extra) : 0;
    if (status && res != 0) {
        u2_set_status(status, 2); /* pending wait status */
    }
    return res;
}

static int64_t ABI detour_DcWaitSpawnEntity(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09E90: (wait-spawn-entity [archetype] [name]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWaitSpawnEntityAt(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A09DC0: (wait-spawn-entity-at [archetype] [pos]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWaitSplineDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A25740: (wait-spline-distance entity target-dist) */
    if (!g_u2_image_base || argc < 2 || !argv) {
        u2_set_status(status, 0);
        return 0;
    }
    uint32_t ent_id = (uint32_t)argv[0];
    float target_dist = u2_reg_to_f32(argv[1]);
    typedef uint8_t * (ABI *GetEntityFn)(uint64_t, uint32_t);
    GetEntityFn get_ent = (GetEntityFn)(uintptr_t)(g_u2_image_base + 0x009BBE40ull);
    uint8_t *ent = get_ent ? get_ent(0, ent_id) : NULL;
    if (!ent) {
        u2_set_status(status, 0);
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
        u2_set_status(status, 0);
        return 1;
    }
    u2_set_status(status, 2); /* pending wait */
    return 0;
}

static int64_t ABI detour_DcWaitTankAimAtObj(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3A40: (wait-tank-aim-at-obj [tank] [target]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA26D0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcWaitTankAimAtPos(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3AA0: (wait-tank-aim-at-pos ...) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *TankWaitFn)(int32_t, uint64_t *, uint32_t, uint32_t, uint64_t, void *, uint64_t, uint64_t);
    TankWaitFn fn = (TankWaitFn)(uintptr_t)(g_u2_image_base + 0x00AA3490ull);
    return fn ? fn(argc, argv, 2, 2, (uintptr_t)status, NULL, 0, 0) : 1;
}

static int64_t ABI detour_DcWaitTankAimForward(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3430: (wait-tank-aim-forward [tank]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x00AA26D0ull);
    return fn ? fn(argc, argv, status) : 0;
}

static int64_t ABI detour_DcWaitTankFireAtObj(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3B00: (wait-tank-fire-at-obj ...) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *TankWaitFn)(int32_t, uint64_t *, uint32_t, uint32_t, uint64_t, void *, uint64_t, uint64_t);
    TankWaitFn fn = (TankWaitFn)(uintptr_t)(g_u2_image_base + 0x00AA3490ull);
    return fn ? fn(argc, argv, 2, 3, (uintptr_t)status, NULL, 0, 0) : 1;
}

static int64_t ABI detour_DcWaitTankFireAtPos(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3B60: (wait-tank-fire-at-pos ...) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *TankWaitFn)(int32_t, uint64_t *, uint32_t, uint32_t, uint64_t, void *, uint64_t, uint64_t);
    TankWaitFn fn = (TankWaitFn)(uintptr_t)(g_u2_image_base + 0x00AA3490ull);
    return fn ? fn(argc, argv, 2, 4, (uintptr_t)status, NULL, 0, 0) : 1;
}

static int64_t ABI detour_DcWaitTankMoveTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3C60: (wait-tank-move-to ...) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *TankWaitFn)(int32_t, uint64_t *, uint32_t, uint32_t, uint64_t, void *, uint64_t, uint64_t);
    TankWaitFn fn = (TankWaitFn)(uintptr_t)(g_u2_image_base + 0x00AA3490ull);
    return fn ? fn(argc, argv, 2, 4, (uintptr_t)status, NULL, 0, 0) : 1;
}

static int64_t ABI detour_DcWaitTankReverseTo(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AA3BC0: (wait-tank-reverse-to ...) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    typedef int64_t (ABI *TankWaitFn)(int32_t, uint64_t *, uint32_t, uint32_t, uint64_t, void *, uint64_t, uint64_t);
    TankWaitFn fn = (TankWaitFn)(uintptr_t)(g_u2_image_base + 0x00AA3490ull);
    return fn ? fn(argc, argv, 2, 4, (uintptr_t)status, NULL, 0, 0) : 1;
}

static int64_t ABI detour_DcWaitUntil(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A40E60: (wait-until [predicate]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWaitUntilFrame(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A40D40: (wait-until-frame [target-frame]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcWaitUntilNetTime(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A40870: (wait-until-net-time [target-time]) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    return 1;
}

static int64_t ABI detour_DcRailActivateMover(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB8340: (rail-activate-mover [mover-idx] [active-p]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    uint32_t mover_idx = (argc > 0 && argv) ? (uint32_t)argv[0] : 0;
    int32_t active = (argc >= 2 && argv) ? (argv[1] != 0) : 1;
    uint8_t *rail_mgr = g_u2_image_base + 0x010508B0ull;
    if (rail_mgr && mover_idx < 2) {
        *(uint32_t *)(rail_mgr + 0x54 + (size_t)mover_idx * 0x30) = active ? 1 : 0;
    }
    return 0;
}

static int64_t ABI detour_DcRailAnimateToMover(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8130: (rail-animate-to-mover vehicle-id [mover-idx]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailDeactivate(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB97D0: (rail-deactivate [rail]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailDisableCareen(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8B80: (rail-disable-careen [vehicle-id] [disabled-bool]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailEnableCollideWithObjects(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8EA0: (rail-enable-collide-with-objects [vehicle-id] [enabled-bool]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailEnableGroundHug(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8430: (rail-enable-ground-hug vehicle-id [enable-p]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailEnableTributary(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB96B0: (rail-enable-tributary [rail] [tributary-id]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailEnableTruckJump(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB9BB0: (rail-enable-truck-jump [flag]) */
    (void)argc;
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailFireTurretGrenade(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8CD0: (rail-fire-turret-grenade [turret-id] [target-id] [param]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailFollowMover(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB81E0: (rail-follow-mover vehicle-id [mover-idx]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailForceCareen(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8C40: (rail-force-careen [vehicle-id]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailGetMoverLocator(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB7D00: (rail-get-mover-locator [out-loc]) */
    (void)argc;
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailGetSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB84E0: (rail-get-spline [vehicle-id]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailHasVehicle(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB94B0: (rail-has-vehicle? vehicle-id) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailOverrideSfxState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9070: (rail-override-sfx-state [vehicle-id] [sfx-state]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailScrub(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB8F90: (rail-scrub vehicle-id scrub-pos) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailSetAcceleration(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB9590: (rail-set-acceleration [rail] [accel]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailSetAllVehiclesToMoverSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB7BF0: (rail-set-all-vehicles-to-mover-speed [speed]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float speed = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *target_speed = (float *)(uintptr_t)(g_u2_image_base + 0x0156DF80ull);
    if (target_speed) *target_speed = speed;
    return 1;
}

static int64_t ABI detour_DcRailSetFollowDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB8560: (rail-set-follow-distance [rail] [min-dist] [max-dist]) */
    u2_set_status(status, 0);
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
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB7FD0: (rail-set-mover-follow-distance [distance]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float dist = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *mover_dist = (float *)(uintptr_t)(g_u2_image_base + 0x0156DF78ull);
    if (mover_dist) *mover_dist = dist;
    return 1;
}

static int64_t ABI detour_DcRailSetMoverIntialDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB7E80: (rail-set-mover-initial-distance [distance]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;
    float dist = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *mover_init = (float *)(uintptr_t)(g_u2_image_base + 0x0156DF7Cull);
    if (mover_init) *mover_init = dist;
    return 1;
}

static int64_t ABI detour_DcRailSetMoverSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB80A0: (rail-set-mover-speed speed [mover-idx]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailSetSpeed(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9230: (rail-set-speed vehicle-id speed [immediate-p]) */
    u2_set_status(status, 0);
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
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;

    typedef void (ABI *LogFn)(void *, uint32_t, const char *);
    LogFn log_fn = (LogFn)(uintptr_t)(g_u2_image_base + 0x00A34240ull);
    if (log_fn) log_fn((void *)(g_u2_image_base + 0x01163FB0ull), 1, "rail-shutdown");

    typedef uint8_t * (ABI *NextFn)(void *, uint32_t);
    NextFn next_fn = (NextFn)(uintptr_t)(g_u2_image_base + 0x009BCDF0ull);

    typedef uint8_t (ABI *CheckFn)(void *);
    CheckFn check_fn = (CheckFn)(uintptr_t)(g_u2_image_base + 0x000011D0ull);

    typedef void (ABI *KillTreeFn)(void *);
    KillTreeFn kill_tree = (KillTreeFn)(uintptr_t)(g_u2_image_base + 0x009BE760ull);

    uint8_t *rax = (uint8_t *)(g_u2_image_base + 0x0115F1A0ull);
    uint8_t *cur = *(uint8_t **)(rax + 0xC50);

    while (cur && next_fn) {
        uint8_t *mover = next_fn(cur, 0);
        cur = mover;
        if (!cur) break;
        if ((*(uint16_t *)(cur + 0x34) & 1) != 0) {
            if (check_fn && check_fn(cur)) {
                if (kill_tree) kill_tree(cur);
            }
        }
    }

    uint8_t *rcx = (uint8_t *)(g_u2_image_base + 0x0115BD20ull);
    uint8_t *p = *(uint8_t **)rcx;
    if (p) {
        uint32_t c = *(uint32_t *)(rcx + 8);
        if (c) {
            uint8_t *target = *(uint8_t **)p;
            if (target && *(uint32_t *)(target + 0x44) == c) {
                if (kill_tree) kill_tree(target);
            }
        }
    }
    return 0;
}

static int64_t ABI detour_DcRailSpawn(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00AB9920: (rail-spawn [rail] [spawner-id]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcRailUseFixedAprefInIgc(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9410: (rail-use-fixed-apref-in-igc [vehicle-id] [enabled-bool]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTrainAllowUnphysicalize(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A68430: (train-allow-unphysicalize [train] [allow-p]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTrainDecouple(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A67E40: (train-decouple [train] [car-idx] [front-or-back] [flag]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTrainGetAcceleration(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A68230: (train-get-acceleration [train]) */
    u2_set_status(status, 0);
    uint8_t *train = u2_resolve_train(argc, argv);
    float accel = train ? *(const float *)(train + 0x540) : 10.0f;
    return (int64_t)u2_f32_to_reg(accel);
}

static int64_t ABI detour_DcTrainGetDistance(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A67B90: (train-get-distance [train]) */
    u2_set_status(status, 0);
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    typedef float (ABI *SplineDistFn)(void *, int32_t);
    SplineDistFn dist_fn = (SplineDistFn)(uintptr_t)(g_u2_image_base + 0x00A23EC0ull);
    float dist = dist_fn ? dist_fn(train + 0x3B0, 1) : *(const float *)(train + 0x3B0);
    return (int64_t)u2_f32_to_reg(dist);
}

static int64_t ABI detour_DcTrainGetHeadCar(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A67C40: (train-get-head-car [train]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTrainGetSpline(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A67AE0: (train-get-spline [train]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTrainKillAllRiders(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A68530: (train-kill-all-riders [train] [kill-type]) */
    u2_set_status(status, 0);
    uint8_t *train = u2_resolve_train(argc, argv);
    if (!train) return 0;
    int32_t kill_type = (argc >= 2) ? (int32_t)argv[1] : 0;
    typedef void (ABI *KillRidersFn)(void *, int32_t);
    KillRidersFn fn = (KillRidersFn)(uintptr_t)(g_u2_image_base + 0x00A70BE0ull);
    if (fn) fn(train, kill_type);
    return 0;
}

static int64_t ABI detour_DcTrainSetAcceleration(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A68140: (train-set-acceleration [train] accel) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTrainSetSpeedMph(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A67F60: (train-set-speed-mph [train] speed-mph [trans-time]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTrainSpawn(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A67960: (train-spawn type-symbol) */
    u2_set_status(status, 0);
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
    u2_set_status(status, 2);
    return 0;
}

static int64_t ABI detour_DcTrainSpawnOnTrack(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A68740: (train-spawn-on-track train-id spline-id [dist]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTrainTeleport(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A67CD0: (train-teleport [train] spline-id [dist]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTrainTrackPlayer(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    if (!argv) { u2_set_status(status, 0); return 0; }
    /* 0x00A68300: (train-track-player [train] [enable-p]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcTrainTransferPendingP(int32_t argc, uint64_t *argv, uint32_t *status) {
    if (!g_u2_image_base) { u2_set_status(status, 0); return 0; }
    /* 0x00A685E0: (train-transfer-pending? [train]) */
    u2_set_status(status, 0);
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

static int64_t ABI detour_DcAddLoadModule(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FE90: (add-load-module mod-sym [heap-type]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    uint32_t kind = (uint32_t)argv[1];
    uint32_t heap_tag = 0;

    if (kind == 0xA4DDDED4u) {
        if (*(const uint8_t *)(g_u2_image_base + 0x015690A0ull) != 0) return 0;
        if (*(const uint32_t *)(g_u2_image_base + 0x011A65C0ull) == 0) return 0;
        heap_tag = 0x08u;
    } else if (kind == 0x1AA8A79Bu) {
        heap_tag = 0x45u;
    } else {
        heap_tag = 0x12u;
    }

    typedef uint8_t (ABI *AddLoadModFn)(uint32_t, const uint32_t *);
    AddLoadModFn fn = (AddLoadModFn)(uintptr_t)(g_u2_image_base + 0x00A16250ull);
    return fn ? (int64_t)fn(sym, &heap_tag) : 0;
}

static int64_t ABI detour_DcAddImportModule(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0FF10: (add-import-module mod-sym [heap-type]) */
    u2_set_status(status, 0);
    if (!g_u2_image_base || argc < 2 || !argv) return 0;
    uint32_t sym = (uint32_t)argv[0];
    uint32_t kind = (uint32_t)argv[1];
    uint32_t heap_tag = 0;

    if (kind == 0xA4DDDED4u) {
        if (*(const uint8_t *)(g_u2_image_base + 0x015690A0ull) != 0) return 0;
        if (*(const uint32_t *)(g_u2_image_base + 0x011A65C0ull) == 0) return 0;
        heap_tag = 0x08u;
    } else if (kind == 0x1AA8A79Bu) {
        heap_tag = 0x45u;
    } else {
        heap_tag = 0x12u;
    }

    typedef uint8_t (ABI *AddImportModFn)(uint32_t, const uint32_t *);
    AddImportModFn fn = (AddImportModFn)(uintptr_t)(g_u2_image_base + 0x00A16BB0ull);
    return fn ? (int64_t)fn(sym, &heap_tag) : 0;
}

static int64_t ABI detour_DcKillAllAttachables(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009F2BF0: (kill-all-attachables) */
    (void)argc; (void)argv;
    u2_set_status(status, 0);
    if (!g_u2_image_base) return 0;

    typedef uint8_t * (ABI *NextEntityFn)(void *, uint32_t);
    NextEntityFn next_fn = (NextEntityFn)(uintptr_t)(g_u2_image_base + 0x009BCDF0ull);
    typedef void (ABI *SendEventFn)(void *, uint32_t, void *, void *, void *, void *, void *);
    SendEventFn send_event = (SendEventFn)(uintptr_t)(g_u2_image_base + 0x004C4090ull);

    if (!next_fn || !send_event) return 0;

    uint8_t *mgr = g_u2_image_base + 0x0115F1A0ull;
    uint8_t *cur = next_fn(*(void **)(mgr + 0xC90), 0);

    while (cur) {
        if ((*(const uint16_t *)(cur + 0x34) & 1u) != 0) {
            uint8_t evt_res[32] = {0};
            uint8_t p1[24] = {0};
            uint8_t p2[24] = {0};
            uint8_t p3[24] = {0};
            uint8_t p4[24] = {0};
            send_event(evt_res, 0x9CD64D13u, cur, p1, p2, p3, p4);
        }
        cur = next_fn(cur, 0);
    }
    return 0;
}

/* ============================================================================
 * Phase 4 (Palier B): Pure C++ Engine Subsystems
 *   - catmull-rom.cpp & catmull-rom-manager.cpp
 *   - spline-tracker.cpp & vehicle-spline-tracker.cpp
 *   - camerazoom.cpp
 *   - rail-vehicle.cpp, rail-vehicle-behavior.cpp, rail-vehicle-manager.cpp
 * ============================================================================ */

static const char * ABI detour_CatmullRomSpline_GetName(void *self) {
    /* 0x00468260: CatmullRomSpline::GetName() const */
    if (!self || !g_u2_image_base) return NULL;
    const uint8_t *data = *(const uint8_t * const *)((const uint8_t *)self + 0x10);
    if (!data) return NULL;
    typedef const char * (ABI *StringIdToStringFn)(uint32_t, int32_t);
    StringIdToStringFn fn = (StringIdToStringFn)(uintptr_t)(g_u2_image_base + 0x00CE33A0ull);
    return fn(*(const uint32_t *)data, 0);
}

static uint32_t ABI detour_CatmullRomSpline_IsLooped(void *self) {
    /* 0x004684C0: CatmullRomSpline::IsLooped() const */
    if (!self) return 0;
    const uint8_t *data = *(const uint8_t * const *)((const uint8_t *)self + 0x10);
    if (!data) return 0;
    return (uint32_t)(data[0x14] & 1u);
}

static uint32_t ABI detour_CatmullRomSpline_GetControlPointCount(void *self) {
    /* 0x00468520: CatmullRomSpline::GetControlPointCount() const */
    if (!self) return 0;
    const uint8_t *data = *(const uint8_t * const *)((const uint8_t *)self + 0x10);
    if (!data) return 0;
    return (uint32_t)(data[0x14] & 1u) + *(const uint32_t *)(data + 0x28);
}

static float ABI detour_CatmullRomSpline_GetTotalArcLength(void *self) {
    /* 0x00468D90: CatmullRomSpline::GetTotalArcLength() const */
    if (!self) return 0.0f;
    const uint8_t *data = *(const uint8_t * const *)((const uint8_t *)self + 0x10);
    if (!data) return 0.0f;
    const float *cumul = *(const float * const *)(data + 0x48);
    if (!cumul) return 0.0f;
    uint32_t total = (uint32_t)(data[0x14] & 1u) + *(const uint32_t *)(data + 0x28);
    uint32_t segs = (total == 0) ? 0u : (total - 1u);
    return (segs == 0) ? 0.0f : cumul[segs - 1u];
}

static float ABI detour_CatmullRomSpline_GetCumulSegArcLength(void *self, int32_t seg_idx) {
    /* 0x00469F60: CatmullRomSpline::GetCumulSegArcLength(int) const */
    if (!self || seg_idx <= 0) return 0.0f;
    const uint8_t *data = *(const uint8_t * const *)((const uint8_t *)self + 0x10);
    if (!data) return 0.0f;
    const float *cumul = *(const float * const *)(data + 0x48);
    if (!cumul) return 0.0f;
    return cumul[seg_idx - 1];
}

static void ABI detour_CatmullRomManager_OnSpawnGameObject(void *mgr, void *game_obj) {
    /* 0x004643E0: CatmullRomManager::OnSpawnGameObject(GameObject *) */
    if (!mgr || !game_obj || !g_u2_image_base) return;
    uintptr_t vtbl = *(const uintptr_t *)game_obj;
    if (!vtbl) return;
    typedef void * (ABI *GetSpawnerDataFn)(void *);
    GetSpawnerDataFn get_id = *(const GetSpawnerDataFn *)(vtbl + 0xA0);
    if (!get_id) return;
    void *id = get_id(game_obj);
    if (!id) return;

    typedef void (ABI *BindGameObjectFn)(void *, void *);
    BindGameObjectFn bind_fn = (BindGameObjectFn)(uintptr_t)(g_u2_image_base + 0x004678E0ull);

    for (size_t offset = 0x20; offset != 0x19820; offset += 0x110) {
        uint8_t *base = *(uint8_t **)((uint8_t *)mgr + 0x8);
        if (!base) break;
        if (*(const uint64_t *)(base + offset - 0x20) != 0) {
            const uint8_t *spline_data = *(const uint8_t * const *)(base + offset);
            if (spline_data && *(void * const *)(spline_data + 0x8) == id) {
                bind_fn(base + offset - 0x10, game_obj);
            }
        }
    }
}

static void ABI detour_CatmullRomManager_OnDestroyGameObject(void *mgr, void *game_obj) {
    /* 0x00464490: CatmullRomManager::OnDestroyGameObject(GameObject *) */
    if (!mgr || !game_obj || !g_u2_image_base) return;
    uintptr_t vtbl = *(const uintptr_t *)game_obj;
    if (!vtbl) return;
    typedef void * (ABI *GetSpawnerDataFn)(void *);
    GetSpawnerDataFn get_id = *(const GetSpawnerDataFn *)(vtbl + 0xA0);
    if (!get_id) return;
    void *id = get_id(game_obj);
    if (!id) return;

    typedef void (ABI *BindGameObjectFn)(void *, void *);
    BindGameObjectFn bind_fn = (BindGameObjectFn)(uintptr_t)(g_u2_image_base + 0x004678E0ull);

    for (size_t offset = 0x20; offset != 0x19820; offset += 0x110) {
        uint8_t *base = *(uint8_t **)((uint8_t *)mgr + 0x8);
        if (!base) break;
        if (*(const uint64_t *)(base + offset - 0x20) != 0) {
            const uint8_t *spline_data = *(const uint8_t * const *)(base + offset);
            if (spline_data && *(void * const *)(spline_data + 0x8) == id) {
                bind_fn(base + offset - 0x10, NULL);
            }
        }
    }
}

static void ABI detour_CatmullRomManager_RemoveSpline(void *mgr, void *spline_data) {
    /* 0x00465240: CatmullRomManager::RemoveSpline(CatmullRomSplineData *) */
    if (!mgr || !g_u2_image_base) return;
    typedef void (ABI *RemoveCallbackFn)(void *, void *);
    typedef void (ABI *UnregisterSpecialSplineFn)(void *, void *);
    UnregisterSpecialSplineFn unreg_fn = (UnregisterSpecialSplineFn)(uintptr_t)(g_u2_image_base + 0x003D1EA0ull);

    for (size_t offset = 0; offset != 0x19800; offset += 0x110) {
        uint8_t *base = *(uint8_t **)((uint8_t *)mgr + 0x8);
        if (!base) break;
        if (*(void **)(base + offset) == spline_data) {
            uint32_t type = spline_data ? *(const uint32_t *)((const uint8_t *)spline_data + 0x10) : 0;
            int is_special = (type == 0x0E3532C9u || type == 0x642C65B1u || type == 0x6749641Eu);
            RemoveCallbackFn cb = *(const RemoveCallbackFn *)((const uint8_t *)mgr + 0x68);
            if (cb) {
                cb(base + offset + 0x10, *(void **)((uint8_t *)mgr + 0x70));
            }
            if (is_special) {
                unreg_fn(g_u2_image_base + 0x01112430ull, base + offset + 0x10);
            }
            (*(uint32_t *)mgr)--;
            base = *(uint8_t **)((uint8_t *)mgr + 0x8);
            if (base) {
                *(uint64_t *)(base + offset) = 0;
                *(uint64_t *)(base + offset + 0x20) = 0;
            }
        }
    }
}

static void ABI detour_CatmullRomManager_WalkSplines(void *mgr, void (ABI *cb)(void *, void *), void *user_data, int32_t all_flag) {
    /* 0x00466320: CatmullRomManager::WalkSplines(callback, user_data, bool) */
    if (!mgr || !cb) return;
    for (size_t offset = 0x10; offset != 0x19810; offset += 0x110) {
        uint8_t *base = *(uint8_t **)((uint8_t *)mgr + 0x8);
        if (!base) break;
        if (*(const uint64_t *)(base + offset - 0x10) != 0) {
            if (all_flag == 1 || (base[offset] & 0x5u) == 0) {
                cb(base + offset, user_data);
            }
        }
    }
}

static void * ABI detour_SplineTracker_GetSpline0(void *self) {
    /* 0x00A24040: SplineTracker::GetSpline() */
    if (!self || !g_u2_image_base) return NULL;
    int64_t idx = *(const int32_t *)((const uint8_t *)self + 0x30);
    if (idx < 0) return NULL;
    uint8_t *arr = *(uint8_t **)(g_u2_image_base + 0x0104EB30ull + 0x8);
    if (!arr) return NULL;
    uint8_t *entry = arr + (size_t)idx * 0x110;
    if (*(void **)(entry + 0x20) != *(void * const *)((const uint8_t *)self + 0x38)) return NULL;
    return entry + 0x10;
}

static void * ABI detour_SplineTracker_GetSpline1(void *self) {
    /* 0x00A24080: SplineTracker::GetSpline() const */
    return detour_SplineTracker_GetSpline0(self);
}

static uint32_t ABI detour_SplineTracker_GetSplineNameId(void *self) {
    /* 0x00A240C0: SplineTracker::GetSplineNameId() const */
    if (!self || !g_u2_image_base) return 0;
    int64_t idx = *(const int32_t *)((const uint8_t *)self + 0x30);
    if (idx < 0) return 0;
    uint8_t *arr = *(uint8_t **)(g_u2_image_base + 0x0104EB30ull + 0x8);
    if (!arr) return 0;
    uint8_t *entry = arr + (size_t)idx * 0x110;
    const uint32_t *data = *(const uint32_t * const *)(entry + 0x20);
    if ((const void *)data != *(const void * const *)((const uint8_t *)self + 0x38) || !data) return 0;
    return *data;
}

static void ABI detour_SplineTracker_FinalizeDistance(void *self) {
    /* 0x00A24110: SplineTracker::FinalizeDistance() */
    if (!self || !g_u2_image_base) return;
    typedef float (ABI *GetRealDistFn)(void *, uint32_t);
    GetRealDistFn fn = (GetRealDistFn)(uintptr_t)(g_u2_image_base + 0x00A23EC0ull);
    float d = fn(self, 1);
    *(float *)((uint8_t *)self + 0x40) = d;
    *(uint32_t *)((uint8_t *)self + 0x44) = 0;
    *(uint32_t *)((uint8_t *)self + 0x50) = 0;
}

static float ABI detour_VehicleSplineTracker_GetCurrentVirtualDistance_m(void *self, int32_t mode) {
    /* 0x00ABA7B0: VehicleSplineTracker::GetCurrentVirtualDistance_m(int) const */
    if (!self) return 0.0f;
    if (mode == 0) return *(const float *)((const uint8_t *)self + 0x40);
    if (mode == 1) return *(const float *)((const uint8_t *)self + 0x190);
    return 0.0f;
}

static float ABI detour_VehicleSplineTracker_GetCurrentRealDistance_m(void *self, int32_t mode, uint32_t wrap) {
    /* 0x00ABA840: VehicleSplineTracker::GetCurrentRealDistance_m(int, bool) const */
    if (!self || !g_u2_image_base) return 0.0f;
    typedef float (ABI *GetRealDistFn)(void *, uint32_t);
    GetRealDistFn fn = (GetRealDistFn)(uintptr_t)(g_u2_image_base + 0x00A23EC0ull);
    if (mode == 0) return fn(self, wrap & 0xFFu);
    if (mode == 1) return fn((uint8_t *)self + 0x150, wrap & 0xFFu);
    return 0.0f;
}

static void ABI detour_CameraZoom_OverideHorizontalAngularConstraints(const float *dir, float min_a, float max_a) {
    /* 0x0044BF00: CameraZoom::OverideHorizontalAngularConstraints(Vector const&, float, float) */
    if (!dir || !g_u2_image_base) return;
    *(uint8_t *)(g_u2_image_base + 0x0147FDE0ull) = 1;
    *(float *)(g_u2_image_base + 0x0104EB10ull) = min_a;
    memcpy(g_u2_image_base + 0x011142B0ull, dir, 16);
    *(float *)(g_u2_image_base + 0x011142ACull) = max_a;
}

static void ABI detour_CameraZoom_ClearHorizontalAngularConstraints(void) {
    /* 0x0044BFA0: CameraZoom::ClearHorizontalAngularConstraints() */
    if (!g_u2_image_base) return;
    *(uint8_t *)(g_u2_image_base + 0x0147FDE0ull) = 0;
    *(uint32_t *)(g_u2_image_base + 0x0104EB10ull) = 0xBF800000u; /* -1.0f */
    memcpy(g_u2_image_base + 0x011142B0ull, g_u2_image_base + 0x0147FDD0ull, 16);
    *(uint32_t *)(g_u2_image_base + 0x011142ACull) = 0;
}

static void ABI detour_CameraZoom_OverideVerticalAngularConstraints(float min_a, float max_a) {
    /* 0x0044BFE0: CameraZoom::OverideVerticalAngularConstraints(float, float) */
    if (!g_u2_image_base) return;
    *(uint8_t *)(g_u2_image_base + 0x0147FDE1ull) = 1;
    *(float *)(g_u2_image_base + 0x0104EB14ull) = min_a;
    *(float *)(g_u2_image_base + 0x0104EB18ull) = max_a;
}

static void ABI detour_CameraZoom_ClearVerticalAngularConstraints(void) {
    /* 0x0044C060: CameraZoom::ClearVerticalAngularConstraints() */
    if (!g_u2_image_base) return;
    *(uint8_t *)(g_u2_image_base + 0x0147FDE1ull) = 0;
    *(float *)(g_u2_image_base + 0x0104EB14ull) = *(const float *)(g_u2_image_base + 0x0104EB1Cull);
    *(float *)(g_u2_image_base + 0x0104EB18ull) = *(const float *)(g_u2_image_base + 0x0104EB20ull);
}

static void ABI detour_CameraZoom_SetDefaultVerticalAngularConstraints(float min_a, float max_a) {
    /* 0x0044C090: CameraZoom::SetDefaultVerticalAngularConstraints(float, float) */
    if (!g_u2_image_base) return;
    *(float *)(g_u2_image_base + 0x0104EB1Cull) = min_a;
    *(float *)(g_u2_image_base + 0x0104EB20ull) = max_a;
    if (*(const uint8_t *)(g_u2_image_base + 0x0147FDE1ull) == 0) {
        *(float *)(g_u2_image_base + 0x0104EB14ull) = min_a;
        *(float *)(g_u2_image_base + 0x0104EB18ull) = max_a;
    }
}

static void ABI detour_RailVehicle_InitBehavior(void *self, void *dc_behavior) {
    /* 0x000013D0: RailVehicle::InitBehavior(DcRailVehicleBehavior const*) */
    if (!self || !dc_behavior || !g_u2_image_base) return;
    typedef void (ABI *ContainerInitFn)(void *, void *, void *);
    ContainerInitFn fn = (ContainerInitFn)(uintptr_t)(g_u2_image_base + 0x00A8EAA0ull);
    fn((uint8_t *)self + 0xEB0, self, dc_behavior);
}

static void ABI detour_RailVehicle_SetFollowDistance(void *self, float begin_dist, float end_dist) {
    /* 0x00001460: RailVehicle::SetFollowDistance(float, float) */
    if (!self) return;
    *(float *)((uint8_t *)self + 0x560) = begin_dist;
    *(float *)((uint8_t *)self + 0x564) = end_dist;
}

static void * ABI detour_RailVehicle_GetVehicleClass(void *self) {
    /* 0x00001DD0: RailVehicle::GetVehicleClass() */
    if (!self || !g_u2_image_base) return NULL;
    void *cls = *(void **)((uint8_t *)self + 0x260);
    if (!cls) {
        uint32_t sid = *(const uint32_t *)((const uint8_t *)self + 0x25C);
        typedef void * (ABI *LookupTypeFn)(uint32_t);
        LookupTypeFn fn = (LookupTypeFn)(uintptr_t)(g_u2_image_base + 0x00D75840ull);
        cls = fn(sid);
        *(void **)((uint8_t *)self + 0x260) = cls;
    }
    return cls;
}

static void ABI detour_RailVehicle_ResetWheels(void *self) {
    /* 0x00001ED0: RailVehicle::ResetWheels() */
    if (!self) return;
    uint32_t count = *(const uint32_t *)((const uint8_t *)self + 0x750);
    for (uint32_t i = 0; i < count; ++i) {
        *((uint8_t *)self + 0x820 + (size_t)i * 0xD0 + 0xC8) = 0;
    }
}

static void * ABI detour_RailVehicle_GetDamageable(void *self, uint32_t index) {
    /* 0x00002430: RailVehicle::GetDamageable(unsigned int) */
    if (!self || !g_u2_image_base) return NULL;
    uint8_t *cls = (uint8_t *)detour_RailVehicle_GetVehicleClass(self);
    if (!cls) return NULL;
    uint32_t count = *(const uint32_t *)(cls + 0xD0);
    if (index >= count) return NULL;
    uint8_t *arr = *(uint8_t **)(cls + 0xD8);
    return arr ? (arr + (size_t)index * 0x30) : NULL;
}

static void ABI detour_RailVehicle_OnIgcBind(void *self) {
    /* 0x00005630: RailVehicle::OnIgcBind() */
    if (!self || !g_u2_image_base) return;
    int32_t cnt = *(const int32_t *)((const uint8_t *)self + 0xDF0);
    if (cnt == 0) {
        *(uint8_t *)(g_u2_image_base + 0x011362F8ull + 0x31) = 1;
    }
    *(int32_t *)((uint8_t *)self + 0xDF0) = cnt + 1;
    *(uint32_t *)((uint8_t *)self + 0xE08) = 0xFFFFFFFFu;
}

static void ABI detour_RailVehicle_OnIgcUnbind(void *self) {
    /* 0x000056E0: RailVehicle::OnIgcUnbind() */
    if (!self || !g_u2_image_base) return;
    int32_t cnt = *(const int32_t *)((const uint8_t *)self + 0xDF0);
    if (cnt > 0) {
        cnt--;
        *(int32_t *)((uint8_t *)self + 0xDF0) = cnt;
        if (cnt == 0) {
            uint32_t flags = *(const uint32_t *)((const uint8_t *)self + 0x5A0);
            flags = (flags & ~0x2020u) | 0x2020u;
            *(uint32_t *)((uint8_t *)self + 0x5A0) = flags;
            *(uint32_t *)((uint8_t *)self + 0xE0C) = 0;
            *(uint8_t *)(g_u2_image_base + 0x011362F8ull + 0x31) = 0;
            typedef float (ABI *GetTimeFn)(void *);
            GetTimeFn get_time = (GetTimeFn)(uintptr_t)(g_u2_image_base + 0x00A90590ull);
            float t = get_time(g_u2_image_base + 0x0116454Bull);
            *(float *)((uint8_t *)self + 0xDFC) = t + *(const float *)(g_u2_image_base + 0x00DF440Cull);
        }
    }
}

static void * ABI detour_RailVehicleBehavior_GetDcBehavior(void *self) {
    /* 0x00A8DE50: RailVehicleBehavior::GetDcBehavior() const */
    if (!self) return NULL;
    return *(void **)((uint8_t *)self + 0x18);
}

static void * ABI detour_RailVehicleBehaviorContainer_GetCurrentBehavior(void *self) {
    /* 0x00A8EA30: RailVehicleBehaviorContainer::GetCurrentBehavior() */
    if (!self) return NULL;
    uint32_t type = *(const uint32_t *)self;
    if (type <= 1u) {
        return (uint8_t *)self + 0x70;
    }
    if (type != 2u) {
        *(uint32_t *)self = 2u;
    }
    return (uint8_t *)self + 0x10;
}

static void * ABI detour_RailVehicleManager_GetMover(void *self, uint32_t idx) {
    /* 0x00A8FCE0: RailVehicleManager::GetMover(unsigned int) */
    (void)self;
    if (!g_u2_image_base) return NULL;
    return g_u2_image_base + 0x0153EFA8ull + 0xD18ull + (size_t)idx * 0x30;
}

static void ABI detour_RailVehicleManager_DetachFromRail(void *self, void *vehicle, int32_t rail_idx) {
    /* 0x00A903E0: RailVehicleManager::DetachFromRail(RailVehicle *, int) */
    (void)self;
    (void)vehicle;
    if (!g_u2_image_base || rail_idx < 0) return;
    uint8_t *rail = g_u2_image_base + 0x0153EFA8ull + (size_t)(uint32_t)rail_idx * 0x60;
    rail[0x12C] &= (uint8_t)~0x2u;
    *(uint64_t *)(rail + 0x118) = 0;
    *(uint32_t *)(rail + 0x120) = 0;
    *(float *)(rail + 0x128) = *(const float *)(rail + 0xF8);
}

static const U2NativeDetour s_scene_detours[] = {
    { "DcEntityName", 0x00A127D0, (uintptr_t)detour_DcEntityName },
    { "DcObjectSpawnedP", 0x00A12780, (uintptr_t)detour_DcObjectSpawnedP },
    { "DcEntityAutoSpawnOnce", 0x00A12830, (uintptr_t)detour_DcEntityAutoSpawnOnce },
    { "DcPartModuleLoadedP", 0x00A00970, (uintptr_t)detour_DcPartModuleLoadedP },
    { "DcDisableFrameForward", 0x00A038A0, (uintptr_t)detour_DcDisableFrameForward },
    { "DcUnparent", 0x00A03940, (uintptr_t)detour_DcUnparent },
    { "DcEnableGadgetry", 0x00A039C0, (uintptr_t)detour_DcEnableGadgetry },
    { "DcDisableGadgetry", 0x00A03A70, (uintptr_t)detour_DcDisableGadgetry },
    { "DcSymbolEqualP", 0x00397620, (uintptr_t)detour_DcSymbolEqualP },
    { "DcCameraAimOverrideDirection", 0x00A0DE40, (uintptr_t)detour_DcCameraAimOverrideDirection },
    { "DcCameraCancelAimOverride", 0x00A0DE30, (uintptr_t)detour_DcCameraCancelAimOverride },
    { "DcCameraRestoreLocator", 0x00A0E9B0, (uintptr_t)detour_DcCameraRestoreLocator },
    { "DcCameraAbandon", 0x00A0E5E0, (uintptr_t)detour_DcCameraAbandon },
    { "DcCameraDisable", 0x00A0E500, (uintptr_t)detour_DcCameraDisable },
    { "DcSplineTransferPendingP", 0x00A25A20, (uintptr_t)detour_DcSplineTransferPendingP },
    { "DcDebugPause", 0x00A102A0, (uintptr_t)detour_DcDebugPause },
    { "DcDiscP", 0x009FD910, (uintptr_t)detour_DcDiscP },
    { "DcResetAimFov", 0x0090D780, (uintptr_t)detour_DcResetAimFov },
    { "DcOverrideAimFov", 0x0090D7A0, (uintptr_t)detour_DcOverrideAimFov },
    { "DcGetHarvestingPriority", 0x00A01AB0, (uintptr_t)detour_DcGetHarvestingPriority },
    { "DcSetHarvestingPriority", 0x00A01AC0, (uintptr_t)detour_DcSetHarvestingPriority },
    { "DcResetHarvestingPriority", 0x00A01AE0, (uintptr_t)detour_DcResetHarvestingPriority },
    { "DcHarvestingEnabledP", 0x00A01AF0, (uintptr_t)detour_DcHarvestingEnabledP },
    { "DcSetHarvestingEnabled", 0x00A01B00, (uintptr_t)detour_DcSetHarvestingEnabled },
    { "DcGroundPrintsType", 0x00A01F00, (uintptr_t)detour_DcGroundPrintsType },
    { "DcHideHud", 0x00A0CD00, (uintptr_t)detour_DcHideHud },
    { "DcIsFma", 0x00A0FE80, (uintptr_t)detour_DcIsFma },
    { "DcJournalClearPage", 0x009FD090, (uintptr_t)detour_DcJournalClearPage },
    { "DcJournalGetPage", 0x009FD110, (uintptr_t)detour_DcJournalGetPage },
    { "DcKillAllParticles", 0x00A0FE60, (uintptr_t)detour_DcKillAllParticles },
    { "DcDespawnMessage", 0x005DC8C0, (uintptr_t)detour_DcDespawnMessage },
    { "DcPauseCoopCombo", 0x00667F60, (uintptr_t)detour_DcPauseCoopCombo },
    { "DcGetCaptureZone", 0x006687D0, (uintptr_t)detour_DcGetCaptureZone },
    { "DcNetEndWave", 0x006687E0, (uintptr_t)detour_DcNetEndWave },
    { "DcNetGetGrappleTime", 0x006687F0, (uintptr_t)detour_DcNetGetGrappleTime },
    { "DcNetGetBoosterEnabled", 0x00668810, (uintptr_t)detour_DcNetGetBoosterEnabled },
    { "DcNetGetBoosterAmount", 0x00668820, (uintptr_t)detour_DcNetGetBoosterAmount },
    { "DcSetForceTension", 0x00667EE0, (uintptr_t)detour_DcSetForceTension },
    { "DcNetCoopTask", 0x006697E0, (uintptr_t)detour_DcNetCoopTask },
    { "DcNetPlyaerCoopId", 0x00669240, (uintptr_t)detour_DcNetPlyaerCoopId },
    { "DcNetKillTerritory", 0x00669800, (uintptr_t)detour_DcNetKillTerritory },
    { "DcNetSpawnTerritory", 0x00669840, (uintptr_t)detour_DcNetSpawnTerritory },
    { "DcNetSpawnFlag", 0x006698C0, (uintptr_t)detour_DcNetSpawnFlag },
    { "DcNetRespawnFlag", 0x006698E0, (uintptr_t)detour_DcNetRespawnFlag },
    { "DcNetCoopTeamFailedP", 0x00669C20, (uintptr_t)detour_DcNetCoopTeamFailedP },
    { "DcNetCoopScriptId", 0x00669C30, (uintptr_t)detour_DcNetCoopScriptId },
    { "DcNetCoopHostScriptId", 0x00669C40, (uintptr_t)detour_DcNetCoopHostScriptId },
    { "DcNetIsCinema", 0x00669C50, (uintptr_t)detour_DcNetIsCinema },
    { "DcNetIsHost", 0x00669C80, (uintptr_t)detour_DcNetIsHost },
    { "DcPsnDemoShowUpsell", 0x009FD590, (uintptr_t)detour_DcPsnDemoShowUpsell },
    { "DcDebugDrawCross", 0x009FE070, (uintptr_t)detour_DcDebugDrawCross },
    { "DcAutoSaveGame", 0x009FE820, (uintptr_t)detour_DcAutoSaveGame },
    { "DcUpdateTaskMenu", 0x009FE8C0, (uintptr_t)detour_DcUpdateTaskMenu },
    { "DcGameTaskGraphLogin", 0x009FE8F0, (uintptr_t)detour_DcGameTaskGraphLogin },
    { "DcRemoveDynamicLight", 0x009FEDF0, (uintptr_t)detour_DcRemoveDynamicLight },
    { "DcRestoreBlur", 0x009FFF20, (uintptr_t)detour_DcRestoreBlur },
    { "DcDeactivateBlur", 0x009FFF50, (uintptr_t)detour_DcDeactivateBlur },
    { "DcRailSetIgcFadeOutTime", 0x00AB93F0, (uintptr_t)detour_DcRailSetIgcFadeOutTime },
    { "DcTrainGetDefaultAcceleration", 0x00A682E0, (uintptr_t)detour_DcTrainGetDefaultAcceleration },
    { "DcKillAllDroppedWeapons", 0x00A0F1B0, (uintptr_t)detour_DcKillAllDroppedWeapons },
    { "DcKillAllBreakables", 0x00A0F260, (uintptr_t)detour_DcKillAllBreakables },
    { "DcSetFocusableGrappled", 0x00A0F2C0, (uintptr_t)detour_DcSetFocusableGrappled },
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
    { "DcSetMaxCharacterWetness", 0x00A02510, (uintptr_t)detour_DcSetMaxCharacterWetness },
    { "DcSetTimeToDryPeriod", 0x00A02780, (uintptr_t)detour_DcSetTimeToDryPeriod },
    { "DcForceReflections", 0x00A02960, (uintptr_t)detour_DcForceReflections },
    { "DcUseReflectionPlane", 0x00A029A0, (uintptr_t)detour_DcUseReflectionPlane },
    { "DcUseVisibleWaterUnitForReflectionWithTolerance", 0x00A02B70, (uintptr_t)detour_DcUseVisibleWaterUnitForReflectionWithTolerance },
    { "DcUseVisibleWaterUnitForReflection", 0x00A02BC0, (uintptr_t)detour_DcUseVisibleWaterUnitForReflection },
    { "DcUseReflections", 0x00A02C10, (uintptr_t)detour_DcUseReflections },
    { "DcSetRainRipplesState", 0x00A027F0, (uintptr_t)detour_DcSetRainRipplesState },
    { "DcStopScriptRumbles", 0x00A105B0, (uintptr_t)detour_DcStopScriptRumbles },
    { "DcStringFromStringId", 0x009FDE30, (uintptr_t)detour_DcStringFromStringId },
    { "DcStringIdFromString", 0x009FDE00, (uintptr_t)detour_DcStringIdFromString },
    { "DcSwimDisable", 0x0090D760, (uintptr_t)detour_DcSwimDisable },
    { "DcSwimDisallowStandOnBottom", 0x0090D680, (uintptr_t)detour_DcSwimDisallowStandOnBottom },
    { "DcSwimEnable", 0x0090D770, (uintptr_t)detour_DcSwimEnable },
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
    { "DcWeaponSetReset", 0x0090AD00, (uintptr_t)detour_DcWeaponSetReset },
    { "DcInvalidStringId", 0x009FDE50, (uintptr_t)detour_DcInvalidStringId },
    { "DcPrintPoint", 0x00A10290, (uintptr_t)detour_DcPrintNoOp },
    { "DcPrintSymbol", 0x00A10280, (uintptr_t)detour_DcPrintNoOp },
    { "DcOceanBobEnable", 0x00A033A0, (uintptr_t)detour_DcNoOp0 },
    { "DcOceanBobDisable", 0x00A03390, (uintptr_t)detour_DcNoOp0 },
    { "DcDisplay", 0x009FD000, (uintptr_t)detour_DcNoOp0 },
    { "DcGetDda", 0x009FD010, (uintptr_t)detour_DcGetDda },
    { "DcDisplayInt", 0x009FCFF0, (uintptr_t)detour_DcNoOp0 },
    { "DcMessage", 0x005DC620, (uintptr_t)detour_DcNoOp0 },
    { "DcAnimStateAdvancePhase", 0x00397690, (uintptr_t)detour_DcNoOp0 },
    { "DcHideHint", 0x00A0CC10, (uintptr_t)detour_DcHideHint },
    { "DcGetObjectId", 0x00A0BEE0, (uintptr_t)detour_DcGetObjectId },
    { "DcGetSelfId", 0x00A04A00, (uintptr_t)detour_DcGetSelfId },
    { "DcSetObjectHealth", 0x00A04A20, (uintptr_t)detour_DcSetObjectHealth },
    { "DcGetObjectHealth", 0x00A04B10, (uintptr_t)detour_DcGetObjectHealth },
    { "DcRenderSettingsApplyDefaults", 0x00A10760, (uintptr_t)detour_DcRenderSettingsApplyDefaults },
    { "DcClearHarvestedBodies", 0x00A01A90, (uintptr_t)detour_DcClearHarvestedBodies },
    { "DcResetActors", 0x009FD690, (uintptr_t)detour_DcResetActors },
    { "DcSetBarrelFlowMultiplier", 0x00A01EF0, (uintptr_t)detour_DcNoOp0 },
    { "DcSetJetskiFlowMultiplier", 0x00A01B70, (uintptr_t)detour_DcNoOp0 },
    { "DcSetJetskiMaxThrottle", 0x00A01B60, (uintptr_t)detour_DcNoOp0 },
    { "DcSetJetskiNearStupidRampInRuins2", 0x00A01B50, (uintptr_t)detour_DcNoOp0 },
    { "DcJetskiClearEngineSoundOverride", 0x00A01B20, (uintptr_t)detour_DcNoOp0 },
    { "DcJetskiOverrideEngineSound", 0x00A01B30, (uintptr_t)detour_DcNoOp0 },
    { "DcJetskiStartEngineSound", 0x00A01B40, (uintptr_t)detour_DcNoOp0 },
    { "DcSetE3LoadingScreen", 0x009FD5B0, (uintptr_t)detour_DcNoOp0 },
    { "DcEntitySetLevelB", 0x00A64C30, (uintptr_t)detour_DcNoOp0 },
    { "DcFmaResetPartModules", 0x00A00130, (uintptr_t)detour_DcNoOp0 },
    { "DcSpawnParticlesAtJoint", 0x00A00950, (uintptr_t)detour_DcSpawnParticlesAtJoint },
    { "DcSpawnParticlesAttached", 0x00A00960, (uintptr_t)detour_DcSpawnParticlesAttached },
    { "DcNetHideWaveScreen", 0x00668850, (uintptr_t)detour_DcNetHideWaveScreen },
    { "DcStringFromFloat", 0x009FDE60, (uintptr_t)detour_DcStringFromFloat },
    { "DcDebugDrawLine", 0x009FE090, (uintptr_t)detour_DcDebugDrawLine },
    { "DcPlayMovie", 0x009FE100, (uintptr_t)detour_DcPlayMovie },
    { "DcStartRumble", 0x009FF3D0, (uintptr_t)detour_DcStartRumble },
    { "DcStopRumble", 0x009FF370, (uintptr_t)detour_DcStopRumble },
    { "DcNetAllPlayersDeadP", 0x006694B0, (uintptr_t)detour_DcNetAllPlayersDeadP },
    { "DcDebugDrawString", 0x009FDFB0, (uintptr_t)detour_DcDebugDrawString },
    { "DcSetCharacterWetMaskState", 0x00A02690, (uintptr_t)detour_DcSetCharacterWetMaskState },
    { "DcUseWaterUnitForReflection", 0x00A02C70, (uintptr_t)detour_DcUseWaterUnitForReflection },
    { "DcLerpReflectionPlaneOfWaterUnits", 0x00A02B10, (uintptr_t)detour_DcLerpReflectionPlaneOfWaterUnits },
    { "DcRemoveCoverActionPacksFromObject", 0x00A04DC0, (uintptr_t)detour_DcRemoveCoverActionPacksFromObject },
    { "DcRegenerateCoverActionPacksFromObject", 0x00A04E20, (uintptr_t)detour_DcRegenerateCoverActionPacksFromObject },
    { "DcEnableNavBlockerObject", 0x00A04F60, (uintptr_t)detour_DcEnableNavBlockerObject },
    { "DcBlockNavPolyUnderObject", 0x00A05030, (uintptr_t)detour_DcBlockNavPolyUnderObject },
    { "DcKillEntity", 0x00A09D60, (uintptr_t)detour_DcKillEntity },
    { "DcSetWhiteFade", 0x00A106A0, (uintptr_t)detour_DcSetWhiteFade },
    { "DcSetBlackFade", 0x00A10700, (uintptr_t)detour_DcSetBlackFade },
    { "DcMakeInert", 0x009FD850, (uintptr_t)detour_DcMakeInert },
    { "DcDisablePlatformControlOnObject", 0x00A04E80, (uintptr_t)detour_DcDisablePlatformControlOnObject },
    { "DcDisableNavBlockerObject", 0x00A04EF0, (uintptr_t)detour_DcDisableNavBlockerObject },
    { "DcUnblockNavPolyUnderObject", 0x00A04FC0, (uintptr_t)detour_DcUnblockNavPolyUnderObject },
    { "DcDumpLevelsInfo", 0x00A04900, (uintptr_t)detour_DcDumpLevelsInfo },
    { "DcLerpReflectionPlanes", 0x00A02AA0, (uintptr_t)detour_DcLerpReflectionPlanes },
    { "DcSlowMoEffect", 0x00A0EE10, (uintptr_t)detour_DcSlowMoEffect },
    { "DcRailSetNpcArchetype", 0x00AB9660, (uintptr_t)detour_DcRailSetNpcArchetype },
    { "DcDebugDrawLocator", 0x009FE000, (uintptr_t)detour_DcDebugDrawLocator },
    { "DcAddDynamicLight", 0x009FF2C0, (uintptr_t)detour_DcAddDynamicLight },
    { "DcAddDynamicLightJoint", 0x009FF010, (uintptr_t)detour_DcAddDynamicLightJoint },
    { "DcApplyExplosionImpulse", 0x00A03B20, (uintptr_t)detour_DcApplyExplosionImpulse },
    { "DcAttachFlashlight", 0x00A0A4C0, (uintptr_t)detour_DcAttachFlashlight },
    { "DcCameraAnimationDisable", 0x00A033B0, (uintptr_t)detour_DcCameraAnimationDisable },
    { "DcCameraAnimationEnable", 0x00A03480, (uintptr_t)detour_DcCameraAnimationEnable },
    { "DcCameraShake", 0x00A0E040, (uintptr_t)detour_DcCameraShake },
    { "DcConstrainRopeAtJoint", 0x00A04430, (uintptr_t)detour_DcConstrainRopeAtJoint },
    { "DcCreateWeaponDrawables", 0x00A0C410, (uintptr_t)detour_DcCreateWeaponDrawables },
    { "DcDetachRope", 0x00A04530, (uintptr_t)detour_DcDetachRope },
    { "DcDropRope", 0x00A045E0, (uintptr_t)detour_DcDropRope },
    { "DcDoRumblePtr", 0x00A105F0, (uintptr_t)detour_DcDoRumblePtr },
    { "DcJournalSetPage", 0x009FD130, (uintptr_t)detour_DcJournalSetPage },
    { "DcLetterboxOff", 0x009FFD60, (uintptr_t)detour_DcLetterboxOff },
    { "DcLetterboxOn", 0x009FFE00, (uintptr_t)detour_DcLetterboxOn },
    { "DcRopeEnableCharacterCollision", 0x00A04270, (uintptr_t)detour_DcRopeEnableCharacterCollision },
    { "DcRopeDisableCharacterCollision", 0x00A04340, (uintptr_t)detour_DcRopeDisableCharacterCollision },
    { "DcUseRainIntensityValues", 0x00A031C0, (uintptr_t)detour_DcUseRainIntensityValues },
    { "DcUseWindIntensityValues", 0x00A03290, (uintptr_t)detour_DcUseWindIntensityValues },
    { "DcKillFlashlight", 0x00A0A130, (uintptr_t)detour_DcKillFlashlight },
    { "DcCameraFadeOut", 0x00A0E380, (uintptr_t)detour_DcCameraFadeOut },
    { "DcCameraShakeInZoomCamera", 0x00A0DCA0, (uintptr_t)detour_DcCameraShakeInZoomCamera },
    { "DcDisplayButtonHud", 0x00A0CD20, (uintptr_t)detour_DcDisplayButtonHud },
    { "DcFadeToBlack", 0x009FFBA0, (uintptr_t)detour_DcFadeToBlack },
    { "DcFadeToBlackPers", 0x009FFA40, (uintptr_t)detour_DcFadeToBlackPers },
    { "DcFadeToWhite", 0x009FFAC0, (uintptr_t)detour_DcFadeToWhite },
    { "DcGetWaterOffset", 0x00A02F20, (uintptr_t)detour_DcGetWaterOffset },
    { "DcGetWaterState", 0x00A02D90, (uintptr_t)detour_DcGetWaterState },
    { "DcGivePlayerFlashlight", 0x00A0ADC0, (uintptr_t)detour_DcGivePlayerFlashlight },
    { "DcGiveSullivanFlashlight", 0x00A0ABC0, (uintptr_t)detour_DcGiveSullivanFlashlight },
    { "DcKillPlayerFlashlight", 0x00A0A920, (uintptr_t)detour_DcKillPlayerFlashlight },
    { "DcKillSullivanFlashlight", 0x00A0A750, (uintptr_t)detour_DcKillSullivanFlashlight },
    { "DcHeliEnableWind", 0x00A7EB60, (uintptr_t)detour_DcHeliEnableWind },
    { "DcKillParticles", 0x00A00140, (uintptr_t)detour_DcKillParticles },
    { "DcScanParticleDirectories", 0x00A19360, (uintptr_t)detour_DcScanParticleDirectories },
    { "DcSetDeformationOfWaterUnit", 0x00A02CC0, (uintptr_t)detour_DcSetDeformationOfWaterUnit },
    { "DcSetDynamicLightColor", 0x009FEE20, (uintptr_t)detour_DcSetDynamicLightColor },
    { "DcSetLightFov", 0x009FFC80, (uintptr_t)detour_DcSetLightFov },
    { "DcSetMeshVisible", 0x00A0BA20, (uintptr_t)detour_DcSetMeshVisible },
    { "DcSetRainState", 0x00A02830, (uintptr_t)detour_DcSetRainState },
    { "DcSetWaterOffset", 0x00A02FF0, (uintptr_t)detour_DcSetWaterOffset },
    { "DcSetWaterState", 0x00A02E20, (uintptr_t)detour_DcSetWaterState },
    { "DcSpawnSnowDisturb", 0x00A01BB0, (uintptr_t)detour_DcSpawnSnowDisturb },
    { "DcSullivanFlashlightToHand", 0x00A0A6A0, (uintptr_t)detour_DcSullivanFlashlightToHand },
    { "DcSullivanFlashlightToWaist", 0x00A0A5F0, (uintptr_t)detour_DcSullivanFlashlightToWaist },
    { "DcTurnOnFlashlight", 0x00A0A250, (uintptr_t)detour_DcTurnOnFlashlight },
    { "DcUpdateDynamicLight", 0x009FEEE0, (uintptr_t)detour_DcUpdateDynamicLight },
    { "DcUseMaterialFromWaterUnit", 0x00A02020, (uintptr_t)detour_DcUseMaterialFromWaterUnit },
    { "DcSetVisible", 0x00A0BD30, (uintptr_t)detour_DcSetVisible },
    { "DcDifficultyInterp", 0x009FDAD0, (uintptr_t)detour_DcDifficultyInterp },
    { "DcPadGetButtonPressInterval", 0x00A0D060, (uintptr_t)detour_DcPadGetButtonPressInterval },
    { "DcAllowCameraCutCompensate", 0x00A0DD40, (uintptr_t)detour_DcAllowCameraCutCompensate },
    { "DcDisableCoverCamera", 0x00A0E290, (uintptr_t)detour_DcDisableCoverCamera },
    { "DcDisableMessage", 0x005DC990, (uintptr_t)detour_DcDisableMessage },
    { "DcDisplayMessage", 0x009FE140, (uintptr_t)detour_DcDisplayMessage },
    { "DcDumpEntityInfo", 0x00A04970, (uintptr_t)detour_DcDumpEntityInfo },
    { "DcEnableActionPack", 0x00A0A060, (uintptr_t)detour_DcEnableActionPack },
    { "DcEnableMessage", 0x005DC8E0, (uintptr_t)detour_DcEnableMessage },
    { "DcEndScript", 0x00A44190, (uintptr_t)detour_DcEndScript },
    { "DcFacingPointP", 0x00A0F6D0, (uintptr_t)detour_DcFacingPointP },
    { "DcFireWeapon", 0x00A0C930, (uintptr_t)detour_DcFireWeapon },
    { "DcFireWeaponAtTarget", 0x00A0C4C0, (uintptr_t)detour_DcFireWeaponAtTarget },
    { "DcFollowSpline", 0x00A25E50, (uintptr_t)detour_DcFollowSpline },
    { "DcForGroup", 0x0054B830, (uintptr_t)detour_DcForGroup },
    { "DcCameraLocalSpace", 0x00A0EA20, (uintptr_t)detour_DcCameraLocalSpace },
    { "DcForceAimingCameraUltraTight", 0x00A03670, (uintptr_t)detour_DcForceAimingCameraUltraTight },
    { "DcForceNoShiftCameraZoomMode", 0x00A035E0, (uintptr_t)detour_DcForceNoShiftCameraZoomMode },
    { "DcForceStealthOff", 0x00A0BC40, (uintptr_t)detour_DcForceStealthOff },
    { "DcForgetSignal", 0x00A3FF40, (uintptr_t)detour_DcForgetSignal },
    { "DcFreezeBindingWhenNonPhysical", 0x00A03700, (uintptr_t)detour_DcFreezeBindingWhenNonPhysical },
    { "DcGetActionPackLocator", 0x00A060C0, (uintptr_t)detour_DcGetActionPackLocator },
    { "DcGetAlignLocator", 0x00A066F0, (uintptr_t)detour_DcGetAlignLocator },
    { "DcGetApReferenceEffectLocator", 0x00A07340, (uintptr_t)detour_DcGetApReferenceEffectLocator },
    { "DcGetApReferenceLocator", 0x00A07990, (uintptr_t)detour_DcGetApReferenceLocator },
    { "DcGetApReferenceLocatorRadial", 0x00A06C20, (uintptr_t)detour_DcGetApReferenceLocatorRadial },
    { "DcGetBoolean", 0x00A42BF0, (uintptr_t)detour_DcGetBoolean },
    { "DcGetCurrentTime", 0x00A44800, (uintptr_t)detour_DcGetCurrentTime },
    { "DcGetFloat", 0x00A43170, (uintptr_t)detour_DcGetFloat },
    { "DcGetInt32", 0x00A42EB0, (uintptr_t)detour_DcGetInt32 },
    { "DcGetJointAngleDiffFromBindpose", 0x00A06180, (uintptr_t)detour_DcGetJointAngleDiffFromBindpose },
    { "DcGetJointLocator", 0x00A07E80, (uintptr_t)detour_DcGetJointLocator },
    { "DcGetJointPosition", 0x00A120C0, (uintptr_t)detour_DcGetJointPosition },
    { "DcGetLocatorDistance", 0x00A0EF50, (uintptr_t)detour_DcGetLocatorDistance },
    { "DcGetObjectLocator", 0x00A08630, (uintptr_t)detour_DcGetObjectLocator },
    { "DcGetObjectLocatorRadial", 0x00A081A0, (uintptr_t)detour_DcGetObjectLocatorRadial },
    { "DcGetObjectOffsetLocator", 0x00A121E0, (uintptr_t)detour_DcGetObjectOffsetLocator },
    { "DcGetObjectOffsetPosition", 0x00A12390, (uintptr_t)detour_DcGetObjectOffsetPosition },
    { "DcGetObjectPosition", 0x00A12610, (uintptr_t)detour_DcGetObjectPosition },
    { "DcGetObjectState", 0x00A04C10, (uintptr_t)detour_DcGetObjectState },
    { "DcGetOriginLocator", 0x00A05F60, (uintptr_t)detour_DcGetOriginLocator },
    { "DcGetPlayerAimTime", 0x005DCBA0, (uintptr_t)detour_DcGetPlayerAimTime },
    { "DcGetQuaternionPitch", 0x009F37C0, (uintptr_t)detour_DcGetQuaternionPitch },
    { "DcGetScriptState", 0x00A44290, (uintptr_t)detour_DcGetScriptState },
    { "DcGetStatValue", 0x0097BD10, (uintptr_t)detour_DcGetStatValue },
    { "DcGetStateTime", 0x00A44960, (uintptr_t)detour_DcGetStateTime },
    { "DcGetSymbol", 0x00A43440, (uintptr_t)detour_DcGetSymbol },
    { "DcGrabRope", 0x00A04740, (uintptr_t)detour_DcGrabRope },
    { "DcGrenadeDirectorChangeSettings", 0x0069B910, (uintptr_t)detour_DcGrenadeDirectorChangeSettings },
    { "DcGroupAnd", 0x0054AFD0, (uintptr_t)detour_DcGroupAnd },
    { "DcGroupAt", 0x0054A6B0, (uintptr_t)detour_DcGroupAt },
    { "DcGroupCount", 0x00549810, (uintptr_t)detour_DcGroupCount },
    { "DcGroupIsMemberSymbolP", 0x0054A250, (uintptr_t)detour_DcGroupIsMemberSymbolP },
    { "DcGroupOr", 0x0054A760, (uintptr_t)detour_DcGroupOr },
    { "DcHeliAnimateGun", 0x00A7ECD0, (uintptr_t)detour_DcHeliAnimateGun },
    { "DcHeliFireMissile", 0x00A7F140, (uintptr_t)detour_DcHeliFireMissile },
    { "DcHeliSetBasePitch", 0x00A7EC00, (uintptr_t)detour_DcHeliSetBasePitch },
    { "DcHeliSetFiringState", 0x00A7F5C0, (uintptr_t)detour_DcHeliSetFiringState },
    { "DcHeliSetHover", 0x00A7ED80, (uintptr_t)detour_DcHeliSetHover },
    { "DcHeliSetMgStats", 0x00A7F6B0, (uintptr_t)detour_DcHeliSetMgStats },
    { "DcHeliSetTargetLocator", 0x00A7F2E0, (uintptr_t)detour_DcHeliSetTargetLocator },
    { "DcHeliSetTargetObject", 0x00A7F4A0, (uintptr_t)detour_DcHeliSetTargetObject },
    { "DcHideMessage", 0x005DCA40, (uintptr_t)detour_DcHideMessage },
    { "DcUnhideMessage", 0x005DCAF0, (uintptr_t)detour_DcUnhideMessage },
    { "DcUnhideMessageDeprecated", 0x005DC700, (uintptr_t)detour_DcUnhideMessageDeprecated },
    { "DcHideMessageDeprecated", 0x005DC650, (uintptr_t)detour_DcHideMessageDeprecated },
    { "DcInFrontOfTankP", 0x00AA3E10, (uintptr_t)detour_DcInFrontOfTankP },
    { "DcIncrementStatDifficulty", 0x0097C010, (uintptr_t)detour_DcIncrementStatDifficulty },
    { "DcIsCameraStickPressed", 0x00A0D610, (uintptr_t)detour_DcIsCameraStickPressed },
    { "DcIsOnScreen", 0x009FD200, (uintptr_t)detour_DcIsOnScreen },
    { "DcIsSpeaking", 0x00A013F0, (uintptr_t)detour_DcIsSpeaking },
    { "DcIsStickPressedTowards", 0x00A0D6B0, (uintptr_t)detour_DcIsStickPressedTowards },
    { "DcJournalDisable", 0x00A10510, (uintptr_t)detour_DcJournalDisable },
    { "DcKillDroppedWeaponsInSphere", 0x00A09B10, (uintptr_t)detour_DcKillDroppedWeaponsInSphere },
    { "DcLayerToScriptControllerIndices", 0x00EA2F7E, (uintptr_t)detour_DcLayerToScriptControllerIndices },
    { "DcLerpReflectionPlanesByRotation", 0x00A02A20, (uintptr_t)detour_DcLerpReflectionPlanesByRotation },
    { "DcMakeActiveCharacterMoreWet", 0x00A020E0, (uintptr_t)detour_DcMakeActiveCharacterMoreWet },
    { "DcMakeCharacterMoreWet", 0x00A023C0, (uintptr_t)detour_DcMakeCharacterMoreWet },
    { "DcMirrorAimingAt", 0x009FC990, (uintptr_t)detour_DcMirrorAimingAt },
    { "DcObjectInRegionP", 0x00A0FCA0, (uintptr_t)detour_DcObjectInRegionP },
    { "DcObjectOnObjectP", 0x00A04040, (uintptr_t)detour_DcObjectOnObjectP },
    { "DcOfferContextualEntity", 0x00A3FB60, (uintptr_t)detour_DcOfferContextualEntity },
    { "DcOfferContextualProcess", 0x00A3FA50, (uintptr_t)detour_DcOfferContextualProcess },
    { "DcPadGetGestureInterval", 0x00A0CEF0, (uintptr_t)detour_DcPadGetGestureInterval },
    { "DcPadGetInfo", 0x00A0D210, (uintptr_t)detour_DcPadGetInfo },
    { "DcPredictOnPhysicsDriven", 0x00A037C0, (uintptr_t)detour_DcPredictOnPhysicsDriven },
    { "DcPrintf", 0x00A10110, (uintptr_t)detour_DcPrintf },
    { "DcRemoveContextualEntity", 0x00A3F960, (uintptr_t)detour_DcRemoveContextualEntity },
    { "DcRenameObject", 0x00A04D10, (uintptr_t)detour_DcRenameObject },
    { "DcResetStat", 0x0097BE50, (uintptr_t)detour_DcResetStat },
    { "DcReverseLocator", 0x00A05CB0, (uintptr_t)detour_DcReverseLocator },
    { "DcRotateLocatorAngle", 0x00A05090, (uintptr_t)detour_DcRotateLocatorAngle },
    { "DcRotateLocatorLookat", 0x00A05620, (uintptr_t)detour_DcRotateLocatorLookat },
    { "DcSayPreRoll", 0x00A019F0, (uintptr_t)detour_DcSayPreRoll },
    { "DcSetBoolean", 0x00A42060, (uintptr_t)detour_DcSetBoolean },
    { "DcSetCharacterWetMask", 0x00A02550, (uintptr_t)detour_DcSetCharacterWetMask },
    { "DcSetCharacterWetMaskValue", 0x00A026E0, (uintptr_t)detour_DcSetCharacterWetMaskValue },
    { "DcSetClockMultiplier", 0x00A0EE60, (uintptr_t)detour_DcSetClockMultiplier },
    { "DcSetFloat", 0x00A42630, (uintptr_t)detour_DcSetFloat },
    { "DcSetFocusableIgnore", 0x00A0F3C0, (uintptr_t)detour_DcSetFocusableIgnore },
    { "DcSetIceCaveStupidGearsHack", 0x009FCC50, (uintptr_t)detour_DcSetIceCaveStupidGearsHack },
    { "DcSetInt32", 0x00A42350, (uintptr_t)detour_DcSetInt32 },
    { "DcSetRagdollFlow", 0x009FD5C0, (uintptr_t)detour_DcSetRagdollFlow },
    { "DcSetRenderSettings", 0x009FFF80, (uintptr_t)detour_DcSetRenderSettings },
    { "DcSetShaderInstanceParam", 0x00A107B0, (uintptr_t)detour_DcSetShaderInstanceParam },
    { "DcSetShaderMaterialParam", 0x00A10DD0, (uintptr_t)detour_DcSetShaderMaterialParam },
    { "DcSetSingleShaderInstanceParam", 0x00A10A50, (uintptr_t)detour_DcSetSingleShaderInstanceParam },
    { "DcSetSplineFollowAcceleration", 0x00A25CB0, (uintptr_t)detour_DcSetSplineFollowAcceleration },
    { "DcSetSplineFollowDistance", 0x00A25C00, (uintptr_t)detour_DcSetSplineFollowDistance },
    { "DcSetSplineFollowSpeed", 0x00A25D60, (uintptr_t)detour_DcSetSplineFollowSpeed },
    { "DcSetSymbol", 0x00A42910, (uintptr_t)detour_DcSetSymbol },
    { "DcSetTableShaderMaterialValuesParam", 0x009F3B70, (uintptr_t)detour_DcSetTableShaderMaterialValuesParam },
    { "DcShowBinoculars", 0x00A0CB90, (uintptr_t)detour_DcShowBinoculars },
    { "DcShutUp", 0x00A011E0, (uintptr_t)detour_DcShutUp },
    { "DcSignal", 0x00A40480, (uintptr_t)detour_DcSignal },
    { "DcSlaForceTackle", 0x00A0F4A0, (uintptr_t)detour_DcSlaForceTackle },
    { "DcBattleSelectBestSpawnerFromGroup", 0x00A11190, (uintptr_t)detour_DcBattleSelectBestSpawnerFromGroup },
    { "DcCameraAimOverridePoint", 0x00A0DF40, (uintptr_t)detour_DcCameraAimOverridePoint },
    { "DcCameraEnable", 0x00A0E680, (uintptr_t)detour_DcCameraEnable },
    { "DcCameraLocator", 0x00A128B0, (uintptr_t)detour_DcCameraLocator },
    { "DcEmitBulletFromSpawner", 0x00A0BF40, (uintptr_t)detour_DcEmitBulletFromSpawner },
    { "DcFmaRequestPartModule", 0x00A00050, (uintptr_t)detour_DcFmaRequestPartModule },
    { "DcGetSpawnerLocator", 0x00A09330, (uintptr_t)detour_DcGetSpawnerLocator },
    { "DcGetSpawnerLocatorProjected", 0x00A08D10, (uintptr_t)detour_DcGetSpawnerLocatorProjected },
    { "DcGetSpawnerLocatorRadial", 0x00A088C0, (uintptr_t)detour_DcGetSpawnerLocatorRadial },
    { "DcGetSpawnerPosition", 0x00A094B0, (uintptr_t)detour_DcGetSpawnerPosition },
    { "DcGroupSpawnedCount", 0x00549BF0, (uintptr_t)detour_DcGroupSpawnedCount },
    { "DcHeliMoveTo", 0x00A7FEE0, (uintptr_t)detour_DcHeliMoveTo },
    { "DcHeliMoveToSpline", 0x00A7FDD0, (uintptr_t)detour_DcHeliMoveToSpline },
    { "DcKillPartSpawner", 0x00A099B0, (uintptr_t)detour_DcKillPartSpawner },
    { "DcLevelsFullySpawnedIgnorePlayerP", 0x009FE400, (uintptr_t)detour_DcLevelsFullySpawnedIgnorePlayerP },
    { "DcLevelsFullySpawnedP", 0x009FE5E0, (uintptr_t)detour_DcLevelsFullySpawnedP },
    { "DcMakeVector", 0x009FDCF0, (uintptr_t)detour_DcMakeVector },
    { "DcMakeVector3", 0x009FDC20, (uintptr_t)detour_DcMakeVector3 },
    { "DcReloadModule", 0x00A19200, (uintptr_t)detour_DcReloadModule },
    { "DcReloadParticleModule", 0x00A19180, (uintptr_t)detour_DcReloadParticleModule },
    { "DcSelectBestSchemaSpawner", 0x00A10EB0, (uintptr_t)detour_DcSelectBestSchemaSpawner },
    { "DcSetCollisionLayer", 0x00A03CD0, (uintptr_t)detour_DcSetCollisionLayer },
    { "DcSetCurrentBySpawner", 0x009FD710, (uintptr_t)detour_DcSetCurrentBySpawner },
    { "DcSetDefaultFollowCameraSettings", 0x00A0EBF0, (uintptr_t)detour_DcSetDefaultFollowCameraSettings },
    { "DcSetObjectFaction", 0x009FCF40, (uintptr_t)detour_DcSetObjectFaction },
    { "DcSetOffBalanceBySpawner", 0x008E9160, (uintptr_t)detour_DcSetOffBalanceBySpawner },
    { "DcSpawnEntity", 0x00A09F40, (uintptr_t)detour_DcSpawnEntity },
    { "DcSpawnEntityAt", 0x009F3900, (uintptr_t)detour_DcSpawnEntityAt },
    { "DcSpawnExplosion", 0x009FF610, (uintptr_t)detour_DcSpawnExplosion },
    { "DcSpawnExplosionNpcOnly", 0x009FF410, (uintptr_t)detour_DcSpawnExplosionNpcOnly },
    { "DcSpawnFloatingBarrelAtPhase", 0x00A03E20, (uintptr_t)detour_DcSpawnFloatingBarrelAtPhase },
    { "DcSpawnMessage", 0x005DC7B0, (uintptr_t)detour_DcSpawnMessage },
    { "DcSpawnParticlesAtLocator", 0x00A002D0, (uintptr_t)detour_DcSpawnParticlesAtLocator },
    { "DcSpawnScript", 0x00A44460, (uintptr_t)detour_DcSpawnScript },
    { "DcSplineActivate", 0x00A26180, (uintptr_t)detour_DcSplineActivate },
    { "DcSplineDistanceLocator", 0x00A25F60, (uintptr_t)detour_DcSplineDistanceLocator },
    { "DcSplineDistanceSpawner", 0x00A260E0, (uintptr_t)detour_DcSplineDistanceSpawner },
    { "DcSplineDistanceVehicleAnimation", 0x00AB8640, (uintptr_t)detour_DcSplineDistanceVehicleAnimation },
    { "DcSplineLengthP", 0x00A256B0, (uintptr_t)detour_DcSplineLengthP },
    { "DcSplineLocatorAtDistance", 0x00EA2315, (uintptr_t)detour_DcSplineLocatorAtDistance },
    { "DcSsError", 0x00A44670, (uintptr_t)detour_DcSsError },
    { "DcStickySpawner", 0x00A0B8F0, (uintptr_t)detour_DcStickySpawner },
    { "DcStopSpeaking", 0x00A01570, (uintptr_t)detour_DcStopSpeaking },
    { "DcSwapEntity", 0x00A09590, (uintptr_t)detour_DcSwapEntity },
    { "DcTankAimAtObj", 0x00AA2AC0, (uintptr_t)detour_DcTankAimAtObj },
    { "DcTankAimAtPos", 0x00AA2E90, (uintptr_t)detour_DcTankAimAtPos },
    { "DcTankAimAtPosInstant", 0x00AA2B10, (uintptr_t)detour_DcTankAimAtPosInstant },
    { "DcTankAimForward", 0x00AA2A70, (uintptr_t)detour_DcTankAimForward },
    { "DcSpawnAttachable", 0x00A0F540, (uintptr_t)detour_DcSpawnAttachable },
    { "DcTankDistToPoint", 0x00AA1300, (uintptr_t)detour_DcTankDistToPoint },
    { "DcTankFindBestTargetMP", 0x00AA4950, (uintptr_t)detour_DcTankFindBestTargetMP },
    { "DcTankFire", 0x00AA2930, (uintptr_t)detour_DcTankFire },
    { "DcTankFireAtObj", 0x00AA2EE0, (uintptr_t)detour_DcTankFireAtObj },
    { "DcTankFireAtPos", 0x00AA2F30, (uintptr_t)detour_DcTankFireAtPos },
    { "DcTankForceFireCannon", 0x00AA42C0, (uintptr_t)detour_DcTankForceFireCannon },
    { "DcTankGetHealth", 0x00AA3D80, (uintptr_t)detour_DcTankGetHealth },
    { "DcTankMgCeaseFire", 0x00AA2000, (uintptr_t)detour_DcTankMgCeaseFire },
    { "DcTankMgFireAtLoc", 0x00AA2120, (uintptr_t)detour_DcTankMgFireAtLoc },
    { "DcTankMgFireAtObj", 0x00AA2680, (uintptr_t)detour_DcTankMgFireAtObj },
    { "DcTankMoveTo", 0x00AA33E0, (uintptr_t)detour_DcTankMoveTo },
    { "DcTankMoveTrackingObject", 0x00AA1550, (uintptr_t)detour_DcTankMoveTrackingObject },
    { "DcTankReverseTo", 0x00AA2F80, (uintptr_t)detour_DcTankReverseTo },
    { "DcTankSetHealth", 0x00AA3CC0, (uintptr_t)detour_DcTankSetHealth },
    { "DcTankTeleport", 0x00AA1BA0, (uintptr_t)detour_DcTankTeleport },
    { "DcTankWaitTeleport", 0x00AA4420, (uintptr_t)detour_DcTankWaitTeleport },
    { "DcTaskElapsedTime", 0x00A102B0, (uintptr_t)detour_DcTaskElapsedTime },
    { "DcTaskFailed", 0x009FE900, (uintptr_t)detour_DcTaskFailed },
    { "DcTaskFind", 0x009FEB90, (uintptr_t)detour_DcTaskFind },
    { "DcTaskNodeActiveP", 0x00A10340, (uintptr_t)detour_DcTaskNodeActiveP },
    { "DcTaskSubnodeComplete", 0x009FECA0, (uintptr_t)detour_DcTaskSubnodeComplete },
    { "DcTaskSubnodePlay", 0x009FEC00, (uintptr_t)detour_DcTaskSubnodePlay },
    { "DcTransferToSpline", 0x00A25A90, (uintptr_t)detour_DcTransferToSpline },
    { "DcTranslateYLoop", 0x009FC590, (uintptr_t)detour_DcTranslateYLoop },
    { "DcUnstickySpawner", 0x00A0B7C0, (uintptr_t)detour_DcUnstickySpawner },
    { "DcWait", 0x00A3E690, (uintptr_t)detour_DcWait },
    { "DcWaitBoolean", 0x00A41610, (uintptr_t)detour_DcWaitBoolean },
    { "DcWaitFor", 0x00A3FC50, (uintptr_t)detour_DcWaitFor },
    { "DcWaitFrames", 0x00A41270, (uintptr_t)detour_DcWaitFrames },
    { "DcWaitHeliFireMissile", 0x00A7EE30, (uintptr_t)detour_DcWaitHeliFireMissile },
    { "DcWaitHeliMoveTo", 0x00A7FA70, (uintptr_t)detour_DcWaitHeliMoveTo },
    { "DcWaitHeliMoveToSpline", 0x00A7F800, (uintptr_t)detour_DcWaitHeliMoveToSpline },
    { "DcWaitInt32", 0x00A41D40, (uintptr_t)detour_DcWaitInt32 },
    { "DcWaitOneFrame", 0x00A40F70, (uintptr_t)detour_DcWaitOneFrame },
    { "DcWaitPlayerMoveTo", 0x00A0B350, (uintptr_t)detour_DcWaitPlayerMoveTo },
    { "DcWaitPlayerTurnTo", 0x00A0AFC0, (uintptr_t)detour_DcWaitPlayerTurnTo },
    { "DcWaitSay", 0x00A016D0, (uintptr_t)detour_DcWaitSay },
    { "DcWaitSpawnEntity", 0x00A09E90, (uintptr_t)detour_DcWaitSpawnEntity },
    { "DcWaitSpawnEntityAt", 0x00A09DC0, (uintptr_t)detour_DcWaitSpawnEntityAt },
    { "DcWaitSplineDistance", 0x00A25740, (uintptr_t)detour_DcWaitSplineDistance },
    { "DcWaitTankAimAtObj", 0x00AA3A40, (uintptr_t)detour_DcWaitTankAimAtObj },
    { "DcWaitTankAimAtPos", 0x00AA3AA0, (uintptr_t)detour_DcWaitTankAimAtPos },
    { "DcWaitTankAimForward", 0x00AA3430, (uintptr_t)detour_DcWaitTankAimForward },
    { "DcWaitTankFireAtObj", 0x00AA3B00, (uintptr_t)detour_DcWaitTankFireAtObj },
    { "DcWaitTankFireAtPos", 0x00AA3B60, (uintptr_t)detour_DcWaitTankFireAtPos },
    { "DcWaitTankMoveTo", 0x00AA3C60, (uintptr_t)detour_DcWaitTankMoveTo },
    { "DcWaitTankReverseTo", 0x00AA3BC0, (uintptr_t)detour_DcWaitTankReverseTo },
    { "DcWaitUntil", 0x00A40E60, (uintptr_t)detour_DcWaitUntil },
    { "DcWaitUntilFrame", 0x00A40D40, (uintptr_t)detour_DcWaitUntilFrame },
    { "DcWaitUntilNetTime", 0x00A40870, (uintptr_t)detour_DcWaitUntilNetTime },
    { "DcRailActivateMover", 0x00AB8340, (uintptr_t)detour_DcRailActivateMover },
    { "DcRailAnimateToMover", 0x00AB8130, (uintptr_t)detour_DcRailAnimateToMover },
    { "DcRailDeactivate", 0x00AB97D0, (uintptr_t)detour_DcRailDeactivate },
    { "DcRailDisableCareen", 0x00AB8B80, (uintptr_t)detour_DcRailDisableCareen },
    { "DcRailEnableCollideWithObjects", 0x00AB8EA0, (uintptr_t)detour_DcRailEnableCollideWithObjects },
    { "DcRailEnableGroundHug", 0x00AB8430, (uintptr_t)detour_DcRailEnableGroundHug },
    { "DcRailEnableTributary", 0x00AB96B0, (uintptr_t)detour_DcRailEnableTributary },
    { "DcRailEnableTruckJump", 0x00AB9BB0, (uintptr_t)detour_DcRailEnableTruckJump },
    { "DcRailFireTurretGrenade", 0x00AB8CD0, (uintptr_t)detour_DcRailFireTurretGrenade },
    { "DcRailFollowMover", 0x00AB81E0, (uintptr_t)detour_DcRailFollowMover },
    { "DcRailForceCareen", 0x00AB8C40, (uintptr_t)detour_DcRailForceCareen },
    { "DcRailGetMoverLocator", 0x00AB7D00, (uintptr_t)detour_DcRailGetMoverLocator },
    { "DcRailGetSpline", 0x00AB84E0, (uintptr_t)detour_DcRailGetSpline },
    { "DcRailHasVehicle", 0x00AB94B0, (uintptr_t)detour_DcRailHasVehicle },
    { "DcRailOverrideSfxState", 0x00AB9070, (uintptr_t)detour_DcRailOverrideSfxState },
    { "DcRailScrub", 0x00AB8F90, (uintptr_t)detour_DcRailScrub },
    { "DcRailSetAcceleration", 0x00AB9590, (uintptr_t)detour_DcRailSetAcceleration },
    { "DcRailSetAllVehiclesToMoverSpeed", 0x00AB7BF0, (uintptr_t)detour_DcRailSetAllVehiclesToMoverSpeed },
    { "DcRailSetFollowDistance", 0x00AB8560, (uintptr_t)detour_DcRailSetFollowDistance },
    { "DcRailSetMoverFollowDistance", 0x00AB7FD0, (uintptr_t)detour_DcRailSetMoverFollowDistance },
    { "DcRailSetMoverIntialDistance", 0x00AB7E80, (uintptr_t)detour_DcRailSetMoverIntialDistance },
    { "DcRailSetMoverSpeed", 0x00AB80A0, (uintptr_t)detour_DcRailSetMoverSpeed },
    { "DcRailSetSpeed", 0x00AB9230, (uintptr_t)detour_DcRailSetSpeed },
    { "DcRailShutdown", 0x00AB9340, (uintptr_t)detour_DcRailShutdown },
    { "DcRailSpawn", 0x00AB9920, (uintptr_t)detour_DcRailSpawn },
    { "DcRailUseFixedAprefInIgc", 0x00AB9410, (uintptr_t)detour_DcRailUseFixedAprefInIgc },
    { "DcTrainAllowUnphysicalize", 0x00A68430, (uintptr_t)detour_DcTrainAllowUnphysicalize },
    { "DcTrainDecouple", 0x00A67E40, (uintptr_t)detour_DcTrainDecouple },
    { "DcTrainGetAcceleration", 0x00A68230, (uintptr_t)detour_DcTrainGetAcceleration },
    { "DcTrainGetDistance", 0x00A67B90, (uintptr_t)detour_DcTrainGetDistance },
    { "DcTrainGetHeadCar", 0x00A67C40, (uintptr_t)detour_DcTrainGetHeadCar },
    { "DcTrainGetSpline", 0x00A67AE0, (uintptr_t)detour_DcTrainGetSpline },
    { "DcTrainKillAllRiders", 0x00A68530, (uintptr_t)detour_DcTrainKillAllRiders },
    { "DcTrainSetAcceleration", 0x00A68140, (uintptr_t)detour_DcTrainSetAcceleration },
    { "DcTrainSetSpeedMph", 0x00A67F60, (uintptr_t)detour_DcTrainSetSpeedMph },
    { "DcTrainSpawn", 0x00A67960, (uintptr_t)detour_DcTrainSpawn },
    { "DcTrainSpawnOnTrack", 0x00A68740, (uintptr_t)detour_DcTrainSpawnOnTrack },
    { "DcTrainTeleport", 0x00A67CD0, (uintptr_t)detour_DcTrainTeleport },
    { "DcTrainTrackPlayer", 0x00A68300, (uintptr_t)detour_DcTrainTrackPlayer },
    { "DcTrainTransferPendingP", 0x00A685E0, (uintptr_t)detour_DcTrainTransferPendingP },
    { "DcAddLoadModule", 0x00A0FE90, (uintptr_t)detour_DcAddLoadModule },
    { "DcAddImportModule", 0x00A0FF10, (uintptr_t)detour_DcAddImportModule },
    { "DcKillAllAttachables", 0x009F2BF0, (uintptr_t)detour_DcKillAllAttachables },
    /* Phase 4 (Palier B): Pure C++ Engine Subsystems */
    { "CatmullRomSpline_GetName", 0x00468260, (uintptr_t)detour_CatmullRomSpline_GetName },
    { "CatmullRomSpline_IsLooped", 0x004684C0, (uintptr_t)detour_CatmullRomSpline_IsLooped },
    { "CatmullRomSpline_GetControlPointCount", 0x00468520, (uintptr_t)detour_CatmullRomSpline_GetControlPointCount },
    { "CatmullRomSpline_GetTotalArcLength", 0x00468D90, (uintptr_t)detour_CatmullRomSpline_GetTotalArcLength },
    { "CatmullRomSpline_GetCumulSegArcLength", 0x00469F60, (uintptr_t)detour_CatmullRomSpline_GetCumulSegArcLength },
    { "CatmullRomManager_OnSpawnGameObject", 0x004643E0, (uintptr_t)detour_CatmullRomManager_OnSpawnGameObject },
    { "CatmullRomManager_OnDestroyGameObject", 0x00464490, (uintptr_t)detour_CatmullRomManager_OnDestroyGameObject },
    { "CatmullRomManager_RemoveSpline", 0x00465240, (uintptr_t)detour_CatmullRomManager_RemoveSpline },
    { "CatmullRomManager_WalkSplines", 0x00466320, (uintptr_t)detour_CatmullRomManager_WalkSplines },
    { "SplineTracker_GetSpline0", 0x00A24040, (uintptr_t)detour_SplineTracker_GetSpline0 },
    { "SplineTracker_GetSpline1", 0x00A24080, (uintptr_t)detour_SplineTracker_GetSpline1 },
    { "SplineTracker_GetSplineNameId", 0x00A240C0, (uintptr_t)detour_SplineTracker_GetSplineNameId },
    { "SplineTracker_FinalizeDistance", 0x00A24110, (uintptr_t)detour_SplineTracker_FinalizeDistance },
    { "VehicleSplineTracker_GetCurrentVirtualDistance_m", 0x00ABA7B0, (uintptr_t)detour_VehicleSplineTracker_GetCurrentVirtualDistance_m },
    { "VehicleSplineTracker_GetCurrentRealDistance_m", 0x00ABA840, (uintptr_t)detour_VehicleSplineTracker_GetCurrentRealDistance_m },
    { "CameraZoom_OverideHorizontalAngularConstraints", 0x0044BF00, (uintptr_t)detour_CameraZoom_OverideHorizontalAngularConstraints },
    { "CameraZoom_ClearHorizontalAngularConstraints", 0x0044BFA0, (uintptr_t)detour_CameraZoom_ClearHorizontalAngularConstraints },
    { "CameraZoom_OverideVerticalAngularConstraints", 0x0044BFE0, (uintptr_t)detour_CameraZoom_OverideVerticalAngularConstraints },
    { "CameraZoom_ClearVerticalAngularConstraints", 0x0044C060, (uintptr_t)detour_CameraZoom_ClearVerticalAngularConstraints },
    { "CameraZoom_SetDefaultVerticalAngularConstraints", 0x0044C090, (uintptr_t)detour_CameraZoom_SetDefaultVerticalAngularConstraints },
    { "RailVehicle_InitBehavior", 0x000013D0, (uintptr_t)detour_RailVehicle_InitBehavior },
    { "RailVehicle_SetFollowDistance", 0x00001460, (uintptr_t)detour_RailVehicle_SetFollowDistance },
    { "RailVehicle_GetVehicleClass", 0x00001DD0, (uintptr_t)detour_RailVehicle_GetVehicleClass },
    { "RailVehicle_ResetWheels", 0x00001ED0, (uintptr_t)detour_RailVehicle_ResetWheels },
    { "RailVehicle_GetDamageable", 0x00002430, (uintptr_t)detour_RailVehicle_GetDamageable },
    { "RailVehicle_OnIgcBind", 0x00005630, (uintptr_t)detour_RailVehicle_OnIgcBind },
    { "RailVehicle_OnIgcUnbind", 0x000056E0, (uintptr_t)detour_RailVehicle_OnIgcUnbind },
    { "RailVehicleBehavior_GetDcBehavior", 0x00A8DE50, (uintptr_t)detour_RailVehicleBehavior_GetDcBehavior },
    { "RailVehicleBehaviorContainer_GetCurrentBehavior", 0x00A8EA30, (uintptr_t)detour_RailVehicleBehaviorContainer_GetCurrentBehavior },
    { "RailVehicleManager_GetMover", 0x00A8FCE0, (uintptr_t)detour_RailVehicleManager_GetMover },
    { "RailVehicleManager_DetachFromRail", 0x00A903E0, (uintptr_t)detour_RailVehicleManager_DetachFromRail },
};

size_t u2_get_scene_detours(const U2NativeDetour **out) {
    if (out) *out = s_scene_detours;
    return sizeof(s_scene_detours) / sizeof(s_scene_detours[0]);
}
