const fs = require('fs');
const engineMap = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));
const dcNatives = engineMap.dc_natives || {};

const srcFiles = [
    'src/runtime_u2_player.c',
    'src/runtime_u2_npc.c',
    'src/runtime_u2_scene.c',
    'src/runtime_u2_audio.c',
    'src/runtime_u2_event.c'
];

const hooked = new Set();
for (const file of srcFiles) {
    const content = fs.readFileSync(file, 'utf8');
    const regex = /\{\s*"([A-Za-z0-9_]+)"\s*,\s*(0x[0-9A-Fa-f]+)\s*,/g;
    let match;
    while ((match = regex.exec(content)) !== null) {
        hooked.add(match[1]);
    }
}

const unhooked = [];
for (const [name, entry] of Object.entries(dcNatives)) {
    if (!hooked.has(name)) {
        unhooked.push({
            name,
            offset: entry.func_offset,
            prologue: entry.prologue_hex || ''
        });
    }
}

const filterArg = process.argv[2] || '';
const filtered = unhooked.filter(u => !filterArg || u.name.toLowerCase().includes(filterArg.toLowerCase()));

console.log(`Unhooked candidates matching '${filterArg}': ${filtered.length} (total unhooked: ${unhooked.length})`);
filtered.slice(0, 15).forEach(c => {
    console.log(`  ${c.name.padEnd(35)} @ ${c.offset} (prologue: ${c.prologue.substring(0, 16)})`);
});
