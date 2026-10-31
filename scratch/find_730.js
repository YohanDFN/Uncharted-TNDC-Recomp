const fs = require('fs');
const lines = fs.readFileSync('scratch/disasm_memory_full.txt', 'utf8').split('\n');

for (let i = 0; i < lines.length; i++) {
    if (lines[i].includes('0x730')) {
        console.log(`Line ${i}:`);
        for (let j = Math.max(0, i - 10); j <= Math.min(lines.length - 1, i + 15); j++) {
            console.log(lines[j]);
        }
        console.log('----------------------------------------------------');
    }
}

