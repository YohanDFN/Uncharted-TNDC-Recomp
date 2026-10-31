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

const code = Buffer.alloc(0x100);
fs.readSync(fd, code, 0, 0x100, imageFileOffset + 0x00B33690);
const elf = makeElf64(code, 0x00B33690);
fs.writeFileSync('scratch/temp.elf', elf);
const out = execSync(`llvm-objdump -d --no-show-raw-insn scratch/temp.elf`).toString();
console.log(out);

fs.closeSync(fd);

