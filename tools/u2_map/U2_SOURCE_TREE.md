# Uncharted 2 (`big2-ps4_Shipping.elf`) — Architecture & Code Source C++

- **Exécutable analysé** : `C:\Users\Yohan\Desktop\Uncharted\CUSA02343\big2-ps4_Shipping.elf`
- **Taille de l'image mémoire (Guest)** : `24,361,112` octets
- **Point d'entrée** : `0x000000A0`
- **Fonctions x86-64 détectées** : `16,420`
- **Fichiers sources C++ originaux (`.cpp` / `.h`)** : `574`
- **Assertions Naughty Dog (`int 0x41` avec nom de méthode C++ & ligne)** : `7,433`
- **Fonctions natives DC00 (`Dc*`)** : `843` (`833` adresses résolues)

## 1. Fichiers Sources C++ Originaux de Naughty Dog (`.cpp` / `.h`)

| Fichier Source C++ | Nb Fonctions | Méthodes C++ identifiées (Ligne) | Adresses (`image + offset`) |
|--------------------|--------------|----------------------------------|-----------------------------|
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/ai/agent/npc.h` | 346 | - | `0x0020A920`, `0x0020BA60`, `0x0020D050`, `0x00212010` (+342) |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/memory.cpp` | 272 | - | `0x001FAA70`, `0x001FB530`, `0x001FC080`, `0x001FF730` (+268) |
| `d:/dev/CP9/big2-ps4/src/game/ai/controller/animaction-controller.cpp` | 204 | - | `0x002B55E0`, `0x002B5680`, `0x002B6230`, `0x002B6290` (+200) |
| `d:/dev/CP9/big2-ps4/src/game/net/bitstream.cpp` | 164 | - | `0x005E1410`, `0x005E14C0`, `0x005E1550`, `0x005E15D0` (+160) |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/gamephys.cpp` | 152 | - | `0x0022D170`, `0x00243E40`, `0x00245BE0`, `0x00246D10` (+148) |
| `d:/dev/CP9/big2-ps4/src/game/anim/anim-control.cpp` | 141 | - | `0x00201210`, `0x0021EDA0`, `0x002249F0`, `0x00245BE0` (+137) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ndgi/ndgi-orbis-device.cpp` | 111 | - | `0x00575010`, `0x005BBD90`, `0x00B62B80`, `0x00B63F70` (+107) |
| `d:/dev/CP9/big2-ps4/src/game/state-script/ss-context.cpp` | 110 | - | `0x00236F40`, `0x002DDD20`, `0x002DDF20`, `0x002DE250` (+106) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/api.cpp` | 100 | - | `0x00142B70`, `0x00143110`, `0x001435B0`, `0x001437B0` (+96) |
| `d:/dev/CP9/big2-ps4/src/game/coll/feature-db-ref.cpp` | 80 | - | `0x0047DD20`, `0x0047DE70`, `0x0047DFC0`, `0x0047E250` (+76) |
| `d:/dev/CP9/shared-ps4/src/bputil/ucc-gameplay/bp_ucctweaks.cpp` | 76 | - | `0x00244EF0`, `0x0024FBE0`, `0x003C1C50`, `0x003FC0B0` (+72) |
| `d:/dev/CP9/big2-ps4/src/game/ai/agent/npc-manager.cpp` | 75 | - | `0x0021EDA0`, `0x0022E310`, `0x0022E790`, `0x0022E880` (+71) |
| `../../Common/Base/Thread/CriticalSection/Posix/hkPosixCriticalSection.inl` | 63 | - | `0x00064E70`, `0x00069A90`, `0x0006D3B0`, `0x0006D400` (+59) |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/havok.cpp` | 59 | - | `0x004D5290`, `0x004DB080`, `0x004E18C0`, `0x004E3750` (+55) |
| `d:/dev/CP9/big2-ps4/src/game/player/player-collision.cpp` | 59 | - | `0x00684F70`, `0x006853F0`, `0x00685760`, `0x00685AD0` (+55) |
| `d:/dev/CP9/shared-ps4/src/ndlib/io/streaming.cpp` | 55 | - | `0x003B5BB0`, `0x003B5CA0`, `0x003B5DA0`, `0x003B5EA0` (+51) |
| `d:/dev/CP9/shared-ps4/src/corelib/system/platform/mutex-ps4.cpp` | 51 | - | `0x003F4470`, `0x004E2B50`, `0x00B28EB0`, `0x00B29090` (+47) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/snd_stream/src/common/snd_stream.cpp` | 49 | - | `0x0017F100`, `0x0017F1A0`, `0x0017F290`, `0x0017F300` (+45) |
| `d:/dev/CP9/big2-ps4/src/game/catmull-rom.cpp` | 49 | - | `0x00299450`, `0x00299DD0`, `0x0029A780`, `0x0029C810` (+45) |
| `d:/dev/CP9/shared-ps4/src/orbisanim/source/commands.cpp` | 45 | - | `0x00D39A10`, `0x00D39BD0`, `0x00D39D60`, `0x00D39F10` (+41) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/boomrang2/brb2_synth_interface.cpp` | 44 | - | `0x00150E00`, `0x00151160`, `0x001514B0`, `0x00151820` (+40) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/valloc.cpp` | 42 | - | `0x001791E0`, `0x001793E0`, `0x001795E0`, `0x00179DD0` (+38) |
| `d:/dev/CP9/shared-ps4/src/ndlib/frame-params.cpp` | 41 | - | `0x003FACB0`, `0x004B90D0`, `0x004D10C0`, `0x005704C0` (+37) |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/jointed-body.cpp` | 40 | - | `0x00212F80`, `0x00213780`, `0x0022FBE0`, `0x0029EBC0` (+36) |
| `d:/dev/CP9/big2-ps4/src/game/state-script/ss-animate.cpp` | 39 | - | `0x002E8A30`, `0x002E8BD0`, `0x002E8D30`, `0x002E9060` (+35) |
| `d:/dev/CP9/big2-ps4/src/game/ai/base/ai-util.cpp` | 38 | - | `0x002791C0`, `0x002792C0`, `0x002794B0`, `0x002795A0` (+34) |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/ai/skill/archetype.h` | 37 | - | `0x002168D0`, `0x00236F40`, `0x0023FA90`, `0x00243E40` (+33) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/containers/hashtable.h` | 35 | - | `0x003C8AE0`, `0x003E2AC0`, `0x003EF040`, `0x00481E20` (+31) |
| `d:/dev/CP9/big2-ps4/src/game/player/player-weapon.cpp` | 35 | - | `0x0094F3A0`, `0x009541F0`, `0x009545D0`, `0x009546C0` (+31) |
| `d:/dev/CP9/big2-ps4/src/game/process-game-object.cpp` | 35 | - | `0x00236F40`, `0x00289650`, `0x004560F0`, `0x004585C0` (+31) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ndgi/ndgi-orbis.cpp` | 35 | - | `0x00BEB9A0`, `0x00C05F70`, `0x00C0FAA0`, `0x00C117E0` (+31) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/sndhand.cpp` | 33 | - | `0x001757C0`, `0x00175CA0`, `0x00175E90`, `0x00176350` (+29) |
| `d:/dev/CP9/big2-ps4/src/game/ai/component/shooting-logic.cpp` | 33 | - | `0x0029F5C0`, `0x0029F6E0`, `0x0029F960`, `0x002A0660` (+29) |
| `d:/dev/CP9/big2-ps4/src/game/vehicle/rail-vehicle.cpp` | 33 | - | `0x000013D0`, `0x00001460`, `0x00001610`, `0x00001DD0` (+29) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/boomrang2/snd_synth_interface.cpp` | 31 | - | `0x00158FE0`, `0x00159900`, `0x00159A50`, `0x00159E60` (+27) |
| `d:/dev/CP9/big2-ps4/src/game/attach-system.cpp` | 31 | - | `0x0021C5E0`, `0x0021EDA0`, `0x00223470`, `0x002249F0` (+27) |
| `../../Common/Internal/GeometryProcessing/ConvexHull/hkgpConvexHullInternals.h` | 29 | - | `0x001975F0`, `0x00198040`, `0x00198140`, `0x00198230` (+25) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/util/bigsort.h` | 29 | - | `0x001FC080`, `0x001FF730`, `0x00200590`, `0x00213BC0` (+25) |
| `d:/dev/CP9/big2-ps4/src/game/net/net-event.cpp` | 25 | - | `0x0060C990`, `0x0060CDE0`, `0x0060D290`, `0x0060FF80` (+21) |
| `d:/dev/CP9/big2-ps4/src/game/weapon/projectile-process.cpp` | 25 | - | `0x00AF0970`, `0x00AF0AA0`, `0x00AF1920`, `0x00AFD370` (+21) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ndgi/ndgi-orbis-device-context.cpp` | 25 | - | `0x00BF76C0`, `0x00BFAFF0`, `0x00BFBF20`, `0x00BFC120` (+21) |
| `d:/dev/CP9/big2-ps4/src/game/anim/anim-action.cpp` | 24 | - | `0x002C1C80`, `0x002C2010`, `0x002C2150`, `0x002C9DB0` (+20) |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/player/commonactions.h` | 22 | - | `0x00684700`, `0x00684A50`, `0x00688B70`, `0x006B0490` (+18) |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/rigid-body.cpp` | 19 | - | `0x0027A4C0`, `0x00528D90`, `0x00528F80`, `0x00529330` (+15) |
| `d:/dev/CP9/shared-ps4/src/corelib/system/platform/condition-variable-ps4.cpp` | 19 | - | `0x00B28EB0`, `0x00B38220`, `0x00B38330`, `0x00B383A0` (+15) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/loader.cpp` | 18 | - | `0x00172D20`, `0x00172E90`, `0x00172F80`, `0x00172FF0` (+14) |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/player/terrain.h` | 17 | - | `0x00689450`, `0x00689C20`, `0x00689E60`, `0x0068A1F0` (+13) |
| `d:/dev/CP9/shared-ps4/src/ndlib/level/game-loading.cpp` | 17 | - | `0x003F6680`, `0x003F68C0`, `0x00B744F0`, `0x00B74770` (+13) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ingame-shaders.cpp` | 17 | - | `0x00B922E0`, `0x00BDF9B0`, `0x00BE2120`, `0x00BE2230` (+13) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/interface/texture-mgr-base.cpp` | 17 | - | `0x00BE9C00`, `0x00BE9EB0`, `0x00BEA040`, `0x00BEA6D0` (+13) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/interface/fg-instance.cpp` | 16 | - | `0x00BE48D0`, `0x00BE4F90`, `0x00BE5260`, `0x00BE5320` (+12) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/containers/list-array.h` | 15 | - | `0x0056AEF0`, `0x0056C0E0`, `0x0056D170`, `0x0056D430` (+11) |
| `d:/dev/CP9/big2-ps4/src/game/ai/topology/nav-blocker-mgr.cpp` | 15 | - | `0x002308A0`, `0x00236F40`, `0x0036AB60`, `0x0036AF60` (+11) |
| `d:/dev/CP9/big2-ps4/src/game/process/process.cpp` | 15 | - | `0x001FE420`, `0x001FE510`, `0x001FE7E0`, `0x001FE8C0` (+11) |
| `d:/dev/CP9/big2-ps4/src/game/catmull-rom-manager.cpp` | 14 | - | `0x00464230`, `0x004643E0`, `0x00464490`, `0x00464540` (+10) |
| `d:/dev/CP9/big2-ps4/src/game/script-funcs.cpp` | 14 | - | `0x009FD130`, `0x009FDF30`, `0x00A002D0`, `0x00A01BB0` (+10) |
| `d:/dev/CP9/big2-ps4/src/game/script-manager.cpp` | 14 | - | `0x003F68C0`, `0x00A138E0`, `0x00A14120`, `0x00A14F20` (+10) |
| `d:/dev/CP9/shared-ps4/src/corelib/system/platform/thread-ps4.cpp` | 14 | - | `0x00B271E0`, `0x00B2A2B0`, `0x00B2A3F0`, `0x00B38AE0` (+10) |
| `d:/dev/CP9/shared-ps4/src/ndlib/save/ndsavedata.cpp` | 14 | - | `0x005ACAB0`, `0x005ACB60`, `0x005AE480`, `0x005C8840` (+10) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/ccsound.cpp` | 13 | - | `0x00162F20`, `0x001637D0`, `0x00163920`, `0x00163C40` (+9) |
| `d:/dev/CP9/big2-ps4/src/game/ai/topology/nav-mesh.cpp` | 13 | - | `0x0027BC90`, `0x00300A90`, `0x00320BF0`, `0x003285F0` (+9) |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/wind.cpp` | 13 | - | `0x00537D90`, `0x00537FA0`, `0x00538380`, `0x00538660` (+9) |
| `d:/dev/CP9/big2-ps4/src/game/menu/menuhost.cpp` | 13 | - | `0x005C7BB0`, `0x005C8840`, `0x005CC610`, `0x005D1BF0` (+9) |
| `d:/dev/CP9/big2-ps4/src/game/player/cover-edge.cpp` | 13 | - | `0x0068A0F0`, `0x0068A500`, `0x0068C170`, `0x00770840` (+9) |
| `d:/dev/CP9/big2-ps4/src/game/vehicle/rail-vehicle-manager.cpp` | 13 | - | `0x00A8E380`, `0x00A8EC00`, `0x00A8FCE0`, `0x00A8FD50` (+9) |
| `d:/dev/CP9/shared-ps4/src/ndlib/io/package-mgr.cpp` | 13 | - | `0x0056AEF0`, `0x00B65F40`, `0x00B672D0`, `0x00B67F80` (+9) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/background.cpp` | 13 | - | `0x0056F300`, `0x00B8F4B0`, `0x00B8F620`, `0x00B8F720` (+9) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/containers/page-table.h` | 12 | - | `0x00AC72D0`, `0x00B95B60`, `0x00C29E70`, `0x00C348D0` (+8) |
| `d:/dev/CP9/big2-ps4/src/game/net/net-player-data.cpp` | 12 | - | `0x005AB090`, `0x005AB2A0`, `0x005AB380`, `0x0064EB30` (+8) |
| `d:/dev/CP9/big2-ps4/src/game/weapon/processweapon.cpp` | 12 | - | `0x00ADA470`, `0x00ADA5E0`, `0x00ADA8B0`, `0x00ADB050` (+8) |
| `d:/dev/CP9/shared-ps4/src/ndlib/save/savebuffer.cpp` | 12 | - | `0x009EB390`, `0x00CD0CC0`, `0x00CD0E10`, `0x00CD0E80` (+8) |
| `D:/depot/Release/2011_3/Custom/2011_3_ND_Toaster/Source/Common/Internal/GeometryProcessing/Mesh/hkgpMesh.cpp` | 11 | - | `0x001A01C0`, `0x001A8290`, `0x001A88C0`, `0x001A99E0` (+7) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/grains.cpp` | 11 | - | `0x00169F00`, `0x0016AC60`, `0x0016B900`, `0x0016B9E0` (+7) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/ngen/geo-group.h` | 11 | - | `0x0056F300`, `0x00B8F620`, `0x00B908B0`, `0x00BE2FB0` (+7) |
| `d:/dev/CP9/big2-ps4/src/game/ai/agent/npc.cpp` | 11 | - | `0x00236F40`, `0x0023E530`, `0x0023E640`, `0x0023E730` (+7) |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/havok-alloc.cpp` | 11 | - | `0x004EB980`, `0x004EBF40`, `0x004EC310`, `0x004ED290` (+7) |
| `d:/dev/CP9/big2-ps4/src/game/level.cpp` | 11 | - | `0x002077F0`, `0x0020EA60`, `0x0020F9C0`, `0x0056EA40` (+7) |
| `d:/dev/CP9/big2-ps4/src/game/player/player-fight-npc-moves.cpp` | 11 | - | `0x00831E30`, `0x00832920`, `0x00833430`, `0x00833750` (+7) |
| `d:/dev/CP9/big2-ps4/src/game/targetable.cpp` | 11 | - | `0x00236F40`, `0x00289650`, `0x003EF6C0`, `0x003F14A0` (+7) |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/memory-new-malloc-ps4.cpp` | 11 | - | `0x00B32AA0`, `0x00B32AE0`, `0x00B32B20`, `0x00B32B60` (+7) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/interface/material-table.cpp` | 11 | - | `0x0056F300`, `0x005B4E90`, `0x005BBD90`, `0x00BE78D0` (+7) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/memory/heaparena.h` | 10 | - | `0x006700C0`, `0x009B5C20`, `0x009B9530`, `0x009BDB90` (+6) |
| `d:/dev/CP9/big2-ps4/src/game/anim/anim-table.cpp` | 10 | - | `0x0025BA10`, `0x002926B0`, `0x003A52C0`, `0x003A5E70` (+6) |
| `d:/dev/CP9/big2-ps4/src/game/character-manager.cpp` | 10 | - | `0x00251CF0`, `0x0027ACB0`, `0x002DAAA0`, `0x003650E0` (+6) |
| `d:/dev/CP9/big2-ps4/src/game/train/train.cpp` | 10 | - | `0x00A69CF0`, `0x00A6B3C0`, `0x00A6CBC0`, `0x00A6CCF0` (+6) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/interface/texture-mgr.cpp` | 10 | - | `0x003FACB0`, `0x00BECF50`, `0x00BED600`, `0x00BEDD20` (+6) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/boomrang2/api_brb2.cpp` | 9 | - | `0x0014E6B0`, `0x0014F1F0`, `0x0014F260`, `0x0014F2E0` (+5) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/background.h` | 9 | - | `0x0056EC70`, `0x0056F300`, `0x00570390`, `0x005704C0` (+5) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/particle/particle-engine.h` | 9 | - | `0x00C5D1E0`, `0x00C65220`, `0x00C65580`, `0x00C65690` (+5) |
| `d:/dev/CP9/big2-ps4/src/game/anim/anim-util.cpp` | 9 | - | `0x003A7360`, `0x003A7570`, `0x003A8730`, `0x003A88F0` (+5) |
| `d:/dev/CP9/big2-ps4/src/game/net/net-game-manager.cpp` | 9 | - | `0x006400D0`, `0x00640FC0`, `0x00642370`, `0x00642840` (+5) |
| `d:/dev/CP9/big2-ps4/src/game/player/cedgedb.cpp` | 9 | - | `0x00681130`, `0x006811C0`, `0x00681540`, `0x00681A60` (+5) |
| `d:/dev/CP9/big2-ps4/src/game/vehicle/rail-vehicle-behavior.cpp` | 9 | - | `0x00A7D830`, `0x00A8DE50`, `0x00A8DEB0`, `0x00A8DF40` (+5) |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/memory-area-ps4.cpp` | 9 | - | `0x00B33F80`, `0x00B34AB0`, `0x00B40950`, `0x00B40A40` (+5) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/dynamic-buffer.cpp` | 9 | - | `0x00BF5A30`, `0x00BF65D0`, `0x00C28180`, `0x00C282E0` (+5) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/material.cpp` | 9 | - | `0x00C2CFD0`, `0x00C2D4A0`, `0x00C2F890`, `0x00C300A0` (+5) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/snd_stream/src/common/snd_stream_threading.cpp` | 8 | - | `0x0018C950`, `0x0018C9A0`, `0x0018CE80`, `0x0018D0A0` (+4) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/memory/relocatable-heap.h` | 8 | - | `0x0067B710`, `0x00A00140`, `0x00C5FBA0`, `0x00C6C9B0` (+4) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/interface/bg-geometry.h` | 8 | - | `0x00BC4C60`, `0x00BC4F40`, `0x00BC50A0`, `0x00BC57B0` (+4) |
| `d:/dev/CP9/big2-ps4/src/game/camera/camerazoom.cpp` | 8 | - | `0x0044BF00`, `0x0044BFE0`, `0x0044C7F0`, `0x0044F180` (+4) |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/breakable.cpp` | 8 | - | `0x004D4D60`, `0x004D5290`, `0x004D65E0`, `0x004D6840` (+4) |
| `d:/dev/CP9/big2-ps4/src/game/player/player-fight-combo-moves.cpp` | 8 | - | `0x008040A0`, `0x00806620`, `0x00808A10`, `0x0080A850` (+4) |
| `d:/dev/CP9/big2-ps4/src/game/vehicle/helicopter.cpp` | 8 | - | `0x00A81DC0`, `0x00A821B0`, `0x00A84730`, `0x00A855E0` (+4) |
| `d:/dev/CP9/shared-ps4/src/ndlib/io/package.cpp` | 8 | - | `0x00B68D50`, `0x00B6D300`, `0x00B6EAE0`, `0x00B6ECA0` (+4) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/lighting/post-lut.cpp` | 8 | - | `0x00BF7430`, `0x00BF79E0`, `0x00BF8390`, `0x00BF8AE0` (+4) |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/ai/component/target-appraiser.h` | 7 | - | `0x002AE5C0`, `0x002AE7E0`, `0x002AEDB0`, `0x002AFA60` (+3) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/memory/memory.h` | 7 | - | `0x00B2FEA0`, `0x00B305B0`, `0x00B800A0`, `0x00C18970` (+3) |
| `d:/dev/CP9/big2-ps4/src/game/ai/agent/navigating-character.cpp` | 7 | - | `0x00219540`, `0x002196F0`, `0x002197C0`, `0x00219CB0` (+3) |
| `d:/dev/CP9/big2-ps4/src/game/ai/agent/npc-knowledge.cpp` | 7 | - | `0x00226D90`, `0x002274D0`, `0x0022CE40`, `0x0022CEA0` (+3) |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/havok-collision-cast.cpp` | 7 | - | `0x004F03D0`, `0x004F0880`, `0x004F1DC0`, `0x004F2130` (+3) |
| `d:/dev/CP9/big2-ps4/src/game/level-mgr.cpp` | 7 | - | `0x00568090`, `0x00568460`, `0x00569130`, `0x0056A0C0` (+3) |
| `d:/dev/CP9/big2-ps4/src/game/player/player-weapon-gets.cpp` | 7 | - | `0x0094E800`, `0x0094EB50`, `0x0094ED20`, `0x0094F080` (+3) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/cubemap-set.cpp` | 7 | - | `0x003FBBC0`, `0x00B6D010`, `0x00B91880`, `0x00B91F10` (+3) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/json-dom.cpp` | 7 | - | `0x00B92990`, `0x00B97380`, `0x00BDF9B0`, `0x00C2BCF0` (+3) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-renderer.cpp` | 7 | - | `0x00C580D0`, `0x00C5A430`, `0x00C5A760`, `0x00C5A840` (+3) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-setup.cpp` | 7 | - | `0x00D06BE0`, `0x00D08C10`, `0x00D08C80`, `0x00D096F0` (+3) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/texture-dictionary-manager.cpp` | 7 | - | `0x00BF0590`, `0x00BF19F0`, `0x00CBD910`, `0x00CBE0D0` (+3) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/boomrang2/../../common/boomrang2/voice_context.h` | 6 | - | `0x00159E60`, `0x0015BD00`, `0x0015BD80`, `0x0015C3B0` (+2) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/ndgi/ndgi-orbis-mem-allocator.h` | 6 | - | `0x00C08D90`, `0x00C0BF80`, `0x00C17E40`, `0x00C18970` (+2) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/post/dynamic-stream-mgr.h` | 6 | - | `0x00BDBB10`, `0x00BDC190`, `0x00BDC670`, `0x00CC2690` (+2) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/util/bit-reader.h` | 6 | - | `0x0048D920`, `0x004BC510`, `0x004BC5F0`, `0x00A19A00` (+2) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/util/splash.h` | 6 | - | `0x00C0A6D0`, `0x00C26100`, `0x00C4BE60`, `0x00C4C5F0` (+2) |
| `d:/dev/CP9/big2-ps4/src/game/anim/anim-state-layer.cpp` | 6 | - | `0x0039DB80`, `0x0039DCB0`, `0x003A0250`, `0x003A16C0` (+2) |
| `d:/dev/CP9/big2-ps4/src/game/anim/anim-state.cpp` | 6 | - | `0x003A2460`, `0x003A2610`, `0x003A26F0`, `0x003A2E30` (+2) |
| `d:/dev/CP9/big2-ps4/src/game/cutscene/cutscene.cpp` | 6 | - | `0x00484B30`, `0x00484D60`, `0x004851A0`, `0x00488BA0` (+2) |
| `d:/dev/CP9/big2-ps4/src/game/hud/hud.cpp` | 6 | - | `0x00552C70`, `0x00554DF0`, `0x00554F70`, `0x00554FD0` (+2) |
| `d:/dev/CP9/big2-ps4/src/game/player/jumps/player-jump-landings.cpp` | 6 | - | `0x006F5510`, `0x006F6460`, `0x006F76D0`, `0x006F80C0` (+2) |
| `d:/dev/CP9/big2-ps4/src/game/player/player-fast-grenade.cpp` | 6 | - | `0x007FE9B0`, `0x007FF540`, `0x008008E0`, `0x00800B60` (+2) |
| `d:/dev/CP9/big2-ps4/src/game/player/player-sync-action-controller.cpp` | 6 | - | `0x0092A300`, `0x0092AE50`, `0x0092BC20`, `0x0092CCF0` (+2) |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/memory-ps4.cpp` | 6 | - | `0x005756A0`, `0x00B33C20`, `0x00B33D10`, `0x00B33E90` (+2) |
| `d:/dev/CP9/shared-ps4/src/ndlib/anim/anim-mgr.cpp` | 6 | - | `0x00B500C0`, `0x00B50740`, `0x00B50B80`, `0x00B50FF0` (+2) |
| `d:/dev/CP9/shared-ps4/src/ndlib/movieplayer/movie-player.cpp` | 6 | - | `0x00B82A10`, `0x00B84480`, `0x00B84760`, `0x00B84DD0` (+2) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ndgi/cached-device-context.cpp` | 6 | - | `0x00BFBF20`, `0x00BFC120`, `0x00BFC320`, `0x00BFC4B0` (+2) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/object-cache.cpp` | 6 | - | `0x00C4BE60`, `0x00C4C5F0`, `0x00C4CC60`, `0x00C4D220` (+2) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/view-frustum.cpp` | 6 | - | `0x00CCCB30`, `0x00CCCC90`, `0x00CCD080`, `0x00CCD3D0` (+2) |
| `d:/dev/CP9/shared-ps4/src/ndlib/save/savegui.cpp` | 6 | - | `0x00D167B0`, `0x00D16A10`, `0x00D16C00`, `0x00D16E50` (+2) |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Physics/Utilities/Constraint/Bilateral/hkpConstraintUtils.cpp` | 5 | - | `0x00129860`, `0x00129F90`, `0x0012A0B0`, `0x0012A1D0` (+1) |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/snd_stream/src/common/snd_stream_scream.cpp` | 5 | - | `0x00189390`, `0x001898A0`, `0x0018B790`, `0x0018B920` (+1) |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/containers/sorted-index-array.h` | 5 | - | `0x00C061A0`, `0x00C06A10`, `0x00C06BA0`, `0x00C08D90` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/drawscript.cpp` | 5 | - | `0x003FACB0`, `0x004B7C20`, `0x004B8E50`, `0x004B90D0` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/composite-body.cpp` | 5 | - | `0x004E3AC0`, `0x004E3C00`, `0x004E5700`, `0x004E5A50` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/group-script-funcs.cpp` | 5 | - | `0x00549200`, `0x00549BF0`, `0x0054A760`, `0x0054AFD0` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/player/player-base.cpp` | 5 | - | `0x00720B30`, `0x00722B40`, `0x00724F20`, `0x00726330` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/player/player-move.cpp` | 5 | - | `0x008C9280`, `0x008C9D30`, `0x008CBCC0`, `0x008D4190` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/player/player-script-funcs.cpp` | 5 | - | `0x0090D1F0`, `0x0090D350`, `0x0090D4B0`, `0x0090D4F0` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/render/decal.cpp` | 5 | - | `0x009DFFC0`, `0x009E02B0`, `0x009E0950`, `0x00AEC670` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/script-module.cpp` | 5 | - | `0x00A19B20`, `0x00A19CE0`, `0x00A1B4D0`, `0x00A1B780` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/spline-tracker.cpp` | 5 | - | `0x00A23220`, `0x00A238C0`, `0x00A24140`, `0x00A243C0` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/state-script/ss-manager.cpp` | 5 | - | `0x00A32BE0`, `0x00A34FB0`, `0x00A36D80`, `0x00A370E0` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/unlockablemanager.cpp` | 5 | - | `0x00975190`, `0x00A75450`, `0x00A76320`, `0x00A76460` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/vehicle/tank.cpp` | 5 | - | `0x00AA65D0`, `0x00AAAA40`, `0x00AB1530`, `0x00AB2D60` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/weapon/projectile-bolt.cpp` | 5 | - | `0x00AF0CA0`, `0x00AF1920`, `0x00AF32E0`, `0x00AF35A0` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/weapon/projectile-grenade.cpp` | 5 | - | `0x00AFC7C0`, `0x00AFD400`, `0x00AFDF90`, `0x00AFF310` (+1) |
| `d:/dev/CP9/big2-ps4/src/game/weapon/turret.cpp` | 5 | - | `0x00B1C2D0`, `0x00B1DF00`, `0x00B1E110`, `0x00B1EDC0` (+1) |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/allocator-pool.cpp` | 5 | - | `0x00B42AA0`, `0x00B45580`, `0x00B49B00`, `0x00BD6EA0` (+1) |
| `d:/dev/CP9/shared-ps4/src/corelib/util/msg-mem.cpp` | 5 | - | `0x00B39E80`, `0x00B39FB0`, `0x00B3A420`, `0x00B3A540` (+1) |
| `d:/dev/CP9/shared-ps4/src/ndlib/io/streaming-state.cpp` | 5 | - | `0x003FBBC0`, `0x00B6D010`, `0x00B701E0`, `0x00B88580` (+1) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/fg-draw-mgr.cpp` | 5 | - | `0x00BD3BC0`, `0x00BD3FE0`, `0x00BD4260`, `0x00BD4F80` (+1) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/interface/texture-streamer.cpp` | 5 | - | `0x00BEF890`, `0x00BF0590`, `0x00BF37C0`, `0x00BF5150` (+1) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/mesh-gpu-inst.cpp` | 5 | - | `0x00C34280`, `0x00C34550`, `0x00C346E0`, `0x00C348D0` (+1) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/vertex-stream.cpp` | 5 | - | `0x00C4E180`, `0x00C4E420`, `0x00C4E5F0`, `0x00C4E890` (+1) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/particle-root-data.cpp` | 5 | - | `0x00C5FBA0`, `0x00C6CA60`, `0x00C6E830`, `0x00C6FF70` (+1) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/particle.cpp` | 5 | - | `0x00C6E830`, `0x00C6FF70`, `0x00C746A0`, `0x00C77750` (+1) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/scene-view.cpp` | 5 | - | `0x00C901C0`, `0x00CB61D0`, `0x00CB6240`, `0x00CB6520` (+1) |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/util/screen-shot.cpp` | 5 | - | `0x00CC5700`, `0x00CC59F0`, `0x00CC6270`, `0x00CC6450` (+1) |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Common/Base/Thread/Semaphore/Posix/hkPosixSemaphore.cpp` | 4 | - | `0x001261B0`, `0x001262D0`, `0x00126360`, `0x00126470` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Common/Serialize/ResourceDatabase/hkResourceHandle.cpp` | 4 | - | `0x000EF5B0`, `0x000EF900`, `0x000F0180`, `0x000F07F0` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/989snd.cpp` | 4 | - | `0x00142750`, `0x00142820`, `0x00142900`, `0x001429A0` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/distancemodel.cpp` | 4 | - | `0x00166D50`, `0x00166EE0`, `0x00166FC0`, `0x001673B0` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/groupmixer.cpp` | 4 | - | `0x00170800`, `0x00170F30`, `0x001712E0`, `0x00171400` |
| `d:/dev/CP9/big2-ps4/src/game/actor-viewer.cpp` | 4 | - | `0x001FC080`, `0x001FF070`, `0x001FF730`, `0x00200590` |
| `d:/dev/CP9/big2-ps4/src/game/anim/anim-state-instance.cpp` | 4 | - | `0x0039BC60`, `0x0039BD90`, `0x0039C180`, `0x0039C2B0` |
| `d:/dev/CP9/big2-ps4/src/game/audio/arm.cpp` | 4 | - | `0x003B5020`, `0x003B64E0`, `0x003C80D0`, `0x003C8A60` |
| `d:/dev/CP9/big2-ps4/src/game/camera/camerafollow2.cpp` | 4 | - | `0x00423D10`, `0x004270E0`, `0x0042D940`, `0x0042F150` |
| `d:/dev/CP9/big2-ps4/src/game/camera/cameramanager.cpp` | 4 | - | `0x00434000`, `0x00436240`, `0x009E5770`, `0x00C6FF70` |
| `d:/dev/CP9/big2-ps4/src/game/camera/tracking-spline.cpp` | 4 | - | `0x0040FD10`, `0x00419C30`, `0x00452C50`, `0x00455C20` |
| `d:/dev/CP9/big2-ps4/src/game/character-leg-ik.cpp` | 4 | - | `0x0046CB60`, `0x0046D520`, `0x0046EC80`, `0x00471090` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/debugdraw/havok-debug-draw.cpp` | 4 | - | `0x004E9710`, `0x004E9B20`, `0x004EA020`, `0x004EA720` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/havok-data.cpp` | 4 | - | `0x004F70C0`, `0x004F74B0`, `0x004F7FD0`, `0x004F8410` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-aim.cpp` | 4 | - | `0x00717D80`, `0x00717FF0`, `0x00718B20`, `0x00718F40` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-cover.cpp` | 4 | - | `0x00786E10`, `0x0078F870`, `0x00793570`, `0x0079A6B0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-edge.cpp` | 4 | - | `0x007BF6F0`, `0x007C0720`, `0x007C50F0`, `0x007C8110` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-script.cpp` | 4 | - | `0x009107B0`, `0x00910900`, `0x00910D90`, `0x009122B0` |
| `d:/dev/CP9/big2-ps4/src/game/player/terrain.cpp` | 4 | - | `0x00689A30`, `0x0097DC50`, `0x0097EC30`, `0x00981080` |
| `d:/dev/CP9/big2-ps4/src/game/processlist.cpp` | 4 | - | `0x009B0A30`, `0x009B0D70`, `0x009B0EB0`, `0x00A371B0` |
| `d:/dev/CP9/big2-ps4/src/game/spu/spumanager.cpp` | 4 | - | `0x003F4470`, `0x00A26300`, `0x00A26380`, `0x00AC0820` |
| `d:/dev/CP9/big2-ps4/src/game/state-script/ss-instance.cpp` | 4 | - | `0x00A2E7B0`, `0x00A2ECC0`, `0x00A2F550`, `0x00A30A20` |
| `d:/dev/CP9/big2-ps4/src/game/state-script/ss-menu.cpp` | 4 | - | `0x00A3B2D0`, `0x00A3C7C0`, `0x00A3E2A0`, `0x00A3E520` |
| `d:/dev/CP9/big2-ps4/src/game/tasks/task.cpp` | 4 | - | `0x00A5AA00`, `0x00A5B430`, `0x00A5C320`, `0x00A5F970` |
| `d:/dev/CP9/big2-ps4/src/game/vehicle/vehicle-spline-tracker.cpp` | 4 | - | `0x00AB9FE0`, `0x00ABA310`, `0x00ABA7B0`, `0x00ABA840` |
| `d:/dev/CP9/big2-ps4/src/game/water/watersplash.cpp` | 4 | - | `0x00AC7AD0`, `0x00AC8480`, `0x00AC90C0`, `0x00AC9B70` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/projectile-base.cpp` | 4 | - | `0x00AEBB70`, `0x00AEC670`, `0x00AEDCD0`, `0x00AEEFA0` |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/bigmalloc.cpp` | 4 | - | `0x00B30830`, `0x00B30AA0`, `0x00B31BB0`, `0x00D73230` |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/memory-linearizer.cpp` | 4 | - | `0x00B410B0`, `0x00B412E0`, `0x00B41680`, `0x00B84E80` |
| `d:/dev/CP9/shared-ps4/src/ndlib/io/package-util.cpp` | 4 | - | `0x00B68D50`, `0x00B6DE20`, `0x00B6DE90`, `0x00B6E800` |
| `d:/dev/CP9/shared-ps4/src/ndlib/lights/dynamic-light-game-interface.cpp` | 4 | - | `0x00B79420`, `0x00B7A380`, `0x00B7B080`, `0x00C8EB50` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/meshraycaster.cpp` | 4 | - | `0x00C45060`, `0x00C460D0`, `0x00C47790`, `0x00C47FB0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-cull.cpp` | 4 | - | `0x00D0C0F0`, `0x00D1FA00`, `0x00D1FB50`, `0x00D22190` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/scene-rendering.cpp` | 4 | - | `0x00CAD160`, `0x00CAEFD0`, `0x00CB0E10`, `0x00CB2E90` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/shading/parameter-lookup.cpp` | 4 | - | `0x00C26100`, `0x00C2D4A0`, `0x00CB72F0`, `0x00CB7760` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/window-context.cpp` | 4 | - | `0x00CC79A0`, `0x00CC7CC0`, `0x00CCF830`, `0x00CCFBF0` |
| `../../Common/Internal/GeometryProcessing/Triangulator/hkgpTriangulator.inl` | 3 | - | `0x00DD6540`, `0x00DD6FD0`, `0x00DD8390` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.cpp` | 3 | - | `0x000D8FC0`, `0x000D9060`, `0x000D9100` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Physics/Collide/Util/ShapeShrinker/hkpShapeShrinker.cpp` | 3 | - | `0x001C6710`, `0x001C7780`, `0x001C7F90` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/bussconfiguration.cpp` | 3 | - | `0x00161F30`, `0x00162210`, `0x00162380` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/globalvars.cpp` | 3 | - | `0x001688D0`, `0x00168A80`, `0x00169060` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/ai/component/prop-inventory.h` | 3 | - | `0x0029D9E0`, `0x0029E100`, `0x0029EBC0` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/system/platform/thread.h` | 3 | - | `0x00B271E0`, `0x00B83D20`, `0x00B893E0` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/ngen/mesh.h` | 3 | - | `0x00BC4C60`, `0x00BE6750`, `0x00C47790` |
| `d:/dev/CP9/big2-ps4/src/game/ai/component/scripted-animation.cpp` | 3 | - | `0x002256C0`, `0x0029F2F0`, `0x0029F4A0` |
| `d:/dev/CP9/big2-ps4/src/game/ai/component/target-appraiser.cpp` | 3 | - | `0x002AE3C0`, `0x002AE7E0`, `0x002B0660` |
| `d:/dev/CP9/big2-ps4/src/game/ai/controller/cinematic-controller.cpp` | 3 | - | `0x002B7480`, `0x002B79C0`, `0x002B7E40` |
| `d:/dev/CP9/big2-ps4/src/game/ai/controller/cover-controller.cpp` | 3 | - | `0x002BB710`, `0x002BC5C0`, `0x002BD900` |
| `d:/dev/CP9/big2-ps4/src/game/ai/controller/script-ap-controller.cpp` | 3 | - | `0x002E6E50`, `0x002E70E0`, `0x002E7790` |
| `d:/dev/CP9/big2-ps4/src/game/ai/controller/script-controller.cpp` | 3 | - | `0x002E8A30`, `0x002E8BD0`, `0x002E8D30` |
| `d:/dev/CP9/big2-ps4/src/game/ai/skill/hunting-skill.cpp` | 3 | - | `0x003222F0`, `0x00324DA0`, `0x003256E0` |
| `d:/dev/CP9/big2-ps4/src/game/audio/audio-control.cpp` | 3 | - | `0x003C9A50`, `0x003CA870`, `0x00B113A0` |
| `d:/dev/CP9/big2-ps4/src/game/big.cpp` | 3 | - | `0x003F2700`, `0x003F5CE0`, `0x003F68C0` |
| `d:/dev/CP9/big2-ps4/src/game/dmenu.cpp` | 3 | - | `0x004AC400`, `0x004AC590`, `0x004AD250` |
| `d:/dev/CP9/big2-ps4/src/game/effectcontrol.cpp` | 3 | - | `0x00488BA0`, `0x004BD2E0`, `0x009950D0` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/broadphase.cpp` | 3 | - | `0x004DDF70`, `0x004DE460`, `0x004DE9D0` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/buoyancy.cpp` | 3 | - | `0x004E12F0`, `0x004E18C0`, `0x004E1A50` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/havok-contact-callback-queue-util.cpp` | 3 | - | `0x004F5580`, `0x004F57E0`, `0x004F5A20` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/havok-wws-cast.cpp` | 3 | - | `0x0050B160`, `0x0050B5F0`, `0x0050BBF0` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/rigid-body-base.cpp` | 3 | - | `0x00236F40`, `0x004E5D40`, `0x00527340` |
| `d:/dev/CP9/big2-ps4/src/game/platform-control.cpp` | 3 | - | `0x0067CBE0`, `0x0067DA10`, `0x0067E020` |
| `d:/dev/CP9/big2-ps4/src/game/player/flashlight.cpp` | 3 | - | `0x006995F0`, `0x0069A190`, `0x007EDD80` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-aiming-machine3.cpp` | 3 | - | `0x00719D80`, `0x0071A1B0`, `0x0071BC20` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-death.cpp` | 3 | - | `0x0079F750`, `0x007A1C70`, `0x007A2A70` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-fight-lazarevic.cpp` | 3 | - | `0x0081AEF0`, `0x00821D40`, `0x00825D50` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-fire.cpp` | 3 | - | `0x008715F0`, `0x008719B0`, `0x008722D0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-move-mode.cpp` | 3 | - | `0x008B16B0`, `0x008B1D20`, `0x008B22E0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-targeting.cpp` | 3 | - | `0x00933040`, `0x00933B00`, `0x00935B20` |
| `d:/dev/CP9/big2-ps4/src/game/player/player.cpp` | 3 | - | `0x007EDD80`, `0x00974EA0`, `0x00974F00` |
| `d:/dev/CP9/big2-ps4/src/game/player/playerstatmanager.cpp` | 3 | - | `0x0097BAC0`, `0x0097BBA0`, `0x009EB390` |
| `d:/dev/CP9/big2-ps4/src/game/process-attachable.cpp` | 3 | - | `0x009846B0`, `0x00987840`, `0x009881C0` |
| `d:/dev/CP9/big2-ps4/src/game/process-explosion.cpp` | 3 | - | `0x0098AFE0`, `0x0098C800`, `0x0098FC40` |
| `d:/dev/CP9/big2-ps4/src/game/process-phys-rope.cpp` | 3 | - | `0x009A2FD0`, `0x009A34E0`, `0x009A44E0` |
| `d:/dev/CP9/big2-ps4/src/game/process/process-mgr.cpp` | 3 | - | `0x009B8CD0`, `0x009B9530`, `0x009BD660` |
| `d:/dev/CP9/big2-ps4/src/game/settings.cpp` | 3 | - | `0x00A1C710`, `0x00A1CD50`, `0x00A1D600` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/projectile-bullet.cpp` | 3 | - | `0x00AF8E80`, `0x00AF9120`, `0x00AF9890` |
| `d:/dev/CP9/shared-ps4/src/corelib/job/job-scheduling.cpp` | 3 | - | `0x00B3BBF0`, `0x00B3C2C0`, `0x00B3E4C0` |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/allocator-heap.cpp` | 3 | - | `0x00B2F280`, `0x00B2F960`, `0x00B300E0` |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/scoped-temp-allocator.cpp` | 3 | - | `0x00B371A0`, `0x00B377B0`, `0x00B37B70` |
| `d:/dev/CP9/shared-ps4/src/ndlib/audio/virtual-bank.cpp` | 3 | - | `0x00B5C5D0`, `0x00B5CF50`, `0x00B5E3C0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/movieplayer/movie-player-shared.cpp` | 3 | - | `0x00B80C90`, `0x00B80DB0`, `0x00B854A0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/dev/menu-selectgeo.cpp` | 3 | - | `0x00BC5E60`, `0x00BC64C0`, `0x00BC6B90` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/lighting/lightenv-mgr.cpp` | 3 | - | `0x00D01020`, `0x00D02D70`, `0x00D02F60` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/geo-group.cpp` | 3 | - | `0x00B90BA0`, `0x00C29BA0`, `0x00C29D40` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-copy-for-render.cpp` | 3 | - | `0x00D0B2E0`, `0x00D1F2B0`, `0x00D1F300` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-engine.cpp` | 3 | - | `0x00D060E0`, `0x00D06BE0`, `0x00D07FB0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-preupdate.cpp` | 3 | - | `0x00D096F0`, `0x00D2D6D0`, `0x00D321D0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/post/post-geometry.cpp` | 3 | - | `0x00C7AF20`, `0x00C7B5F0`, `0x00C7C980` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/render-mgr.cpp` | 3 | - | `0x00C8F1A0`, `0x00C901C0`, `0x00C9D300` |
| `d:/dev/CP9/shared-ps4/src/ndlib/script/tlou/script-module.cpp` | 3 | - | `0x00CDA050`, `0x00CDAA20`, `0x00CDAE20` |
| `../../Geometry/Internal/Algorithms/Gsk/hkcdGskImpl.inl` | 2 | - | `0x0003BE50`, `0x0003C4E0` |
| `../../Geometry/Internal/DataStructures/StaticMeshTree/hkcdStaticMeshTree.inl` | 2 | - | `0x00D96620`, `0x00D96980` |
| `../../Physics/Internal/Collide/BvCompressedMesh/hkpBvCompressedMeshShapeInternals.inl` | 2 | - | `0x00D8C4D0`, `0x00D97180` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Animation/Animation/Mapper/hkaSkeletonMapper.cpp` | 2 | - | `0x00021B50`, `0x00021E10` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Common/Base/System/Error/hkError.cpp` | 2 | - | `0x0011E960`, `0x0011EAF0` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Common/Base/System/Io/Socket/Bsd/hkBsdSocket.cpp` | 2 | - | `0x00121EB0`, `0x00122150` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Common/Base/Thread/Thread/Posix/hkPosixThread.cpp` | 2 | - | `0x00126610`, `0x00126670` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Common/GeometryUtilities/Misc/hkGeometryUtils.cpp` | 2 | - | `0x000E7600`, `0x000E8D00` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Physics/Collide/Util/Welding/hkpMeshWeldingUtility.cpp` | 2 | - | `0x000E1020`, `0x000E2A80` |
| `D:/depot/Release/2011_3/Custom/2011_3_ND_Toaster/Source/Physics/Internal/Collide/BvCompressedMesh/hkpBvCompressedMeshShape.cpp` | 2 | - | `0x0003A830`, `0x00D98D30` |
| `D:/depot/Release/2011_3/Custom/2011_3_ND_Toaster/Source/Physics/Internal/Collide/StaticCompound/hkpStaticCompoundShape.cpp` | 2 | - | `0x000323D0`, `0x00034D30` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/boomrang2/ir_pack.cpp` | 2 | - | `0x00156C20`, `0x00157A80` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/sndutils.cpp` | 2 | - | `0x00178AF0`, `0x00178B90` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/snd_stream/src/common/snd_stream_double_list.cpp` | 2 | - | `0x00185AF0`, `0x00185BA0` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/snd_stream/src/common/snd_stream_parsing.cpp` | 2 | - | `0x001863B0`, `0x00186640` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/biglib/process/type-factory.h` | 2 | - | `0x009B6900`, `0x00D48F10` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/biglib/util/stacktree.h` | 2 | - | `0x0056F300`, `0x0067FCD0` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/gamephys/havok-alloc.h` | 2 | - | `0x004EE7D0`, `0x004EEAD0` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/system/platform/condition-variable.h` | 2 | - | `0x00B271E0`, `0x00B83D20` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/system/platform/mutex.h` | 2 | - | `0x00B271E0`, `0x00B83D20` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/memory/smalloc.h` | 2 | - | `0x004EC0E0`, `0x004EFD40` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/net/nptrophymanager.h` | 2 | - | `0x003F68C0`, `0x00A75000` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/enemy-focus.h` | 2 | - | `0x0044C7F0`, `0x0071BC20` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/interface/material-table.h` | 2 | - | `0x005558C0`, `0x00C1C2C0` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/interface/texture-mgr-base.h` | 2 | - | `0x00BECF50`, `0x00BF37C0` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/ndgi/ndgi.h` | 2 | - | `0x00C208D0`, `0x00C20AE0` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/ngen/shader-pool.h` | 2 | - | `0x00C31F50`, `0x00C33300` |
| `d:/dev/CP9/big2-ps4/src/game/ai/action-pack/action-pack-mgr.cpp` | 2 | - | `0x00204440`, `0x00204860` |
| `d:/dev/CP9/big2-ps4/src/game/ai/action-pack/cinematic-action-pack.cpp` | 2 | - | `0x002052D0`, `0x00208720` |
| `d:/dev/CP9/big2-ps4/src/game/ai/agent/body-manager.cpp` | 2 | - | `0x00212DC0`, `0x00212F80` |
| `d:/dev/CP9/big2-ps4/src/game/ai/characters/chloe.cpp` | 2 | - | `0x0027EF20`, `0x0027F170` |
| `d:/dev/CP9/big2-ps4/src/game/ai/characters/flynn.cpp` | 2 | - | `0x00283D60`, `0x00283F60` |
| `d:/dev/CP9/big2-ps4/src/game/ai/characters/lazarevic.cpp` | 2 | - | `0x00286B70`, `0x00287070` |
| `d:/dev/CP9/big2-ps4/src/game/ai/characters/security-guard.cpp` | 2 | - | `0x0028EC30`, `0x0028ED70` |
| `d:/dev/CP9/big2-ps4/src/game/ai/characters/sla-beast.cpp` | 2 | - | `0x00290500`, `0x00290D60` |
| `d:/dev/CP9/big2-ps4/src/game/ai/characters/soldier.cpp` | 2 | - | `0x00292960`, `0x00294030` |
| `d:/dev/CP9/big2-ps4/src/game/ai/component/prop-inventory.cpp` | 2 | - | `0x0029D9E0`, `0x0029EBC0` |
| `d:/dev/CP9/big2-ps4/src/game/ai/component/vision.cpp` | 2 | - | `0x002B2AD0`, `0x002B4140` |
| `d:/dev/CP9/big2-ps4/src/game/ai/controller/animation-controllers.cpp` | 2 | - | `0x00260CC0`, `0x002B6380` |
| `d:/dev/CP9/big2-ps4/src/game/ai/controller/sla-controller.cpp` | 2 | - | `0x002EAD20`, `0x002EAEF0` |
| `d:/dev/CP9/big2-ps4/src/game/ai/controller/traversal-controller.cpp` | 2 | - | `0x002EFCC0`, `0x002F0B50` |
| `d:/dev/CP9/big2-ps4/src/game/ai/npc-speech-controller.cpp` | 2 | - | `0x00215600`, `0x002FC900` |
| `d:/dev/CP9/big2-ps4/src/game/ai/skill/npc-melee-combat-skill.cpp` | 2 | - | `0x0025FC50`, `0x0033D5B0` |
| `d:/dev/CP9/big2-ps4/src/game/ai/skill/sla-open-combat-skill.cpp` | 2 | - | `0x00359300`, `0x0035B770` |
| `d:/dev/CP9/big2-ps4/src/game/ai/weapon/weapon-inventory.cpp` | 2 | - | `0x0038FB10`, `0x0095AF00` |
| `d:/dev/CP9/big2-ps4/src/game/armik.cpp` | 2 | - | `0x003B21D0`, `0x003B3230` |
| `d:/dev/CP9/big2-ps4/src/game/audio/audiofile.cpp` | 2 | - | `0x003CE4A0`, `0x003D16A0` |
| `d:/dev/CP9/big2-ps4/src/game/barrel.cpp` | 2 | - | `0x003EF6C0`, `0x003F12C0` |
| `d:/dev/CP9/big2-ps4/src/game/camera/camerashake.cpp` | 2 | - | `0x00446350`, `0x004472A0` |
| `d:/dev/CP9/big2-ps4/src/game/carry-object.cpp` | 2 | - | `0x00458F00`, `0x0045BCD0` |
| `d:/dev/CP9/big2-ps4/src/game/character-leg-raycaster.cpp` | 2 | - | `0x00471AE0`, `0x00471F90` |
| `d:/dev/CP9/big2-ps4/src/game/drawcontrol.cpp` | 2 | - | `0x004B6E20`, `0x004B73F0` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/jointed-body-penetration.cpp` | 2 | - | `0x005120B0`, `0x00512AF0` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/rigid-body-fse.cpp` | 2 | - | `0x005279C0`, `0x00527B20` |
| `d:/dev/CP9/big2-ps4/src/game/hint.cpp` | 2 | - | `0x0054ED60`, `0x0054FC10` |
| `d:/dev/CP9/big2-ps4/src/game/joypad.cpp` | 2 | - | `0x003FD500`, `0x00567780` |
| `d:/dev/CP9/big2-ps4/src/game/net/net-anim-command-generator.cpp` | 2 | - | `0x005F5650`, `0x005F5770` |
| `d:/dev/CP9/big2-ps4/src/game/net/net-character-animate.cpp` | 2 | - | `0x005F8660`, `0x005F8770` |
| `d:/dev/CP9/big2-ps4/src/game/net/net-character-death.cpp` | 2 | - | `0x005F9490`, `0x005F9900` |
| `d:/dev/CP9/big2-ps4/src/game/net/net-character.cpp` | 2 | - | `0x006086A0`, `0x00608A80` |
| `d:/dev/CP9/big2-ps4/src/game/net/net-player-tracker.cpp` | 2 | - | `0x00662FA0`, `0x00664BC0` |
| `d:/dev/CP9/big2-ps4/src/game/net/net-snapshot.cpp` | 2 | - | `0x0066B070`, `0x0066B430` |
| `d:/dev/CP9/big2-ps4/src/game/particle-tester.cpp` | 2 | - | `0x00677EE0`, `0x00678090` |
| `d:/dev/CP9/big2-ps4/src/game/partspawner.cpp` | 2 | - | `0x006793B0`, `0x00679540` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-breathing.cpp` | 2 | - | `0x00729A90`, `0x0072AD40` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-climb-action.cpp` | 2 | - | `0x00736B40`, `0x0073BFA0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-event-handler.cpp` | 2 | - | `0x007EDD80`, `0x007F40F0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-fight-simple-moves.cpp` | 2 | - | `0x0085E570`, `0x00860B40` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-net-fight.cpp` | 2 | - | `0x008E0D90`, `0x008E3DE0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-peek-over.cpp` | 2 | - | `0x008EF3B0`, `0x008F04E0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-wall-rope.cpp` | 2 | - | `0x00943C50`, `0x00946090` |
| `d:/dev/CP9/big2-ps4/src/game/render/active-levels.cpp` | 2 | - | `0x0056F300`, `0x009DFA80` |
| `d:/dev/CP9/big2-ps4/src/game/rendercamera.cpp` | 2 | - | `0x0071BC20`, `0x009DE130` |
| `d:/dev/CP9/big2-ps4/src/game/state-script/ss-action.cpp` | 2 | - | `0x00A26510`, `0x00A26740` |
| `d:/dev/CP9/big2-ps4/src/game/vehicle/tank-movement.cpp` | 2 | - | `0x00A9BDE0`, `0x00A9EE30` |
| `d:/dev/CP9/big2-ps4/src/game/wall-rope.cpp` | 2 | - | `0x00ABC870`, `0x00ABD0C0` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/dropped-weapon-limiter.cpp` | 2 | - | `0x00AD1600`, `0x00AD1820` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/gatling-barrel.cpp` | 2 | - | `0x00AD34A0`, `0x00AD35D0` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/multi-projectile-bullet.cpp` | 2 | - | `0x00AD67D0`, `0x00AD8220` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/projectile-rocket.cpp` | 2 | - | `0x00B0CBE0`, `0x00B0CE00` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/shell-casing.cpp` | 2 | - | `0x00B10FB0`, `0x00B113A0` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/turret-aa-tank.cpp` | 2 | - | `0x00B14E60`, `0x00B16650` |
| `d:/dev/CP9/shared-ps4/src/bputil/ucc-gameplay/bp_ucc_hacks.cpp` | 2 | - | `0x00D514B0`, `0x00D51680` |
| `d:/dev/CP9/shared-ps4/src/corelib/containers/fixedsizeheap.cpp` | 2 | - | `0x00B26AC0`, `0x00B26C10` |
| `d:/dev/CP9/shared-ps4/src/corelib/containers/tagged-heap.cpp` | 2 | - | `0x00B42030`, `0x00B424D0` |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/allocator-tagged-heap.cpp` | 2 | - | `0x00D73790`, `0x00D73F50` |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/heaparena.cpp` | 2 | - | `0x00B32430`, `0x00B32590` |
| `d:/dev/CP9/shared-ps4/src/ndlib/anim/nd-anim-util.cpp` | 2 | - | `0x00B566F0`, `0x00B57190` |
| `d:/dev/CP9/shared-ps4/src/ndlib/io/loadingheap.cpp` | 2 | - | `0x00B65160`, `0x00B65510` |
| `d:/dev/CP9/shared-ps4/src/ndlib/memory/relocatable-heap.cpp` | 2 | - | `0x00CEEA70`, `0x00CEF1F0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/photo-mode/photo-mode-manager.cpp` | 2 | - | `0x00B8A010`, `0x00B8A3A0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/dev/render-options.cpp` | 2 | - | `0x00BD1F30`, `0x00BD2020` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/gui/hud-quad.cpp` | 2 | - | `0x00BDBB10`, `0x00BDC670` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ndgi/submit-queue.cpp` | 2 | - | `0x00C23530`, `0x00C236B0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/buffers.cpp` | 2 | - | `0x00C23930`, `0x00C23E50` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/compute-queue-mgr.cpp` | 2 | - | `0x00C24250`, `0x00C24550` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/const-buffer-mgr.cpp` | 2 | - | `0x00C24B30`, `0x00C26100` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/module-pool.cpp` | 2 | - | `0x00C565E0`, `0x00C56DF0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-draw-env.cpp` | 2 | - | `0x00D0C0F0`, `0x00D24470` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-draw-reg.cpp` | 2 | - | `0x00D0A060`, `0x00D24F60` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-light-probes.cpp` | 2 | - | `0x00D0C0F0`, `0x00D25C40` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-make-quad.cpp` | 2 | - | `0x00D27D20`, `0x00D29B00` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-make-ribbon.cpp` | 2 | - | `0x00D2B7A0`, `0x00D2C0C0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-update.cpp` | 2 | - | `0x00D0BCB0`, `0x00D0DB00` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/util/glyph.cpp` | 2 | - | `0x00CC2690`, `0x00CC29A0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/save/savestatemachine.cpp` | 2 | - | `0x00D17720`, `0x00D18B00` |
| `d:/dev/CP9/shared-ps4/src/ndlib/script/tlou/script-manager.cpp` | 2 | - | `0x00CD8470`, `0x00CD99E0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/smoketests/smoke-test-cmds.cpp` | 2 | - | `0x00D1A8E0`, `0x00D1AA20` |
| `d:/dev/CP9/shared-ps4/src/ndlib/util/stringid.cpp` | 2 | - | `0x00A14120`, `0x00CE33A0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/water/rain-mgr.cpp` | 2 | - | `0x00CE3BF0`, `0x00CE4280` |
| `../../Common/Base/Reflection/Registry/hkTypeInfoRegistry.h` | 1 | - | `0x00DBC900` |
| `../../Common/Internal/GeometryProcessing/Mesh/hkgpMesh.h` | 1 | - | `0x00DDC1D0` |
| `../../Geometry/Internal/DataStructures/DynamicTree/hkcdDynamicTree.inl` | 1 | - | `0x00D8D800` |
| `../../Physics/Internal/Collide/StaticCompound/hkpStaticCompoundShape_Internals.inl` | 1 | - | `0x000362B0` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Animation/Animation/Rig/hkaSkeletonUtils.cpp` | 1 | - | `0x00025A90` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Common/Base/Reflection/Util/hkVariantDataUtil.cpp` | 1 | - | `0x0011A8F0` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Common/Base/Types/Properties/hkRefCountedProperties.cpp` | 1 | - | `0x00127E50` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Common/Serialize/Util/hkStructureLayout.cpp` | 1 | - | `0x000F50A0` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Physics/Collide/Dispatch/hkpCollisionDispatcher.cpp` | 1 | - | `0x000BE290` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShapeConstructor.cpp` | 1 | - | `0x001C5F60` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Physics/Dynamics/World/Simulation/Continuous/hkpContinuousSimulation.cpp` | 1 | - | `0x000730B0` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Physics/Dynamics/World/Simulation/Multithreaded/hkpMultithreadedSimulation.cpp` | 1 | - | `0x00079530` |
| `D:/Dev/CP9/shared-ps4/src/physics/hk2011_3_0_r1/Source/Physics/Dynamics/World/hkpWorld.cpp` | 1 | - | `0x00069A90` |
| `D:/depot/Release/2011_3/Custom/2011_3_ND_Toaster/Source/Common/Internal/GeometryProcessing/ConvexHull/hkgpConvexHull.cpp` | 1 | - | `0x00196210` |
| `D:/depot/Release/2011_3/Custom/2011_3_ND_Toaster/Source/Physics/Internal/BroadPhase/TreeBroadPhase/hkpTreeBroadPhase.cpp` | 1 | - | `0x0002A310` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/autopan.cpp` | 1 | - | `0x0014A9F0` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/autopb.cpp` | 1 | - | `0x0014AD70` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/autoptch.cpp` | 1 | - | `0x0014B070` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/autovol.cpp` | 1 | - | `0x0014B370` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/scream/src/common/boomrang2/brb2_synth_interface.h` | 1 | - | `0x00DC67F0` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/snd_stream/src/common/mp3_ofl_parser.cpp` | 1 | - | `0x00190690` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/snd_stream/src/common/sequencer.cpp` | 1 | - | `0x00191660` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/snd_stream/src/ps4/snd_host_fileio_ps4.cpp` | 1 | - | `0x0018FC10` |
| `D:/dev/cp9/shared-ps4/src/scea_shared/wws_sdk/components/wws_scream/snd_stream/src/ps4/snd_host_ps4.cpp` | 1 | - | `0x00190250` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/catmull-rom.h` | 1 | - | `0x006BE0D0` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/gamephys/broadphase.h` | 1 | - | `0x004DE9D0` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/gamephys/rigid-body.h` | 1 | - | `0x00D78690` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/player/player-free-rope.h` | 1 | - | `0x0087C460` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/region.h` | 1 | - | `0x00571AA0` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/rendercamera.h` | 1 | - | `0x009DC750` |
| `d:/dev/CP9/BP/Bin/../../big2-ps4/src/game/widgets/dev-widgets.h` | 1 | - | `0x00B24F70` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/corelib/math/simd.h` | 1 | - | `0x0036F460` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/anim/ik/ik-chain.h` | 1 | - | `0x00B585C0` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/interface/texture-mgr.h` | 1 | - | `0x00BF37C0` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/ngen/vertex-stream.h` | 1 | - | `0x00C3B6F0` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/particle/particle-handle.h` | 1 | - | `0x00C6E830` |
| `d:/dev/CP9/BP/Bin/../../shared-ps4/src/ndlib/render/shading/parameter-binding.h` | 1 | - | `0x00C2D4A0` |
| `d:/dev/CP9/big2-ps4/src/biglib/io/prefetch.cpp` | 1 | - | `0x00D47E10` |
| `d:/dev/CP9/big2-ps4/src/game/ai/action-pack/action-pack.cpp` | 1 | - | `0x002077F0` |
| `d:/dev/CP9/big2-ps4/src/game/ai/action-pack/cover-action-pack.cpp` | 1 | - | `0x0020C1F0` |
| `d:/dev/CP9/big2-ps4/src/game/ai/action-pack/scripted-action-pack.cpp` | 1 | - | `0x0020EA60` |
| `d:/dev/CP9/big2-ps4/src/game/ai/action-pack/sla-action-pack.cpp` | 1 | - | `0x0020F9C0` |
| `d:/dev/CP9/big2-ps4/src/game/ai/agent/npc-spawn-info.cpp` | 1 | - | `0x00233600` |
| `d:/dev/CP9/big2-ps4/src/game/ai/characters/chicken.cpp` | 1 | - | `0x0027BC90` |
| `d:/dev/CP9/big2-ps4/src/game/ai/characters/elena.cpp` | 1 | - | `0x00281070` |
| `d:/dev/CP9/big2-ps4/src/game/ai/characters/npc-stand-in.cpp` | 1 | - | `0x0028B140` |
| `d:/dev/CP9/big2-ps4/src/game/ai/characters/sullivan.cpp` | 1 | - | `0x00294900` |
| `d:/dev/CP9/big2-ps4/src/game/ai/component/patrol.cpp` | 1 | - | `0x00299450` |
| `d:/dev/CP9/big2-ps4/src/game/ai/controller/hit-controller.cpp` | 1 | - | `0x002C88B0` |
| `d:/dev/CP9/big2-ps4/src/game/ai/controller/sync-action-controller.cpp` | 1 | - | `0x002EE400` |
| `d:/dev/CP9/big2-ps4/src/game/ai/influence-map.cpp` | 1 | - | `0x002F6840` |
| `d:/dev/CP9/big2-ps4/src/game/ai/joint-modifiers/joint-modifiers.cpp` | 1 | - | `0x002F8B40` |
| `d:/dev/CP9/big2-ps4/src/game/ai/reaction/death-hint-mgr.cpp` | 1 | - | `0x002FCF30` |
| `d:/dev/CP9/big2-ps4/src/game/ai/skill/investigate-skill.cpp` | 1 | - | `0x003285F0` |
| `d:/dev/CP9/big2-ps4/src/game/ai/skill/net-skill.cpp` | 1 | - | `0x0033B840` |
| `d:/dev/CP9/big2-ps4/src/game/ai/skill/open-combat-skill.cpp` | 1 | - | `0x0033EB80` |
| `d:/dev/CP9/big2-ps4/src/game/ai/skill/sla-grenade-skill.cpp` | 1 | - | `0x00356DD0` |
| `d:/dev/CP9/big2-ps4/src/game/ai/topology/nav-state-machine.cpp` | 1 | - | `0x00386380` |
| `d:/dev/CP9/big2-ps4/src/game/anim/anim-cmd-generator-layer.cpp` | 1 | - | `0x003914F0` |
| `d:/dev/CP9/big2-ps4/src/game/anim/anim-simple-layer.cpp` | 1 | - | `0x0039A2B0` |
| `d:/dev/CP9/big2-ps4/src/game/anim/mesh-table.cpp` | 1 | - | `0x003AAF80` |
| `d:/dev/CP9/big2-ps4/src/game/anim/spu/joint-modifier/leg-ik-solver.plugin.cpp` | 1 | - | `0x003ADE60` |
| `d:/dev/CP9/big2-ps4/src/game/anim/spu/joint-modifier/weapon-ik-solver.plugin.cpp` | 1 | - | `0x003AFBD0` |
| `d:/dev/CP9/big2-ps4/src/game/attached-animation-control.cpp` | 1 | - | `0x003B4C10` |
| `d:/dev/CP9/big2-ps4/src/game/audio/audiospline.cpp` | 1 | - | `0x003D2010` |
| `d:/dev/CP9/big2-ps4/src/game/audio/audiotty.cpp` | 1 | - | `0x003D2010` |
| `d:/dev/CP9/big2-ps4/src/game/audio/music.cpp` | 1 | - | `0x003D9770` |
| `d:/dev/CP9/big2-ps4/src/game/audio/vox.cpp` | 1 | - | `0x003EC170` |
| `d:/dev/CP9/big2-ps4/src/game/camera/camera.cpp` | 1 | - | `0x0040DF60` |
| `d:/dev/CP9/big2-ps4/src/game/camera/cameracinema.cpp` | 1 | - | `0x004110A0` |
| `d:/dev/CP9/big2-ps4/src/game/camera/cameracinemafollow.cpp` | 1 | - | `0x00413920` |
| `d:/dev/CP9/big2-ps4/src/game/camera/cameracover.cpp` | 1 | - | `0x00417450` |
| `d:/dev/CP9/big2-ps4/src/game/camera/camerafinal.cpp` | 1 | - | `0x0041FFE0` |
| `d:/dev/CP9/big2-ps4/src/game/camera/cameramenu.cpp` | 1 | - | `0x0043B670` |
| `d:/dev/CP9/big2-ps4/src/game/character-lip-sync.cpp` | 1 | - | `0x00473010` |
| `d:/dev/CP9/big2-ps4/src/game/cutscene/cutscene-mgr.cpp` | 1 | - | `0x00480770` |
| `d:/dev/CP9/big2-ps4/src/game/data-loading.cpp` | 1 | - | `0x0048D920` |
| `d:/dev/CP9/big2-ps4/src/game/deferred-event.cpp` | 1 | - | `0x00490060` |
| `d:/dev/CP9/big2-ps4/src/game/devmenu.cpp` | 1 | - | `0x00492C60` |
| `d:/dev/CP9/big2-ps4/src/game/entityspawner.cpp` | 1 | - | `0x004BD590` |
| `d:/dev/CP9/big2-ps4/src/game/event-script-funcs.cpp` | 1 | - | `0x004C2EC0` |
| `d:/dev/CP9/big2-ps4/src/game/footik.cpp` | 1 | - | `0x004C7540` |
| `d:/dev/CP9/big2-ps4/src/game/free-rope.cpp` | 1 | - | `0x004CDB20` |
| `d:/dev/CP9/big2-ps4/src/game/fx/groundprints.cpp` | 1 | - | `0x004D0730` |
| `d:/dev/CP9/big2-ps4/src/game/fx/wetfx.cpp` | 1 | - | `0x004D10C0` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/collision-cast.cpp` | 1 | - | `0x004E3750` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/debris.cpp` | 1 | - | `0x004E8480` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/havok-contact-callbacks.cpp` | 1 | - | `0x004F6790` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/havok-test-rays.cpp` | 1 | - | `0x00509550` |
| `d:/dev/CP9/big2-ps4/src/game/gamephys/rope/physpusherrope.cpp` | 1 | - | `0x00533400` |
| `d:/dev/CP9/big2-ps4/src/game/hiddentreasure.cpp` | 1 | - | `0x0054C950` |
| `d:/dev/CP9/big2-ps4/src/game/hud/hudweapon.cpp` | 1 | - | `0x00563050` |
| `d:/dev/CP9/big2-ps4/src/game/menu/bp_menudatasource.cpp` | 1 | - | `0x00578EB0` |
| `d:/dev/CP9/big2-ps4/src/game/menu/menucontrol.cpp` | 1 | - | `0x0057A600` |
| `d:/dev/CP9/big2-ps4/src/game/menu/menudrawer.cpp` | 1 | - | `0x005B5C10` |
| `d:/dev/CP9/big2-ps4/src/game/menu/menutreasurescene.cpp` | 1 | - | `0x005D74F0` |
| `d:/dev/CP9/big2-ps4/src/game/messagemanager.cpp` | 1 | - | `0x005D9200` |
| `d:/dev/CP9/big2-ps4/src/game/moneymanager.cpp` | 1 | - | `0x005DECF0` |
| `d:/dev/CP9/big2-ps4/src/game/moveactioncontroller.cpp` | 1 | - | `0x005DF3B0` |
| `d:/dev/CP9/big2-ps4/src/game/net/net-character-fire-animation.cpp` | 1 | - | `0x005F9C90` |
| `d:/dev/CP9/big2-ps4/src/game/net/net-character-reaction.cpp` | 1 | - | `0x005FAB00` |
| `d:/dev/CP9/big2-ps4/src/game/net/net-simple-snapshot.cpp` | 1 | - | `0x0066AB90` |
| `d:/dev/CP9/big2-ps4/src/game/net/net-territory.cpp` | 1 | - | `0x00671CD0` |
| `d:/dev/CP9/big2-ps4/src/game/particle-tracker.cpp` | 1 | - | `0x00678350` |
| `d:/dev/CP9/big2-ps4/src/game/parttracker.cpp` | 1 | - | `0x0067A4C0` |
| `d:/dev/CP9/big2-ps4/src/game/player/calc-start-phase.cpp` | 1 | - | `0x0067E600` |
| `d:/dev/CP9/big2-ps4/src/game/player/jumps/player-from-edge-jumps.cpp` | 1 | - | `0x006DE970` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-arms.cpp` | 1 | - | `0x0071FE20` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-carry.cpp` | 1 | - | `0x0072FB40` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-cover-edge.cpp` | 1 | - | `0x00775040` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-edge-overhead.cpp` | 1 | - | `0x007B3CE0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-evade.cpp` | 1 | - | `0x007E6880` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-face.cpp` | 1 | - | `0x007F6750` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-fight.cpp` | 1 | - | `0x0086B6D0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-free-rope.cpp` | 1 | - | `0x008798F0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-gunmove-state.cpp` | 1 | - | `0x00884E60` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-hair.cpp` | 1 | - | `0x0088BA70` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-hands.cpp` | 1 | - | `0x0088BF70` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-look-at.cpp` | 1 | - | `0x0089AA10` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-menu.cpp` | 1 | - | `0x0089C460` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-move-carry-npc.cpp` | 1 | - | `0x008AABA0` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-move-state.cpp` | 1 | - | `0x008B7170` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-pickup.cpp` | 1 | - | `0x008F7310` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-shot-reaction.cpp` | 1 | - | `0x00914890` |
| `d:/dev/CP9/big2-ps4/src/game/player/player-turret.cpp` | 1 | - | `0x0093C690` |
| `d:/dev/CP9/big2-ps4/src/game/player/process-character-sync.cpp` | 1 | - | `0x0097C7C0` |
| `d:/dev/CP9/big2-ps4/src/game/process-light.cpp` | 1 | - | `0x0099F5F0` |
| `d:/dev/CP9/big2-ps4/src/game/process-lightning-controller.cpp` | 1 | - | `0x009A0380` |
| `d:/dev/CP9/big2-ps4/src/game/process-platform.cpp` | 1 | - | `0x009A4D40` |
| `d:/dev/CP9/big2-ps4/src/game/process-ragdoll.cpp` | 1 | - | `0x009A5BD0` |
| `d:/dev/CP9/big2-ps4/src/game/process-shield.cpp` | 1 | - | `0x009ABA10` |
| `d:/dev/CP9/big2-ps4/src/game/process/new-process.cpp` | 1 | - | `0x009B5C20` |
| `d:/dev/CP9/big2-ps4/src/game/process/process-heap.cpp` | 1 | - | `0x009B7770` |
| `d:/dev/CP9/big2-ps4/src/game/region.cpp` | 1 | - | `0x009C58D0` |
| `d:/dev/CP9/big2-ps4/src/game/render/full-screen-effects.cpp` | 1 | - | `0x009E39C0` |
| `d:/dev/CP9/big2-ps4/src/game/render/screen-shot-ctrl.cpp` | 1 | - | `0x009E5FA0` |
| `d:/dev/CP9/big2-ps4/src/game/render/spupostjobmanager.cpp` | 1 | - | `0x009E87C0` |
| `d:/dev/CP9/big2-ps4/src/game/save/savedata.cpp` | 1 | - | `0x009F1FE0` |
| `d:/dev/CP9/big2-ps4/src/game/simple-animator.cpp` | 1 | - | `0x00A21B20` |
| `d:/dev/CP9/big2-ps4/src/game/spu/ppu-emu.cpp` | 1 | - | `0x00A262B0` |
| `d:/dev/CP9/big2-ps4/src/game/state-script/ss-track-group.cpp` | 1 | - | `0x00A477C0` |
| `d:/dev/CP9/big2-ps4/src/game/state-script/ss-track.cpp` | 1 | - | `0x00A49300` |
| `d:/dev/CP9/big2-ps4/src/game/sunflare.cpp` | 1 | - | `0x00A4DEF0` |
| `d:/dev/CP9/big2-ps4/src/game/vehicle/ai-skill-rail-vehicle.cpp` | 1 | - | `0x00A7D830` |
| `d:/dev/CP9/big2-ps4/src/game/vehicle/tank-destruction.cpp` | 1 | - | `0x00A96AC0` |
| `d:/dev/CP9/big2-ps4/src/game/water/BP/waterDraw.cpp` | 1 | - | `0x00ABF7D0` |
| `d:/dev/CP9/big2-ps4/src/game/water/waterinit.cpp` | 1 | - | `0x00AC26E0` |
| `d:/dev/CP9/big2-ps4/src/game/water/watermgr.cpp` | 1 | - | `0x00AC5B20` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/aim-stabilizer.cpp` | 1 | - | `0x00AD0D60` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/empty-magazine.cpp` | 1 | - | `0x00AD3150` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/multi-projectile-bullet-instant.cpp` | 1 | - | `0x00AD3E80` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/tracer.cpp` | 1 | - | `0x00B12880` |
| `d:/dev/CP9/big2-ps4/src/game/weapon/weapon-pickup.cpp` | 1 | - | `0x00B23310` |
| `d:/dev/CP9/big2-ps4/src/game/widgets/dev-widgets.cpp` | 1 | - | `0x00B24F70` |
| `d:/dev/CP9/shared-ps4/src/bputil/ucc-gameplay/bp_ucc_hacks.h` | 1 | - | `0x00D51680` |
| `d:/dev/CP9/shared-ps4/src/bputil/ucc-gameplay/bp_ucc_speedrun.cpp` | 1 | - | `0x00D55190` |
| `d:/dev/CP9/shared-ps4/src/bputil/ucc-gameplay/bp_ucctweaks.h` | 1 | - | `0x00D57E00` |
| `d:/dev/CP9/shared-ps4/src/common/common-allocator.cpp` | 1 | - | `0x00B42AA0` |
| `d:/dev/CP9/shared-ps4/src/corelib/job/job-trace.cpp` | 1 | - | `0x00B3F8D0` |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/memory-map.cpp` | 1 | - | `0x00B327D0` |
| `d:/dev/CP9/shared-ps4/src/corelib/memory/memory-util.cpp` | 1 | - | `0x00B34910` |
| `d:/dev/CP9/shared-ps4/src/corelib/util/msg.cpp` | 1 | - | `0x00B3B1C0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/anim/anim-commands.cpp` | 1 | - | `0x00B4BAB0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/anim/joint-cache.cpp` | 1 | - | `0x00B54BC0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/anim/nd-anim-plugins.cpp` | 1 | - | `0x00B55F10` |
| `d:/dev/CP9/shared-ps4/src/ndlib/anim/plugin/retarget.plugin.cpp` | 1 | - | `0x00B58B30` |
| `d:/dev/CP9/shared-ps4/src/ndlib/anim/skel-table.cpp` | 1 | - | `0x00B5A010` |
| `d:/dev/CP9/shared-ps4/src/ndlib/fx/wetmask.cpp` | 1 | - | `0x00B646F0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/lights/runtime-lights.cpp` | 1 | - | `0x00B7F5F0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/memory/allocator-levelmem.cpp` | 1 | - | `0x00B800A0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/memory/relocatable-heap-rec.cpp` | 1 | - | `0x00CEE840` |
| `d:/dev/CP9/shared-ps4/src/ndlib/memory/smalloc.cpp` | 1 | - | `0x00B80930` |
| `d:/dev/CP9/shared-ps4/src/ndlib/net/entitlements.cpp` | 1 | - | `0x00B88BA0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/net/nptrophymanager.cpp` | 1 | - | `0x00B89540` |
| `d:/dev/CP9/shared-ps4/src/ndlib/profiling/profile-cpu.cpp` | 1 | - | `0x00B8E3A0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/profiling/profile-mgr.cpp` | 1 | - | `0x00B8E600` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/foreground.cpp` | 1 | - | `0x00BD6EA0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/frustum-corners.cpp` | 1 | - | `0x00BD8A60` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/grenade-arc.cpp` | 1 | - | `0x00BD99E0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/gui/hud-draw-queue.cpp` | 1 | - | `0x00BDACA0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/interface/bg-geometry.cpp` | 1 | - | `0x00BE2FB0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/lighting/ambient-probe-cache-mgr.cpp` | 1 | - | `0x00CF9890` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/lighting/foreground-light-env.cpp` | 1 | - | `0x00D1D500` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/lighting/light-rendering-win.cpp` | 1 | - | `0x00CFA460` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/movie-rendering.cpp` | 1 | - | `0x00BFAFF0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ndgi/frame-submission.cpp` | 1 | - | `0x00BFCF50` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/geometry-desc.cpp` | 1 | - | `0x00C2A960` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/ngen/mesh.cpp` | 1 | - | `0x00C3FCE0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-build.cpp` | 1 | - | `0x00D03F30` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/ngen/particle-make-instance.cpp` | 1 | - | `0x00D26380` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/particle-eval.cpp` | 1 | - | `0x00C67F50` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/particle/particle-node-state.cpp` | 1 | - | `0x00C6A680` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/post/post-shading-win.cpp` | 1 | - | `0x00C85210` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/post/post-shape-win.cpp` | 1 | - | `0x00C89460` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/render-globals.cpp` | 1 | - | `0x00C8AA80` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/render-target-mgr.cpp` | 1 | - | `0x00CA9620` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/shadowing/shadow-frustum.cpp` | 1 | - | `0x00D12DB0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/submit-done-keep-alive.cpp` | 1 | - | `0x00CBD1A0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/texture/tga-reader.cpp` | 1 | - | `0x00CBF8B0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/util/screen-shot-bmp.cpp` | 1 | - | `0x00D14F80` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/util/screen-shot-tga.cpp` | 1 | - | `0x00D15F80` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/util/text.cpp` | 1 | - | `0x00CC79A0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/render/vis-probe.cpp` | 1 | - | `0x00CCE790` |
| `d:/dev/CP9/shared-ps4/src/ndlib/save/savemanager.cpp` | 1 | - | `0x00CD2CC0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/text/languagemanager.cpp` | 1 | - | `0x00CDC2A0` |
| `d:/dev/CP9/shared-ps4/src/ndlib/text/textdb.cpp` | 1 | - | `0x00CDD960` |
| `d:/dev/CP9/shared-ps4/src/orbisanim/source/commandblock.cpp` | 1 | - | `0x00D3B2E0` |
| `a.h` | 0 | - | - |
| `h.C` | 0 | - | - |

## 2. Fonctions Natives C++ (`Dc*`) Résolues

| Nom C++ | Symbole Lisp | Hash `StringId` | Adresse Fonction (`image + offset`) | Prologue |
|---------|--------------|-----------------|-------------------------------------|----------|
| `DcActionPackEnabledP` | `action-pack-enabled?` | `0x5C28E86B` | `0x00A09FF0` | `554889e5534883ec` |
| `DcActivePlayerCount` | `active-player-count` | `0xC27AD2F6` | `0x00667F40` | `554889e5488d3d15` |
| `DcAddDynamicLight` | `add-dynamic-light` | `0xC639AD71` | `0x009FF2C0` | `554889e541574156` |
| `DcAddDynamicLightJoint` | `add-dynamic-light-joint` | `0x92509D49` | `0x009FF010` | `554889e541574156` |
| `DcAddImportModule` | `add-import-module` | `0xC66A1CE6` | `0x00A0FF10` | `554889e5534883ec` |
| `DcAddLoadModule` | `add-load-module` | `0xE0E51DAF` | `0x00A0FE90` | `554889e5534883ec` |
| `DcAlivePlayerCount` | `alive-player-count` | `0xC1D452C4` | `0x00667F20` | `554889e5488d3d35` |
| `DcAllEnemyNpcsDie` | `all-enemy-npcs-die` | `0x24B4ACF3` | `0x002DE9D0` | `554889e5488d3dd5` |
| `DcAllowCameraCutCompensate` | `allow-camera-cut-compensate` | `0xFAA7265B` | `0x00A0DD40` | `554889e5534883ec` |
| `DcAllowLegIkByOthers` | `allow-leg-ik-by-others` | `0x7DF5B4E3` | `0x009FCDF0` | `554889e541574156` |
| `DcAngleMod` | `angle-mod` | `0x0B26368F` | `0x00397630` | `554889e54883ec10` |
| `DcAnimStateAdvancePhase` | `anim-state-advance-phase` | `0x2481D93E` | `0x00397690` | `31c0c3666666662e` |
| `DcAnimate` | `animate` | `0x84591781` | `0x00397C50` | `488d0d91f2ad0031` |
| `DcApplyExplosionImpulse` | `apply-explosion-impulse` | `0xE693AB67` | `0x00A03B20` | `554889e541574156` |
| `DcAttachFlashlight` | `attach-flashlight` | `0xC37FECC0` | `0x00A0A4C0` | `554889e541574156` |
| `DcAutoSaveGame` | `auto-save-game` | `0x85802E50` | `0x009FE820` | `554889e541565348` |
| `DcBattleSelectBestSpawnerFromGroup` | `battle-select-best-spawner-from-group` | `0x42365F84` | `0x00A11190` | `554889e541574156` |
| `DcBlockNavPolyUnderObject` | `block-nav-poly-under-object` | `0x7BA8ED16` | `0x00A05030` | `554889e541565385` |
| `DcBroadcastEventToNpcs` | `broadcast-event-to-npcs` | `0x8679C923` | `0x004C3A00` | `554889e541574156` |
| `DcCSpawnNpc` | `c-spawn-npc` | `0x970C3E2C` | `0x002E57B0` | `554889e541574156` |
| `DcCSpawnNpcS` | `c-spawn-npc-s` | `0x7E1308AA` | `0x002E55A0` | `554889e541574156` |
| `DcCacheMusicPlaylist` | `cache-music-playlist` | `0xDB22D0D6` | `0x009FD460` | `554889e541574156` |
| `DcCacheSoundBank` | `cache-sound-bank` | `0x17AE9A8B` | `0x009FD540` | `554889e5488b3648` |
| `DcCacheSoundStream` | `cache-sound-stream` | `0x617F9354` | `0x009FD510` | `554889e5488b0648` |
| `DcCacheSpeechStream` | `cache-speech-stream` | `0x03290A06` | `0x009FD4E0` | `554889e5488b0648` |
| `DcCalcNewPhaseFromSpeed` | `calc-new-phase-from-speed` | `0x6E75BABE` | `0x00A0FBF0` | `554889e553504889` |
| `DcCalcStartPhase` | `calc-start-phase` | `0xDFCFB620` | `0x00A0FB70` | `554889e553504889` |
| `DcCalcStartPhaseMaxFrames` | `calc-start-phase-max-frames` | `0x9AB7F010` | `0x00A0FAF0` | `554889e553504889` |
| `DcCalcStartPhaseRange` | `calc-start-phase-range` | `0x353AC3EE` | `0x00A0FA70` | `554889e553504889` |
| `DcCameraAbandon` | `camera-abandon` | `0x02E9965F` | `0x00A0E5E0` | `554889e541565385` |
| `DcCameraAimOverrideDirection` | `camera-aim-override-direction` | `0x3C3D70D3` | `0x00A0DE40` | `554889e5534883ec` |
| `DcCameraAimOverridePoint` | `camera-aim-override-point` | `0xD57D36E9` | `0x00A0DF40` | `554889e5535085ff` |
| `DcCameraAnimationDisable` | `camera-animation-disable` | `0xD820968B` | `0x00A033B0` | `554889e541574156` |
| `DcCameraAnimationEnable` | `camera-animation-enable` | `0xA4AC1C2F` | `0x00A03480` | `554889e541574156` |
| `DcCameraCancelAimOverride` | `camera-cancel-aim-override` | `0x0603B595` | `0x00A0DE30` | `c7058212a7000000` |
| `DcCameraDisable` | `camera-disable` | `0x29C9E722` | `0x00A0E500` | `554889e541565385` |
| `DcCameraDisableAll` | `camera-disable-all` | `0xD754EF39` | `0x00A0E330` | `554889e5488b0515` |
| `DcCameraEnable` | `camera-enable` | `0xB71CA7A3` | `0x00A0E680` | `554889e541574156` |
| `DcCameraFadeOut` | `camera-fade-out` | `0xB5E93ED4` | `0x00A0E380` | `554889e541574156` |
| `DcCameraLocalSpace` | `camera-local-space` | `0xDCB3081B` | `0x00A0EA20` | `554889e541565348` |
| `DcCameraLocator` | `camera-locator` | `0xBA364A1C` | `0x00A128B0` | `554889e5534883ec` |
| `DcCameraMovedBackwardsP` | `camera-moved-backwards?` | `0xC31D3021` | `0x00A0DC40` | `85ff7e06c5fa1006` |
| `DcCameraRestoreLocator` | `camera-restore-locator` | `0xD40D2524` | `0x00A0E9B0` | `c605f906a7000131` |
| `DcCameraShake` | `camera-shake` | `0xF3819432` | `0x00A0E040` | `554889e541574156` |
| `DcCameraShakeInZoomCamera` | `camera-shake-in-zoom-camera` | `0x7D1AC5EB` | `0x00A0DCA0` | `554889e5534883ec` |
| `DcCameraStoreLocator` | `camera-store-locator` | `0x291173D8` | `0x00A0E9C0` | `488d0d4158700048` |
| `DcChooseStumbleAnim` | `choose-stumble-anim` | `0x4A5A1790` | `0x00907370` | `554889e541574156` |
| `DcClearHarvestedBodies` | `clear-harvested-bodies` | `0xCC24252E` | `0x00A01A90` | `554889e5488d3d05` |
| `DcClearLookAt` | `clear-look-at` | `0x583D42F2` | `0x002E4830` | `554889e541574156` |
| `DcConstrainRopeAtJoint` | `constrain-rope-at-joint` | `0x2976EC09` | `0x00A04430` | `554889e541574156` |
| `DcCoverThrowGrenadeAnim` | `cover-throw-grenade-anim` | `0x3B74114C` | `0x002E4070` | `554889e541574156` |
| `DcCreateBeltAttachment` | `create-belt-attachment` | `0x6409BEA0` | `0x0090AD30` | `554889e541574156` |
| `DcCreateWeaponDrawables` | `create-weapon-drawables` | `0x6C7BC0AB` | `0x00A0C410` | `554889e541574156` |
| `DcCurrentTime` | `current-time` | `0x388BC9A1` | `0x00A0EDB0` | `b80100000085ff7e` |
| `DcDeactivateBlur` | `deactivate-blur` | `0xD862F50C` | `0x009FFF50` | `554889e5488d0dc5` |
| `DcDebugBreak` | `debug-break` | `0x6699F752` | `0x009FDF30` | `554889e5535031c0` |
| `DcDebugDrawCross` | `debug-draw-cross` | `0x227F2974` | `0x009FE070` | `554889e585ff7e06` |
| `DcDebugDrawLine` | `debug-draw-line` | `0xBD63A7BA` | `0x009FE090` | `554889e531c0b900` |
| `DcDebugDrawLocator` | `debug-draw-locator` | `0xE847689B` | `0x009FE000` | `554889e585ff7e4d` |
| `DcDebugDrawString` | `debug-draw-string` | `0xEB7947DB` | `0x009FDFB0` | `554889e5535089f8` |
| `DcDebugPause` | `debug-pause` | `0x7195C4EF` | `0x00A102A0` | `488d0531936400c6` |
| `DcDemoResetDefaults` | `demo-reset-defaults` | `0x492DE666` | `0x0090DD50` | `488d054d187400c7` |
| `DcDespawnMessage` | `despawn-message` | `0xB1F17BD7` | `0x005DC8C0` | `554889e58b36488d` |
| `DcDetachRope` | `detach-rope` | `0x4D209D87` | `0x00A04530` | `554889e541565385` |
| `DcDifficultyInterp` | `difficulty-interp` | `0x0057202D` | `0x009FDAD0` | `554889e54883ec20` |
| `DcDisableCoverCamera` | `disable-cover-camera` | `0xE1846603` | `0x00A0E290` | `554889e5534883ec` |
| `DcDisableDynoJumpFailLooks` | `disable-dyno-jump-fail-looks` | `0x402FACAB` | `0x0090CA90` | `554889e5534883ec` |
| `DcDisableEdgeGrab` | `disable-edge-grab` | `0x5D2D16C9` | `0x0090CE40` | `554889e5534881ec` |
| `DcDisableFallToDeath` | `disable-fall-to-death` | `0xB555B6E3` | `0x0090CBC0` | `554889e5534881ec` |
| `DcDisableFollowCameraCollision` | `disable-follow-camera-collision` | `0x931EB73C` | `0x0090F4F0` | `554889e5534883ec` |
| `DcDisableForceLedgeDrop` | `disable-force-ledge-drop` | `0x4ED6B3A5` | `0x0090CB20` | `554889e5534883ec` |
| `DcDisableFrameForward` | `disable-frame-forward` | `0x80DEC5B0` | `0x00A038A0` | `554889e541565385` |
| `DcDisableGadgetry` | `disable-gadgetry` | `0xF00D650C` | `0x00A03A70` | `554889e541565385` |
| `DcDisableLookAroundDof` | `disable-look-around-dof` | `0x60C027EF` | `0x0090FD30` | `554889e5534883ec` |
| `DcDisableLowCoverExitAround` | `disable-low-cover-exit-around` | `0xB6B6E273` | `0x0090FC30` | `554889e5534883ec` |
| `DcDisableMessage` | `disable-message` | `0xCB0ED5FB` | `0x005DC990` | `554889e54883ec10` |
| `DcDisableNavBlockerObject` | `disable-nav-blocker-object` | `0xC12D0539` | `0x00A04EF0` | `554889e541565385` |
| `DcDisablePlatformControlOnObject` | `disable-platform-control-on-object` | `0x88E795A7` | `0x00A04E80` | `554889e541565385` |
| `DcDisablePlayerCoverExit` | `disable-player-cover-exit` | `0xF86DB330` | `0x00906950` | `554889e5534883ec` |
| `DcDisallowLegIkByOthers` | `disallow-leg-ik-by-others` | `0xF3F258BE` | `0x009FCCF0` | `554889e541574156` |
| `DcDiscP` | `disc?` | `0x96880C26` | `0x009FD910` | `8b06488d0d27bd65` |
| `DcDisplay` | `display` | `0xBB0304D1` | `0x009FD000` | `31c0c3666666662e` |
| `DcDisplayButtonHud` | `display-button-hud` | `0x8F3A8A73` | `0x00A0CD20` | `554889e541574156` |
| `DcDisplayInt` | `display-int` | `0x7CE24226` | `0x009FCFF0` | `31c0c3666666662e` |
| `DcDisplayMessage` | `display-message` | `0xB34BBA3A` | `0x009FE140` | `554889e541574156` |
| `DcDoRumblePtr` | `do-rumble-ptr` | `0x943CE1C7` | `0x00A105F0` | `554889e541565349` |
| `DcDontCollideAgainstAllies` | `dont-collide-against-allies` | `0x5C9A1633` | `0x009FCBB0` | `554889e5534883ec` |
| `DcDrawReticle` | `draw-reticle` | `0xC0988264` | `0x0090E6E0` | `554889e5488d3d05` |
| `DcDropRope` | `drop-rope` | `0x7107D57E` | `0x00A045E0` | `554889e541574156` |
| `DcDumpEntityInfo` | `dump-entity-info` | `0xA4814C81` | `0x00A04970` | `554889e541565348` |
| `DcDumpLevelsInfo` | `dump-levels-info` | `0x38AE2B20` | `0x00A04900` | `554889e55350488d` |
| `DcElapsedTime` | `elapsed-time` | `0x8E5B8EFC` | `0x00A0ED60` | `b80100000085ff7e` |
| `DcEmitBulletFromSpawner` | `emit-bullet-from-spawner` | `0x9062B153` | `0x00A0BF40` | `554889e541574156` |
| `DcEnableActionPack` | `enable-action-pack` | `0x138ABEF3` | `0x00A0A060` | `554889e541574156` |
| `DcEnableDynoJumpFailLooks` | `enable-dyno-jump-fail-looks` | `0x86550364` | `0x0090CA00` | `554889e5534883ec` |
| `DcEnableGadgetry` | `enable-gadgetry` | `0xE46DCAC5` | `0x00A039C0` | `554889e541565385` |
| `DcEnableMessage` | `enable-message` | `0x7AA34AC4` | `0x005DC8E0` | `554889e54883ec10` |
| `DcEnableNavBlockerObject` | `enable-nav-blocker-object` | `0x451FD95B` | `0x00A04F60` | `554889e541565385` |
| `DcEndScript` | `end-script` | `0x9896D682` | `0x00A44190` | `554889e54156534c` |
| `DcEntityAliveP` | `entity-alive?` | `0x3F58662D` | `0x00A127E0` | `554889e5535085ff` |
| `DcEntityAutoSpawnOnce` | `entity-auto-spawn-once` | `0x0B9B4812` | `0x00A12830` | `554889e5535085ff` |
| `DcEntityName` | `entity-name` | `0x24B6345C` | `0x00A127D0` | `488b068b4028c366` |
| `DcEntitySetLevelB` | `entity-set-level!` | `0x9F684FA6` | `0x00A64C30` | `31c0c3666666662e` |
| `DcEventArgBoolean` | `event-arg-boolean` | `0x77162698` | `0x004C2A40` | `554889e5488d1512` |
| `DcEventArgBoundFrame` | `event-arg-bound-frame` | `0x5EE829E2` | `0x004C28F0` | `554889e541574156` |
| `DcEventArgFloat` | `event-arg-float` | `0xC69511C1` | `0x004C2BF0` | `554889e541574156` |
| `DcEventArgInt32` | `event-arg-int-32` | `0x62A32E72` | `0x004C2BE0` | `488d1554bc9b00e9` |
| `DcEventArgString` | `event-arg-string` | `0xF9453071` | `0x004C2EC0` | `554889e541574156` |
| `DcEventArgSymbol` | `event-arg-symbol` | `0x36D703B7` | `0x004C2DA0` | `554889e541574156` |
| `DcEventAttackSourceP` | `event-attack-source?` | `0x566FCBA4` | `0x004C3730` | `554889e541574156` |
| `DcEventAttackTypeP` | `event-attack-type?` | `0x488D3BB8` | `0x004C38A0` | `554889e541574156` |
| `DcEventGetAttackInfo` | `event-get-attack-info` | `0xF2776CC0` | `0x004C3D50` | `554889e541574156` |
| `DcEventLevelP` | `event-level?` | `0x7FEEF039` | `0x004C34C0` | `554889e541574156` |
| `DcEventMyLevelP` | `event-my-level?` | `0x617579D2` | `0x004C32D0` | `554889e541574156` |
| `DcEventObjectDamage` | `event-object-damage` | `0xF56F37C9` | `0x004C3610` | `554889e54156534c` |
| `DcEventReturnBoolean` | `event-return-boolean` | `0x7771F473` | `0x004C1FF0` | `554889e541574156` |
| `DcEventReturnFloat` | `event-return-float` | `0x338C1B7B` | `0x004C2550` | `554889e541574156` |
| `DcEventReturnInt32` | `event-return-int-32` | `0xE5D40210` | `0x004C2390` | `554889e541574156` |
| `DcEventReturnSymbol` | `event-return-symbol` | `0xB1A02FD5` | `0x004C2720` | `554889e541574156` |
| `DcEventReturnUint32` | `event-return-uint-32` | `0xA0F5E780` | `0x004C21D0` | `554889e541574156` |
| `DcEventSender` | `event-sender` | `0x030B1EA2` | `0x004C1DF0` | `554889e54156534c` |
| `DcEventSenderP` | `event-sender?` | `0xEA2054F4` | `0x004C1ED0` | `554889e541574156` |
| `DcEventTrainCarP` | `event-train-car?` | `0x1C6DCAF0` | `0x004C3020` | `554889e541574156` |
| `DcEventTrainP` | `event-train?` | `0x2A1DEF35` | `0x004C31A0` | `554889e541574156` |
| `DcFacingPointP` | `facing-point?` | `0x18AF3A3C` | `0x00A0F6D0` | `554889e541574156` |
| `DcFadeToBlack` | `fade-to-black` | `0xDEA49A81` | `0x009FFBA0` | `554889e5534883ec` |
| `DcFadeToBlackPers` | `fade-to-black-pers` | `0x77E20A49` | `0x009FFA40` | `554889e5534883ec` |
| `DcFadeToWhite` | `fade-to-white` | `0x5DAF1DDD` | `0x009FFAC0` | `554889e5534883ec` |
| `DcFallToDeath` | `fall-to-death` | `0x2722ADE3` | `0x0090C410` | `554889e5534881ec` |
| `DcFireWeapon` | `fire-weapon` | `0xC7FD990C` | `0x00A0C930` | `554889e541574156` |
| `DcFireWeaponAtTarget` | `fire-weapon-at-target` | `0xF472F811` | `0x00A0C4C0` | `554889e541574156` |
| `DcFmaRequestPartModule` | `fma-request-part-module` | `0x5B61666F` | `0x00A00050` | `554889e541574156` |
| `DcFmaResetPartModules` | `fma-reset-part-modules` | `0x3D69B8F7` | `0x00A00130` | `31c0c3666666662e` |
| `DcFollowSpline` | `follow-spline` | `0x0985239F` | `0x00A25E50` | `554889e541574156` |
| `DcForGroup` | `for-group` | `0xEB281C9C` | `0x0054B830` | `554889e541574156` |
| `DcForceAimingCameraUltraTight` | `force-aiming-camera-ultra-tight` | `0x653EDCDF` | `0x00A03670` | `554889e5534883ec` |
| `DcForceBoardJetski` | `force-board-jetski` | `0x3FF2398E` | `0x00A01BA0` | `31c0c3666666662e` |
| `DcForceCollisionForAimFocus` | `force-collision-for-aim-focus` | `0x844E9FD7` | `0x00906F90` | `554889e5534883ec` |
| `DcForceDeathCamera` | `force-death-camera` | `0xD76A667D` | `0x0090EFF0` | `554889e5534883ec` |
| `DcForceDistanceStopIncrease` | `force-distance-stop-increase` | `0x05267174` | `0x00906740` | `554889e5534883ec` |
| `DcForceDockJetski` | `force-dock-jetski` | `0xABAA7382` | `0x00A01B90` | `31c0c3666666662e` |
| `DcForceHairAnim` | `force-hair-anim` | `0xD7539738` | `0x00906840` | `554889e54883ec10` |
| `DcForceHighLedgeDrop` | `force-high-ledge-drop` | `0x258DF119` | `0x0090F630` | `554889e5534883ec` |
| `DcForceNoShiftCameraZoomMode` | `force-no-shift-camera-zoom-mode` | `0x0C474223` | `0x00A035E0` | `554889e5534883ec` |
| `DcForcePlayerSwim` | `force-player-swim` | `0xFA72A080` | `0x0090D740` | `833e01488d05c620` |
| `DcForceReflections` | `force-reflections` | `0x172B8004` | `0x00A02960` | `554889e5535085ff` |
| `DcForceStealthOff` | `force-stealth-off` | `0xE3FC9E0B` | `0x00A0BC40` | `554889e5534883ec` |
| `DcForceUboatCollision` | `force-uboat-collision` | `0xE96CD997` | `0x0090C960` | `554889e5534883ec` |
| `DcForgetSignal` | `forget-signal` | `0xFC37CD06` | `0x00A3FF40` | `554889e541574156` |
| `DcFreezeBindingWhenNonPhysical` | `freeze-binding-when-non-physical` | `0x5205E230` | `0x00A03700` | `554889e541574156` |
| `DcFromUps` | `from-ups` | `0x6CD3AABF` | `0x003976A0` | `c5fa1006c5fa5946` |
| `DcFromUpsNoDt` | `from-ups-no-dt` | `0x9CEF4D21` | `0x009FD6B0` | `554889e5535083ff` |
| `DcGameTaskGraphLogin` | `game-task-graph-login` | `0xB8831CF4` | `0x009FE8F0` | `554889e5488b3ee8` |
| `DcGetActionPackLocator` | `get-action-pack-locator` | `0x30A7A25C` | `0x00A060C0` | `554889e541565348` |
| `DcGetAlignLocator` | `get-align-locator` | `0x7922FC22` | `0x00A066F0` | `554889e541574156` |
| `DcGetAnimationPhase` | `get-animation-phase` | `0x05516ACB` | `0x003976C0` | `554889e541574156` |
| `DcGetApReferenceEffectLocator` | `get-ap-reference-effect-locator` | `0xC6B66B4C` | `0x00A07340` | `554889e541574156` |
| `DcGetApReferenceLocator` | `get-ap-reference-locator` | `0x4E10B00C` | `0x00A07990` | `554889e541574156` |
| `DcGetApReferenceLocatorRadial` | `get-ap-reference-locator-radial` | `0x6825AE79` | `0x00A06C20` | `554889e541574156` |
| `DcGetBoolean` | `get-boolean` | `0xF307E431` | `0x00A42BF0` | `554889e541574156` |
| `DcGetCaptureZone` | `get-capture-zone` | `0xDFFEB259` | `0x006687D0` | `31c0c3666666662e` |
| `DcGetCurrentTime` | `get-current-time` | `0x55B2CA6C` | `0x00A44800` | `554889e541574156` |
| `DcGetDda` | `get-dda` | `0xA86D881D` | `0x009FD010` | `554889e54883ec10` |
| `DcGetDifficulty` | `get-difficulty` | `0xDCF596C6` | `0x009FD040` | `833d3519ad0001b8` |
| `DcGetFloat` | `get-float` | `0x1CB8C51F` | `0x00A43170` | `554889e541574156` |
| `DcGetGrappleApRef` | `get-grapple-ap-ref` | `0xC08B6878` | `0x009102C0` | `554889e541574156` |
| `DcGetHarvestingPriority` | `get-harvesting-priority` | `0x445ED996` | `0x00A01AB0` | `488d05e9c170000f` |
| `DcGetInt32` | `get-int-32` | `0x71666D4D` | `0x00A42EB0` | `554889e541574156` |
| `DcGetJointAngleDiffFromBindpose` | `get-joint-angle-diff-from-bindpose` | `0x2FD346D0` | `0x00A06180` | `554889e541574156` |
| `DcGetJointLocator` | `get-joint-locator` | `0xF6A75AAD` | `0x00A07E80` | `554889e541574156` |
| `DcGetJointPosition` | `get-joint-position` | `0x4902715B` | `0x00A120C0` | `554889e541565348` |
| `DcGetLocatorDistance` | `get-locator-distance` | `0x500A967E` | `0x00A0EF50` | `554889e541565348` |
| `DcGetMeleeTeachDest` | `get-melee-teach-dest` | `0x73D95253` | `0x00908D10` | `554889e541565348` |
| `DcGetObjectHealth` | `get-object-health` | `0xE990B4BD` | `0x00A04B10` | `554889e541565348` |
| `DcGetObjectId` | `get-object-id` | `0x845BE77A` | `0x00A0BEE0` | `554889e541565385` |
| `DcGetObjectLocator` | `get-object-locator` | `0xB4C97EE4` | `0x00A08630` | `554889e541574156` |
| `DcGetObjectLocatorRadial` | `get-object-locator-radial` | `0x8F74AFF2` | `0x00A081A0` | `554889e541574156` |
| `DcGetObjectOffsetLocator` | `get-object-offset-locator` | `0x28FAB5FF` | `0x00A121E0` | `554889e541574156` |
| `DcGetObjectOffsetPosition` | `get-object-offset-position` | `0x39F8C8B8` | `0x00A12390` | `554889e541574156` |
| `DcGetObjectPosition` | `get-object-position` | `0x1A227342` | `0x00A12610` | `554889e541565348` |
| `DcGetObjectState` | `get-object-state` | `0x874DD706` | `0x00A04C10` | `554889e541574156` |
| `DcGetOriginLocator` | `get-origin-locator` | `0x9927F0D1` | `0x00A05F60` | `554889e541565348` |
| `DcGetPlayerAimTime` | `get-player-aim-time` | `0x53B6ED2C` | `0x005DCBA0` | `554889e5534881ec` |
| `DcGetPlayerCarriedObject` | `get-player-carried-object` | `0xE4549025` | `0x009068C0` | `554889e541574156` |
| `DcGetPlayerClipPercentFull` | `get-player-clip-percent-full` | `0xCAD226B3` | `0x005DCE00` | `554889e54883ec10` |
| `DcGetPlayerEdgeBbject` | `get-player-edge-bbject` | `0xDB3EF96A` | `0x0090DCA0` | `554889e5534883ec` |
| `DcGetPlayerHealth` | `get-player-health` | `0xAF909084` | `0x00910140` | `554889e55350488d` |
| `DcGetPlayerPlatform` | `get-player-platform` | `0x2F1BD5DD` | `0x0090CF40` | `554889e541574156` |
| `DcGetPlayerSpeed` | `get-player-speed` | `0x19800ED3` | `0x00910020` | `554889e54883ec10` |
| `DcGetPlayerTeachComboSquare` | `get-player-teach-combo-square` | `0x21034914` | `0x005DCDF0` | `488d05c8abb7000f` |
| `DcGetPlayerTeachComboTriangle` | `get-player-teach-combo-triangle` | `0x8642C309` | `0x005DCDE0` | `488d05d7abb7000f` |
| `DcGetPlayerTimeInCombat` | `get-player-time-in-combat` | `0xAB98161A` | `0x005DCD60` | `c5f857c0833d2914` |
| `DcGetPlayerTimeSinceCover` | `get-player-time-since-cover` | `0x659EB76A` | `0x005DCCE0` | `488d0d39f0b70048` |
| `DcGetQuaternionPitch` | `get-quaternion-pitch` | `0xD885C954` | `0x009F37C0` | `554889e54883ec10` |
| `DcGetScriptState` | `get-script-state` | `0x1359246D` | `0x00A44290` | `554889e541565385` |
| `DcGetSelfId` | `get-self-id` | `0xF6F9C2E4` | `0x00A04A00` | `554889e5be6655b7` |
| `DcGetSpawnerLocator` | `get-spawner-locator` | `0x072AF1AC` | `0x00A09330` | `554889e541574156` |
| `DcGetSpawnerLocatorProjected` | `get-spawner-locator-projected` | `0xD857F64E` | `0x00A08D10` | `554889e541574156` |
| `DcGetSpawnerLocatorRadial` | `get-spawner-locator-radial` | `0xF2186986` | `0x00A088C0` | `554889e541574156` |
| `DcGetSpawnerPosition` | `get-spawner-position` | `0x49D090E5` | `0x00A094B0` | `554889e541574156` |
| `DcGetStatValue` | `get-stat-value` | `0x5492073C` | `0x0097BD10` | `5349bb0000000001` |
| `DcGetStateTime` | `get-state-time` | `0x899EE046` | `0x00A44960` | `554889e541574156` |
| `DcGetSymbol` | `get-symbol` | `0x25124088` | `0x00A43440` | `554889e541574156` |
| `DcGetWaterOffset` | `get-water-offset` | `0x94BD9EEB` | `0x00A02F20` | `554889e541574156` |
| `DcGetWaterState` | `get-water-state` | `0x68E64032` | `0x00A02D90` | `554889e5535085ff` |
| `DcGivePlayerFlashlight` | `give-player-flashlight` | `0x0E67F0C9` | `0x00A0ADC0` | `554889e541574156` |
| `DcGivePlayerWeapon` | `give-player-weapon` | `0x3D37A8AA` | `0x00906B20` | `554889e541574156` |
| `DcGiveSullivanFlashlight` | `give-sullivan-flashlight` | `0x9B509CF9` | `0x00A0ABC0` | `554889e541565348` |
| `DcGrabRope` | `grab-rope` | `0x37DDF093` | `0x00A04740` | `554889e541574156` |
| `DcGrenadeDirectorChangeSettings` | `grenade-director-change-settings` | `0x6769FB9E` | `0x0069B910` | `554889e541565349` |
| `DcGrenadeDirectorDisable` | `grenade-director-disable` | `0xA7E3BDC5` | `0x0069BA40` | `554889e5833d312f` |
| `DcGrenadeDirectorEnable` | `grenade-director-enable` | `0xC77239E4` | `0x0069BAB0` | `554889e541565348` |
| `DcGrenadeDirectorIsEnabledP` | `grenade-director-is-enabled?` | `0x7052DAE7` | `0x0069B9C0` | `554889e5833db12f` |
| `DcGroundPrintsDisable` | `ground-prints-disable` | `0xEB78A103` | `0x00A01EB0` | `488d05b9567100c6` |
| `DcGroundPrintsEnable` | `ground-prints-enable` | `0x7FA59498` | `0x00A01ED0` | `488d0599567100c6` |
| `DcGroundPrintsType` | `ground-prints-type` | `0x3755F909` | `0x00A01F00` | `554889e55350bb01` |
| `DcGroupAnd` | `group-and` | `0x0B30CE4C` | `0x0054AFD0` | `554889e541574156` |
| `DcGroupAt` | `group-at` | `0x33570B73` | `0x0054A6B0` | `554889e541574156` |
| `DcGroupCount` | `group-count` | `0x4D338DEE` | `0x00549810` | `554889e541574156` |
| `DcGroupIsMemberSymbolP` | `group-is-member-symbol?` | `0x25807462` | `0x0054A250` | `554889e541574156` |
| `DcGroupOr` | `group-or` | `0x464FBA35` | `0x0054A760` | `554889e541574156` |
| `DcGroupSpawnedCount` | `group-spawned-count` | `0xEA491C13` | `0x00549BF0` | `554889e541574156` |
| `DcGuarantee1Bullet` | `guarantee-1-bullet` | `0x0EDF00A9` | `0x0090C8F0` | `554889e5488d0d25` |
| `DcHarvestingEnabledP` | `harvesting-enabled?` | `0xCCA41F6D` | `0x00A01AF0` | `488d05a9c170000f` |
| `DcHasAnimP` | `has-anim?` | `0x54D3F01B` | `0x00397A20` | `554889e541574156` |
| `DcHeliAnimateGun` | `heli-animate-gun` | `0x5DCEAFC9` | `0x00A7ECD0` | `554889e541574156` |
| `DcHeliEnableWind` | `heli-enable-wind` | `0x2B74FD3A` | `0x00A7EB60` | `554889e5534883ec` |
| `DcHeliFireMissile` | `heli-fire-missile` | `0xDBD62611` | `0x00A7F140` | `554889e541574156` |
| `DcHeliMoveTo` | `heli-move-to` | `0xC4748BE4` | `0x00A7FEE0` | `554889e541574156` |
| `DcHeliMoveToSpline` | `heli-move-to-spline` | `0xB5E9A606` | `0x00A7FDD0` | `554889e541574156` |
| `DcHeliSetBasePitch` | `heli-set-base-pitch` | `0x28DFED19` | `0x00A7EC00` | `554889e541574156` |
| `DcHeliSetFiringState` | `heli-set-firing-state` | `0x19E52453` | `0x00A7F5C0` | `554889e541574156` |
| `DcHeliSetHover` | `heli-set-hover` | `0xB55B90A0` | `0x00A7ED80` | `554889e541574156` |
| `DcHeliSetMgStats` | `heli-set-mg-stats` | `0x54AA59B5` | `0x00A7F6B0` | `554889e541574156` |
| `DcHeliSetTargetLocator` | `heli-set-target-locator` | `0x8234A90B` | `0x00A7F2E0` | `554889e541574156` |
| `DcHeliSetTargetObject` | `heli-set-target-object` | `0xB224EDF6` | `0x00A7F4A0` | `554889e541574156` |
| `DcHideHint` | `hide-hint` | `0x4D93A099` | `0x00A0CC10` | `554889e5534883ec` |
| `DcHideHud` | `hide-hud` | `0x44A7360E` | `0x00A0CD00` | `488d050133720048` |
| `DcHideMessage` | `hide-message` | `0x238A1DD3` | `0x005DCA40` | `554889e54883ec10` |
| `DcHideMessageDeprecated` | `hide-message-deprecated` | `0x8987668B` | `0x005DC650` | `554889e54883ec10` |
| `DcHolsterPlayerWeapon` | `holster-player-weapon` | `0x90FFF8B1` | `0x0090D7C0` | `554889e541574156` |
| `DcHudSetCounter` | `hud-set-counter` | `0x14887B7E` | `0x00A10470` | `8b068b4e08488d15` |
| `DcHudSetTimer` | `hud-set-timer` | `0x47F1FB04` | `0x00A104B0` | `c5fa1006c5fa5905` |
| `DcInFrontOfTankP` | `in-front-of-tank?` | `0x27654789` | `0x00AA3E10` | `554889e541574156` |
| `DcIncrementStat` | `increment-stat` | `0x356D1DB1` | `0x0097BDF0` | `554889e583ff0275` |
| `DcIncrementStatDifficulty` | `increment-stat-difficulty` | `0x07389FA9` | `0x0097C010` | `554889e541574156` |
| `DcIncrementTaskStat` | `increment-task-stat` | `0x739938AF` | `0x0097BE20` | `554889e583ff0375` |
| `DcInvalidStringId` | `invalid-string-id` | `0x99D8B976` | `0x009FDE50` | `31c0c3666666662e` |
| `DcIsCameraStickPressed` | `is-camera-stick-pressed` | `0x4ADFE187` | `0x00A0D610` | `488d05698b7000c5` |
| `DcIsDifficultyVeryEasy` | `is-difficulty-very-easy` | `0xA4B464C9` | `0x009FCFC0` | `488d0579c6650083` |
| `DcIsFma` | `is-fma` | `0x094728AF` | `0x00A0FE80` | `488d05b99764000f` |
| `DcIsJapanP` | `is-japan?` | `0x39DB8774` | `0x009FD8B0` | `488d0d89bd6500b0` |
| `DcIsOnScreen` | `is-on-screen` | `0x1E87DE60` | `0x009FD200` | `554889e541574156` |
| `DcIsSpeaking` | `is-speaking` | `0xEC6D8062` | `0x00A013F0` | `554889e541574156` |
| `DcIsStickPressedTowards` | `is-stick-pressed-towards` | `0x1EC8CE5C` | `0x00A0D6B0` | `554889e5534883ec` |
| `DcJetskiClearEngineSoundOverride` | `jetski-clear-engine-sound-override` | `0x639C300F` | `0x00A01B20` | `31c0c3666666662e` |
| `DcJetskiOverrideEngineSound` | `jetski-override-engine-sound` | `0x3983BE7D` | `0x00A01B30` | `31c0c3666666662e` |
| `DcJetskiStartEngineSound` | `jetski-start-engine-sound` | `0xC88EFECE` | `0x00A01B40` | `31c0c3666666662e` |
| `DcJournalClearPage` | `journal-clear-page` | `0xD933FE27` | `0x009FD090` | `488d05a9c56500c7` |
| `DcJournalDisable` | `journal-disable` | `0x8781BF23` | `0x00A10510` | `554889e5534883ec` |
| `DcJournalGetPage` | `journal-get-page` | `0x2E5FD939` | `0x009FD110` | `488d0529c565008b` |
| `DcJournalOpenP` | `journal-open?` | `0xEA864952` | `0x009FD0B0` | `488d0d69ec750048` |
| `DcJournalSetAmbientScale` | `journal-set-ambient-scale` | `0x67311D72` | `0x009FD060` | `85ff7e06c5fa1006` |
| `DcJournalSetPage` | `journal-set-page` | `0xE9C1106D` | `0x009FD130` | `554889e541565348` |
| `DcJump` | `jump` | `0xB420E516` | `0x00A43ED0` | `554889e541574156` |
| `DcKillAllAttachables` | `kill-all-attachables` | `0x7B197528` | `0x009F2BF0` | `554889e541574156` |
| `DcKillAllBreakables` | `kill-all-breakables` | `0xA8024193` | `0x00A0F260` | `554889e55350488d` |
| `DcKillAllDroppedWeapons` | `kill-all-dropped-weapons` | `0x50DC2915` | `0x00A0F1B0` | `554889e541574156` |
| `DcKillAllParticles` | `kill-all-particles` | `0xE178EA44` | `0x00A0FE60` | `488d0509209c00c6` |
| `DcKillDroppedWeaponsInSphere` | `kill-dropped-weapons-in-sphere` | `0x8DAC1F65` | `0x00A09B10` | `554889e541565348` |
| `DcKillEntity` | `kill-entity` | `0xDF781FBF` | `0x00A09D60` | `554889e541574156` |
| `DcKillFlashlight` | `kill-flashlight` | `0x46EABE53` | `0x00A0A130` | `554889e541574156` |
| `DcKillMessage` | `kill-message` | `0x4052E46E` | `0x005DC630` | `554889e58b36488d` |
| `DcKillPartSpawner` | `kill-part-spawner` | `0x0AB7AFB8` | `0x00A099B0` | `554889e541574156` |
| `DcKillParticles` | `kill-particles` | `0x389CE791` | `0x00A00140` | `554889e541574156` |
| `DcKillPlayer` | `kill-player` | `0x2FAE80C5` | `0x0090C790` | `554889e541574156` |
| `DcKillPlayerFlashlight` | `kill-player-flashlight` | `0x94008BCE` | `0x00A0A920` | `554889e541565348` |
| `DcKillPlayerNoAnim` | `kill-player-no-anim` | `0xC63EAEFA` | `0x0090C5F0` | `554889e541574156` |
| `DcKillSullivanFlashlight` | `kill-sullivan-flashlight` | `0xEAD294BD` | `0x00A0A750` | `554889e541574156` |
| `DcLayerToScriptControllerIndices` | `layer-to-script-controller-indices` | `0xF5F06733` | `0x00EA2F7E` | `6c61796572203d3d` |
| `DcLerpReflectionPlaneOfWaterUnits` | `lerp-reflection-plane-of-water-units` | `0xEDF05AAB` | `0x00A02B10` | `554889e5535083ff` |
| `DcLerpReflectionPlanes` | `lerp-reflection-planes` | `0x26E8F878` | `0x00A02AA0` | `554889e5535083ff` |
| `DcLerpReflectionPlanesByRotation` | `lerp-reflection-planes-by-rotation` | `0x0EB4D1E7` | `0x00A02A20` | `554889e5535083ff` |
| `DcLerpScale` | `lerp-scale` | `0x40274C92` | `0x009FDB90` | `c5fa1006c5fa104e` |
| `DcLetterboxOff` | `letterbox-off` | `0x624BF689` | `0x009FFD60` | `554889e5535085ff` |
| `DcLetterboxOn` | `letterbox-on` | `0x32839068` | `0x009FFE00` | `554889e541565348` |
| `DcLevelsFullySpawnedIgnorePlayerP` | `levels-fully-spawned-ignore-player?` | `0xA20CB8D2` | `0x009FE400` | `554889e541574156` |
| `DcLevelsFullySpawnedP` | `levels-fully-spawned?` | `0x4553458F` | `0x009FE5E0` | `554889e541574156` |
| `DcLookAt` | `look-at` | `0x58B2D6FD` | `0x002E4C80` | `554889e541574156` |
| `DcLookAtObject` | `look-at-object` | `0xEA4F3C96` | `0x002E4A40` | `554889e541574156` |
| `DcMakeActiveCharacterMoreWet` | `make-active-character-more-wet` | `0x8CDD98D5` | `0x00A020E0` | `554889e541565348` |
| `DcMakeCharacterMoreWet` | `make-character-more-wet` | `0x5B488D87` | `0x00A023C0` | `554889e5535085ff` |
| `DcMakeInert` | `make-inert` | `0x8E947C31` | `0x009FD850` | `554889e5488d056d` |
| `DcMakeVector` | `make-vector` | `0xB021733F` | `0x009FDCF0` | `554889e5534883ec` |
| `DcMakeVector3` | `make-vector-3` | `0xB90400B2` | `0x009FDC20` | `554889e5534883ec` |
| `DcMessage` | `message` | `0xC64E7E79` | `0x005DC620` | `31c0c3666666662e` |
| `DcMinmax` | `minmax` | `0x29BA2151` | `0x009FDBF0` | `c5fa1006c5fa5f46` |
| `DcMirrorAimingAt` | `mirror-aiming-at` | `0xCFCCAA8C` | `0x009FC990` | `554889e541574156` |
| `DcNetAllPlayersDeadP` | `net-all-players-dead?` | `0xEB633936` | `0x006694B0` | `554889e5534883ec` |
| `DcNetAllPlayersInRegionP` | `net-all-players-in-region?` | `0xB9C4FE7A` | `0x00669500` | `554889e541574156` |
| `DcNetCaptureFlag` | `net-capture-flag` | `0x4D52E367` | `0x00669960` | `554889e541574156` |
| `DcNetCoopHostScriptId` | `net-coop-host-script-id` | `0x7708F645` | `0x00669C40` | `31c0c3666666662e` |
| `DcNetCoopScriptId` | `net-coop-script-id` | `0xC12DB877` | `0x00669C30` | `31c0c3666666662e` |
| `DcNetCoopTask` | `net-coop-task` | `0xA615D1D6` | `0x006697E0` | `554889e5bf7d069a` |
| `DcNetCoopTeamFailedP` | `net-coop-team-failed?` | `0xBFE7BA4E` | `0x00669C20` | `488d05391bad000f` |
| `DcNetCurrentNetTask` | `net-current-net-task` | `0x601D65AB` | `0x00669780` | `554889e5bf7d069a` |
| `DcNetDrawObjectiveIcon` | `net-draw-objective-icon` | `0x1F8C0A58` | `0x00669250` | `554889e541574156` |
| `DcNetEndWave` | `net-end-wave` | `0xA3251A6A` | `0x006687E0` | `554889e5e867f5fa` |
| `DcNetGetBoosterAmount` | `net-get-booster-amount` | `0x28F84ACC` | `0x00668820` | `554889e54883ec10` |
| `DcNetGetBoosterEnabled` | `net-get-booster-enabled` | `0x344C16FE` | `0x00668810` | `554889e58b36e865` |
| `DcNetGetGrappleTime` | `net-get-grapple-time` | `0x8F971AB5` | `0x006687F0` | `554889e5bfdbd287` |
| `DcNetHeliMoveOnSpline` | `net-heli-move-on-spline` | `0xEE072D9A` | `0x00A7FCC0` | `554889e541574156` |
| `DcNetHideWaveScreen` | `net-hide-wave-screen` | `0x9EF00F55` | `0x00668850` | `554889e55350833d` |
| `DcNetHudSetGrappleIndicator` | `net-hud-set-grapple-indicator` | `0x6BDF859B` | `0x00668980` | `554889e55350833d` |
| `DcNetIsCinema` | `net-is-cinema` | `0x89676506` | `0x00669C50` | `833d254de600027c` |
| `DcNetIsGameTypeP` | `net-is-game-type?` | `0xD7BCF149` | `0x00667E60` | `554889e541565349` |
| `DcNetIsHost` | `net-is-host` | `0x4E80EEA5` | `0x00669C80` | `833df54ce600027c` |
| `DcNetKillEntity` | `net-kill-entity` | `0xEF97DF8A` | `0x006696A0` | `554889e541574156` |
| `DcNetKillTerritory` | `net-kill-territory` | `0x3C7B4B72` | `0x00669800` | `554889e5833d7151` |
| `DcNetNpcConfirmCoopPlayersContact` | `net-npc-confirm-coop-players-contact` | `0x2335480A` | `0x006690C0` | `554889e541574156` |
| `DcNetPlyaerCoopId` | `net-plyaer-coop-id` | `0x3408A884` | `0x00669240` | `31c0c3666666662e` |
| `DcNetRespawnFlag` | `net-respawn-flag` | `0xDB671BC1` | `0x006698E0` | `554889e541565385` |
| `DcNetRevivePlayer` | `net-revive-player` | `0x7FADF1FF` | `0x00667D40` | `554889e5534881ec` |
| `DcNetSendEvent` | `net-send-event` | `0x672AE8AA` | `0x004C3B60` | `554889e541574156` |
| `DcNetShowWaveScreen` | `net-show-wave-screen` | `0x25C46C02` | `0x00668890` | `554889e55350833d` |
| `DcNetSpawnFlag` | `net-spawn-flag` | `0x938F6AAF` | `0x006698C0` | `554889e5488d3d95` |
| `DcNetSpawnTerritory` | `net-spawn-territory` | `0x204CBFB2` | `0x00669840` | `554889e5488d0515` |
| `DcNpcAddSkill` | `npc-add-skill` | `0xED41C60B` | `0x002E3E70` | `554889e541574156` |
| `DcNpcAddTraversalSkill` | `npc-add-traversal-skill` | `0x63B30199` | `0x002DE790` | `554889e541574156` |
| `DcNpcAimAt` | `npc-aim-at` | `0xCDDA1161` | `0x002E0A50` | `554889e541574156` |
| `DcNpcAliveCount` | `npc-alive-count` | `0x1A69B5AD` | `0x002DF000` | `488d05b1ece20048` |
| `DcNpcAliveP` | `npc-alive?` | `0xE5325BD4` | `0x002DD900` | `554889e5535085ff` |
| `DcNpcAttachToBattleManager` | `npc-attach-to-battle-manager` | `0x4E3F7482` | `0x002E0530` | `554889e541574156` |
| `DcNpcCanSeeP` | `npc-can-see?` | `0x55345489` | `0x002DA420` | `554889e541574156` |
| `DcNpcChangeSniperParams` | `npc-change-sniper-params` | `0xF9516DB0` | `0x002DED80` | `554889e541574156` |
| `DcNpcChangeVisionSettings` | `npc-change-vision-settings` | `0x1272E557` | `0x002DA600` | `554889e541574156` |
| `DcNpcCollectOne` | `npc-collect-one` | `0x8FF445E9` | `0x002E4FB0` | `554889e541574156` |
| `DcNpcCombatBehaviorTime` | `npc-combat-behavior-time` | `0x84BE6CF9` | `0x002DD720` | `554889e541565348` |
| `DcNpcDestroyHeldObject` | `npc-destroy-held-object` | `0xC4CAC9A0` | `0x002DCB10` | `554889e541565348` |
| `DcNpcDie` | `npc-die` | `0x238A99A6` | `0x002E4680` | `554889e541574156` |
| `DcNpcDisableDealNoDamageWhenOffscreen` | `npc-disable-deal-no-damage-when-offscreen` | `0x95D8CFE0` | `0x002DEF60` | `554889e5534883ec` |
| `DcNpcDisableFlinch` | `npc-disable-flinch` | `0x8E1197F4` | `0x002DC1D0` | `554889e541574156` |
| `DcNpcDropObject` | `npc-drop-object` | `0x2E1D3291` | `0x002DCC50` | `554889e541565348` |
| `DcNpcDropWeapon` | `npc-drop-weapon` | `0x94A52BA0` | `0x002DB680` | `554889e541574156` |
| `DcNpcEnableBlockFirstTapDelay` | `npc-enable-block-first-tap-delay` | `0xF6CB805B` | `0x002E3B50` | `554889e541574156` |
| `DcNpcEnableHitReactions` | `npc-enable-hit-reactions` | `0x63E74530` | `0x002DD4B0` | `554889e541574156` |
| `DcNpcEnableShooting` | `npc-enable-shooting` | `0x2FC044DE` | `0x002DC680` | `554889e541574156` |
| `DcNpcEnteredActionPackP` | `npc-entered-action-pack?` | `0x9754326D` | `0x002DFFB0` | `554889e541565385` |
| `DcNpcFace` | `npc-face` | `0xF3906882` | `0x002DBB30` | `554889e541574156` |
| `DcNpcFireWeapon` | `npc-fire-weapon` | `0xE3B772A8` | `0x002DBE80` | `554889e541574156` |
| `DcNpcFiredRecentlyP` | `npc-fired-recently?` | `0x50B1376B` | `0x002DEAB0` | `554889e541574156` |
| `DcNpcGrabObject` | `npc-grab-object` | `0x91717C44` | `0x002DCD90` | `554889e541574156` |
| `DcNpcGroupAliveCount` | `npc-group-alive-count` | `0xE8C6C993` | `0x002DB160` | `488d0d5d5fb90031` |
| `DcNpcGroupDeadCount` | `npc-group-dead-count` | `0x034B1021` | `0x002DAEE0` | `488d0d0762b900ba` |
| `DcNpcGroupFindFirstAlive` | `npc-group-find-first-alive` | `0x97E6E340` | `0x002D7160` | `554889e541574156` |
| `DcNpcHasConfirmedStatusP` | `npc-has-confirmed-status?` | `0x2361A73D` | `0x002E2160` | `554889e541574156` |
| `DcNpcHunt` | `npc-hunt` | `0x85D88BD8` | `0x002DDA30` | `554889e541574156` |
| `DcNpcIgnoreExplosionDamage` | `npc-ignore-explosion-damage` | `0x341E10D7` | `0x002DC9C0` | `554889e541565349` |
| `DcNpcInCombatP` | `npc-in-combat?` | `0xEB265E4D` | `0x002DD830` | `554889e541565385` |
| `DcNpcInMeleeWithPlayerP` | `npc-in-melee-with-player?` | `0x56A2902B` | `0x002E0050` | `554889e541565385` |
| `DcNpcIsInScriptedAnimP` | `npc-is-in-scripted-anim?` | `0x0B765DC4` | `0x002DEEC0` | `554889e541574156` |
| `DcNpcIsNavMapPositionBlockedP` | `npc-is-nav-map-position-blocked?` | `0xC73764DF` | `0x002E07A0` | `554889e541574156` |
| `DcNpcIsPlayerInFrontP` | `npc-is-player-in-front?` | `0x7ADD53D9` | `0x002DF270` | `554889e541574156` |
| `DcNpcKilledP` | `npc-killed?` | `0xE4FC48C3` | `0x002DD990` | `554889e5535085ff` |
| `DcNpcLazSetParam` | `npc-laz-set-param` | `0x8E54B88D` | `0x002DA290` | `554889e541574156` |
| `DcNpcLowerWeapon` | `npc-lower-weapon` | `0xD96C9F58` | `0x002DBD00` | `554889e541565385` |
| `DcNpcMakeOthersAwareOf` | `npc-make-others-aware-of` | `0x95CA7E04` | `0x002DC780` | `554889e541574156` |
| `DcNpcMarkAsDefunct` | `npc-mark-as-defunct` | `0x1A6601A0` | `0x002E4F60` | `554889e553508b1e` |
| `DcNpcMaxCount` | `npc-max-count` | `0x1A933D69` | `0x002DEFF0` | `b80f000000c3662e` |
| `DcNpcMoveTo` | `npc-move-to` | `0x7CB0BB43` | `0x002E1C30` | `4889d031d24889c1` |
| `DcNpcMoveToSpawner` | `npc-move-to-spawner` | `0xB89B4C90` | `0x002E15E0` | `4889d031d24889c1` |
| `DcNpcMovementSpeed` | `npc-movement-speed` | `0xF654643C` | `0x002DCA40` | `554889e541574156` |
| `DcNpcMyBattleManagerId` | `npc-my-battle-manager-id` | `0x2BBEE040` | `0x002E4EF0` | `554889e553508b1e` |
| `DcNpcOverrideContactStatus` | `npc-override-contact-status` | `0xBC4EAE4F` | `0x002DC2D0` | `554889e541574156` |
| `DcNpcPlayFacialAnim` | `npc-play-facial-anim` | `0x01630394` | `0x002DF010` | `554889e541574156` |
| `DcNpcPlayingScriptedAnimationP` | `npc-playing-scripted-animation?` | `0x1E3D6840` | `0x002DEA10` | `554889e541574156` |
| `DcNpcPreferTarget` | `npc-prefer-target` | `0x90362D38` | `0x002DAD80` | `554889e541574156` |
| `DcNpcReloadWeapon` | `npc-reload-weapon` | `0xEE5C93B6` | `0x002DBDA0` | `554889e541565385` |
| `DcNpcRemoveSkill` | `npc-remove-skill` | `0xE7C8DB08` | `0x002E3D70` | `554889e541574156` |
| `DcNpcRemoveTraversalSkill` | `npc-remove-traversal-skill` | `0x56F22E2C` | `0x002DE6F0` | `554889e541574156` |
| `DcNpcSetAccuracyMultiplier` | `npc-set-accuracy-multiplier` | `0x8715973E` | `0x002E00F0` | `554889e541574156` |
| `DcNpcSetAllowAdjustHeadToNavMap` | `npc-set-allow-adjust-head-to-nav-map` | `0x76704022` | `0x002E3C00` | `554889e541574156` |
| `DcNpcSetAmbient` | `npc-set-ambient` | `0x2F06525E` | `0x002DA960` | `554889e541574156` |
| `DcNpcSetBuddySniperHideTime` | `npc-set-buddy-sniper-hide-time` | `0x3480A7B1` | `0x002DEC70` | `554889e541574156` |
| `DcNpcSetCanBeHit` | `npc-set-can-be-hit` | `0xF67CEE41` | `0x002DD580` | `554889e541574156` |
| `DcNpcSetCanDie` | `npc-set-can-die` | `0xB67490B3` | `0x002DD410` | `554889e541574156` |
| `DcNpcSetCombatBehavior` | `npc-set-combat-behavior` | `0xBA80EF41` | `0x002DE560` | `554889e541574156` |
| `DcNpcSetCommGroupId` | `npc-set-comm-group-id` | `0x2FDF3330` | `0x002E0430` | `554889e541574156` |
| `DcNpcSetCoverAdvanceSpline` | `npc-set-cover-advance-spline` | `0xA548A80A` | `0x002DA300` | `554889e541574156` |
| `DcNpcSetDemeanor` | `npc-set-demeanor` | `0x6054BCDD` | `0x002DC0D0` | `554889e541574156` |
| `DcNpcSetGuardRadioDelay` | `npc-set-guard-radio-delay` | `0xFDD5F3A0` | `0x002DEB60` | `554889e541574156` |
| `DcNpcSetHuntingZoneCenter` | `npc-set-hunting-zone-center` | `0xB5AFE88E` | `0x002E3330` | `554889e541574156` |
| `DcNpcSetHuntingZoneMarker` | `npc-set-hunting-zone-marker` | `0x72A43DAC` | `0x002E3570` | `554889e541574156` |
| `DcNpcSetHuntingZoneRadius` | `npc-set-hunting-zone-radius` | `0x7CA728E6` | `0x002E31C0` | `554889e541574156` |
| `DcNpcSetHuntingZoneYThreshold` | `npc-set-hunting-zone-y-threshold` | `0x764D89D6` | `0x002E3050` | `554889e541574156` |
| `DcNpcSetIgnore` | `npc-set-ignore` | `0x2290EECD` | `0x002DD350` | `554889e541574156` |
| `DcNpcSetInCombat` | `npc-set-in-combat` | `0xBD097FB5` | `0x002DA720` | `554889e541574156` |
| `DcNpcSetLeadDrakeSpline` | `npc-set-lead-drake-spline` | `0x9DC7EC70` | `0x002DFEE0` | `554889e541574156` |
| `DcNpcSetMovementSpeedScale` | `npc-set-movement-speed-scale` | `0xFEC8D941` | `0x002DD030` | `554889e541574156` |
| `DcNpcSetPatrolRoute` | `npc-set-patrol-route` | `0x8070D2C0` | `0x002DE620` | `554889e541574156` |
| `DcNpcSetPerfectAim` | `npc-set-perfect-aim` | `0x4D008BBC` | `0x002DC490` | `554889e541574156` |
| `DcNpcSetPreoccupied` | `npc-set-preoccupied` | `0xDE715E33` | `0x002DA830` | `554889e541574156` |
| `DcNpcSetShotDamageMultiplier` | `npc-set-shot-damage-multiplier` | `0x6157E60A` | `0x002DC570` | `554889e541574156` |
| `DcNpcSetToPlayerFaction` | `npc-set-to-player-faction` | `0xEA79B72C` | `0x002DC960` | `554889e5535083ff` |
| `DcNpcSetWeaponVisible` | `npc-set-weapon-visible` | `0xD331A5B5` | `0x002DB170` | `554889e541574156` |
| `DcNpcSetZoneCenter` | `npc-set-zone-center` | `0xF87637A0` | `0x002E3450` | `554889e541574156` |
| `DcNpcSetZoneMarker` | `npc-set-zone-marker` | `0x3F7DE282` | `0x002E3AA0` | `554889e541574156` |
| `DcNpcSetZoneRadius` | `npc-set-zone-radius` | `0x317EF7C8` | `0x002E3280` | `554889e541574156` |
| `DcNpcSetZoneUpdateMethod` | `npc-set-zone-update-method` | `0x4AB54ECC` | `0x002DCF50` | `554889e541574156` |
| `DcNpcSetZoneYThreshold` | `npc-set-zone-y-threshold` | `0x3EF5A899` | `0x002E3110` | `554889e541574156` |
| `DcNpcShowHitEffects` | `npc-show-hit-effects` | `0x0B1421E2` | `0x002DD1F0` | `554889e541574156` |
| `DcNpcSniperDisableWatchArea` | `npc-sniper-disable-watch-area` | `0xF8B9878C` | `0x002DB890` | `554889e541574156` |
| `DcNpcSniperSetWatchArea` | `npc-sniper-set-watch-area` | `0xAEA6AF12` | `0x002DB980` | `554889e541574156` |
| `DcNpcStop` | `npc-stop` | `0x2B949798` | `0x002DBF50` | `554889e541574156` |
| `DcNpcSwapWithRagdoll` | `npc-swap-with-ragdoll` | `0xC2633EBC` | `0x002DACE0` | `554889e541565385` |
| `DcNpcTakeWeapon` | `npc-take-weapon` | `0xC52E64F4` | `0x002DB320` | `554889e541574156` |
| `DcNpcTargetInZoneP` | `npc-target-in-zone?` | `0x4875B09D` | `0x002E2F20` | `554889e541574156` |
| `DcNpcThrowGrenadeAtPlayer` | `npc-throw-grenade-at-player` | `0x73F7057A` | `0x002E2350` | `554889e541574156` |
| `DcNpcTrackPlayerWhileBound` | `npc-track-player-while-bound` | `0x016A8633` | `0x002E1C40` | `554889e541574156` |
| `DcNpcTriggerAmbush` | `npc-trigger-ambush` | `0x2DE6BD3E` | `0x002DD0D0` | `554889e541565385` |
| `DcNpcTurnTo` | `npc-turn-to` | `0x46944D98` | `0x002DFD20` | `554889e541574156` |
| `DcNpcUnbind` | `npc-unbind` | `0x896FD647` | `0x002E1D20` | `554889e541574156` |
| `DcNpcUpdateMoveDest` | `npc-update-move-dest` | `0x196DB2D3` | `0x002E01D0` | `554889e541574156` |
| `DcNpcUseCap` | `npc-use-cap` | `0x1569BEFB` | `0x002E3F60` | `554889e541574156` |
| `DcNpcUseClosestCover` | `npc-use-closest-cover` | `0x0DF8A402` | `0x002E43B0` | `554889e541574156` |
| `DcNpcUseScriptedAp` | `npc-use-scripted-ap` | `0xAF0B3CA0` | `0x002DE250` | `554889e541574156` |
| `DcNpcUsingSkillP` | `npc-using-skill?` | `0x4ED55C14` | `0x002E3CB0` | `554889e541574156` |
| `DcNpcsGetPlayerPosition` | `npcs-get-player-position` | `0x69BEC34B` | `0x002DE830` | `554889e541574156` |
| `DcObjectInRegionP` | `object-in-region?` | `0x78543A1C` | `0x00A0FCA0` | `554889e541574156` |
| `DcObjectOnObjectP` | `object-on-object?` | `0x4A6F015A` | `0x00A04040` | `554889e541574156` |
| `DcObjectSpawnedP` | `object-spawned?` | `0xA93D2926` | `0x00A12780` | `554889e5535085ff` |
| `DcOceanBobDisable` | `ocean-bob-disable` | `0xA74871B2` | `0x00A03390` | `31c0c3666666662e` |
| `DcOceanBobEnable` | `ocean-bob-enable` | `0x87461458` | `0x00A033A0` | `31c0c3666666662e` |
| `DcOfferContextualEntity` | `offer-contextual-entity` | `0xCCF7E165` | `0x00A3FB60` | `554889e541574156` |
| `DcOfferContextualProcess` | `offer-contextual-process` | `0x8BBC4B1E` | `0x00A3FA50` | `554889e541574156` |
| `DcOverrideAimFov` | `override-aim-fov` | `0x22E90916` | `0x0090D7A0` | `c5fa1006488d0565` |
| `DcPadGetButtonPressInterval` | `pad-get-button-press-interval` | `0xD5305FD6` | `0x00A0D060` | `554889e5534883ec` |
| `DcPadGetGestureInterval` | `pad-get-gesture-interval` | `0x14FD1F7B` | `0x00A0CEF0` | `554889e541574156` |
| `DcPadGetInfo` | `pad-get-info` | `0x6EFFA5EC` | `0x00A0D210` | `554889e541565348` |
| `DcPartModuleLoadedP` | `part-module-loaded?` | `0x77CF5D09` | `0x00A00970` | `554889e5535085ff` |
| `DcPauseCoopCombo` | `pause-coop-combo` | `0x05740D1B` | `0x00667F60` | `554889e5488d3d75` |
| `DcPlayMovie` | `play-movie` | `0x0804EE71` | `0x009FE100` | `554889e541565348` |
| `DcPlayMusicStinger` | `play-music-stinger` | `0x2F4147D8` | `0x009FD3F0` | `554889e541565385` |
| `DcPlaySound` | `play-sound` | `0x49B63DA6` | `0x00A010C0` | `554889e541574156` |
| `DcPlaySoundAtJoint` | `play-sound-at-joint` | `0x3670E863` | `0x00A00DA0` | `554889e541574156` |
| `DcPlaySoundAtPosition` | `play-sound-at-position` | `0xB1969208` | `0x00A00A30` | `554889e541574156` |
| `DcPlaySoundAttached` | `play-sound-attached` | `0x385747B6` | `0x00A00F30` | `554889e541574156` |
| `DcPlayerAllowPushOffLedgeStealthKill` | `player-allow-push-off-ledge-stealth-kill` | `0x6C5C0A69` | `0x0090DF10` | `554889e5534883ec` |
| `DcPlayerBlindFireOnlyRight` | `player-blind-fire-only-right` | `0xE253573D` | `0x00A0B720` | `554889e5534883ec` |
| `DcPlayerBlindfireTargeting` | `player-blindfire-targeting` | `0xA772A5A3` | `0x0090F930` | `554889e5534883ec` |
| `DcPlayerCanMelee` | `player-can-melee` | `0xC7BB77F1` | `0x00908570` | `554889e5534883ec` |
| `DcPlayerCanMeleeCombo` | `player-can-melee-combo` | `0x98018A52` | `0x00908310` | `554889e5534883ec` |
| `DcPlayerCheckForGotHit` | `player-check-for-got-hit` | `0x4A5E469F` | `0x00A0EF20` | `554889e5488d0df5` |
| `DcPlayerClimbUp` | `player-climb-up` | `0x928AC7FC` | `0x0090A8C0` | `554889e541574156` |
| `DcPlayerClimbingUpP` | `player-climbing-up?` | `0xAB7B43B9` | `0x0090C3D0` | `554889e5488d0d45` |
| `DcPlayerDisableBrutalMelee` | `player-disable-brutal-melee` | `0x8E83955A` | `0x0090E1F0` | `554889e5534883ec` |
| `DcPlayerDisableCoverExitOver` | `player-disable-cover-exit-over` | `0x6F570C9D` | `0x0090F730` | `554889e5534883ec` |
| `DcPlayerDisableCoverSwitch` | `player-disable-cover-switch` | `0xE8426144` | `0x0090FB30` | `554889e5534883ec` |
| `DcPlayerDisableDynoDropToForeground` | `player-disable-dyno-drop-to-foreground` | `0xC2F6486D` | `0x0090EBA0` | `554889e5534883ec` |
| `DcPlayerDisableEdge180` | `player-disable-edge-180` | `0x79ECFBDC` | `0x0090EE30` | `554889e5534883ec` |
| `DcPlayerDisableFromEdgeJumps` | `player-disable-from-edge-jumps` | `0x75FDDB26` | `0x0090E290` | `554889e5534883ec` |
| `DcPlayerDisableJump` | `player-disable-jump` | `0x4330E22D` | `0x0090ED90` | `554889e5534883ec` |
| `DcPlayerDisableJumpAwayFromEdge` | `player-disable-jump-away-from-edge` | `0xAC8A12D2` | `0x00907030` | `554889e5534883ec` |
| `DcPlayerDisableMelee` | `player-disable-melee` | `0x38040704` | `0x0090E9C0` | `554889e5534883ec` |
| `DcPlayerEdgeCharacterRelativeControls` | `player-edge-character-relative-controls` | `0xCA1772C6` | `0x0090DFB0` | `554889e5534883ec` |
| `DcPlayerEnableClimbGear` | `player-enable-climb-gear` | `0x56A5851C` | `0x00908220` | `554889e5534883ec` |
| `DcPlayerEnableMoveTracker` | `player-enable-move-tracker` | `0x3CF1C20B` | `0x00909430` | `554889e541565348` |
| `DcPlayerFacingPositionP` | `player-facing-position?` | `0x11ACABB5` | `0x0090BEE0` | `554889e541574156` |
| `DcPlayerFireAtReticle` | `player-fire-at-reticle` | `0x0FC49C24` | `0x0090E710` | `554889e541574156` |
| `DcPlayerForceAiming` | `player-force-aiming` | `0x0F13FDAA` | `0x0090B2F0` | `554889e541574156` |
| `DcPlayerForceAimingTime` | `player-force-aiming-time` | `0x0FDCB8C4` | `0x0090B1F0` | `554889e5534883ec` |
| `DcPlayerForceComboLightTutorial` | `player-force-combo-light-tutorial` | `0xB01C8169` | `0x00909150` | `554889e5534883ec` |
| `DcPlayerForceComboMediumTutorial` | `player-force-combo-medium-tutorial` | `0x3220EDD8` | `0x00909240` | `554889e5534883ec` |
| `DcPlayerForceDynoJumpUp` | `player-force-dyno-jump-up` | `0xC9B3D52A` | `0x009070D0` | `554889e5534883ec` |
| `DcPlayerForceEdgeFall` | `player-force-edge-fall` | `0x792AC0C2` | `0x0090CCC0` | `554889e55350488d` |
| `DcPlayerForceFightFinisher` | `player-force-fight-finisher` | `0x0FC620EA` | `0x00908C00` | `554889e541574156` |
| `DcPlayerForceHanging` | `player-force-hanging` | `0xC3366842` | `0x0090EED0` | `554889e5534883ec` |
| `DcPlayerForceJumpAt` | `player-force-jump-at` | `0x3A646FB5` | `0x0090B3D0` | `554889e541565348` |
| `DcPlayerForceNearDynoJumps` | `player-force-near-dyno-jumps` | `0x6694C1BA` | `0x00908180` | `554889e5534883ec` |
| `DcPlayerForceNoStealth` | `player-force-no-stealth` | `0x15AEF90B` | `0x0090F830` | `554889e5534883ec` |
| `DcPlayerForceNumPunches` | `player-force-num-punches` | `0x87FBA57C` | `0x00908AF0` | `554889e541574156` |
| `DcPlayerForceStealth` | `player-force-stealth` | `0x184280D1` | `0x0090FA30` | `554889e5534883ec` |
| `DcPlayerForegroundEdgeSearch` | `player-foreground-edge-search` | `0x029BD1FC` | `0x0090EB00` | `554889e5534883ec` |
| `DcPlayerGoRagdoll` | `player-go-ragdoll` | `0xF1811405` | `0x0090C4F0` | `554889e5534881ec` |
| `DcPlayerHangingOnBarP` | `player-hanging-on-bar?` | `0x92509722` | `0x0090AB80` | `554889e55350488d` |
| `DcPlayerHasWeaponAndAmmoP` | `player-has-weapon-and-ammo?` | `0x548B9030` | `0x00907290` | `488d05894a850048` |
| `DcPlayerHasWeaponAndNoAmmoP` | `player-has-weapon-and-no-ammo?` | `0x03B61318` | `0x009071B0` | `488d05694b850048` |
| `DcPlayerHasWeaponP` | `player-has-weapon?` | `0xF2DB441A` | `0x00906A50` | `488d05c952850048` |
| `DcPlayerHideThisFrame` | `player-hide-this-frame` | `0x3DC64B27` | `0x0090E330` | `488d0de9d9840048` |
| `DcPlayerHideWeapons` | `player-hide-weapons` | `0x15B08523` | `0x0090E4A0` | `554889e5534883ec` |
| `DcPlayerInCombatP` | `player-in-combat?` | `0xC6728174` | `0x0090DC20` | `554889e55350488d` |
| `DcPlayerInCoverP` | `player-in-cover?` | `0x97B2FD28` | `0x0090BBB0` | `488d0d6901850048` |
| `DcPlayerInFriendlyVillage` | `player-in-friendly-village` | `0xDD9574EB` | `0x0090EA60` | `554889e5534883ec` |
| `DcPlayerInNpcWayP` | `player-in-npc-way?` | `0xF69EB865` | `0x0090F190` | `554889e585ff0f8e` |
| `DcPlayerInRegionP` | `player-in-region?` | `0xFAD44485` | `0x0090BD70` | `554889e541565385` |
| `DcPlayerInStateP` | `player-in-state?` | `0xE885C4D4` | `0x0090BC10` | `554889e541574156` |
| `DcPlayerInitiateMelee` | `player-initiate-melee` | `0xCB550F6B` | `0x00909420` | `488d05bae58400c6` |
| `DcPlayerInstantDrawWeapon` | `player-instant-draw-weapon` | `0x033DE9F7` | `0x0090DDE0` | `554889e5535085ff` |
| `DcPlayerIsAimingP` | `player-is-aiming?` | `0x53838E80` | `0x0090BC00` | `554889e5e8674b06` |
| `DcPlayerLerpAnimUpdateRate` | `player-lerp-anim-update-rate` | `0x1D9C28FC` | `0x003973F0` | `554889e541574156` |
| `DcPlayerMoveTo` | `player-move-to` | `0x3B502BDB` | `0x00A0B680` | `85ff0f8e8a000000` |
| `DcPlayerNoEdgeFall` | `player-no-edge-fall` | `0x6BDC75A8` | `0x009080E0` | `554889e5534883ec` |
| `DcPlayerNoReload` | `player-no-reload` | `0xEBFE460D` | `0x0090EC40` | `554889e541565348` |
| `DcPlayerOnJetskiP` | `player-on-jetski?` | `0xBB33D8A9` | `0x00A01B80` | `31c0c3666666662e` |
| `DcPlayerOnNarrowEdge` | `player-on-narrow-edge` | `0x3385FA01` | `0x0090E360` | `554889e5534883ec` |
| `DcPlayerPushForwardSpeed` | `player-push-forward-speed` | `0x06F6D605` | `0x00908750` | `554889e55350488d` |
| `DcPlayerPushingP` | `player-pushing?` | `0x68B188A0` | `0x0090AC70` | `554889e541565383` |
| `DcPlayerPushingSpeed` | `player-pushing-speed` | `0x70E42A4C` | `0x0090ABE0` | `554889e55350488d` |
| `DcPlayerRemoveWeapons` | `player-remove-weapons` | `0x5BB8329C` | `0x00907170` | `554889e5488d0da5` |
| `DcPlayerRunSpeed` | `player-run-speed` | `0xC6E56494` | `0x0090B180` | `554889e54883ec10` |
| `DcPlayerSaveWeaponSet` | `player-save-weapon-set` | `0x9955E16F` | `0x0090B830` | `554889e5534881ec` |
| `DcPlayerSetCarryNpcCamOffset` | `player-set-carry-npc-cam-offset` | `0x65E9551D` | `0x0090E170` | `554889e54883ec10` |
| `DcPlayerSetCarryNpcSlowWalk` | `player-set-carry-npc-slow-walk` | `0x593B7A3D` | `0x0090FF30` | `554889e5534883ec` |
| `DcPlayerSetCornerClimbDistScale` | `player-set-corner-climb-dist-scale` | `0x9B47D004` | `0x0090DE90` | `554889e54883ec10` |
| `DcPlayerSetDynoJumpAimAngle` | `player-set-dyno-jump-aim-angle` | `0xC5DF59CE` | `0x0090E0F0` | `554889e54883ec10` |
| `DcPlayerSetProneMode` | `player-set-prone-mode` | `0x88A2D5C2` | `0x009089D0` | `554889e541574156` |
| `DcPlayerSetTeachingComboMode` | `player-set-teaching-combo-mode` | `0x21E74B93` | `0x00909330` | `554889e5534883ec` |
| `DcPlayerTakeMeleeDamage` | `player-take-melee-damage` | `0x42DBEBFE` | `0x009101A0` | `554889e541574156` |
| `DcPlayerTeachCombo` | `player-teach-combo` | `0x6C6572C2` | `0x005DCDB0` | `488d0589c8a70083` |
| `DcPlayerTrainWreckDynoJumpHack` | `player-train-wreck-dyno-jump-hack` | `0x2A0B2311` | `0x0090E050` | `554889e5534883ec` |
| `DcPlayerTurnTo` | `player-turn-to` | `0x0174DD00` | `0x00A0B2D0` | `85ff7e74488b0648` |
| `DcPlayerUnderTrainP` | `player-under-train?` | `0xAD70C8DF` | `0x0090A660` | `554889e55350488d` |
| `DcPlayerUnparent` | `player-unparent` | `0x1DC1D601` | `0x0090F5F0` | `554889e5488d0d25` |
| `DcPlayerUseLargeNpcEdgeCollision` | `player-use-large-npc-edge-collision` | `0x6C6EAB6E` | `0x0090E400` | `554889e5534883ec` |
| `DcPowFloat` | `pow-float` | `0x614A6E07` | `0x009FDB60` | `554889e54883ec10` |
| `DcPredictOnPhysicsDriven` | `predict-on-physics-driven` | `0x9D051196` | `0x00A037C0` | `554889e541574156` |
| `DcPrintEvent` | `print-event` | `0xFE2D2DC2` | `0x004C1800` | `554889e54156534c` |
| `DcPrintFloat` | `print-float` | `0x272D183B` | `0x009FDF00` | `31c0c3666666662e` |
| `DcPrintInteger` | `print-integer` | `0x6E61504E` | `0x009FDEF0` | `31c0c3666666662e` |
| `DcPrintPoint` | `print-point` | `0xCF323EFA` | `0x00A10290` | `31c0c3666666662e` |
| `DcPrintString` | `print-string` | `0x8024F1BF` | `0x009FDEE0` | `31c0c3666666662e` |
| `DcPrintStringWs` | `print-string-ws` | `0x234EDACF` | `0x009FDEA0` | `554889e583ff0275` |
| `DcPrintSymbol` | `print-symbol` | `0x4FB6C279` | `0x00A10280` | `31c0c3666666662e` |
| `DcPrintVector` | `print-vector` | `0x8A32D4A6` | `0x009FDF10` | `31c0c3666666662e` |
| `DcPrintVector3` | `print-vector-3` | `0x45878D73` | `0x009FDF20` | `31c0c3666666662e` |
| `DcPrintf` | `printf` | `0xC107125D` | `0x00A10110` | `554889e5534881ec` |
| `DcPsnDemoShowUpsell` | `psn-demo-show-upsell` | `0x421618FC` | `0x009FD590` | `488d05a9907300c6` |
| `DcRailActivate` | `rail-activate` | `0xD0EE3B5B` | `0x00AB98D0` | `554889e5535085ff` |
| `DcRailActivateMover` | `rail-activate-mover` | `0xA57010DA` | `0x00AB8340` | `554889e541574156` |
| `DcRailAnimateToMover` | `rail-animate-to-mover` | `0x020771EF` | `0x00AB8130` | `554889e541574156` |
| `DcRailDeactivate` | `rail-deactivate` | `0x8DAF015E` | `0x00AB97D0` | `554889e541574156` |
| `DcRailDeactivateMover` | `rail-deactivate-mover` | `0x7C0C3BCE` | `0x00AB8290` | `554889e5535031db` |
| `DcRailDisableCareen` | `rail-disable-careen` | `0xC52F2C68` | `0x00AB8B80` | `554889e541574156` |
| `DcRailEnableCollideWithObjects` | `rail-enable-collide-with-objects` | `0x17054BCB` | `0x00AB8EA0` | `554889e541574156` |
| `DcRailEnableGroundHug` | `rail-enable-ground-hug` | `0x7D2FA4C6` | `0x00AB8430` | `554889e541574156` |
| `DcRailEnableNpcShooting` | `rail-enable-npc-shooting` | `0x392231A6` | `0x00AB8E40` | `554889e5488d056d` |
| `DcRailEnableTributary` | `rail-enable-tributary` | `0x1B9BC27A` | `0x00AB96B0` | `554889e541574156` |
| `DcRailEnableTruckJump` | `rail-enable-truck-jump` | `0x31873FF8` | `0x00AB9BB0` | `554889e5534883ec` |
| `DcRailFireTurretGrenade` | `rail-fire-turret-grenade` | `0x15D2B122` | `0x00AB8CD0` | `554889e541574156` |
| `DcRailFollowMover` | `rail-follow-mover` | `0x832952B6` | `0x00AB81E0` | `554889e541574156` |
| `DcRailForceCareen` | `rail-force-careen` | `0xEA5B4822` | `0x00AB8C40` | `554889e541574156` |
| `DcRailGetMoverLocator` | `rail-get-mover-locator` | `0x7D0DD7F9` | `0x00AB7D00` | `554889e541574156` |
| `DcRailGetSpline` | `rail-get-spline` | `0x9847F5E9` | `0x00AB84E0` | `554889e541565385` |
| `DcRailHasVehicle` | `rail-has-vehicle` | `0xD2FFDB06` | `0x00AB94B0` | `554889e541574156` |
| `DcRailKillAllPhysicsObjects` | `rail-kill-all-physics-objects` | `0x2066EFE9` | `0x00AB8F60` | `554889e5488d3d45` |
| `DcRailMode` | `rail-mode` | `0xEE63C2B2` | `0x00AB8610` | `554889e5535085ff` |
| `DcRailOverrideSfxState` | `rail-override-sfx-state` | `0xF8026162` | `0x00AB9070` | `554889e541574156` |
| `DcRailOverrideSpeedForAudio` | `rail-override-speed-for-audio` | `0xD3B6CABE` | `0x00AB9120` | `554889e541574156` |
| `DcRailScrub` | `rail-scrub` | `0xC300E067` | `0x00AB8F90` | `554889e541574156` |
| `DcRailSetAcceleration` | `rail-set-acceleration` | `0x6C65EA31` | `0x00AB9590` | `554889e541574156` |
| `DcRailSetAllVehiclesToMoverSpeed` | `rail-set-all-vehicles-to-mover-speed` | `0x2EA5A660` | `0x00AB7BF0` | `554889e541574156` |
| `DcRailSetFollowDistance` | `rail-set-follow-distance` | `0x334660E2` | `0x00AB8560` | `554889e541574156` |
| `DcRailSetIgcFadeOutTime` | `rail-set-igc-fade-out-time` | `0x827972A4` | `0x00AB93F0` | `c5f857c085ff7e04` |
| `DcRailSetMoverFollowDistance` | `rail-set-mover-follow-distance` | `0xD8A42677` | `0x00AB7FD0` | `554889e541565348` |
| `DcRailSetMoverIntialDistance` | `rail-set-mover-intial-distance` | `0xF03D5B80` | `0x00AB7E80` | `554889e541574156` |
| `DcRailSetMoverSpeed` | `rail-set-mover-speed` | `0xBE8978A4` | `0x00AB80A0` | `554889e5535031db` |
| `DcRailSetNpcArchetype` | `rail-set-npc-archetype` | `0xA95B4A9B` | `0x00AB9660` | `554889e5535085ff` |
| `DcRailSetSpeed` | `rail-set-speed` | `0x877634B0` | `0x00AB9230` | `554889e541574156` |
| `DcRailShutdown` | `rail-shutdown` | `0xC0E7184F` | `0x00AB9340` | `554889e55350488d` |
| `DcRailSpawn` | `rail-spawn` | `0xDE7E4738` | `0x00AB9920` | `554889e541574156` |
| `DcRailUseFixedAprefInIgc` | `rail-use-fixed-apref-in-igc` | `0x4A0864D1` | `0x00AB9410` | `554889e541574156` |
| `DcRandomFloat` | `random-float` | `0x00DD2C2E` | `0x009FDA20` | `554889e54883ec10` |
| `DcRandomInteger` | `random-integer` | `0xC4A0E51B` | `0x009FD930` | `554889e5535085ff` |
| `DcRegenerateCoverActionPacksFromObject` | `regenerate-cover-action-packs-from-object` | `0xFBEBBCDA` | `0x00A04E20` | `554889e541565385` |
| `DcReloadModule` | `reload-module` | `0x0E530D18` | `0x00A19200` | `554889e541574156` |
| `DcReloadParticleModule` | `reload-particle-module` | `0x56FB3996` | `0x00A19180` | `554889e541565348` |
| `DcRemoveContextualEntity` | `remove-contextual-entity` | `0xCE547C11` | `0x00A3F960` | `554889e54156534c` |
| `DcRemoveCoverActionPacksFromObject` | `remove-cover-action-packs-from-object` | `0x4BBC253B` | `0x00A04DC0` | `554889e541565385` |
| `DcRemoveDynamicLight` | `remove-dynamic-light` | `0x28DC0AE4` | `0x009FEDF0` | `554889e5535085ff` |
| `DcRemovePlayerWeapon` | `remove-player-weapon` | `0x379BE9FD` | `0x00906E10` | `554889e5488d0505` |
| `DcRenameObject` | `rename-object` | `0x3F50EB13` | `0x00A04D10` | `554889e541574156` |
| `DcRenderSettingsApplyDefaults` | `render-settings-apply-defaults` | `0x9BBE8C16` | `0x00A10760` | `554889e58b3e893d` |
| `DcResetActors` | `reset-actors` | `0xD55E5522` | `0x009FD690` | `554889e5e837bbf6` |
| `DcResetAimFov` | `reset-aim-fov` | `0x6EC37668` | `0x0090D780` | `488d0589207400c7` |
| `DcResetHarvestingPriority` | `reset-harvesting-priority` | `0x2C101629` | `0x00A01AE0` | `488d05b9c17000c6` |
| `DcResetStat` | `reset-stat` | `0xD6AEAC89` | `0x0097BE50` | `554889e541574156` |
| `DcRestoreBlur` | `restore-blur` | `0xDCFA292B` | `0x009FFF20` | `554889e5488d0df5` |
| `DcReverseLocator` | `reverse-locator` | `0x2711E149` | `0x00A05CB0` | `554889e541574156` |
| `DcRopeDisableCharacterCollision` | `rope-disable-character-collision` | `0xA49F8426` | `0x00A04340` | `554889e541574156` |
| `DcRopeEnableCharacterCollision` | `rope-enable-character-collision` | `0xA4A7B2E6` | `0x00A04270` | `554889e541574156` |
| `DcRotateLocatorAngle` | `rotate-locator-angle` | `0x8801D9D3` | `0x00A05090` | `554889e541574156` |
| `DcRotateLocatorLookat` | `rotate-locator-lookat` | `0xE36D7F8E` | `0x00A05620` | `554889e541574156` |
| `DcSay` | `say` | `0x235CFA87` | `0x00A01A80` | `baffffffffe99617` |
| `DcSayPreRoll` | `say-pre-roll` | `0x159DCDD2` | `0x00A019F0` | `554889e541565348` |
| `DcScanParticleDirectories` | `scan-particle-directories` | `0x058DE911` | `0x00A19360` | `554889e541574156` |
| `DcSelectBestSchemaSpawner` | `select-best-schema-spawner` | `0x4895DC59` | `0x00A10EB0` | `554889e541574156` |
| `DcSendEvent` | `send-event` | `0xB3189857` | `0x004C18F0` | `554889e541574156` |
| `DcSendSimpleEvent` | `send-simple-event` | `0x54CB0A8D` | `0x004C1CF0` | `554889e541574156` |
| `DcSetAnimationSpeed` | `set-animation-speed` | `0xAF4A581F` | `0x00397880` | `554889e541574156` |
| `DcSetBarrelFlowMultiplier` | `set-barrel-flow-multiplier` | `0x3FF04910` | `0x00A01EF0` | `31c0c3666666662e` |
| `DcSetBlackFade` | `set-black-fade` | `0x57505A6E` | `0x00A10700` | `554889e5534883ec` |
| `DcSetBoolean` | `set-boolean` | `0xBA711E0D` | `0x00A42060` | `554889e541574156` |
| `DcSetCharacterWetMask` | `set-character-wet-mask` | `0x664CFB37` | `0x00A02550` | `554889e541574156` |
| `DcSetCharacterWetMaskState` | `set-character-wet-mask-state` | `0xBD0C58B9` | `0x00A02690` | `554889e5535085ff` |
| `DcSetCharacterWetMaskValue` | `set-character-wet-mask-value` | `0x591FFCD9` | `0x00A026E0` | `554889e5535085ff` |
| `DcSetClockMultiplier` | `set-clock-multiplier` | `0x2607C5F4` | `0x00A0EE60` | `554889e541574156` |
| `DcSetCollisionLayer` | `set-collision-layer` | `0x1B6BB746` | `0x00A03CD0` | `554889e541574156` |
| `DcSetCoverPushBackDist` | `set-cover-push-back-dist` | `0x47A854A4` | `0x0090B060` | `554889e541565348` |
| `DcSetCurrentBySpawner` | `set-current-by-spawner` | `0xB78DDD1A` | `0x009FD710` | `554889e5534883ec` |
| `DcSetDefaultFollowCameraSettings` | `set-default-follow-camera-settings` | `0x38F67D8D` | `0x00A0EBF0` | `554889e54883ec10` |
| `DcSetDeformationOfWaterUnit` | `set-deformation-of-water-unit` | `0xB010691C` | `0x00A02CC0` | `554889e5535083ff` |
| `DcSetDynamicLightColor` | `set-dynamic-light-color` | `0xA1D959C1` | `0x009FEE20` | `554889e541574156` |
| `DcSetE3LoadingScreen` | `set-e-3-loading-screen` | `0xEB9B8D2F` | `0x009FD5B0` | `31c0c3666666662e` |
| `DcSetFloat` | `set-float` | `0xC1BC5183` | `0x00A42630` | `554889e541574156` |
| `DcSetFocusableGrappled` | `set-focusable-grappled` | `0xBF697B95` | `0x00A0F2C0` | `554889e541574156` |
| `DcSetFocusableIgnore` | `set-focusable-ignore` | `0x472480C9` | `0x00A0F3C0` | `554889e541574156` |
| `DcSetForceTension` | `set-force-tension` | `0x58070796` | `0x00667EE0` | `554889e5833d916a` |
| `DcSetHarvestingEnabled` | `set-harvesting-enabled` | `0x6C742763` | `0x00A01B00` | `85ff7e0d833e0048` |
| `DcSetHarvestingPriority` | `set-harvesting-priority` | `0x39ACE3D6` | `0x00A01AC0` | `85ff7e180fb606b1` |
| `DcSetHealthParameters` | `set-health-parameters` | `0x572D826C` | `0x002DAAA0` | `554889e541574156` |
| `DcSetIceCaveStupidGearsHack` | `set-ice-cave-stupid-gears-hack` | `0xF1A86EF0` | `0x009FCC50` | `554889e5534883ec` |
| `DcSetInt32` | `set-int-32` | `0x55A43C77` | `0x00A42350` | `554889e541574156` |
| `DcSetJetskiFlowMultiplier` | `set-jetski-flow-multiplier` | `0x4EA4FF7B` | `0x00A01B70` | `31c0c3666666662e` |
| `DcSetJetskiMaxThrottle` | `set-jetski-max-throttle` | `0x7D981505` | `0x00A01B60` | `31c0c3666666662e` |
| `DcSetJetskiNearStupidRampInRuins2` | `set-jetski-near-stupid-ramp-in-ruins-2` | `0x6877F1BB` | `0x00A01B50` | `31c0c3666666662e` |
| `DcSetLightFov` | `set-light-fov` | `0xBDA58A56` | `0x009FFC80` | `554889e541574156` |
| `DcSetLookatStopPhase` | `set-lookat-stop-phase` | `0xE63BAFC6` | `0x00397320` | `554889e541574156` |
| `DcSetMaxCharacterWetness` | `set-max-character-wetness` | `0xCBAA0C97` | `0x00A02510` | `554889e5535085ff` |
| `DcSetMeshVisible` | `set-mesh-visible` | `0x83A68745` | `0x00A0BA20` | `554889e541574156` |
| `DcSetMinFallDist` | `set-min-fall-dist` | `0xDEA27032` | `0x0090E660` | `554889e54883ec10` |
| `DcSetMixerOverride` | `set-mixer-override` | `0xB4EE8F1D` | `0x009FD3B0` | `554889e5535085ff` |
| `DcSetMoveMode` | `set-move-mode` | `0x7F4EEFA5` | `0x0090D4F0` | `554889e541574156` |
| `DcSetObjectFaction` | `set-object-faction` | `0xDDE27074` | `0x009FCF40` | `554889e541565349` |
| `DcSetObjectHealth` | `set-object-health` | `0xCF2E79AE` | `0x00A04A20` | `554889e541574156` |
| `DcSetOffBalanceBySpawner` | `set-off-balance-by-spawner` | `0x8694E92A` | `0x008E9160` | `554889e5534883ec` |
| `DcSetPlayerCanOnlyDrawPistol` | `set-player-can-only-draw-pistol` | `0xE6230053` | `0x0090B9E0` | `554889e5534883ec` |
| `DcSetPlayerClimbSpawner` | `set-player-climb-spawner` | `0x04E41EBC` | `0x0090E540` | `554889e54883ec10` |
| `DcSetPlayerHealth` | `set-player-health` | `0x892E5D97` | `0x009100F0` | `554889e5488d0d25` |
| `DcSetPlayerInCombat` | `set-player-in-combat` | `0x498067E7` | `0x0090DB40` | `554889e5534881ec` |
| `DcSetPlayerInfiniteAmmo` | `set-player-infinite-ammo` | `0xAB1B6459` | `0x0090BA80` | `554889e5534883ec` |
| `DcSetPlayerInvincible` | `set-player-invincible` | `0x024B9BE2` | `0x0090BB10` | `554889e5534883ec` |
| `DcSetPlayerJumpSpawner` | `set-player-jump-spawner` | `0xEC27D02A` | `0x0090EF70` | `554889e54883ec10` |
| `DcSetPlayerMaxFallHeight` | `set-player-max-fall-height` | `0x80BAFE8C` | `0x00906EE0` | `554889e5534883ec` |
| `DcSetPlayerNoIgcKillGrenade` | `set-player-no-igc-kill-grenade` | `0xF1B16E3A` | `0x0090E910` | `554889e5534883ec` |
| `DcSetPlayerOnCollapsingBuilding` | `set-player-on-collapsing-building` | `0xCE9461C8` | `0x0090F0F0` | `554889e5534883ec` |
| `DcSetRagdollFlow` | `set-ragdoll-flow` | `0x8B588B25` | `0x009FD5C0` | `554889e553504889` |
| `DcSetRainRipplesState` | `set-rain-ripples-state` | `0x85D0299E` | `0x00A027F0` | `554889e5535085ff` |
| `DcSetRainState` | `set-rain-state` | `0xB71A0088` | `0x00A02830` | `554889e541574156` |
| `DcSetReloadPistolIfEmpty` | `set-reload-pistol-if-empty` | `0x1F4159DD` | `0x0090B940` | `554889e5534883ec` |
| `DcSetRenderSettings` | `set-render-settings` | `0xFAB05D37` | `0x009FFF80` | `554889e541574156` |
| `DcSetReverseJumpEnable` | `set-reverse-jump-enable` | `0x835B6FE2` | `0x0090D140` | `554889e5534883ec` |
| `DcSetRunType` | `set-run-type` | `0xEDE11587` | `0x0090D1F0` | `554889e541574156` |
| `DcSetShaderInstanceParam` | `set-shader-instance-param` | `0x34301F63` | `0x00A107B0` | `554889e541574156` |
| `DcSetShaderMaterialParam` | `set-shader-material-param` | `0x5C817DC0` | `0x00A10DD0` | `554889e541574156` |
| `DcSetSingleShaderInstanceParam` | `set-single-shader-instance-param` | `0x48059F73` | `0x00A10A50` | `554889e541574156` |
| `DcSetSplineFollowAcceleration` | `set-spline-follow-acceleration` | `0x7D6C9090` | `0x00A25CB0` | `554889e541574156` |
| `DcSetSplineFollowDistance` | `set-spline-follow-distance` | `0x490DC812` | `0x00A25C00` | `554889e541574156` |
| `DcSetSplineFollowSpeed` | `set-spline-follow-speed` | `0x26344B84` | `0x00A25D60` | `554889e541574156` |
| `DcSetSwimDragMultiplier` | `set-swim-drag-multiplier` | `0xB1DA0D91` | `0x0090D650` | `554889e583ff0175` |
| `DcSetSymbol` | `set-symbol` | `0x01D011B2` | `0x00A42910` | `554889e541574156` |
| `DcSetTableShaderMaterialValuesParam` | `set-table-shader-material-values-param` | `0xDF146CA7` | `0x009F3B70` | `554889e541574156` |
| `DcSetTargetSpawnerAngle` | `set-target-spawner-angle` | `0x174C4F7E` | `0x0090FE30` | `554889e541565348` |
| `DcSetTimeToDryPeriod` | `set-time-to-dry-period` | `0x6D2B84BA` | `0x00A02780` | `554889e5535085ff` |
| `DcSetVisible` | `set-visible` | `0x4E0D08CA` | `0x00A0BD30` | `554889e541574156` |
| `DcSetWalkMode` | `set-walk-mode` | `0x8855ACDB` | `0x0090D4B0` | `554889e5488b05bd` |
| `DcSetWalkType` | `set-walk-type` | `0x16EA1A1D` | `0x0090D350` | `554889e541574156` |
| `DcSetWaterOffset` | `set-water-offset` | `0xD89AAACD` | `0x00A02FF0` | `554889e541574156` |
| `DcSetWaterState` | `set-water-state` | `0xF0A972D0` | `0x00A02E20` | `554889e5535083ff` |
| `DcSetWhiteFade` | `set-white-fade` | `0x6F684F90` | `0x00A106A0` | `554889e5534883ec` |
| `DcShowBinoculars` | `show-binoculars` | `0x5AAF0C76` | `0x00A0CB90` | `488d057134720048` |
| `DcShutUp` | `shut-up` | `0x60683CCB` | `0x00A011E0` | `554889e541574156` |
| `DcSignal` | `signal` | `0x64B0F52E` | `0x00A40480` | `554889e541574156` |
| `DcSignalRaisedP` | `signal-raised?` | `0xBB99C26A` | `0x00A40220` | `554889e541574156` |
| `DcSlaForceTackle` | `sla-force-tackle` | `0x6A3EC659` | `0x00A0F4A0` | `554889e5534883ec` |
| `DcSlowMoEffect` | `slow-mo-effect` | `0x503B595E` | `0x00A0EE10` | `554889e54889f0c5` |
| `DcSpawnAttachable` | `spawn-attachable` | `0xA028680C` | `0x00A0F540` | `554889e541574156` |
| `DcSpawnEntity` | `spawn-entity` | `0xF43DB80A` | `0x00A09F40` | `554889e541574156` |
| `DcSpawnEntityAt` | `spawn-entity-at` | `0x7417FCD6` | `0x009F3900` | `554889e541574156` |
| `DcSpawnExplosion` | `spawn-explosion` | `0x6B4E7ABD` | `0x009FF610` | `554889e541574156` |
| `DcSpawnExplosionNpcOnly` | `spawn-explosion-npc-only` | `0x68F64E3C` | `0x009FF410` | `554889e541574156` |
| `DcSpawnFloatingBarrelAtPhase` | `spawn-floating-barrel-at-phase` | `0x6BF42AF8` | `0x00A03E20` | `554889e541574156` |
| `DcSpawnMessage` | `spawn-message` | `0xB69D2CEF` | `0x005DC7B0` | `554889e541565348` |
| `DcSpawnParticlesAtJoint` | `spawn-particles-at-joint` | `0xB507CCD3` | `0x00A00950` | `ba02000000e92625` |
| `DcSpawnParticlesAtLocator` | `spawn-particles-at-locator` | `0x532A47BA` | `0x00A002D0` | `554889e541574156` |
| `DcSpawnParticlesAttached` | `spawn-particles-attached` | `0xBB206306` | `0x00A00960` | `ba01000000e91625` |
| `DcSpawnScript` | `spawn-script` | `0x849A7D83` | `0x00A44460` | `554889e541574156` |
| `DcSpawnSnowDisturb` | `spawn-snow-disturb` | `0x8BC6F669` | `0x00A01BB0` | `554889e541574156` |
| `DcSplineActivate` | `spline-activate` | `0xC61B9C4E` | `0x00A26180` | `554889e541574156` |
| `DcSplineDistanceLocator` | `spline-distance-locator` | `0x817AEAE5` | `0x00A25F60` | `554889e541574156` |
| `DcSplineDistanceSpawner` | `spline-distance-spawner` | `0x4D284EEE` | `0x00A260E0` | `554889e541574156` |
| `DcSplineDistanceVehicleAnimation` | `spline-distance-vehicle-animation` | `0x281D8C8A` | `0x00AB8640` | `554889e541574156` |
| `DcSplineLengthP` | `spline-length?` | `0x746C2638` | `0x00A256B0` | `554889e54883ec10` |
| `DcSplineLocatorAtDistance` | `spline-locator-at-distance` | `0xB44C7CAC` | `0x00EA2315` | `704672616d650044` |
| `DcSplineTransferPendingP` | `spline-transfer-pending?` | `0x03FB7B87` | `0x00A25A20` | `554889e5535085ff` |
| `DcSsError` | `ss-error` | `0x77F4FEFA` | `0x00A44670` | `554889e5534881ec` |
| `DcStartMovieAudio` | `start-movie-audio` | `0x0CD096E7` | `0x009FE0D0` | `554889e5488d0505` |
| `DcStartRumble` | `start-rumble` | `0x35140815` | `0x009FF3D0` | `554889e5535085ff` |
| `DcStateCharacterAccumulatesSnowFromGround` | `state-character-accumulates-snow-from-ground` | `0x8532A3E0` | `0x00A01FE0` | `554889e5535085ff` |
| `DcStateCharacterAccumulatesSnowFromSky` | `state-character-accumulates-snow-from-sky` | `0xB3F0137E` | `0x00A01F70` | `554889e5535083ff` |
| `DcStateCharacterAccumulatesWetnessFromWater` | `state-character-accumulates-wetness-from-water` | `0x5DDB29D7` | `0x00A01F30` | `554889e5535085ff` |
| `DcStickySpawner` | `sticky-spawner` | `0xC6071DF8` | `0x00A0B8F0` | `554889e541574156` |
| `DcStopAnimating` | `stop-animating` | `0xD9CB0C6D` | `0x00397AB0` | `554889e541574156` |
| `DcStopRumble` | `stop-rumble` | `0xCF53AF61` | `0x009FF370` | `554889e5535085ff` |
| `DcStopScriptRumbles` | `stop-script-rumbles` | `0xE59BA17D` | `0x00A105B0` | `b835000000488d0d` |
| `DcStopSound` | `stop-sound` | `0x6F26C192` | `0x00A01160` | `554889e541565385` |
| `DcStopSpeaking` | `stop-speaking` | `0x2279D61E` | `0x00A01570` | `554889e541574156` |
| `DcStringFromFloat` | `string-from-float` | `0x5A5528D0` | `0x009FDE60` | `554889e55350c5f8` |
| `DcStringFromStringId` | `string-from-string-id` | `0x1B2ADEA7` | `0x009FDE30` | `85ff7e098b3e31f6` |
| `DcStringIdFromString` | `string-id-from-string` | `0x9E1773DC` | `0x009FDE00` | `554889e531c085ff` |
| `DcSullivanFlashlightToHand` | `sullivan-flashlight-to-hand` | `0x59A3DA2C` | `0x00A0A6A0` | `488d0d6951640048` |
| `DcSullivanFlashlightToWaist` | `sullivan-flashlight-to-waist` | `0x11799CD1` | `0x00A0A5F0` | `488d0d1952640048` |
| `DcSuppressIdleSpeech` | `suppress-idle-speech` | `0xA2C6B659` | `0x00A01360` | `554889e54883ec10` |
| `DcSwapEntity` | `swap-entity` | `0x326EF848` | `0x00A09590` | `554889e541574156` |
| `DcSwimAllowStandOnBottom` | `swim-allow-stand-on-bottom` | `0x5542401B` | `0x0090D6B0` | `554889e5534883ec` |
| `DcSwimDisable` | `swim-disable` | `0x2C5C5A4A` | `0x0090D760` | `554889e5e8d76801` |
| `DcSwimDisallowStandOnBottom` | `swim-disallow-stand-on-bottom` | `0x315FB50B` | `0x0090D680` | `554889e5488d055d` |
| `DcSwimEnable` | `swim-enable` | `0x4CBBC128` | `0x0090D770` | `554889e5e8c76901` |
| `DcSymbolEqualP` | `symbol-equal?` | `0x6CDE9101` | `0x00397620` | `8b063b46080f94c0` |
| `DcSyncAudioMetaData` | `sync-audio-meta-data` | `0x6D35F28D` | `0x009FD560` | `554889e5488d05e5` |
| `DcSyncVoxMetaData` | `sync-vox-meta-data` | `0x7A44FC2A` | `0x009FD1F0` | `554889e531ffe8c5` |
| `DcSyncVoxMpMetaData` | `sync-vox-mp-meta-data` | `0x41BF1CAF` | `0x009FD1E0` | `554889e5e837619e` |
| `DcTagBoolean` | `tag-boolean` | `0x5EF1F0DB` | `0x00A43700` | `554889e541574156` |
| `DcTagFloat` | `tag-float` | `0x4BF01CBF` | `0x00A439D0` | `554889e541574156` |
| `DcTagInt32` | `tag-int-32` | `0x5F6F364F` | `0x00A43880` | `554889e541574156` |
| `DcTagSpecifiedP` | `tag-specified?` | `0xD510E933` | `0x00A43DE0` | `554889e541574156` |
| `DcTagString` | `tag-string` | `0xC489284C` | `0x00A43B40` | `554889e541574156` |
| `DcTagSymbol` | `tag-symbol` | `0x0B1B1B8A` | `0x00A43C90` | `554889e541574156` |
| `DcTankAimAtObj` | `tank-aim-at-obj` | `0xE36719FE` | `0x00AA2AC0` | `554889e54883ec10` |
| `DcTankAimAtPos` | `tank-aim-at-pos` | `0x868E7A7B` | `0x00AA2E90` | `554889e54883ec10` |
| `DcTankAimAtPosInstant` | `tank-aim-at-pos-instant` | `0x1D39C0C0` | `0x00AA2B10` | `554889e54883ec10` |
| `DcTankAimForward` | `tank-aim-forward` | `0x13D24475` | `0x00AA2A70` | `554889e54883ec10` |
| `DcTankDistToPoint` | `tank-dist-to-point` | `0x830C56D0` | `0x00AA1300` | `554889e541574156` |
| `DcTankFindBestTargetMP` | `tank-find-best-target-mp` | `0xB22210CE` | `0x00AA4950` | `554889e541574156` |
| `DcTankFire` | `tank-fire` | `0x0A96C6E3` | `0x00AA2930` | `554889e541574156` |
| `DcTankFireAtObj` | `tank-fire-at-obj` | `0x9FD56E3A` | `0x00AA2EE0` | `554889e54883ec10` |
| `DcTankFireAtPos` | `tank-fire-at-pos` | `0xFA3C0DBF` | `0x00AA2F30` | `554889e54883ec10` |
| `DcTankForceFireCannon` | `tank-force-fire-cannon` | `0x0FB6411A` | `0x00AA42C0` | `554889e541574156` |
| `DcTankGetHealth` | `tank-get-health` | `0x2666750F` | `0x00AA3D80` | `554889e541574156` |
| `DcTankMgCeaseFire` | `tank-mg-cease-fire` | `0x99B74C7C` | `0x00AA2000` | `554889e54883ec10` |
| `DcTankMgFireAtLoc` | `tank-mg-fire-at-loc` | `0x66E6E69A` | `0x00AA2120` | `554889e54883ec10` |
| `DcTankMgFireAtObj` | `tank-mg-fire-at-obj` | `0x5B33403B` | `0x00AA2680` | `554889e54883ec10` |
| `DcTankMoveTo` | `tank-move-to` | `0x95A702CF` | `0x00AA33E0` | `554889e54883ec10` |
| `DcTankMoveTrackingObject` | `tank-move-tracking-object` | `0xE30E0949` | `0x00AA1550` | `554889e54883ec10` |
| `DcTankReverseTo` | `tank-reverse-to` | `0xAFEAD71C` | `0x00AA2F80` | `554889e541574156` |
| `DcTankSetHealth` | `tank-set-health` | `0x02A42435` | `0x00AA3CC0` | `554889e541574156` |
| `DcTankTeleport` | `tank-teleport` | `0x932ED089` | `0x00AA1BA0` | `554889e54883ec10` |
| `DcTankWaitTeleport` | `tank-wait-teleport` | `0xB4E2D175` | `0x00AA4420` | `554889e541574156` |
| `DcTaskElapsedTime` | `task-elapsed-time` | `0x9B53A1B0` | `0x00A102B0` | `554889e541574156` |
| `DcTaskFailed` | `task-failed` | `0x1B805494` | `0x009FE900` | `554889e541574156` |
| `DcTaskFind` | `task-find` | `0x32EA7B74` | `0x009FEB90` | `554889e541574156` |
| `DcTaskNodeActiveP` | `task-node-active?` | `0x8CE160DB` | `0x00A10340` | `554889e541574156` |
| `DcTaskSubnodeActiveP` | `task-subnode-active?` | `0x1A0506FC` | `0x009FEC80` | `554889e5488b3ee8` |
| `DcTaskSubnodeComplete` | `task-subnode-complete` | `0xD4050C6E` | `0x009FECA0` | `554889e541565348` |
| `DcTaskSubnodePlay` | `task-subnode-play` | `0xC1A6D2BC` | `0x009FEC00` | `554889e5534883ec` |
| `DcThrowGrenade` | `throw-grenade` | `0x6333DA76` | `0x002E2C30` | `554889e541574156` |
| `DcThrowSlaGrenade` | `throw-sla-grenade` | `0x0EC5CFC6` | `0x002E2810` | `554889e541574156` |
| `DcTrainAllowUnphysicalize` | `train-allow-unphysicalize` | `0x8C31728A` | `0x00A68430` | `554889e541574156` |
| `DcTrainDecouple` | `train-decouple` | `0xFA24ECD8` | `0x00A67E40` | `554889e541574156` |
| `DcTrainGetAcceleration` | `train-get-acceleration` | `0xE43BDB88` | `0x00A68230` | `554889e541565348` |
| `DcTrainGetDefaultAcceleration` | `train-get-default-acceleration` | `0x4A413880` | `0x00A682E0` | `48c74424f8000000` |
| `DcTrainGetDistance` | `train-get-distance` | `0xE0D61234` | `0x00A67B90` | `554889e541565348` |
| `DcTrainGetHeadCar` | `train-get-head-car` | `0xB9AD92C8` | `0x00A67C40` | `554889e541565385` |
| `DcTrainGetSpline` | `train-get-spline` | `0xFC1B8E09` | `0x00A67AE0` | `554889e541565385` |
| `DcTrainKillAllRiders` | `train-kill-all-riders` | `0x705684E8` | `0x00A68530` | `554889e541574156` |
| `DcTrainSetAcceleration` | `train-set-acceleration` | `0xA81CEFAE` | `0x00A68140` | `554889e541574156` |
| `DcTrainSetSpeedMph` | `train-set-speed-mph` | `0xD4F03954` | `0x00A67F60` | `554889e541574156` |
| `DcTrainSpawn` | `train-spawn` | `0xCBF5C031` | `0x00A67960` | `554889e541574156` |
| `DcTrainSpawnOnTrack` | `train-spawn-on-track` | `0x3D05016C` | `0x00A68740` | `554889e541574156` |
| `DcTrainTeleport` | `train-teleport` | `0x9D5456E8` | `0x00A67CD0` | `554889e541574156` |
| `DcTrainTrackPlayer` | `train-track-player` | `0x011A0490` | `0x00A68300` | `554889e541574156` |
| `DcTrainTransferPendingP` | `train-transfer-pending?` | `0xC14382CE` | `0x00A685E0` | `554889e541574156` |
| `DcTransferToSpline` | `transfer-to-spline` | `0xF515BD98` | `0x00A25A90` | `554889e541574156` |
| `DcTranslateYLoop` | `translate-y-loop` | `0x913BC948` | `0x009FC590` | `554889e541574156` |
| `DcTurnOnFlashlight` | `turn-on-flashlight` | `0x48B1B449` | `0x00A0A250` | `554889e541574156` |
| `DcTutorialHintsOnP` | `tutorial-hints-on?` | `0xDF85EA60` | `0x00A0F490` | `488d05a9a1640048` |
| `DcUnblockNavPolyUnderObject` | `unblock-nav-poly-under-object` | `0x7E5AD594` | `0x00A04FC0` | `554889e541565385` |
| `DcUnhideMessage` | `unhide-message` | `0xE40CA42A` | `0x005DCAF0` | `554889e54883ec10` |
| `DcUnhideMessageDeprecated` | `unhide-message-deprecated` | `0xB2127739` | `0x005DC700` | `554889e54883ec10` |
| `DcUnitsPerFrame` | `units-per-frame` | `0x319F4E3C` | `0x00A0ECF0` | `85ff7e4dc5fa1006` |
| `DcUnitsPerSecond` | `units-per-second` | `0x0EEC468D` | `0x00A0EC70` | `85ff7e5fc5fa1006` |
| `DcUnparent` | `unparent` | `0x71977AEC` | `0x00A03940` | `554889e541565385` |
| `DcUnstickySpawner` | `unsticky-spawner` | `0xF8717A0A` | `0x00A0B7C0` | `554889e541574156` |
| `DcUpdateApRef` | `update-ap-ref` | `0x6D7DD92D` | `0x00397520` | `554889e541574156` |
| `DcUpdateDynamicLight` | `update-dynamic-light` | `0x2F5DC938` | `0x009FEEE0` | `554889e541574156` |
| `DcUpdateFightRef` | `update-fight-ref` | `0x7347F3A6` | `0x00667F80` | `554889e541574156` |
| `DcUpdateTaskMenu` | `update-task-menu` | `0x5047B2C8` | `0x009FE8C0` | `554889e5488b3dad` |
| `DcUseAudioRainLevel` | `use-audio-rain-level` | `0x00C11B61` | `0x00A009F0` | `554889e5535085ff` |
| `DcUseMaterialFromWaterUnit` | `use-material-from-water-unit` | `0x8589A890` | `0x00A02020` | `554889e541565383` |
| `DcUseRainIntensityValues` | `use-rain-intensity-values` | `0x8FCBB113` | `0x00A031C0` | `554889e5535083ff` |
| `DcUseReflectionPlane` | `use-reflection-plane` | `0x8F0342EB` | `0x00A029A0` | `554889e5535085ff` |
| `DcUseReflections` | `use-reflections` | `0x91155CD8` | `0x00A02C10` | `554889e5535085ff` |
| `DcUseVisibleWaterUnitForReflection` | `use-visible-water-unit-for-reflection` | `0x23D4A292` | `0x00A02BC0` | `554889e5535085ff` |
| `DcUseVisibleWaterUnitForReflectionWithTolerance` | `use-visible-water-unit-for-reflection-with-tolerance` | `0x969C4BC6` | `0x00A02B70` | `554889e5535083ff` |
| `DcUseWaterUnitForReflection` | `use-water-unit-for-reflection` | `0x6FFB28A3` | `0x00A02C70` | `554889e5535083ff` |
| `DcUseWaterUnitForReflectionIfVisible` | `use-water-unit-for-reflection-if-visible` | `0xE75D4472` | `0x00A02C50` | `488d05291d7600c6` |
| `DcUseWindIntensityValues` | `use-wind-intensity-values` | `0xDD75C117` | `0x00A03290` | `554889e541565348` |
| `DcWait` | `wait` | `0x568EAB14` | `0x00A3E690` | `554889e541574156` |
| `DcWaitAnimate` | `wait-animate` | `0x933344D6` | `0x003969A0` | `554889e541574156` |
| `DcWaitBoolean` | `wait-boolean` | `0x9C2425A8` | `0x00A41610` | `554889e541574156` |
| `DcWaitFor` | `wait-for` | `0xCA1359A3` | `0x00A3FC50` | `554889e541574156` |
| `DcWaitForNpcNotBusy` | `wait-for-npc-not-busy` | `0x83ADC6C2` | `0x002DF6A0` | `554889e541574156` |
| `DcWaitFrames` | `wait-frames` | `0x6E22F8D5` | `0x00A41270` | `554889e541574156` |
| `DcWaitHeliFireMissile` | `wait-heli-fire-missile` | `0x203DF587` | `0x00A7EE30` | `554889e541574156` |
| `DcWaitHeliMoveTo` | `wait-heli-move-to` | `0xCD5F5D97` | `0x00A7FA70` | `554889e541574156` |
| `DcWaitHeliMoveToSpline` | `wait-heli-move-to-spline` | `0x83064E7E` | `0x00A7F800` | `554889e541574156` |
| `DcWaitInt32` | `wait-int-32` | `0xB154C41C` | `0x00A41D40` | `554889e541574156` |
| `DcWaitNpcBind` | `wait-npc-bind` | `0xA011225A` | `0x002E1E80` | `554889e541574156` |
| `DcWaitNpcLeaveScriptedAp` | `wait-npc-leave-scripted-ap` | `0xFD09B97F` | `0x002DDD20` | `554889e541574156` |
| `DcWaitNpcMoveTo` | `wait-npc-move-to` | `0x9240B713` | `0x002E15F0` | `4889d0ba01000000` |
| `DcWaitNpcMoveToSpawner` | `wait-npc-move-to-spawner` | `0x8E74A4E8` | `0x002E0F10` | `4889d0ba01000000` |
| `DcWaitNpcTurnTo` | `wait-npc-turn-to` | `0xA86441C8` | `0x002DF9A0` | `554889e541574156` |
| `DcWaitNpcUseClosestCover` | `wait-npc-use-closest-cover` | `0xCD422D55` | `0x002E4130` | `554889e541574156` |
| `DcWaitNpcUseScriptedAp` | `wait-npc-use-scripted-ap` | `0x99E4D4D8` | `0x002DDF20` | `554889e541574156` |
| `DcWaitOneFrame` | `wait-one-frame` | `0xB7ECEEDC` | `0x00A40F70` | `554889e541574156` |
| `DcWaitPlayerMoveTo` | `wait-player-move-to` | `0x061AD4D4` | `0x00A0B350` | `554889e541574156` |
| `DcWaitPlayerTurnTo` | `wait-player-turn-to` | `0x3C3E220F` | `0x00A0AFC0` | `554889e541574156` |
| `DcWaitSay` | `wait-say` | `0x95F6B8DD` | `0x00A016D0` | `554889e541574156` |
| `DcWaitSpawnEntity` | `wait-spawn-entity` | `0xFD166E79` | `0x00A09E90` | `554889e541574156` |
| `DcWaitSpawnEntityAt` | `wait-spawn-entity-at` | `0xDB171895` | `0x00A09DC0` | `554889e541574156` |
| `DcWaitSplineDistance` | `wait-spline-distance` | `0x98B7BBFC` | `0x00A25740` | `554889e541574156` |
| `DcWaitTankAimAtObj` | `wait-tank-aim-at-obj` | `0x4C67FDBD` | `0x00AA3A40` | `554889e54883ec20` |
| `DcWaitTankAimAtPos` | `wait-tank-aim-at-pos` | `0x298E9E38` | `0x00AA3AA0` | `554889e54883ec20` |
| `DcWaitTankAimForward` | `wait-tank-aim-forward` | `0xDA56ECC6` | `0x00AA3430` | `554889e54883ec20` |
| `DcWaitTankFireAtObj` | `wait-tank-fire-at-obj` | `0x5651C689` | `0x00AA3B00` | `554889e54883ec20` |
| `DcWaitTankFireAtPos` | `wait-tank-fire-at-pos` | `0x33B8A50C` | `0x00AA3B60` | `554889e54883ec20` |
| `DcWaitTankMoveTo` | `wait-tank-move-to` | `0x9C8CD4BC` | `0x00AA3C60` | `554889e54883ec20` |
| `DcWaitTankReverseTo` | `wait-tank-reverse-to` | `0x00EA335F` | `0x00AA3BC0` | `554889e541574156` |
| `DcWaitUntil` | `wait-until` | `0xD1E7C609` | `0x00A40E60` | `554889e541574156` |
| `DcWaitUntilFrame` | `wait-until-frame` | `0x19D1CE7D` | `0x00A40D40` | `554889e541574156` |
| `DcWaitUntilNetTime` | `wait-until-net-time` | `0xBC239ABE` | `0x00A40870` | `554889e541574156` |
| `DcWeaponSetReset` | `weapon-set-reset` | `0x212F3614` | `0x0090AD00` | `554889e5bf4e0e9d` |
| `DcXSpawnNpcAt` | `x-spawn-npc-at` | `0x1E7DFD23` | `0x00A64C40` | `554889e5534883ec` |
| `DcY6uLSA` | `y-6-u-lsa` | `0x73D1A78D` | `0x00000000` | `2f6c696265786563` |
