#!/usr/bin/env python3
"""
Naughty Dog "DC" (Dog-Code / Drake-Code) Bytecode Disassembler & Lisp Decompiler
Reverse-engineered from Uncharted: The Nathan Drake Collection (PS4) — U1 / U2 / U3
VM Function: ExecuteScriptCode (FUN_00a1c950)
"""

import sys
import os
import re
import struct
import argparse
import json
from pathlib import Path
from typing import List, Dict, Tuple, Optional, Any, Set

# ==============================================================================
# OPCODES TABLES (Uncharted 1 vs Uncharted 2/3)
# ==============================================================================

class OpFormat:
    NONE = 0
    R_R_R = 1        # dst, src1, src2
    R_R = 2          # dst, src1
    R_IMM16 = 3      # dst, imm16 (lo=b2, hi=b3)
    JUMP = 4         # target (lo=b1, hi=b3)
    JUMP_COND = 5    # cond_reg (b2), target (lo=b1, hi=b3)
    CALL_SCRIPT = 6  # dst, func_reg, argc
    CALL_NATIVE = 7  # dst, func_reg, argc
    LOAD_CONST = 8   # dst, const_idx (imm16 = (b3<<8)|b2)
    LOAD_SYM = 9     # dst, sym_reg_or_idx
    STORE_MEM = 10   # [src1], src2
    LOAD_MEM = 11    # dst, [src1]
    ARITH_IMM = 12   # dst, src1, imm8
    RET_FMT = 13     # ret_flag=b1, ret_reg=b2

# Base opcodes (Uncharted 2 / Uncharted 3)
OPCODES_U2: Dict[int, Tuple[str, int, str]] = {
    0x00: ("RET",         OpFormat.RET_FMT,     "Return from function / yield"),
    0x01: ("ADD_INT",     OpFormat.R_R_R,       "R[dst] = R[src1] + R[src2] (int64)"),
    0x02: ("SUB_INT",     OpFormat.R_R_R,       "R[dst] = R[src1] - R[src2] (int64)"),
    0x03: ("MUL_INT",     OpFormat.R_R_R,       "R[dst] = R[src1] * R[src2] (int64)"),
    0x04: ("DIV_INT",     OpFormat.R_R_R,       "R[dst] = R[src1] / R[src2] (int64)"),
    0x05: ("ADD_FLOAT",   OpFormat.R_R_R,       "R[dst] = R[src1] + R[src2] (float)"),
    0x06: ("SUB_FLOAT",   OpFormat.R_R_R,       "R[dst] = R[src1] - R[src2] (float)"),
    0x07: ("MUL_FLOAT",   OpFormat.R_R_R,       "R[dst] = R[src1] * R[src2] (float)"),
    0x08: ("DIV_FLOAT",   OpFormat.R_R_R,       "R[dst] = R[src1] / R[src2] (float)"),
    0x09: ("LOAD_POOL",   OpFormat.R_R,         "R[dst] = ConstPool[R[src1]]"),
    0x0A: ("SHL_INT",     OpFormat.R_R_R,       "R[dst] = R[src1] << R[src2]"),
    0x0B: ("SHR_INT",     OpFormat.R_R_R,       "R[dst] = R[src1] >> R[src2]"),
    0x0C: ("LOAD_IMM16",  OpFormat.R_IMM16,     "R[dst] = imm16"),
    0x0D: ("LOAD_U64",    OpFormat.LOAD_MEM,    "R[dst] = *(uint64_t*)R[src1]"),
    0x0E: ("LOAD_U32",    OpFormat.LOAD_MEM,    "R[dst] = *(uint32_t*)R[src1]"),
    0x0F: ("LOAD_PTR",    OpFormat.LOAD_MEM,    "R[dst] = *(void**)R[src1]"),
    0x10: ("STORE_32",    OpFormat.STORE_MEM,   "*(uint32_t*)R[src1] = (uint32)R[src2]"),
    0x11: ("STORE_U32",   OpFormat.STORE_MEM,   "*(uint32_t*)R[src1] = (uint32)R[src2]"),
    0x12: ("STORE_64",    OpFormat.STORE_MEM,   "*(uint64_t*)R[src1] = (uint64)R[src2]"),
    0x13: ("STORE_8",     OpFormat.STORE_MEM,   "*(uint8_t*)R[src1] = (uint8)R[src2]"),
    0x14: ("STORE_16",    OpFormat.STORE_MEM,   "*(uint16_t*)R[src1] = (uint16)R[src2]"),
    0x15: ("LOAD_SYM",    OpFormat.LOAD_SYM,    "R[dst] = ResolveSymbol(src1)"),
    0x16: ("MOV",         OpFormat.R_R,         "R[dst] = R[src1]"),
    0x17: ("MOV_ALT1",    OpFormat.R_R,         "R[dst] = R[src1]"),
    0x18: ("MOV_ALT2",    OpFormat.R_R,         "R[dst] = R[src1]"),
    0x19: ("FTOI",        OpFormat.R_R,         "R[dst] = (int)R[src1]"),
    0x1A: ("ITOF",        OpFormat.R_R,         "R[dst] = (float)R[src1]"),
    0x1B: ("CALL_DEFUN",  OpFormat.CALL_SCRIPT, "Call script defun R[src1](argc=b3) -> R[dst]"),
    0x1C: ("CALL_NATIVE", OpFormat.CALL_NATIVE, "Call native C++ function R[src1](argc=b3) -> R[dst]"),
    0x1D: ("CMP_EQ",      OpFormat.R_R_R,       "R[dst] = (R[src1] == R[src2])"),
    0x1E: ("CMP_GT",      OpFormat.R_R_R,       "R[dst] = (R[src1] > R[src2])"),
    0x1F: ("CMP_GE",      OpFormat.R_R_R,       "R[dst] = (R[src1] >= R[src2])"),
    0x20: ("CMP_LT",      OpFormat.R_R_R,       "R[dst] = (R[src1] < R[src2])"),
    0x21: ("CMP_LE",      OpFormat.R_R_R,       "R[dst] = (R[src1] <= R[src2])"),
    0x22: ("FCMP_EQ",     OpFormat.R_R_R,       "R[dst] = (float)R[src1] == (float)R[src2]"),
    0x23: ("FCMP_GT",     OpFormat.R_R_R,       "R[dst] = (float)R[src1] > (float)R[src2]"),
    0x24: ("FCMP_GE",     OpFormat.R_R_R,       "R[dst] = (float)R[src1] >= (float)R[src2]"),
    0x25: ("FCMP_LT",     OpFormat.R_R_R,       "R[dst] = (float)R[src1] < (float)R[src2]"),
    0x26: ("FCMP_LE",     OpFormat.R_R_R,       "R[dst] = (float)R[src1] <= (float)R[src2]"),
    0x27: ("MOD_INT",     OpFormat.R_R_R,       "R[dst] = R[src1] % R[src2]"),
    0x28: ("CALL_HELPER", OpFormat.R_R_R,       "R[dst] = Helper(R[src1], R[src2])"),
    0x29: ("ABS_INT",     OpFormat.R_R,         "R[dst] = abs(R[src1])"),
    0x2A: ("ABS_FLOAT",   OpFormat.R_R,         "R[dst] = fabsf(R[src1])"),
    0x2B: ("MIN_FLOAT",   OpFormat.R_R_R,       "R[dst] = fminf(R[src1], R[src2])"),
    0x2C: ("MAX_FLOAT",   OpFormat.R_R_R,       "R[dst] = fmaxf(R[src1], R[src2])"),
    0x2D: ("JMP",         OpFormat.JUMP,        "Unconditional jump to PC"),
    0x2E: ("JNZ",         OpFormat.JUMP_COND,   "Jump to PC if R[src1] != 0"),
    0x2F: ("JZ",          OpFormat.JUMP_COND,   "Jump to PC if R[src1] == 0"),
    0x30: ("NOT",         OpFormat.R_R,         "R[dst] = !R[src1]"),
    0x31: ("AND_BIT",     OpFormat.R_R_R,       "R[dst] = R[src1] & R[src2]"),
    0x32: ("NOT_BIT",     OpFormat.R_R,         "R[dst] = ~R[src1]"),
    0x33: ("OR_BIT",      OpFormat.R_R_R,       "R[dst] = R[src1] | R[src2]"),
    0x34: ("XOR_BIT",     OpFormat.R_R_R,       "R[dst] = R[src1] ^ R[src2]"),
    0x35: ("NOR_BIT",     OpFormat.R_R_R,       "R[dst] = ~(R[src1] | R[src2])"),
    0x36: ("AND_LOGIC",   OpFormat.R_R_R,       "R[dst] = R[src1] && R[src2]"),
    0x37: ("OR_LOGIC",    OpFormat.R_R_R,       "R[dst] = R[src1] || R[src2]"),
    0x38: ("NEG_INT",     OpFormat.R_R,         "R[dst] = -R[src1]"),
    0x39: ("NEG_FLOAT",   OpFormat.R_R,         "R[dst] = -(float)R[src1]"),
    0x3A: ("GET_ARGC",    OpFormat.R_R,         "R[dst] = Frame.argc"),
    0x3B: ("ADD_IMM",     OpFormat.ARITH_IMM,   "R[dst] = R[src1] + imm8"),
    0x3C: ("SUB_IMM",     OpFormat.ARITH_IMM,   "R[dst] = R[src1] - imm8"),
    0x3D: ("MUL_IMM",     OpFormat.ARITH_IMM,   "R[dst] = R[src1] * imm8"),
    0x3E: ("DIV_IMM",     OpFormat.ARITH_IMM,   "R[dst] = R[src1] / imm8"),
    0x3F: ("LOAD_I32",    OpFormat.LOAD_CONST,  "R[dst] = (int32)ConstPool[idx]"),
    0x40: ("LOAD_F32",    OpFormat.LOAD_CONST,  "R[dst] = (float)ConstPool[idx]"),
    0x41: ("LOAD_I64",    OpFormat.LOAD_CONST,  "R[dst] = (ptr/str)ConstPool[idx]"),
    0x42: ("LOAD_SYM_C",  OpFormat.LOAD_CONST,  "R[dst] = (sym)ConstPool[idx]"),
    0x43: ("MOV_ARG",     OpFormat.R_R,         "R[dst] = R[src1]"),
    0x44: ("LOAD_CONST",  OpFormat.LOAD_CONST,  "R[dst] = ConstPool[idx]"),
    0x45: ("LOAD_I16",    OpFormat.LOAD_CONST,  "R[dst] = (int16)ConstPool[idx]"),
    0x46: ("LOAD_U16",    OpFormat.LOAD_CONST,  "R[dst] = (uint16)ConstPool[idx]"),
    0x49: ("LOAD_M_I8",   OpFormat.LOAD_MEM,    "R[dst] = *(int8_t*)R[src1]"),
    0x4A: ("LOAD_M_U8",   OpFormat.LOAD_MEM,    "R[dst] = *(uint8_t*)R[src1]"),
    0x4B: ("LOAD_M_I16",  OpFormat.LOAD_MEM,    "R[dst] = *(int16_t*)R[src1]"),
    0x4C: ("LOAD_M_U16",  OpFormat.LOAD_MEM,    "R[dst] = *(uint16_t*)R[src1]"),
    0x4D: ("LOAD_M_I32",  OpFormat.LOAD_MEM,    "R[dst] = *(int32_t*)R[src1]"),
}

# Uncharted 1 opcodes (where 0x3A..0x48 are shifted by -5 relative to U2)
OPCODES_U1: Dict[int, Tuple[str, int, str]] = dict(OPCODES_U2)
OPCODES_U1.update({
    0x3A: ("LOAD_I32",    OpFormat.LOAD_CONST,  "R[dst] = (int32)ConstPool[idx]"),
    0x3B: ("LOAD_F32",    OpFormat.LOAD_CONST,  "R[dst] = (float)ConstPool[idx]"),
    0x3C: ("LOAD_STR_C",  OpFormat.LOAD_CONST,  "R[dst] = (str/ptr)ConstPool[idx]"),
    0x3D: ("LOAD_SYM_C",  OpFormat.LOAD_CONST,  "R[dst] = (sym)ConstPool[idx]"),
    0x3E: ("LOAD_I8",     OpFormat.LOAD_CONST,  "R[dst] = (int8)ConstPool[idx]"),
    0x3F: ("LOAD_U8",     OpFormat.LOAD_CONST,  "R[dst] = (uint8)ConstPool[idx]"),
    0x40: ("LOAD_I16",    OpFormat.LOAD_CONST,  "R[dst] = (int16)ConstPool[idx]"),
    0x41: ("LOAD_U16",    OpFormat.LOAD_CONST,  "R[dst] = (uint16)ConstPool[idx]"),
    0x44: ("LOAD_M_I8",   OpFormat.LOAD_MEM,    "R[dst] = *(int8_t*)R[src1]"),
    0x45: ("LOAD_M_U8",   OpFormat.LOAD_MEM,    "R[dst] = *(uint8_t*)R[src1]"),
    0x46: ("LOAD_M_I16",  OpFormat.LOAD_MEM,    "R[dst] = *(int16_t*)R[src1]"),
    0x47: ("LOAD_M_U16",  OpFormat.LOAD_MEM,    "R[dst] = *(uint16_t*)R[src1]"),
    0x48: ("LOAD_M_I32",  OpFormat.LOAD_MEM,    "R[dst] = *(int32_t*)R[src1]"),
})

OPCODES = OPCODES_U2


# ==============================================================================
# NAUGHTY DOG STRINGID HASH (CRC-32 / POLY 0x04C11DB7) & SYMBOL RESOLVER
# ==============================================================================

def _build_crc32_be_table(poly: int = 0x04C11DB7) -> List[int]:
    table = []
    for i in range(256):
        crc = (i << 24) & 0xFFFFFFFF
        for _ in range(8):
            if crc & 0x80000000:
                crc = ((crc << 1) ^ poly) & 0xFFFFFFFF
            else:
                crc = (crc << 1) & 0xFFFFFFFF
        table.append(crc)
    return table

_CRC32_BE_TABLE = _build_crc32_be_table(0x04C11DB7)


def nd_crc32_raw(data: bytes, init: int = 0) -> int:
    crc = init & 0xFFFFFFFF
    for b in data:
        crc = ((crc << 8) & 0xFFFFFFFF) ^ _CRC32_BE_TABLE[((crc >> 24) ^ b) & 0xFF]
    return crc


def _step_zero_byte(crc: int) -> int:
    return ((crc << 8) & 0xFFFFFFFF) ^ _CRC32_BE_TABLE[(crc >> 24) & 0xFF]


def _solve_nd_hash_params() -> Tuple[int, int]:
    """
    Use two known (string, hash) pairs from Uncharted's DC00 export table:
      s1 = b"top-level"                 (len 9)  -> 0x87B03989
      s2 = b"*ai-behavior-test-update*" (len 25) -> 0x4280E374
    Since Hash(s) = CRC_raw(s, 0) ^ T^len(s)(init) ^ xorout,
    T^9(init) ^ T^25(init) = (CRC_raw(s1, 0) ^ 0x87B03989) ^ (CRC_raw(s2, 0) ^ 0x4280E374).
    """
    s1 = b"top-level"
    h1 = 0x87B03989
    s2 = b"*ai-behavior-test-update*"
    h2 = 0x4280E374

    r1 = nd_crc32_raw(s1, 0) ^ h1
    r2 = nd_crc32_raw(s2, 0) ^ h2
    target_diff = r1 ^ r2

    # Build 32x32 GF(2) matrix for (T^9 ^ T^25)
    cols = []
    for bit in range(32):
        v = 1 << bit
        v9 = v
        for _ in range(9):
            v9 = _step_zero_byte(v9)
        v25 = v9
        for _ in range(16):
            v25 = _step_zero_byte(v25)
        cols.append(v9 ^ v25)

    # Gaussian elimination over GF(2)
    rows = []
    for r in range(32):
        mask = 0
        for c in range(32):
            if (cols[c] >> r) & 1:
                mask |= (1 << c)
        rhs = (target_diff >> r) & 1
        rows.append([mask, rhs])

    pivot_col_for_row = {}
    row_idx = 0
    for c in range(32):
        pivot = None
        for r in range(row_idx, 32):
            if (rows[r][0] >> c) & 1:
                pivot = r
                break
        if pivot is None:
            continue
        rows[row_idx], rows[pivot] = rows[pivot], rows[row_idx]
        for r in range(32):
            if r != row_idx and ((rows[r][0] >> c) & 1):
                rows[r][0] ^= rows[row_idx][0]
                rows[r][1] ^= rows[row_idx][1]
        pivot_col_for_row[row_idx] = c
        row_idx += 1

    init_val = 0
    for r, c in pivot_col_for_row.items():
        if rows[r][1]:
            init_val |= (1 << c)

    init_shifted_9 = init_val
    for _ in range(9):
        init_shifted_9 = _step_zero_byte(init_shifted_9)
    xorout_val = r1 ^ init_shifted_9
    return init_val, xorout_val

_ND_HASH_INIT, _ND_HASH_XOROUT = _solve_nd_hash_params()


def nd_string_hash(s: str) -> int:
    """Compute Naughty Dog StringId 32-bit hash for a symbol string."""
    raw = s.encode("ascii", errors="ignore")
    return nd_crc32_raw(raw, _ND_HASH_INIT) ^ _ND_HASH_XOROUT


def _camel_to_kebab_variants(name: str) -> List[str]:
    """Convert C++ native name like 'DcEntityAliveP' or 'DcEntitySetLevelB' to Lisp symbol variants."""
    variants = [name, name.lower()]
    core = name[2:] if name.startswith("Dc") and len(name) > 2 else name

    # Split CamelCase into tokens
    tokens = re.findall(r"[A-Z]+(?=[A-Z][a-z]|\d|\b)|[A-Z]?[a-z]+|\d+", core)
    if not tokens:
        return variants

    lower_tokens = [t.lower() for t in tokens]
    base_kebab = "-".join(lower_tokens)
    variants.append(base_kebab)
    variants.append("dc-" + base_kebab)

    # Also join trailing digits to previous token (e.g., int-32 -> int32, vector-3 -> vector3)
    joined_digits = []
    for tok in lower_tokens:
        if tok.isdigit() and joined_digits:
            joined_digits[-1] = joined_digits[-1] + tok
        else:
            joined_digits.append(tok)
    joined_kebab = "-".join(joined_digits)
    variants.append(joined_kebab)

    # Predicate trailing 'P' -> '?'
    if lower_tokens[-1] == "p" and len(lower_tokens) > 1:
        pred_base = "-".join(lower_tokens[:-1])
        pred_joined = "-".join(joined_digits[:-1]) if len(joined_digits) > 1 else pred_base
        variants.append(pred_base + "?")
        variants.append(pred_joined + "?")
        variants.append(pred_base)
        if lower_tokens[0] == "is" and len(lower_tokens) > 2:
            variants.append("-".join(lower_tokens[1:-1]) + "?")

    # Bang/mutator trailing 'B' -> '!'
    if lower_tokens[-1] == "b" and len(lower_tokens) > 1:
        bang_base = "-".join(lower_tokens[:-1])
        bang_joined = "-".join(joined_digits[:-1]) if len(joined_digits) > 1 else bang_base
        variants.append(bang_base + "!")
        variants.append(bang_joined + "!")
        variants.append(bang_base)

    # Without 'get-' prefix
    if lower_tokens[0] == "get" and len(lower_tokens) > 1:
        variants.append("-".join(lower_tokens[1:]))
        variants.append("-".join(joined_digits[1:]))

    return list(dict.fromkeys(variants))


# Global cache of known hash -> symbol name across files
GLOBAL_HASH_TO_NAME: Dict[int, str] = {
    0x87B03989: "top-level",
    0x5047B2C8: "end-script",
    0xB8831CF4: "game-task-graph-login",
    0x6E51B48F: "*task-graph*",
}
_GLOBAL_DB_INITIALIZED = False


def _init_global_symbol_db():
    global _GLOBAL_DB_INITIALIZED
    if _GLOBAL_DB_INITIALIZED:
        return
    _GLOBAL_DB_INITIALIZED = True

    common_symbols = [
        "top-level", "self", "other", "player", "target", "true", "false", "nil", "none",
        "default", "all", "any", "state-time", "current-time", "go", "spawn", "kill",
        "format", "print", "printf", "message", "end-script", "return", "exit",
        "not", "and", "or", "xor", "mod", "abs", "min", "max", "lerp", "clamp",
        "sin", "cos", "tan", "atan", "sqrt", "length", "distance", "normalize",
        "vector", "vector3", "vector4", "matrix", "quaternion", " meters", "seconds",
        "send-event", "event-sender", "event-message", "task-status", "task-close!",
        "ai", "events", "hints", "window", "train", "heli", "camera", "sound", "music",
        "active", "inactive", "dead", "alive", "idle", "combat", "alert", "patrol",
        "enter", "exit", "trans", "code", "post", "event", "update", "init", "done",
    ]
    for sym in common_symbols:
        for form in (sym, f"*{sym}*", f":{sym}"):
            h = nd_string_hash(form)
            GLOBAL_HASH_TO_NAME.setdefault(h, form)

    tools_dir = Path(__file__).resolve().parent
    natives_file = tools_dir / "dc_natives.json"
    if natives_file.exists():
        try:
            with open(natives_file, "r", encoding="utf-8") as f:
                natives = json.load(f)
            for n in natives:
                variants = _camel_to_kebab_variants(n)
                # Prefer clean Lisp kebab name (with ? or !)
                preferred = variants[2] if len(variants) > 2 else n
                for v in variants:
                    if v.endswith("?") or v.endswith("!"):
                        preferred = v
                        break
                for v in variants:
                    h = nd_string_hash(v)
                    GLOBAL_HASH_TO_NAME.setdefault(h, preferred)
        except Exception:
            pass


def prescan_binary_symbols(raw_data: bytes):
    """Scan a DC00 binary and register all discovered symbols/strings into GLOBAL_HASH_TO_NAME."""
    DcSymbolResolver(raw_data)


class DcSymbolResolver:
    """Resolves raw numbers, offsets, and hashes to human-readable symbol names."""
    def __init__(self, raw_data: bytes):
        _init_global_symbol_db()
        self.raw_data = raw_data
        self.offset_to_str: Dict[int, str] = {}
        self.hash_to_name: Dict[int, str] = dict(GLOBAL_HASH_TO_NAME)
        self._scan_file_symbols()

    def register_symbol(self, name: str, sym_hash: Optional[int] = None, offset: Optional[int] = None):
        if not name:
            return
        if offset is not None:
            self.offset_to_str[offset] = name
        h = nd_string_hash(name)
        self.hash_to_name[h] = name
        GLOBAL_HASH_TO_NAME[h] = name
        if sym_hash is not None and sym_hash != 0:
            self.hash_to_name[sym_hash] = name
            GLOBAL_HASH_TO_NAME[sym_hash] = name

    def _scan_file_symbols(self):
        """Scan file for tagged symbols (0x637BDB3E in U1, 0x0B3952E7 in U2/U3) and ASCII strings."""
        data = self.raw_data
        for tag in (b"\x3e\xdb\x7b\x63", b"\xe7\x52\x39\x0b"):
            pos = 0
            while True:
                idx = data.find(tag, pos)
                if idx == -1:
                    break
                end = idx + 4
                while end < len(data) and data[end] != 0:
                    end += 1
                raw = data[idx + 4:end]
                try:
                    s = raw.decode("ascii")
                    if s and all(0x20 <= ord(c) <= 0x7E for c in s):
                        self.offset_to_str[idx] = s
                        self.offset_to_str[idx + 4] = s
                        self.register_symbol(s)
                except Exception:
                    pass
                pos = idx + 4

        # Also scan general null-terminated printable ASCII strings
        i = 0
        n = len(data)
        while i < n - 4:
            if 0x20 <= data[i] <= 0x7E:
                j = i
                while j < n and 0x20 <= data[j] <= 0x7E:
                    j += 1
                if j - i >= 3 and j < n and data[j] == 0:
                    # Skip if preceded by string tag (already handled cleanly above)
                    if i >= 4 and data[i - 4:i] in (b"\x3e\xdb\x7b\x63", b"\xe7\x52\x39\x0b"):
                        i = j + 1
                        continue
                    try:
                        s = data[i:j].decode("ascii")
                        # Ignore accidental 2-char prefix from tag bytes
                        if s.startswith("{c") and len(s) > 4:
                            s = s[2:]
                            self.offset_to_str[i + 2] = s
                        self.offset_to_str.setdefault(i, s)
                        if all(c.isalnum() or c in "-_*?!./:" for c in s):
                            self.register_symbol(s)
                    except Exception:
                        pass
                    i = j
            i += 1

        # Scan U1 32-byte export table at 0x38 to register exact (hash, string) pairs
        if n >= 0x58 and data[:4] in (b"00CD", b"DC00"):
            u1_tags = {0x9ED499E1, 0xCABD357F, 0xD3D57B81}
            pos = 0x38
            while pos + 32 <= n:
                sym_hash, ttag, name_ptr, pad, desc_ptr = struct.unpack_from("<IIQQQ", data, pos)
                if ttag not in u1_tags or not (0 < name_ptr < n):
                    break
                s = self.resolve_ptr(name_ptr)
                if s:
                    self.register_symbol(s, sym_hash, name_ptr)
                pos += 32

    def resolve_ptr(self, ptr: int) -> Optional[str]:
        """Check if pointer matches a known string or symbol in file."""
        if ptr in self.offset_to_str:
            return self.offset_to_str[ptr]
        if (ptr + 4) in self.offset_to_str:
            return self.offset_to_str[ptr + 4]
        if 0 < ptr < len(self.raw_data):
            end = ptr
            while end < len(self.raw_data) and 0x20 <= self.raw_data[end] <= 0x7E:
                end += 1
            if end - ptr >= 2 and end < len(self.raw_data) and self.raw_data[end] == 0:
                s = self.raw_data[ptr:end].decode("ascii", errors="ignore")
                self.offset_to_str[ptr] = s
                return s
        return None

    def resolve_hash(self, h: int) -> Optional[str]:
        return self.hash_to_name.get(h & 0xFFFFFFFF)

    def resolve_constant(self, val_u64: int, kind: str = "auto") -> str:
        """Resolve a 64-bit constant entry to a readable Lisp representation."""
        u32 = val_u64 & 0xFFFFFFFF
        hi32 = (val_u64 >> 32) & 0xFFFFFFFF

        # 1. Known symbol hash
        if hi32 == 0 and u32 in self.hash_to_name:
            sym = self.hash_to_name[u32]
            if kind == "call":
                return sym
            return f"'{sym}" if not (sym.startswith("*") or sym.startswith(":")) else sym

        # 2. String pointer in file
        if hi32 == 0 and 0x40 <= val_u64 < len(self.raw_data):
            s = self.resolve_ptr(val_u64)
            if s:
                return f'"{s}"'

        # 3. Float32 check
        if kind in ("float", "auto") and hi32 == 0 and u32 != 0:
            f32 = struct.unpack("<f", struct.pack("<I", u32))[0]
            if kind == "float" and f32 == f32:
                return f"{f32:.4f}".rstrip("0").rstrip(".") + ".0" if f32.is_integer() else f"{f32:.4f}".rstrip("0")
            # For auto, check if exponent is in normal game float range [-8192, 8192] and low 12 mantissa bits are clean
            exp = (u32 >> 23) & 0xFF
            if 115 <= exp <= 145 and (u32 & 0xFFF) == 0 and f32 == f32:
                return f"{f32:g}"

        if kind == "sym":
            return self.hash_to_name.get(u32, f"sym_0x{u32:08X}")
        if kind == "call":
            return self.hash_to_name.get(u32, f"fn_0x{u32:08X}")

        if u32 > 0xFFFF:
            return self.hash_to_name.get(u32, f"0x{u32:08X}")
        return f"{u32}"


# ==============================================================================
# DISASSEMBLER ENGINE
# ==============================================================================

def format_reg(reg_id: int, is_u1: bool = False) -> str:
    """Format register name, annotating argument registers."""
    if is_u1:
        if 64 <= reg_id <= 79:
            return f"r{reg_id}(arg{reg_id - 64})"
    else:
        if 24 <= reg_id <= 31:
            return f"r{reg_id}(arg{reg_id - 24})"
        if 64 <= reg_id <= 79:
            return f"r{reg_id}(arg{reg_id - 64})"
    return f"r{reg_id}"


def is_arg_reg(reg_id: int) -> Optional[int]:
    if 64 <= reg_id <= 79:
        return reg_id - 64
    if 24 <= reg_id <= 31:
        return reg_id - 24
    return None


class Instruction:
    def __init__(self, pc: int, raw_bytes: bytes, is_u1: bool = False):
        self.pc = pc
        self.raw = raw_bytes
        self.is_u1 = is_u1
        self.opcode = raw_bytes[0]
        self.b1 = raw_bytes[1]  # dst / target_lo / ret_flag
        self.b2 = raw_bytes[2]  # src1 / imm_lo / ret_reg
        self.b3 = raw_bytes[3]  # src2 / imm_hi / argc

        table = OPCODES_U1 if is_u1 else OPCODES_U2
        info = table.get(self.opcode, (f"UNKNOWN_0x{self.opcode:02X}", OpFormat.NONE, "Unknown Opcode"))
        self.mnemonic = info[0]
        self.fmt = info[1]
        self.desc = info[2]
        self.target_pc: Optional[int] = None

        if self.fmt in (OpFormat.JUMP, OpFormat.JUMP_COND):
            self.target_pc = (self.b3 << 8) | self.b1

    def imm16(self) -> int:
        return (self.b3 << 8) | self.b2

    def format_asm(self, pool: Optional[List[str]] = None) -> str:
        reg_d = format_reg(self.b1, self.is_u1)
        reg_s1 = format_reg(self.b2, self.is_u1)
        reg_s2 = format_reg(self.b3, self.is_u1)

        if self.fmt == OpFormat.RET_FMT:
            if self.b1 != 0:
                args = f"{reg_s1}"
            else:
                args = ""
        elif self.fmt == OpFormat.NONE:
            args = ""
        elif self.fmt == OpFormat.R_R_R:
            args = f"{reg_d}, {reg_s1}, {reg_s2}"
        elif self.fmt == OpFormat.R_R:
            args = f"{reg_d}, {reg_s1}"
        elif self.fmt == OpFormat.R_IMM16:
            args = f"{reg_d}, #0x{self.imm16():04X}"
        elif self.fmt == OpFormat.JUMP:
            args = f"loc_{self.target_pc:04X}"
        elif self.fmt == OpFormat.JUMP_COND:
            args = f"{reg_s1}, loc_{self.target_pc:04X}"
        elif self.fmt in (OpFormat.CALL_SCRIPT, OpFormat.CALL_NATIVE):
            args = f"{reg_d}, {reg_s1}(argc={self.b3})"
        elif self.fmt == OpFormat.LOAD_CONST:
            c_idx = self.imm16()
            if pool and c_idx < len(pool):
                args = f"{reg_d}, const[{c_idx}] (= {pool[c_idx]})"
            else:
                args = f"{reg_d}, const[{c_idx}]"
        elif self.fmt == OpFormat.LOAD_SYM:
            if self.is_u1:
                args = f"{reg_d}, {reg_s1}"
            else:
                c_idx = self.imm16()
                if pool and c_idx < len(pool):
                    args = f"{reg_d}, const[{c_idx}] (= {pool[c_idx]})"
                else:
                    args = f"{reg_d}, const[{c_idx}]"
        elif self.fmt == OpFormat.LOAD_MEM:
            args = f"{reg_d}, [{reg_s1}]"
        elif self.fmt == OpFormat.STORE_MEM:
            args = f"[{reg_s1}], {reg_s2}"
        elif self.fmt == OpFormat.ARITH_IMM:
            args = f"{reg_d}, {reg_s1}, #{self.b3}"
        else:
            args = f"{self.b1:02X} {self.b2:02X} {self.b3:02X}"

        return f"{self.mnemonic:<14} {args}".rstrip()

    def to_lisp(self, pool: Optional[List[str]] = None) -> str:
        d = format_reg(self.b1, self.is_u1)
        s1 = format_reg(self.b2, self.is_u1)
        s2 = format_reg(self.b3, self.is_u1)
        target = f"loc_{self.target_pc:04X}" if self.target_pc is not None else "loc_????"

        c_idx = self.imm16()
        resolved_const = pool[c_idx] if (pool and c_idx < len(pool)) else f"const[{c_idx}]"

        if self.opcode == 0x00:
            return f"(return {s1})" if self.b1 != 0 else "(return)"
        if self.opcode == 0x09:
            return f"(set! {d} const[{s1}])"
        if self.opcode == 0x15:
            return f"(set! {d} (resolve-symbol {s1 if self.is_u1 else resolved_const}))"
        if self.fmt == OpFormat.LOAD_CONST:
            return f"(set! {d} {resolved_const})"
        if self.fmt == OpFormat.LOAD_MEM:
            return f"(set! {d} (deref {s1}))"
        if self.fmt == OpFormat.R_R and self.mnemonic.startswith("MOV"):
            return f"(set! {d} {s1})"

        lisp_map = {
            0x01: f"(set! {d} (+ {s1} {s2}))",
            0x02: f"(set! {d} (- {s1} {s2}))",
            0x03: f"(set! {d} (* {s1} {s2}))",
            0x04: f"(set! {d} (/ {s1} {s2}))",
            0x05: f"(set! {d} (+.f {s1} {s2}))",
            0x06: f"(set! {d} (-.f {s1} {s2}))",
            0x07: f"(set! {d} (*.f {s1} {s2}))",
            0x08: f"(set! {d} (/.f {s1} {s2}))",
            0x0C: f"(set! {d} 0x{self.imm16():04X})",
            0x10: f"(store-u32! {s1} {s2})",
            0x11: f"(store-u32! {s1} {s2})",
            0x12: f"(store-u64! {s1} {s2})",
            0x19: f"(set! {d} (int {s1}))",
            0x1A: f"(set! {d} (float {s1}))",
            0x1B: f"(set! {d} (apply-defun {s1} :args {self.b3}))",
            0x1C: f"(set! {d} (call-native {s1} :args {self.b3}))",
            0x1D: f"(set! {d} (= {s1} {s2}))",
            0x1E: f"(set! {d} (> {s1} {s2}))",
            0x1F: f"(set! {d} (>= {s1} {s2}))",
            0x20: f"(set! {d} (< {s1} {s2}))",
            0x21: f"(set! {d} (<= {s1} {s2}))",
            0x22: f"(set! {d} (=.f {s1} {s2}))",
            0x23: f"(set! {d} (>.f {s1} {s2}))",
            0x24: f"(set! {d} (>=.f {s1} {s2}))",
            0x25: f"(set! {d} (<.f {s1} {s2}))",
            0x26: f"(set! {d} (<=.f {s1} {s2}))",
            0x27: f"(set! {d} (mod {s1} {s2}))",
            0x29: f"(set! {d} (abs {s1}))",
            0x2A: f"(set! {d} (abs.f {s1}))",
            0x2D: f"(goto {target})",
            0x2E: f"(if (!= {s1} 0) (goto {target}))",
            0x2F: f"(if (= {s1} 0) (goto {target}))",
            0x30: f"(set! {d} (not {s1}))",
            0x31: f"(set! {d} (logand {s1} {s2}))",
            0x32: f"(set! {d} (lognot {s1}))",
            0x33: f"(set! {d} (logior {s1} {s2}))",
            0x34: f"(set! {d} (logxor {s1} {s2}))",
            0x36: f"(set! {d} (and {s1} {s2}))",
            0x37: f"(set! {d} (or {s1} {s2}))",
            0x38: f"(set! {d} (- {s1}))",
            0x39: f"(set! {d} (-.f {s1}))",
            0x3B: f"(set! {d} (+ {s1} {self.b3}))",
            0x3C: f"(set! {d} (- {s1} {self.b3}))",
            0x3D: f"(set! {d} (* {s1} {self.b3}))",
            0x3E: f"(set! {d} (/ {s1} {self.b3}))",
        }
        return lisp_map.get(self.opcode, f";; {self.format_asm(pool)}")


# ==============================================================================
# DC00 CONTAINER, EXPORT TABLE, DATA DEFINITIONS & FUNCTION PARSER
# ==============================================================================

class DcExportEntry:
    def __init__(self, idx: int, sym_hash: int, type_tag: int, name: str, desc_offset: int):
        self.idx = idx
        self.hash = sym_hash
        self.type_tag = type_tag
        self.name = name
        self.desc_offset = desc_offset


class DcDataDef:
    def __init__(self, name: str, sym_hash: int, type_tag: int, offset: int, fields: List[str]):
        self.name = name
        self.hash = sym_hash
        self.type_tag = type_tag
        self.offset = offset
        self.fields = fields


class DcFunctionDef:
    def __init__(
        self,
        idx: int,
        func_hash: int,
        desc_offset: int,
        code_start: int,
        code_end: int,
        raw_pool: List[int],
        pool_str: List[str],
        name: Optional[str] = None,
        is_u1: bool = False,
    ):
        self.idx = idx
        self.hash = func_hash
        self.desc_offset = desc_offset
        self.code_start = code_start
        self.code_end = code_end
        self.code_size = max(0, code_end - code_start)
        self.raw_pool = raw_pool
        self.pool = pool_str
        self.name = name or GLOBAL_HASH_TO_NAME.get(func_hash, f"func_{func_hash:08X}")
        self.is_u1 = is_u1


class DcContainer:
    """Parses full Naughty Dog DC00 container (Uncharted 1, 2, and 3), exports, data, and functions."""
    def __init__(self, data: bytes):
        self.data = data
        self.is_dc = False
        self.is_u1 = False
        self.version = 0
        self.file_size = len(data)
        self.num_sections = 0
        self.exports: List[DcExportEntry] = []
        self.data_defs: List[DcDataDef] = []
        self.functions: List[DcFunctionDef] = []
        self.resolver = DcSymbolResolver(data)

        if len(data) >= 0x38 and (data[:4] == b"00CD" or data[:4] == b"DC00"):
            self.is_dc = True
            self.version = struct.unpack_from("<I", data, 4)[0]
            self.num_sections = struct.unpack_from("<I", data, 0x24)[0] if len(data) >= 0x28 else 0
            self._parse_container()

    def _parse_container(self):
        data = self.data
        n = len(data)

        # 1. Detect Uncharted 1 32-byte export records starting at 0x38
        u1_tags = {0x9ED499E1, 0xCABD357F, 0xD3D57B81}
        desc_to_export: Dict[int, DcExportEntry] = {}

        if n >= 0x58:
            first_tag = struct.unpack_from("<I", data, 0x3C)[0]
            if first_tag in u1_tags:
                self.is_u1 = True
                pos = 0x38
                idx = 0
                while pos + 32 <= n:
                    sym_hash, ttag, name_ptr, pad, desc_ptr = struct.unpack_from("<IIQQQ", data, pos)
                    if ttag not in u1_tags or not (0 < name_ptr < n):
                        break
                    name = self.resolver.resolve_ptr(name_ptr) or f"sym_{sym_hash:08X}"
                    self.resolver.register_symbol(name, sym_hash, name_ptr)
                    entry = DcExportEntry(idx, sym_hash, ttag, name, desc_ptr)
                    self.exports.append(entry)
                    desc_to_export[desc_ptr] = entry
                    pos += 32
                    idx += 1

        # 2. If not U1 export table, check U2 16-byte directory table at 0x28
        if not self.exports and self.num_sections > 0 and 0x28 + self.num_sections * 16 <= n:
            valid_u2 = True
            temp_entries = []
            for idx in range(self.num_sections):
                hdr_off = 0x28 + idx * 16
                h, pad1, desc_off = struct.unpack_from("<IIQ", data, hdr_off)
                if not (0x28 <= desc_off < n):
                    valid_u2 = False
                    break
                name = self.resolver.resolve_hash(h) or f"sym_{h:08X}"
                entry = DcExportEntry(idx, h, pad1, name, desc_off)
                temp_entries.append(entry)
            if valid_u2 and temp_entries:
                self.exports = temp_entries
                for e in temp_entries:
                    desc_to_export[e.desc_offset] = e

        # 3. Discover all Function Descriptors across the binary
        #    - U1 descriptor tag: 0xE134263D followed by (u64 code_start, u64 pool_start, u32 0xC4AB6121/...)
        #    - U2/U3 descriptor tag: 0x9ED499E1 or 0x832A49D8 followed by (u64 code_start, u64 pool_start, u32 0xAB3EB31F/0)
        raw_func_specs: List[Tuple[int, int, int, int, Optional[str], bool]] = []
        seen_code_starts: Set[int] = set()

        for off in range(0x28, n - 24, 4):
            tag = struct.unpack_from("<I", data, off)[0]
            if tag not in (0xE134263D, 0x9ED499E1, 0x832A49D8):
                continue
            # Avoid matching 0x9ED499E1 inside the U1 export table at 0x38..
            if self.is_u1 and off < 0x38 + len(self.exports) * 32:
                continue

            c_start, p_start, end_marker = struct.unpack_from("<QQI", data, off + 4)
            if not (0x38 <= c_start < p_start <= n):
                continue
            if (p_start - c_start) % 4 != 0 or (p_start - c_start) > 0x40000:
                continue

            is_u1_fn = (tag == 0xE134263D) or self.is_u1
            # Validate marker either at end_marker or at c_start - 4
            pre_tag = struct.unpack_from("<I", data, c_start - 4)[0] if c_start >= 4 else 0
            if is_u1_fn:
                if pre_tag != 0x1DB7E10D and end_marker != 0xC4AB6121:
                    continue
            else:
                if end_marker != 0xAB3EB31F and pre_tag != 0x1ED2E0A2 and tag != 0x832A49D8:
                    continue

            if c_start in seen_code_starts:
                continue
            seen_code_starts.add(c_start)

            # Determine function name & hash
            desc_ptr = off + 4
            fn_name = None
            fn_hash = 0
            if desc_ptr in desc_to_export:
                exp = desc_to_export[desc_ptr]
                fn_name = exp.name
                fn_hash = exp.hash
            elif off in desc_to_export:
                exp = desc_to_export[off]
                fn_name = exp.name
                fn_hash = exp.hash
            else:
                # Look backwards for a preceding 0x637BDB3E string tag (U1 inline symbol header)
                scan_back = max(0, off - 64)
                tag_pos = data.rfind(b"\x3e\xdb\x7b\x63", scan_back, off)
                if tag_pos != -1:
                    s = self.resolver.resolve_ptr(tag_pos + 4)
                    if s:
                        fn_name = s
                        fn_hash = nd_string_hash(s)

            if fn_hash == 0 and fn_name:
                fn_hash = nd_string_hash(fn_name)
            elif fn_hash == 0:
                fn_hash = c_start

            raw_func_specs.append((desc_ptr, fn_hash, c_start, p_start, fn_name, is_u1_fn))

        # 4. Also check U3 header top-level at 0x60 if present
        if n >= 0x80 and not self.is_u1:
            # In U3 (e.g., test-tasks.bin), 0x60 has LOAD_SYM r0, const[0]; CALL_NATIVE; RET
            w0, w1, w2 = struct.unpack_from("<III", data, 0x60)
            if (w0 & 0xFF) == 0x15 and (w1 & 0xFF) == 0x1C and (w2 & 0xFF) == 0x00:
                if 0x60 not in seen_code_starts:
                    seen_code_starts.add(0x60)
                    raw_func_specs.append((0x60, 0x87B03989, 0x60, 0x70, "top-level", False))

        # Sort functions by code_start so we know exact boundaries for each constant pool
        raw_func_specs.sort(key=lambda x: x[2])

        for idx, (desc_off, fn_hash, c_start, p_start, fn_name, is_u1_fn) in enumerate(raw_func_specs):
            # If p_start - 4 is a block delimiter (0x1DB7E10D or 0x1ED2E0A2), exclude it from bytecode
            c_end = p_start
            if c_end - 4 >= c_start:
                last_word = struct.unpack_from("<I", data, c_end - 4)[0]
                if last_word in (0x1DB7E10D, 0x1ED2E0A2):
                    c_end -= 4
            # Also strip trailing zero padding after RET
            while c_end - 4 > c_start:
                prev_word = struct.unpack_from("<I", data, c_end - 4)[0]
                prev2_word = struct.unpack_from("<I", data, c_end - 8)[0] if c_end - 8 >= c_start else 1
                if prev_word == 0 and (prev2_word & 0xFF) == 0x00:
                    c_end -= 4
                else:
                    break

            # Determine upper bound of constant pool
            next_block = n
            if idx + 1 < len(raw_func_specs):
                next_c_start = raw_func_specs[idx + 1][2]
                next_block = next_c_start - 4 if next_c_start >= 4 else next_c_start

            raw_pool: List[int] = []
            pool_str: List[str] = []
            curr = p_start
            delimiters = {0xA06D20CB, 0x1ED2E0A2, 0x1DB7E10D, 0x637BDB3E, 0x0B3952E7}

            while curr + 4 <= next_block and len(raw_pool) < 512:
                tag = struct.unpack_from("<I", data, curr)[0]
                if tag in delimiters:
                    break
                if curr + 8 <= next_block:
                    next_tag = struct.unpack_from("<I", data, curr + 4)[0]
                    if next_tag in delimiters:
                        val_u32 = tag
                        raw_pool.append(val_u32)
                        pool_str.append(self.resolver.resolve_constant(val_u32))
                        curr += 4
                        break
                    val_u64 = struct.unpack_from("<Q", data, curr)[0]
                    raw_pool.append(val_u64)
                    pool_str.append(self.resolver.resolve_constant(val_u64))
                    curr += 8
                else:
                    raw_pool.append(tag)
                    pool_str.append(self.resolver.resolve_constant(tag))
                    curr += 4

            self.functions.append(
                DcFunctionDef(
                    idx=idx,
                    func_hash=fn_hash,
                    desc_offset=desc_off,
                    code_start=c_start,
                    code_end=c_end,
                    raw_pool=raw_pool,
                    pool_str=pool_str,
                    name=fn_name,
                    is_u1=is_u1_fn,
                )
            )

        # 5. Parse non-function data definitions from export table
        func_desc_offsets = {fn.desc_offset for fn in self.functions}
        for exp in self.exports:
            if exp.desc_offset in func_desc_offsets or exp.type_tag == 0x9ED499E1:
                continue
            fields = self._extract_data_fields(exp.desc_offset)
            self.data_defs.append(DcDataDef(exp.name, exp.hash, exp.type_tag, exp.desc_offset, fields))

    def _extract_data_fields(self, desc_offset: int, max_fields: int = 24) -> List[str]:
        data = self.data
        n = len(data)
        fields = []
        if not (0 < desc_offset < n - 8):
            return fields

        # Check if this is a (type_tag, count, array_ptr) descriptor like *movie-task-ids* or *task-graph*
        if desc_offset + 20 <= n:
            sub_tag, count, arr_ptr = struct.unpack_from("<IQQ", data, desc_offset)
            if 0 < count <= 256 and 0x40 <= arr_ptr < n:
                fields.append(f":type 0x{sub_tag:08X} :count {count}")
                curr = arr_ptr
                for i in range(min(count, max_fields)):
                    if curr + 16 > n:
                        break
                    v1, v2 = struct.unpack_from("<QQ", data, curr)
                    s1 = self.resolver.resolve_constant(v1)
                    s2 = self.resolver.resolve_constant(v2)
                    fields.append(f"({s1} {s2})")
                    curr += 16
                if count > max_fields:
                    fields.append(f"... ; +{count - max_fields} more entries")
                return fields

        # Generic 64-bit fields up to next 0x637BDB3E tag or delimiter
        curr = desc_offset
        for _ in range(max_fields):
            if curr + 8 > n:
                break
            w_lo, w_hi = struct.unpack_from("<II", data, curr)
            if w_lo in (0x637BDB3E, 0x1DB7E10D, 0x1ED2E0A2, 0xA06D20CB) or w_hi == 0x637BDB3E:
                break
            val_u64 = struct.unpack_from("<Q", data, curr)[0]
            if val_u64 != 0:
                fields.append(self.resolver.resolve_constant(val_u64))
            curr += 8
        return fields


# ==============================================================================
# HIGH-LEVEL LISP DECOMPILER (AST & CONTROL-FLOW RECONSTRUCTION)
# ==============================================================================

def decompile_function_to_lisp(container: DcContainer, func: DcFunctionDef) -> str:
    """
    Decompiles a DC00 bytecode function into readable high-level Lisp source code
    by folding register expressions, resolving symbols/strings/floats from the
    function's constant pool, and structuring conditional jumps into if/when/unless.
    """
    code_chunk = container.data[func.code_start:func.code_end]
    instructions = disassemble_stream(code_chunk, base_pc=0, is_u1=func.is_u1)
    if not instructions:
        return f"(defun {func.name} ()\n  nil)"

    resolver = container.resolver

    def get_const(idx: int, kind: str = "auto") -> str:
        if 0 <= idx < len(func.raw_pool):
            return resolver.resolve_constant(func.raw_pool[idx], kind=kind)
        return f"const[{idx}]"

    # 1. Detect incoming parameters (arg0, arg1, ...) read before written
    written_regs: Set[int] = set()
    param_indices: Set[int] = set()
    for ins in instructions:
        # Check source registers
        srcs = []
        if ins.fmt in (OpFormat.R_R, OpFormat.LOAD_MEM, OpFormat.JUMP_COND, OpFormat.ARITH_IMM):
            srcs.append(ins.b2)
        elif ins.fmt in (OpFormat.R_R_R, OpFormat.STORE_MEM):
            srcs.extend([ins.b2, ins.b3])
        for sr in srcs:
            arg_idx = is_arg_reg(sr)
            if arg_idx is not None and sr not in written_regs:
                param_indices.add(arg_idx)
        # Mark destination register
        if ins.fmt in (
            OpFormat.R_R, OpFormat.R_R_R, OpFormat.R_IMM16, OpFormat.LOAD_CONST,
            OpFormat.LOAD_SYM, OpFormat.LOAD_MEM, OpFormat.ARITH_IMM,
            OpFormat.CALL_NATIVE, OpFormat.CALL_SCRIPT
        ):
            written_regs.add(ins.b1)

    max_param = max(param_indices) + 1 if param_indices else 0
    param_list = " ".join(f"arg{i}" for i in range(max_param))

    # 2. Symbolic Execution & Control-Flow Block Structuring
    reg_val: Dict[int, str] = {}
    reg_const_idx: Dict[int, int] = {}
    for i in range(max_param):
        reg_val[64 + i] = f"arg{i}"
        reg_val[24 + i] = f"arg{i}"

    # Pre-scan jump targets and conditional blocks
    jump_targets: Set[int] = {ins.target_pc for ins in instructions if ins.target_pc is not None}
    pc_to_idx: Dict[int, int] = {ins.pc: idx for idx, ins in enumerate(instructions)}

    def eval_reg(r: int) -> str:
        if r in reg_val:
            return reg_val[r]
        arg_i = is_arg_reg(r)
        if arg_i is not None:
            return f"arg{arg_i}"
        return f"r{r}"

    def decompile_range(start_idx: int, end_idx: int, indent: int = 1) -> List[str]:
        stmts: List[str] = []
        pad = "  " * indent
        i = start_idx

        while i < end_idx:
            ins = instructions[i]
            pc = ins.pc
            op = ins.opcode
            d, s1, s2 = ins.b1, ins.b2, ins.b3

            # Skip trailing padding 00 00 00 00 after an explicit return
            if ins.raw == b"\x00\x00\x00\x00" and i > start_idx and instructions[i - 1].opcode == 0x00:
                i += 1
                continue

            # Handle U1 3-instruction symbol load sequence: 0C (LOAD_IMM16) + 09 (LOAD_POOL) + 15 (LOAD_SYM)
            if (
                func.is_u1
                and op == 0x0C
                and i + 2 < end_idx
                and instructions[i + 1].opcode == 0x09
                and instructions[i + 2].opcode == 0x15
                and instructions[i + 1].b1 == d
                and instructions[i + 2].b1 == d
            ):
                c_idx = ins.imm16()
                reg_const_idx[d] = c_idx
                reg_val[d] = get_const(c_idx, kind="call")
                i += 3
                continue

            if op == 0x0C:  # LOAD_IMM16
                imm = ins.imm16()
                reg_const_idx[d] = imm
                if imm == 0xFFFF:
                    reg_val[d] = "#t"
                elif imm == 0:
                    reg_val[d] = "0"
                else:
                    reg_val[d] = f"{imm}" if imm < 1000 else f"0x{imm:04X}"

            elif op == 0x09:  # LOAD_POOL (U1)
                c_idx = reg_const_idx.get(s1, s1)
                reg_const_idx[d] = c_idx
                reg_val[d] = get_const(c_idx, kind="sym")

            elif op == 0x15:  # LOAD_SYM
                if func.is_u1:
                    c_idx = reg_const_idx.get(s1, s1)
                else:
                    c_idx = ins.imm16()
                reg_val[d] = get_const(c_idx, kind="call")

            elif ins.fmt == OpFormat.LOAD_CONST:
                c_idx = ins.imm16()
                if ins.mnemonic == "LOAD_F32":
                    reg_val[d] = get_const(c_idx, kind="float")
                elif ins.mnemonic in ("LOAD_SYM_C", "LOAD_CONST"):
                    reg_val[d] = get_const(c_idx, kind="auto")
                else:
                    reg_val[d] = get_const(c_idx, kind="auto")

            elif op in (0x16, 0x17, 0x18, 0x43):  # MOV
                val = eval_reg(s1)
                reg_val[d] = val
                if s1 in reg_const_idx:
                    reg_const_idx[d] = reg_const_idx[s1]
                # If moving an incoming arg0.. into local r0.. at function prologue, bind r0 -> arg0
                arg_i = is_arg_reg(s1)
                if arg_i is not None and d < 16 and pc < 8:
                    reg_val[d] = f"arg{arg_i}"

            elif op in (0x1B, 0x1C):  # CALL_DEFUN / CALL_NATIVE
                fn_expr = eval_reg(s1)
                argc = s2
                call_args = []
                base_arg_reg = 64 if (func.is_u1 or (64 in reg_val)) else 24
                for a_i in range(argc):
                    if (64 + a_i) in reg_val and (func.is_u1 or (24 + a_i) not in reg_val):
                        call_args.append(reg_val[64 + a_i])
                    elif (24 + a_i) in reg_val:
                        call_args.append(reg_val[24 + a_i])
                    else:
                        call_args.append(f"arg{a_i}")

                call_str = f"({fn_expr}" + ("" if not call_args else " " + " ".join(call_args)) + ")"
                reg_val[d] = call_str

                # Check if the result register `d` is used in a subsequent instruction before being overwritten
                used_later = False
                for k in range(i + 1, min(end_idx, i + 10)):
                    nxt = instructions[k]
                    if nxt.opcode in (0x2E, 0x2F) and nxt.b2 == d:
                        used_later = True
                        break
                    if nxt.fmt in (OpFormat.R_R, OpFormat.LOAD_MEM, OpFormat.ARITH_IMM) and nxt.b2 == d:
                        used_later = True
                        break
                    if nxt.fmt in (OpFormat.R_R_R, OpFormat.STORE_MEM) and (nxt.b2 == d or nxt.b3 == d):
                        used_later = True
                        break
                    if nxt.opcode == 0x00 and nxt.b1 != 0 and nxt.b2 == d:
                        used_later = True
                        break
                    if nxt.b1 == d and nxt.fmt not in (OpFormat.JUMP, OpFormat.JUMP_COND, OpFormat.STORE_MEM):
                        break

                if not used_later:
                    stmts.append(f"{pad}{call_str}")

            elif op in (0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x27):
                op_sym = {
                    0x01: "+", 0x02: "-", 0x03: "*", 0x04: "/",
                    0x05: "+.f", 0x06: "-.f", 0x07: "*.f", 0x08: "/.f", 0x27: "mod"
                }[op]
                reg_val[d] = f"({op_sym} {eval_reg(s1)} {eval_reg(s2)})"

            elif op in (0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26):
                cmp_sym = {
                    0x1D: "=", 0x1E: ">", 0x1F: ">=", 0x20: "<", 0x21: "<=",
                    0x22: "=.f", 0x23: ">.f", 0x24: ">=.f", 0x25: "<.f", 0x26: "<=.f"
                }[op]
                rhs = eval_reg(s2)
                if rhs == "#t":
                    rhs = "0"
                reg_val[d] = f"({cmp_sym} {eval_reg(s1)} {rhs})"

            elif op in (0x30, 0x32, 0x38, 0x39, 0x19, 0x1A, 0x29, 0x2A):
                u_sym = {
                    0x30: "not", 0x32: "lognot", 0x38: "-", 0x39: "-.f",
                    0x19: "int", 0x1A: "float", 0x29: "abs", 0x2A: "abs.f"
                }[op]
                reg_val[d] = f"({u_sym} {eval_reg(s1)})"

            elif op in (0x31, 0x33, 0x34, 0x36, 0x37):
                b_sym = {0x31: "logand", 0x33: "logior", 0x34: "logxor", 0x36: "and", 0x37: "or"}[op]
                reg_val[d] = f"({b_sym} {eval_reg(s1)} {eval_reg(s2)})"

            elif ins.fmt == OpFormat.ARITH_IMM:
                a_sym = {0x3B: "+", 0x3C: "-", 0x3D: "*", 0x3E: "/"}[op]
                reg_val[d] = f"({a_sym} {eval_reg(s1)} {s2})"

            elif ins.fmt == OpFormat.LOAD_MEM:
                reg_val[d] = f"(-> {eval_reg(s1)})"

            elif ins.fmt == OpFormat.STORE_MEM:
                stmts.append(f"{pad}(set! (-> {eval_reg(s1)}) {eval_reg(s2)})")

            elif op in (0x2F, 0x2E):  # JZ / JNZ conditional branch
                cond_expr = eval_reg(s1)
                if op == 0x2E:
                    cond_expr = f"(not {cond_expr})"
                target_pc = ins.target_pc
                target_idx = pc_to_idx.get(target_pc, -1)

                # Forward branch within current range -> structured when / if-else
                if target_idx > i and target_idx <= end_idx:
                    # Unconditional "(if #t ...)" wrapper at start of U3 state blocks
                    if cond_expr == "#t":
                        inner = decompile_range(i + 1, target_idx, indent)
                        stmts.extend(inner)
                        i = target_idx
                        continue

                    # Check if the instruction right before target_idx is an unconditional JMP (else branch)
                    prev_ins = instructions[target_idx - 1]
                    if prev_ins.opcode == 0x2D and prev_ins.target_pc is not None:
                        else_end_idx = pc_to_idx.get(prev_ins.target_pc, -1)
                        if else_end_idx >= target_idx and else_end_idx <= end_idx:
                            then_stmts = decompile_range(i + 1, target_idx - 1, indent + 1)
                            if else_end_idx == target_idx:
                                # JMP jumps to the exact same target -> simple (when cond ...)
                                stmts.append(f"{pad}(when {cond_expr}")
                                stmts.extend(then_stmts or [f"{pad}  nil"])
                                stmts[-1] += ")"
                                i = target_idx
                                continue
                            else:
                                else_stmts = decompile_range(target_idx, else_end_idx, indent + 1)
                                stmts.append(f"{pad}(if {cond_expr}")
                                if len(then_stmts) <= 1:
                                    stmts.extend(then_stmts or [f"{pad}  nil"])
                                else:
                                    stmts.append(f"{pad}  (begin")
                                    for ts in then_stmts:
                                        stmts.append("  " + ts)
                                    stmts[-1] += ")"
                                if len(else_stmts) == 1:
                                    stmts.extend(else_stmts)
                                else:
                                    stmts.append(f"{pad}  (begin")
                                    for es in else_stmts:
                                        stmts.append("  " + es)
                                    stmts[-1] += ")"
                                stmts[-1] += ")"
                                i = else_end_idx
                                continue

                    # Standard (when cond ...) block
                    then_stmts = decompile_range(i + 1, target_idx, indent + 1)
                    stmts.append(f"{pad}(when {cond_expr}")
                    stmts.extend(then_stmts or [f"{pad}  nil"])
                    stmts[-1] += ")"
                    i = target_idx
                    continue
                else:
                    stmts.append(f"{pad}(when (not {cond_expr}) (goto loc_{target_pc:04X}))")

            elif op == 0x2D:  # Unconditional JMP
                if ins.target_pc == 0:
                    stmts.append(f"{pad}(loop-continue)")
                elif ins.target_pc is not None and pc_to_idx.get(ins.target_pc, -1) != i + 1:
                    stmts.append(f"{pad}(goto loc_{ins.target_pc:04X})")

            elif op == 0x00:  # RET
                if d != 0:
                    ret_val = eval_reg(s1)
                    if ret_val == "0":
                        ret_val = "#f"
                    stmts.append(f"{pad}{ret_val}")
                elif i < end_idx - 1:
                    stmts.append(f"{pad}(return)")

            i += 1

        return stmts

    body_lines = decompile_range(0, len(instructions), indent=1)
    if not body_lines:
        body_lines = ["  nil"]

    header = f"(defun {func.name} ({param_list})"
    result = [header] + body_lines
    result[-1] += ")"
    return "\n".join(result)


# ==============================================================================
# FORMATTING & CLI DISASSEMBLY
# ==============================================================================

def disassemble_stream(data: bytes, base_pc: int = 0, is_u1: bool = False) -> List[Instruction]:
    instructions: List[Instruction] = []
    count = len(data) // 4
    for i in range(count):
        chunk = data[i * 4:(i + 1) * 4]
        instructions.append(Instruction(base_pc + i, chunk, is_u1=is_u1))
    return instructions


def format_data_def(ddef: DcDataDef) -> str:
    kind = "defscenario" if ddef.type_tag == 0xCABD357F else "defsymbol"
    if not ddef.fields:
        return f"({kind} {ddef.name} ; [0x{ddef.offset:06X}] (Hash: 0x{ddef.hash:08X})\n  nil)\n"
    lines = [f"({kind} {ddef.name} ; [0x{ddef.offset:06X}] (Hash: 0x{ddef.hash:08X})"]
    for f in ddef.fields:
        lines.append(f"  {f}")
    lines[-1] += ")\n"
    return "\n".join(lines)


def format_function_disassembly(container: DcContainer, func: DcFunctionDef, show_lisp: bool = True) -> str:
    lines = []
    lines.append(";" * 86)
    lines.append(
        f";; FUNCTION #{func.idx}: {func.name} "
        f"(Hash: 0x{func.hash:08X}, Offset: 0x{func.code_start:06X}..0x{func.code_end:06X}, Size: {func.code_size}b)"
    )
    if func.pool:
        lines.append(f";; CONSTANT POOL ({len(func.pool)} entries):")
        for ci, cval in enumerate(func.pool):
            raw_v = func.raw_pool[ci]
            lines.append(f";;   const[{ci:2d}] = {cval:<36} ; raw: 0x{raw_v:08X}")
    lines.append(";" * 86)

    if show_lisp:
        lines.append("")
        lines.append(";; ── DECOMPILED LISP SOURCE ─────────────────────────────────────────────────────────")
        lines.append(decompile_function_to_lisp(container, func))
        lines.append("")
        lines.append(";; ── BYTECODE DISASSEMBLY ───────────────────────────────────────────────────────────")

    code_chunk = container.data[func.code_start:func.code_end]
    instructions = disassemble_stream(code_chunk, base_pc=0, is_u1=func.is_u1)
    labels = {ins.target_pc for ins in instructions if ins.target_pc is not None}

    for ins in instructions:
        if ins.pc in labels:
            lines.append(f";; loc_{ins.pc:04X}:")
        hex_str = " ".join(f"{b:02X}" for b in ins.raw)
        asm_str = ins.format_asm(pool=func.pool)
        semantics = ins.to_lisp(pool=func.pool) if show_lisp else ins.desc
        lines.append(f";;   [{ins.pc:04X}]  {hex_str:<11}  {asm_str:<42}  {semantics}")

    lines.append("")
    return "\n".join(lines)


def disassemble_file(path: str, offset: int = 0, count: Optional[int] = None, show_lisp: bool = True):
    with open(path, "rb") as f:
        data = f.read()

    container = DcContainer(data)
    if container.is_dc and offset == 0:
        print(f"Container: Naughty Dog DC00 (Version: {container.version}, File Size: {len(data):,} bytes)")
        print(f"Exports: {len(container.exports)} | Functions: {len(container.functions)} | Data Defs: {len(container.data_defs)}")
        print(f"Unique strings/symbols discovered: {len(container.resolver.offset_to_str)}")
        print()

        if container.data_defs:
            print(";; ── EXPORTED DATA / SCENARIO DEFINITIONS ───────────────────────────────────────────")
            for ddef in container.data_defs:
                print(format_data_def(ddef))

        if container.functions:
            for fn in container.functions:
                print(format_function_disassembly(container, fn, show_lisp=show_lisp))
        elif not container.data_defs:
            print("No structured functions or data definitions found.")
    else:
        if offset:
            data = data[offset:]
        if count:
            data = data[:count * 4]
        instructions = disassemble_stream(data, base_pc=offset // 4)
        print_disassembly(instructions, show_lisp=show_lisp)


def print_disassembly(instructions: List[Instruction], pool: Optional[List[str]] = None, show_lisp: bool = False):
    labels = {ins.target_pc for ins in instructions if ins.target_pc is not None}
    print("=" * 80)
    print(f"{'PC / ADDR':<12} {'RAW HEX':<12} {'ASSEMBLY':<34} {'SEMANTICS / LISP'}")
    print("=" * 80)
    for ins in instructions:
        if ins.pc in labels:
            print(f"\nloc_{ins.pc:04X}:")
        hex_str = " ".join(f"{b:02X}" for b in ins.raw)
        asm_str = ins.format_asm(pool=pool)
        semantics = ins.to_lisp(pool=pool) if show_lisp else f";; {ins.desc}"
        print(f"[{ins.pc:04X} / +{ins.pc*4:04X}]  {hex_str:<10}  {asm_str:<32}  {semantics}")
    print("=" * 80)


def scan_for_bytecode(data: bytes, min_instructions: int = 12) -> List[Tuple[int, int]]:
    container = DcContainer(data)
    if container.functions:
        return [(fn.code_start, fn.code_size // 4) for fn in container.functions]
    candidates = []
    data_len = len(data)
    valid_ops = set(OPCODES.keys())
    i = 0
    while i <= data_len - 16:
        op = data[i]
        if op in valid_ops and op < 0x50:
            seq_len = 0
            cur = i
            while cur <= data_len - 4:
                curr_op = data[cur]
                if curr_op in valid_ops and curr_op < 0x50:
                    seq_len += 1
                    cur += 4
                    if curr_op == 0x00 and seq_len >= min_instructions:
                        break
                else:
                    break
            if seq_len >= min_instructions:
                candidates.append((i, seq_len))
                i = cur
                continue
        i += 4
    return candidates


# ==============================================================================
# ENTRY POINT & CLI
# ==============================================================================

def main():
    normalized_argv = []
    for arg in sys.argv[1:]:
        if arg.startswith("---"):
            normalized_argv.append("--" + arg.lstrip("-"))
        else:
            normalized_argv.append(arg)

    parser = argparse.ArgumentParser(description="Naughty Dog DC Bytecode Disassembler & Lisp Decompiler")
    parser.add_argument("--file", "-f", help="Path to .bin script or dumped bytecode")
    parser.add_argument("--offset", "-o", type=lambda x: int(x, 0), default=0, help="Start offset in file")
    parser.add_argument("--count", "-c", type=int, default=None, help="Number of instructions to disassemble")
    parser.add_argument("--hex", "-x", help="Raw hex string to disassemble")
    parser.add_argument("--lisp", "-l", action="store_true", default=True, help="Reconstruct high-level Lisp S-expressions")
    parser.add_argument("--no-lisp", action="store_false", dest="lisp", help="Disable Lisp decompilation")
    parser.add_argument("--scan", "-s", help="Scan binary file for DC script functions")
    args = parser.parse_args(normalized_argv)

    if args.hex:
        raw = bytes.fromhex(args.hex.replace(" ", "").replace(",", ""))
        instructions = disassemble_stream(raw)
        print_disassembly(instructions, show_lisp=args.lisp)
        return

    if args.scan:
        with open(args.scan, "rb") as f:
            content = f.read()
        print(f"Scanning {args.scan} ({len(content):,} bytes) for DC script functions...")
        results = scan_for_bytecode(content, min_instructions=10)
        print(f"\nFound {len(results)} function blocks:")
        for offset, count in results[:100]:
            print(f"  - Offset 0x{offset:08X} (+{offset:<8}): {count:3d} instructions ({count * 4} bytes)")
        return

    if args.file:
        disassemble_file(args.file, offset=args.offset, count=args.count, show_lisp=args.lisp)
        return

    print("Usage: py tools/dc_disasm.py --file <script.bin> [--lisp]")

if __name__ == "__main__":
    main()
