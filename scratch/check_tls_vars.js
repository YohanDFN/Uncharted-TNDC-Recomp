const fs = require('fs');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const imageFileOffset = 0x183ce8;

function readGuestI64(guestOffset) {
    const b = Buffer.alloc(8);
    fs.readSync(fd, b, 0, 8, imageFileOffset + guestOffset);
    return b.readBigInt64LE(0);
}

for (let off = 0x104aca0; off <= 0x104ad20; off += 8) {
    const val = readGuestI64(off);
    console.log(`0x${off.toString(16)}: ${val} (0x${val.toString(16)})`);
}

fs.closeSync(fd);

