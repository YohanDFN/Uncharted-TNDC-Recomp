const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

const sta = (data.assertions || []).filter(a => 
    JSON.stringify(a).includes('ScopedTempAllocator') || (a.message && a.message.includes('scoped-temp-allocator'))
);
console.log('ScopedTempAllocator assertions:');
for (const s of sta) {
    console.log(`0x${s.function_offset} [${s.cpp_file}] ${s.expression} (${s.message})`);
}

