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

disasmFn('0x0090B060', 0x30, 'DcSetCoverPushBackDist');
disasmFn('0x0090E660', 0x30, 'DcSetMinFallDist');
disasmFn('0x0090DE90', 0x30, 'DcPlayerSetCornerClimbDistScale');
disasmFn('0x0090D6B0', 0x20, 'DcSwimAllowStandOnBottom');
disasmFn('0x0090D680', 0x20, 'DcSwimDisallowStandOnBottom');
disasmFn('0x00A01B60', 0x20, 'DcSetJetskiMaxThrottle');
disasmFn('0x00906740', 0x20, 'DcForceDistanceStopIncrease');
disasmFn('0x00A0EF50', 0x40, 'DcGetLocatorDistance');

fs.closeSync(fd);

