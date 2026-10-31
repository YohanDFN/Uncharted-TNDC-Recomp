const fs = require('fs');

const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));
const scriptC = fs.readFileSync('src/runtime_u2_script.c', 'utf8');

const regex = /\{\s*"([^"]+)",\s*(0x[0-9a-fA-F]+),\s*\(uintptr_t\)/g;
let match;

const dcNatives = data.dc_natives || {};
const dcMapByName = new Map();
for (const [k, v] of Object.entries(dcNatives)) {
    dcMapByName.set(k.toLowerCase(), v);
    if (v.cpp_name) dcMapByName.set(v.cpp_name.toLowerCase(), v);
}

const mismatches = [];
const verified = [];

while ((match = regex.exec(scriptC)) !== null) {
    const name = match[1];
    const offsetStr = match[2];
    const offset = parseInt(offsetStr, 16);
    
    const info = dcMapByName.get(name.toLowerCase());
    if (info) {
        const realOffset = parseInt(info.func_offset, 16);
        if (realOffset !== offset) {
            mismatches.push({
                name,
                detourOffset: offsetStr,
                realOffset: info.func_offset,
                diff: offset - realOffset,
                lisp: info.lisp_symbol
            });
        } else {
            verified.push({ name, offset: offsetStr });
        }
    } else {
        mismatches.push({
            name,
            detourOffset: offsetStr,
            realOffset: 'NOT_FOUND_IN_ENGINE',
            diff: 0
        });
    }
}

console.log(`Verified matches: ${verified.length}`);
console.log(`Mismatches or not found in dc_natives: ${mismatches.length}`);
for (const m of mismatches) {
    console.log(`  ${m.name}: detour=${m.detourOffset}, real=${m.realOffset} (diff=${m.diff})`);
}

