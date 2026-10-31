const fs = require('fs');

const lines = fs.readFileSync('scratch/disasm_memory_full.txt', 'utf8').split('\n');

function printRange(startHex, endHex) {
    const start = parseInt(startHex, 16);
    const end = parseInt(endHex, 16);
    console.log(`\n=================== RANGE 0x${startHex} - 0x${endHex} ===================`);
    for (const l of lines) {
        const m = l.match(/^\s*([0-9a-f]+):/);
        if (m) {
            const addr = parseInt(m[1], 16);
            if (addr >= start && addr <= end) {
                console.log(l);
            }
        }
    }
}

// PopAllocator (0x00B361C0)
printRange('b361c0', 'b36290');

// GetAllocatorStackDepth (0x00B362A0)
printRange('b362a0', 'b36320');

