const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));
const dcNatives = data.dc_natives || {};

const files = ['src/runtime_u2_player.c', 'src/runtime_u2_npc.c', 'src/runtime_u2_scene.c', 'src/runtime_u2_audio.c', 'src/runtime_u2_event.c'];
const hooked = new Set();
for (let f of files) {
    const txt = fs.readFileSync(f, 'utf8');
    const re = /\{\s*"([A-Za-z0-9_]+)"\s*,\s*(0x[0-9A-Fa-f]+)\s*,/g;
    let m;
    while ((m = re.exec(txt)) !== null) {
        hooked.add(m[1]);
    }
}

const total = Object.keys(dcNatives).length;
console.log(`Total DC natives in DB: ${total}`);
console.log(`Hooked and unique: ${hooked.size}`);
console.log(`Remaining unhooked: ${total - hooked.size}`);

