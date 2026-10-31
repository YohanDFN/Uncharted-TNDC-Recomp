const fs = require('fs');

const fd = fs.openSync('out/big2/boot-linked.bin', 'r');
const imageFileOffset = 0x183ce8;

function checkBytes(offset, name) {
    const b = Buffer.alloc(16);
    fs.readSync(fd, b, 0, 16, imageFileOffset + offset);
    console.log(`0x${offset.toString(16).toUpperCase()}: [${name}] -> ${b.toString('hex').match(/../g).join(' ')}`);
}

checkBytes(0x00B35BA0, 'AllocateAllocStack');
checkBytes(0x00B35C80, 'InitializeThreadAllocStack');
checkBytes(0x00B35D80, 'FreeThreadAllocStack');
checkBytes(0x00B360D0, 'PushAllocator Direct');
checkBytes(0x00B361C0, 'PopAllocator');
checkBytes(0x00B362A0, 'GetAllocatorStackDepth');

fs.closeSync(fd);

