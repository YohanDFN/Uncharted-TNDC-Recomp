"""Verify the bounded column capture against the PAK and cached translated shaders.

Run after the capture described in docs/NATIVE_TEMPLE_COLOR_SAMPLING.md.
No process writes, package modifications or shader execution.
"""
import argparse
import hashlib
import json
import re
import struct
import subprocess
from pathlib import Path


def sha(data):
    return hashlib.sha256(data).hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--pak', type=Path, required=True)
    parser.add_argument('--runtime-dir', type=Path, default=Path('user'))
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    root = Path(__file__).resolve().parent.parent
    runtime = args.runtime_dir
    pak = args.pak.read_bytes()
    count, table = struct.unpack_from('<II', pak, 0x10)
    assert count == 2 and 0x18 <= table <= len(pak)-count*12
    page, page_size, kind = struct.unpack_from('<III', pak, table)
    assert kind == 0 and page <= len(pak)-page_size
    uv_row = json.loads((root/'docs/preuves_avancement/2026-11-12/uv-streams.json').read_text())[7]
    uv_offset, uv_size = int(uv_row['data_offset'], 16), uv_row['vertices']*4
    assert uv_offset <= page_size-uv_size
    uv = pak[page+uv_offset:page+uv_offset+uv_size]
    assert uv == (runtime/'diag_column_uv.bin').read_bytes()
    # Historical resident extent independently matched to the original GPU;
    # diagnostic comparison only, never used by the native texture loader.
    column = pak[0xA262B8:0xA262B8+5120]
    assert sha(column) == '4b9f0e9526ade0aab5edd273636e69236efc7b24b46b9e7d06dda2109829923f'
    for binding in (1, 4):
        assert column == (runtime/f'diag_column_image_{binding}.bin').read_bytes()
    shaders = {}
    for key in ('f1ef5c58', '50411a0b'):
        path = runtime/f'cache/CUSA02343/0x00000000{key}_0.spv'
        asm = subprocess.check_output(['spirv-dis', str(path)], text=True)
        shaders[key] = (sha(path.read_bytes()), asm)
    fs, vs = shaders['f1ef5c58'][1], shaders['50411a0b'][1]
    # These result IDs are scoped to the captured SPIR-V variant and hashed
    # below. Fail explicitly on another compiler/cache variant.
    fs_ids = (216,217,218,219,220,221,222,223,224,225,226,227,
              228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244)
    vs_ids = (299,300,301,302,303,304,305,306,307,308,714,715,716,717)
    def instructions(asm, ids):
        lines = asm.splitlines()
        result = []
        for i, line in enumerate(lines):
            match = re.match(r'\s*%(\d+) = ', line)
            if match and int(match[1]) in ids:
                result.append(line.strip())
                if i+1 < len(lines) and 'OpStore' in lines[i+1]:
                    result.append(lines[i+1].strip())
        return result
    assert '%241 = OpLoad %69 %fs_img1' in fs
    assert '%242 = OpLoad %82 %fs_samp0' in fs
    assert '%243 = OpSampledImage %72 %241 %242' in fs
    assert '%240 = OpCompositeConstruct %f32vec2_id %233 %239' in fs
    assert '%300 = OpAccessChain %_ptr_StorageBuffer_u32_id %ssbo_7 %u32_id_0 %299' in vs
    assert '%302 = OpExtInst %f32vec2_id %218 UnpackHalf2x16 %301' in vs
    for pointer, value in ((714,307),(715,308),(716,307),(717,308)):
        assert f'OpStore %{pointer} %{value}' in vs
    report = {
        'scenario': 'menu-temple, native snapshot mesh 7 / Submesh21, g_tColorMap',
        'capture': 'bounded FindPreparedStage diagnostic, two signature-matched draws, no guest writes',
        'page0': hex(page), 'uv_page_offset': hex(uv_offset),
        'uv_bytes': len(uv), 'uv_sha256': sha(uv), 'uv_equals_original_shader_buffer': True,
        'resident_image_bytes': len(column), 'resident_image_sha256': sha(column),
        'image_bindings_matching_all_resident_bytes': [1, 4],
        'primary_image': 1, 'primary_sampler': 0,
        'sampler0_raw': ['00fff00000000400', '4000000008f00000'],
        'sampler0': {'wrap_xyz': True, 'normalized': True, 'min_mag': 'AnisoLinear',
                     'mip': 'Linear', 'max_anisotropy': 4, 'lod_bias': 0},
        'fs_spv_sha256': shaders['f1ef5c58'][0], 'vs_spv_sha256': shaders['50411a0b'][0],
        'fs_primary_sample_instructions': instructions(fs, fs_ids),
        'vs_uv_instructions': instructions(vs, vs_ids),
        'observation': 'UV half2 buffer copied unchanged to attr0.xy and zw; primary image1 uses interpolated attr0.xy and sampler0',
        'viewer_conventions': ['bilinear first resident mip only, no anisotropy or mip selection',
                               'sRGB decode/filter/light/encode, viewer directional light',
                               'base color only; other original material inputs are not reproduced'],
    }
    args.output.write_text(json.dumps(report, ensure_ascii=False, indent=2)+'\n', encoding='utf-8')
    print(f'UV and resident images match; shader instructions verified: {args.output}')


if __name__ == '__main__':
    main()
