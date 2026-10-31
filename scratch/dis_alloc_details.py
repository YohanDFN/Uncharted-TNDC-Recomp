import sys, subprocess
from pathlib import Path
sys.path.insert(0, str(Path("tools").resolve()))
from u2_elf_extract import load_ps4_executable

game_elf = Path(r"C:\Users\Yohan\Desktop\Uncharted\CUSA02343\big2-ps4_Shipping.elf")
image, entry, loads = load_ps4_executable(game_elf)

def disasm_bytes(b):
    p = subprocess.Popen(
        ["C:/msys64/clang64/bin/llvm-mc.exe", "-disassemble", "-triple=x86_64"],
        stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    hex_str = " ".join(f"0x{x:02x}" for x in b)
    out, err = p.communicate(hex_str.encode("ascii"))
    return [l.strip() for l in out.decode("utf-8", errors="ignore").splitlines() if l.strip()]

print("=== 0x00B35B80 .. 0x00B35D00 (AllocStack creation) ===")
for l in disasm_bytes(image[0x00B35B80:0x00B35B80+200]):
    print(" ", l)

