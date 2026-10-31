const fs = require('fs');
const { execSync } = require('child_process');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');

function disasm(offset, size, label) {
    const buf = Buffer.alloc(size);
    fs.readSync(fd, buf, 0, size, offset);
    fs.writeFileSync('scratch/temp.bin', buf);
    const startAddr = '0x' + offset.toString(16);
    console.log(`\n=== [${label}] Offset: ${startAddr} (${size} bytes) ===`);
    try {
        const out = execSync(`llvm-objdump -D -b binary -m i386:x86-64 --adjust-vma=${startAddr} scratch/temp.bin`).toString();
        // Skip header lines
        const lines = out.split('\n').filter(l => !l.includes('file format') && !l.includes('Disassembly of section'));
        console.log(lines.slice(0, 50).join('\n'));
    } catch (err) {
        console.error(err.message);
    }
}

// Disassemble key memory functions
disasm(0x00B35BA0, 0x50, 'AllocateAllocStack (0x00B35BA0)');
disasm(0x00B35C80, 0x80, 'InitializeThreadAllocStack (0x00B35C80)');
disasm(0x00B35D80, 0x40, 'FreeThreadAllocStack (0x00B35D80)');
disasm(0x00B35EA0, 0x80, 'PushAllocator 1 (0x00B35EA0)');
disasm(0x00B360D0, 0x80, 'PushAllocator 2 (0x00B360D0)');
disasm(0x00B361C0, 0x60, 'PopAllocator (0x00B361C0)');
disasm(0x00B362A0, 0x50, 'GetAllocatorStackDepth (0x00B362A0)');

fs.closeSync(fd);

