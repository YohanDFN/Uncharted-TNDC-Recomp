const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

const ialloc = (data.assertions || []).filter(a => 
    JSON.stringify(a).includes('IAllocator') || JSON.stringify(a).includes('Allocator')
);
console.log('Allocator assertions count:', ialloc.length);
const sample = ialloc.slice(0, 20);
for (const s of sample) {
    console.log(`0x${s.function_offset} [${s.cpp_file}] ${s.expression} (${s.message})`);
}

