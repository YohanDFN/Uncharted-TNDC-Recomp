const fs = require('fs');

const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));
const sourceFiles = data.source_files || {};
let memPath = null;
let memInfo = null;

for (const [k, v] of Object.entries(sourceFiles)) {
    if (k.includes('memory.cpp')) {
        memPath = k;
        memInfo = v;
        break;
    }
}

console.log('Path:', memPath);
if (memInfo) {
    console.log('Functions count:', memInfo.functions?.length || 0);
    console.log('Methods:', memInfo.methods);
    console.log('Sample functions:', (memInfo.functions || []).slice(0, 30));
}

const assertions = data.assertions || [];
const memAsserts = assertions.filter(a => a.file && a.file.includes('memory.cpp'));
console.log('Assertions count:', memAsserts.length);
console.log('Sample assertions:');
for (const a of memAsserts.slice(0, 25)) {
    console.log(`  L${a.line}: [${a.method}] ${a.expr} -> ${a.msg || ''}`);
}

