const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

const memAsserts = (data.assertions || []).filter(a => 
    a.message && a.message.includes('memory')
);

const topAss = memAsserts.filter(a => JSON.stringify(a).includes('Top'));
console.log('Top assertions:', topAss);

// Search for any assertion referencing m_allocatorStackSize
const stackAss = (data.assertions || []).filter(a => 
    a.expression && a.expression.includes('m_allocatorStackSize')
);
console.log('Total stack assertions across whole binary:', stackAss.length);
const callers = new Set(stackAss.map(a => a.function_offset));
console.log('Unique function offsets calling/inlining stack checks:', callers.size);
for (const c of Array.from(callers).slice(0, 15)) {
    console.log(`  ${c}`);
}

