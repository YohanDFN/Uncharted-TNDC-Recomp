const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

for (const [k, v] of Object.entries(data.source_files || {})) {
    for (const f of v.functions || []) {
        if (f.toLowerCase().includes('a12d')) {
            console.log(`Source: ${k}, fn: ${f}`);
        }
    }
}
const dc = data.dc_natives || [];
for (const d of dc) {
    if (d.name && d.name.toLowerCase().includes('not')) {
        console.log(`DC native:`, d);
    }
}

