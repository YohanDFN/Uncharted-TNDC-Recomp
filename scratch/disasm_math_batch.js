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

disasmFn('0x00397630', 0x50, 'DcAngleMod');
disasmFn('0x009FDC20', 0x80, 'DcMakeVector3');
disasmFn('0x009FDCF0', 0x80, 'DcMakeVector');
disasmFn('0x009FDB90', 0x60, 'DcLerpScale');
disasmFn('0x009F37C0', 0x70, 'DcGetQuaternionPitch');

fs.closeSync(fd);

