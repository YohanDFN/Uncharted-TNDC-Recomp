const fs = require('fs');
const lines = fs.readFileSync('scratch/disasm_memory_full.txt', 'utf8').split('\n');

for (const l of lines) {
    const m = l.match(/^\s*([0-9a-f]+):/);
    if (m) {
        const addr = parseInt(m[1], 16);
        if (addr >= 0xb35d00 && addr <= 0xb35d80) {
            console.log(l);
        }
    }
}

