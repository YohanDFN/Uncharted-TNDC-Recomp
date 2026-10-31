const fs = require('fs');
const data = JSON.parse(fs.readFileSync('tools/u2_map/u2_engine_map.json', 'utf8'));

function findFunction(addrHex) {
    const target = parseInt(addrHex, 16);
    console.log(`\nLooking for 0x${addrHex}:`);
    for (const [file, info] of Object.entries(data.source_files || {})) {
        for (const fn of info.functions || []) {
            const fOff = parseInt(fn, 16);
            if (Math.abs(fOff - target) < 0x200) {
                console.log(`  Source file: ${file}, fn: ${fn} (diff: ${target - fOff})`);
            }
        }
    }
}

findFunction('a1b564');
findFunction('a164eb');
findFunction('3fbd00');
findFunction('b3db0e');
findFunction('b2a389');

