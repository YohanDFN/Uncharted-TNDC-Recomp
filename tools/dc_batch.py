#!/usr/bin/env python3
"""
Naughty Dog DC Bytecode Batch Disassembler
==========================================
Scans all .bin DC script files in dc1/ps4 and dc2/ directories,
disassembles each one and writes human-readable output to an output folder.

Usage:
    py tools/dc_batch.py [GAME_DIR] [--out OUTPUT_DIR] [--lisp] [--filter PATTERN]

Example:
    py tools/dc_batch.py "C:\\Users\\Yohan\\Desktop\\Uncharted\\CUSA02343" --lisp
    py tools/dc_batch.py "C:\\Users\\Yohan\\Desktop\\Uncharted\\CUSA02343" --filter "ss-train"
"""

import sys
import os
import struct
import argparse
import json
from pathlib import Path
from typing import List, Dict, Tuple, Optional
from datetime import datetime

# ── Import dc_disasm from the same tools/ directory ────────────────────────────
TOOLS_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(TOOLS_DIR))

from dc_disasm import (
    Instruction, OpFormat, OPCODES, GLOBAL_HASH_TO_NAME, prescan_binary_symbols,
    disassemble_stream, disassemble_file, print_disassembly,
    scan_for_bytecode, DcContainer, format_function_disassembly, format_data_def
)


# ── Constant Pool Parser ────────────────────────────────────────────────────────

def read_const_pool(data: bytes) -> List[Tuple[int, str]]:
    """
    Read strings and symbols from file, stripping 0x637BDB3E ('{c') tags.
    """
    strings = []
    i = 0
    while i < len(data) - 3:
        if 0x20 <= data[i] <= 0x7E:
            j = i
            while j < len(data) and 0x20 <= data[j] <= 0x7E:
                j += 1
            start = i
            if j - start >= 2 and data[start:start+2] == b"{c" and start >= 2 and data[start-2:start] == b"\x3e\xdb":
                start += 2
            if j - start >= 4:
                text = data[start:j].decode("ascii", errors="replace")
                strings.append((start, text))
            i = j
        i += 1
    return strings


# ── Output Formatting ───────────────────────────────────────────────────────────

def format_header(filename: str, size: int, num_funcs: int, num_instr: int, strings: List[Tuple[int, str]], num_defs: int = 0) -> str:
    lines = []
    lines.append("=" * 90)
    lines.append(f"  FILE : {filename}")
    lines.append(f"  SIZE : {size:,} bytes  |  FUNCTIONS : {num_funcs}  |  DATA DEFS : {num_defs}  |  INSTRUCTIONS : {num_instr}")
    lines.append(f"  DATE : {datetime.now().strftime('%Y-%m-%d %H:%M')}")
    lines.append("=" * 90)
    if strings:
        lines.append("")
        lines.append(";; ── STRINGS / SYMBOLS FOUND IN CONSTANT POOL ──────────────────────────────────")
        for offset, text in strings[:60]:
            lines.append(f";;   [0x{offset:06X}]  {text!r}")
        if len(strings) > 60:
            lines.append(f";;   ... and {len(strings) - 60} more strings")
    lines.append("")
    return "\n".join(lines)


def disassemble_to_string(data: bytes, filepath: str, show_lisp: bool = True, container: Optional[DcContainer] = None) -> str:
    """Disassemble a DC00 binary and return it as a formatted string."""
    if container is None:
        container = DcContainer(data)
    out_lines = []
    strings = read_const_pool(data)

    if container.is_dc and (container.functions or container.data_defs):
        total_instr = sum(fn.code_size // 4 for fn in container.functions)
        out_lines.append(format_header(
            Path(filepath).name, len(data),
            len(container.functions), total_instr, strings,
            num_defs=len(container.data_defs)
        ))
        if container.data_defs:
            out_lines.append(";; ── EXPORTED DATA / SCENARIO DEFINITIONS ──────────────────────────────────────")
            out_lines.append("")
            for ddef in container.data_defs:
                out_lines.append(format_data_def(ddef))
        for fn in container.functions:
            out_lines.append(format_function_disassembly(container, fn, show_lisp=show_lisp))
        return "\n".join(out_lines)

    # Fallback: stream disassembly
    code_off = 0x5C if container.is_dc else 0
    code_data = data[code_off:]
    num_instr = len(code_data) // 4
    out_lines.append(format_header(Path(filepath).name, len(data), 0, num_instr, strings))

    instructions = disassemble_stream(code_data, base_pc=0)
    labels = {ins.target_pc for ins in instructions if ins.target_pc is not None}

    out_lines.append(f"{'PC':<8} {'HEX':<12} {'ASSEMBLY':<36}  {'S-EXPRESSION (Lisp)' if show_lisp else 'DESCRIPTION'}")
    out_lines.append("-" * 90)

    for ins in instructions:
        if ins.pc in labels:
            out_lines.append(f"\nloc_{ins.pc:04X}:")

        hex_str = " ".join(f"{b:02X}" for b in ins.raw)
        asm_str = ins.format_asm()
        semantics = ins.to_lisp() if show_lisp else f";; {ins.desc}"
        out_lines.append(f"[{ins.pc:04X}]  {hex_str:<10}  {asm_str:<36}  {semantics}")

    return "\n".join(out_lines)


# ── Batch Engine ────────────────────────────────────────────────────────────────

def find_dc_files(game_dir: Path, target_game: str = "u2") -> List[Path]:
    """Find .bin DC script files for the selected game (default: u2 -> u2data)."""
    game_map = {
        "u1": ("u1data",),
        "1":  ("u1data",),
        "u2": ("u2data",),
        "2":  ("u2data",),
        "u3": ("u3data",),
        "3":  ("u3data",),
        "all": ("u1data", "u2data", "u3data"),
    }
    data_dirs = game_map.get(target_game.lower(), ("u2data",))
    found = []
    for data_dir_name in data_dirs:
        base = game_dir / data_dir_name / "build" / "main"
        if not base.exists():
            continue
        for dc_dir in base.glob("dc*"):
            ps4_dir = dc_dir / "ps4"
            if ps4_dir.exists():
                for f in sorted(ps4_dir.glob("*.bin")):
                    found.append(f)
            # Also pick up .bin directly in dc2/, etc.
            for f in sorted(dc_dir.glob("*.bin")):
                if "ps4" not in f.parts:
                    found.append(f)
    return found


def batch_disassemble(
    game_dir: Path,
    output_dir: Path,
    show_lisp: bool = True,
    name_filter: Optional[str] = None,
    target_game: str = "u2"
):
    files = find_dc_files(game_dir, target_game=target_game)
    if not files:
        print(f"[ERROR] No .bin DC scripts found in {game_dir} (game={target_game})")
        sys.exit(1)

    if name_filter:
        files = [f for f in files if name_filter.lower() in str(f).lower()]

    output_dir.mkdir(parents=True, exist_ok=True)

    index_entries = []
    total_instructions = 0
    total_size = 0

    print(f"Pre-scanning {len(files)} script files to build global symbol hash dictionary...")
    for filepath in files:
        try:
            raw = filepath.read_bytes()
            if len(raw) >= 64 and raw[:4] in (b"00CD", b"DC00"):
                prescan_binary_symbols(raw)
        except Exception:
            pass
    print(f"Global dictionary loaded: {len(GLOBAL_HASH_TO_NAME):,} symbol hashes.")
    print(f"Disassembling {len(files)} script files to: {output_dir}")
    print()

    for i, filepath in enumerate(files, 1):
        rel = filepath.relative_to(game_dir)
        size = filepath.stat().st_size

        # Preserve subdirectory structure in output
        out_rel = Path(str(rel).replace("\\", "_").replace("/", "_")).with_suffix(".asm")
        out_path = output_dir / out_rel

        try:
            raw = filepath.read_bytes()
            container = DcContainer(raw)

            # Skip obvious non-DC tiny stubs (< 64 bytes)
            if size < 64:
                content = f";; [STUB] {filepath.name} - {size} bytes (too small for bytecode)\n"
                num_instr = 0
            elif not container.is_dc:
                content = f";; [SKIP] {filepath.name} - not a DC00 container (magic={raw[:4]})\n"
                num_instr = 0
            else:
                if container.functions or container.data_defs:
                    num_instr = sum(fn.code_size // 4 for fn in container.functions)
                else:
                    num_instr = (len(raw) - 0x5C) // 4
                content = disassemble_to_string(raw, str(filepath), show_lisp=show_lisp, container=container)

            out_path.write_text(content, encoding="utf-8")
            total_instructions += num_instr
            total_size += size

            status = "OK" if size >= 64 else "  "
            print(f"  [{i:3d}/{len(files)}] {status}  {filepath.name:<46}  {size:>8,} bytes  ->  {out_path.name}")

            index_entries.append({
                "file": filepath.name,
                "path": str(rel),
                "size": size,
                "instructions": num_instr,
                "output": str(out_path.name),
            })

        except Exception as e:
            print(f"  [{i:3d}/{len(files)}] !!  {filepath.name:<46}  ERROR: {e}")
            index_entries.append({
                "file": filepath.name,
                "path": str(rel),
                "size": size,
                "error": str(e),
            })

    # Write JSON index
    index_path = output_dir / "_index.json"
    with open(index_path, "w", encoding="utf-8") as f:
        json.dump({
            "generated": datetime.now().isoformat(),
            "game_dir": str(game_dir),
            "total_files": len(files),
            "total_instructions": total_instructions,
            "total_bytes": total_size,
            "files": index_entries,
        }, f, indent=2, ensure_ascii=False)

    # Write Markdown summary
    summary_path = output_dir / "_SUMMARY.md"
    with open(summary_path, "w", encoding="utf-8") as f:
        f.write(f"# Naughty Dog DC Script Disassembly\n\n")
        f.write(f"**Game:** `{game_dir}`  \n")
        f.write(f"**Generated:** {datetime.now().strftime('%Y-%m-%d %H:%M')}  \n")
        f.write(f"**Total Files:** {len(files)}  \n")
        f.write(f"**Total Instructions:** {total_instructions:,}  \n")
        f.write(f"**Total Size:** {total_size:,} bytes  \n\n")
        f.write("## File List (sorted by size)\n\n")
        f.write("| File | Size (bytes) | Instructions | Output |\n")
        f.write("|------|-------------|-------------|--------|\n")
        for e in sorted(index_entries, key=lambda x: x.get("size", 0), reverse=True):
            err = f"⚠ {e['error']}" if "error" in e else f"`{e['output']}`"
            f.write(f"| `{e['file']}` | {e.get('size',0):,} | {e.get('instructions', 0):,} | {err} |\n")

    print()
    print("=" * 60)
    print(f"  Done! {len(files)} files disassembled.")
    print(f"  Total: {total_instructions:,} instructions across {total_size/1024/1024:.1f} MB of scripts")
    print(f"  Output:  {output_dir}")
    print(f"  Index:   {index_path.name}")
    print(f"  Summary: {summary_path.name}")
    print("=" * 60)


# ── Main ────────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description="Naughty Dog DC Script Batch Disassembler — Uncharted PS4",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  py tools/dc_batch.py "C:\\Uncharted\\CUSA02343"
  py tools/dc_batch.py "C:\\Uncharted\\CUSA02343" --out dc_output --lisp
  py tools/dc_batch.py "C:\\Uncharted\\CUSA02343" --filter "ss-train" --lisp
        """
    )
    parser.add_argument(
        "game_dir",
        nargs="?",
        default=r"C:\Users\Yohan\Desktop\Uncharted\CUSA02343",
        help="Path to the game folder (containing u2data, eboot.bin...)"
    )
    parser.add_argument(
        "--out", "-o",
        default=None,
        help="Output directory (default: dc_disassembly/ beside the game dir)"
    )
    parser.add_argument(
        "--lisp", "-l",
        action="store_true",
        default=True,
        help="Emit pseudo-Lisp S-expressions (default: on)"
    )
    parser.add_argument(
        "--no-lisp",
        action="store_false",
        dest="lisp",
        help="Emit description comments instead of Lisp"
    )
    parser.add_argument(
        "--filter", "-f",
        default=None,
        help="Only disassemble files whose path/name contains this string"
    )
    parser.add_argument(
        "--game", "-g",
        default="u2",
        choices=["u1", "u2", "u3", "all", "1", "2", "3"],
        help="Target game to disassemble (default: u2 for Uncharted 2)"
    )
    args = parser.parse_args()

    game_dir = Path(args.game_dir).resolve()
    if not game_dir.exists():
        print(f"[ERROR] Game directory not found: {game_dir}")
        sys.exit(1)

    if args.out:
        output_dir = Path(args.out).resolve()
    else:
        output_dir = game_dir.parent / "dc_disassembly"

    batch_disassemble(
        game_dir=game_dir,
        output_dir=output_dir,
        show_lisp=args.lisp,
        name_filter=args.filter,
        target_game=args.game,
    )


if __name__ == "__main__":
    main()
