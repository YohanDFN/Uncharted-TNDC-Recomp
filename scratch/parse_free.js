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

// FreeThreadAllocStack & FreeAllocStack around 0xb35d80
printRange('b35d70', 'b35df0');

// 0xb36d40
printRange('b36d30', 'b36dc0');

