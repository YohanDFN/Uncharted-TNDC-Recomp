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
     * Fast-forward UCCApp to Title Screen (state 7) only if explicitly requested via BB_U2_FAST_TITLE=1.
     * By default, let the engine naturally run its boot and intro cinematic sequence.
     */
    const char *fast_title_env = getenv("BB_U2_FAST_TITLE");
    if (fast_title_env && *fast_title_env && *fast_title_env != '0') {
        if (image && image_size > 0x010532D0 + 0x20) {
            *(uint32_t *)(image + 0x010532D0) = 7;
            *(uint8_t *)(image + 0x010532D0 + 0x14) = 1;
            printf("[U2 Engine] Initialized UCCApp state = 7 (Title Screen) at 0x010532D0 (via BB_U2_FAST_TITLE=1)\n");
            fflush(stdout);
        }

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
static void ABI detour_GfxWaitEopFence(void *sync_obj, uint64_t target_tick, uint32_t mode) {
    (void)mode;
    if (!sync_obj) return;
    uint64_t *cur_tick = (uint64_t *)((uint8_t *)sync_obj + 0x20);
    if (*cur_tick < target_tick) {
        *cur_tick = target_tick;
    }
    static uint64_t s_sync_calls = 0;
    static uint32_t s_last_ucc_state = 0xFFFFFFFF;
    s_sync_calls++;

    uint32_t cur_state = 0;
    if (g_u2_image_base) {
        cur_state = *(uint32_t *)(g_u2_image_base + 0x010532D0);
        if (cur_state != s_last_ucc_state) {
            printf("[U2 Engine] State Transition: UCCApp State %u -> %u\n", s_last_ucc_state, cur_state);
            fflush(stdout);
            s_last_ucc_state = cur_state;
        }
    }

    if ((s_sync_calls <= 10) || (s_sync_calls % 60 == 0)) {
        uint32_t pad = 0;
        if (g_u2_image_base) {
            pad = *(uint32_t *)(g_u2_image_base + 0x01116180 + 0x864);
        }
        printf("[U2 Loop] Frame #%" PRIu64 " | UCCApp State=%u | Pad=0x%04x | EOP Tick=%" PRIu64 "\n",
               s_sync_calls, cur_state, pad & 0xFFFF, target_tick);
        fflush(stdout);
    }
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
    write_detour_jmp64(image + 0x00B29F10, (uintptr_t)detour_GfxWaitEopFence);
    ++installed;
    write_detour_jmp64(image + 0x0050E150, (uintptr_t)detour_Renderer_InitParticles);
    ++installed;
    write_detour_jmp64(image + 0x00CE33A0, (uintptr_t)detour_StringIdToStringInternal);
    ++installed;

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
