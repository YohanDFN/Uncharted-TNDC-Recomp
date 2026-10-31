const fs = require('fs');

const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));
const scriptC = fs.readFileSync('src/runtime_u2_script.c', 'utf8');

const dcNatives = data.dc_natives || {};

// Find currently detoured offsets in scriptC
const regex = /\{\s*"([^"]+)",\s*(0x[0-9a-fA-F]+),\s*\(uintptr_t\)/g;
let match;
const installedOffsets = new Set();
while ((match = regex.exec(scriptC)) !== null) {
    installedOffsets.add(parseInt(match[2], 16));
}

console.log(`Total dc_natives in engine map: ${Object.keys(dcNatives).length}`);
console.log(`Currently installed: ${installedOffsets.size}`);

const candidates = [];
for (const [name, info] of Object.entries(dcNatives)) {
    const off = parseInt(info.func_offset, 16);
    if (!installedOffsets.has(off)) {
        candidates.push({
            name,
            lisp: info.lisp_symbol,
            hash: info.symbol_hash,
            offset: info.func_offset,
            prologue: info.prologue_hex
        });
    }
}

console.log(`Remaining candidates: ${candidates.length}`);

// Group by category based on name prefix or keywords
const categories = {
    Math_Vector: [],
    Entity_Transform: [],
    Camera: [],
    Animation: [],
    Sound_Audio: [],
    Physics_Collision: [],
    Spawner: [],
    Other: []
};

for (const c of candidates) {
    const n = c.name.toLowerCase();
    if (n.includes('vector') || n.includes('matrix') || n.includes('quat') || n.includes('math') || n.includes('angle') || n.includes('rand') || n.includes('float') || n.includes('int') || n.includes('distance')) {
        categories.Math_Vector.push(c);
    } else if (n.includes('entity') || n.includes('transform') || n.includes('pos') || n.includes('rot')) {
        categories.Entity_Transform.push(c);
    } else if (n.includes('camera')) {
        categories.Camera.push(c);
    } else if (n.includes('anim')) {
        categories.Animation.push(c);
    } else if (n.includes('snd') || n.includes('sound') || n.includes('audio') || n.includes('vox') || n.includes('music')) {
        categories.Sound_Audio.push(c);
    } else if (n.includes('physics') || n.includes('collision') || n.includes('raycast') || n.includes('sphere')) {
        categories.Physics_Collision.push(c);
    } else if (n.includes('spawner') || n.includes('spawn')) {
        categories.Spawner.push(c);
    } else {
        categories.Other.push(c);
    }
}

for (const [cat, list] of Object.entries(categories)) {
    console.log(`\nCategory [${cat}]: ${list.length} functions`);
    for (const item of list.slice(0, 15)) {
        console.log(`  ${item.name.padEnd(35)} @ ${item.offset} (${item.lisp})`);
    }
}

