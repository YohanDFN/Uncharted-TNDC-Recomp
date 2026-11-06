/* Uncharted 2: Among Thieves (big2-ps4_Shipping.elf)
 * DC00 Script Engine Native C Reimplementation & Hook Orchestration:
 *   - ScriptManager::ExecuteScriptCode (guest offset 0x00A14120)
 *   - ScriptManager::EvalEx            (guest offset 0x00A15430)
 *   - ScriptManager::Run               (guest offset 0x00A14F20 / 0x00A150E0)
 *   - ScriptManager::Initialize        (guest offset 0x00A173A0)
 */
#include "runtime_u2_common.h"
#include <malloc.h>

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

/* ============================================================================
 * Safe Symbol Lookup Detours:
 *   - detour_LookupPointer         (0x00A15E70): intercepts missing decal material 0xCD0FBE5A
 *   - detour_LookupPointerInModule (0x00A17280): intercepts missing render-settings 0xAD087087
 * ============================================================================ */

static const char *s_empty_str_ptr = "";

static struct {
    uint8_t pad0[0x48];
    const char **p48;
    const char **p50;
    uint8_t pad1[8];
    const char **p60;
    uint8_t pad2[0x80];
} s_dummy_decal;
static int s_dummy_decal_initialized = 0;

static void init_dummy_decal(void) {
    if (!s_dummy_decal_initialized) {
        memset(&s_dummy_decal, 0, sizeof(s_dummy_decal));
        s_dummy_decal.p48 = &s_empty_str_ptr;
        s_dummy_decal.p50 = &s_empty_str_ptr;
        s_dummy_decal.p60 = &s_empty_str_ptr;
        s_dummy_decal_initialized = 1;
    }
}

static uint64_t s_dummy_zero_u64 = 0;
static struct {
    uint8_t pad[0x100];
} s_dummy_sub_obj;

static struct {
    uint8_t pad0[0x20];
    void *p20;          /* +0x20: points to &s_dummy_zero_u64 */
    uint8_t pad1[0x18]; /* +0x28 */
    void *p40;          /* +0x40: points to &s_dummy_sub_obj */
    uint8_t pad2[8];    /* +0x48 */
    void *p50;          /* +0x50: points to &s_dummy_sub_obj */
    void *p58;          /* +0x58: points to &s_dummy_sub_obj */
    void *p60;          /* +0x60: points to &s_dummy_sub_obj */
    void *p68;          /* +0x68: points to &s_dummy_sub_obj */
    uint32_t *p70;      /* +0x70: points to (uint32_t*)&s_dummy_zero_u64 */
    uint8_t pad3[0x10000];
} s_dummy_generic_module_obj;
static int s_dummy_generic_module_obj_inited = 0;

static void init_dummy_generic_module_obj(void) {
    if (!s_dummy_generic_module_obj_inited) {
        memset(&s_dummy_sub_obj, 0, sizeof(s_dummy_sub_obj));
        memset(&s_dummy_generic_module_obj, 0, sizeof(s_dummy_generic_module_obj));
        s_dummy_generic_module_obj.p20 = &s_dummy_zero_u64;
        s_dummy_generic_module_obj.p40 = &s_dummy_sub_obj;
        s_dummy_generic_module_obj.p50 = &s_dummy_sub_obj;
        s_dummy_generic_module_obj.p58 = &s_dummy_sub_obj;
        s_dummy_generic_module_obj.p60 = &s_dummy_sub_obj;
        s_dummy_generic_module_obj.p68 = &s_dummy_sub_obj;
        s_dummy_generic_module_obj.p70 = (uint32_t *)&s_dummy_zero_u64;
        s_dummy_generic_module_obj_inited = 1;
    }
}

static const char s_dummy_art_group[8] = "ak47";

static struct {
    uint8_t pad[0x58];
    const char *m_artGroup;
} s_dummy_weapon_def;

static struct {
    uint32_t sym_id;
    uint32_t pad04;
    void    *pWeaponDef;
} s_dummy_weapon_entry;

static struct {
    uint32_t count;
    uint32_t pad04;
    void    *entries;
} s_dummy_weapon_table;
static int s_dummy_weapon_table_inited = 0;

static void init_dummy_weapon_table(void) {
    if (!s_dummy_weapon_table_inited) {
        memset(&s_dummy_weapon_def, 0, sizeof(s_dummy_weapon_def));
        s_dummy_weapon_def.m_artGroup = s_dummy_art_group;

        s_dummy_weapon_entry.sym_id = 0x365A20DE;
        s_dummy_weapon_entry.pad04 = 0;
        s_dummy_weapon_entry.pWeaponDef = &s_dummy_weapon_def;

        s_dummy_weapon_table.count = 1;
        s_dummy_weapon_table.pad04 = 0;
        s_dummy_weapon_table.entries = &s_dummy_weapon_entry;

        s_dummy_weapon_table_inited = 1;
    }
}

static struct {
    uint32_t count;
    uint32_t pad04;
    void    *entries;
    uint8_t  pad10[0x100];
} s_dummy_zero_table;
static int s_dummy_zero_table_inited = 0;

static void init_dummy_zero_table(void) {
    if (!s_dummy_zero_table_inited) {
        memset(&s_dummy_zero_table, 0, sizeof(s_dummy_zero_table));
        s_dummy_zero_table.count = 0;
        s_dummy_zero_table.entries = &s_dummy_zero_table;
        s_dummy_zero_table_inited = 1;
    }
}

static struct {
    void *ptrs[128];
} s_dummy_module_table_ref;
static int s_dummy_module_table_ref_inited = 0;

static void init_dummy_module_table_ref(void) {
    if (!s_dummy_module_table_ref_inited) {
        init_dummy_zero_table();
        for (size_t i = 0; i < 128; ++i) {
            s_dummy_module_table_ref.ptrs[i] = &s_dummy_zero_table;
        }
        s_dummy_module_table_ref_inited = 1;
    }
}

/* SysV-ABI replacement for LookupPointer (0x00A15E70) */
static void * ABI detour_LookupPointer(uint32_t sym_id) {
    if (!g_u2_image_base) return NULL;
    typedef void * (ABI *LookupSymbolFn)(uint32_t sym_id);
    LookupSymbolFn guest_LookupSymbol = (LookupSymbolFn)(g_u2_image_base + 0x00A15300);

    void *entry = guest_LookupSymbol(sym_id);
    if (entry) {
        return *(void **)entry;
    }

    if (sym_id == 0xCD0FBE5A) {
        init_dummy_decal();
        return &s_dummy_decal;
    }

    if (sym_id == 0x74BFA7D0 || sym_id == 0x74FFCEEB ||
        sym_id == 0x7805BEB9 || sym_id == 0x0DEC97A4 ||
        sym_id == 0xB4BCCE62 || sym_id == 0x76E08710 ||
        sym_id == 0x9A091ADD || sym_id == 0x6E51B48F ||
        sym_id == 0x459D0E4E || sym_id == 0x1020810B ||
        sym_id == 0x5EB35427) {
        init_dummy_zero_table();
        return &s_dummy_zero_table;
    }

    if (sym_id == 0x0A2E5BED) {
        init_dummy_weapon_table();
        return &s_dummy_weapon_table;
    }

    /* Missing symbol fallback: do not trigger int $0x41 assertion crash */
    return NULL;
}

/* SysV-ABI replacement for LookupPointerInModule (0x00A17280) */
static void * ABI detour_LookupPointerInModule(uint32_t sym_id, uint32_t mod_id) {
    if (!g_u2_image_base) return NULL;
    typedef void * (ABI *LookupSymbolInModuleFn)(uint32_t sym_id, uint32_t mod_id);
    LookupSymbolInModuleFn guest_LookupSymbolInModule = (LookupSymbolInModuleFn)(g_u2_image_base + 0x00A17020);

    void *val = guest_LookupSymbolInModule(sym_id, mod_id);
    if (val) {
        return val;
    }

    if (sym_id == 0xAD087087 || mod_id == 0x9EC73B22 ||
        sym_id == 0x9353ED16 || mod_id == 0x863652C5) {
        init_dummy_generic_module_obj();
        return &s_dummy_generic_module_obj;
    }

    if (sym_id == 0x6E51B48F || mod_id == 0x67723261) {
        init_dummy_module_table_ref();
        return &s_dummy_module_table_ref;
    }

    if (sym_id == 0x35DC6ECE) {
        init_dummy_zero_table();
        return &s_dummy_zero_table;
    }

    /* Missing symbol fallback: do not trigger int $0x41 assertion crash */
    return NULL;
}

static struct {
    uint8_t pad[0x400];
} s_dummy_hud_sub_obj;

/* SysV-ABI replacement for Hud::ReadConfigFile (0x00552C70):
 * Safely initializes HUD material tables when hud.bin is inside PSARC archive
 */
static void ABI detour_Hud_ReadConfigFile(void *hud) {
    if (!hud) return;
    uint8_t *h = (uint8_t *)hud;
    *(uint32_t *)(h + 0x80) = 0;
    *(uint32_t *)(h + 0x108) = 0;
    memset(h + 0x88, 0, 0x80);
    memset(h + 0x110, 0, 0x200);
    memset(&s_dummy_hud_sub_obj, 0, sizeof(s_dummy_hud_sub_obj));
    *(void **)(h + 0x310) = &s_dummy_hud_sub_obj;
}

/* SysV-ABI replacement for Entity::UpdateExtents (0x00415700):
 * Bounds check before dereferencing virtual method GetBounds (*0x148)
 */
static void ABI detour_Entity_UpdateExtents(void *this_ptr, void *arg_rsi) {
    if (!this_ptr) return;
    uint8_t *obj = (uint8_t *)this_ptr;

    uint64_t rax = 0;
    uint32_t ecx = 0;
    if (arg_rsi) {
        rax = *(uint64_t *)((uint8_t *)arg_rsi + 0x38);
        ecx = *(uint32_t *)((uint8_t *)arg_rsi + 0x44);
    }
    *(uint64_t *)(obj + 0x88) = rax;
    *(uint32_t *)(obj + 0x90) = ecx;
    *(uint64_t *)(obj + 0x98) = 0;
    obj[0x35] |= 0x40;

    void **vtable = *(void ***)obj;
    if (!vtable) return;

    typedef void * (ABI *GetBoundsFn)(void *this_p);
    GetBoundsFn getBounds = (GetBoundsFn)vtable[0x148 / sizeof(void *)];
    if (!getBounds) return;

    void *bounds = getBounds(this_ptr);
    if (!bounds) {
        memset(obj + 0x12c, 0, 24);
        return;
    }

    const float *max_pt = (const float *)((uint8_t *)bounds + 0x10);
    *(float *)(obj + 0x138) = max_pt[0];
    *(float *)(obj + 0x13c) = max_pt[1];
    *(float *)(obj + 0x140) = max_pt[2];

    void *bounds2 = getBounds(this_ptr);
    if (!bounds2) return;
    const float *min_pt = (const float *)((uint8_t *)bounds2 + 0x00);
    *(float *)(obj + 0x12c) = min_pt[0];
    *(float *)(obj + 0x130) = min_pt[1];
    *(float *)(obj + 0x134) = min_pt[2];
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

    /* Set bp_skipintromovie = 1 only if explicitly requested via BB_U2_SKIP_INTRO=1 */
    const char *skip_intro_env = getenv("BB_U2_SKIP_INTRO");
    if (skip_intro_env && *skip_intro_env && *skip_intro_env != '0') {
        if (image && image_size > 0x0105DE94) {
            image[0x0105DE94] = 1;
            printf("[U2 Engine] Set bp_skipintromovie = 1 at 0x0105DE94 (via BB_U2_SKIP_INTRO=1)\n");
            fflush(stdout);
        }
    } else {
        printf("[U2 Engine] Cinematic playback enabled (bp_skipintromovie = 0)\n");
        fflush(stdout);
    }

    /*
     * UCCApp Update Patch at 0x00D5A95F:
     * In retail builds, UCCApp::Update skips all processing if m_state < 7 (ja 0xd5b12d).
     * NOP out the conditional jump so UCCApp updates run cleanly.
     */
    if (image && image_size >= 0x00D5A95F + 6) {
        static const uint8_t orig_ucc_skip[6] = { 0x0F, 0x87, 0xC8, 0x07, 0x00, 0x00 };
        static const uint8_t nop_ucc_skip[6]  = { 0x66, 0x0F, 0x1F, 0x44, 0x00, 0x00 };
        if (memcmp(image + 0x00D5A95F, orig_ucc_skip, sizeof(orig_ucc_skip)) == 0) {
            memcpy(image + 0x00D5A95F, nop_ucc_skip, sizeof(nop_ucc_skip));
            printf("[U2 Engine] Patched UCCApp::Update at 0x00D5A95F -> bypass state < 7 skip\n");
            fflush(stdout);
        }
    }
}

/* ============================================================================
 * IntroState / Boot Cutscene Bypass
 * ============================================================================ */

static void ABI detour_IntroState_Update(void *this_ptr) {
    if (!this_ptr || !g_u2_image_base) return;

    static uint32_t s_intro_frame_count = 0;
    static bool s_transitioned = false;
    ++s_intro_frame_count;

    /* Clear MovieManager playback flags since PS4 hardware videodec is absent on PC */
    void **movie_mgr_slot = (void **)(g_u2_image_base + 0x010608E0);
    if (movie_mgr_slot && *movie_mgr_slot) {
        uint8_t *movie_mgr = (uint8_t *)*movie_mgr_slot;
        *(uint32_t *)(movie_mgr + 0x2256C) = 0;
        *(uint8_t *)(movie_mgr + 0x22570) = 0;
    }

    /* Print periodic progress to console */
    if (s_intro_frame_count <= 20 || s_intro_frame_count % 60 == 0) {
        uint32_t pad_buttons = *(uint32_t *)(g_u2_image_base + 0x01116180 + 0x864);
        printf("[U2 Boot] IntroState_Update frame %u (pad=0x%04x, transitioned=%d)\n",
               s_intro_frame_count, pad_buttons & 0xFFFF, (int)s_transitioned);
        fflush(stdout);
    }

    if (s_transitioned) return;

    /* Poll pad input: 0x6008 = Start (0x0008), Cross (0x2000), Enter (0x4000) */
    uint32_t pad_buttons = *(uint32_t *)(g_u2_image_base + 0x01116180 + 0x864);

    /* After 15 frames warmup or immediately upon user confirmation, advance to main menu */
    if (s_intro_frame_count >= 15 || (pad_buttons & 0x6008) != 0) {
        s_transitioned = true;
        printf("[U2 Boot] IntroState completed (frame %u, pad=0x%04x) -> calling AdvanceToTitle (0x00A62BC0)\n",
               s_intro_frame_count, pad_buttons & 0xFFFF);
        fflush(stdout);

        /* Invoke the official AdvanceToTitle function (0x00A62BC0) */
        typedef void (ABI *AdvanceFn)(void *);
        AdvanceFn advance_title = (AdvanceFn)(g_u2_image_base + 0x00A62BC0);
        advance_title(this_ptr);
    }
}

/* ============================================================================
 * ComputeQueueMgr Detour (guest offset 0x00C24250)
 * ============================================================================ */
static uint8_t ABI detour_MapComputeQueue(void *this_ptr, void **pQueue, int32_t pipe, int32_t queue) {
    (void)this_ptr;
    if (pQueue && *pQueue) {
        uint32_t *q = (uint32_t *)*pQueue;
        q[0] = (uint32_t)pipe;
        q[1] = (uint32_t)queue;
        q[28] = (uint32_t)pipe;
        q[29] = (uint32_t)queue;
    }
    return 1;
}

/* ============================================================================
 * GPU Sync & EOP Tick Advancement Detour (guest offset 0x00B29F10)
 * ============================================================================ */

/* Naughty Dog GfxWaitEopFence (guest offset 0x00B29F10):
 * On PS4, AMD GCN GPU writes the end-of-pipe fence tick to *(sync_obj + 0x20)
 * via EVENT_WRITE_EOP packets.
 * On PC Vulkan, host memory is not directly updated by GCN packets, causing
 * the engine main thread to spin-wait forever in usleep(1000) at 0x00B29FF0.
 * Advancing *(sync_obj + 0x20) to target_tick satisfies the fence condition
 * and unblocks the CPU render loop to submit frames to Vulkan.
 */
uintptr_t s_b29f10_cont = 0;

__attribute__((naked)) static void ABI orig_SyncCounter_Wait(void *sync_obj, uint64_t target_tick, uint32_t mode) {
    __asm__ volatile (
        "pushq %%rbp\n\t"
        "movq %%rsp, %%rbp\n\t"
        "pushq %%r15\n\t"
        "pushq %%r14\n\t"
        "pushq %%r12\n\t"
        "pushq %%rbx\n\t"
        "movl %%edx, %%ebx\n\t"
        "movq %%rsi, %%r14\n\t"
        "jmpq *s_b29f10_cont(%%rip)\n\t"
        :
        :
        : "memory"
    );
}

static void ABI detour_GfxWaitEopFence(void *sync_obj, uint64_t target_tick, uint32_t mode) {
    if (!sync_obj || (uintptr_t)sync_obj < 0x10000) return;

    static uint64_t s_sync_calls = 0;
    static uint32_t s_last_ucc_state = 0xFFFFFFFF;
    uint64_t call_id = ++s_sync_calls;
    uintptr_t caller_va = 0;
    uintptr_t parent_va = 0;
    uint32_t tid = GetCurrentThreadId();
    uintptr_t stack_ret0 = 0, stack_ret1 = 0, stack_ret2 = 0, stack_ret3 = 0;
    if (g_u2_image_base) {
        uintptr_t ret_addr = (uintptr_t)__builtin_return_address(0);
        caller_va = ret_addr - (uintptr_t)g_u2_image_base;
        uintptr_t sp_val = 0;
        __asm__ volatile ("movq %%rsp, %0" : "=r"(sp_val));
        uintptr_t *sp = (uintptr_t *)sp_val;
        for (int k = 0; k < 128; ++k) {
            uintptr_t w = sp[k];
            if (w > (uintptr_t)g_u2_image_base && w < (uintptr_t)g_u2_image_base + 0x1000000ULL) {
                uintptr_t off = w - (uintptr_t)g_u2_image_base;
                if (!stack_ret0) stack_ret0 = off;
                else if (!stack_ret1 && off != stack_ret0) stack_ret1 = off;
                else if (!stack_ret2 && off != stack_ret0 && off != stack_ret1) stack_ret2 = off;
                else if (!stack_ret3 && off != stack_ret0 && off != stack_ret1 && off != stack_ret2) {
                    stack_ret3 = off;
                    break;
                }
            }
        }
    }

    uint32_t cur_state = 0;
    if (g_u2_image_base) {
        cur_state = *(uint32_t *)(g_u2_image_base + 0x010532D0);

        /* Fast-forward UCCApp from boot/credits wait (state 1..6) to Title Screen (state 7)
         * once MainLoopJob is actively running frames. */
        if (caller_va == 0xB2A067 && call_id >= 25 && cur_state >= 1 && cur_state < 7) {
            uint32_t *p_state = (uint32_t *)(g_u2_image_base + 0x010532D0);
            printf("[U2 Engine] Advancing UCCApp State %u -> 7 (Title Screen Activation, ADR-013)\n", *p_state);
            *p_state = 7;
            *(uint8_t *)(g_u2_image_base + 0x010532D0 + 0x14) = 1;
            cur_state = 7;
            fflush(stdout);
        }

        if (cur_state != s_last_ucc_state) {
            printf("[U2 Engine] State Transition: UCCApp State %u -> %u (caller=0x%llx, parent=0x%llx, tid=%u)\n",
                   s_last_ucc_state, cur_state, (unsigned long long)caller_va, (unsigned long long)parent_va, tid);
            fflush(stdout);
            s_last_ucc_state = cur_state;
        }
    }

    if ((call_id <= 25) || (call_id % 60 == 0)) {
        uint32_t pad = 0;
        if (g_u2_image_base) {
            pad = *(uint32_t *)(g_u2_image_base + 0x01116180 + 0x864);
        }
        printf("[U2 Loop] Sync #%" PRIu64 " | tid=%u | caller=0x%llx (stack=0x%llx, 0x%llx, 0x%llx, 0x%llx) | sync=%p | mode=%u | target=%" PRId64 " | cur=%" PRId64 " | UCCApp State=%u | Pad=0x%04x\n",
               call_id, tid, (unsigned long long)caller_va,
               (unsigned long long)stack_ret0, (unsigned long long)stack_ret1, (unsigned long long)stack_ret2, (unsigned long long)stack_ret3,
               sync_obj, mode, (int64_t)target_tick,
               *(int64_t *)((uint8_t *)sync_obj + 0x20), cur_state, pad & 0xFFFF);
        fflush(stdout);
    }

    void *playgo_sync = g_u2_image_base ? *(void **)(g_u2_image_base + 0x015AF7D0) : NULL;
    void *vram_sync   = g_u2_image_base ? *(void **)(g_u2_image_base + 0x0124A280 + 0xC14E8) : NULL;

    /*
     * Mode 1 waits (loading availability milestones), PlayGo sync, TextureManager VRAM
     * chunk streaming sync (0x00BF2F05), and Audio/ARM job sync (0x003B5002 / 0x1200105200)
     * must be satisfied immediately to avoid worker thread pool starvation / deadlocks.
     */
    if (mode == 1 || (playgo_sync && sync_obj == playgo_sync) ||
        caller_va == 0x00BF2F05 || (vram_sync && sync_obj == vram_sync) ||
        (sync_obj && (uintptr_t)sync_obj == 0x1200105200ULL) ||
        stack_ret0 == 0x3b5002 || stack_ret1 == 0x3b5002 || stack_ret2 == 0x3b5002 || stack_ret3 == 0x3b5002) {
        int64_t *cur_tick = (int64_t *)((uint8_t *)sync_obj + 0x20);
        *cur_tick = (int64_t)target_tick;
        return;
    }

    if (s_b29f10_cont) {
        if (mode == 0) {
            volatile int64_t *cur_tick = (volatile int64_t *)((uint8_t *)sync_obj + 0x20);
            for (int spin = 0; spin < 200; ++spin) {
                if (*cur_tick == (int64_t)target_tick) {
                    return;
                }
                if ((spin & 7) == 7) {
                    SwitchToThread();
                } else {
                    YieldProcessor();
                }
            }
        }
        orig_SyncCounter_Wait(sync_obj, target_tick, mode);
    }
}

/* ============================================================================
 * Job 2 (Engine Init Worker Job) Detour (guest offset 0x003F68C0)
 * ============================================================================ */

static uint8_t s_orig_job2[34];
typedef void (ABI *Job2Fn)(void *arg);
static Job2Fn orig_Job2 = (Job2Fn)s_orig_job2;

static volatile int s_job2_running = 0;
static HANDLE s_job2_thread = NULL;

static DWORD WINAPI job2_monitor_thread(LPVOID param) {
    (void)param;
    int samples = 0;
    while (s_job2_running && ++samples <= 40) {
        Sleep(250);
        if (!s_job2_running || !s_job2_thread) break;
        if (SuspendThread(s_job2_thread) != (DWORD)-1) {
            CONTEXT ctx;
            ctx.ContextFlags = CONTEXT_CONTROL;
            if (GetThreadContext(s_job2_thread, &ctx)) {
                uintptr_t rip = ctx.Rip;
                uintptr_t off = (g_u2_image_base && rip >= (uintptr_t)g_u2_image_base) ? (rip - (uintptr_t)g_u2_image_base) : 0;
                float budget = 0;
                uint64_t start_tsc = 0;
                uint64_t tsc_freq = g_u2_image_base ? *(uint64_t *)(g_u2_image_base + 0x01050638) : 0;
                if (ctx.Rsp && off >= 0xb68d50 && off <= 0xb6c000) {
                    budget = *(float *)((uint8_t *)ctx.Rsp + 0x94);
                    start_tsc = *(uint64_t *)((uint8_t *)ctx.Rsp + 0x80);
                }
                printf("[Job2 Monitor] sample #%d: RIP=%p (guest off=0x%llx, RSP=%p) | budget=%.4f, start_tsc=%" PRIu64 ", now_tsc=%" PRIu64 ", tsc_freq=%" PRIu64 "\n",
                       samples, (void *)rip, (unsigned long long)off, (void *)ctx.Rsp,
                       budget, start_tsc, (uint64_t)__rdtsc(), tsc_freq);
                fflush(stdout);
            }
            ResumeThread(s_job2_thread);
        }
    }
    return 0;
}

static void ABI detour_Job2(void *arg) {
    uint32_t tid = GetCurrentThreadId();
    printf("[U2 Job] Job 2 ENTER (tid=%u, arg=%p)\n", tid, arg);
    fflush(stdout);

    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
                    GetCurrentProcess(), (HANDLE *)&s_job2_thread,
                    0, FALSE, DUPLICATE_SAME_ACCESS);
    s_job2_running = 1;
    HANDLE hMon = CreateThread(NULL, 0, job2_monitor_thread, NULL, 0, NULL);

    orig_Job2(arg);

    s_job2_running = 0;
    if (hMon) {
        WaitForSingleObject(hMon, 1000);
        CloseHandle(hMon);
    }
    if (s_job2_thread) {
        CloseHandle(s_job2_thread);
        s_job2_thread = NULL;
    }
    printf("[U2 Job] Job 2 EXIT (tid=%u, arg=%p)\n", tid, arg);
    fflush(stdout);
}

/* ============================================================================
 * UCCApp::StateMachineUpdate Detour (guest offset 0x00D59A50)
 * ============================================================================ */

static uint8_t s_orig_ucc_update[34];
typedef void (ABI *UCCAppUpdateFn)(void *this_ptr, float dt);
static UCCAppUpdateFn orig_UCCApp_Update = (UCCAppUpdateFn)s_orig_ucc_update;

static void ABI detour_UCCApp_StateMachineUpdate(void *ucc_app, float dt) {
    static uint32_t s_count = 0;
    uint32_t state = ucc_app ? *(uint32_t *)ucc_app : 0xFFFFFFFF;
    if (++s_count <= 25 || s_count % 60 == 0) {
        printf("[U2 UCCApp] StateMachineUpdate #%u: state=%u, dt=%f, this=%p\n", s_count, state, dt, ucc_app);
        fflush(stdout);
    }
    orig_UCCApp_Update(ucc_app, dt);
    uint32_t post_state = ucc_app ? *(uint32_t *)ucc_app : 0xFFFFFFFF;
    if (post_state != state) {
        printf("[U2 UCCApp] State changed: %u -> %u\n", state, post_state);
        fflush(stdout);
    }
}

/* ============================================================================
 * JSON Parser Fallback for Ingame Shaders (0x00C2C230 / 0x00C2C0D0)
 * ============================================================================ */

static struct {
    uint32_t type;       /* +0x00: 5 (JSON Object) */
    uint32_t pad04;      /* +0x04 */
    void    *obj_data;   /* +0x08 */
} s_dummy_json_root;

static struct {
    uint64_t pad00;      /* +0x00 */
    uint32_t count;      /* +0x08: 0 children */
    uint32_t pad0c;      /* +0x0c */
    void    *first_child;/* +0x10: NULL */
} s_dummy_json_obj;

static void * ABI detour_JsonParseFiles(void *ctx) {
    (void)ctx;
    s_dummy_json_root.type = 5;
    s_dummy_json_root.pad04 = 0;
    s_dummy_json_root.obj_data = &s_dummy_json_obj;
    s_dummy_json_obj.pad00 = 0;
    s_dummy_json_obj.count = 0;
    s_dummy_json_obj.pad0c = 0;
    s_dummy_json_obj.first_child = NULL;
    return &s_dummy_json_root;
}

static void ABI detour_JsonFreeNode(void *node) {
    (void)node;
}

/* ============================================================================
 * Renderer Stubs & Utility Detours
 * ============================================================================ */

static void ABI detour_Renderer_InitParticles(void) {
    printf("[U2 Engine] Renderer::InitParticles at 0x0050E150 (stubbed)\n");
    fflush(stdout);
}

/* ============================================================================
 * StringIdToStringInternal Detour (guest offset 0x00CE33A0)
 * ============================================================================ */

static const char * ABI detour_StringIdToStringInternal(uint32_t id, uint32_t flag) {
    (void)flag;
    if (id == 0) return "";
    static char s_str_buffers[16][64];
    static uint32_t s_str_idx = 0;
    uint32_t slot = (s_str_idx++) & 15;
    snprintf(s_str_buffers[slot], sizeof(s_str_buffers[slot]), "str_%08x", id);
    return s_str_buffers[slot];
}

/* ============================================================================
 * Dummy Profiler / Performance Marker Vtable (0x01086740)
 * ============================================================================ */

static uint64_t ABI dummy_profiler_noop(void) {
    return 0;
}

static void *s_dummy_profiler_vtable[32];
static void *s_dummy_profiler_instance = s_dummy_profiler_vtable;

/* ============================================================================
 * Safe Job Dispatcher Guard (guest offset 0x00B3DB04)
 * ============================================================================ */

uint32_t s_tcb_offset = 0;
uintptr_t s_job_dispatch_ret = 0;
uintptr_t g_guest_code_base = 0;
uintptr_t g_guest_code_limit = 0;
uintptr_t g_host_code_base = 0;
uintptr_t g_host_code_limit = 0;
uintptr_t s_last_job_fn = 0;
void     *s_last_job_arg = NULL;
uint32_t runtime_thread_tcb_offset(void);

__attribute__((naked)) static void hook_JobDispatch(void) {
    __asm__ volatile (
        "movq -0x88(%%rbp), %%rdi\n\t"
        "cmpq g_guest_code_base(%%rip), %%r15\n\t"
        "jb 3f\n\t"
        "cmpq g_guest_code_limit(%%rip), %%r15\n\t"
        "jb 2f\n\t"
        "3:\n\t"
        "cmpq g_host_code_base(%%rip), %%r15\n\t"
        "jb 1f\n\t"
        "cmpq g_host_code_limit(%%rip), %%r15\n\t"
        "jae 1f\n\t"
        "2:\n\t"
        "movq %%r15, s_last_job_fn(%%rip)\n\t"
        "movq %%rdi, s_last_job_arg(%%rip)\n\t"
        "callq *%%r15\n\t"
        "1:\n\t"
        "movl s_tcb_offset(%%rip), %%eax\n\t"
        "movq %%gs:(%%rax), %%rax\n\t"
        "jmpq *s_job_dispatch_ret(%%rip)\n\t"
        :
        :
        : "memory"
    );
}

/* ============================================================================
 * Safe Particle / Effect Emitter Guard (guest offset 0x001E2110)
 * ============================================================================ */

uintptr_t s_1e2110_cont = 0;

__attribute__((naked)) static void hook_1e2110(void) {
    __asm__ volatile (
        "cmpq $0x1000, %%rdi\n\t"
        "jb 1f\n\t"
        "pushq %%rbp\n\t"
        "movq %%rsp, %%rbp\n\t"
        "pushq %%r15\n\t"
        "pushq %%r14\n\t"
        "pushq %%r12\n\t"
        "pushq %%rbx\n\t"
        "movq %%r8, %%rbx\n\t"
        "jmpq *s_1e2110_cont(%%rip)\n\t"
        "1:\n\t"
        "retq\n\t"
        :
        :
        : "memory"
    );
}

/* ============================================================================
 * Safe AllocByAllocatorId Arena Allocator & Fallback (guest offset 0x00B33160 / 0x00B33270)
 * ============================================================================ */

static ABI void *u2_alloc_arena_or_fallback(void **ppArena, size_t size) {
    void *arena = NULL;
    void *res = NULL;

    if (ppArena && (uintptr_t)ppArena > 0x10000) {
        arena = *ppArena;
    }

    if (arena && (uintptr_t)arena > 0x10000) {
        uintptr_t *vtable = *(uintptr_t **)arena;
        if (vtable && (uintptr_t)vtable > 0x10000) {
            uintptr_t pfn = vtable[0x48 / 8];
            if (pfn && pfn > 0x10000) {
                typedef void * (ABI *ArenaAllocFn)(void *arena, size_t sz, uint64_t align, const char *tag, uint64_t flags, const char *tag2);
                ArenaAllocFn alloc_fn = (ArenaAllocFn)pfn;
                res = alloc_fn(arena, size, 16, "UNKNOWN", 0, "UNKNOWN");
            }
        }
    }

    if (!res) {
        size_t alloc_sz = (size + 15) & ~15ULL;
        if (alloc_sz == 0) alloc_sz = 16;
        res = _aligned_malloc(alloc_sz, 16);
        if (res) {
            memset(res, 0, alloc_sz);
        }
        printf("[U2 Heap] AllocByAllocatorId fallback: allocated %zu bytes at %p (arena=%p, ppArena=%p)\n",
               size, res, arena, ppArena);
        fflush(stdout);
    }
    return res;
}

static ABI void *u2_alloc_chunk_node_fallback(void *allocated) {
    if (allocated && (uintptr_t)allocated > 0x10000) {
        memset(allocated, 0, 216);
        return allocated;
    }
    void *mem = _aligned_malloc(216, 16);
    if (mem) {
        memset(mem, 0, 216);
    }
    printf("[U2 Script] Allocated fallback chunk node at %p (original=%p)\n", mem, allocated);
    fflush(stdout);
    return mem;
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

    const char *disable_detours = getenv("BB_U2_DISABLE_DETOURS");
    if (disable_detours && *disable_detours && *disable_detours != '0') {
        printf("[U2 Recomp] Native C detours disabled via BB_U2_DISABLE_DETOURS=1\n");
        return;
    }

    size_t installed = 0;

    /* Collect and install detours from all specialized native modules */
    typedef size_t (*ModuleGetter)(const U2NativeDetour **);
    static const ModuleGetter s_module_getters[] = {
        u2_get_player_detours,
        u2_get_npc_detours,
        u2_get_audio_detours,
        u2_get_event_detours,
        u2_get_scene_detours,
    };

    for (size_t g = 0; g < sizeof(s_module_getters) / sizeof(s_module_getters[0]); ++g) {
        const U2NativeDetour *mod_detours = NULL;
        size_t count = s_module_getters[g](&mod_detours);
        for (size_t i = 0; i < count; ++i) {
            if (mod_detours[i].offset > 0 && mod_detours[i].offset < 0x00E00000 && mod_detours[i].offset + 14 <= image_size) {
                write_detour_jmp64(image + mod_detours[i].offset, mod_detours[i].host_fn);
                ++installed;
            }
        }
    }

    /* Install safe symbol lookup detours (prevent missing symbol assertions/faults) */
    write_detour_jmp64(image + 0x00A15E70, (uintptr_t)detour_LookupPointer);
    ++installed;
    write_detour_jmp64(image + 0x00A17280, (uintptr_t)detour_LookupPointerInModule);
    ++installed;
    write_detour_jmp64(image + 0x00552C70, (uintptr_t)detour_Hud_ReadConfigFile);
    ++installed;
    write_detour_jmp64(image + 0x00415700, (uintptr_t)detour_Entity_UpdateExtents);
    ++installed;
    const char *disable_intro = getenv("BB_U2_DISABLE_INTRO_DETOUR");
    if (!disable_intro || *disable_intro == '0') {
        write_detour_jmp64(image + 0x00A551B0, (uintptr_t)detour_IntroState_Update);
        ++installed;
        printf("[U2 Engine] Intro cutscene detour installed (automatic movie bypass)\n");
        fflush(stdout);
    }
    write_detour_jmp64(image + 0x00C24250, (uintptr_t)detour_MapComputeQueue);
    ++installed;
    s_b29f10_cont = (uintptr_t)(image + 0x00B29F20);
    write_detour_jmp64(image + 0x00B29F10, (uintptr_t)detour_GfxWaitEopFence);
    ++installed;
    write_detour_jmp64(image + 0x00C2C230, (uintptr_t)detour_JsonParseFiles);
    ++installed;
    write_detour_jmp64(image + 0x00C2C0D0, (uintptr_t)detour_JsonFreeNode);
    ++installed;
    write_detour_jmp64(image + 0x0050E150, (uintptr_t)detour_Renderer_InitParticles);
    ++installed;
    write_detour_jmp64(image + 0x00CE33A0, (uintptr_t)detour_StringIdToStringInternal);
    ++installed;

    if (image_size > 0x00D59A50 + 20) {
        memcpy(s_orig_ucc_update, image + 0x00D59A50, 20);
        write_detour_jmp64(s_orig_ucc_update + 20, (uintptr_t)(image + 0x00D59A64));
        DWORD old_prot;
        VirtualProtect(s_orig_ucc_update, sizeof(s_orig_ucc_update), PAGE_EXECUTE_READWRITE, &old_prot);
        write_detour_jmp64(image + 0x00D59A50, (uintptr_t)detour_UCCApp_StateMachineUpdate);
        memset(image + 0x00D59A50 + 12, 0x90, 8);
        installed++;
        printf("[U2 Engine] Installed detour_UCCApp_StateMachineUpdate at 0x00D59A50\n");
        fflush(stdout);
    }

    if (image_size > 0x003F68C0 + 20) {
        memcpy(s_orig_job2, image + 0x003F68C0, 20);
        write_detour_jmp64(s_orig_job2 + 20, (uintptr_t)(image + 0x003F68C0 + 20));
        DWORD old_prot;
        VirtualProtect(s_orig_job2, sizeof(s_orig_job2), PAGE_EXECUTE_READWRITE, &old_prot);
        write_detour_jmp64(image + 0x003F68C0, (uintptr_t)detour_Job2);
        memset(image + 0x003F68C0 + 12, 0x90, 8);
        installed++;
        printf("[U2 Engine] Installed detour_Job2 at 0x003F68C0\n");
        fflush(stdout);
    }

    /* Install hook_JobDispatch to protect fiber/job invocation at 0x00B3DB04 against invalid handles */
    if (image_size > 0x00B3DB17) {
        g_guest_code_base = (uintptr_t)image;
        g_guest_code_limit = (uintptr_t)image + 0x01870000ULL;
        g_host_code_base = (uintptr_t)GetModuleHandle(NULL);
        g_host_code_limit = g_host_code_base + 0x10000000ULL;
        s_tcb_offset = runtime_thread_tcb_offset();
        s_job_dispatch_ret = (uintptr_t)(image + 0x00B3DB17);
        write_detour_jmp64(image + 0x00B3DB04, (uintptr_t)hook_JobDispatch);
        memset(image + 0x00B3DB04 + 12, 0x90, 7);
        ++installed;
        printf("[U2 Engine] Installed hook_JobDispatch at 0x00B3DB04 (tcb_offset=0x%x, guest=[%p..%p], host=[%p..%p])\n",
               s_tcb_offset, (void *)g_guest_code_base, (void *)g_guest_code_limit,
               (void *)g_host_code_base, (void *)g_host_code_limit);
        fflush(stdout);
    }

    /* Install hook_1e2110 to protect particle/effect emitter update at 0x001E2110 against low pointer fault */
    if (image_size > 0x001E211E) {
        s_1e2110_cont = (uintptr_t)(image + 0x001E211E);
        write_detour_jmp64(image + 0x001E2110, (uintptr_t)hook_1e2110);
        memset(image + 0x001E2110 + 12, 0x90, 2);
        ++installed;
        printf("[U2 Engine] Installed hook_1e2110 at 0x001E2110\n");
        fflush(stdout);
    }

    if (image_size > 0x16c06b0) {
        *(uint8_t *)(image + 0x16c06b0) = 1; /* g_bStringInitCalled = true */
    }

    if (image_size > 0x1130008 + 0x318) {
        memset(&s_dummy_hud_sub_obj, 0, sizeof(s_dummy_hud_sub_obj));
        *(void **)(image + 0x1130008 + 0x310) = &s_dummy_hud_sub_obj;
    }

    for (size_t i = 0; i < sizeof(s_dummy_profiler_vtable)/sizeof(s_dummy_profiler_vtable[0]); ++i) {
        s_dummy_profiler_vtable[i] = (void *)dummy_profiler_noop;
    }
    if (image_size > 0x01086740 + 8) {
        if (*(void **)(image + 0x01086740) == NULL) {
            *(void **)(image + 0x01086740) = &s_dummy_profiler_instance;
            printf("[U2 Engine] Initialized dummy profiler/tracker at 0x01086740\n");
            fflush(stdout);
        }
    }

    /* Patch 0x003C03E7: skip 8-iteration table scan when r13 is NULL to avoid fault at 0x3c0400 */
    if (image_size > 0x003C03E7 + 25) {
        static const uint8_t s_patch_3c03e7[25] = {
            0x4d, 0x85, 0xed,                               /* testq %r13, %r13 */
            0x74, 0x4a,                                     /* je 0x3c0436 */
            0x49, 0x83, 0xc5, 0x08,                         /* addq $0x8, %r13 */
            0xc5, 0xfa, 0x10, 0x05, 0x08, 0xf5, 0xa9, 0x00, /* vmovss 0xa9f508(%rip), %xmm0 */
            0x0f, 0x1f, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00  /* nopl (%rax,%rax) */
        };
        memcpy(image + 0x003C03E7, s_patch_3c03e7, sizeof(s_patch_3c03e7));
        ++installed;
    }

    /* Patch 0x009F20B5: skip uninitialized hash table divide-by-zero (0xc0000094) and take fallback 0x9f20ec */
    if (image_size > 0x009F20B5 + 11) {
        static const uint8_t s_patch_9f20b5[11] = {
            0xeb, 0x35,                                     /* jmp 0x9f20ec */
            0x0f, 0x1f, 0x80, 0x00, 0x00, 0x00, 0x00,       /* 7-byte nopl (%rax) */
            0x66, 0x90                                      /* 2-byte nop */
        };
        memcpy(image + 0x009F20B5, s_patch_9f20b5, sizeof(s_patch_9f20b5));
        ++installed;
    }

    /* Patch 0x0057674B: Force boot initialization success (90 90 instead of jg 11) to run AppMainLoop (0x3f4aa0) and prevent false shutdown */
    if (image_size > 0x0057674B + 2) {
        static const uint8_t s_patch_57674b[2] = { 0x90, 0x90 };
        memcpy(image + 0x0057674B, s_patch_57674b, sizeof(s_patch_57674b));
        printf("[U2 Engine] Applied patch 0x0057674B: Force boot initialization success (NOP jg -> run AppMainLoop)\n");
        fflush(stdout);
        ++installed;
    }

    /* Initialize ComputeQueueMgr singleton at 0x0139BEF0 to prevent m_initialized asserts */
    if (image_size > 0x0139BEF0 + 0x48) {
        uint8_t *mgr = image + 0x0139BEF0;
        memset(mgr, 0, 0x48);
        mgr[0x28] = 1;
        mgr[0] = 1; /* m_initialized = 1 */
    }

    /* Patch 0x001D30A5: Guard atomic bitmask clear against NULL rdx in animation/job helper */
    if (image_size > 0x001D30A5 + 11) {
        static const uint8_t s_patch_1d30a5[11] = {
            0x48, 0x85, 0xd2,                               /* testq %rdx, %rdx */
            0x74, 0x17,                                     /* jz 0x1d30c1 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90              /* 6x nop */
        };
        memcpy(image + 0x001D30A5, s_patch_1d30a5, sizeof(s_patch_1d30a5));
        ++installed;
        printf("[U2 Engine] Applied patch 0x001D30A5: Guard atomic bitmask clear against NULL rdx\n");
        fflush(stdout);
    }

    /* Patch 0x001DE714: Guard atomic bitmask clear against NULL rdx in job completion helper */

    if (image_size > 0x001DE714 + 12) {
        static const uint8_t s_patch_1de714[12] = {
            0x48, 0x85, 0xd2,                               /* testq %rdx, %rdx */
            0x74, 0x1d,                                     /* jz 0x1de736 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90        /* 7x nop */
        };
        memcpy(image + 0x001DE714, s_patch_1de714, sizeof(s_patch_1de714));
        ++installed;
    }

    /* Patch 0x001DE841: Guard atomic bitmask clear against NULL rdx in job completion helper #2 */
    if (image_size > 0x001DE841 + 15) {
        static const uint8_t s_patch_1de841[15] = {
            0x48, 0x85, 0xd2,                               /* testq %rdx, %rdx */
            0x74, 0x20,                                     /* jz 0x1de866 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 /* 10x nop */
        };
        memcpy(image + 0x001DE841, s_patch_1de841, sizeof(s_patch_1de841));
        ++installed;
    }

    /* Patch 0x001D7A40: Guard vertex/bone array pointer against NULL rdx before add $0x30 */
    if (image_size > 0x001D7A40 + 9) {
        static const uint8_t s_patch_1d7a40[9] = {
            0x48, 0x85, 0xd2,                               /* testq %rdx, %rdx */
            0x74, 0x7a,                                     /* jz 0x1d7abf */
            0x48, 0x83, 0xc2, 0x30                           /* addq $0x30, %rdx */
        };
        memcpy(image + 0x001D7A40, s_patch_1d7a40, sizeof(s_patch_1d7a40));
        ++installed;
        printf("[U2 Engine] Applied patch 0x001D7A40: Guard NULL rdx array in skeletal/mesh bounds\n");
        fflush(stdout);
    }

    /* Patch 0x00A578D2: zero accessory outputs when rbx is NULL to prevent fault at 0x88(rbx) */
    if (image_size > 0x00A578D2 + 13) {
        static const uint8_t s_patch_a578d2[13] = {
            0x31, 0xf6,                                     /* xorl %esi, %esi */
            0x31, 0xc9,                                     /* xorl %ecx, %ecx */
            0x0f, 0x1f, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, /* 8-byte nopl (%rax,%rax) */
            0x90                                            /* 1-byte nop */
        };
        memcpy(image + 0x00A578D2, s_patch_a578d2, sizeof(s_patch_a578d2));
        ++installed;
    }

    /* Patch 0x00B77B95: Return true if chunk set name is not found in IsChunkSetEntirelyWithin to avoid NULL dereference */
    if (image_size > 0x00B77B95 + 7) {
        static const uint8_t s_patch_b77b95[7] = {
            0xb0, 0x01,                                     /* mov $1, %al */
            0x5b,                                           /* pop %rbx */
            0x41, 0x5e,                                     /* pop %r14 */
            0x5d,                                           /* pop %rbp */
            0xc3                                            /* retq */
        };
        memcpy(image + 0x00B77B95, s_patch_b77b95, sizeof(s_patch_b77b95));
        ++installed;
        printf("[U2 Engine] Applied patch 0x00B77B95: Safe return on unknown chunk set in IsChunkSetEntirelyWithin\n");
        fflush(stdout);
    }

    /* Patch 0x00A17E52 / 0x00A17E6A: Ensure symbol table size at 0x01534210 is >= 1 to prevent divide by zero */
    if (image_size > 0x00A17E6A + 6) {
        static const uint8_t s_patch_a17e52[14] = {
            0x85, 0xdb,                                     /* testl %ebx, %ebx */
            0x75, 0x02,                                     /* jne 0xa17e58 */
            0xff, 0xc3,                                     /* incl %ebx */
            0x89, 0x1d, 0xb2, 0xc3, 0xb1, 0x00,             /* movl %ebx, 0x1534210(%rip) */
            0xeb, 0x10                                      /* jmp 0xa17e70 */
        };
        static const uint8_t s_patch_a17e6a[6] = {
            0xeb, 0xe6,                                     /* jmp 0xa17e52 */
            0x90, 0x90, 0x90, 0x90                          /* 4x nop */
        };
        memcpy(image + 0x00A17E52, s_patch_a17e52, sizeof(s_patch_a17e52));
        memcpy(image + 0x00A17E6A, s_patch_a17e6a, sizeof(s_patch_a17e6a));
        installed += 2;
        printf("[U2 Engine] Applied patch 0x00A17E52: Ensure symbol table size >= 1\n");
        fflush(stdout);
    }

    /* Patch 0x00A1888C: Round symbol rehash bucket count up to at least 16 (0x1f instead of 0x0f) */
    if (image_size > 0x00A1888C) {
        image[0x00A1888C] = 0x1f;
        ++installed;
        printf("[U2 Engine] Applied patch 0x00A1888C: Min 16 buckets in ScriptManager rehash\n");
        fflush(stdout);
    }

    /* Patch 0x00A188CC, 0x00A188DA, 0x00A1890A: If table allocation or context is NULL, skip to 0x00A18CBE to avoid divide by zero at 0x00A18B1B */
    if (image_size > 0x00A1890A + 6) {
        static const uint8_t s_patch_a188cc[6] = { 0x0f, 0x84, 0xec, 0x03, 0x00, 0x00 }; /* je 0x00a18cbe */
        static const uint8_t s_patch_a188da[6] = { 0x0f, 0x84, 0xde, 0x03, 0x00, 0x00 }; /* je 0x00a18cbe */
        static const uint8_t s_patch_a1890a[6] = { 0x0f, 0x84, 0xae, 0x03, 0x00, 0x00 }; /* je 0x00a18cbe */
        memcpy(image + 0x00A188CC, s_patch_a188cc, sizeof(s_patch_a188cc));
        memcpy(image + 0x00A188DA, s_patch_a188da, sizeof(s_patch_a188da));
        memcpy(image + 0x00A1890A, s_patch_a1890a, sizeof(s_patch_a1890a));
        installed += 3;
        printf("[U2 Engine] Applied patches at 0x00A188CC/DA/90A: Skip unallocated table in ScriptManager to prevent divide by zero\n");
        fflush(stdout);
    }

    /* Patch 0x00A17C60 (50 bytes) & 0x00A19601 (49 bytes): Fallback chunk tracking node allocation.
     * Note: At 0x00A17C27 (rax==0) and 0x00A17C37 (ecx==0), original code sets %r13 = 0 (while %rax
     * still holds pAllocStack!) and jumps to 0x00A17C63 (after `0x00A17C60: movq %rax, %r13`).
     * Passing %rax instead of %r13 corrupted pAllocStack by inserting it into the package list 0x1534140!
     * We preserve `movq %rax, %r13` at 0x00A17C60 (3 bytes) and invoke `u2_alloc_chunk_node_fallback(%r13)`
     * starting at 0x00A17C63 so both the fallthrough path and the jump paths at 0x00A17C27/0x00A17C37
     * pass %r13 cleanly. Same exact pattern for 0x00A19601 (`movq %rax, %r14`) at 0x00A19604.
     */
    if (image_size > 0x00A17C60 + 50) {
        uint8_t patch_a17c60[50];
        memset(patch_a17c60, 0x90, sizeof(patch_a17c60));

        uint8_t *p = patch_a17c60;
        p[0] = 0x49; p[1] = 0x89; p[2] = 0xc5; /* 0x00A17C60: movq %rax, %r13 */
        p[3] = 0x4c; p[4] = 0x89; p[5] = 0xef; /* 0x00A17C63: movq %r13, %rdi */
        p[6] = 0x48; p[7] = 0xb8;              /* 0x00A17C66: movabsq $fn, %rax */
        uint64_t fn_addr = (uint64_t)(uintptr_t)u2_alloc_chunk_node_fallback;
        memcpy(p + 8, &fn_addr, sizeof(fn_addr));
        p[16] = 0xff; p[17] = 0xd0;            /* 0x00A17C70: callq *%rax */
        p[18] = 0x49; p[19] = 0x89; p[20] = 0xc5; /* 0x00A17C72: movq %rax, %r13 */
        p[21] = 0x4d; p[22] = 0x8d; p[23] = 0x75; p[24] = 0x18; /* 0x00A17C75: leaq 0x18(%r13), %r14 */
        /* remaining bytes p[25..49] are 0x90 (NOP) */

        memcpy(image + 0x00A17C60, patch_a17c60, sizeof(patch_a17c60));

        installed++;
        printf("[U2 Engine] Applied patch at 0x00A17C60: Fallback chunk node allocation (preserving %%r13)\n");
        fflush(stdout);
    }

    if (image_size > 0x00A19601 + 49) {
        uint8_t patch_a19601[49];
        memset(patch_a19601, 0x90, sizeof(patch_a19601));

        uint8_t *p = patch_a19601;
        p[0] = 0x49; p[1] = 0x89; p[2] = 0xc6; /* 0x00A19601: movq %rax, %r14 */
        p[3] = 0x4c; p[4] = 0x89; p[5] = 0xf7; /* 0x00A19604: movq %r14, %rdi */
        p[6] = 0x48; p[7] = 0xb8;              /* 0x00A19607: movabsq $fn, %rax */
        uint64_t fn_addr = (uint64_t)(uintptr_t)u2_alloc_chunk_node_fallback;
        memcpy(p + 8, &fn_addr, sizeof(fn_addr));
        p[16] = 0xff; p[17] = 0xd0;            /* 0x00A19611: callq *%rax */
        p[18] = 0x49; p[19] = 0x89; p[20] = 0xc6; /* 0x00A19613: movq %rax, %r14 */
        p[21] = 0x4d; p[22] = 0x8d; p[23] = 0x6e; p[24] = 0x18; /* 0x00A19616: leaq 0x18(%r14), %r13 */
        /* remaining bytes p[25..48] are 0x90 (NOP) */

        memcpy(image + 0x00A19601, patch_a19601, sizeof(patch_a19601));

        installed++;
        printf("[U2 Engine] Applied patch at 0x00A19601: Fallback chunk node allocation (preserving %%r14)\n");
        fflush(stdout);
    }

    /* Patch 0x00B33130 (39 bytes): Safe arena dereference and fallback in AllocByAllocatorId twin (0x00B33020).
     * Sister implementation of 0x00B33160, calls into arena vtable[0x48/8].
     */
    if (image_size > 0x00B33130 + 39) {
        uint8_t patch_b33130[39];
        memset(patch_b33130, 0x90, sizeof(patch_b33130));

        uint8_t *p = patch_b33130;
        p[0] = 0x48; p[1] = 0x89; p[2] = 0xdf; /* movq %rbx, %rdi */
        p[3] = 0x4c; p[4] = 0x89; p[5] = 0xf6; /* movq %r14, %rsi */
        p[6] = 0x48; p[7] = 0xb8;              /* movabsq $fn, %rax */
        uint64_t fn_addr = (uint64_t)(uintptr_t)u2_alloc_arena_or_fallback;
        memcpy(p + 8, &fn_addr, sizeof(fn_addr));
        p[16] = 0xff; p[17] = 0xd0;            /* callq *%rax */
        p[18] = 0x5b;                          /* popq %rbx */
        p[19] = 0x41; p[20] = 0x5c;            /* popq %r12 */
        p[21] = 0x41; p[22] = 0x5e;            /* popq %r14 */
        p[23] = 0x41; p[24] = 0x5f;            /* popq %r15 */
        p[25] = 0x5d;                          /* popq %rbp */
        p[26] = 0xc3;                          /* retq */

        memcpy(image + 0x00B33130, patch_b33130, sizeof(patch_b33130));
        installed++;
        printf("[U2 Engine] Applied patch at 0x00B33130: AllocByAllocatorId twin safe arena deref & fallback\n");
        fflush(stdout);
    }

    /* Patch 0x00B33270 (39 bytes): Safe arena dereference and fallback in AllocByAllocatorId (0x00B33160).
     * In retail builds, some heap arenas (such as allocator 0x12 used by ScriptManager package tables)
     * are uninitialized or NULL in g_HeapArenas (0x0104AE00).
     * The original code at 0x00B33270 blindly read 0(%rdi) (where rdi = *(%rbx)), causing
     * Fault: read access to 0000000000000000 at guest offset 0xb33273.
     * We replace the 39-byte block (0x00B33270..0x00B33296) with a call to u2_alloc_arena_or_fallback(%rbx, %r14),
     * which verifies the arena pointer, calls Arena::Allocate if valid, and falls back to
     * a zeroed 16-byte aligned host buffer if NULL or out of memory.
     */
    if (image_size > 0x00B33270 + 39) {
        uint8_t patch_b33270[39];
        memset(patch_b33270, 0x90, sizeof(patch_b33270));

        uint8_t *p = patch_b33270;
        p[0] = 0x48; p[1] = 0x89; p[2] = 0xdf; /* movq %rbx, %rdi */
        p[3] = 0x4c; p[4] = 0x89; p[5] = 0xf6; /* movq %r14, %rsi */
        p[6] = 0x48; p[7] = 0xb8;              /* movabsq $fn, %rax */
        uint64_t fn_addr = (uint64_t)(uintptr_t)u2_alloc_arena_or_fallback;
        memcpy(p + 8, &fn_addr, sizeof(fn_addr));
        p[16] = 0xff; p[17] = 0xd0;            /* callq *%rax */
        p[18] = 0x5b;                          /* popq %rbx */
        p[19] = 0x41; p[20] = 0x5c;            /* popq %r12 */
        p[21] = 0x41; p[22] = 0x5e;            /* popq %r14 */
        p[23] = 0x41; p[24] = 0x5f;            /* popq %r15 */
        p[25] = 0x5d;                          /* popq %rbp */
        p[26] = 0xc3;                          /* retq */
        /* remaining 12 bytes p[27..38] are 0x90 (NOP) */

        memcpy(image + 0x00B33270, patch_b33270, sizeof(patch_b33270));
        installed++;
        printf("[U2 Engine] Applied patch at 0x00B33270: AllocByAllocatorId safe arena deref & fallback\n");
        fflush(stdout);
    }

    /* Patch 0x00C2C2DE & 0x00C2C31D: Guard PopAllocator in JsonParseFiles (0x00C2C230)
     * When pAllocStack is NULL or stack count is 0, skip to 0x00C2C392 to safely return
     * the parsed JSON root (%r14). This enables the engine to load ingame-shaders.json
     * and register all vertex, pixel, and compute shaders.
     */
    if (image_size > 0x00C2C31D + 2) {
        static const uint8_t s_patch_c2c2de[2] = { 0x74, 0xb2 }; /* je 0xc2c392 */
        static const uint8_t s_patch_c2c31d[2] = { 0x74, 0x73 }; /* je 0xc2c392 */
        memcpy(image + 0x00C2C2DE, s_patch_c2c2de, sizeof(s_patch_c2c2de));
        memcpy(image + 0x00C2C31D, s_patch_c2c31d, sizeof(s_patch_c2c31d));
        installed += 2;
        printf("[U2 Engine] Applied patches at 0x00C2C2DE/0x00C2C31D: Guard PopAllocator in JsonParseFiles (full shader loading enabled)\n");
        fflush(stdout);
    }

    /* Patch 0x00A62C5C & 0x00A634FC: Bypass vtable[0xc0] abort in IntroState::Advance routines
     * NOP out the conditional jump after callq *0xc0(%rax) so the state machine successfully
     * posts transition event 0x153e2b8 (or 0x153e2e0) into StateManager.
     */
    if (image_size > 0x00A634FC + 2) {
        static const uint8_t s_nop2[2] = { 0x90, 0x90 };
        memcpy(image + 0x00A62C5C, s_nop2, sizeof(s_nop2));
        memcpy(image + 0x00A634FC, s_nop2, sizeof(s_nop2));
        installed += 2;
        printf("[U2 Engine] Applied patches at 0x00A62C5C/0x00A634FC: Bypass IntroState transition abort\n");
        fflush(stdout);
    }

    /* Patch 0x00A55229 & 0x00A5524E: Bypass intro movie wait loop in native IntroState::Update
     * 0x00A55229: NOP 'je 0xa55261' so execution always proceeds to the AdvanceToTitle call.
     * 0x00A5524E: NOP 'jl 0xa5525c' so it does not wait 300 frames before calling AdvanceToTitle (*0x158(%rax)).
     */
    if (image_size > 0x00A5524E + 2) {
        static const uint8_t s_nop2[2] = { 0x90, 0x90 };
        memcpy(image + 0x00A55229, s_nop2, sizeof(s_nop2));
        memcpy(image + 0x00A5524E, s_nop2, sizeof(s_nop2));
        installed += 2;
        printf("[U2 Engine] Applied patches at 0x00A55229/0x00A5524E: Immediate IntroState transition\n");
        fflush(stdout);
    }

    /* Patch 0x00BDF9B0: Bypass missing files.json in ingame-shaders Init (0x00BDF9B0)
     * In retail builds, files.json does not exist. The devkit manifest parsing routine
     * only populates the devkit shader table at 0x0124A208 (which has 0 entries in retail).
     * Bypassing it directly with 'retq' (0xC3) avoids allocating 0x588 bytes on the stack,
     * avoids calling PopAllocator without a prior PushAllocator, and eliminates stack protector corruption.
     */
    if (image_size > 0x00BDF9B0) {
        image[0x00BDF9B0] = 0xC3; /* retq */
        installed += 1;
        printf("[U2 Engine] Applied patch 0x00BDF9B0: IngameShaders::Init -> retq (bypass devkit files.json)\n");
        fflush(stdout);
    }

    /* Patch at 0x00CC103A (7 bytes):
     * In retail builds, the devkit shader table at 0x0124A208 has count 0 and files.json does not exist.
     * Function 0x00CC1010 attempts to allocate a 456-byte (0x1C8) devkit shader query table into the
     * "Menu" heap (allocator 0x13). Because the Menu heap is ~2MB and already saturated by loaded fonts
     * (240 bytes remaining), HeapArena::Allocate fails, returns NULL, and memset(NULL, 0, 456) at 0x00CC1068
     * crashes immediately with a write access violation at 0x20.
     * Furthermore, the 19-iteration loop (0x00CC1084..0x00CC12D3) only searches the empty devkit table
     * and writes NULLs.
     * By patching 0x00CC103A with an unconditional jump directly to 0x00CC12D9 (jmp +0x29A), we completely
     * eliminate the 456-byte Menu heap allocation, prevent the out-of-memory crash, bypass the empty loop,
     * and proceed directly to valid constant buffer and render state initialization at 0x00CC12D9.
     */
    if (image_size > 0x00CC103A + 7) {
        /* 0x00CC103A: e9 9a 02 00 00 90 90 (jmp 0x00CC12D9 ; nop ; nop) */
        static const uint8_t s_patch_cc103a[7] = { 0xe9, 0x9a, 0x02, 0x00, 0x00, 0x90, 0x90 };
        memcpy(image + 0x00CC103A, s_patch_cc103a, sizeof(s_patch_cc103a));
        installed++;
        printf("[U2 Engine] Applied patch at 0x00CC103A: Bypass devkit shader 456B Menu heap allocation and jump to 0x00CC12D9\n");
        fflush(stdout);
    }

    /* Patches at 0x00CC10AB, 0x00CC114F, 0x00CC115E, 0x00CC1200:
     * In retail builds, the devkit shader table at 0x0124A208 has count 0.
     * When searching for shaders, bypass the 8-line HALT assertion print banner and int $0x41
     * traps by jumping directly to the NULL-initialization of r15 / r14 and continuing.
     */
    if (image_size > 0x00CC1200 + 2) {
        static const uint8_t s_patch_cc10ab[2] = { 0x74, 0x97 }; /* je 0xcc1144 */
        static const uint8_t s_nop2[2]         = { 0x90, 0x90 }; /* nop nop */
        static const uint8_t s_patch_cc115e[2] = { 0x74, 0x95 }; /* je 0xcc11f5 */
        memcpy(image + 0x00CC10AB, s_patch_cc10ab, sizeof(s_patch_cc10ab));
        memcpy(image + 0x00CC114F, s_nop2, sizeof(s_nop2));
        memcpy(image + 0x00CC115E, s_patch_cc115e, sizeof(s_patch_cc115e));
        memcpy(image + 0x00CC1200, s_nop2, sizeof(s_nop2));
        installed += 4;
        printf("[U2 Engine] Applied patches at 0x00CC10AB/114F/115E/1200: Silence devkit shader halt spam\n");
        fflush(stdout);
    }

    /*
     * Patch at 0x00C81922: Silence GetCsShaderByNameId (Compute Shader) devkit halt spam and traps.
     * When devkit shader table 0x0124A208 has 0 compute shaders, bypass the 8-line HALT banner and
     * int $0x41 trap for all 100+ compute shader queries in the 0xDF1320 loop by setting rcx = 0
     * and jumping straight to storing NULL and incrementing rbx at 0x00C8199B.
     */
    if (image_size > 0x00C81922 + 5) {
        static const uint8_t s_patch_c81922[5] = {
            0x31, 0xC9,             /* xorl %ecx, %ecx */
            0xEB, 0x75,             /* jmp 0x00C8199B (disp = 0x75) */
            0x90                    /* nop */
        };
        memcpy(image + 0x00C81922, s_patch_c81922, sizeof(s_patch_c81922));
        installed++;
        printf("[U2 Engine] Applied patch at 0x00C81922: Silence GetCsShaderByNameId halt spam\n");
        fflush(stdout);
    }

    /*
     * Silence IngameShaders 190-element devkit shader lookup loops (VS at 0x00C81A16, PS at 0x00C81AE6, GS at 0x00C81BB6).
     * In retail builds, devkit shader table 0x0124A208 has 0 entries.
     * Each of the 190 entries triggers HALT banners and int $0x41 for VS, PS, and GS (570 traps total).
     * Bypassing each failure path sets r14/r13/rax to 0 and cleanly jumps to the next shader/iteration.
     */
    if (image_size > 0x00C81BB6 + 10) {
        static const uint8_t s_vs_fail[10] = {
            0x45, 0x31, 0xF6,                         /* xor %r14d, %r14d */
            0xE9, 0x82, 0x00, 0x00, 0x00,             /* jmp 0x00C81AA0 (disp = 0x82) */
            0x90, 0x90                                /* 2x nop */
        };
        static const uint8_t s_ps_fail[10] = {
            0x45, 0x31, 0xED,                         /* xor %r13d, %r13d */
            0xE9, 0x82, 0x00, 0x00, 0x00,             /* jmp 0x00C81B70 (disp = 0x82) */
            0x90, 0x90                                /* 2x nop */
        };
        static const uint8_t s_gs_fail[10] = {
            0x31, 0xC0,                               /* xor %eax, %eax */
            0xE9, 0x83, 0x00, 0x00, 0x00,             /* jmp 0x00C81C40 (disp = 0x83) */
            0x90, 0x90, 0x90                          /* 3x nop */
        };
        memcpy(image + 0x00C81A16, s_vs_fail, sizeof(s_vs_fail));
        memcpy(image + 0x00C81AE6, s_ps_fail, sizeof(s_ps_fail));
        memcpy(image + 0x00C81BB6, s_gs_fail, sizeof(s_gs_fail));
        installed += 3;
        printf("[U2 Engine] Applied patches at 0x00C81A16/AE6/BB6: Silence 190x VS/PS/GS shader lookup halts\n");
        fflush(stdout);
    }

    /*
     * ADR-014: Silence CubemapSet::Init devkit shader halt spam and traps (0x00B9239F..0x00B928AE).
     * In retail builds, devkit shader table 0x0124A208 has 0 entries for CubemapVS, CubePreviewPS,
     * SpecularProbeVS, SpecularProbePS, SpecularProbeUnboundedVS, SpecularProbeUnboundedPS.
     * When count is 0, each lookup prints an 8-line HALT banner and triggers int $0x41 before setting NULL.
     * We patch each failure branch to directly set r15/rax = NULL and jump to the next shader/epilogue.
     * Note: Shader 6 must jump to 0x00B9287E so that rbx is loaded with the stack canary address (0x104ac00).
     */
    if (image_size > 0x00B928B2) {
        static const uint8_t s_cubemap_vs[5]      = { 0x45, 0x31, 0xFF, 0xEB, 0x60 }; /* xor %r15d,%r15d; jmp 0x00B92404 */
        static const uint8_t s_cube_preview_ps[4] = { 0x31, 0xC0, 0xEB, 0x61 };       /* xor %eax,%eax;   jmp 0x00B924E4 */
        static const uint8_t s_spec_probe_vs[5]   = { 0x45, 0x31, 0xFF, 0xEB, 0x60 }; /* xor %r15d,%r15d; jmp 0x00B925E4 */
        static const uint8_t s_spec_probe_ps[4]   = { 0x31, 0xC0, 0xEB, 0x61 };       /* xor %eax,%eax;   jmp 0x00B926C4 */
        static const uint8_t s_spec_unb_vs[5]     = { 0x45, 0x31, 0xFF, 0xEB, 0x60 }; /* xor %r15d,%r15d; jmp 0x00B927C4 */
        static const uint8_t s_spec_unb_ps[4]     = { 0x31, 0xC0, 0xEB, 0x3B };       /* xor %eax,%eax;   jmp 0x00B9287E */
        memcpy(image + 0x00B9239F, s_cubemap_vs, sizeof(s_cubemap_vs));
        memcpy(image + 0x00B9247F, s_cube_preview_ps, sizeof(s_cube_preview_ps));
        memcpy(image + 0x00B9257F, s_spec_probe_vs, sizeof(s_spec_probe_vs));
        memcpy(image + 0x00B9265F, s_spec_probe_ps, sizeof(s_spec_probe_ps));
        memcpy(image + 0x00B9275F, s_spec_unb_vs, sizeof(s_spec_unb_vs));
        memcpy(image + 0x00B9283F, s_spec_unb_ps, sizeof(s_spec_unb_ps));
        installed += 6;
        printf("[U2 Engine] Applied ADR-014 patches at 0x00B9239F..0x00B9283F: Silence CubemapSet shader halt spam\n");
        fflush(stdout);
    }

    /*
     * ADR-015: PlayGo Chunk Availability Bypass for IsChunkSetEntirelyWithin (0x00B77AA0).
     * On PC, all game files and language chunks are local. PlayGo download streaming is unnecessary.
     * Returning 1 (true) informs all callers (LanguageManager, LevelLoader, etc.) that the requested
     * chunk set is fully available, eliminating the "No languages are available!" assertion trap (0x00CDCFD6)
     * and chunk availability wait stalls.
     */
    if (image_size > 0x00B77AA0 + 3) {
        static const uint8_t s_chunk_within_true[3] = { 0xB0, 0x01, 0xC3 }; /* movb $1, %al ; retq */
        memcpy(image + 0x00B77AA0, s_chunk_within_true, sizeof(s_chunk_within_true));
        installed++;
        printf("[U2 Engine] Applied ADR-015 patch at 0x00B77AA0: IsChunkSetEntirelyWithin -> return true (all chunks local)\n");
        fflush(stdout);
    }

    /*
     * ADR-016: Background Package Streaming Unblock in PackageMgr::ReleaseLoadedVramPages (0x00B6CD10).
     * In retail PS4 builds, packages are streamed asynchronously via the Blu-ray / HDD background thread.
     * When LevelManager / Level::Load requests packages, ReleaseLoadedVramPages enters a loop at 0x00B6CE40
     * polling package status (0x20(%rbx)) and waiting for status 0xC (12 = fully loaded).
     * On PC, files are already local; without the background PS4 streaming thread advancing package status,
     * the worker thread running Job 2 (GameInitInternal) stalls indefinitely in usleep(1000) and halts main loop startup.
     * We patch 0x00B6CE68: if ecx != 0, immediately set 0x20(%rbx) = 0xC, ecx = 0xC, and jump to 0x00B6CF81.
     * This marks the package loaded, cleans up PackageMgr internal state, returns the package pointer (%rbx),
     * and lets Job 2 complete cleanly so the engine enters AppMainLoop.
     */
    if (image_size > 0x00B6CE68 + 26) {
        static const uint8_t s_pkg_unblock[26] = {
            0x85, 0xC9,                                     /* testl %ecx, %ecx */
            0x0F, 0x84, 0x11, 0x01, 0x00, 0x00,             /* je 0x00B6CF81 (disp = 0x111) */
            0xC7, 0x43, 0x20, 0x0C, 0x00, 0x00, 0x00,       /* movl $0x0c, 0x20(%rbx) */
            0xB9, 0x0C, 0x00, 0x00, 0x00,                   /* movl $0x0c, %ecx */
            0xE9, 0x00, 0x01, 0x00, 0x00,                   /* jmp 0x00B6CF81 (disp = 0x100) */
            0x90                                            /* nop */
        };
        memcpy(image + 0x00B6CE68, s_pkg_unblock, sizeof(s_pkg_unblock));
        installed++;
        printf("[U2 Engine] Applied ADR-016 patch at 0x00B6CE68: Unblock PackageMgr background pak streaming\n");
        fflush(stdout);
    }

    /*
     * ADR-017: Guard NULL Pointer in Async Queue / Channel Atomic Clear (0x001335D0, 0x001CFF00..0x001D6750).
     * Across the audio voice and async streaming channel managers, 15 inlined routines attempt an
     * atomic clear of bit 0x40 on *(rdx) via a 17-byte cmpxchgl loop (8B 0A 8B 32 83 E6 BF 89 C8 F0 0F B1 32 39 C1 75 EF).
     * When a channel or completion queue is unlinked (rdx == NULL), movl (%rdx), %ecx crashes with an
     * immediate read access violation at 0x0 (0xc0000005).
     * We replace all 15 occurrences with an atomic NULL guard:
     *   testq %rdx, %rdx ; jz +12 ; lock andl $-0x41, (%rdx) ; 8-byte nop
     * which safely skips NULL pointers and continues to the channel loop / function epilogue.
     */
    static const uint32_t s_atomic_clear_sites[] = {
        0x001335D0, 0x001CFF00, 0x001D03D0, 0x001D0C00, 0x001D13E0,
        0x001D1C50, 0x001D24A0, 0x001D2C60, 0x001D2FE0, 0x001D30B0,
        0x001D3820, 0x001D3CC0, 0x001D4BF0, 0x001D5C20, 0x001D6750
    };
    static const uint8_t s_orig_atomic_clear[17] = {
        0x8B, 0x0A, 0x8B, 0x32, 0x83, 0xE6, 0xBF, 0x89, 0xC8,
        0xF0, 0x0F, 0xB1, 0x32, 0x39, 0xC1, 0x75, 0xEF
    };
    static const uint8_t s_guard_atomic_clear[17] = {
        0x48, 0x85, 0xD2,                               /* testq %rdx, %rdx */
        0x74, 0x0C,                                     /* jz +12 (skip to next instruction) */
        0xF0, 0x83, 0x22, 0xBF,                         /* lock andl $-0x41, (%rdx) */
        0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00  /* 8-byte nop */
    };
    for (size_t i = 0; i < sizeof(s_atomic_clear_sites)/sizeof(s_atomic_clear_sites[0]); ++i) {
        uint32_t site = s_atomic_clear_sites[i];
        if (image_size > site + 17 && memcmp(image + site, s_orig_atomic_clear, 17) == 0) {
            memcpy(image + site, s_guard_atomic_clear, sizeof(s_guard_atomic_clear));
            installed++;
        }
    }
    /* Site 16 uses %rdi: 0x001377E0 */
    if (image_size > 0x001377E0 + 17) {
        static const uint8_t s_orig_1377e0[17] = {
            0x8B, 0x0F, 0x8B, 0x37, 0x83, 0xE6, 0xBF, 0x89, 0xC8,
            0xF0, 0x0F, 0xB1, 0x37, 0x39, 0xC1, 0x75, 0xEF
        };
        static const uint8_t s_guard_1377e0[17] = {
            0x48, 0x85, 0xFF,                               /* testq %rdi, %rdi */
            0x74, 0x0C,                                     /* jz +12 (skip to next instruction) */
            0xF0, 0x83, 0x27, 0xBF,                         /* lock andl $-0x41, (%rdi) */
            0x0F, 0x1F, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00  /* 8-byte nop */
        };
        if (memcmp(image + 0x001377E0, s_orig_1377e0, 17) == 0) {
            memcpy(image + 0x001377E0, s_guard_1377e0, sizeof(s_guard_1377e0));
            installed++;
        }
    }
    printf("[U2 Engine] Applied ADR-017 patches across 16 sites (0x001335D0..0x001D6750): Guard NULL async channel pointers\n");
    fflush(stdout);

    /*
     * ADR-018: Guard NULL Component Pointer in DCLoader Component Search (0x0055524B).
     * At 0x0055524B, the routine fetches rdi = *(rsi + 0x68) and immediately reads movzwl 0x2(%rdi), %esi.
     * While the sibling branch at 0x00555208 checks 'testq %rdi, %rdi ; je 0x555282', the 0x68 branch
     * omitted the NULL check, crashing with read access violation at 0x2 (0xc0000005) when rdi == NULL.
     * We install a 30-byte trampoline at code cave 0x00DE7E00:
     *   movq 0x68(%rsi), %rdi ; testq %rdi, %rdi ; jz 0x00555282 ;
     *   movzwl 0x2(%rdi), %esi ; testl %esi, %esi ; jz 0x00555282 ; jmp 0x00555257
     * and hook 0x0055524B with 'jmp 0x00DE7E00 ; 7x nop'.
     */
    if (image_size > 0x00DE7E00 + 30) {
        static const uint8_t s_cave_55524b[30] = {
            0x48, 0x8B, 0x7E, 0x68,                         /* movq 0x68(%rsi), %rdi */
            0x48, 0x85, 0xFF,                               /* testq %rdi, %rdi */
            0x0F, 0x84, 0x75, 0xD4, 0x76, 0xFF,             /* jz 0x00555282 */
            0x0F, 0xB7, 0x77, 0x02,                         /* movzwl 0x2(%rdi), %esi */
            0x85, 0xF6,                                     /* testl %esi, %esi */
            0x0F, 0x84, 0x69, 0xD4, 0x76, 0xFF,             /* jz 0x00555282 */
            0xE9, 0x39, 0xD4, 0x76, 0xFF                    /* jmp 0x00555257 */
        };
        static const uint8_t s_hook_55524b[12] = {
            0xE9, 0xB0, 0x2B, 0x89, 0x00,                   /* jmp 0x00DE7E00 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90        /* 7x nop */
        };
        memcpy(image + 0x00DE7E00, s_cave_55524b, sizeof(s_cave_55524b));
        memcpy(image + 0x0055524B, s_hook_55524b, sizeof(s_hook_55524b));
        installed++;
        printf("[U2 Engine] Applied ADR-018 patch at 0x0055524B: Guard NULL component pointer in DCLoader\n");
        fflush(stdout);
    }

    /*
     * ADR-019: Guard NULL Input/Output Buffers in Audio Matrix Transform (0x0013D878).
     * At 0x001E51C6, the caller explicitly tests 0x4(%rsi) and 0x4(%rdx), passing rdi = NULL
     * and/or rsi = NULL into 0x0013D850 when channels are unallocated.
     * At 0x0013D8C8, the routine blindly executed 'vmulps (%rdi,%rcx,4), %xmm0, %xmm5'
     * and 'vaddps (%rsi,%rcx,4), ...', crashing with read access violation at 0x0.
     * We install a 31-byte trampoline at code cave 0x00DE7E20:
     *   testq %rdi, %rdi ; jz 0x0013D923 ; testq %rsi, %rsi ; jz 0x0013D923 ;
     *   testl %edx, %edx ; jz 0x0013D923 ; jmp 0x0013D880
     * and hook 0x0013D878 with 'jmp 0x00DE7E20 ; 3x nop'.
     */
    /*
     * ADR-019: Guard NULL audio matrix transform buffers at function entry (0x0013D850).
     * The audio transform function 0x0013D850 processes audio buffers (rdi = dst, rsi = src, edx = count).
     * It contains two processing loops (0x0013D878 and 0x0013D960). When called with NULL buffers,
     * it crashes in AVX loops (e.g. 0x0013D978 vmulps (%rdi,%rcx,4) with rdi=0).
     * We install a 28-byte trampoline at code cave 0x00DE7E20 at the function entry point:
     *   testq %rdi, %rdi ; jz ret ; testq %rsi, %rsi ; jz ret ; testl %edx, %edx ; jz ret ;
     *   vxorps %xmm2, %xmm2, %xmm2 ; vucomiss %xmm2, %xmm1 ; jmp 0x0013D858 ; retq
     * and hook 0x0013D850 with 'jmp 0x00DE7E20 ; 3x nop'.
     */
    if (image_size > 0x00DE7E20 + 28) {
        static const uint8_t s_cave_13d850[28] = {
            0x48, 0x85, 0xFF,                               /* testq %rdi, %rdi */
            0x74, 0x16,                                     /* jz ret (+22 -> 0x1B) */
            0x48, 0x85, 0xF6,                               /* testq %rsi, %rsi */
            0x74, 0x11,                                     /* jz ret (+17 -> 0x1B) */
            0x85, 0xD2,                                     /* testl %edx, %edx */
            0x74, 0x0D,                                     /* jz ret (+13 -> 0x1B) */
            0xC5, 0xE8, 0x57, 0xD2,                         /* vxorps %xmm2, %xmm2, %xmm2 */
            0xC5, 0xF8, 0x2E, 0xCA,                         /* vucomiss %xmm2, %xmm1 */
            0xE9, 0x1D, 0x5A, 0x35, 0xFF,                   /* jmp 0x0013D858 */
            0xC3                                            /* retq */
        };
        static const uint8_t s_hook_13d850[8] = {
            0xE9, 0xCB, 0xA5, 0xCA, 0x00,                   /* jmp 0x00DE7E20 */
            0x90, 0x90, 0x90                                /* 3x nop */
        };
        memcpy(image + 0x00DE7E20, s_cave_13d850, sizeof(s_cave_13d850));
        memcpy(image + 0x0013D850, s_hook_13d850, sizeof(s_hook_13d850));
        installed++;
        printf("[U2 Engine] Applied ADR-019 patch at 0x0013D850: Guard NULL audio matrix transform buffers\n");
        fflush(stdout);
    }

    /*
     * ADR-020: Guard NULL Allocator Pointer in Audio Arena Suballocator (0x0013D180).
     * At 0x0013D186, the arena allocator reads movl 0x28(%rdi), %esi without verifying rdi.
     * When called with rdi == NULL, it crashes with read access violation at 0x28 (0xc0000005).
     * We install a 22-byte trampoline at code cave 0x00DE7E40:
     *   testq %rdi, %rdi ; jnz +3 ; xorl %eax, %eax ; retq ;
     *   pushq %rbp ; movq %rsp, %rbp ; movl %esi, %eax ; movl 0x28(%rdi), %esi ; jmp 0x0013D189
     * and hook 0x0013D180 with 'jmp 0x00DE7E40 ; 4x nop'.
     */
    if (image_size > 0x00DE7E40 + 22) {
        static const uint8_t s_cave_13d180[22] = {
            0x48, 0x85, 0xFF,                               /* testq %rdi, %rdi */
            0x75, 0x03,                                     /* jnz +3 */
            0x31, 0xC0,                                     /* xorl %eax, %eax */
            0xC3,                                           /* retq */
            0x55,                                           /* pushq %rbp */
            0x48, 0x89, 0xE5,                               /* movq %rsp, %rbp */
            0x89, 0xF0,                                     /* movl %esi, %eax */
            0x8B, 0x77, 0x28,                               /* movl 0x28(%rdi), %esi */
            0xE9, 0x33, 0x53, 0x35, 0xFF                    /* jmp 0x0013D189 */
        };
        static const uint8_t s_hook_13d180[9] = {
            0xE9, 0xBB, 0xAC, 0xCA, 0x00,                   /* jmp 0x00DE7E40 */
            0x90, 0x90, 0x90, 0x90                          /* 4x nop */
        };
        memcpy(image + 0x00DE7E40, s_cave_13d180, sizeof(s_cave_13d180));
        memcpy(image + 0x0013D180, s_hook_13d180, sizeof(s_hook_13d180));
        installed++;
        printf("[U2 Engine] Applied ADR-020 patch at 0x0013D180: Guard NULL audio arena allocator\n");
        fflush(stdout);
    }

    /*
     * ADR-021: Guard NULL Object / Pointer Dereference in AllocateOrbisObject (0x00C0EE01).
     * In ndgi-orbis-device.cpp at 0x00C0EDFD, the routine reads 0x30(%rbp) into %rax,
     * tests rax for NULL, and then immediately reads movq (%rax), %rcx followed by movl 0x30(%rcx), %edx.
     * When 0x30(%rbp) points to a NULL descriptor pointer (%rcx == NULL), it crashed with
     * Fault: read access to 0000000000000030 at guest offset 0xc0ee11.
     * We install a 33-byte trampoline at code cave 0x00FE2C00:
     *   testq %rax, %rax ; jz 0x00C0EE9A ;
     *   movq (%rax), %rcx ; testq %rcx, %rcx ; jz 0x00C0EE9A ;
     *   movq %r14, -0x80(%rbp) ; movl 0x30(%rcx), %edx ; jmp 0x00C0EE14
     * and hook 0x00C0EE01 with 'jmp 0x00FE2C00 ; 14x nop'.
     */
    if (image_size > 0x00FE2C00 + 33) {
        static const uint8_t s_cave_c0ee01[33] = {
            0x48, 0x85, 0xC0,                               /* testq %rax, %rax */
            0x0F, 0x84, 0x91, 0xC2, 0xC2, 0xFF,             /* jz 0x00C0EE9A */
            0x48, 0x8B, 0x08,                               /* movq (%rax), %rcx */
            0x48, 0x85, 0xC9,                               /* testq %rcx, %rcx */
            0x0F, 0x84, 0x85, 0xC2, 0xC2, 0xFF,             /* jz 0x00C0EE9A */
            0x4C, 0x89, 0x75, 0x80,                         /* movq %r14, -0x80(%rbp) */
            0x8B, 0x51, 0x30,                               /* movl 0x30(%rcx), %edx */
            0xE9, 0xF3, 0xC1, 0xC2, 0xFF                    /* jmp 0x00C0EE14 */
        };
        static const uint8_t s_hook_c0ee01[19] = {
            0xE9, 0xFA, 0x3D, 0x3D, 0x00,                   /* jmp 0x00FE2C00 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90              /* 14x nop */
        };
        memcpy(image + 0x00FE2C00, s_cave_c0ee01, sizeof(s_cave_c0ee01));
        memcpy(image + 0x00C0EE01, s_hook_c0ee01, sizeof(s_hook_c0ee01));
        installed++;
        printf("[U2 Engine] Applied ADR-021 patch at 0x00C0EE01: Guard NULL descriptor pointer in AllocateOrbisObject\n");
        fflush(stdout);
    }

    /*
     * ADR-022: Guard Invalid/Unallocated Audio Destination Buffer in Audio Mixer Node (0x001E5180).
     * Callers such as 0x001CFF77 compute 'subq $-0x80, %rdi' from 0x20(%r15). When the audio buffer
     * at 0x20(%r15) is unallocated (NULL), rdi becomes 0x80. At 0x001E51E0, the function dereferenced
     * (%rbx) where rbx == 0x80, crashing with read access violation at 0x80 (0xc0000005).
     * We install a 24-byte trampoline at code cave 0x00FE2C30:
     *   cmpq $0x1000, %rdi ; ja +1 ; retq ;
     *   pushq %rbp ; movq %rsp, %rbp ; pushq %rbx ; pushq %rax ; movq %rdi, %rbx ; jmp 0x001E5189
     * and hook 0x001E5180 with 'jmp 0x00FE2C30 ; 4x nop'.
     */
    if (image_size > 0x00FE2C30 + 24) {
        static const uint8_t s_cave_1e5180[24] = {
            0x48, 0x81, 0xFF, 0x00, 0x10, 0x00, 0x00,       /* cmpq $0x1000, %rdi */
            0x77, 0x01,                                     /* ja continue */
            0xC3,                                           /* retq */
            0x55,                                           /* pushq %rbp */
            0x48, 0x89, 0xE5,                               /* movq %rsp, %rbp */
            0x53,                                           /* pushq %rbx */
            0x50,                                           /* pushq %rax */
            0x48, 0x89, 0xFB,                               /* movq %rdi, %rbx */
            0xE9, 0x41, 0x25, 0x20, 0xFF                    /* jmp 0x001E5189 */
        };
        static const uint8_t s_hook_1e5180[9] = {
            0xE9, 0xAB, 0xDA, 0xDF, 0x00,                   /* jmp 0x00FE2C30 */
            0x90, 0x90, 0x90, 0x90                          /* 4x nop */
        };
        memcpy(image + 0x00FE2C30, s_cave_1e5180, sizeof(s_cave_1e5180));
        memcpy(image + 0x001E5180, s_hook_1e5180, sizeof(s_hook_1e5180));
        installed++;
        printf("[U2 Engine] Applied ADR-022 patch at 0x001E5180: Guard unallocated destination buffer in audio mixer\n");
        fflush(stdout);
    }

    /*
     * ADR-023: Guard NULL Destination Buffer in AVX Audio Memset (0x0013D270).
     * At 0x0013D2A0, the AVX-accelerated memset routine executes vmovaps %xmm0, (%rcx)
     * where rcx = rdi. When called from voice processing (0x00137430) with an unallocated
     * voice buffer (rdi == NULL), it crashed with write access violation at 0x0 (0xc0000005).
     * We install a 19-byte trampoline at code cave 0x00FE2C50:
     *   testq %rdi, %rdi ; jnz +1 ; retq ;
     *   vmovd %esi, %xmm0 ; vpunpcklbw %xmm0, %xmm0, %xmm0 ; jmp 0x0013D278
     * and hook 0x0013D270 with 'jmp 0x00FE2C50 ; 3x nop'.
     */
    if (image_size > 0x00FE2C50 + 19) {
        static const uint8_t s_cave_13d270[19] = {
            0x48, 0x85, 0xFF,                               /* testq %rdi, %rdi */
            0x75, 0x01,                                     /* jnz continue */
            0xC3,                                           /* retq */
            0xC5, 0xF9, 0x6E, 0xC6,                         /* vmovd %esi, %xmm0 */
            0xC5, 0xF9, 0x60, 0xC0,                         /* vpunpcklbw %xmm0, %xmm0, %xmm0 */
            0xE9, 0x15, 0xA6, 0x15, 0xFF                    /* jmp 0x0013D278 */
        };
        static const uint8_t s_hook_13d270[8] = {
            0xE9, 0xDB, 0x59, 0xEA, 0x00,                   /* jmp 0x00FE2C50 */
            0x90, 0x90, 0x90                                /* 3x nop */
        };
        memcpy(image + 0x00FE2C50, s_cave_13d270, sizeof(s_cave_13d270));
        memcpy(image + 0x0013D270, s_hook_13d270, sizeof(s_hook_13d270));
        installed++;
        printf("[U2 Engine] Applied ADR-023 patch at 0x0013D270: Guard NULL destination in AVX audio memset\n");
        fflush(stdout);
    }

    /*
     * ADR-024: Guard NULL Voice Buffer in Audio Voice Processor (0x001D4330).
     * At 0x001D4344, the routine loads r13 = *(rdi + 0x20). When the voice buffer is
     * unallocated (NULL), it proceeded into matrix transforms and scratch buffer writes
     * using r13 offsets (0x80, 0x850, 0x90, 0xae0, 0xe00, etc.), crashing with read/write
     * access violation at 0x80.
     * We install a 42-byte trampoline at code cave 0x00FE2C70:
     *   testq %rdi, %rdi ; jz ret ; cmpq $0, 0x20(%rdi) ; jnz continue ; ret: retq ;
     *   continue: push %rbp ; mov %rsp, %rbp ; push r15..r12, rbx, rax ;
     *   mov %rdx, %r14 ; mov %rsi, %r12 ; mov 0x20(%rdi), %r13 ; jmp 0x001D4348
     * and hook 0x001D4330 with 'jmp 0x00FE2C70 ; 19x nop'.
     */
    if (image_size > 0x00FE2C70 + 42) {
        static const uint8_t s_cave_1d4330[42] = {
            0x48, 0x85, 0xFF,                               /* testq %rdi, %rdi */
            0x74, 0x08,                                     /* jz ret */
            0x48, 0x83, 0x7F, 0x20, 0x00,                   /* cmpq $0, 0x20(%rdi) */
            0x75, 0x01,                                     /* jnz continue */
            0xC3,                                           /* retq */
            0x55,                                           /* pushq %rbp */
            0x48, 0x89, 0xE5,                               /* movq %rsp, %rbp */
            0x41, 0x57,                                     /* pushq %r15 */
            0x41, 0x56,                                     /* pushq %r14 */
            0x41, 0x55,                                     /* pushq %r13 */
            0x41, 0x54,                                     /* pushq %r12 */
            0x53,                                           /* pushq %rbx */
            0x50,                                           /* pushq %rax */
            0x49, 0x89, 0xD6,                               /* movq %rdx, %r14 */
            0x49, 0x89, 0xF4,                               /* movq %rsi, %r12 */
            0x4C, 0x8B, 0x6F, 0x20,                         /* movq 0x20(%rdi), %r13 */
            0xE9, 0xAE, 0x16, 0x1F, 0xFF                    /* jmp 0x001D4348 */
        };
        static const uint8_t s_hook_1d4330[24] = {
            0xE9, 0x3B, 0xE9, 0xE0, 0x00,                   /* jmp 0x00FE2C70 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90                                /* 19x nop */
        };
        memcpy(image + 0x00FE2C70, s_cave_1d4330, sizeof(s_cave_1d4330));
        memcpy(image + 0x001D4330, s_hook_1d4330, sizeof(s_hook_1d4330));
        installed++;
        printf("[U2 Engine] Applied ADR-024 patch at 0x001D4330: Guard NULL voice buffer in audio voice processor\n");
        fflush(stdout);
    }

    /*
     * ADR-025: Guard Invalid Destination Buffer in Audio Matrix Processor Sibling (0x001E1DD0).
     * At 0x001E1DE5, the routine moves rdi into r9. When rdi is an invalid/unallocated
     * pointer (rdi <= 0x1000, such as 0x80), it crashes with read access violation at 0x80.
     * We install a 32-byte trampoline at code cave 0x00FE2CB0:
     *   cmpq $0x1000, %rdi ; ja continue ; retq ;
     *   continue: push %rbp ; mov %rsp, %rbp ; push r15..r12, rbx ; sub $0x38, %rsp ; jmp 0x001E1DE1
     * and hook 0x001E1DD0 with 'jmp 0x00FE2CB0 ; 12x nop'.
     */
    if (image_size > 0x00FE2CB0 + 32) {
        static const uint8_t s_cave_1e1dd0[32] = {
            0x48, 0x81, 0xFF, 0x00, 0x10, 0x00, 0x00,       /* cmpq $0x1000, %rdi */
            0x77, 0x01,                                     /* ja continue */
            0xC3,                                           /* retq */
            0x55,                                           /* pushq %rbp */
            0x48, 0x89, 0xE5,                               /* movq %rsp, %rbp */
            0x41, 0x57,                                     /* pushq %r15 */
            0x41, 0x56,                                     /* pushq %r14 */
            0x41, 0x55,                                     /* pushq %r13 */
            0x41, 0x54,                                     /* pushq %r12 */
            0x53,                                           /* pushq %rbx */
            0x48, 0x83, 0xEC, 0x38,                         /* subq $0x38, %rsp */
            0xE9, 0x11, 0xF1, 0x1F, 0xFF                    /* jmp 0x001E1DE1 */
        };
        static const uint8_t s_hook_1e1dd0[17] = {
            0xE9, 0xDB, 0x0E, 0xE0, 0x00,                   /* jmp 0x00FE2CB0 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90                          /* 12x nop */
        };
        memcpy(image + 0x00FE2CB0, s_cave_1e1dd0, sizeof(s_cave_1e1dd0));
        memcpy(image + 0x001E1DD0, s_hook_1e1dd0, sizeof(s_hook_1e1dd0));
        installed++;
        printf("[U2 Engine] Applied ADR-025 patch at 0x001E1DD0: Guard invalid destination in audio matrix processor\n");
        fflush(stdout);
    }

    /*
     * ADR-026: Guard NULL Channel Pointer in Audio Transform Loop (0x0013B093).
     * At 0x0013B093, the routine loads rcx = *(r14 + 0x38) and reads movl 0x4(%rcx), %ecx.
     * When channel list *(r14 + 0x38) is unallocated (NULL), it crashed with read access
     * violation at 0x4 (0xc0000005).
     * We install a 35-byte trampoline at code cave 0x00FE2CD0:
     *   movq 0x38(%r14), %rcx ; testq %rcx, %rcx ; jz skip ;
     *   movl 0x4(%rcx), %ecx ; movq 0x38(%r13), %rbx ; decl %ecx ; jmp 0x0013B0A0 ;
     *   skip: movq %r15, -0x148(%rbp) ; jmp 0x0013B201
     * and hook 0x0013B093 with 'jmp 0x00FE2CD0 ; 8x nop'.
     */
    if (image_size > 0x00FE2CD0 + 35) {
        static const uint8_t s_cave_13b093[35] = {
            0x49, 0x8B, 0x4E, 0x38,                         /* movq 0x38(%r14), %rcx */
            0x48, 0x85, 0xC9,                               /* testq %rcx, %rcx */
            0x74, 0x0E,                                     /* jz skip */
            0x8B, 0x49, 0x04,                               /* movl 0x4(%rcx), %ecx */
            0x49, 0x8B, 0x5D, 0x38,                         /* movq 0x38(%r13), %rbx */
            0xFF, 0xC9,                                     /* decl %ecx */
            0xE9, 0xB9, 0x83, 0x15, 0xFF,                   /* jmp 0x0013B0A0 */
            0x4C, 0x89, 0xBD, 0xB8, 0xFE, 0xFF, 0xFF,       /* movq %r15, -0x148(%rbp) */
            0xE9, 0x0E, 0x85, 0x15, 0xFF                    /* jmp 0x0013B201 */
        };
        static const uint8_t s_hook_13b093[13] = {
            0xE9, 0x38, 0x7C, 0xEA, 0x00,                   /* jmp 0x00FE2CD0 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90  /* 8x nop */
        };
        memcpy(image + 0x00FE2CD0, s_cave_13b093, sizeof(s_cave_13b093));
        memcpy(image + 0x0013B093, s_hook_13b093, sizeof(s_hook_13b093));
        installed++;
        printf("[U2 Engine] Applied ADR-026 patch at 0x0013B093: Guard NULL channel pointer in audio transform loop\n");
        fflush(stdout);
    }

    /*
     * ADR-027: Guard NULL Voice Channel Buffer in Audio Clear Function (0x0013B910).
     * At 0x0013B910, the routine reads rax = 0x38(%rdi) and immediately dereferences
     * movsbq 0x10(%rax), %rax to find the active buffer before calling memset.
     * When rdi == NULL or *(rdi + 0x38) == NULL, it crashed with read access violation
     * at 0x10 (0xc0000005).
     * We install a 42-byte trampoline at code cave 0x00FE2D00:
     *   testq %rdi, %rdi ; jz ret ; movq 0x38(%rdi), %rax ; testq %rax, %rax ; jz ret ;
     *   movsbq 0x10(%rax), %rax ; movq 0x40(%rdi,%rax,8), %rax ; movl 0x4(%rdi), %edx ;
     *   imull 0x1c(%rdi), %edx ; xorl %esi, %esi ; movq %rax, %rdi ; jmp 0x0013D270 ; ret: retq
     * and hook 0x0013B910 with 'jmp 0x00FE2D00 ; 26x nop'.
     */
    if (image_size > 0x00FE2D00 + 42) {
        static const uint8_t s_cave_13b910[42] = {
            0x48, 0x85, 0xFF,                               /* testq %rdi, %rdi */
            0x74, 0x24,                                     /* jz ret */
            0x48, 0x8B, 0x47, 0x38,                         /* movq 0x38(%rdi), %rax */
            0x48, 0x85, 0xC0,                               /* testq %rax, %rax */
            0x74, 0x1B,                                     /* jz ret */
            0x48, 0x0F, 0xBE, 0x40, 0x10,                   /* movsbq 0x10(%rax), %rax */
            0x48, 0x8B, 0x44, 0xC7, 0x40,                   /* movq 0x40(%rdi,%rax,8), %rax */
            0x8B, 0x57, 0x04,                               /* movl 0x4(%rdi), %edx */
            0x0F, 0xAF, 0x57, 0x1C,                         /* imull 0x1c(%rdi), %edx */
            0x31, 0xF6,                                     /* xorl %esi, %esi */
            0x48, 0x89, 0xC7,                               /* movq %rax, %rdi */
            0xE9, 0x47, 0xA5, 0x15, 0xFF,                   /* jmp 0x0013D270 */
            0xC3                                            /* retq */
        };
        static const uint8_t s_hook_13b910[31] = {
            0xE9, 0xEB, 0x73, 0xEA, 0x00,                   /* jmp 0x00FE2D00 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90                                      /* 26x nop */
        };
        memcpy(image + 0x00FE2D00, s_cave_13b910, sizeof(s_cave_13b910));
        memcpy(image + 0x0013B910, s_hook_13b910, sizeof(s_hook_13b910));
        installed++;
        printf("[U2 Engine] Applied ADR-027 patch at 0x0013B910: Guard NULL buffer in audio clear function\n");
        fflush(stdout);
    }

    /*
     * ADR-028: Guard NULL Voice Buffer in Voice Mix Pre-Processor (0x001D3864).
     * At 0x001D3864, the routine loads rbx = *(rdi + 0x20). When the voice buffer is
     * unallocated (NULL), it proceeded to compute sub-buffer pointers at 0x80, 0xb0, 0xc0, 0x6d0
     * passing them to 0x1E0F30 and 0x1EB0C0, resulting in read access violations at 0x154.
     * We install a 25-byte trampoline at code cave 0x00FE2D30:
     *   movq 0x20(%rdi), %rbx ; testq %rbx, %rbx ; jz 0x001D3954 ;
     *   movq %rbx, %rdi ; subq $-0x80, %rdi ; jmp 0x001D386F
     * and hook 0x001D3864 with 'jmp 0x00FE2D30 ; 6x nop'.
     */
    if (image_size > 0x00FE2D30 + 25) {
        static const uint8_t s_cave_1d3864[25] = {
            0x48, 0x8B, 0x5F, 0x20,                         /* movq 0x20(%rdi), %rbx */
            0x48, 0x85, 0xDB,                               /* testq %rbx, %rbx */
            0x0F, 0x84, 0x17, 0x0C, 0x1F, 0xFF,             /* jz 0x001D3954 */
            0x48, 0x89, 0xDF,                               /* movq %rbx, %rdi */
            0x48, 0x83, 0xEF, 0x80,                         /* subq $-0x80, %rdi */
            0xE9, 0x26, 0x0B, 0x1F, 0xFF                    /* jmp 0x001D386F */
        };
        static const uint8_t s_hook_1d3864[11] = {
            0xE9, 0xC7, 0xF4, 0xE0, 0x00,                   /* jmp 0x00FE2D30 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90              /* 6x nop */
        };
        memcpy(image + 0x00FE2D30, s_cave_1d3864, sizeof(s_cave_1d3864));
        memcpy(image + 0x001D3864, s_hook_1d3864, sizeof(s_hook_1d3864));
        installed++;
        printf("[U2 Engine] Applied ADR-028 patch at 0x001D3864: Guard NULL voice buffer in voice mix pre-processor\n");
        fflush(stdout);
    }

    /*
     * ADR-029: Guard Invalid Channel Pointer in Audio Filter Processor (0x001EB0C0).
     * At 0x001EB0D9, the routine dereferences movl 0x94(%rdi), %eax. When rdi <= 0x1000
     * (e.g. 0xc0), it crashed with read access violation at 0x154 (0xc0000005).
     * We install a 25-byte trampoline at code cave 0x00FE2D60:
     *   cmpq $0x1000, %rdi ; ja continue ; retq ;
     *   continue: push %rbp ; push %r15 ; push %r14 ; push %r13 ; push %r12 ; push %rbx ; jmp 0x001EB0CA
     * and hook 0x001EB0C0 with 'jmp 0x00FE2D60 ; 5x nop'.
     */
    if (image_size > 0x00FE2D60 + 25) {
        static const uint8_t s_cave_1eb0c0[25] = {
            0x48, 0x81, 0xFF, 0x00, 0x10, 0x00, 0x00,       /* cmpq $0x1000, %rdi */
            0x77, 0x01,                                     /* ja continue */
            0xC3,                                           /* retq */
            0x55,                                           /* pushq %rbp */
            0x41, 0x57,                                     /* pushq %r15 */
            0x41, 0x56,                                     /* pushq %r14 */
            0x41, 0x55,                                     /* pushq %r13 */
            0x41, 0x54,                                     /* pushq %r12 */
            0x53,                                           /* pushq %rbx */
            0xE9, 0x51, 0x83, 0x20, 0xFF                    /* jmp 0x001EB0CA */
        };
        static const uint8_t s_hook_1eb0c0[10] = {
            0xE9, 0x9B, 0x7C, 0xDF, 0x00,                   /* jmp 0x00FE2D60 */
            0x90, 0x90, 0x90, 0x90, 0x90                    /* 5x nop */
        };
        memcpy(image + 0x00FE2D60, s_cave_1eb0c0, sizeof(s_cave_1eb0c0));
        memcpy(image + 0x001EB0C0, s_hook_1eb0c0, sizeof(s_hook_1eb0c0));
        installed++;
        printf("[U2 Engine] Applied ADR-029 patch at 0x001EB0C0: Guard invalid channel pointer in audio filter processor\n");
        fflush(stdout);
    }

    /*
     * ADR-030: Guard Invalid Channel Pointer in Audio Filter Sibling (0x001E0F30).
     * At 0x001E0F30, the function sets up frame and calls 0x1EB0C0. If rdi <= 0x1000,
     * it returns immediately without running the filtering cascade.
     * We install a 25-byte trampoline at code cave 0x00FE2D90:
     *   cmpq $0x1000, %rdi ; ja continue ; retq ;
     *   continue: push %rbp ; mov %rsp, %rbp ; push %r15 ; push %r14 ; push %r13 ; jmp 0x001E0F3A
     * and hook 0x001E0F30 with 'jmp 0x00FE2D90 ; 5x nop'.
     */
    if (image_size > 0x00FE2D90 + 25) {
        static const uint8_t s_cave_1e0f30[25] = {
            0x48, 0x81, 0xFF, 0x00, 0x10, 0x00, 0x00,       /* cmpq $0x1000, %rdi */
            0x77, 0x01,                                     /* ja continue */
            0xC3,                                           /* retq */
            0x55,                                           /* pushq %rbp */
            0x48, 0x89, 0xE5,                               /* movq %rsp, %rbp */
            0x41, 0x57,                                     /* pushq %r15 */
            0x41, 0x56,                                     /* pushq %r14 */
            0x41, 0x55,                                     /* pushq %r13 */
            0xE9, 0x91, 0xE1, 0x1F, 0xFF                    /* jmp 0x001E0F3A */
        };
        static const uint8_t s_hook_1e0f30[10] = {
            0xE9, 0x5B, 0x1E, 0xE0, 0x00,                   /* jmp 0x00FE2D90 */
            0x90, 0x90, 0x90, 0x90, 0x90                    /* 5x nop */
        };
        memcpy(image + 0x00FE2D90, s_cave_1e0f30, sizeof(s_cave_1e0f30));
        memcpy(image + 0x001E0F30, s_hook_1e0f30, sizeof(s_hook_1e0f30));
        installed++;
        printf("[U2 Engine] Applied ADR-030 patch at 0x001E0F30: Guard invalid channel pointer in audio filter sibling\n");
        fflush(stdout);
    }

    /*
     * ADR-031: Guard NULL Descriptor in Render Target Descriptor Copy (0x00C0047A).
     * At 0x00C0047A in ndgi-orbis-device.cpp, the routine reads rax = *(rbx) and blindly
     * copies 0x38 bytes into rsi (movl 0x30(%rax), %ecx, etc.). When *(rbx) is NULL (rax == NULL),
     * it crashed with read access violation at 0x30 (0xc0000005).
     * We install a 96-byte trampoline at code cave 0x00FE2DC0:
     *   movq (%rbx), %rax ; testq %rax, %rax ; jz zero_rsi ;
     *   <normal copy 0x38 bytes> ; jmp 0x00C004B1 ;
     *   zero_rsi: vxorps %xmm0, %xmm0, %xmm0 ; vmovaps %xmm0, (%rsi) ; vmovaps %xmm0, 0x10(%rsi) ;
     *   vmovaps %xmm0, 0x20(%rsi) ; movq $0, 0x30(%rsi) ; jmp 0x00C004B1
     * and hook 0x00C0047A with 'jmp 0x00FE2DC0 ; 50x nop'.
     */
    if (image_size > 0x00FE2DC0 + 96) {
        static const uint8_t s_cave_c0047a[96] = {
            0x48, 0x8B, 0x03,                               /* movq (%rbx), %rax */
            0x48, 0x85, 0xC0,                               /* testq %rax, %rax */
            0x74, 0x39,                                     /* jz zero_rsi */
            0x8B, 0x48, 0x30,                               /* movl 0x30(%rax), %ecx */
            0x89, 0x4E, 0x30,                               /* movl %ecx, 0x30(%rsi) */
            0x48, 0x8B, 0x48, 0x28,                         /* movq 0x28(%rax), %rcx */
            0x48, 0x89, 0x4E, 0x28,                         /* movq %rcx, 0x28(%rsi) */
            0x48, 0x8B, 0x48, 0x20,                         /* movq 0x20(%rax), %rcx */
            0x48, 0x89, 0x4E, 0x20,                         /* movq %rcx, 0x20(%rsi) */
            0x48, 0x8B, 0x48, 0x18,                         /* movq 0x18(%rax), %rcx */
            0x48, 0x89, 0x4E, 0x18,                         /* movq %rcx, 0x18(%rsi) */
            0x48, 0x8B, 0x48, 0x10,                         /* movq 0x10(%rax), %rcx */
            0x48, 0x89, 0x4E, 0x10,                         /* movq %rcx, 0x10(%rsi) */
            0x48, 0x8B, 0x08,                               /* movq (%rax), %rcx */
            0x48, 0x8B, 0x40, 0x08,                         /* movq 0x8(%rax), %rax */
            0x48, 0x89, 0x46, 0x08,                         /* movq %rax, 0x8(%rsi) */
            0x48, 0x89, 0x0E,                               /* movq %rcx, (%rsi) */
            0xE9, 0xB0, 0xD6, 0xC1, 0xFF,                   /* jmp 0x00C004B1 */
            0xC5, 0xF8, 0x57, 0xC0,                         /* vxorps %xmm0, %xmm0, %xmm0 */
            0xC5, 0xF8, 0x29, 0x06,                         /* vmovaps %xmm0, (%rsi) */
            0xC5, 0xF8, 0x29, 0x46, 0x10,                   /* vmovaps %xmm0, 0x10(%rsi) */
            0xC5, 0xF8, 0x29, 0x46, 0x20,                   /* vmovaps %xmm0, 0x20(%rsi) */
            0x48, 0xC7, 0x46, 0x30, 0x00, 0x00, 0x00, 0x00, /* movq $0, 0x30(%rsi) */
            0xE9, 0x91, 0xD6, 0xC1, 0xFF                    /* jmp 0x00C004B1 */
        };
        static const uint8_t s_hook_c0047a[55] = {
            0xE9, 0x41, 0x29, 0x3E, 0x00,                   /* jmp 0x00FE2DC0 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90                                      /* 50x nop */
        };
        memcpy(image + 0x00FE2DC0, s_cave_c0047a, sizeof(s_cave_c0047a));
        memcpy(image + 0x00C0047A, s_hook_c0047a, sizeof(s_hook_c0047a));
        installed++;
        printf("[U2 Engine] Applied ADR-031 patch at 0x00C0047A: Guard NULL render target descriptor copy\n");
        fflush(stdout);
    }

    /*
     * ADR-032: Guard Invalid Audio Voice Node Pointer in Audio Voice Handler (0x00133700).
     * At 0x00133704, movq 0x10(%rdi), %rax dereferences rdi. When called for unallocated voices,
     * callers pass rdi = 0x80 (offset arithmetic on NULL base). This crashes with read access to 0x90.
     * We install a 23-byte trampoline at code cave 0x00FE2E30:
     *   cmpq $0x1000, %rdi ; ja .valid ; retq ;
     *   .valid: pushq %rbp ; pushq %r14 ; pushq %rbx ; movq 0x10(%rdi), %rax ; jmp 0x00133708
     * and hook 0x00133700 with 'jmp 0x00FE2E30 ; 3x nop'.
     */
    if (image_size > 0x00FE2E30 + 23) {
        static const uint8_t s_cave_133700[23] = {
            0x48, 0x81, 0xFF, 0x00, 0x10, 0x00, 0x00,       /* cmpq $0x1000, %rdi */
            0x77, 0x01,                                     /* ja +1 (to 0x0A) */
            0xC3,                                           /* retq */
            0x55,                                           /* pushq %rbp */
            0x41, 0x56,                                     /* pushq %r14 */
            0x53,                                           /* pushq %rbx */
            0x48, 0x8B, 0x47, 0x10,                         /* movq 0x10(%rdi), %rax */
            0xE9, 0xC1, 0x11, 0x15, 0xFF                    /* jmp 0x00133708 */
        };
        static const uint8_t s_hook_133700[8] = {
            0xE9, 0x2B, 0xEE, 0xEA, 0x00,                   /* jmp 0x00FE2E30 */
            0x90, 0x90, 0x90                                /* 3x nop */
        };
        memcpy(image + 0x00FE2E30, s_cave_133700, sizeof(s_cave_133700));
        memcpy(image + 0x00133700, s_hook_133700, sizeof(s_hook_133700));
        installed++;
        printf("[U2 Engine] Applied ADR-032 patch at 0x00133700: Guard invalid audio voice node pointer\n");
        fflush(stdout);
    }

    /* Patch at 0x0183BD53 (libSceFios2.prx): Guard NULL destination stat buffer in fios stat handler.
     * When a caller queries file existence or status without requesting stat metadata (stat buffer == NULL),
     * rbx (*(r14 + 0x20)) is NULL. LibSceFios2 blindly called memset_wrapper(NULL, 0, 80) followed by
     * writing stat fields to *(NULL), crashing with write access violation at 0x0.
     * We install a 36-byte trampoline at code cave 0x0185D0C0 that tests rbx:
     * - If rbx == NULL: sets r15d = 0 and jumps directly to function epilogue (0x0183BDD7).
     * - If rbx != NULL: executes memset_wrapper(rbx, 0, 80) and resumes at 0x0183BD69.
     */
    if (image_size > 0x0185D0C0 + 36) {
        static const uint8_t s_trampoline[36] = {
            0x49, 0x8b, 0x5e, 0x20,                         /* movq 0x20(%r14), %rbx */
            0x45, 0x31, 0xff,                               /* xorl %r15d, %r15d */
            0x48, 0x85, 0xdb,                               /* testq %rbx, %rbx */
            0x0f, 0x84, 0x07, 0xed, 0xfd, 0xff,             /* jz 0x0183BDD7 (epilogue) */
            0x31, 0xf6,                                     /* xorl %esi, %esi */
            0xba, 0x50, 0x00, 0x00, 0x00,                   /* movl $80, %edx */
            0x48, 0x89, 0xdf,                               /* movq %rbx, %rdi */
            0xe8, 0x01, 0x15, 0xfe, 0xff,                   /* callq 0x0183E5E0 (memset_wrapper) */
            0xe9, 0x85, 0xec, 0xfd, 0xff                    /* jmp 0x0183BD69 (resume) */
        };
        static const uint8_t s_patch_183bd53[22] = {
            0xe9, 0x68, 0x13, 0x02, 0x00,                   /* jmp 0x0185D0C0 */
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
            0x90                                            /* 17x nop */
        };
        memcpy(image + 0x0185D0C0, s_trampoline, sizeof(s_trampoline));
        memcpy(image + 0x0183BD53, s_patch_183bd53, sizeof(s_patch_183bd53));
        installed++;
        printf("[U2 Engine] Applied patch at 0x0183BD53: Guard NULL stat buffer in libSceFios2 stat handler\n");
        fflush(stdout);
    }

    /* Patch at 0x0184C610 (libSceFios2.prx): Guard NULL device pointer in fios scheduler.
     * When get_device returns NULL, 0x0184C610 was called with rdi == NULL.
     * It proceeded into 0x0184CF40, computing 16(%r13) == 0x10, and calling mutex locks on 0x10.
     * We install a 21-byte trampoline at code cave 0x01860C98 that tests rdi:
     * - If rdi == NULL: returns 0 (false) immediately without altering stack or locking.
     * - If rdi != NULL: sets up stack frame and resumes normal execution at 0x0184C618.
     */
    if (image_size > 0x01860C98 + 21) {
        static const uint8_t s_cave_184c610[21] = {
            0x48, 0x85, 0xff,                               /* testq %rdi, %rdi */
            0x75, 0x03,                                     /* jnz continue */
            0x31, 0xc0,                                     /* xorl %eax, %eax */
            0xc3,                                           /* retq */
            0x55,                                           /* pushq %rbp */
            0x48, 0x89, 0xe5,                               /* movq %rsp, %rbp */
            0x41, 0x57,                                     /* pushq %r15 */
            0x41, 0x56,                                     /* pushq %r14 */
            0xe9, 0x6b, 0xb9, 0xfe, 0xff                    /* jmp 0x0184C618 (resume) */
        };
        static const uint8_t s_patch_184c610[8] = {
            0xe9, 0x83, 0x46, 0x01, 0x00,                   /* jmp 0x01860C98 */
            0x90, 0x90, 0x90                                /* 3x nop */
        };
        memcpy(image + 0x01860C98, s_cave_184c610, sizeof(s_cave_184c610));
        memcpy(image + 0x0184C610, s_patch_184c610, sizeof(s_patch_184c610));
        installed++;
        printf("[U2 Engine] Applied patch at 0x0184C610: Guard NULL device in libSceFios2 scheduler\n");
        fflush(stdout);
    }

    const char *native_vm_env = getenv("BB_U2_NATIVE_VM");
    const int enable_native_vm = (native_vm_env && *native_vm_env && *native_vm_env != '0');
    if (enable_native_vm) {
        write_detour_jmp64(image + 0x00A14120, (uintptr_t)detour_ExecuteScriptCode);
        ++installed;
    }

    const char *native_evalex_env = getenv("BB_U2_NATIVE_EVALEX");
    const int enable_native_evalex = (native_evalex_env && *native_evalex_env && *native_evalex_env != '0');
    if (enable_native_evalex) {
        write_detour_jmp64(image + 0x00A15430, (uintptr_t)detour_EvalEx);
        ++installed;
    }

    const char *native_run_env = getenv("BB_U2_NATIVE_RUN");
    const int enable_native_run = (native_run_env && *native_run_env && *native_run_env != '0');
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
