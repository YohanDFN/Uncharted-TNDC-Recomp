import json

with open('tools/u2_map/u2_engine_map.json', 'r', encoding='utf-8') as f:
    d = json.load(f)

funcs = [int(f, 16) for f in d.get('source_files', {}).get('d:/dev/CP9/shared-ps4/src/corelib/memory/memory.cpp', {}).get('functions', [])]

# group by 64KB blocks
blocks = {}
for f in sorted(funcs):
    blk = f & 0xFFF00000
    blocks.setdefault(blk, []).append(f)

for blk, flist in sorted(blocks.items()):
    print(f"Block 0x{blk:08X}..0x{blk+0x100000:08X}: {len(flist)} functions (from 0x{min(flist):08X} to 0x{max(flist):08X})")

