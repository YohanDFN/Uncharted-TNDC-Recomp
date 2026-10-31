const fs = require('fs');
const elf = fs.readFileSync('out/big2/eboot.elf');
function fileToVaddr(f) { return f >= 0x4000 && f < 0xfe84b0 ? f - 0x4000 : null; }
function vaddrToFile(v) { return v >= 0 && v < 0xfe44b0 ? v + 0x4000 : null; }
function getCString(va) {
    const fo = vaddrToFile(va);
    if (!fo) return null;
    let e = fo;
    while (e < elf.length && elf[e] >= 32 && elf[e] <= 126) e++;
    return elf.slice(fo, e).toString('ascii');
}

// Search around 0x9fde60 in registration
for (let i = 0x4000; i < 0xfe84b0 - 30; i++) {
    if (elf[i] === 0x48 && elf[i+1] === 0x8d && elf[i+2] === 0x05) {
        const fnRip = fileToVaddr(i + 7);
        const fnDisp = elf.readInt32LE(i + 3);
        const fnVa = fnRip + fnDisp;
        if (fnVa >= 0x9fde00 && fnVa <= 0x9fdf50) {
            // Find string
            for (let j = i; j < i + 100; j++) {
                if (elf[j] === 0x48 && elf[j+1] === 0x8d && elf[j+2] === 0x35) {
                    const strRip = fileToVaddr(j + 7);
                    const strDisp = elf.readInt32LE(j + 3);
                    const str = getCString(strRip + strDisp);
                    console.log(`0x${fnVa.toString(16)}: ${str}`);
                    break;
                }
            }
        }
    }
}
