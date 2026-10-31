const fs = require('fs');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const imageFileOffset = 0x183ce8;

function readGuestU64(guestOffset) {
    const b = Buffer.alloc(8);
    fs.readSync(fd, b, 0, 8, imageFileOffset + guestOffset);
    return b.readBigUInt64LE(0);
}

function readGuestI64(guestOffset) {
    const b = Buffer.alloc(8);
    fs.readSync(fd, b, 0, 8, imageFileOffset + guestOffset);
    return b.readBigInt64LE(0);
}

const val = readGuestI64(0x104acd0);
console.log(`Value at 0x104acd0: ${val} (0x${val.toString(16)})`);

fs.closeSync(fd);

