import sys, subprocess
from pathlib import Path
sys.path.insert(0, str(Path("tools").resolve()))
from u2_elf_extract import load_ps4_executable
import json

with open('tools/u2_map/u2_engine_map.json', 'r', encoding='utf-8') as f:
    d = json.load(f)

# Find all functions in 0x00CD7000 to 0x00CD9000
funcs = [int(f, 16) for f in d.get('source_files', {}).get('d:/dev/CP9/shared-ps4/src/corelib/memory/memory.cpp', {}).get('functions', [])]
print(f"Total functions in memory.cpp: {len(funcs)}")
for f in sorted(funcs):
    if 0x00CD6000 <= f <= 0x00CDA000:
        print(f"  0x{f:08X}")

