import json

with open('tools/u2_map/u2_engine_map.json', 'r', encoding='utf-8') as f:
    d = json.load(f)

print("Assertions type:", type(d.get('assertions')))
if isinstance(d.get('assertions'), dict):
    print("Assertions keys sample:", list(d['assertions'].keys())[:10])
    first_key = list(d['assertions'].keys())[0]
    print("Sample assertion:", d['assertions'][first_key])
elif isinstance(d.get('assertions'), list):
    print("Assertions len:", len(d['assertions']))
    if d['assertions']:
        print("Sample assertion:", d['assertions'][0])

