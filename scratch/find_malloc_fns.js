const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

for (const k of Object.keys(data.source_files || {})) {
    if (k.includes('memory-new-malloc') || k.includes('memory-ps4.cpp')) {
        console.log(`\n=== File: ${k} ===`);
        console.log(data.source_files[k].functions);
        const asserts = (data.assertions || []).filter(a => a.message && a.message.includes(k.replace(/.*\//, '')));
        for (const a of asserts) {
            console.log(`  0x${a.function_offset}: [${a.cpp_file}] ${a.expression}`);
        }
    }
}

