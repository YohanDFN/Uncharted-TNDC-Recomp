/* Uncharted 2: Among Thieves (big2-ps4_Shipping.elf)
 * Common definitions & shared helpers for native C runtime modules
 */
#ifndef RUNTIME_U2_COMMON_H
#define RUNTIME_U2_COMMON_H

#include "runtime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Global guest ELF image base address */
extern uint8_t *g_u2_image_base;

/* Detour descriptor */
typedef struct U2NativeDetour {
    const char *name;
    uintptr_t   offset;
    uintptr_t   host_fn;
} U2NativeDetour;

/* Native DC function signature */
typedef int64_t (ABI *U2DcNativeFn)(int32_t argc, uint64_t *argv, uint32_t *out_status);

/* Safe status write helper:
 * ExecuteScriptCode (0x00A14627) and EvalEx (0x00A1578C) pre-initialize *status = 0
 * before calling any native function, and original ELF routines only write to (%rdx)
 * when signalling non-zero status (1 = abort, 2 = yield). Never write val == 0 to %rdx,
 * as functions called directly from C++ (e.g. 0x009F2BF0) may hold a heap pointer in %rdx.
 */
static inline void u2_set_status(uint32_t *status, uint32_t val) {
    if (val == 0) return;
    uintptr_t p = (uintptr_t)status;
    if (p >= 0x10000ull && p < 0x0000800000000000ull && (p & 3) == 0) {
        *status = val;
    }
}

/* Float <-> Register conversion helpers */
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

static inline double u2_reg_to_f64(uint64_t r) {
    double d;
    memcpy(&d, &r, sizeof(d));
    return d;
}

static inline uint64_t u2_f64_to_reg(double d) {
    uint64_t u;
    memcpy(&u, &d, sizeof(u));
    return u;
}

/* Helper: Resolve active player object from global handle at image + 0x0115BD20 */
static inline uint8_t *u2_get_active_player(void) {
    if (!g_u2_image_base) return NULL;
    uint8_t *handle = g_u2_image_base + 0x0115BD20ull;
    uint8_t **slot = *(uint8_t ***)(handle + 0x00);
    if (!slot) return NULL;
    uint32_t id = *(const uint32_t *)(handle + 0x08);
    if (!id) return NULL;
    uint8_t *player = *slot;
    if (!player || *(const uint32_t *)(player + 0x44) != id) return NULL;
    return player;
}

/* Module detour table accessors */
size_t u2_get_player_detours(const U2NativeDetour **out);
size_t u2_get_npc_detours(const U2NativeDetour **out);
size_t u2_get_audio_detours(const U2NativeDetour **out);
size_t u2_get_event_detours(const U2NativeDetour **out);
size_t u2_get_scene_detours(const U2NativeDetour **out);
size_t u2_get_engine_detours(const U2NativeDetour **out);
int    u2_run_engine_unit_tests(uint8_t *image);

#ifdef __cplusplus
}
#endif

#endif /* RUNTIME_U2_COMMON_H */
