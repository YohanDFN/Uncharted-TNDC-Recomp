import sys
from pathlib import Path
sys.path.insert(0, str(Path("tools").resolve()))
from u2_elf_extract import load_ps4_executable

game_elf = Path(r"C:\Users\Yohan\Desktop\Uncharted\CUSA02343\big2-ps4_Shipping.elf")
image, entry, loads = load_ps4_executable(game_elf)

import re

# find strings containing AllocStack or Allocator
strings = re.findall(rb'([A-Za-z0-9_:]*(?:AllocStack|Allocator)[A-Za-z0-9_:]*)', image)
unique_s = sorted(set(s.decode('ascii', errors='ignore') for s in strings if len(s) > 5))
print(f"Total matching strings: {len(unique_s)}")
for s in unique_s[:40]:
    print(f"  {s}")

