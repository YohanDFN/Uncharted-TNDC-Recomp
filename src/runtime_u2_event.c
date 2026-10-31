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
};

size_t u2_get_event_detours(const U2NativeDetour **out) {
    if (out) *out = s_event_detours;
    return sizeof(s_event_detours) / sizeof(s_event_detours[0]);
}
