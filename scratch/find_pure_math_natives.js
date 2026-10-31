const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

const dc = data.dc_natives || {};

const mathFns = [];
for (const [k, v] of Object.entries(dc)) {
    const s = (v.lisp_symbol || '').toLowerCase();
    const name = k.toLowerCase();
    if (s.includes('vec') || s.includes('dist') || s.includes('angle') || s.includes('point') || s.includes('math') || s.includes('matrix') || s.includes('quat') || s.includes('rot') || s.includes('dot') || s.includes('cross') || s.includes('lerp') || s.includes('clamp') || s.includes('abs') || s.includes('sin') || s.includes('cos') || s.includes('tan') || s.includes('sqrt') || s.includes('random')) {
        mathFns.push(v);
    }
}

console.log(`Found ${mathFns.length} math/vector/geometry natives:`);
for (const m of mathFns) {
    console.log(`  ${m.cpp_name.padEnd(35)} @ ${m.func_offset} (${m.lisp_symbol})`);
}

