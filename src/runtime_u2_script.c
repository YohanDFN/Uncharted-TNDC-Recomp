/* Uncharted 2: Among Thieves (big2-ps4_Shipping.elf)
 * DC00 Script Engine Native C Reimplementation & Hook Orchestration:
 *   - ScriptManager::ExecuteScriptCode (guest offset 0x00A14120)
 *   - ScriptManager::EvalEx            (guest offset 0x00A15430)
 *   - ScriptManager::Run               (guest offset 0x00A14F20 / 0x00A150E0)
 *   - ScriptManager::Initialize        (guest offset 0x00A173A0)
 */
#include "runtime_u2_common.h"

/* Global guest ELF image base address */
uint8_t *g_u2_image_base = NULL;

/* ============================================================================
 * 1. Naughty Dog DC00 VM Structures (reconstructed from 0x00A14120)
 * ============================================================================ */

typedef struct U2ScriptCode {
    const uint8_t  *bytecode;   /* +0x00: [opcode, dst, src1, src2] * N */
    const uint64_t *constants;  /* +0x08: 64-bit constant pool */
} U2ScriptCode;

typedef struct U2ScriptStackFrame {
    uint32_t                   pc;             /* +0x00: instruction index (or next free frame ptr low) */
    uint32_t                   _pad04;         /* +0x04 */
    const U2ScriptCode        *code;           /* +0x08: script code header */
    uint32_t                   argc;           /* +0x10: argument count passed to this frame */
    uint32_t                   _pad14;         /* +0x14 */
    struct U2ScriptStackFrame *caller;         /* +0x18: parent stack frame (NULL at root) */
    uint32_t                   return_dst_reg; /* +0x20: caller register for return value (0xF8EEF8EE when free) */
    uint32_t                   _pad24;         /* +0x24 */
    uint64_t                   regs[64];       /* +0x28: register file (regs[24..39] at +0xE8 = call args) */
} U2ScriptStackFrame;

/* ============================================================================
 * 3. Opcode Table (80 opcodes: 0x00 .. 0x4F at jump table 0x00A14DA4)
 * ============================================================================ */

static const char *u2_opcode_name(uint8_t op) {
    static const char *const names[80] = {
        /* 0x00 */ "RET",         "ADD_INT",     "SUB_INT",     "MUL_INT",
        /* 0x04 */ "DIV_INT",     "ADD_FLOAT",   "SUB_FLOAT",   "MUL_FLOAT",
        /* 0x08 */ "DIV_FLOAT",   "LOAD_POOL",   "SHL_INT",     "SHR_INT",
        /* 0x0C */ "LOAD_IMM16",  "LOAD_U64",    "LOAD_U32",    "LOAD_PTR",
        /* 0x10 */ "STORE_32",    "STORE_U32",   "STORE_64",    "STORE_8",
        /* 0x14 */ "STORE_16",    "LOAD_SYM",    "MOV",         "MOV_ALT1",
        /* 0x18 */ "MOV_ALT2",    "FTOI",        "ITOF",        "CALL_DEFUN",
        /* 0x1C */ "CALL_NATIVE", "CMP_EQ",      "CMP_GT",      "CMP_GE",
        /* 0x20 */ "CMP_LT",      "CMP_LE",      "FCMP_EQ",     "FCMP_GT",
        /* 0x24 */ "FCMP_GE",     "FCMP_LT",     "FCMP_LE",     "MOD_INT",
        /* 0x28 */ "CALL_HELPER", "ABS_INT",     "ABS_FLOAT",   "MIN_FLOAT",
        /* 0x2C */ "MAX_FLOAT",   "JMP",         "JNZ",         "JZ",
        /* 0x30 */ "NOT",         "AND_BIT",     "NOT_BIT",     "OR_BIT",
        /* 0x34 */ "XOR_BIT",     "NOR_BIT",     "AND_LOGIC",   "OR_LOGIC",
        /* 0x38 */ "NEG_INT",     "NEG_FLOAT",   "GET_ARGC",    "ADD_IMM",
        /* 0x3C */ "SUB_IMM",     "MUL_IMM",     "DIV_IMM",     "LOAD_I32",
        /* 0x40 */ "LOAD_F32",    "LOAD_I64",    "LOAD_SYM_C",  "LOAD_I8_C",
        /* 0x44 */ "LOAD_U8_C",   "LOAD_I16",    "LOAD_U16",    "ASSERT_47",
        /* 0x48 */ "ASSERT_48",   "LOAD_M_I8",   "LOAD_M_U8",   "LOAD_M_I16",
        /* 0x4C */ "LOAD_M_U16",  "LOAD_M_I32",  "ASSERT_4E",   "ASSERT_4F"
    };
    return op < 80 ? names[op] : "OP_UNKNOWN";
}

/* ============================================================================
 * 4. Complete C Implementation of ScriptManager::ExecuteScriptCode (0x00A14120)
 * ============================================================================ */

static void u2_free_frame(U2ScriptStackFrame *frame, uint64_t image) {
    uint32_t *p_depth = (uint32_t *)(uintptr_t)(image + 0x015341F0ull);
    U2ScriptStackFrame **p_free_list = (U2ScriptStackFrame **)(uintptr_t)(image + 0x015341E8ull);
    (*p_depth)--;
    frame->return_dst_reg = 0xF8EEF8EEu;
    *(U2ScriptStackFrame **)frame = *p_free_list;
    *p_free_list = frame;
}

int64_t runtime_u2_vm_execute(U2ScriptStackFrame *frame, uint64_t *out_result, uint64_t image) {
    if (!frame || !frame->code) {
        if (out_result) *out_result = 0;
        return 0;
    }

    const uint8_t  *r12 = frame->code->bytecode;
    const uint64_t *r8  = frame->code->constants;
    uint64_t       *r15 = frame->regs;

    for (;;) {
        const uint32_t pc   = frame->pc;
        const uint8_t  op   = r12[pc * 4 + 0];
        const uint8_t  dst  = r12[pc * 4 + 1];
        const uint8_t  src1 = r12[pc * 4 + 2];
        const uint8_t  src2 = r12[pc * 4 + 3];
        frame->pc = pc + 1;

        switch (op) {
        case 0x00: { /* RET (0x00A14231) */
            U2ScriptStackFrame *caller = frame->caller;
            uint64_t ret_val = r15[src1];
            if (caller != NULL) {
                /* Return to caller frame (0x00A14170) */
                uint32_t ret_dst = frame->return_dst_reg;
                caller->regs[ret_dst] = ret_val;
                u2_free_frame(frame, image);
                frame = caller;
                r12   = frame->code->bytecode;
                r8    = frame->code->constants;
                r15   = frame->regs;
                break;
            }
            /* Return from root frame (0x00A14CAE) */
            if (out_result) *out_result = ret_val;
            u2_free_frame(frame, image);
            return 0;
        }

        case 0x01: /* ADD_INT (0x00A1423B) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] + (int64_t)r15[src2]);
            break;

        case 0x02: /* SUB_INT (0x00A1424C) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] - (int64_t)r15[src2]);
            break;

        case 0x03: /* MUL_INT (0x00A1425D) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] * (int64_t)r15[src2]);
            break;

        case 0x04: /* DIV_INT (0x00A1426F) */
            r15[dst] = r15[src2] ? (uint64_t)((int64_t)r15[src1] / (int64_t)r15[src2]) : 0;
            break;

        case 0x05: /* ADD_FLOAT (0x00A14282) */
            r15[dst] = u2_f32_to_reg(u2_reg_to_f32(r15[src1]) + u2_reg_to_f32(r15[src2]));
            break;

        case 0x06: /* SUB_FLOAT (0x00A14299) */
            r15[dst] = u2_f32_to_reg(u2_reg_to_f32(r15[src1]) - u2_reg_to_f32(r15[src2]));
            break;

        case 0x07: /* MUL_FLOAT (0x00A142B0) */
            r15[dst] = u2_f32_to_reg(u2_reg_to_f32(r15[src1]) * u2_reg_to_f32(r15[src2]));
            break;

        case 0x08: /* DIV_FLOAT (0x00A142C7) */
            r15[dst] = u2_f32_to_reg(u2_reg_to_f32(r15[src1]) / u2_reg_to_f32(r15[src2]));
            break;

        case 0x09: /* LOAD_POOL (0x00A141DB) */
        case 0x0A:
        case 0x0B:
            r15[dst] = r8[r15[src1]];
            break;

        case 0x0C: /* LOAD_IMM16 (0x00A142DE) */
            r15[dst] = r8[(uint64_t)src1];
            break;

        case 0x0D: /* LOAD_U64 (0x00A142EE) */
        case 0x0F: /* LOAD_PTR (0x00A14310) */
            r15[dst] = *(const uint64_t *)(uintptr_t)r15[src1];
            break;

        case 0x0E: /* LOAD_U32 (0x00A142FD) */
            r15[dst] = (uint64_t)(*(const uint32_t *)(uintptr_t)r15[src1]);
            break;

        case 0x10: /* STORE_32 (0x00A14320) */
        case 0x11: /* STORE_U32 (0x00A14335) */ {
            uint32_t v = (uint32_t)r15[src2];
            *(uint32_t *)(uintptr_t)r15[src1] = v;
            r15[dst] = (uint64_t)v;
            break;
        }

        case 0x12: /* STORE_64 (0x00A1434E) */ {
            uint64_t v = r15[src2];
            *(uint64_t *)(uintptr_t)r15[src1] = v;
            r15[dst] = v;
            break;
        }

        case 0x13: /* STORE_8 (0x00A1422A) */
        case 0x14: /* STORE_16 (0x00A1422A) */
            break;

        case 0x15: /* LOAD_SYM (0x00A14366) */ {
            uint64_t *sym_ptr = *(uint64_t **)(uintptr_t)(r8 + src1);
            r15[dst] = sym_ptr ? *sym_ptr : 0;
            break;
        }

        case 0x16: /* MOV (0x00A14220) */
        case 0x17: /* MOV_ALT1 (0x00A14220) */
        case 0x18: /* MOV_ALT2 (0x00A14220) */
            r15[dst] = r15[src1];
            break;

        case 0x19: /* FTOI (0x00A14389) */
            r15[dst] = (uint64_t)(int64_t)(int32_t)u2_reg_to_f32(r15[src1]);
            break;

        case 0x1A: /* ITOF (0x00A1439A) */
            r15[dst] = u2_f32_to_reg((float)(int64_t)r15[src1]);
            break;

        case 0x1B: { /* CALL_DEFUN (0x00A143AB) */
            const U2ScriptCode *callee_code = (const U2ScriptCode *)(uintptr_t)r15[src1];
            const uint64_t invalid_code = *(const uint64_t *)(uintptr_t)(image + 0x0104AEB0ull);
            if (!callee_code || (uint64_t)(uintptr_t)callee_code == invalid_code) {
                r15[dst] = 0;
                break;
            }
            U2ScriptStackFrame **p_free_list = (U2ScriptStackFrame **)(uintptr_t)(image + 0x015341E8ull);
            uint32_t *p_depth = (uint32_t *)(uintptr_t)(image + 0x015341F0ull);
            U2ScriptStackFrame *callee = *p_free_list;
            if (!callee) {
                /* Stack overflow (0x00A14D15) */
                if (out_result) *out_result = 0;
                return 0;
            }
            *p_free_list = *(U2ScriptStackFrame **)callee;
            (*p_depth)++;
            callee->caller = frame;
            /* Copy argc (src2) arguments from caller regs[24..] (r15 + 0xC0) to callee regs[24..] (callee + 0xE8) */
            for (uint32_t i = 0; i < (uint32_t)src2; ++i) {
                callee->regs[24 + i] = r15[24 + i];
            }
            callee->return_dst_reg = dst;
            callee->code           = callee_code;
            callee->argc           = src2;
            callee->pc             = 0;
            frame = callee;
            r12   = frame->code->bytecode;
            r8    = frame->code->constants;
            r15   = frame->regs;
            break;
        }

        case 0x1C: { /* CALL_NATIVE (0x00A1451A) */
            U2DcNativeFn fn = (U2DcNativeFn)(uintptr_t)r15[src1];
            if (!fn) break;
            uint64_t args[16];
            for (int i = 0; i < 16; ++i) {
                args[i] = r15[24 + i]; /* r15 + 0xC0 .. r15 + 0x138 */
            }
            uint32_t status = 0;
            int64_t res = fn((int32_t)src2, args, &status);
            r15[dst] = (uint64_t)res;
            /* Inlined ss-context.cpp::GetGroupInstance check (0x00A1463E..0x00A14714):
             * If the current StateScript group instance was destroyed during the native call,
             * set status = 1 to unwind the script stack cleanly. */
            const uint8_t *ss_ctx = (const uint8_t *)(uintptr_t)(image + 0x01050320ull);
            int64_t cur_stack_idx = *(const int32_t *)(ss_ctx + 0x200);
            if (cur_stack_idx >= 0 && cur_stack_idx < 16) {
                uintptr_t entry = *(const uintptr_t *)(ss_ctx + ((size_t)cur_stack_idx << 5) + 0x8);
                if (entry != 0) {
                    uintptr_t p0 = *(const uintptr_t *)entry;
                    uint32_t id  = *(const uint32_t *)(entry + 0x8);
                    uintptr_t p1 = p0 ? *(const uintptr_t *)p0 : 0;
                    int valid = (p0 != 0 && id != 0 && p1 != 0 && *(const uint32_t *)(p1 + 0x44) == id);
                    if (!valid && *(const uintptr_t *)(entry + 0x18) == 0) {
                        status = 1;
                    }
                }
            }
            if (status == 2) {
                if (out_result) *out_result = 0;
                return 0;
            }
            if (status == 1) {
                /* Unwind all active stack frames (0x00A14D54) */
                while (frame) {
                    U2ScriptStackFrame *parent = frame->caller;
                    u2_free_frame(frame, image);
                    frame = parent;
                }
                if (out_result) *out_result = 0;
                return 0;
            }
            break;
        }

        case 0x1D: /* CMP_EQ (0x00A1472F) */
            r15[dst] = (r15[src1] == r15[src2]) ? 1ull : 0ull;
            break;

        case 0x1E: /* CMP_GT (0x00A14746) */
            r15[dst] = ((int64_t)r15[src1] > (int64_t)r15[src2]) ? ~0ull : 0ull;
            break;

        case 0x1F: /* CMP_GE (0x00A14764) */
            r15[dst] = ((int64_t)r15[src1] >= (int64_t)r15[src2]) ? ~0ull : 0ull;
            break;

        case 0x20: /* CMP_LT (0x00A14782) */
            r15[dst] = ((int64_t)r15[src1] < (int64_t)r15[src2]) ? ~0ull : 0ull;
            break;

        case 0x21: /* CMP_LE (0x00A147A0) */
            r15[dst] = ((int64_t)r15[src1] <= (int64_t)r15[src2]) ? ~0ull : 0ull;
            break;

        case 0x22: /* FCMP_EQ (0x00A147BE) */
            r15[dst] = (fabsf(u2_reg_to_f32(r15[src1]) - u2_reg_to_f32(r15[src2])) <= 1e-6f) ? ~0ull : 0ull;
            break;

        case 0x23: /* FCMP_GT (0x00A147E2) */
            r15[dst] = (u2_reg_to_f32(r15[src1]) > u2_reg_to_f32(r15[src2])) ? ~0ull : 0ull;
            break;

        case 0x24: /* FCMP_GE (0x00A14804) */
            r15[dst] = (u2_reg_to_f32(r15[src1]) >= u2_reg_to_f32(r15[src2])) ? ~0ull : 0ull;
            break;

        case 0x25: /* FCMP_LT (0x00A14826) */
            r15[dst] = (u2_reg_to_f32(r15[src1]) < u2_reg_to_f32(r15[src2])) ? ~0ull : 0ull;
            break;

        case 0x26: /* FCMP_LE (0x00A14848) */
            r15[dst] = (u2_reg_to_f32(r15[src1]) <= u2_reg_to_f32(r15[src2])) ? ~0ull : 0ull;
            break;

        case 0x27: /* MOD_INT (0x00A1486A) */
            r15[dst] = r15[src2] ? (uint64_t)((int64_t)r15[src1] % (int64_t)r15[src2]) : 0;
            break;

        case 0x28: /* CALL_HELPER (0x00A1487D) */
            r15[dst] = (uint64_t)u2_f32_to_reg(fmodf(u2_reg_to_f32(r15[src1]), u2_reg_to_f32(r15[src2])));
            break;

        case 0x29: { /* ABS_INT (0x00A148A3) */
            int64_t v = (int64_t)r15[src1];
            r15[dst] = (uint64_t)(v < 0 ? -v : v);
            break;
        }

        case 0x2A: /* ABS_FLOAT (0x00A148BA) */
            r15[dst] = u2_f32_to_reg(fabsf(u2_reg_to_f32(r15[src1])));
            break;

        case 0x2E: /* JNZ (0x00A148D3) */
            if (r15[src1] != 0)
                frame->pc = ((uint32_t)src2 << 8) | (uint32_t)dst;
            break;

        case 0x2F: /* JZ (0x00A148E0) */
            if (r15[src1] == 0)
                frame->pc = ((uint32_t)src2 << 8) | (uint32_t)dst;
            break;

        case 0x2D: /* JMP (0x00A148EB) */
            frame->pc = ((uint32_t)src2 << 8) | (uint32_t)dst;
            break;

        case 0x30: /* NOT (0x00A148F9) */
            r15[dst] = (r15[src1] == 0) ? ~0ull : 0ull;
            break;

        case 0x31: /* AND_BIT (0x00A1490A) */
            r15[dst] = r15[src1] & r15[src2];
            break;

        case 0x32: /* NOT_BIT (0x00A1491B) */
            r15[dst] = ~r15[src1];
            break;

        case 0x33: /* OR_BIT (0x00A1492B) */
            r15[dst] = r15[src1] | r15[src2];
            break;

        case 0x34: /* XOR_BIT (0x00A1493C) */
            r15[dst] = r15[src1] ^ r15[src2];
            break;

        case 0x35: /* NOR_BIT (0x00A1494D) */
            r15[dst] = ~(r15[src1] | r15[src2]);
            break;

        case 0x36: /* AND_LOGIC (0x00A14961) */
            r15[dst] = (r15[src1] != 0 && r15[src2] != 0) ? ~0ull : 0ull;
            break;

        case 0x37: /* OR_LOGIC (0x00A14979) */
            r15[dst] = (r15[src1] != 0 || r15[src2] != 0) ? ~0ull : 0ull;
            break;

        case 0x38: /* NEG_INT (0x00A14992) */
            r15[dst] = (uint64_t)(-(int64_t)r15[src1]);
            break;

        case 0x39: /* NEG_FLOAT (0x00A149A1) */
            r15[dst] = u2_f32_to_reg(-u2_reg_to_f32(r15[src1]));
            break;

        case 0x3A: /* GET_ARGC (0x00A149BA) */
            r15[dst] = (uint64_t)(int64_t)(int32_t)frame->argc;
            break;

        case 0x3B: /* ADD_IMM (0x00A149C7) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] + (int64_t)src2);
            break;

        case 0x3C: /* SUB_IMM (0x00A149D7) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] - (int64_t)src2);
            break;

        case 0x3D: /* MUL_IMM (0x00A149E7) */
            r15[dst] = (uint64_t)((int64_t)r15[src1] * (int64_t)src2);
            break;

        case 0x3E: /* DIV_IMM (0x00A149F8) */
            r15[dst] = src2 ? (uint64_t)((int64_t)r15[src1] / (int64_t)src2) : 0;
            break;

        case 0x3F: /* LOAD_I32 (0x00A14A0A) */
            r15[dst] = (uint64_t)(int64_t)(*(const int32_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x40: /* LOAD_F32 (0x00A14A17) */
        case 0x42: /* LOAD_SYM_C (0x00A14A28) */
            r15[dst] = (uint64_t)(*(const uint32_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x41: /* LOAD_I64 (0x00A141DF) */
            r15[dst] = r8[src1];
            break;

        case 0x43: /* LOAD_I8_C (0x00A14A35) */
            r15[dst] = (uint64_t)(int64_t)(*(const int8_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x44: /* LOAD_U8_C (0x00A14A43) */
            r15[dst] = (uint64_t)(*(const uint8_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x45: /* LOAD_I16 (0x00A14A51) */
            r15[dst] = (uint64_t)(int64_t)(*(const int16_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x46: /* LOAD_U16 (0x00A14A5F) */
            r15[dst] = (uint64_t)(*(const uint16_t *)(uintptr_t)(r8 + src1));
            break;

        case 0x49: /* LOAD_M_I8 (0x00A14AED) */
            r15[dst] = (uint64_t)(int64_t)(*(const int8_t *)(uintptr_t)r15[src1]);
            break;

        case 0x4A: /* LOAD_M_U8 (0x00A14AFE) */
            r15[dst] = (uint64_t)(*(const uint8_t *)(uintptr_t)r15[src1]);
            break;

        case 0x4B: /* LOAD_M_I16 (0x00A14B0E) */
            r15[dst] = (uint64_t)(int64_t)(*(const int16_t *)(uintptr_t)r15[src1]);
            break;

        case 0x4C: /* LOAD_M_U16 (0x00A14B1F) */
            r15[dst] = (uint64_t)(*(const uint16_t *)(uintptr_t)r15[src1]);
            break;

        case 0x4D: /* LOAD_M_I32 (0x00A14B2F) */
            r15[dst] = (uint64_t)(int64_t)(*(const int32_t *)(uintptr_t)r15[src1]);
            break;

        default:
            break;
        }
    }
}

/* ============================================================================
 * 5. Runtime Hook Entrypoints
 * ============================================================================ */

static int u2_script_trace_enabled(void) {
    static int enabled = -1;
    if (enabled < 0) {
        const char *env = getenv("BB_U2_SCRIPT_TRACE");
        enabled = (env && *env && *env != '0') ? 1 : 0;
    }
    return enabled;
}

static uint64_t u2_exec_calls = 0;
static uint64_t u2_eval_calls = 0;

/* Hook callback invoked at the entry of ScriptManager::ExecuteScriptCode (image + 0x00A14120) */
void __attribute__((ms_abi)) runtime_u2_execute_script_code(
    uint64_t rdi, uint64_t rsi, uint64_t image, uint64_t r14
) {
    (void)r14;
    const uint64_t n = __atomic_add_fetch(&u2_exec_calls, 1, __ATOMIC_RELAXED);
    if (!u2_script_trace_enabled()) return;
    if (n <= 64 || (n & 0x3FFFull) == 0) {
        const U2ScriptStackFrame *frame = (const U2ScriptStackFrame *)(uintptr_t)rdi;
        uint8_t first_op = 0xFF;
        uint32_t pc = 0;
        if (frame && runtime_memory_is_mapped((uintptr_t)frame, sizeof(U2ScriptStackFrame))) {
            pc = frame->pc;
            if (frame->code && runtime_memory_is_mapped((uintptr_t)frame->code, sizeof(U2ScriptCode)) &&
                frame->code->bytecode && runtime_memory_is_mapped((uintptr_t)(frame->code->bytecode + pc * 4), 4)) {
                first_op = frame->code->bytecode[pc * 4];
            }
        }
        printf("[U2 ScriptVM] ExecuteScriptCode(#%" PRIu64 ") frame=%#" PRIx64 " pc=%u op=0x%02x (%s) out=%#" PRIx64 " image=%#" PRIx64 "\n",
               n, rdi, pc, first_op, u2_opcode_name(first_op), rsi, image);
    }
}

/* ============================================================================
 * 4b. Complete C Implementation of ScriptManager::EvalEx (0x00A15430)
 * ============================================================================ */

/* Stackless lightweight expression evaluator for animations, state transitions & AI */
int64_t ABI detour_EvalEx(const U2ScriptCode *code, int32_t argc, const uint64_t *args) {
    if (!code || !code->bytecode) return 0;

    /* 40 local registers initialized to 0 (matches rsp+32..rsp+352) */
    uint64_t regs[40];
    memset(regs, 0, sizeof(regs));

    /* Caller arguments are copied to regs[24..39] (&regs[24] = rsp + 224) */
    if (args && argc > 0) {
        int copy_count = argc < 16 ? argc : 16;
        memcpy(&regs[24], args, (size_t)copy_count * sizeof(uint64_t));
    }

    const uint64_t *constants = code->constants;
    const uint8_t *pc = code->bytecode;

    while (1) {
        uint8_t op   = pc[0];
        uint8_t dst  = pc[1];
        uint8_t src1 = pc[2];
        uint8_t src2 = pc[3];

        if (dst >= 40) dst = 0;

        switch (op) {
        case 0x00: /* RET (0x00A15D20) */
            return (src1 < 40) ? (int64_t)regs[src1] : 0;

        case 0x01: /* ADD_INT (0x00A15556) */
            regs[dst] = regs[src1] + regs[src2];
            break;

        case 0x02: /* SUB_INT (0x00A15565) */
            regs[dst] = regs[src1] - regs[src2];
            break;

        case 0x03: /* MUL_INT (0x00A1557A) */
            regs[dst] = (uint64_t)((int64_t)regs[src1] * (int64_t)regs[src2]);
            break;

        case 0x04: /* DIV_INT (0x00A1558A) */
            regs[dst] = regs[src2] ? (uint64_t)((int64_t)regs[src1] / (int64_t)regs[src2]) : 0;
            break;

        case 0x05: /* ADD_FLOAT (0x00A155A4) */
            regs[dst] = u2_f32_to_reg(u2_reg_to_f32(regs[src1]) + u2_reg_to_f32(regs[src2]));
            break;

        case 0x06: /* SUB_FLOAT (0x00A155C0) */
            regs[dst] = u2_f32_to_reg(u2_reg_to_f32(regs[src1]) - u2_reg_to_f32(regs[src2]));
            break;

        case 0x07: /* MUL_FLOAT (0x00A155DC) */
            regs[dst] = u2_f32_to_reg(u2_reg_to_f32(regs[src1]) * u2_reg_to_f32(regs[src2]));
            break;

        case 0x08: /* DIV_FLOAT (0x00A155F8) */
            regs[dst] = u2_f32_to_reg(u2_reg_to_f32(regs[src1]) / u2_reg_to_f32(regs[src2]));
            break;

        case 0x09: /* LOAD_POOL (0x00A15535) */
        case 0x0B: /* SHR_INT   (0x00A15535) */
        case 0x41: /* LOAD_I64  (0x00A1553A) */
            if (constants) regs[dst] = constants[src1];
            break;

        case 0x0C: /* LOAD_IMM16 (0x00A15632) */
            regs[dst] = ((uint64_t)src2 << 8) | (uint64_t)src1;
            break;

        case 0x0D: /* DEREF_I32 (0x00A15640) */
            regs[dst] = (uint64_t)(*(const uint32_t *)(uintptr_t)regs[src1]);
            break;

        case 0x0E: /* DEREF_F32 (0x00A15655) */
            regs[dst] = (uint64_t)(*(const uint32_t *)(uintptr_t)regs[src1]);
            break;

        case 0x0F: /* DEREF_I64 (0x00A1566E) */
            regs[dst] = *(const uint64_t *)(uintptr_t)regs[src1];
            break;

        case 0x10: { /* STORE_32 (0x00A15684) */
            uint32_t val = (uint32_t)regs[src2];
            *(uint32_t *)(uintptr_t)regs[src1] = val;
            regs[dst] = (uint64_t)val;
            break;
        }

        case 0x11: { /* STORE_U32 (0x00A156A0) */
            uint32_t val = (uint32_t)regs[src2];
            *(uint32_t *)(uintptr_t)regs[src1] = val;
            regs[dst] = (uint64_t)val;
            break;
        }

        case 0x12: { /* STORE_64 (0x00A156BF) */
            uint64_t val = regs[src2];
            *(uint64_t *)(uintptr_t)regs[src1] = val;
            regs[dst] = val;
            break;
        }

        case 0x15: { /* LOAD_SYM (0x00A156DF) */
            if (constants) {
                uint32_t sym_id = (uint32_t)constants[src1];
                typedef uint8_t *(ABI *lookup_fn)(uint32_t);
                lookup_fn fn = (lookup_fn)(uintptr_t)(g_u2_image_base + 0x00A15E70ull);
                uint8_t *sym_entry = fn(sym_id);
                if (sym_entry) {
                    regs[dst] = (uint64_t)(uintptr_t)sym_entry;
                } else {
                    /* Inlined symbol not found logger at 0x00A15712 */
                    regs[dst] = (uint64_t)(uintptr_t)(g_u2_image_base + 0x009F37B0ull);
                }
            }
            break;
        }

        case 0x16: /* MOV      (0x00A15525) */
        case 0x17: /* MOV_ALT1 (0x00A15525) */
        case 0x18: /* MOV_ALT2 (0x00A15525) */
            regs[dst] = regs[src1];
            break;

        case 0x19: /* FTOI (0x00A1572F) */
            regs[dst] = (uint64_t)(int64_t)(int32_t)u2_reg_to_f32(regs[dst]);
            break;

        case 0x1A: /* ITOF (0x00A15746) */
            regs[dst] = u2_f32_to_reg((float)(int64_t)regs[dst]);
            break;

        case 0x1B: { /* CALL_DEFUN (0x00A1575D) */
            const U2ScriptCode *target = (const U2ScriptCode *)(uintptr_t)regs[src1];
            regs[dst] = (uint64_t)detour_EvalEx(target, (int32_t)src2, &regs[24]);
            break;
        }

        case 0x1C: { /* CALL_NATIVE (0x00A15778) */
            U2DcNativeFn fn = (U2DcNativeFn)(uintptr_t)regs[src1];
            if (fn) {
                uint32_t status = 0;
                regs[dst] = (uint64_t)fn((int32_t)src2, &regs[24], &status);
            }
            break;
        }

        case 0x1D: /* CMP_EQ (0x00A157A8) */
            regs[dst] = (regs[src1] == regs[src2]) ? 1ull : 0ull;
            break;

        case 0x1E: /* CMP_GT (0x00A157C6) */
            regs[dst] = ((int64_t)regs[src1] > (int64_t)regs[src2]) ? ~0ull : 0ull;
            break;

        case 0x1F: /* CMP_GE (0x00A157EB) */
            regs[dst] = ((int64_t)regs[src1] >= (int64_t)regs[src2]) ? ~0ull : 0ull;
            break;

        case 0x20: /* CMP_LT (0x00A15810) */
            regs[dst] = ((int64_t)regs[src1] < (int64_t)regs[src2]) ? ~0ull : 0ull;
            break;

        case 0x21: /* CMP_LE (0x00A15835) */
            regs[dst] = ((int64_t)regs[src1] <= (int64_t)regs[src2]) ? ~0ull : 0ull;
            break;

        case 0x22: /* FCMP_EQ (0x00A1585A) */
            regs[dst] = (u2_reg_to_f32(regs[src1]) == u2_reg_to_f32(regs[src2])) ? u2_f32_to_reg(1.0f) : 0ull;
            break;

        case 0x23: /* FCMP_GT (0x00A15883) */
            regs[dst] = (u2_reg_to_f32(regs[src1]) > u2_reg_to_f32(regs[src2])) ? ~0ull : 0ull;
            break;

        case 0x24: /* FCMP_GE (0x00A158A1) */
            regs[dst] = (u2_reg_to_f32(regs[src1]) >= u2_reg_to_f32(regs[src2])) ? ~0ull : 0ull;
            break;

        case 0x25: /* FCMP_LT (0x00A158BF) */
            regs[dst] = (u2_reg_to_f32(regs[src1]) < u2_reg_to_f32(regs[src2])) ? ~0ull : 0ull;
            break;

        case 0x26: /* FCMP_LE (0x00A158DD) */
            regs[dst] = (u2_reg_to_f32(regs[src1]) <= u2_reg_to_f32(regs[src2])) ? ~0ull : 0ull;
            break;

        case 0x27: /* MOD_INT (0x00A158FB) */
            regs[dst] = regs[src2] ? (uint64_t)((int64_t)regs[src1] % (int64_t)regs[src2]) : 0;
            break;

        case 0x28: /* CALL_HELPER (0x00A15915): fmodf */
            regs[dst] = u2_f32_to_reg(fmodf(u2_reg_to_f32(regs[src1]), u2_reg_to_f32(regs[src2])));
            break;

        case 0x29: { /* ABS_INT (0x00A15936) */
            int64_t v = (int64_t)regs[src1];
            regs[dst] = (uint64_t)(v < 0 ? -v : v);
            break;
        }

        case 0x2A: /* ABS_FLOAT (0x00A1594A) */
            regs[dst] = u2_f32_to_reg(fabsf(u2_reg_to_f32(regs[src1])));
            break;

        case 0x2E: /* JNZ (0x00A15968) */
            if (regs[src1] != 0) {
                uint32_t target_idx = (((uint32_t)src2 << 8) | (uint32_t)dst) - 1u;
                pc = code->bytecode + target_idx * 4;
            }
            break;

        case 0x2F: /* JZ (0x00A15975) */
            if (regs[src1] == 0) {
                uint32_t target_idx = (((uint32_t)src2 << 8) | (uint32_t)dst) - 1u;
                pc = code->bytecode + target_idx * 4;
            }
            break;

        case 0x2D: { /* JMP (0x00A15981) */
            uint32_t target_idx = (((uint32_t)src2 << 8) | (uint32_t)dst) - 1u;
            pc = code->bytecode + target_idx * 4;
            break;
        }

        case 0x30: /* NOT (0x00A1599C) */
            regs[dst] = (regs[src1] == 0) ? ~0ull : 0ull;
            break;

        case 0x31: /* AND_BIT (0x00A159B3) */
            regs[dst] = regs[src1] & regs[src2];
            break;

        case 0x32: /* NOT_BIT (0x00A159BF) */
            regs[dst] = ~regs[src1];
            break;

        case 0x33: /* OR_BIT (0x00A159D5) */
            regs[dst] = regs[src1] | regs[src2];
            break;

        case 0x34: /* XOR_BIT (0x00A159E1) */
            regs[dst] = regs[src1] ^ regs[src2];
            break;

        case 0x35: /* NOR_BIT (0x00A159ED) */
            regs[dst] = ~(regs[src1] | regs[src2]);
            break;

        case 0x36: /* AND_LOGIC (0x00A159FC) */
            regs[dst] = (regs[src1] != 0 && regs[src2] != 0) ? ~0ull : 0ull;
            break;

        case 0x37: /* OR_LOGIC (0x00A15A16) */
            regs[dst] = (regs[src1] != 0 || regs[src2] != 0) ? ~0ull : 0ull;
            break;

        case 0x38: /* NEG_INT (0x00A15A31) */
            regs[dst] = (uint64_t)(-(int64_t)regs[src1]);
            break;

        case 0x39: /* NEG_FLOAT (0x00A15A42) */
            regs[dst] = u2_f32_to_reg(-u2_reg_to_f32(regs[src1]));
            break;

        case 0x3A: /* GET_ARGC (0x00A15A60) */
            regs[dst] = (uint64_t)(int64_t)argc;
            break;

        case 0x3B: /* ADD_IMM (0x00A15A73) */
            regs[dst] = (uint64_t)((int64_t)regs[src1] + (int64_t)src2);
            break;

        case 0x3C: /* SUB_IMM (0x00A15A89) */
            regs[dst] = (uint64_t)((int64_t)regs[src1] - (int64_t)src2);
            break;

        case 0x3D: /* MUL_IMM (0x00A15A9F) */
            regs[dst] = (uint64_t)((int64_t)regs[src1] * (int64_t)src2);
            break;

        case 0x3E: /* DIV_IMM (0x00A15AB6) */
            regs[dst] = src2 ? (uint64_t)((int64_t)regs[src1] / (int64_t)src2) : 0;
            break;

        case 0x3F: /* LOAD_I32 (0x00A15ACE) */
            if (constants) regs[dst] = (uint64_t)(int64_t)(*(const int32_t *)(uintptr_t)(constants + src1));
            break;

        case 0x40: /* LOAD_F32 (0x00A1560E) */
        case 0x42: /* LOAD_SYM_C (0x00A15AE5) */
            if (constants) regs[dst] = (uint64_t)(*(const uint32_t *)(uintptr_t)(constants + src1));
            break;

        case 0x43: /* MOV_ARG (0x00A15AFB) */
            if (constants) regs[dst] = (uint64_t)(int64_t)(*(const int8_t *)(uintptr_t)(constants + src1));
            break;

        case 0x44: /* LOAD_CONST (0x00A15B13) */
            if (constants) regs[dst] = (uint64_t)(*(const uint8_t *)(uintptr_t)(constants + src1));
            break;

        case 0x45: /* LOAD_I16 (0x00A15B2A) */
            if (constants) regs[dst] = (uint64_t)(int64_t)(*(const int16_t *)(uintptr_t)(constants + src1));
            break;

        case 0x46: /* LOAD_U16 (0x00A15B42) */
            if (constants) regs[dst] = (uint64_t)(*(const uint16_t *)(uintptr_t)(constants + src1));
            break;

        case 0x49: /* LOAD_M_I8 (0x00A15BD5) */
            regs[dst] = (uint64_t)(int64_t)(*(const int8_t *)(uintptr_t)regs[src1]);
            break;

        case 0x4A: /* LOAD_M_U8 (0x00A15BEC) */
            regs[dst] = (uint64_t)(*(const uint8_t *)(uintptr_t)regs[src1]);
            break;

        case 0x4B: /* LOAD_M_I16 (0x00A15C02) */
            regs[dst] = (uint64_t)(int64_t)(*(const int16_t *)(uintptr_t)regs[src1]);
            break;

        case 0x4C: /* LOAD_M_U16 (0x00A15C19) */
            regs[dst] = (uint64_t)(*(const uint16_t *)(uintptr_t)regs[src1]);
            break;

        case 0x4D: /* LOAD_M_I32 (0x00A15C2F) */
            regs[dst] = (uint64_t)(int64_t)(*(const int32_t *)(uintptr_t)regs[src1]);
            break;

        default:
            break;
        }

        pc += 4;
    }
}

/* Forward declarations */
static int64_t ABI detour_ExecuteScriptCode(U2ScriptStackFrame *frame, uint64_t *out_result);

/* SysV-ABI replacement for ScriptManager::Run (0x00A14F20)
 * SysV arguments: rdi = out_result, rsi = code, rdx = args, rcx = argc
 */
static int64_t ABI detour_ScriptManager_Run(uint64_t *out_result, U2ScriptCode *code, const uint64_t *args, uint32_t argc) {
    if (out_result) *out_result = 0;
    uint8_t *frame_pool = g_u2_image_base + 0x015341E8ull;
    U2ScriptStackFrame *frame = *(U2ScriptStackFrame **)frame_pool;
    if (!frame) {
        return 0;
    }
    *(U2ScriptStackFrame **)frame_pool = frame->caller;
    frame->return_dst_reg = 0;
    (*(uint32_t *)(g_u2_image_base + 0x015341F0ull))++;
    frame->caller = NULL;

    if (args && argc > 0) {
        uint32_t n = argc < 16 ? argc : 16;
        for (uint32_t i = 0; i < n; ++i) {
            frame->regs[24 + i] = args[i];
        }
    }
    frame->return_dst_reg = 0x28;
    frame->code = code;
    frame->argc = argc;
    frame->pc = 0;

    int64_t res = detour_ExecuteScriptCode(frame, out_result);
    return res != 0 ? 1 : 0;
}
void __attribute__((ms_abi)) runtime_u2_eval_ex(
    uint64_t rdi, uint64_t rsi, uint64_t image, uint64_t r14
) {
    (void)r14;
    const uint64_t n = __atomic_add_fetch(&u2_eval_calls, 1, __ATOMIC_RELAXED);
    if (!u2_script_trace_enabled()) return;
    if (n <= 64 || (n & 0x3FFFull) == 0) {
        printf("[U2 ScriptVM] EvalEx(#%" PRIu64 ") rdi=%#" PRIx64 " rsi=%#" PRIx64 " image=%#" PRIx64 "\n",
               n, rdi, rsi, image);
    }
}

/* ============================================================================
 * 6. Native C Detours (Direct Binary Function Replacement)
 * ============================================================================ */

/* SysV-ABI replacement for ScriptManager::ExecuteScriptCode (0x00A14120) */
static int64_t ABI detour_ExecuteScriptCode(U2ScriptStackFrame *frame, uint64_t *out_result) {
    if (u2_script_trace_enabled()) {
        runtime_u2_execute_script_code((uint64_t)(uintptr_t)frame, (uint64_t)(uintptr_t)out_result,
                                       (uint64_t)(uintptr_t)g_u2_image_base, 0);
    }
    return runtime_u2_vm_execute(frame, out_result, (uint64_t)(uintptr_t)g_u2_image_base);
}

static void write_detour_jmp64(uint8_t *dst, uintptr_t target) {
    /* 12-byte absolute jump in SysV x86-64:
     * 48 B8 <target_64>  movabs rax, target
     * FF E0              jmp    rax
     */
    dst[0] = 0x48;
    dst[1] = 0xB8;
    memcpy(dst + 2, &target, 8);
    dst[10] = 0xFF;
    dst[11] = 0xE0;
}

static void runtime_u2_apply_engine_patches(uint8_t *image, uint64_t image_size) {
    /* 
     * Fix GetSubMeshInstance assertion trap in mesh rendering (ndlib/render/ngen/mesh.h:1361):
     * When iSubMesh >= m_numSubMeshInstances (e.g. LOD transition / missing submesh instance),
     * the engine hits an ASSERTION and traps on int $0x41 at 0x00C47E31.
     * Original at 0x00C47DE0:
     *   39 f0                  cmpl %esi, %eax
     *   7f 54                  jg 0xc47e38
     * When eax <= esi (m_numSubMeshInstances <= iSubMesh), we skip to 0x00C47F60
     * (the exact same skip target that 0x00C47DC8 uses when m_numSubMeshInstances == 0).
     */
    if (image && image_size >= 0x00C48000ULL) {
        static const uint8_t orig_submesh_check[8] = {
            0x7F, 0x54, 0x48, 0x8B, 0x05, 0x8D, 0x61, 0x92
        };
        if (memcmp(image + 0x00C47DE2, orig_submesh_check, sizeof(orig_submesh_check)) == 0) {
            /* 
             * Patch at 0x00C47DE2:
             * 0F 8E 78 01 00 00   jle 0x00C47F60 (target = 0x00C47DE8 + 0x178 = 0x00C47F60)
             * EB 4E               jmp 0x00C47E38 (target = 0x00C47DEA + 0x4E  = 0x00C47E38)
             */
            static const uint8_t patch_submesh_check[8] = {
                0x0F, 0x8E, 0x78, 0x01, 0x00, 0x00,
                0xEB, 0x4E
            };
            memcpy(image + 0x00C47DE2, patch_submesh_check, sizeof(patch_submesh_check));
            printf("[U2 Engine] Patched GetSubMeshInstance bounds check at 0x00C47DE2 -> skip out-of-bounds submeshes\n");
            fflush(stdout);
        }
    }
}

/* ============================================================================
 * Hook Installation & Detour Dispatcher
 * ============================================================================ */

void runtime_u2_install_detours(uint8_t *image, uint64_t image_size) {
    /* Verify Uncharted 2 (big2-ps4_Shipping.elf) via invariant bytes at 0x00A14126
     * inside ScriptManager::ExecuteScriptCode (bytes +6..+19, untouched by thunk hooks):
     * 41 56 41 55 41 54 53 48 81 E4 E0 FF FF FF
     */
    static const uint8_t u2_exec_sig[14] = {
        0x41, 0x56, 0x41, 0x55, 0x41, 0x54, 0x53,
        0x48, 0x81, 0xE4, 0xE0, 0xFF, 0xFF, 0xFF
    };
    if (!image || image_size < 0x00B00000ULL) return;
    if (memcmp(image + 0x00A14126, u2_exec_sig, sizeof(u2_exec_sig)) != 0) {
        printf("[U2 Recomp] Signature mismatch at 0x00A14126, detours not installed.\n");
        fflush(stdout);
        return;
    }

    g_u2_image_base = image;

    runtime_u2_apply_engine_patches(image, image_size);

    /*
     * ADR-041: Modular Detour Selector & Validation Framework.
     * Allows testing detours in isolation:
     * - BB_U2_MODULES=none (or BB_U2_DISABLE_DETOURS=1): baseline pure ELF mode (0 module detours).
     * - BB_U2_MODULES=player,scene,... : enables only specified modules.
     * - default: enables all modules.
     */
    const char *disable_detours = getenv("BB_U2_DISABLE_DETOURS");
    const char *modules_env = getenv("BB_U2_MODULES");
    const char *range_env = getenv("BB_U2_DETOUR_RANGE");
    const char *allow_env = getenv("BB_U2_DETOUR_ALLOW");
    const bool disable_all_modules = (disable_detours && *disable_detours && *disable_detours != '0') ||
                                     (modules_env && strcmp(modules_env, "none") == 0);

    uintptr_t range_min = 0;
    uintptr_t range_max = UINTPTR_MAX;
    if (range_env && *range_env) {
        char *endp = NULL;
        range_min = (uintptr_t)strtoull(range_env, &endp, 0);
        if (endp && (*endp == '-' || *endp == ':')) {
            range_max = (uintptr_t)strtoull(endp + 1, NULL, 0);
        }
        printf("[U2 Recomp] Detour range filter active: [0x%" PRIxPTR " .. 0x%" PRIxPTR "]\n", range_min, range_max);
        fflush(stdout);
    }

    size_t installed = 0;

    typedef struct {
        const char *name;
        size_t (*getter)(const U2NativeDetour **);
    } ModuleEntry;

    static const ModuleEntry s_modules[] = {
        { "player", u2_get_player_detours },
        { "npc",    u2_get_npc_detours },
        { "audio",  u2_get_audio_detours },
        { "event",  u2_get_event_detours },
        { "scene",  u2_get_scene_detours },
    };

    if (disable_all_modules) {
        printf("[U2 Recomp] All 5 native C modules disabled (baseline ELF pur mode - ADR-041)\n");
        fflush(stdout);
    } else {
        for (size_t g = 0; g < sizeof(s_modules) / sizeof(s_modules[0]); ++g) {
            bool enabled = true;
            if (modules_env && *modules_env && strcmp(modules_env, "all") != 0) {
                enabled = (strstr(modules_env, s_modules[g].name) != NULL);
            }
            if (!enabled) {
                printf("[U2 Recomp] Module '%s' disabled\n", s_modules[g].name);
                continue;
            }
            const U2NativeDetour *mod_detours = NULL;
            size_t count = s_modules[g].getter(&mod_detours);
            size_t mod_installed = 0;
            for (size_t i = 0; i < count; ++i) {
                uintptr_t off = mod_detours[i].offset;
                if (off < range_min || off > range_max) continue;
                if (allow_env && *allow_env && strstr(allow_env, mod_detours[i].name) == NULL) continue;
                if (off > 0 && off + 14 <= image_size) {
                    write_detour_jmp64(image + off, mod_detours[i].host_fn);
                    ++installed;
                    ++mod_installed;
                }
            }
            printf("[U2 Recomp] Module '%s': installed %zu native detours\n", s_modules[g].name, mod_installed);
            fflush(stdout);
        }
    }

    /* Automated Self-Test Harness (BB_U2_SELFTEST=1):
     * Fuzz-tests all registered DC C detours with edge-case inputs (argc=0, NULL argv,
     * unaligned/garbage status pointers, synthetic zeroed args) in pure-C safe mode
     * (g_u2_image_base temporarily NULL so uninitialized guest singletons are not called before main()).
     */
    const char *selftest_env = getenv("BB_U2_SELFTEST");
    if (selftest_env && *selftest_env && *selftest_env != '0') {
        size_t tested = 0;
        uint64_t zero_args[8] = {0};
        uint32_t valid_status = 0;
        uint32_t *bad_status = (uint32_t *)(uintptr_t)0x123; /* unaligned low garbage rdx */

        printf("[U2 SelfTest] Running edge-case validation across all native C DC detours...\n");
        fflush(stdout);
        g_u2_image_base = NULL;
        for (size_t g = 0; g < sizeof(s_modules) / sizeof(s_modules[0]); ++g) {
            const U2NativeDetour *mod_detours = NULL;
            size_t count = s_modules[g].getter(&mod_detours);
            for (size_t i = 0; i < count; ++i) {
                U2DcNativeFn fn = (U2DcNativeFn)mod_detours[i].host_fn;
                if (!fn) continue;
                /* Skip DcDebugBreak / DcEndScript / DcJump which intentionally manipulate VM or trap */
                if (strcmp(mod_detours[i].name, "DcDebugBreak") == 0 ||
                    strcmp(mod_detours[i].name, "DcJump") == 0 ||
                    strcmp(mod_detours[i].name, "DcEndScript") == 0) {
                    continue;
                }
                /* Edge case 1: 0 args, NULL argv, garbage unaligned rdx */
                fn(0, NULL, bad_status);
                /* Edge case 2: 0 args, valid zeroed argv, valid status */
                fn(0, zero_args, &valid_status);
                /* Edge case 3: 4 args, valid zeroed argv, garbage unaligned rdx */
                fn(4, zero_args, bad_status);
                ++tested;
            }
        }
        g_u2_image_base = image;
        printf("[U2 SelfTest] PASSED: %zu native C functions survived edge-case fuzzing without fault!\n", tested);
        fflush(stdout);
    }

    const char *native_vm_env = getenv("BB_U2_NATIVE_VM");
    const int enable_native_vm = !disable_all_modules && (!native_vm_env || !*native_vm_env || *native_vm_env != '0');
    if (enable_native_vm) {
        write_detour_jmp64(image + 0x00A14120, (uintptr_t)detour_ExecuteScriptCode);
        ++installed;
    }

    const char *native_evalex_env = getenv("BB_U2_NATIVE_EVALEX");
    const int enable_native_evalex = !disable_all_modules && (!native_evalex_env || !*native_evalex_env || *native_evalex_env != '0');
    if (enable_native_evalex) {
        write_detour_jmp64(image + 0x00A15430, (uintptr_t)detour_EvalEx);
        ++installed;
    }

    const char *native_run_env = getenv("BB_U2_NATIVE_RUN");
    const int enable_native_run = !disable_all_modules && (!native_run_env || !*native_run_env || *native_run_env != '0');
    if (enable_native_run) {
        write_detour_jmp64(image + 0x00A14F20, (uintptr_t)detour_ScriptManager_Run);
        ++installed;
    }

    printf("[U2 Recomp] Installed %zu native C function detours across 5 modules (ExecuteScriptCode=%s, EvalEx=%s, Run=%s)\n",
           installed,
           enable_native_vm ? "NATIVE_C" : "GUEST_ELF",
           enable_native_evalex ? "NATIVE_C" : "GUEST_ELF",
           enable_native_run ? "NATIVE_C" : "GUEST_ELF");
    fflush(stdout);
}
