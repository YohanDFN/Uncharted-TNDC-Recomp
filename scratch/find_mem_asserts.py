import json

with open('tools/u2_map/u2_engine_map.json', 'r', encoding='utf-8') as f:
    asserts = json.load(f)['assertions']

mem_a = [a for a in asserts if any(x in a.get('file', '').lower() for x in ['alloc', 'heap', 'mem'])]
print(f"Total memory-related assertions: {len(mem_a)}")
for a in mem_a[:35]:
    print(f"{a.get('file')}:{a.get('line')} in {a.get('method')}(): {a.get('expr')}")

