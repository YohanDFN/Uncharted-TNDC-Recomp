const fs = require('fs');
const { execSync } = require('child_process');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const imageFileOffset = 0x183ce8;

function makeElf64(codeBytes, vma) {
    // 64-bit ELF header + 1 program header (LOAD)
    const ehdrSize = 64;
    const phdrSize = 56;
    const headerSize = ehdrSize + phdrSize;
    const totalSize = headerSize + codeBytes.length;

    const elf = Buffer.alloc(totalSize);
    // e_ident
    elf.set([0x7f, 0x45, 0x4c, 0x46, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0], 0);
    elf.writeUInt16LE(2, 16); // ET_EXEC
    elf.writeUInt16LE(62, 18); // EM_X86_64
    elf.writeUInt32LE(1, 20); // EV_CURRENT
    elf.writeBigUInt64LE(BigInt(vma), 24); // e_entry
    elf.writeBigUInt64LE(BigInt(ehdrSize), 32); // e_phoff
    elf.writeBigUInt64LE(0n, 40); // e_shoff
    elf.writeUInt32LE(0, 48); // e_flags
    elf.writeUInt16LE(ehdrSize, 52); // e_ehsize
    elf.writeUInt16LE(phdrSize, 54); // e_phentsize
    elf.writeUInt16LE(1, 56); // e_phnum
    elf.writeUInt16LE(0, 58); // e_shentsize
    elf.writeUInt16LE(0, 60); // e_shnum
    elf.writeUInt16LE(0, 62); // e_shstrndx

    // phdr 0 (LOAD)
    elf.writeUInt32LE(1, ehdrSize + 0); // PT_LOAD
    elf.writeUInt32LE(7, ehdrSize + 4); // PF_R | PF_W | PF_X
    elf.writeBigUInt64LE(0n, ehdrSize + 8); // p_offset
    elf.writeBigUInt64LE(BigInt(vma - headerSize), ehdrSize + 16); // p_vaddr
    elf.writeBigUInt64LE(BigInt(vma - headerSize), ehdrSize + 24); // p_paddr
    elf.writeBigUInt64LE(BigInt(totalSize), ehdrSize + 32); // p_filesz
    elf.writeBigUInt64LE(BigInt(totalSize), ehdrSize + 40); // p_memsz
    elf.writeBigUInt64LE(0x1000n, ehdrSize + 48); // p_align

    codeBytes.copy(elf, headerSize);
    return elf;
}

function disasm(guestOffset, size, label) {
    const code = Buffer.alloc(size);
    fs.readSync(fd, code, 0, size, imageFileOffset + guestOffset);
    const elf = makeElf64(code, guestOffset);
    fs.writeFileSync('scratch/temp.elf', elf);
    console.log(`\n============================================================`);
    console.log(`=== ${label} @ 0x${guestOffset.toString(16).toUpperCase()} (${size} bytes) ===`);
    console.log(`============================================================`);
    try {
        const out = execSync(`llvm-objdump -d --no-show-raw-insn scratch/temp.elf`).toString();
        const lines = out.split('\n').filter(l => l.trim().length > 0 && !l.includes('file format') && !l.includes('Disassembly of'));
        console.log(lines.join('\n'));
    } catch (err) {
        console.error(err.message);
    }
}

// Inspect the memory functions in 0x00B35B00..0x00B36300
disasm(0x00B35BA0, 0x40, 'AllocateAllocStack');
disasm(0x00B35C80, 0x60, 'InitializeThreadAllocStack');
disasm(0x00B35D80, 0x38, 'FreeThreadAllocStack');
disasm(0x00B35EA0, 0x80, 'PushAllocator');
disasm(0x00B361C0, 0x60, 'PopAllocator');
disasm(0x00B362A0, 0x40, 'GetAllocatorStackDepth');

fs.closeSync(fd);

