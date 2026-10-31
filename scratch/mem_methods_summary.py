import json

with open('tools/u2_map/u2_engine_map.json', 'r', encoding='utf-8') as f:
    asserts = json.load(f)['assertions']

mem_asserts = [a for a in asserts if 'memory.cpp' in a.get('message', '')]
method_counts = {}
for a in mem_asserts:
    m = a.get('cpp_file')
    e = a.get('expression')
    method_counts[m] = method_counts.get(m, 0) + 1

print(f"Total methods found in memory.cpp: {len(method_counts)}")
for m, c in sorted(method_counts.items(), key=lambda x: -x[1]):
    print(f"  {m:35s}: {c} assertion occurrences")

