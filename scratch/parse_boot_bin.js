const fs = require('fs');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');

// Read BBPROBE5 header
const headerBuf = Buffer.alloc(56);
fs.readSync(fd, headerBuf, 0, 56, 0);

const magic = headerBuf.subarray(0, 8).toString('utf8');
const size = headerBuf.readBigUInt64LE(8);
const entry = headerBuf.readBigUInt64LE(16);
const ns = Number(headerBuf.readBigUInt64LE(24));
const nr = Number(headerBuf.readBigUInt64LE(32));
const importCount = Number(headerBuf.readBigUInt64LE(40));
const flags = headerBuf.readBigUInt64LE(48);

console.log({ magic, size: Number(size), entry: Number(entry), ns, nr, importCount, flags: Number(flags) });

let pos = 56;
pos += 8; // procparam
pos += 32; // main_tls
const moduleCount = Number(readU64(pos)); pos += 8;
pos += moduleCount * 56;
const bindingCount = Number(readU64(pos)); pos += 8;
pos += bindingCount * 24;
pos += ns * 24; // segments
pos += importCount * 128; // names
pos += nr * 32; // relocations

const imageFileOffset = pos;
console.log(`Image starts at file offset: 0x${imageFileOffset.toString(16)} (${imageFileOffset})`);

function readU64(offset) {
    const b = Buffer.alloc(8);
    fs.readSync(fd, b, 0, 8, offset);
    return b.readBigUInt64LE(0);
}

// Helper to read memory at guest offset
function readGuestBytes(guestOffset, len) {
    const b = Buffer.alloc(len);
    fs.readSync(fd, b, 0, len, imageFileOffset + guestOffset);
    return b;
}

// Let's verify guest offset 0x00A14120 (ScriptManager::ExecuteScriptCode)
const u2Sig = readGuestBytes(0x00A14120, 16);
console.log('ExecuteScriptCode at 0x00A14120:', u2Sig.toString('hex').match(/../g).join(' '));

fs.closeSync(fd);

