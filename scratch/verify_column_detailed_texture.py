"""Offline comparison of the selected dictionary resource with an original GPU capture."""
import argparse
import csv
import hashlib
import json
import struct
from pathlib import Path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--game-dir', type=Path, required=True)
    parser.add_argument('--gpu-capture', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    root = Path(__file__).resolve().parent.parent
    mappings = json.loads((root/'docs/preuves_avancement/2026-11-12/material-image-names.json').read_text(encoding='utf-8'))['mappings']
    references = [r for r in mappings if r['material'] == 7 and r['parameter'] == 'g_tColorMap']
    assert len(references) == 1
    cooked = references[0]['cooked']+'.ndb'
    directory = args.game_dir/'u2data/build/main/texturedict1/ps4'
    with (directory/'TextureDictionaries.csv').open(encoding='utf-8-sig', newline='') as f:
        rows = [{k.strip(): v.strip() for k, v in row.items()}
                for row in csv.DictReader(f, skipinitialspace=True)]
    rows = [r for r in rows if r['path']+'.ndb' == cooked]
    assert len(rows) == 1 and rows[0]['dictionary'] == 'chunk-21'
    archive = directory/'chunk-21-tstrm.pak'
    with archive.open('rb') as f:
        header = f.read(0x2C)
        count, table = struct.unpack_from('<II', header, 0x10)
        assert count == 1 and struct.unpack_from('<I', header, 8)[0] == 0
        f.seek(table)
        page_offset, size, kind = struct.unpack('<III', f.read(12))
        assert kind == 0 and size <= 4*1024*1024
        f.seek(page_offset)
        page = f.read(size)
        assert len(page) == size
        vram = page_offset+size
        assert archive.stat().st_size-vram == struct.unpack_from('<I', header, 0x20)[0]
        u32 = lambda at: struct.unpack_from('<I', page, at)[0]
        u64 = lambda at: struct.unpack_from('<Q', page, at)[0]
        def text(at):
            assert 0 <= at < len(page)
            return page[at:page.index(0, at)].decode('utf-8')
        root_offset = struct.unpack_from('<I', header, 12)[0]
        assert text(u64(root_offset+8)) == 'PAK_LOGIN_TABLE'
        hits = []
        for item in range(u32(root_offset+0x20)):
            entry = root_offset+0x28+item*8
            assert u32(entry) == 0
            metadata = u32(entry+4)
            if text(u64(metadata+8)) != 'TEXTURE_DICTIONARY':
                continue
            for i in range(u32(metadata+0x20)):
                descriptor = u64(u64(metadata+0x38)+i*8)
                if text(u64(descriptor+0x38)) == cooked:
                    assert u32(u64(metadata+0x28)+i*4) == u32(descriptor+0x14)
                    hits.append((i, descriptor, text(u64(u64(metadata+0x30)+i*8))))
        assert len(hits) == 1
        index, descriptor, label = hits[0]
        offset, stored, uid, layers, fmt, flags, mips, width, height = struct.unpack_from('<QIIIIIIII', page, descriptor+8)
        assert (stored, layers, fmt, flags, mips, width, height, u32(descriptor+0x34)) == (701440, 1, 72, 0x010F0000, 11, 1024, 1024, 2)
        assert offset+stored <= archive.stat().st_size-vram
        f.seek(vram+offset)
        blocks = f.read(stored)
    captured = args.gpu_capture.read_bytes()
    assert blocks == captured
    sha = lambda b: hashlib.sha256(b).hexdigest()
    low = (root/'docs/preuves_avancement/2026-11-12/column-runtime-image.bin').read_bytes()
    mip4_offset = sum((((max(1,width>>i)+3)//4+7)//8*8)*
                      (((max(1,height>>i)+3)//4+7)//8*8)*8 for i in range(4))
    assert blocks[mip4_offset:mip4_offset+2048] == low[:2048]
    report = {'source': 'exact cooked name from material 7 g_tColorMap', 'csv': rows[0],
              'dictionary': archive.name, 'page_offset': hex(page_offset), 'page_bytes': size,
              'descriptor_page_offset': hex(descriptor), 'dictionary_index': index,
              'resource_label': label, 'cooked_name': cooked, 'uid': hex(uid),
              'vram_file_offset': hex(vram), 'image_vram_offset': hex(offset), 'stored_bytes': stored,
              'width': width, 'height': height, 'mip_count': mips, 'format': fmt,
              'flags': hex(flags), 'layout_tag': u32(descriptor+0x34),
              'compressed_sha256': sha(blocks), 'equals_original_gpu_capture': True,
              'original_gpu_layout': 'Bc1RgbaSrgbBlock, Thin1DThin, 11 levels, 1 layer, 1 sample',
              'mip4_offset': mip4_offset, 'mip4_bytes': 2048,
              'mip4_equals_resident_first_mip': True, 'mip4_sha256': sha(low[:2048]),
              'entire_low_tail_identical': blocks[mip4_offset:] == low,
              'limits': 'first-mip preview, not the original streaming policy; no LOD or anisotropy'}
    args.output.write_bytes((json.dumps(report, ensure_ascii=False, indent=2)+'\n').encode('utf-8'))
    print('Full 1024 chain equals original GPU capture; mip4 equals resident 64x64 first mip.')


if __name__ == '__main__':
    main()
