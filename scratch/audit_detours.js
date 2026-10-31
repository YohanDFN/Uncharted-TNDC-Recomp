const fs = require('fs');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const imageFileOffset = 0x183ce8;

// Read runtime_u2_script.c to extract all { "Name", 0x..., (uintptr_t)... }
const scriptC = fs.readFileSync('src/runtime_u2_script.c', 'utf8');
const regex = /\{\s*"([^"]+)",\s*(0x[0-9a-fA-F]+),\s*\(uintptr_t\)/g;

let match;
const badDetours = [];
const allDetours = [];

while ((match = regex.exec(scriptC)) !== null) {
    const name = match[1];
    const offset = parseInt(match[2], 16);
    allDetours.push({ name, offset });
    
    // Read 8 bytes at offset
    const b = Buffer.alloc(8);
    fs.readSync(fd, b, 0, 8, imageFileOffset + offset);
    const hex = b.toString('hex');
    
    // Check standard function prologues
    // 55 48 89 e5 -> push rbp; mov rbp, rsp
    // 41 57 / 41 56 / 41 55 / 41 54 -> push r15..r12
    // 53 -> push rbx
    // 48 83 ec -> sub rsp, xx
    // 31 c0 c3 -> xor eax, eax; ret
    // c3 -> ret
    // b8 ... c3 -> mov eax, xx; ret
    const isProlog = (
        hex.startsWith('554889e5') ||
        hex.startsWith('55488bec') ||
        hex.startsWith('4157') ||
        hex.startsWith('4156') ||
        hex.startsWith('4155') ||
        hex.startsWith('4154') ||
        hex.startsWith('53') ||
        hex.startsWith('4883ec') ||
        hex.startsWith('4881ec') ||
        hex.startsWith('31c0c3') ||
        hex.startsWith('c3') ||
        hex.startsWith('b8')
    );
    
    if (!isProlog) {
        badDetours.push({ name, offset: '0x' + offset.toString(16), hex });
    }
}

console.log(`Audited ${allDetours.length} detours.`);
console.log(`Found ${badDetours.length} suspicious detours that do not start with a standard function prologue:`);
for (const b of badDetours) {
    console.log(`  ${b.name} @ ${b.offset} -> bytes: ${b.hex}`);
}

fs.closeSync(fd);

