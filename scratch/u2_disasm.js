#!/usr/bin/env node
// SPDX-License-Identifier: GPL-2.0-or-later
/**
 * Instant Disassembler & String/Symbol Resolver for Uncharted 2 ELF.
 * Usage:
 *   node scratch/u2_disasm.js <VA_HEX> [length_bytes]
 * Example:
 *   node scratch/u2_disasm.js 0x00CC1010 64
 */

const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

const ROOT = path.resolve(__dirname, '..');
const ELF_PATH = path.join(ROOT, 'out', 'big2', 'eboot.elf');
const MAP_PATH = path.join(ROOT, 'tools', 'u2_map', 'u2_engine_map.json');

const rawArg = process.argv[2];
if (!rawArg) {
    console.log("Usage: node scratch/u2_disasm.js <VA_HEX> [length_bytes]");
    process.exit(1);
}

const targetVA = parseInt(rawArg.replace(/^0x/i, ''), 16);
const length = parseInt(process.argv[3] || '64', 10);

if (!fs.existsSync(ELF_PATH)) {
    console.error("ELF not found:", ELF_PATH);
    process.exit(1);
}

const elf = fs.readFileSync(ELF_PATH);

// Helper to read zero-terminated string from ELF virtual address
function readStringAtVA(va) {
    const fileOffset = 0x4000 + va;
    if (fileOffset < 0 || fileOffset >= elf.length) return null;
    let end = fileOffset;
    while (end < elf.length && elf[end] !== 0 && (end - fileOffset) < 120) {
        const c = elf[end];
        if (c < 32 || c > 126) return null; // printable ASCII check
        end++;
    }
    if (end > fileOffset && elf[end] === 0) {
        return elf.slice(fileOffset, end).toString('utf8');
    }
    return null;
}

// Load symbol / assertion map if available
let functionMap = new Map();
if (fs.existsSync(MAP_PATH)) {
    try {
        const mapData = JSON.parse(fs.readFileSync(MAP_PATH, 'utf8'));
        if (Array.isArray(mapData.assertions)) {
            for (const a of mapData.assertions) {
                const foff = parseInt(a.function_offset, 16);
                if (!functionMap.has(foff)) {
                    functionMap.set(foff, `${a.cpp_file} (${a.message || ''})`);
                }
            }
        }
    } catch (_) {}
}

const startAddr = Math.max(0, targetVA - 0x10);
const stopAddr = targetVA + length;

console.log(`\n=== Disassembly at 0x${targetVA.toString(16)} (Range: 0x${startAddr.toString(16)} - 0x${stopAddr.toString(16)}) ===\n`);

try {
    const objdumpCmd = `llvm-objdump -d --start-address=0x${startAddr.toString(16)} --stop-address=0x${stopAddr.toString(16)} "${ELF_PATH}"`;
    const out = execSync(objdumpCmd, { encoding: 'utf8' });
    const lines = out.split(/\r?\n/);
    
    for (const line of lines) {
        if (!line.includes(':')) continue;
        const match = line.match(/^\s*([0-9a-fA-F]+):\s+([0-9a-fA-F\s]+)\s+([a-z0-9]+)\s*(.*)$/);
        if (!match) {
            console.log(line);
            continue;
        }
        
        const addrHex = match[1];
        const currentAddr = parseInt(addrHex, 16);
        const bytes = match[2].trim();
        const mnemonic = match[3];
        const operands = match[4].trim();
        
        const isTarget = currentAddr === targetVA;
        const marker = isTarget ? " >>> " : "     ";
        
        let extra = "";
        
        // RIP-relative resolution
        const ripMatch = operands.match(/#\s+0x([0-9a-fA-F]+)/);
        if (ripMatch) {
            const target = parseInt(ripMatch[1], 16);
            const str = readStringAtVA(target);
            if (str) {
                extra += `  ; [str: "${str}"]`;
            }
        }
        
        // Function symbol match
        if (functionMap.has(currentAddr)) {
            extra += `  ; [Func: ${functionMap.get(currentAddr)}]`;
        }
        
        console.log(`${marker}${addrHex.padStart(8, '0')}:  ${bytes.padEnd(20, ' ')}  ${mnemonic.padEnd(8, ' ')} ${operands}${extra}`);
    }
} catch (e) {
    console.error("Disassembly error:", e.message);
}
console.log();

