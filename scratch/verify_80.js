const fs = require('fs');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const imageFileOffset = 0x183ce8;
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

const cleanDetours = [];
while ((match = regex.exec(scriptC)) !== null) {
    const name = match[1];
    const offsetStr = match[2];
    const offset = parseInt(offsetStr, 16);
    const info = dcMapByName.get(name.toLowerCase());
    if (info && parseInt(info.func_offset, 16) === offset) {
        cleanDetours.push({ name, offset: offsetStr, lisp: info.lisp_symbol });
    }
}

console.log(`Total valid, verified DC natives: ${cleanDetours.length}`);
fs.closeSync(fd);

