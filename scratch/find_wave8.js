const fs = require('fs');

// Re-read src/runtime_u2_script.c to see what's already detoured
const cFile = fs.readFileSync('src/runtime_u2_script.c', 'utf8');
const registered = new Set();
const re = /\{\s*"([^"]+)",\s*0x[0-9a-fA-F]+/g;
let m;
while ((m = re.exec(cFile)) !== null) {
    registered.add(m[1]);
}
console.log(`Currently registered in detours: ${registered.size}`);

// Find all DC registrations in eboot.elf
const elf = fs.readFileSync('out/big2/eboot.elf');
function fileToVaddr(f) {
    if (f >= 0x4000 && f < 0xfe84b0) return f - 0x4000;
    return null;
}
function vaddrToFile(v) {
    if (v >= 0 && v < 0xfe44b0) return v + 0x4000;
    return null;
}
function getCString(vaddr) {
    const fileOff = vaddrToFile(vaddr);
    if (!fileOff || fileOff >= elf.length) return null;
    let end = fileOff;
    while (end < elf.length && elf[end] !== 0) {
        if (elf[end] < 32 || elf[end] > 126) return null;
        end++;
    }
    return elf.slice(fileOff, end).toString('ascii');
}

const map = new Map();
for (let i = 0x4000; i < 0xfe84b0 - 30; i++) {
    if (elf[i] === 0x48 && elf[i+1] === 0x8d && elf[i+2] === 0x35) {
        const strRip = fileToVaddr(i + 7);
        const strDisp = elf.readInt32LE(i + 3);
        const strVa = strRip + strDisp;
        const str = getCString(strVa);
        if (str && str.startsWith('Dc')) {
            let fnVa = null;
            for (let j = i - 1; j >= Math.max(0x4000, i - 120); j--) {
                if (elf[j] === 0x48 && elf[j+1] === 0x8d && elf[j+2] === 0x05) {
                    const fnRip = fileToVaddr(j + 7);
                    const fnDisp = elf.readInt32LE(j + 3);
                    fnVa = fnRip + fnDisp;
                    break;
                }
            }
            if (fnVa !== null && !map.has(str)) {
                map.set(str, { name: str, fnVa });
            }
        }
    }
}

const unhooked = Array.from(map.values()).filter(x => !registered.has(x.name));
console.log(`Unhooked functions remaining: ${unhooked.length}`);

// Let's inspect some interesting categories:
// 1. Math / Clock / Time / Random / Logic
// 2. Camera
// 3. Spawner / NPC Alive / Combat
// 4. Player actions (jump, climb, aiming, melee)
const math_time = unhooked.filter(x => /Time|Clock|Date|Math|Sin|Cos|Sqrt|Vector|Matrix|Clamp|Min|Max|Abs|Dist/i.test(x.name));
console.log('\n--- Math / Time / Vector candidates ---');
for (const x of math_time.slice(0, 20)) {
    console.log(`${x.name.padEnd(35)} 0x${x.fnVa.toString(16)}`);
}

const camera = unhooked.filter(x => /^DcCamera/.test(x.name));
console.log('\n--- Camera candidates ---');
for (const x of camera) {
    console.log(`${x.name.padEnd(35)} 0x${x.fnVa.toString(16)}`);
}

const combat_npc = unhooked.filter(x => /^DcNpc.*(Alive|Die|Count|Kill|Weapon|Aim|Shoot|Health|Damage)/i.test(x.name));
console.log('\n--- NPC / Combat candidates ---');
for (const x of combat_npc.slice(0, 20)) {
    console.log(`${x.name.padEnd(35)} 0x${x.fnVa.toString(16)}`);
}
