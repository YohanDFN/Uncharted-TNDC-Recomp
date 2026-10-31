const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

console.log('Top keys:', Object.keys(data));
if (data.assertions && data.assertions.length > 0) {
    console.log('First assertion:', data.assertions[0]);
    const mem = data.assertions.filter(a => JSON.stringify(a).toLowerCase().includes('memory'));
    console.log('Memory assertions count:', mem.length);
    for (const a of mem.slice(0, 15)) {
        console.log(a);
    }
}

