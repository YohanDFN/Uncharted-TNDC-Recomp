import json

with open('tools/u2_map/u2_engine_map.json', 'r', encoding='utf-8') as f:
    data = json.load(f)

source_files = data.get('source_files', {})
mem_info = None
for k, v in source_files.items():
    if 'memory.cpp' in k:
        mem_info = (k, v)
        break

if mem_info:
    path, info = mem_info
    print(f"Path: {path}")
    print(f"Functions count: {len(info.get('functions', []))}")
    print("Methods identified:")
    for m in info.get('methods', []):
        print(f"  {m}")
    print("\nFirst 30 functions:")
    for fn in info.get('functions', [])[:30]:
        print(f"  {fn}")
else:
    print("memory.cpp not found!")

# Also check assertions
assertions = data.get('assertions', [])
mem_asserts = [a for a in assertions if 'memory.cpp' in a.get('file', '')]
print(f"\nAssertions in memory.cpp: {len(mem_asserts)}")
for a in mem_asserts[:25]:
    print(f"  line {a.get('line')}: {a.get('method')} -> {a.get('expr')}")
