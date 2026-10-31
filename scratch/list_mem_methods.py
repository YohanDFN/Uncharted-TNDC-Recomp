import json

with open('tools/u2_map/u2_engine_map.json', 'r', encoding='utf-8') as f:
    asserts = json.load(f)['assertions']

mem_asserts = [a for a in asserts if 'memory.cpp' in a.get('message', '')]
print(f"Total assertions in memory.cpp: {len(mem_asserts)}")

methods = set()
for a in mem_asserts:
    methods.add((a.get('function_offset'), a.get('cpp_file'), a.get('expression')))

for fn, method, expr in sorted(methods):
    print(f"  {fn} -> {method:30s} (assert: {expr})")

