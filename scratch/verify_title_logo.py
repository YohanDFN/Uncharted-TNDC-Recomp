"""Verify menu logo payloads against the bounded original-process captures."""
import argparse
import hashlib
import json
import struct
from pathlib import Path


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--game-dir', type=Path, required=True)
    parser.add_argument('--proof-dir', type=Path, default=Path('docs/preuves_avancement/2026-11-13'))
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    pak = args.game_dir/'u2data/build/main/pak22/ps4/menu.pak'
    with pak.open('rb') as f:
        prefix = f.read(0x2C)
        count, table = struct.unpack_from('<II', prefix, 0x10)
        assert count == 2
        f.seek(table)
        entries = [struct.unpack('<III', f.read(12)) for _ in range(count)]
        pages = []
        for offset, size, kind in entries:
            assert kind == 0 and size <= 4*1024*1024
            f.seek(offset)
            page = f.read(size)
            assert len(page) == size and struct.unpack_from('<I', page, 12)[0] == size
            pages.append(page)
        vram = max(offset+size for offset, size, _ in entries)
        assert pak.stat().st_size-vram == struct.unpack_from('<I', prefix, 0x20)[0]
        root_page, root = struct.unpack_from('<II', prefix, 8)
        rp = pages[root_page]
        records = []
        for login in range(struct.unpack_from('<I', rp, root+32)[0]):
            pi, metadata = struct.unpack_from('<II', rp, root+40+login*8)
            page = pages[pi]
            u32 = lambda at: struct.unpack_from('<I', page, at)[0]
            u64 = lambda at: struct.unpack_from('<Q', page, at)[0]
            def text(at):
                return page[at:page.index(0, at)].decode('utf-8')
            if text(u64(metadata+8)) != 'TEXTURE_TABLE':
                continue
            for i in range(u32(metadata+32)):
                descriptor = u64(u64(metadata+56)+i*8)
                name = text(u64(descriptor+56))
                for slot, suffix in enumerate(('logo2.tga/3DA08A9F6983BA12.ndb', 'logo2-alpha.tga/4C70F66839038231.ndb')):
                    if name != 'd:/dev/CP9/Data/U2/CookData/ND/build/texture2/art/general/menu/'+suffix:
                        continue
                    offset, size, uid, layers, fmt, flags, mips, width, height = struct.unpack_from('<QIIIIIIII', page, descriptor+8)
                    assert (size, layers, fmt, flags, mips, width, height, u32(descriptor+52)) == (702464,1,71 if slot == 0 else 80,0x01000000,12,2048,512,3)
                    assert uid == u32(u64(metadata+40)+i*4) and offset+size <= pak.stat().st_size-vram
                    f.seek(vram+offset)
                    payload = f.read(size)
                    capture = args.proof_dir/('title-color-gpu.bin' if slot == 0 else 'title-alpha-gpu.bin')
                    assert payload == capture.read_bytes()
                    records.append({'cooked_name': name, 'table_index': i, 'descriptor_page_offset': hex(descriptor),
                                    'uid': hex(uid), 'vram_offset': offset, 'bytes': size, 'width': width, 'height': height,
                                    'mip_count': mips, 'format': fmt, 'layout_tag': 3,
                                    'gpu_format': 'Bc1RgbaUnormBlock' if slot == 0 else 'Bc4UnormBlock',
                                    'gpu_tile_mode': 'Thin1DThin',
                                    'sha256': hashlib.sha256(payload).hexdigest(), 'equals_original_capture': True})
    assert len(records) == 2
    args.output.write_bytes((json.dumps({'archive':'menu.pak','vram_file_offset':vram,'planes':records,
        'limits':'identical payloads also occur in localized variants; this does not identify a unique locale or port the UI shader'},indent=2)+'\n').encode('utf-8'))
    print('Both complete logo chains match the captured original GPU resources.')


if __name__ == '__main__':
    main()
