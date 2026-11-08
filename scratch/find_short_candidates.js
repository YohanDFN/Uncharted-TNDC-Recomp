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

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const imageOffset = 0x183ce8;

const results = [];
for (const [name, entry] of Object.entries(dcNatives)) {
    if (hooked.has(name)) continue;
    const off = parseInt(entry.func_offset, 16);
    if (isNaN(off) || off <= 0) continue;
    const buf = Buffer.alloc(128);
    fs.readSync(fd, buf, 0, 128, imageOffset + off);
    let retIdx = -1;
    for (let i = 0; i < buf.length; ++i) {
        if (buf[i] === 0xc3) {
            retIdx = i;
            break;
        }
    }
    if (retIdx !== -1 && retIdx < 60) {
        results.push({
            name,
            off: entry.func_offset,
            len: retIdx + 1,
            hex: buf.subarray(0, retIdx + 1).toString('hex')
        });
    }
}
fs.closeSync(fd);

results.sort((a, b) => a.len - b.len);
console.log(`Found ${results.length} short unhooked DC functions (< 60 bytes):`);
results.slice(0, 25).forEach(r => {
    console.log(`  ${r.name.padEnd(35)} @ ${r.off} (${r.len} bytes, hex: ${r.hex})`);
});
