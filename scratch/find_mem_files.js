const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

const files = Object.keys(data.source_files || {}).filter(k => k.includes('corelib/memory') || k.includes('corelib\\memory'));
console.log('Source files in corelib/memory:');
for (const f of files) {
    console.log(`- ${f} (${data.source_files[f].functions?.length || 0} functions)`);
}

