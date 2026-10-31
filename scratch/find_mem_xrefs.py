import sys, struct
from pathlib import Path
sys.path.insert(0, str(Path("tools").resolve()))
from u2_elf_extract import load_ps4_executable

game_elf = Path(r"C:\Users\Yohan\Desktop\Uncharted\CUSA02343\big2-ps4_Shipping.elf")
image, entry, loads = load_ps4_executable(game_elf)

targets = [
    b"Memory::GetAllocator\x00",
    b"Memory::TopAllocator\x00",
    b"PushAllocator\x00",
    b"PopAllocator\x00",
    b"AllocateAllocStack\x00",
    b"FreeAllocStack\x00",
    b"InitializeThreadAllocStack\x00",
    b"FreeThreadAllocStack\x00",
    b"ScopedTempAllocator\x00",
]

for t in targets:
    t_str = t[:-1].decode('ascii')
    idx = 0
    str_addrs = []
    while True:
        pos = image.find(t, idx)
        if pos == -1:
            break
        str_addrs.append(pos)
        idx = pos + len(t)
    
    print(f"=== String '{t_str}' found at: {[hex(a) for a in str_addrs]} ===")
    # Search RIP-relative references in code segments
    for s_addr in str_addrs:
        xrefs = []
        # scan for lea/mov disp32
        for seg in loads:
            if not (seg['flags'] & 1): # exec only
                continue
            seg_start = seg['vaddr']
            seg_end = seg_start + seg['filesz']
            for p in range(seg_start, seg_end - 7):
                # opcode 48 8d ... (lea)
                if image[p] in (0x48, 0x4c) and image[p+1] == 0x8d:
                    disp = struct.unpack_from('<i', image, p+3)[0]
                    target = p + 7 + disp
                    if target == s_addr:
                        xrefs.append(p)
        print(f"  Xrefs from code: {[hex(x) for x in xrefs[:10]]}")

