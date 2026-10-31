import json

with open("tools/u2_map/u2_engine_map.json", "r") as f:
    d = json.load(f)

natives = d.get("dc_natives", [])
print(f"Total dc_natives: {len(natives)}")

# Filter categories: Math, Vector, Event, Anim, Camera, Player, Sound, Logic
for n in natives:
    name = n.get("name", "")
    offset = n.get("func_offset", 0)
    print(f"{name:40} 0x{offset:08X}")

