#!/usr/bin/env python3
"""
Uncharted 2 (big2-ps4_Shipping.elf) C++ Engine & Symbol Extractor
=================================================================
Unpacks the PS4 SELF/ELF executable for Uncharted 2: Among Thieves,
reconstructs the guest virtual memory image (matching src/probe.c offsets),
and extracts:
  1. Original Naughty Dog C++ source tree (.cpp / .h paths from assertions/logs)
  2. Functions mapped to each .cpp file via RIP-relative LEA cross-references
  3. The DC00 Script VM (ExecuteScriptCode) address and signature in Uncharted 2
  4. Native C++ script functions (Dc*) addresses, hashes, and pointer tables
  5. Ready-to-use C header (u2_guest_hooks.h) for hooking/rewriting functions in src/

Usage:
    py tools/u2_elf_extract.py "C:\\Users\\Yohan\\Desktop\\Uncharted\\CUSA02343"
"""

import sys
import os
import re
import json
import struct
import argparse
from pathlib import Path
from datetime import datetime
from typing import Dict, List, Tuple, Optional, Set, Any

TOOLS_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS_DIR))

from dc_disasm import nd_string_hash, _camel_to_kebab_variants


# ==============================================================================
# 1. PS4 SELF / ELF UNPACKER (MATCHING scripts/prepare.py & src/probe.c)
# ==============================================================================

def load_ps4_executable(exe_path: Path) -> Tuple[bytearray, int, List[Dict[str, int]]]:
    """
    Loads a PS4 SELF (O\\x15=\\x1d) or raw ELF (\\x7fELF) into a flat virtual memory
    image where offset == vaddr (identical to image + offset in src/probe.c).
    Returns (image, entry_vaddr, load_segments).
    """
    raw = exe_path.read_bytes()
    magic = raw[:4]

    if magic == b"O\x15=\x1d":
        count = struct.unpack_from("<H", raw, 24)[0]
        base = 32 + count * 32
        header = struct.unpack_from("<16sHHIQQQIHHHHHH", raw, base)
        if header[0][:7] != b"\x7fELF\x02\x01\x01" or header[2] != 62:
            raise ValueError("Expected little-endian x86-64 ELF inside SELF")
        ph_off = header[5]
        ph_num = header[10]
        ph = [
            dict(
                zip(
                    ("type", "flags", "offset", "vaddr", "paddr", "filesz", "memsz", "align"),
                    struct.unpack_from("<IIQQQQQQ", raw, base + ph_off + i * 56),
                )
            )
            for i in range(ph_num)
        ]
        end = max(p["offset"] + p["filesz"] for p in ph)
        elf = bytearray(end)
        header_end = ph_off + ph_num * 56
        elf[:header_end] = raw[base : base + header_end]
        for i in range(count):
            flags, off, size, mem = struct.unpack_from("<QQQQ", raw, 32 + i * 32)
            if not (flags & 0x800):
                continue
            idx = (flags >> 20) & 4095
            if idx < len(ph):
                p = ph[idx]
                if size == p["filesz"]:
                    elf[p["offset"] : p["offset"] + size] = raw[off : off + size]
        entry_vaddr = header[4]
    elif magic == b"\x7fELF":
        elf = bytearray(raw)
        header = struct.unpack_from("<16sHHIQQQIHHHHHH", elf, 0)
        ph_off = header[5]
        ph_num = header[10]
        ph = [
            dict(
                zip(
                    ("type", "flags", "offset", "vaddr", "paddr", "filesz", "memsz", "align"),
                    struct.unpack_from("<IIQQQQQQ", elf, ph_off + i * 56),
                )
            )
            for i in range(ph_num)
        ]
        entry_vaddr = header[4]
    else:
        raise ValueError(f"Unsupported executable format (magic={magic!r})")

    loads = [p for p in ph if p["type"] in (1, 0x61000010)]
    image_size = max(p["vaddr"] + p["memsz"] for p in loads)
    image = bytearray(image_size)
    for p in loads:
        vaddr = p["vaddr"]
        filesz = p["filesz"]
        off = p["offset"]
        image[vaddr : vaddr + filesz] = elf[off : off + filesz]

    return image, entry_vaddr, loads


# ==============================================================================
# 2. STRING, SOURCE PATH (.cpp/.h) & DC NATIVE SCANNER
# ==============================================================================

SOURCE_PATH_RE = re.compile(
    r"^(?:[A-Za-z0-9_.\-:]+[/\\])*[A-Za-z0-9_.\-]+\.(?:cpp|c|cc|h|hpp|inl|dc|asm)$",
    re.IGNORECASE,
)
DC_NATIVE_RE = re.compile(r"^Dc[A-Z][A-Za-z0-9_]{2,64}$")


def scan_strings_in_image(image: bytearray, loads: List[Dict[str, int]]):
    """
    Scan non-executable loadable segments (and rodata inside segments) for ASCII strings:
      - C++ source file paths (.cpp, .h, .inl, .dc)
      - Dc* native C++ function names
      - General printable strings (assertions, class names, debug messages)
    """
    all_strings: Dict[int, str] = {}
    source_files: Dict[str, List[int]] = {}
    dc_natives: Dict[str, int] = {}

    n = len(image)
    for seg in loads:
        start = seg["vaddr"]
        end = min(n, seg["vaddr"] + seg["filesz"])
        i = start
        while i < end - 4:
            b = image[i]
            if 0x20 <= b <= 0x7E:
                j = i
                while j < end and 0x20 <= image[j] <= 0x7E:
                    j += 1
                length = j - i
                if 3 <= length <= 260 and j < end and image[j] == 0:
                    s = image[i:j].decode("ascii", errors="ignore")
                    all_strings[i] = s

                    # Check if it's a C++ source path (or contains one)
                    cleaned = s.strip()
                    if SOURCE_PATH_RE.match(cleaned):
                        norm_path = cleaned.replace("\\", "/")
                        source_files.setdefault(norm_path, []).append(i)
                    elif ".cpp" in cleaned or ".h" in cleaned:
                        for token in cleaned.split():
                            tok = token.strip(":(),\"'")
                            if SOURCE_PATH_RE.match(tok):
                                norm_path = tok.replace("\\", "/")
                                source_files.setdefault(norm_path, []).append(i)

                    # Check if it's a Dc* native C++ function name
                    if DC_NATIVE_RE.match(cleaned):
                        dc_natives[cleaned] = i

                i = j + 1
            else:
                i += 1

    return all_strings, source_files, dc_natives


# ==============================================================================
# 3. X86-64 FUNCTION BOUNDARY & RIP-RELATIVE XREF ANALYZER
# ==============================================================================

def find_functions_and_xrefs(
    image: bytearray,
    exec_segments: List[Dict[str, int]],
    all_strings: Dict[int, str],
):
    """
    1. Locate all x86-64 function start offsets in executable segments.
    2. Scan all RIP-relative LEA instructions (48/4C 8D modrm disp32) and
       map target string addresses back to the function containing the LEA.
    3. Locate the DC00 Script VM (ExecuteScriptCode) via its opcode switch pattern.
    """
    func_starts: List[int] = []
    for seg in exec_segments:
        s_start = seg["vaddr"]
        s_end = min(len(image), seg["vaddr"] + seg["filesz"])

        # Standard frame-pointer prologues (push rbp; mov rbp, rsp = 55 48 89 E5)
        # and common non-leaf prologues after 16-byte padding (CC / 90)
        pos = s_start
        while pos < s_end - 8:
            if (
                image[pos] == 0x55
                and image[pos + 1] == 0x48
                and image[pos + 2] == 0x89
                and image[pos + 3] == 0xE5
            ):
                func_starts.append(pos)
                pos += 4
                continue
            pos += 1

    func_starts.sort()

    # Fast binary search helper to find enclosing function for any code address
    import bisect

    def get_enclosing_func(code_addr: int) -> Optional[int]:
        idx = bisect.bisect_right(func_starts, code_addr) - 1
        if idx >= 0:
            fn_addr = func_starts[idx]
            if code_addr - fn_addr < 0x20000:
                return fn_addr
        return None

    # Scan for RIP-relative LEA:
    #   REX (0x48 or 0x4C) + 0x8D + ModRM (mod=00, rm=101 -> (modrm & 0xC7) == 0x05) + disp32
    string_xrefs: Dict[int, List[Tuple[int, int]]] = {}  # str_addr -> [(insn_addr, func_addr)]
    func_to_strings: Dict[int, List[str]] = {}
    assertions_list: List[Dict[str, Any]] = []
    func_cpp_names: Dict[int, Tuple[str, str, int]] = {}  # func_addr -> (cpp_file, cpp_func, line)

    for seg in exec_segments:
        s_start = seg["vaddr"]
        s_end = min(len(image), seg["vaddr"] + seg["filesz"]) - 7
        # Use regex search on bytes for ultra-fast scanning across ~20 MB of code
        pattern = re.compile(b"[\x48\x4c]\x8d[\x05\x0d\x15\x1d\x25\x2d\x35\x3d]", re.DOTALL)
        for m in pattern.finditer(image, s_start, s_end):
            at = m.start()
            disp = struct.unpack_from("<i", image, at + 3)[0]
            target = at + 7 + disp
            if target in all_strings:
                fn = get_enclosing_func(at) or at
                string_xrefs.setdefault(target, []).append((at, fn))
                s_val = all_strings[target]
                lst = func_to_strings.setdefault(fn, [])
                if s_val not in lst:
                    lst.append(s_val)

        # Also scan for Naughty Dog assertion traps (FF D0 CD 41 = call rax; int 0x41)
        trap_pat = re.compile(b"\xff\xd0\xcd\x41", re.DOTALL)
        for tm in trap_pat.finditer(image, s_start, s_end):
            trap_at = tm.start()
            win_lo = max(s_start, trap_at - 48)
            fn = get_enclosing_func(trap_at) or trap_at
            regs_str: Dict[int, str] = {}
            for lm in pattern.finditer(image, win_lo, trap_at):
                lat = lm.start()
                modrm = image[lat + 2]
                reg_id = (modrm >> 3) & 7
                disp = struct.unpack_from("<i", image, lat + 3)[0]
                t_str = all_strings.get(lat + 7 + disp)
                if t_str:
                    regs_str[reg_id] = t_str
            # Check for mov r8d, imm32 (41 B8 imm32) or mov ecx, imm32 (B9 imm32) for line number
            line_num = 0
            for back in range(trap_at - 5, win_lo, -1):
                if image[back] == 0x41 and image[back + 1] == 0xB8 and back + 6 <= trap_at:
                    cand = struct.unpack_from("<I", image, back + 2)[0]
                    if 0 < cand < 50000:
                        line_num = cand
                        break
                elif image[back] == 0xB9 and back + 5 <= trap_at:
                    cand = struct.unpack_from("<I", image, back + 1)[0]
                    if 0 < cand < 50000:
                        line_num = cand
                        break
            # In Naughty Dog PS4 AssertFailed: rdi(7)=expr, rsi(6)=file, rdx(2)=func, rcx(1)=msg, r8d=line
            expr_str = regs_str.get(7, "")
            file_str = regs_str.get(6, "")
            func_str = regs_str.get(2, "")
            msg_str = regs_str.get(1, "")
            if file_str or func_str:
                assertions_list.append({
                    "trap_offset": f"0x{trap_at:08X}",
                    "function_offset": f"0x{fn:08X}",
                    "cpp_file": file_str,
                    "cpp_function": func_str,
                    "line": line_num,
                    "expression": expr_str,
                    "message": msg_str,
                })
                if fn not in func_cpp_names and func_str:
                    func_cpp_names[fn] = (file_str, func_str, line_num)

    return func_starts, string_xrefs, func_to_strings, get_enclosing_func, assertions_list, func_cpp_names


# ==============================================================================
# 4. LOCATE DC00 VM (ExecuteScriptCode) & NATIVE FUNCTION TABLE
# ==============================================================================

def find_dc_vm_and_natives(
    image: bytearray,
    loads: List[Dict[str, int]],
    exec_segments: List[Dict[str, int]],
    dc_natives: Dict[str, int],
    string_xrefs: Dict[int, List[Tuple[int, int]]],
    get_enclosing_func,
):
    """
    1. Finds ExecuteScriptCode (the DC00 bytecode interpreter) in big2-ps4_Shipping.elf.
    2. Finds native C++ function implementations for Dc* symbols via:
       - Direct pointer tables in .data/.rodata: (str_ptr, func_ptr) or (hash, func_ptr)
       - Registration calls in .text where LEA loads a Dc* string and another LEA/MOV loads the function pointer.
    """
    exec_ranges = [(s["vaddr"], s["vaddr"] + s["filesz"]) for s in exec_segments]

    def is_exec_addr(addr: int) -> bool:
        return any(lo <= addr < hi for lo, hi in exec_ranges)

    # A. Locate ExecuteScriptCode:
    # Look for functions with a dense jump table or switch over ~70-78 opcodes (cmp reg, 0x4D / 0x4E)
    vm_candidates: List[Dict[str, Any]] = []
    for seg in exec_segments:
        s_start = seg["vaddr"]
        s_end = min(len(image), seg["vaddr"] + seg["filesz"]) - 16
        # Search for cmp al/cl/dl/bl/r8b.., 0x4D or 0x4E (80 F8..FF 4D/4E or 83 F8..FF 4D/4E)
        for m in re.finditer(b"[\x80\x83][\xf8-\xff][\x49-\x50]", image[s_start:s_end]):
            at = s_start + m.start()
            fn = get_enclosing_func(at)
            if fn is not None:
                vm_candidates.append({
                    "function_offset": f"0x{fn:08X}",
                    "switch_cmp_offset": f"0x{at:08X}",
                    "max_opcode": image[at + 2],
                    "prologue_hex": image[fn : fn + 16].hex(),
                })

    # Deduplicate VM candidates by function_offset
    seen_vm = set()
    unique_vm_candidates = []
    for c in vm_candidates:
        if c["function_offset"] not in seen_vm:
            seen_vm.add(c["function_offset"])
            unique_vm_candidates.append(c)

    # B. Resolve Dc* Native C++ Function Addresses
    # Method 1: Registration in .text (LEA reg1, "Dc..." followed within +-48 bytes by LEA reg2, sub_XXXX)
    resolved_natives: Dict[str, Dict[str, Any]] = {}
    lea_pattern = re.compile(b"[\x48\x4c]\x8d[\x05\x0d\x15\x1d\x25\x2d\x35\x3d]", re.DOTALL)

    for name, str_addr in dc_natives.items():
        kebab_variants = _camel_to_kebab_variants(name)
        preferred_lisp = kebab_variants[2] if len(kebab_variants) > 2 else name
        for v in kebab_variants:
            if v.endswith("?") or v.endswith("!"):
                preferred_lisp = v
                break
        sym_hash = nd_string_hash(preferred_lisp)

        func_addr = None
        reg_site = None

        # Check code xrefs to this Dc* string
        for insn_addr, enclosing_fn in string_xrefs.get(str_addr, []):
            win_start = max(0, insn_addr - 32)
            win_end = min(len(image) - 7, insn_addr + 48)
            for m in lea_pattern.finditer(image, win_start, win_end):
                cand_at = m.start()
                if cand_at == insn_addr:
                    continue
                disp = struct.unpack_from("<i", image, cand_at + 3)[0]
                target = cand_at + 7 + disp
                if is_exec_addr(target):
                    func_addr = target
                    reg_site = insn_addr
                    break
            if func_addr:
                break

        resolved_natives[name] = {
            "cpp_name": name,
            "lisp_symbol": preferred_lisp,
            "symbol_hash": f"0x{sym_hash:08X}",
            "string_offset": f"0x{str_addr:08X}",
            "func_offset": f"0x{func_addr:08X}" if func_addr else None,
            "registration_site": f"0x{reg_site:08X}" if reg_site else None,
            "prologue_hex": image[func_addr : func_addr + 16].hex() if func_addr else None,
        }

    # Method 2: Scan data segments for pointer pairs (str_addr, func_addr) or (sym_hash, func_addr)
    str_addr_to_name = {addr: name for name, addr in dc_natives.items()}
    hash_to_name = {}
    for name in dc_natives:
        for v in _camel_to_kebab_variants(name):
            hash_to_name[nd_string_hash(v)] = name

    for seg in loads:
        if seg["flags"] & 1:  # Skip executable segment
            continue
        s_start = seg["vaddr"]
        s_end = min(len(image) - 16, seg["vaddr"] + seg["filesz"] - 16)
        for pos in range(s_start, s_end, 8):
            q0, q1 = struct.unpack_from("<QQ", image, pos)
            if q0 in str_addr_to_name and is_exec_addr(q1):
                name = str_addr_to_name[q0]
                if not resolved_natives[name]["func_offset"]:
                    resolved_natives[name]["func_offset"] = f"0x{q1:08X}"
                    resolved_natives[name]["registration_site"] = f"0x{pos:08X}"
                    resolved_natives[name]["prologue_hex"] = image[q1 : q1 + 16].hex()
            elif (q0 & 0xFFFFFFFF) in hash_to_name and is_exec_addr(q1):
                name = hash_to_name[q0 & 0xFFFFFFFF]
                if name in resolved_natives and not resolved_natives[name]["func_offset"]:
                    resolved_natives[name]["func_offset"] = f"0x{q1:08X}"
                    resolved_natives[name]["registration_site"] = f"0x{pos:08X}"
                    resolved_natives[name]["prologue_hex"] = image[q1 : q1 + 16].hex()

    return unique_vm_candidates, resolved_natives


# ==============================================================================
# 5. PROLOGUE HELPER FOR RuntimeGuestCallback (src/probe.c)
# ==============================================================================

def prologue_moved_bytes(code: bytes) -> int:
    """Matches prologue_bytes() in src/probe.c for safe instruction relocation."""
    if len(code) < 11:
        return 0
    if code[:4] == b"\x55\x48\x89\xe5":
        if code[4] == 0x41 and 0x50 <= code[5] <= 0x57:
            return 6
        if 0x50 <= code[4] <= 0x57:
            return 5
        if code[4] == 0x48 and code[5] == 0x83 and code[6] == 0xEC:
            return 8
        if code[4] == 0x48 and code[5] == 0x81 and code[6] == 0xEC:
            return 11
    return 0


def decompile_dc_native_x64(
    name: str,
    lisp_sym: str,
    sym_hash: str,
    fn_off: int,
    image: bytearray,
    func_cpp_names: Dict[int, Tuple[str, str, int]],
) -> str:
    """
    Decompiles an x86-64 Dc* native function in big2-ps4_Shipping.elf into C++ code.
    Calling convention in Uncharted 2 DC VM:
      int64_t DcFunc(int32_t argc /* edi */, uint64_t *argv /* rsi */)
    """
    code = bytes(image[fn_off : min(len(image), fn_off + 256)])
    lines = []
    lines.append(f"// Lisp: ({lisp_sym} ...) | StringId: {sym_hash} | Guest Offset: 0x{fn_off:08X}")
    if fn_off in func_cpp_names:
        cfile, cfunc, cline = func_cpp_names[fn_off]
        lines.append(f"// Original Source: {cfile}:{cline} ({cfunc})")
    lines.append(f"int64_t {name}(int32_t argc, uint64_t *argv, uint8_t *image) {{")

    # Pattern 1: 31 C0 C3 (xor eax, eax; ret -> no-op / debug stripped in Shipping)
    if code[:3] == b"\x31\xc0\xc3":
        lines.append("    (void)argc; (void)argv; (void)image;")
        lines.append("    return 0; // No-op in Shipping build")
        lines.append("}\n")
        return "\n".join(lines)

    # Pattern 2: 48 8B 06 8B 40 disp8 C3 (mov rax, [rsi]; mov eax, [rax + disp8]; ret)
    if code[:5] == b"\x48\x8b\x06\x8b\x40" and code[6] == 0xC3:
        disp = code[5]
        lines.append("    (void)argc; (void)image;")
        lines.append(f"    uint8_t *obj = (uint8_t *)(uintptr_t)argv[0];")
        lines.append(f"    return *(uint32_t *)(obj + 0x{disp:02X});")
        lines.append("}\n")
        return "\n".join(lines)

    # Pattern 3: C7 05 disp32 imm32 B8 imm32 C3 (mov dword [rip+disp32], imm32; mov eax, imm32; ret)
    if code[:2] == b"\xc7\x05" and code[10] == 0xB8 and code[15] == 0xC3:
        disp = struct.unpack_from("<i", code, 2)[0]
        val = struct.unpack_from("<I", code, 6)[0]
        ret_v = struct.unpack_from("<I", code, 11)[0]
        g_off = fn_off + 10 + disp
        lines.append("    (void)argc; (void)argv;")
        lines.append(f"    *(uint32_t *)(image + 0x{g_off:08X}) = 0x{val:08X}u;")
        lines.append(f"    return {ret_v};")
        lines.append("}\n")
        return "\n".join(lines)

    # Pattern 4: C6 05 disp32 imm8 31 C0 C3 (mov byte [rip+disp32], imm8; xor eax, eax; ret)
    if code[:2] == b"\xc6\x05" and code[7:10] == b"\x31\xc0\xc3":
        disp = struct.unpack_from("<i", code, 2)[0]
        val = code[6]
        g_off = fn_off + 7 + disp
        lines.append("    (void)argc; (void)argv;")
        lines.append(f"    *(uint8_t *)(image + 0x{g_off:08X}) = 0x{val:02X}u;")
        lines.append("    return 0;")
        lines.append("}\n")
        return "\n".join(lines)

    # Pattern 5: 48 8D 05 disp32 C6 40 disp8 imm8 31 C0 C3
    if code[:2] == b"\x48\x8d" and code[2] == 0x05 and code[7] == 0xC6 and code[8] == 0x40 and code[11:14] == b"\x31\xc0\xc3":
        disp = struct.unpack_from("<i", code, 3)[0]
        field_off = code[9]
        val = code[10]
        g_off = fn_off + 7 + disp + field_off
        lines.append("    (void)argc; (void)argv;")
        lines.append(f"    *(uint8_t *)(image + 0x{g_off:08X}) = 0x{val:02X}u;")
        lines.append("    return 0;")
        lines.append("}\n")
        return "\n".join(lines)

    # Pattern 6: 48 8D 05 disp32 0F B6 80 disp32 C3 (lea rax, [rip+disp32]; movzx eax, byte [rax+disp32]; ret)
    if code[:3] == b"\x48\x8d\x05" and code[7:9] == b"\x0f\xb6":
        disp1 = struct.unpack_from("<i", code, 3)[0]
        base_g = fn_off + 7 + disp1
        if code[9] == 0x80:
            disp2 = struct.unpack_from("<i", code, 10)[0]
            lines.append("    (void)argc; (void)argv;")
            lines.append(f"    return *(uint8_t *)(image + 0x{base_g + disp2:08X});")
            lines.append("}\n")
            return "\n".join(lines)

    # General fallback: emit call to original guest function + first 32 bytes hex dump
    hex_preview = " ".join(f"{b:02X}" for b in code[:24])
    lines.append(f"    // Prologue bytes: {hex_preview}")
    lines.append(f"    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);")
    lines.append(f"    return ((GuestFn)(image + 0x{fn_off:08X}ull))(argc, argv);")
    lines.append("}\n")
    return "\n".join(lines)


# ==============================================================================
# 6. MAIN EXTRACTION & REPORT GENERATOR
# ==============================================================================

def extract_u2_engine(game_dir: Path, output_dir: Path):
    exe_path = game_dir / "big2-ps4_Shipping.elf"
    if not exe_path.exists():
        print(f"[ERROR] Uncharted 2 executable not found: {exe_path}")
        sys.exit(1)

    print(f"[1/5] Loading Uncharted 2 PS4 executable: {exe_path.name} ({exe_path.stat().st_size:,} bytes)...")
    image, entry_vaddr, loads = load_ps4_executable(exe_path)
    exec_segments = [p for p in loads if p["flags"] & 1]
    print(f"      Virtual image size: {len(image):,} bytes | Entry point: 0x{entry_vaddr:08X} | Load segments: {len(loads)}")

    print("[2/5] Scanning image for Naughty Dog C++ source paths (.cpp/.h) and Dc* native symbols...")
    all_strings, source_files, dc_natives = scan_strings_in_image(image, loads)

    # Also merge known natives from tools/dc_natives.json
    natives_json_path = TOOLS_DIR / "dc_natives.json"
    if natives_json_path.exists():
        try:
            existing_natives = json.loads(natives_json_path.read_text(encoding="utf-8"))
            for n in existing_natives:
                dc_natives.setdefault(n, 0)
        except Exception:
            pass

    print(f"      Strings found: {len(all_strings):,} | C++ source files: {len(source_files):,} | Dc* natives: {len(dc_natives):,}")

    print("[3/5] Analyzing x86-64 functions, RIP-relative xrefs, and Naughty Dog assertions (int 0x41)...")
    func_starts, string_xrefs, func_to_strings, get_enclosing_func, assertions_list, func_cpp_names = find_functions_and_xrefs(
        image, exec_segments, all_strings
    )
    print(
        f"      Functions: {len(func_starts):,} | Functions with string xrefs: {len(func_to_strings):,} | "
        f"Assertions (with C++ func & line): {len(assertions_list):,}"
    )

    print("[4/5] Locating DC00 VM (ExecuteScriptCode) and resolving Dc* C++ function addresses...")
    vm_candidates, resolved_natives = find_dc_vm_and_natives(
        image, loads, exec_segments, dc_natives, string_xrefs, get_enclosing_func
    )
    resolved_count = sum(1 for v in resolved_natives.values() if v["func_offset"] is not None)
    print(f"      VM candidates: {len(vm_candidates)} | Resolved Dc* function addresses: {resolved_count}/{len(resolved_natives)}")

    # Map C++ source files (.cpp/.h) to the guest functions that reference them
    source_tree_map: Dict[str, Dict[str, Any]] = {}
    for src_path, str_offsets in sorted(source_files.items()):
        funcs_for_file: Set[int] = set()
        cpp_methods: List[str] = []
        assertions: List[str] = []
        for s_off in str_offsets:
            for insn_addr, fn_addr in string_xrefs.get(s_off, []):
                funcs_for_file.add(fn_addr)
                if fn_addr in func_cpp_names:
                    m_name = f"{func_cpp_names[fn_addr][1]} (L{func_cpp_names[fn_addr][2]})"
                    if m_name not in cpp_methods:
                        cpp_methods.append(m_name)
                for s_in_fn in func_to_strings.get(fn_addr, []):
                    if s_in_fn != src_path and len(assertions) < 15 and s_in_fn not in assertions:
                        assertions.append(s_in_fn)
        source_tree_map[src_path] = {
            "string_offsets": [f"0x{o:08X}" for o in str_offsets],
            "function_count": len(funcs_for_file),
            "functions": [f"0x{fn:08X}" for fn in sorted(funcs_for_file)],
            "cpp_methods": cpp_methods,
            "related_strings": assertions,
        }

    # Save reports
    output_dir.mkdir(parents=True, exist_ok=True)
    print(f"[5/5] Writing outputs to: {output_dir}")

    # 1. JSON Database
    json_path = output_dir / "u2_engine_map.json"
    report_data = {
        "generated": datetime.now().isoformat(),
        "executable": str(exe_path),
        "entry_point": f"0x{entry_vaddr:08X}",
        "image_size": len(image),
        "total_functions_detected": len(func_starts),
        "total_cpp_source_files": len(source_tree_map),
        "total_assertions": len(assertions_list),
        "vm_candidates": vm_candidates,
        "source_files": source_tree_map,
        "assertions": assertions_list,
        "dc_natives": resolved_natives,
    }
    json_path.write_text(json.dumps(report_data, indent=2, ensure_ascii=False), encoding="utf-8")

    # 2. Markdown Source Tree & Engine Overview
    md_path = output_dir / "U2_SOURCE_TREE.md"
    with open(md_path, "w", encoding="utf-8") as f:
        f.write("# Uncharted 2 (`big2-ps4_Shipping.elf`) — Architecture & Code Source C++\n\n")
        f.write(f"- **Exécutable analysé** : `{exe_path}`\n")
        f.write(f"- **Taille de l'image mémoire (Guest)** : `{len(image):,}` octets\n")
        f.write(f"- **Point d'entrée** : `0x{entry_vaddr:08X}`\n")
        f.write(f"- **Fonctions x86-64 détectées** : `{len(func_starts):,}`\n")
        f.write(f"- **Fichiers sources C++ originaux (`.cpp` / `.h`)** : `{len(source_tree_map):,}`\n")
        f.write(f"- **Assertions Naughty Dog (`int 0x41` avec nom de méthode C++ & ligne)** : `{len(assertions_list):,}`\n")
        f.write(f"- **Fonctions natives DC00 (`Dc*`)** : `{len(resolved_natives):,}` (`{resolved_count}` adresses résolues)\n\n")

        f.write("## 1. Fichiers Sources C++ Originaux de Naughty Dog (`.cpp` / `.h`)\n\n")
        f.write("| Fichier Source C++ | Nb Fonctions | Méthodes C++ identifiées (Ligne) | Adresses (`image + offset`) |\n")
        f.write("|--------------------|--------------|----------------------------------|-----------------------------|\n")
        for src_path, info in sorted(source_tree_map.items(), key=lambda x: (-x[1]["function_count"], x[0])):
            fn_preview = ", ".join(f"`{fn}`" for fn in info["functions"][:4])
            if len(info["functions"]) > 4:
                fn_preview += f" (+{len(info['functions']) - 4})"
            m_preview = ", ".join(f"`{m}`" for m in info["cpp_methods"][:4])
            if len(info["cpp_methods"]) > 4:
                m_preview += f" (+{len(info['cpp_methods']) - 4})"
            f.write(f"| `{src_path}` | {info['function_count']} | {m_preview or '-'} | {fn_preview or '-'} |\n")
        f.write("\n")

        f.write("## 2. Fonctions Natives C++ (`Dc*`) Résolues\n\n")
        f.write("| Nom C++ | Symbole Lisp | Hash `StringId` | Adresse Fonction (`image + offset`) | Prologue |\n")
        f.write("|---------|--------------|-----------------|-------------------------------------|----------|\n")
        for name, ninfo in sorted(resolved_natives.items()):
            if ninfo["func_offset"]:
                f.write(
                    f"| `{name}` | `{ninfo['lisp_symbol']}` | `{ninfo['symbol_hash']}` | "
                    f"`{ninfo['func_offset']}` | `{ninfo['prologue_hex'][:16]}` |\n"
                )

    # 3. C Header (u2_guest_hooks.h) for hooking inside src/
    hdr_path = output_dir / "u2_guest_hooks.h"
    with open(hdr_path, "w", encoding="utf-8") as f:
        f.write("/* Auto-generated by tools/u2_elf_extract.py from big2-ps4_Shipping.elf */\n")
        f.write("#ifndef U2_GUEST_HOOKS_H\n")
        f.write("#define U2_GUEST_HOOKS_H\n\n")
        f.write("#include <stdint.h>\n\n")
        f.write("typedef struct {\n")
        f.write("    const char *cpp_name;\n")
        f.write("    const char *lisp_name;\n")
        f.write("    uint32_t sym_hash;\n")
        f.write("    uint64_t guest_offset;\n")
        f.write("    uint8_t moved_bytes;\n")
        f.write("    uint8_t signature[16];\n")
        f.write("} U2NativeFunctionInfo;\n\n")
        f.write("static const U2NativeFunctionInfo u2_native_functions[] = {\n")
        for name, ninfo in sorted(resolved_natives.items()):
            if not ninfo["func_offset"]:
                continue
            fn_off = int(ninfo["func_offset"], 16)
            code_bytes = bytes(image[fn_off : fn_off + 16])
            moved = prologue_moved_bytes(code_bytes)
            sig_c = ", ".join(f"0x{b:02X}" for b in code_bytes)
            f.write(
                f'    {{ "{name}", "{ninfo["lisp_symbol"]}", {ninfo["symbol_hash"]}u, '
                f'0x{fn_off:08X}ull, {moved}, {{ {sig_c} }} }},\n'
            )
        f.write("};\n\n")
        f.write("#endif /* U2_GUEST_HOOKS_H */\n")

    # 4. Decompiled C++ Source File for the 833 Dc* Native Functions (u2_dc_natives.cpp)
    cpp_path = output_dir / "u2_dc_natives.cpp"
    with open(cpp_path, "w", encoding="utf-8") as f:
        f.write("// ============================================================================\n")
        f.write("// Uncharted 2: Among Thieves (big2-ps4_Shipping.elf)\n")
        f.write("// Decompiled C++ Native Script Functions (Dc*)\n")
        f.write("// Calling Convention: int64_t DcFunc(int32_t argc, uint64_t *argv, uint8_t *image)\n")
        f.write("// ============================================================================\n\n")
        f.write("#include <stdint.h>\n")
        f.write("#include <stddef.h>\n\n")
        for name, ninfo in sorted(resolved_natives.items()):
            if not ninfo["func_offset"]:
                continue
            fn_off = int(ninfo["func_offset"], 16)
            f.write(
                decompile_dc_native_x64(
                    name, ninfo["lisp_symbol"], ninfo["symbol_hash"], fn_off, image, func_cpp_names
                )
            )
            f.write("\n")

    print()
    print("=" * 72)
    print("  Extraction terminée avec succès !")
    print(f"  1. Base JSON complète   : {json_path}")
    print(f"  2. Arborescence C++ MD  : {md_path}")
    print(f"  3. Header C de Hooking  : {hdr_path}")
    print(f"  4. Fonctions C++ Dc*    : {cpp_path}")
    print("=" * 72)


def main():
    parser = argparse.ArgumentParser(
        description="Uncharted 2 (big2-ps4_Shipping.elf) C++ Engine & Symbol Extractor"
    )
    parser.add_argument(
        "game_dir",
        nargs="?",
        default=r"C:\Users\Yohan\Desktop\Uncharted\CUSA02343",
        help="Path to the CUSA02343 folder containing big2-ps4_Shipping.elf",
    )
    parser.add_argument(
        "--out",
        "-o",
        default=None,
        help="Output directory (default: u2_engine_map beside game_dir)",
    )
    args = parser.parse_args()

    game_dir = Path(args.game_dir).resolve()
    output_dir = Path(args.out).resolve() if args.out else (game_dir.parent / "u2_engine_map")
    extract_u2_engine(game_dir, output_dir)


if __name__ == "__main__":
    main()

