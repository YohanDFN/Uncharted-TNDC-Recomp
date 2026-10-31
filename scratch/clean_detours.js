const fs = require('fs');

const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));
const scriptC = fs.readFileSync('src/runtime_u2_script.c', 'utf8');

const dcNatives = data.dc_natives || {};
const dcMapByName = new Map();
for (const [k, v] of Object.entries(dcNatives)) {
    dcMapByName.set(k.toLowerCase(), v);
    if (v.cpp_name) dcMapByName.set(v.cpp_name.toLowerCase(), v);
}

// Find lines in detours[]
const lines = scriptC.split('\n');
let inDetours = false;
let kept = 0, removed = 0;
const cleanLines = [];

for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    if (line.includes('const U2NativeDetour detours[] = {')) {
        inDetours = true;
        cleanLines.push(line);
        continue;
    }
    if (inDetours && line.includes('};')) {
        inDetours = false;
        cleanLines.push(line);
        continue;
    }
    if (inDetours) {
        const m = line.match(/\{\s*"([^"]+)",\s*(0x[0-9a-fA-F]+),\s*\(uintptr_t\)([^}]+)\},?/);
        if (m) {
            const name = m[1];
            const offsetStr = m[2];
            const hostFn = m[3].trim();
            const offset = parseInt(offsetStr, 16);
            const info = dcMapByName.get(name.toLowerCase());
            if (info && parseInt(info.func_offset, 16) === offset) {
                cleanLines.push(`        { "${name}", ${offsetStr}, (uintptr_t)${hostFn} },`);
                kept++;
            } else {
                cleanLines.push(`        /* Removed invalid offset: ${name} (${offsetStr}) */`);
                removed++;
            }
            continue;
        }
    }
    cleanLines.push(line);
}

console.log(`Kept ${kept} verified detours, removed ${removed} invalid detours.`);
fs.writeFileSync('src/runtime_u2_script.c', cleanLines.join('\n'));

