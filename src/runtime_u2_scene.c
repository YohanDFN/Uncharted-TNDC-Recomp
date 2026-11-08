/* Uncharted 2: Scene Actors, Entity Queries, Shipping Stubs & Utilities */
#include "runtime_u2_common.h"

static int64_t ABI detour_DcObjectSpawnedP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A12780: (object-spawned? [name-id]) */
    (void)status;
    if (status) *status = 0;
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
    /* 0x00A12830: (entity-auto-spawn-once) */
    if (status) *status = 0;
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
    /* 0x00A00970: (part-module-loaded? [symbol-id]) */
    if (status) *status = 0;
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
    /* 0x00A038A0: (disable-frame-forward [obj-id]) */
    if (status) *status = 0;
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
    /* 0x00A03940: (unparent [obj-id]) */
    if (status) *status = 0;
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
    /* 0x00A039C0: (enable-gadgetry [obj-id]) */
    if (status) *status = 0;
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
    /* 0x00A03A70: (disable-gadgetry [obj-id]) */
    if (status) *status = 0;
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

static int64_t ABI detour_DcCameraAbandon(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E5E0: (camera-abandon [name-or-id]) */
    if (status) *status = 0;
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
    /* 0x00A0E500: (camera-disable [name-or-id]) */
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    /* 0x00A25A20: (spline-transfer-pending? [ent]) */
    if (status) *status = 0;
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

static int64_t ABI detour_DcGroundPrintsType(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A01F00: (ground-prints-type ...) */
    (void)argv;
    if (status) *status = 0;
    return (argc > 0) ? 1 : 0;
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

static int64_t ABI detour_DcGetDda(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD010: (get-dda) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    typedef float (ABI *GetDdaFn)(void);
    GetDdaFn get_dda = (GetDdaFn)(uintptr_t)(g_u2_image_base + 0x004ABA00ull);
    float dda = get_dda ? get_dda() : 0.0f;
    return (int64_t)u2_f32_to_reg(dda);
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

static int64_t ABI detour_DcDespawnMessage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x005DC8C0: (despawn-message id) */
    (void)status;
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetEndWave(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006687E0: (net-end-wave) */
    (void)argc; (void)argv;
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    /* 0x00668810: (net-get-booster-enabled booster-id) */
    (void)status;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t booster_id = (argc > 0 && argv) ? (uint32_t)argv[0] : 0u;
    typedef uint8_t (ABI *NetIsBoosterEnabledFn)(uint64_t dummy, uint32_t id);
    NetIsBoosterEnabledFn is_enabled = (NetIsBoosterEnabledFn)(uintptr_t)(g_u2_image_base + 0x005F6080ull);
    return is_enabled ? (int64_t)is_enabled(0, booster_id) : 0;
}

static int64_t ABI detour_DcNetGetBoosterAmount(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00668820: (net-get-booster-amount booster-id) */
    (void)status;
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcSetForceTension(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00667EE0: (set-force-tension flag) */
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    /* 0x006698E0: (net-respawn-flag [flag-name]) */
    if (status) *status = 0;
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
    /* 0x00669C20: (net-coop-team-failed?) */
    (void)argc; (void)argv; (void)status;
    return *(const uint8_t *)(g_u2_image_base + 0x0113D910ull);
}

static int64_t ABI detour_DcNetCoopScriptId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C30: (net-coop-script-id) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetCoopHostScriptId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C40: (net-coop-host-script-id) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetIsCinema(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C50: (net-is-cinema) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    int32_t net_mode = *(const int32_t *)(g_u2_image_base + 0x014CE97Cull);
    if (net_mode < 2) return 0;
    return (*(const uint8_t *)(g_u2_image_base + 0x0113B760ull + 0x209C) != 0) ? 1 : 0;
}

static int64_t ABI detour_DcNetIsHost(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00669C80: (net-is-host) */
    (void)argc; (void)argv;
    if (status) *status = 0;
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
    /* 0x009FD590: (psn-demo-show-upsell) */
    (void)argc; (void)argv; (void)status;
    *(uint8_t *)(g_u2_image_base + 0x0113769Dull) = 1u;
    return 0;
}

static int64_t ABI detour_DcDebugDrawCross(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE070: (debug-draw-cross ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcAutoSaveGame(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE820: (auto-save-game) */
    (void)argc; (void)argv;
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    /* 0x009FE8F0: (game-task-graph-login task) */
    (void)status;
    if (status) *status = 0;
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
    /* 0x009FEDF0: (remove-dynamic-light id) */
    (void)status;
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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

static int64_t ABI detour_DcRenderSettingsApplyDefaults(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10760: (render-settings-apply-defaults [id]) */
    if (status) *status = 0;
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
    /* 0x00A04A20: (set-object-health [ent] [f_health]) */
    if (status) *status = 0;
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
    /* 0x00A04B10: (get-object-health [ent]) */
    if (status) *status = 0;
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
    /* 0x00A00950: (spawn-particles-at-joint ...) */
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, int32_t, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x009F2E80ull);
    return fn(argc, argv, 2, status);
}

static int64_t ABI detour_DcSpawnParticlesAttached(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A00960: (spawn-particles-attached ...) */
    typedef int64_t (ABI *fn_t)(int32_t, uint64_t *, int32_t, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x009F2E80ull);
    return fn(argc, argv, 1, status);
}

static int64_t ABI detour_DcNetHideWaveScreen(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00668850: (net-hide-wave-screen) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    if (*(const int32_t *)(g_u2_image_base + 0x014CE97Cull) > 1) {
        *(uint8_t *)(g_u2_image_base + 0x0104F544ull + 0x1C) = 0;
        return 1;
    }
    return 0;
}

static int64_t ABI detour_DcStringFromFloat(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDE60: (string-from-float f) */
    if (status) *status = 0;
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
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcPlayMovie(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FE100: (play-movie movie-name) */
    if (status) *status = 0;
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
    /* 0x009FF3D0: (start-rumble [symbol]) */
    if (status) *status = 0;
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
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcNetAllPlayersDeadP(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x006694B0: (net-all-players-dead?) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    uint32_t out_val = 0;
    typedef uint8_t (ABI *fn_t)(void *, uint32_t *);
    fn_t fn = (fn_t)(uintptr_t)(g_u2_image_base + 0x0063E040ull);
    return fn ? (fn((void *)(g_u2_image_base + 0x0113B760ull), &out_val) ? 1 : 0) : 0;
}

static int64_t ABI detour_DcDebugDrawString(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FDFB0: (debug-draw-string ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcSetCharacterWetMaskState(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02690: (set-character-wet-mask-state idx state) */
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    /* 0x00A09D60: (kill-entity [ent-id]) */
    if (status) *status = 0;
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

static int64_t ABI detour_DcHideHint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0CC10: (hide-hint) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    *(uint8_t *)(g_u2_image_base + 0x0104F1B8ull) = 0;
    return 0;
}

static int64_t ABI detour_DcGetObjectId(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0BEE0: (get-object-id [obj]) */
    if (status) *status = 0;
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

static int64_t ABI detour_DcSetWhiteFade(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A106A0: (set-white-fade [fade-amount]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float fade = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *p_fade = (float *)(uintptr_t)(g_u2_image_base + 0x01150498ull);
    if (p_fade) *p_fade = fade;
    return 0;
}

static int64_t ABI detour_DcSetBlackFade(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A10700: (set-black-fade [fade-amount]) */
    if (status) *status = 0;
    if (!g_u2_image_base) return 0;
    float fade = (argc > 0 && argv) ? *(const float *)&argv[0] : 0.0f;
    float *p_fade = (float *)(uintptr_t)(g_u2_image_base + 0x01150494ull);
    if (p_fade) *p_fade = fade;
    return 0;
}

static int64_t ABI detour_DcMakeInert(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD850: (make-inert entity) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcDisablePlatformControlOnObject(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04E80: (disable-platform-control-on-object [obj-id]) */
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
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
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcLerpReflectionPlanes(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A02AA0: (lerp-reflection-planes p1 p2 t) */
    if (status) *status = 0;
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
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcRailSetNpcArchetype(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00AB9660: (rail-set-npc-archetype [rail] [archetype]) */
    if (status) *status = 0;
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
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcAddDynamicLight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FF2C0: (add-dynamic-light ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcAddDynamicLightJoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FF010: (add-dynamic-light-joint light entity joint) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcApplyExplosionImpulse(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03B20: (apply-explosion-impulse pos force radius) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcAttachFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0A4C0: (attach-flashlight [entity]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
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

static int64_t ABI detour_DcCameraShake(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0E040: (camera-shake type intensity [duration]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcConstrainRopeAtJoint(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04430: (constrain-rope-at-joint rope joint) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcCreateWeaponDrawables(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A0C410: (create-weapon-drawables [entity]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcDetachRope(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04530: (detach-rope rope) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcDropRope(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A045E0: (drop-rope rope) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 0;
}

static int64_t ABI detour_DcDoRumblePtr(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A105F0: (do-rumble-ptr [motor] [intensity] [duration]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcJournalSetPage(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FD130: (journal-set-page page) */
    if (status) *status = 0;
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
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcLetterboxOn(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x009FFE00: (letterbox-on [time]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcRopeEnableCharacterCollision(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04270: (rope-enable-character-collision rope) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcRopeDisableCharacterCollision(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A04340: (rope-disable-character-collision rope) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcUseRainIntensityValues(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A031C0: (use-rain-intensity-values ...) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcUseWindIntensityValues(int32_t argc, uint64_t *argv, uint32_t *status) {
    /* 0x00A03290: (use-wind-intensity-values ...) */
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

/* Table of detours provided by this module */
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

static int64_t ABI detour_DcDisplayButtonHud(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0CD20: (display-button-hud [btn-id] [action-str]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcFadeToBlack(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FFBA0: (fade-to-black [time] [hold]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcFadeToBlackPers(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FFA40: (fade-to-black-pers [time]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcFadeToWhite(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FFAC0: (fade-to-white [time] [hold]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
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

static int64_t ABI detour_DcGivePlayerFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0ADC0: (give-player-flashlight [enable] [slot]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
}

static int64_t ABI detour_DcGiveSullivanFlashlight(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A0ABC0: (give-sullivan-flashlight [enable] [slot]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
    return 1;
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

static int64_t ABI detour_DcHeliEnableWind(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A7EB60: (heli-enable-wind [enabled-bool]) */
    if (status) *status = 0;
    (void)argc;
    (void)argv;
    return 1;
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

static int64_t ABI detour_DcScanParticleDirectories(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A19360: (scan-particle-directories) */
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

static int64_t ABI detour_DcSetDynamicLightColor(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x009FEE20: (set-dynamic-light-color [light-id] [color-vec]) */
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

static int64_t ABI detour_DcSetRainState(int32_t argc, uint64_t *argv, uint32_t *status) {
/* 0x00A02830: (set-rain-state [state]) */
    (void)argc; (void)argv;
    if (status) *status = 0;
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
};

size_t u2_get_scene_detours(const U2NativeDetour **out) {
    if (out) *out = s_scene_detours;
    return sizeof(s_scene_detours) / sizeof(s_scene_detours[0]);
}
