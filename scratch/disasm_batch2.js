const fs = require('fs');
const { execSync } = require('child_process');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const imageFileOffset = 0x183ce8;

function makeElf64(codeBytes, vma) {
    const ehdrSize = 64, phdrSize = 56;
    const headerSize = ehdrSize + phdrSize;
    const totalSize = headerSize + codeBytes.length;
    const elf = Buffer.alloc(totalSize);
    elf.set([0x7f, 0x45, 0x4c, 0x46, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0], 0);
    elf.writeUInt16LE(2, 16); elf.writeUInt16LE(62, 18); elf.writeUInt32LE(1, 20);
    elf.writeBigUInt64LE(BigInt(vma), 24);
    elf.writeBigUInt64LE(BigInt(ehdrSize), 32);
    elf.writeUInt16LE(ehdrSize, 52); elf.writeUInt16LE(phdrSize, 54); elf.writeUInt16LE(1, 56);
    elf.writeUInt32LE(1, ehdrSize + 0); elf.writeUInt32LE(7, ehdrSize + 4);
    elf.writeBigUInt64LE(0n, ehdrSize + 8);
    elf.writeBigUInt64LE(BigInt(vma - headerSize), ehdrSize + 16);
    elf.writeBigUInt64LE(BigInt(vma - headerSize), ehdrSize + 24);
    elf.writeBigUInt64LE(BigInt(totalSize), ehdrSize + 32);
    elf.writeBigUInt64LE(BigInt(totalSize), ehdrSize + 40);
    elf.writeBigUInt64LE(0x1000n, ehdrSize + 48);
    codeBytes.copy(elf, headerSize);
    return elf;
}

function disasmFn(offsetHex, size, name) {
    const offset = parseInt(offsetHex, 16);
    const code = Buffer.alloc(size);
    fs.readSync(fd, code, 0, size, imageFileOffset + offset);
    const elf = makeElf64(code, offset);
    fs.writeFileSync('scratch/temp.elf', elf);
    const out = execSync(`llvm-objdump -d --no-show-raw-insn scratch/temp.elf`).toString();
    console.log(`\n=== [${name}] @ ${offsetHex} (${size} bytes) ===`);
    const lines = out.split('\n').filter(l => {
        const m = l.match(/^\s*([0-9a-f]+):/);
        return m && parseInt(m[1], 16) >= offset;
    });
    console.log(lines.join('\n'));
}

disasmFn('0x004C2BF0', 0x20, 'DcEventArgFloat');
disasmFn('0x004C2BE0', 0x20, 'DcEventArgInt32');
disasmFn('0x004C2550', 0x20, 'DcEventReturnFloat');
disasmFn('0x004C2390', 0x20, 'DcEventReturnInt32');
disasmFn('0x004C21D0', 0x20, 'DcEventReturnUint32');
disasmFn('0x003976C0', 0x30, 'DcGetAnimationPhase');
disasmFn('0x00397880', 0x30, 'DcSetAnimationSpeed');
disasmFn('0x00397A20', 0x30, 'DcHasAnimP');
disasmFn('0x00397AB0', 0x30, 'DcStopAnimating');
disasmFn('0x00A12780', 0x30, 'DcObjectSpawnedP');
disasmFn('0x00A0E500', 0x30, 'DcCameraDisable');

fs.closeSync(fd);

