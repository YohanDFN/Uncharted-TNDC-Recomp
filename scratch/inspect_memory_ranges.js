const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

const memFile = data.source_files['d:/dev/CP9/shared-ps4/src/corelib/memory/memory.cpp'];
const fns = memFile.functions.map(x => parseInt(x, 16)).sort((a,b) => a - b);

console.log('Total functions:', fns.length);
console.log('Function ranges:');
let ranges = [];
let start = fns[0], prev = fns[0];
for (let i = 1; i < fns.length; i++) {
    if (fns[i] - prev > 0x1000) {
        ranges.push({ start, end: prev, count: fns.filter(x => x >= start && x <= prev).length });
        start = fns[i];
    }
    prev = fns[i];
}
ranges.push({ start, end: prev, count: fns.filter(x => x >= start && x <= prev).length });

for (const r of ranges) {
    console.log(`0x${r.start.toString(16).padStart(8, '0')} - 0x${r.end.toString(16).padStart(8, '0')}: ${r.count} functions`);
}

// Find assertions corresponding to these functions
const asserts = (data.assertions || []).filter(a => a.message && a.message.includes('memory.cpp'));
console.log('\nFunction offsets with direct memory.cpp assertions in range 0x00B00000 - 0x00D00000:');
const seen = new Set();
for (const a of asserts) {
    const off = parseInt(a.function_offset, 16);
    if (off >= 0x00B00000 && off <= 0x00D00000 && !seen.has(off)) {
        seen.add(off);
        console.log(`0x${off.toString(16).padStart(8, '0')}: [${a.cpp_file}] trap=0x${parseInt(a.trap_offset, 16).toString(16)} expr="${a.expression}"`);
    }
}

