const fs = require('fs');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const table = Buffer.alloc(256 * 4);
fs.readSync(fd, table, 0, 256 * 4, 0x183ce8 + 0xdf0390);

const crcTable = new Uint32Array(256);
for (let i = 0; i < 256; i++) {
    crcTable[i] = table.readUInt32LE(i * 4);
}

function nd_hash(str, init = 0) {
    let edi = init >>> 0;
    for (let i = 0; i < str.length; i++) {
        const cl = str.charCodeAt(i);
        const edx = (edi >>> 24) & 0xFF;
        const ecx = (cl ^ edx) & 0xFF;
        edi = ((edi << 8) ^ crcTable[ecx]) >>> 0;
    }
    return edi >>> 0;
}

// Test with "screen-space-tire-track" from 0x7e10 where edi was 0
const test = nd_hash("screen-space-tire-track", 0);
console.log('Test "screen-space-tire-track" with init=0:', '0x' + test.toString(16));

// Search all ASCII strings in boot-linked.bin
const bin = fs.readFileSync('out/big2/boot-linked.bin');
console.log('Extracting and hashing strings from binary...');

const targets = new Set([0xcd0fbe5a, 0xad087087, 0x9ec73b22, 0x309861e2, 0x82da0078]);
const found = new Map();

let start = -1;
for (let i = 0; i < bin.length; i++) {
    const b = bin[i];
    if ((b >= 32 && b <= 126)) {
        if (start === -1) start = i;
    } else {
        if (start !== -1 && (i - start) >= 2) {
            const str = bin.slice(start, i).toString('utf8');
            // Check with init 0 and init 0xFFFFFFFF
            const h0 = nd_hash(str, 0);
            if (targets.has(h0) && !found.has(h0)) {
                found.set(h0, { str, init: 0 });
                console.log('FOUND 0x' + h0.toString(16) + ' (init=0): "' + str + '"');
            }
            const hF = nd_hash(str, 0xFFFFFFFF);
            if (targets.has(hF) && !found.has(hF)) {
                found.set(hF, { str, init: 0xFFFFFFFF });
                console.log('FOUND 0x' + hF.toString(16) + ' (init=0xFFFFFFFF): "' + str + '"');
            }
            // Also test lowercase
            const lower = str.toLowerCase();
            if (lower !== str) {
                const hl0 = nd_hash(lower, 0);
                if (targets.has(hl0) && !found.has(hl0)) {
                    found.set(hl0, { str: lower, init: 0 });
                    console.log('FOUND 0x' + hl0.toString(16) + ' (init=0, lower): "' + lower + '"');
                }
            }
        }
        start = -1;
    }
}

console.log('Search complete. Found ' + found.size + ' targets.');

