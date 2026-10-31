const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

console.log('dc_natives type:', typeof data.dc_natives);
if (typeof data.dc_natives === 'object') {
    for (const [k, v] of Object.entries(data.dc_natives)) {
        if (k.toLowerCase().includes('not') || JSON.stringify(v).toLowerCase().includes('not') || JSON.stringify(v).includes('A12D')) {
            console.log(k, v);
        }
    }
}

