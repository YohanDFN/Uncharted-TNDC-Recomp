const fs = require('fs');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const imageFileOffset = 0x183ce8;

const b = Buffer.alloc(0x60);
fs.readSync(fd, b, 0, 0x60, imageFileOffset + 0x00B35D40);

for (let i = 0; i < 0x60; i += 16) {
    const addr = (0x00B35D40 + i).toString(16).toUpperCase();
    const hex = b.subarray(i, i + 16).toString('hex').match(/../g).join(' ');
    console.log(`0x${addr}: ${hex}`);
}

fs.closeSync(fd);

