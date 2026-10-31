const fs = require('fs');
const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const buf = Buffer.alloc(64);
fs.readSync(fd, buf, 0, 64, 0);
fs.closeSync(fd);
console.log('Hex dump of first 64 bytes:', buf.toString('hex').match(/../g).join(' '));
console.log('ASCII:', buf.toString('latin1').replace(/[^\x20-\x7E]/g, '.'));

