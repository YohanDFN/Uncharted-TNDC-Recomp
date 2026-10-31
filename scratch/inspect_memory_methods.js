const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

const memAsserts = (data.assertions || []).filter(a => 
    a.message && a.message.includes('corelib\\memory\\memory.cpp')
);

console.log('Total asserts for memory.cpp:', memAsserts.length);

const methods = new Map();
for (const a of memAsserts) {
    const fnName = a.cpp_file || 'unknown';
    if (!methods.has(fnName)) {
        methods.set(fnName, new Set());
    }
    methods.get(fnName).add(a.expression);
}

console.log('Unique methods/functions in memory.cpp:');
for (const [name, exprs] of methods.entries()) {
    console.log(`- ${name} (${exprs.size} unique assertions):`);
    for (const e of Array.from(exprs).slice(0, 5)) {
        console.log(`    expr: "${e}"`);
    }
}

