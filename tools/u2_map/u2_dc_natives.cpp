// ============================================================================
// Uncharted 2: Among Thieves (big2-ps4_Shipping.elf)
// Decompiled C++ Native Script Functions (Dc*)
// Calling Convention: int64_t DcFunc(int32_t argc, uint64_t *argv, uint8_t *image)
// ============================================================================

#include <stdint.h>
#include <stddef.h>

// Lisp: (action-pack-enabled? ...) | StringId: 0x5C28E86B | Guest Offset: 0x00A09FF0
int64_t DcActionPackEnabledP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 89 F0 89 FA 48 8B 1D FB 0B 64 00 48 8B 0B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A09FF0ull))(argc, argv);
}

// Lisp: (active-player-count ...) | StringId: 0xC27AD2F6 | Guest Offset: 0x00667F40
int64_t DcActivePlayerCount(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 3D 15 38 AD 00 E8 60 80 FD FF 48 98 5D C3 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00667F40ull))(argc, argv);
}

// Lisp: (add-dynamic-light ...) | StringId: 0xC639AD71 | Guest Offset: 0x009FF2C0
int64_t DcAddDynamicLight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 20 49 89 F7 89 FB 4C 8B 25 25
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FF2C0ull))(argc, argv);
}

// Lisp: (add-dynamic-light-joint ...) | StringId: 0x92509D49 | Guest Offset: 0x009FF010
int64_t DcAddDynamicLightJoint(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC A8 00 00 00 49 89 F6 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FF010ull))(argc, argv);
}

// Lisp: (add-import-module ...) | StringId: 0xC66A1CE6 | Guest Offset: 0x00A0FF10
int64_t DcAddImportModule(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D E0 AC 63 00 48 8B 03 48 89 45 F0 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0FF10ull))(argc, argv);
}

// Lisp: (add-load-module ...) | StringId: 0xE0E51DAF | Guest Offset: 0x00A0FE90
int64_t DcAddLoadModule(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 60 AD 63 00 48 8B 03 48 89 45 F0 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0FE90ull))(argc, argv);
}

// Lisp: (alive-player-count ...) | StringId: 0xC1D452C4 | Guest Offset: 0x00667F20
int64_t DcAlivePlayerCount(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 3D 35 38 AD 00 E8 50 AB FD FF 48 98 5D C3 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00667F20ull))(argc, argv);
}

// Lisp: (all-enemy-npcs-die ...) | StringId: 0x24B4ACF3 | Guest Offset: 0x002DE9D0
int64_t DcAllEnemyNpcsDie(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 3D D5 55 E8 00 48 8D 15 C6 1F B9 00 BE 01 00 00 00 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DE9D0ull))(argc, argv);
}

// Lisp: (allow-camera-cut-compensate ...) | StringId: 0xFAA7265B | Guest Offset: 0x00A0DD40
int64_t DcAllowCameraCutCompensate(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D B0 CE 63 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0DD40ull))(argc, argv);
}

// Lisp: (allow-leg-ik-by-others ...) | StringId: 0x7DF5B4E3 | Guest Offset: 0x009FCDF0
int64_t DcAllowLegIkByOthers(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 89 FB 85 DB 0F 8E EF 00 00 00 44 8B 3E 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FCDF0ull))(argc, argv);
}

// Lisp: (angle-mod ...) | StringId: 0x0B26368F | Guest Offset: 0x00397630
int64_t DcAngleMod(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 C5 FA 10 06 C5 FA 10 0D 4C 7F AC 00 E8 27 F8 A4
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00397630ull))(argc, argv);
}

// Lisp: (anim-state-advance-phase ...) | StringId: 0x2481D93E | Guest Offset: 0x00397690
int64_t DcAnimStateAdvancePhase(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (animate ...) | StringId: 0x84591781 | Guest Offset: 0x00397C50
int64_t DcAnimate(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D 91 F2 AD 00 31 D2 E9 22 E5 FF FF 66 90 48 C7 47 10 00 00 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00397C50ull))(argc, argv);
}

// Lisp: (apply-explosion-impulse ...) | StringId: 0xE693AB67 | Guest Offset: 0x00A03B20
int64_t DcApplyExplosionImpulse(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 48 89 F3 4C 8B 25 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A03B20ull))(argc, argv);
}

// Lisp: (attach-flashlight ...) | StringId: 0xC37FECC0 | Guest Offset: 0x00A0A4C0
int64_t DcAttachFlashlight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F6 89 FB 85 DB 7E 28 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0A4C0ull))(argc, argv);
}

// Lisp: (auto-save-game ...) | StringId: 0x85802E50 | Guest Offset: 0x009FE820
int64_t DcAutoSaveGame(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 4C 8B 35 CE C3 64 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE820ull))(argc, argv);
}

// Lisp: (battle-select-best-spawner-from-group ...) | StringId: 0x42365F84 | Guest Offset: 0x00A11190
int64_t DcBattleSelectBestSpawnerFromGroup(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC D8 00 00 00 48 89 B5 30
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A11190ull))(argc, argv);
}

// Lisp: (block-nav-poly-under-object ...) | StringId: 0x7BA8ED16 | Guest Offset: 0x00A05030
int64_t DcBlockNavPolyUnderObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 49 8B 1E 31 F6 89 DF E8 5A E3 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A05030ull))(argc, argv);
}

// Lisp: (broadcast-event-to-npcs ...) | StringId: 0x8679C923 | Guest Offset: 0x004C3A00
int64_t DcBroadcastEventToNpcs(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F6 41 89 FF 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C3A00ull))(argc, argv);
}

// Lisp: (c-spawn-npc ...) | StringId: 0x970C3E2C | Guest Offset: 0x002E57B0
int64_t DcCSpawnNpc(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 38 01 00 00 4C 8B 2D 35
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E57B0ull))(argc, argv);
}

// Lisp: (c-spawn-npc-s ...) | StringId: 0x7E1308AA | Guest Offset: 0x002E55A0
int64_t DcCSpawnNpcS(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 48 01 00 00 4C 8B 2D 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E55A0ull))(argc, argv);
}

// Lisp: (cache-music-playlist ...) | StringId: 0xDB22D0D6 | Guest Offset: 0x009FD460
int64_t DcCacheMusicPlaylist(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 5D 8B 1E 31 F6 89 DF E8 27 5F 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD460ull))(argc, argv);
}

// Lisp: (cache-sound-bank ...) | StringId: 0x17AE9A8B | Guest Offset: 0x009FD540
int64_t DcCacheSoundBank(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8B 36 48 8D 3D 02 42 71 00 BA 02 00 00 00 31 C9 E8 46 AA
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD540ull))(argc, argv);
}

// Lisp: (cache-sound-stream ...) | StringId: 0x617F9354 | Guest Offset: 0x009FD510
int64_t DcCacheSoundStream(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8B 06 48 8B 4E 08 48 8D 3D 2E 42 71 00 BA 03 00 00 00 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD510ull))(argc, argv);
}

// Lisp: (cache-speech-stream ...) | StringId: 0x03290A06 | Guest Offset: 0x009FD4E0
int64_t DcCacheSpeechStream(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8B 06 48 8B 4E 08 48 8D 3D 5E 42 71 00 BA 05 00 00 00 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD4E0ull))(argc, argv);
}

// Lisp: (calc-new-phase-from-speed ...) | StringId: 0x6E75BABE | Guest Offset: 0x00A0FBF0
int64_t DcCalcNewPhaseFromSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 89 F3 83 E7 FE 83 FF 04 74 35 48 8B 05 70 E3 B5 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0FBF0ull))(argc, argv);
}

// Lisp: (calc-start-phase ...) | StringId: 0xDFCFB620 | Guest Offset: 0x00A0FB70
int64_t DcCalcStartPhase(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 89 F3 83 FF 01 74 35 48 8B 05 F3 E3 B5 00 48 85 C0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0FB70ull))(argc, argv);
}

// Lisp: (calc-start-phase-max-frames ...) | StringId: 0x9AB7F010 | Guest Offset: 0x00A0FAF0
int64_t DcCalcStartPhaseMaxFrames(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 89 F3 83 FF 02 74 35 48 8B 05 73 E4 B5 00 48 85 C0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0FAF0ull))(argc, argv);
}

// Lisp: (calc-start-phase-range ...) | StringId: 0x353AC3EE | Guest Offset: 0x00A0FA70
int64_t DcCalcStartPhaseRange(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 89 F3 83 FF 03 74 35 48 8B 05 F3 E4 B5 00 48 85 C0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0FA70ull))(argc, argv);
}

// Lisp: (camera-abandon ...) | StringId: 0x02E9965F | Guest Offset: 0x00A0E5E0
int64_t DcCameraAbandon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 AA 4D 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0E5E0ull))(argc, argv);
}

// Lisp: (camera-aim-override-direction ...) | StringId: 0x3C3D70D3 | Guest Offset: 0x00A0DE40
int64_t DcCameraAimOverrideDirection(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 28 48 8B 1D B0 CD 63 00 48 8B 03 48 89 45 F0 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0DE40ull))(argc, argv);
}

// Lisp: (camera-aim-override-point ...) | StringId: 0xD57D36E9 | Guest Offset: 0x00A0DF40
int64_t DcCameraAimOverridePoint(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 4D 48 8B 06 48 85 C0 74 45 48 8B 48 08 48 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0DF40ull))(argc, argv);
}

// Lisp: (camera-animation-disable ...) | StringId: 0xD820968B | Guest Offset: 0x00A033B0
int64_t DcCameraAnimationDisable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 48 89 F3 48 8B 05 35
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A033B0ull))(argc, argv);
}

// Lisp: (camera-animation-enable ...) | StringId: 0xA4AC1C2F | Guest Offset: 0x00A03480
int64_t DcCameraAnimationEnable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F4 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A03480ull))(argc, argv);
}

// Lisp: (camera-cancel-aim-override ...) | StringId: 0x0603B595 | Guest Offset: 0x00A0DE30
int64_t DcCameraCancelAimOverride(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv;
    *(uint32_t *)(image + 0x0147F0BC) = 0x00000000u;
    return 1;
}

// Lisp: (camera-disable ...) | StringId: 0x29C9E722 | Guest Offset: 0x00A0E500
int64_t DcCameraDisable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 8A 4E 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0E500ull))(argc, argv);
}

// Lisp: (camera-disable-all ...) | StringId: 0xD754EF39 | Guest Offset: 0x00A0E330
int64_t DcCameraDisableAll(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8B 05 15 0D A7 00 48 8D 0D 0E 0D A7 00 EB 13 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0E330ull))(argc, argv);
}

// Lisp: (camera-enable ...) | StringId: 0xB71CA7A3 | Guest Offset: 0x00A0E680
int64_t DcCameraEnable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 F5 89 FB 85 DB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0E680ull))(argc, argv);
}

// Lisp: (camera-fade-out ...) | StringId: 0xB5E93ED4 | Guest Offset: 0x00A0E380
int64_t DcCameraFadeOut(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 F7 41 89 FC 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0E380ull))(argc, argv);
}

// Lisp: (camera-local-space ...) | StringId: 0xDCB3081B | Guest Offset: 0x00A0EA20
int64_t DcCameraLocalSpace(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 20 4C 8B 35 CE C1 63 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0EA20ull))(argc, argv);
}

// Lisp: (camera-locator ...) | StringId: 0xBA364A1C | Guest Offset: 0x00A128B0
int64_t DcCameraLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 40 83 63 00 48 8B 03 48 89 45 F0 C7
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A128B0ull))(argc, argv);
}

// Lisp: (camera-moved-backwards? ...) | StringId: 0xC31D3021 | Guest Offset: 0x00A0DC40
int64_t DcCameraMovedBackwardsP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 85 FF 7E 06 C5 FA 10 06 EB 08 C5 FA 10 05 62 97 45 00 48 8B 05 C7 13 A7
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0DC40ull))(argc, argv);
}

// Lisp: (camera-restore-locator ...) | StringId: 0xD40D2524 | Guest Offset: 0x00A0E9B0
int64_t DcCameraRestoreLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv;
    *(uint8_t *)(image + 0x0147F0B0) = 0x01u;
    return 0;
}

// Lisp: (camera-shake ...) | StringId: 0xF3819432 | Guest Offset: 0x00A0E040
int64_t DcCameraShake(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 38 49 89 F4 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0E040ull))(argc, argv);
}

// Lisp: (camera-shake-in-zoom-camera ...) | StringId: 0x7D1AC5EB | Guest Offset: 0x00A0DCA0
int64_t DcCameraShakeInZoomCamera(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 50 CF 63 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0DCA0ull))(argc, argv);
}

// Lisp: (camera-store-locator ...) | StringId: 0x291173D8 | Guest Offset: 0x00A0E9C0
int64_t DcCameraStoreLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D 41 58 70 00 48 8B 01 C5 F8 57 C0 C5 F8 28 0D 0A 20 44 00 48 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0E9C0ull))(argc, argv);
}

// Lisp: (choose-stumble-anim ...) | StringId: 0x4A5A1790 | Guest Offset: 0x00907370
int64_t DcChooseStumbleAnim(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC A8 01 00 00 4C 8B 2D 75
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00907370ull))(argc, argv);
}

// Lisp: (clear-harvested-bodies ...) | StringId: 0xCC24252E | Guest Offset: 0x00A01A90
int64_t DcClearHarvestedBodies(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 3D 05 C2 70 00 E8 E0 0B 81 FF 31 C0 5D C3 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01A90ull))(argc, argv);
}

// Lisp: (clear-look-at ...) | StringId: 0x583D42F2 | Guest Offset: 0x002E4830
int64_t DcClearLookAt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 49 89 F7 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E4830ull))(argc, argv);
}

// Lisp: (constrain-rope-at-joint ...) | StringId: 0x2976EC09 | Guest Offset: 0x00A04430
int64_t DcConstrainRopeAtJoint(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 83 FF 02 0F 8C 92 00 00 00 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04430ull))(argc, argv);
}

// Lisp: (cover-throw-grenade-anim ...) | StringId: 0x3B74114C | Guest Offset: 0x002E4070
int64_t DcCoverThrowGrenadeAnim(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 12 44 8B 3E 31 F6 44 89 FF E8 15
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E4070ull))(argc, argv);
}

// Lisp: (create-belt-attachment ...) | StringId: 0x6409BEA0 | Guest Offset: 0x0090AD30
int64_t DcCreateBeltAttachment(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 60 01 00 00 48 89 F3 4C 8B 3D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090AD30ull))(argc, argv);
}

// Lisp: (create-weapon-drawables ...) | StringId: 0x6C7BC0AB | Guest Offset: 0x00A0C410
int64_t DcCreateWeaponDrawables(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 89 F3 83 FF 02 75 7B 4C 8D 25 F6 F8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0C410ull))(argc, argv);
}

// Lisp: (current-time ...) | StringId: 0x388BC9A1 | Guest Offset: 0x00A0EDB0
int64_t DcCurrentTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: B8 01 00 00 00 85 FF 7E 18 48 63 0E B8 01 00 00 00 48 83 F9 09 77 0A 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0EDB0ull))(argc, argv);
}

// Lisp: (deactivate-blur ...) | StringId: 0xD862F50C | Guest Offset: 0x009FFF50
int64_t DcDeactivateBlur(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 0D C5 BD 75 00 48 8B 01 48 85 C0 74 19 8B 49 08 85 C9
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FFF50ull))(argc, argv);
}

// Lisp: (debug-break ...) | StringId: 0x6699F752 | Guest Offset: 0x009FDF30
int64_t DcDebugBreak(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 31 C0 85 FF 7E 03 48 8B 06 48 8B 1D 32 00 B7 00 48 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDF30ull))(argc, argv);
}

// Lisp: (debug-draw-cross ...) | StringId: 0x227F2974 | Guest Offset: 0x009FE070
int64_t DcDebugDrawCross(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 85 FF 7E 06 48 83 3E 00 75 0E 48 8D 35 BA FB 49 00 31 C0 E8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE070ull))(argc, argv);
}

// Lisp: (debug-draw-line ...) | StringId: 0xBD63A7BA | Guest Offset: 0x009FE090
int64_t DcDebugDrawLine(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 31 C0 B9 00 00 00 00 85 FF 7E 08 48 8B 0E B8 01 00 00 00 39
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE090ull))(argc, argv);
}

// Lisp: (debug-draw-locator ...) | StringId: 0xE847689B | Guest Offset: 0x009FE000
int64_t DcDebugDrawLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 85 FF 7E 4D 48 8B 06 48 85 C0 74 45 48 8B 48 08 48 85 C9 74
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE000ull))(argc, argv);
}

// Lisp: (debug-draw-string ...) | StringId: 0xEB7947DB | Guest Offset: 0x009FDFB0
int64_t DcDebugDrawString(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 89 F8 31 FF B9 00 00 00 00 BB 00 00 00 00 85 C0 7E 08
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDFB0ull))(argc, argv);
}

// Lisp: (debug-pause ...) | StringId: 0x7195C4EF | Guest Offset: 0x00A102A0
int64_t DcDebugPause(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv;
    *(uint8_t *)(image + 0x010595E4) = 0x01u;
    return 0;
}

// Lisp: (demo-reset-defaults ...) | StringId: 0x492DE666 | Guest Offset: 0x0090DD50
int64_t DcDemoResetDefaults(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 4D 18 74 00 C7 00 0A D7 A3 3E 48 8D 05 DC B8 74 00 C7 80 88 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090DD50ull))(argc, argv);
}

// Lisp: (despawn-message ...) | StringId: 0xB1F17BD7 | Guest Offset: 0x005DC8C0
int64_t DcDespawnMessage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 8B 36 48 8D 3D D3 D5 B5 00 E8 AE D4 FF FF 31 C0 5D C3 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DC8C0ull))(argc, argv);
}

// Lisp: (detach-rope ...) | StringId: 0x4D209D87 | Guest Offset: 0x00A04530
int64_t DcDetachRope(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 67 8B 1E 31 F6 89 DF E8 5A EE 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04530ull))(argc, argv);
}

// Lisp: (difficulty-interp ...) | StringId: 0x0057202D | Guest Offset: 0x009FDAD0
int64_t DcDifficultyInterp(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 20 C5 F8 57 C0 83 FF 03 7C 66 C5 FA 10 06 C5 FA 11
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDAD0ull))(argc, argv);
}

// Lisp: (disable-cover-camera ...) | StringId: 0xE1846603 | Guest Offset: 0x00A0E290
int64_t DcDisableCoverCamera(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 60 C9 63 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0E290ull))(argc, argv);
}

// Lisp: (disable-dyno-jump-fail-looks ...) | StringId: 0x402FACAB | Guest Offset: 0x0090CA90
int64_t DcDisableDynoJumpFailLooks(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 60 E1 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090CA90ull))(argc, argv);
}

// Lisp: (disable-edge-grab ...) | StringId: 0x5D2D16C9 | Guest Offset: 0x0090CE40
int64_t DcDisableEdgeGrab(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 81 EC 88 00 00 00 48 8B 1D AD DD 73 00 48 8B 03 48 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090CE40ull))(argc, argv);
}

// Lisp: (disable-fall-to-death ...) | StringId: 0xB555B6E3 | Guest Offset: 0x0090CBC0
int64_t DcDisableFallToDeath(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 81 EC 88 00 00 00 48 8B 1D 2D E0 73 00 48 8B 03 48 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090CBC0ull))(argc, argv);
}

// Lisp: (disable-follow-camera-collision ...) | StringId: 0x931EB73C | Guest Offset: 0x0090F4F0
int64_t DcDisableFollowCameraCollision(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 00 B7 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090F4F0ull))(argc, argv);
}

// Lisp: (disable-force-ledge-drop ...) | StringId: 0x4ED6B3A5 | Guest Offset: 0x0090CB20
int64_t DcDisableForceLedgeDrop(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D D0 E0 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090CB20ull))(argc, argv);
}

// Lisp: (disable-frame-forward ...) | StringId: 0x80DEC5B0 | Guest Offset: 0x00A038A0
int64_t DcDisableFrameForward(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 EA FA 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A038A0ull))(argc, argv);
}

// Lisp: (disable-gadgetry ...) | StringId: 0xF00D650C | Guest Offset: 0x00A03A70
int64_t DcDisableGadgetry(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 1A F9 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A03A70ull))(argc, argv);
}

// Lisp: (disable-look-around-dof ...) | StringId: 0x60C027EF | Guest Offset: 0x0090FD30
int64_t DcDisableLookAroundDof(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 AE 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090FD30ull))(argc, argv);
}

// Lisp: (disable-low-cover-exit-around ...) | StringId: 0xB6B6E273 | Guest Offset: 0x0090FC30
int64_t DcDisableLowCoverExitAround(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 AF 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090FC30ull))(argc, argv);
}

// Lisp: (disable-message ...) | StringId: 0xCB0ED5FB | Guest Offset: 0x005DC990
int64_t DcDisableMessage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 4C 8B 05 61 E2 A6 00 49 8B 00 48 89 45 F8 8B 36
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DC990ull))(argc, argv);
}

// Lisp: (disable-nav-blocker-object ...) | StringId: 0xC12D0539 | Guest Offset: 0x00A04EF0
int64_t DcDisableNavBlockerObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 57 8B 1E 31 F6 89 DF E8 9A E4 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04EF0ull))(argc, argv);
}

// Lisp: (disable-platform-control-on-object ...) | StringId: 0x88E795A7 | Guest Offset: 0x00A04E80
int64_t DcDisablePlatformControlOnObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 5D 8B 1E 31 F6 89 DF E8 0A E5 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04E80ull))(argc, argv);
}

// Lisp: (disable-player-cover-exit ...) | StringId: 0xF86DB330 | Guest Offset: 0x00906950
int64_t DcDisablePlayerCoverExit(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D A0 42 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00906950ull))(argc, argv);
}

// Lisp: (disallow-leg-ik-by-others ...) | StringId: 0xF3F258BE | Guest Offset: 0x009FCCF0
int64_t DcDisallowLegIkByOthers(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 89 FB 85 DB 0F 8E 9F 00 00 00 44 8B 3E 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FCCF0ull))(argc, argv);
}

// Lisp: (disc? ...) | StringId: 0x96880C26 | Guest Offset: 0x009FD910
int64_t DcDiscP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 8B 06 48 8D 0D 27 BD 65 00 3B 81 A8 39 00 00 0F 94 C0 0F B6 C0 C3 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD910ull))(argc, argv);
}

// Lisp: (display ...) | StringId: 0xBB0304D1 | Guest Offset: 0x009FD000
int64_t DcDisplay(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (display-button-hud ...) | StringId: 0x8F3A8A73 | Guest Offset: 0x00A0CD20
int64_t DcDisplayButtonHud(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 0F 8E A9 01 00 00 8B 06 48 8D 0D 05
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0CD20ull))(argc, argv);
}

// Lisp: (display-int ...) | StringId: 0x7CE24226 | Guest Offset: 0x009FCFF0
int64_t DcDisplayInt(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (display-message ...) | StringId: 0xB34BBA3A | Guest Offset: 0x009FE140
int64_t DcDisplayMessage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 78 04 00 00 48 8B 05 A5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE140ull))(argc, argv);
}

// Lisp: (do-rumble-ptr ...) | StringId: 0x943CE1C7 | Guest Offset: 0x00A105F0
int64_t DcDoRumblePtr(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 49 89 F6 85 FF 0F 8E 93 00 00 00 41 8B 3E 31 DB 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A105F0ull))(argc, argv);
}

// Lisp: (dont-collide-against-allies ...) | StringId: 0x5C9A1633 | Guest Offset: 0x009FCBB0
int64_t DcDontCollideAgainstAllies(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 40 E0 64 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FCBB0ull))(argc, argv);
}

// Lisp: (draw-reticle ...) | StringId: 0xC0988264 | Guest Offset: 0x0090E6E0
int64_t DcDrawReticle(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 3D 05 93 84 00 C5 FA 10 05 65 7E 55 00 C5 F8 28 C8 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E6E0ull))(argc, argv);
}

// Lisp: (drop-rope ...) | StringId: 0x7107D57E | Guest Offset: 0x00A045E0
int64_t DcDropRope(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 4C 8B 3D 09 66 64 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A045E0ull))(argc, argv);
}

// Lisp: (dump-entity-info ...) | StringId: 0xA4814C81 | Guest Offset: 0x00A04970
int64_t DcDumpEntityInfo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 89 F3 8B 33 48 8D 3D 0D BA 72 00 E8 28 7E B6 FF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04970ull))(argc, argv);
}

// Lisp: (dump-levels-info ...) | StringId: 0x38AE2B20 | Guest Offset: 0x00A04900
int64_t DcDumpLevelsInfo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 8D 3D 83 BA 72 00 BE 01 00 00 00 E8 B9 B6 97 FF 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04900ull))(argc, argv);
}

// Lisp: (elapsed-time ...) | StringId: 0x8E5B8EFC | Guest Offset: 0x00A0ED60
int64_t DcElapsedTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: B8 01 00 00 00 85 FF 7E 18 48 63 0E B8 01 00 00 00 48 83 F9 09 77 0A 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0ED60ull))(argc, argv);
}

// Lisp: (emit-bullet-from-spawner ...) | StringId: 0x9062B153 | Guest Offset: 0x00A0BF40
int64_t DcEmitBulletFromSpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 48 01 00 00 49 89 F4 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0BF40ull))(argc, argv);
}

// Lisp: (enable-action-pack ...) | StringId: 0x138ABEF3 | Guest Offset: 0x00A0A060
int64_t DcEnableActionPack(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F6 89 FB 4C 8B 25 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0A060ull))(argc, argv);
}

// Lisp: (enable-dyno-jump-fail-looks ...) | StringId: 0x86550364 | Guest Offset: 0x0090CA00
int64_t DcEnableDynoJumpFailLooks(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D F0 E1 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090CA00ull))(argc, argv);
}

// Lisp: (enable-gadgetry ...) | StringId: 0xE46DCAC5 | Guest Offset: 0x00A039C0
int64_t DcEnableGadgetry(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 CA F9 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A039C0ull))(argc, argv);
}

// Lisp: (enable-message ...) | StringId: 0x7AA34AC4 | Guest Offset: 0x005DC8E0
int64_t DcEnableMessage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 4C 8B 05 11 E3 A6 00 49 8B 00 48 89 45 F8 8B 36
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DC8E0ull))(argc, argv);
}

// Lisp: (enable-nav-blocker-object ...) | StringId: 0x451FD95B | Guest Offset: 0x00A04F60
int64_t DcEnableNavBlockerObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 43 8B 1E 31 F6 89 DF E8 2A E4 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04F60ull))(argc, argv);
}

// Lisp: (end-script ...) | StringId: 0x9896D682 | Guest Offset: 0x00A44190
int64_t DcEndScript(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 4C 8D 35 82 C1 60 00 49 63 9E 00 02 00 00 48 83 FB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A44190ull))(argc, argv);
}

// Lisp: (entity-alive? ...) | StringId: 0x3F58662D | Guest Offset: 0x00A127E0
int64_t DcEntityAliveP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 0D 8B 1E 31 F6 89 DF E8 AB 0B 2D 00 EB 14 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A127E0ull))(argc, argv);
}

// Lisp: (entity-auto-spawn-once ...) | StringId: 0x0B9B4812 | Guest Offset: 0x00A12830
int64_t DcEntityAutoSpawnOnce(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 0D 8B 1E 31 F6 89 DF E8 5B 0B 2D 00 EB 14 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A12830ull))(argc, argv);
}

// Lisp: (entity-name ...) | StringId: 0x24B6345C | Guest Offset: 0x00A127D0
int64_t DcEntityName(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)image;
    uint8_t *obj = (uint8_t *)(uintptr_t)argv[0];
    return *(uint32_t *)(obj + 0x28);
}

// Lisp: (entity-set-level! ...) | StringId: 0x9F684FA6 | Guest Offset: 0x00A64C30
int64_t DcEntitySetLevelB(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (event-arg-boolean ...) | StringId: 0x77162698 | Guest Offset: 0x004C2A40
int64_t DcEventArgBoolean(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 15 12 BE 9B 00 E8 10 00 00 00 85 C0 0F 95 C0 0F B6 C0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C2A40ull))(argc, argv);
}

// Lisp: (event-arg-bound-frame ...) | StringId: 0x5EE829E2 | Guest Offset: 0x004C28F0
int64_t DcEventArgBoundFrame(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FF 4C 8D 25 18 DA B8 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C28F0ull))(argc, argv);
}

// Lisp: (event-arg-float ...) | StringId: 0xC69511C1 | Guest Offset: 0x004C2BF0
int64_t DcEventArgFloat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F6 41 89 FF 4C 8D 25
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C2BF0ull))(argc, argv);
}

// Lisp: (event-arg-int-32 ...) | StringId: 0x62A32E72 | Guest Offset: 0x004C2BE0
int64_t DcEventArgInt32(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 15 54 BC 9B 00 E9 74 FE FF FF 0F 1F 40 00 55 48 89 E5 41 57 41 56
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C2BE0ull))(argc, argv);
}

// Lisp: (event-arg-string ...) | StringId: 0xF9453071 | Guest Offset: 0x004C2EC0
int64_t DcEventArgString(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FF 4C 8D 25 48 D4 B8 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C2EC0ull))(argc, argv);
}

// Lisp: (event-arg-symbol ...) | StringId: 0x36D703B7 | Guest Offset: 0x004C2DA0
int64_t DcEventArgSymbol(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FF 4C 8D 25 68 D5 B8 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C2DA0ull))(argc, argv);
}

// Lisp: (event-attack-source? ...) | StringId: 0x566FCBA4 | Guest Offset: 0x004C3730
int64_t DcEventAttackSourceP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FF 4C 8D 25 D8 CB B8 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C3730ull))(argc, argv);
}

// Lisp: (event-attack-type? ...) | StringId: 0x488D3BB8 | Guest Offset: 0x004C38A0
int64_t DcEventAttackTypeP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FF 4C 8D 25 68 CA B8 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C38A0ull))(argc, argv);
}

// Lisp: (event-get-attack-info ...) | StringId: 0xF2776CC0 | Guest Offset: 0x004C3D50
int64_t DcEventGetAttackInfo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 4C 8B 3D 9A 6E B8 00 49 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C3D50ull))(argc, argv);
}

// Lisp: (event-level? ...) | StringId: 0x7FEEF039 | Guest Offset: 0x004C34C0
int64_t DcEventLevelP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 0F 44 8B 3E 31 F6 44 89 FF E8 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C34C0ull))(argc, argv);
}

// Lisp: (event-my-level? ...) | StringId: 0x617579D2 | Guest Offset: 0x004C32D0
int64_t DcEventMyLevelP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 4C 8D 3D 3F D0 B8 00 49 63 9F 00 02 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C32D0ull))(argc, argv);
}

// Lisp: (event-object-damage ...) | StringId: 0xF56F37C9 | Guest Offset: 0x004C3610
int64_t DcEventObjectDamage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 4C 8D 35 02 CD B8 00 49 63 9E 00 02 00 00 48 83 FB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C3610ull))(argc, argv);
}

// Lisp: (event-return-boolean ...) | StringId: 0x7771F473 | Guest Offset: 0x004C1FF0
int64_t DcEventReturnBoolean(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 48 89 75 B8 41 89 FF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C1FF0ull))(argc, argv);
}

// Lisp: (event-return-float ...) | StringId: 0x338C1B7B | Guest Offset: 0x004C2550
int64_t DcEventReturnFloat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 48 89 75 B8 41 89 FF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C2550ull))(argc, argv);
}

// Lisp: (event-return-int-32 ...) | StringId: 0xE5D40210 | Guest Offset: 0x004C2390
int64_t DcEventReturnInt32(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 48 89 75 B8 41 89 FF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C2390ull))(argc, argv);
}

// Lisp: (event-return-symbol ...) | StringId: 0xB1A02FD5 | Guest Offset: 0x004C2720
int64_t DcEventReturnSymbol(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 48 89 75 B8 41 89 FF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C2720ull))(argc, argv);
}

// Lisp: (event-return-uint-32 ...) | StringId: 0xA0F5E780 | Guest Offset: 0x004C21D0
int64_t DcEventReturnUint32(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 48 89 75 B8 41 89 FF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C21D0ull))(argc, argv);
}

// Lisp: (event-sender ...) | StringId: 0x030B1EA2 | Guest Offset: 0x004C1DF0
int64_t DcEventSender(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 4C 8D 35 22 E5 B8 00 49 63 9E 00 02 00 00 48 83 FB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C1DF0ull))(argc, argv);
}

// Lisp: (event-sender? ...) | StringId: 0xEA2054F4 | Guest Offset: 0x004C1ED0
int64_t DcEventSenderP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FC 4C 8D 3D 38 E4 B8 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C1ED0ull))(argc, argv);
}

// Lisp: (event-train-car? ...) | StringId: 0x1C6DCAF0 | Guest Offset: 0x004C3020
int64_t DcEventTrainCarP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FC 4C 8D 3D E8 D2 B8 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C3020ull))(argc, argv);
}

// Lisp: (event-train? ...) | StringId: 0x2A1DEF35 | Guest Offset: 0x004C31A0
int64_t DcEventTrainP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FF 4C 8D 25 68 D1 B8 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C31A0ull))(argc, argv);
}

// Lisp: (facing-point? ...) | StringId: 0x18AF3A3C | Guest Offset: 0x00A0F6D0
int64_t DcFacingPointP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FC 41 83 FC 02 75 05 4D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0F6D0ull))(argc, argv);
}

// Lisp: (fade-to-black ...) | StringId: 0xDEA49A81 | Guest Offset: 0x009FFBA0
int64_t DcFadeToBlack(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 78 48 8B 1D 50 B0 64 00 48 8B 03 48 89 45 F0 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FFBA0ull))(argc, argv);
}

// Lisp: (fade-to-black-pers ...) | StringId: 0x77E20A49 | Guest Offset: 0x009FFA40
int64_t DcFadeToBlackPers(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D B0 B1 64 00 48 8B 03 48 89 45 F0 B0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FFA40ull))(argc, argv);
}

// Lisp: (fade-to-white ...) | StringId: 0x5DAF1DDD | Guest Offset: 0x009FFAC0
int64_t DcFadeToWhite(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 78 48 8B 1D 30 B1 64 00 48 8B 03 48 89 45 F0 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FFAC0ull))(argc, argv);
}

// Lisp: (fall-to-death ...) | StringId: 0x2722ADE3 | Guest Offset: 0x0090C410
int64_t DcFallToDeath(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 81 EC 88 00 00 00 48 8B 1D DD E7 73 00 48 8B 03 48 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090C410ull))(argc, argv);
}

// Lisp: (fire-weapon ...) | StringId: 0xC7FD990C | Guest Offset: 0x00A0C930
int64_t DcFireWeapon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 E4 E0 FF FF FF 48 81 EC 20
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0C930ull))(argc, argv);
}

// Lisp: (fire-weapon-at-target ...) | StringId: 0xF472F811 | Guest Offset: 0x00A0C4C0
int64_t DcFireWeaponAtTarget(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 E4 E0 FF FF FF 48 81 EC 20
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0C4C0ull))(argc, argv);
}

// Lisp: (fma-request-part-module ...) | StringId: 0x5B61666F | Guest Offset: 0x00A00050
int64_t DcFmaRequestPartModule(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 85 FF 7E 0D 8B 1E 31 F6 89 DF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A00050ull))(argc, argv);
}

// Lisp: (fma-reset-part-modules ...) | StringId: 0x3D69B8F7 | Guest Offset: 0x00A00130
int64_t DcFmaResetPartModules(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (follow-spline ...) | StringId: 0x0985239F | Guest Offset: 0x00A25E50
int64_t DcFollowSpline(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 89 FB 85 DB 7E 22 45 8B 3E 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A25E50ull))(argc, argv);
}

// Lisp: (for-group ...) | StringId: 0xEB281C9C | Guest Offset: 0x0054B830
int64_t DcForGroup(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 48 89 F0 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0054B830ull))(argc, argv);
}

// Lisp: (force-aiming-camera-ultra-tight ...) | StringId: 0x653EDCDF | Guest Offset: 0x00A03670
int64_t DcForceAimingCameraUltraTight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 80 75 64 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A03670ull))(argc, argv);
}

// Lisp: (force-board-jetski ...) | StringId: 0x3FF2398E | Guest Offset: 0x00A01BA0
int64_t DcForceBoardJetski(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (force-collision-for-aim-focus ...) | StringId: 0x844E9FD7 | Guest Offset: 0x00906F90
int64_t DcForceCollisionForAimFocus(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 60 3C 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00906F90ull))(argc, argv);
}

// Lisp: (force-death-camera ...) | StringId: 0xD76A667D | Guest Offset: 0x0090EFF0
int64_t DcForceDeathCamera(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 00 BC 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090EFF0ull))(argc, argv);
}

// Lisp: (force-distance-stop-increase ...) | StringId: 0x05267174 | Guest Offset: 0x00906740
int64_t DcForceDistanceStopIncrease(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D B0 44 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00906740ull))(argc, argv);
}

// Lisp: (force-dock-jetski ...) | StringId: 0xABAA7382 | Guest Offset: 0x00A01B90
int64_t DcForceDockJetski(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (force-hair-anim ...) | StringId: 0xD7539738 | Guest Offset: 0x00906840
int64_t DcForceHairAnim(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 48 C7 04 24 00 00 00 00 BE 48 01 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00906840ull))(argc, argv);
}

// Lisp: (force-high-ledge-drop ...) | StringId: 0x258DF119 | Guest Offset: 0x0090F630
int64_t DcForceHighLedgeDrop(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 B5 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090F630ull))(argc, argv);
}

// Lisp: (force-no-shift-camera-zoom-mode ...) | StringId: 0x0C474223 | Guest Offset: 0x00A035E0
int64_t DcForceNoShiftCameraZoomMode(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 10 76 64 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A035E0ull))(argc, argv);
}

// Lisp: (force-player-swim ...) | StringId: 0xFA72A080 | Guest Offset: 0x0090D740
int64_t DcForcePlayerSwim(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 83 3E 01 48 8D 05 C6 20 74 00 0F 94 80 DC 00 00 00 31 C0 C3 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D740ull))(argc, argv);
}

// Lisp: (force-reflections ...) | StringId: 0x172B8004 | Guest Offset: 0x00A02960
int64_t DcForceReflections(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 15 83 3E 00 48 8D 05 0C 20 76 00 0F 95 40 5C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02960ull))(argc, argv);
}

// Lisp: (force-stealth-off ...) | StringId: 0xE3FC9E0B | Guest Offset: 0x00A0BC40
int64_t DcForceStealthOff(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D B0 EF 63 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0BC40ull))(argc, argv);
}

// Lisp: (force-uboat-collision ...) | StringId: 0xE96CD997 | Guest Offset: 0x0090C960
int64_t DcForceUboatCollision(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 90 E2 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090C960ull))(argc, argv);
}

// Lisp: (forget-signal ...) | StringId: 0xFC37CD06 | Guest Offset: 0x00A3FF40
int64_t DcForgetSignal(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 49 89 F7 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A3FF40ull))(argc, argv);
}

// Lisp: (freeze-binding-when-non-physical ...) | StringId: 0x5205E230 | Guest Offset: 0x00A03700
int64_t DcFreezeBindingWhenNonPhysical(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 25 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A03700ull))(argc, argv);
}

// Lisp: (from-ups ...) | StringId: 0x6CD3AABF | Guest Offset: 0x003976A0
int64_t DcFromUps(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: C5 FA 10 06 C5 FA 59 46 08 48 C7 44 24 F8 00 00 00 00 C5 FA 11 44 24 F8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x003976A0ull))(argc, argv);
}

// Lisp: (from-ups-no-dt ...) | StringId: 0x9CEF4D21 | Guest Offset: 0x009FD6B0
int64_t DcFromUpsNoDt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 01 75 2A C5 FA 10 06 48 8D 05 22 48 76 00 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD6B0ull))(argc, argv);
}

// Lisp: (game-task-graph-login ...) | StringId: 0xB8831CF4 | Guest Offset: 0x009FE8F0
int64_t DcGameTaskGraphLogin(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8B 3E E8 64 B0 05 00 31 C0 5D C3 55 48 89 E5 41 57 41 56
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE8F0ull))(argc, argv);
}

// Lisp: (get-action-pack-locator ...) | StringId: 0x30A7A25C | Guest Offset: 0x00A060C0
int64_t DcGetActionPackLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 48 89 F0 89 FA 4C 8B 35 29 4B 64 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A060C0ull))(argc, argv);
}

// Lisp: (get-align-locator ...) | StringId: 0x7922FC22 | Guest Offset: 0x00A066F0
int64_t DcGetAlignLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 49 89 F4 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A066F0ull))(argc, argv);
}

// Lisp: (get-animation-phase ...) | StringId: 0x05516ACB | Guest Offset: 0x003976C0
int64_t DcGetAnimationPhase(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 49 89 F7 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x003976C0ull))(argc, argv);
}

// Lisp: (get-ap-reference-effect-locator ...) | StringId: 0xC6B66B4C | Guest Offset: 0x00A07340
int64_t DcGetApReferenceEffectLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 28 01 00 00 49 89 F5 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A07340ull))(argc, argv);
}

// Lisp: (get-ap-reference-locator ...) | StringId: 0x4E10B00C | Guest Offset: 0x00A07990
int64_t DcGetApReferenceLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 68 48 89 F3 4C 8B 35 55
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A07990ull))(argc, argv);
}

// Lisp: (get-ap-reference-locator-radial ...) | StringId: 0x6825AE79 | Guest Offset: 0x00A06C20
int64_t DcGetApReferenceLocatorRadial(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 49 89 F6 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A06C20ull))(argc, argv);
}

// Lisp: (get-boolean ...) | StringId: 0xF307E431 | Guest Offset: 0x00A42BF0
int64_t DcGetBoolean(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F4 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A42BF0ull))(argc, argv);
}

// Lisp: (get-capture-zone ...) | StringId: 0xDFFEB259 | Guest Offset: 0x006687D0
int64_t DcGetCaptureZone(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (get-current-time ...) | StringId: 0x55B2CA6C | Guest Offset: 0x00A44800
int64_t DcGetCurrentTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 10 8B 1E 31 F6 89 DF E8 87 EB 29
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A44800ull))(argc, argv);
}

// Lisp: (get-dda ...) | StringId: 0xA86D881D | Guest Offset: 0x009FD010
int64_t DcGetDda(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 E8 E3 E9 AA FF 48 C7 45 F8 00 00 00 00 C5 FA 11
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD010ull))(argc, argv);
}

// Lisp: (get-difficulty ...) | StringId: 0xDCF596C6 | Guest Offset: 0x009FD040
int64_t DcGetDifficulty(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 83 3D 35 19 AD 00 01 B8 02 00 00 00 7F 0E 48 8D 05 EB C5 65 00 48 63 80
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD040ull))(argc, argv);
}

// Lisp: (get-float ...) | StringId: 0x1CB8C51F | Guest Offset: 0x00A43170
int64_t DcGetFloat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 F4 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A43170ull))(argc, argv);
}

// Lisp: (get-grapple-ap-ref ...) | StringId: 0xC08B6878 | Guest Offset: 0x009102C0
int64_t DcGetGrappleApRef(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 78 4C 8B 3D 2C A9 73 00 49 8B 07 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009102C0ull))(argc, argv);
}

// Lisp: (get-harvesting-priority ...) | StringId: 0x445ED996 | Guest Offset: 0x00A01AB0
int64_t DcGetHarvestingPriority(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 E9 C1 70 00 0F B6 40 10 C3 0F 1F 40 00 85 FF 7E 18 0F B6 06 B1
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01AB0ull))(argc, argv);
}

// Lisp: (get-int-32 ...) | StringId: 0x71666D4D | Guest Offset: 0x00A42EB0
int64_t DcGetInt32(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F4 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A42EB0ull))(argc, argv);
}

// Lisp: (get-joint-angle-diff-from-bindpose ...) | StringId: 0x2FD346D0 | Guest Offset: 0x00A06180
int64_t DcGetJointAngleDiffFromBindpose(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 48 89 F3 4C 8B 2D 65
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A06180ull))(argc, argv);
}

// Lisp: (get-joint-locator ...) | StringId: 0xF6A75AAD | Guest Offset: 0x00A07E80
int64_t DcGetJointLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 F7 89 FB 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A07E80ull))(argc, argv);
}

// Lisp: (get-joint-position ...) | StringId: 0x4902715B | Guest Offset: 0x00A120C0
int64_t DcGetJointPosition(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 4C 8B 35 2E 8B 63 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A120C0ull))(argc, argv);
}

// Lisp: (get-locator-distance ...) | StringId: 0x500A967E | Guest Offset: 0x00A0EF50
int64_t DcGetLocatorDistance(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 49 89 F6 89 FB 83 FB 02 74 3A 48 8B 05
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0EF50ull))(argc, argv);
}

// Lisp: (get-melee-teach-dest ...) | StringId: 0x73D95253 | Guest Offset: 0x00908D10
int64_t DcGetMeleeTeachDest(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 4C 8B 35 DE 1E 74 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00908D10ull))(argc, argv);
}

// Lisp: (get-object-health ...) | StringId: 0xE990B4BD | Guest Offset: 0x00A04B10
int64_t DcGetObjectHealth(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 85 FF 7E 10 8B 1E 31 F6 89 DF E8 76 E8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04B10ull))(argc, argv);
}

// Lisp: (get-object-id ...) | StringId: 0x845BE77A | Guest Offset: 0x00A0BEE0
int64_t DcGetObjectId(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 AA 74 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0BEE0ull))(argc, argv);
}

// Lisp: (get-object-locator ...) | StringId: 0xB4C97EE4 | Guest Offset: 0x00A08630
int64_t DcGetObjectLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 30 4C 8B 25 BA 25 64 00 49 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A08630ull))(argc, argv);
}

// Lisp: (get-object-locator-radial ...) | StringId: 0x8F74AFF2 | Guest Offset: 0x00A081A0
int64_t DcGetObjectLocatorRadial(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 49 89 F4 41 89 FF 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A081A0ull))(argc, argv);
}

// Lisp: (get-object-offset-locator ...) | StringId: 0x28FAB5FF | Guest Offset: 0x00A121E0
int64_t DcGetObjectOffsetLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 28 48 89 F3 4C 8B 3D 09 8A 63 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A121E0ull))(argc, argv);
}

// Lisp: (get-object-offset-position ...) | StringId: 0x39F8C8B8 | Guest Offset: 0x00A12390
int64_t DcGetObjectOffsetPosition(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 48 48 89 F3 4C 8B 3D 59 88 63 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A12390ull))(argc, argv);
}

// Lisp: (get-object-position ...) | StringId: 0x1A227342 | Guest Offset: 0x00A12610
int64_t DcGetObjectPosition(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 4C 8B 35 DE 85 63 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A12610ull))(argc, argv);
}

// Lisp: (get-object-state ...) | StringId: 0x874DD706 | Guest Offset: 0x00A04C10
int64_t DcGetObjectState(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 41 89 FC 45 85 E4 7E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04C10ull))(argc, argv);
}

// Lisp: (get-origin-locator ...) | StringId: 0x9927F0D1 | Guest Offset: 0x00A05F60
int64_t DcGetOriginLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 4C 8B 35 8E 4C 64 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A05F60ull))(argc, argv);
}

// Lisp: (get-player-aim-time ...) | StringId: 0x53B6ED2C | Guest Offset: 0x005DCBA0
int64_t DcGetPlayerAimTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 81 EC 98 00 00 00 48 8B 1D 4D E0 A6 00 48 8B 03 48 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DCBA0ull))(argc, argv);
}

// Lisp: (get-player-carried-object ...) | StringId: 0xE4549025 | Guest Offset: 0x009068C0
int64_t DcGetPlayerCarriedObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 48 8D 0D 4F 54 85 00 48 8B 01 48 85 C0 74
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009068C0ull))(argc, argv);
}

// Lisp: (get-player-clip-percent-full ...) | StringId: 0xCAD226B3 | Guest Offset: 0x005DCE00
int64_t DcGetPlayerClipPercentFull(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 8D 0D 11 EF B7 00 48 8B 01 C5 F8 57 C0 48 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DCE00ull))(argc, argv);
}

// Lisp: (get-player-edge-bbject ...) | StringId: 0xDB3EF96A | Guest Offset: 0x0090DCA0
int64_t DcGetPlayerEdgeBbject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 28 48 8B 1D 50 CF 73 00 48 8B 03 48 89 45 F0 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090DCA0ull))(argc, argv);
}

// Lisp: (get-player-health ...) | StringId: 0xAF909084 | Guest Offset: 0x00910140
int64_t DcGetPlayerHealth(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 8D 0D D3 BB 84 00 48 8B 01 48 85 C0 74 26 8B 49 08
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00910140ull))(argc, argv);
}

// Lisp: (get-player-platform ...) | StringId: 0x2F1BD5DD | Guest Offset: 0x0090CF40
int64_t DcGetPlayerPlatform(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 48 8D 05 CF ED 84 00 48 8B 08 31 DB 48 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090CF40ull))(argc, argv);
}

// Lisp: (get-player-speed ...) | StringId: 0x19800ED3 | Guest Offset: 0x00910020
int64_t DcGetPlayerSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 8D 0D F1 BC 84 00 48 8B 01 48 85 C0 0F 84 82
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00910020ull))(argc, argv);
}

// Lisp: (get-player-teach-combo-square ...) | StringId: 0x21034914 | Guest Offset: 0x005DCDF0
int64_t DcGetPlayerTeachComboSquare(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 C8 AB B7 00 0F B6 00 C3 0F 1F 44 00 00 55 48 89 E5 48 83 EC 10
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DCDF0ull))(argc, argv);
}

// Lisp: (get-player-teach-combo-triangle ...) | StringId: 0x8642C309 | Guest Offset: 0x005DCDE0
int64_t DcGetPlayerTeachComboTriangle(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 D7 AB B7 00 0F B6 00 C3 0F 1F 44 00 00 48 8D 05 C8 AB B7 00 0F
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DCDE0ull))(argc, argv);
}

// Lisp: (get-player-time-in-combat ...) | StringId: 0xAB98161A | Guest Offset: 0x005DCD60
int64_t DcGetPlayerTimeInCombat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: C5 F8 57 C0 83 3D 29 14 F6 00 01 75 20 48 8D 05 0C 94 B3 00 8B 40 44 2B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DCD60ull))(argc, argv);
}

// Lisp: (get-player-time-since-cover ...) | StringId: 0x659EB76A | Guest Offset: 0x005DCCE0
int64_t DcGetPlayerTimeSinceCover(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D 39 F0 B7 00 48 8B 01 48 85 C0 74 4C 8B 49 08 85 C9 74 45 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DCCE0ull))(argc, argv);
}

// Lisp: (get-quaternion-pitch ...) | StringId: 0xD885C954 | Guest Offset: 0x009F37C0
int64_t DcGetQuaternionPitch(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 C5 F8 57 C0 85 FF 0F 8E 09 01 00 00 48 8B 06 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009F37C0ull))(argc, argv);
}

// Lisp: (get-script-state ...) | StringId: 0x1359246D | Guest Offset: 0x00A44290
int64_t DcGetScriptState(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 FA F0 29 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A44290ull))(argc, argv);
}

// Lisp: (get-self-id ...) | StringId: 0xF6F9C2E4 | Guest Offset: 0x00A04A00
int64_t DcGetSelfId(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 BE 66 55 B7 FF E8 32 74 FB FF 48 89 C1 31 C0 48 85 C9 74 03
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04A00ull))(argc, argv);
}

// Lisp: (get-spawner-locator ...) | StringId: 0x072AF1AC | Guest Offset: 0x00A09330
int64_t DcGetSpawnerLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 38 4C 8B 2D B8 18 64 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A09330ull))(argc, argv);
}

// Lisp: (get-spawner-locator-projected ...) | StringId: 0xD857F64E | Guest Offset: 0x00A08D10
int64_t DcGetSpawnerLocatorProjected(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 58 49 89 F7 41 89 FD 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A08D10ull))(argc, argv);
}

// Lisp: (get-spawner-locator-radial ...) | StringId: 0xF2186986 | Guest Offset: 0x00A088C0
int64_t DcGetSpawnerLocatorRadial(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 49 89 F4 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A088C0ull))(argc, argv);
}

// Lisp: (get-spawner-position ...) | StringId: 0x49D090E5 | Guest Offset: 0x00A094B0
int64_t DcGetSpawnerPosition(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 30 4C 8B 25 3A 17 64 00 49 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A094B0ull))(argc, argv);
}

// Lisp: (get-stat-value ...) | StringId: 0x5492073C | Guest Offset: 0x0097BD10
int64_t DcGetStatValue(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 53 49 BB 00 00 00 00 01 00 00 00 83 FF 01 75 56 4C 8D 05 19 00 7E 00 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0097BD10ull))(argc, argv);
}

// Lisp: (get-state-time ...) | StringId: 0x899EE046 | Guest Offset: 0x00A44960
int64_t DcGetStateTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 10 8B 1E 31 F6 89 DF E8 27 EA 29
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A44960ull))(argc, argv);
}

// Lisp: (get-symbol ...) | StringId: 0x25124088 | Guest Offset: 0x00A43440
int64_t DcGetSymbol(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F4 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A43440ull))(argc, argv);
}

// Lisp: (get-water-offset ...) | StringId: 0x94BD9EEB | Guest Offset: 0x00A02F20
int64_t DcGetWaterOffset(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 4C 8B 35 CC 7C 64 00 49 8B 06 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02F20ull))(argc, argv);
}

// Lisp: (get-water-state ...) | StringId: 0x68E64032 | Guest Offset: 0x00A02D90
int64_t DcGetWaterState(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 4A 48 8D 0D DF 1B 76 00 48 8B 81 90 00 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02D90ull))(argc, argv);
}

// Lisp: (give-player-flashlight ...) | StringId: 0x0E67F0C9 | Guest Offset: 0x00A0ADC0
int64_t DcGivePlayerFlashlight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 00 01 00 00 4C 8B 35 27 FE 63
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0ADC0ull))(argc, argv);
}

// Lisp: (give-player-weapon ...) | StringId: 0x3D37A8AA | Guest Offset: 0x00906B20
int64_t DcGivePlayerWeapon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 10 01 00 00 4C 8B 3D C7 40 74
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00906B20ull))(argc, argv);
}

// Lisp: (give-sullivan-flashlight ...) | StringId: 0x9B509CF9 | Guest Offset: 0x00A0ABC0
int64_t DcGiveSullivanFlashlight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 81 EC 00 01 00 00 4C 8B 35 2B 00 64 00 49 8B 06
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0ABC0ull))(argc, argv);
}

// Lisp: (grab-rope ...) | StringId: 0x37DDF093 | Guest Offset: 0x00A04740
int64_t DcGrabRope(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 48 89 F3 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04740ull))(argc, argv);
}

// Lisp: (grenade-director-change-settings ...) | StringId: 0x6769FB9E | Guest Offset: 0x0069B910
int64_t DcGrenadeDirectorChangeSettings(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 49 89 F6 89 FB 83 3D 59 30 E3 00 02 7C 10 48 8D 35
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0069B910ull))(argc, argv);
}

// Lisp: (grenade-director-disable ...) | StringId: 0xA7E3BDC5 | Guest Offset: 0x0069BA40
int64_t DcGrenadeDirectorDisable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 83 3D 31 2F E3 00 02 7C 10 48 8D 35 D3 FC 7E 00 31 C0 89 FA
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0069BA40ull))(argc, argv);
}

// Lisp: (grenade-director-enable ...) | StringId: 0xC77239E4 | Guest Offset: 0x0069BAB0
int64_t DcGrenadeDirectorEnable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 83 3D BA 2E E3 00 02 7C 10 48 8D 35 94
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0069BAB0ull))(argc, argv);
}

// Lisp: (grenade-director-is-enabled? ...) | StringId: 0x7052DAE7 | Guest Offset: 0x0069B9C0
int64_t DcGrenadeDirectorIsEnabledP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 83 3D B1 2F E3 00 02 7C 10 48 8D 35 18 FD 7E 00 31 C0 89 FA
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0069B9C0ull))(argc, argv);
}

// Lisp: (ground-prints-disable ...) | StringId: 0xEB78A103 | Guest Offset: 0x00A01EB0
int64_t DcGroundPrintsDisable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 B9 56 71 00 C6 80 88 0A 00 00 00 B8 01 00 00 00 C3 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01EB0ull))(argc, argv);
}

// Lisp: (ground-prints-enable ...) | StringId: 0x7FA59498 | Guest Offset: 0x00A01ED0
int64_t DcGroundPrintsEnable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 99 56 71 00 C6 80 88 0A 00 00 01 B8 01 00 00 00 C3 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01ED0ull))(argc, argv);
}

// Lisp: (ground-prints-type ...) | StringId: 0x3755F909 | Guest Offset: 0x00A01F00
int64_t DcGroundPrintsType(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 BB 01 00 00 00 85 FF 7F 10 48 8D 35 14 C6 49 00 31 DB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01F00ull))(argc, argv);
}

// Lisp: (group-and ...) | StringId: 0x0B30CE4C | Guest Offset: 0x0054AFD0
int64_t DcGroupAnd(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 48 89 F0 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0054AFD0ull))(argc, argv);
}

// Lisp: (group-at ...) | StringId: 0x33570B73 | Guest Offset: 0x0054A6B0
int64_t DcGroupAt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 41 89 FC 45 85 E4 7E 11 41 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0054A6B0ull))(argc, argv);
}

// Lisp: (group-count ...) | StringId: 0x4D338DEE | Guest Offset: 0x00549810
int64_t DcGroupCount(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 85 FF 7E 10 8B 1E 31 F6 89 DF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00549810ull))(argc, argv);
}

// Lisp: (group-is-member-symbol? ...) | StringId: 0x25807462 | Guest Offset: 0x0054A250
int64_t DcGroupIsMemberSymbolP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FC 45 85 E4 7E 11 41 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0054A250ull))(argc, argv);
}

// Lisp: (group-or ...) | StringId: 0x464FBA35 | Guest Offset: 0x0054A760
int64_t DcGroupOr(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 48 89 F0 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0054A760ull))(argc, argv);
}

// Lisp: (group-spawned-count ...) | StringId: 0xEA491C13 | Guest Offset: 0x00549BF0
int64_t DcGroupSpawnedCount(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 38 4C 8B 35 F8 0F B0 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00549BF0ull))(argc, argv);
}

// Lisp: (guarantee-1-bullet ...) | StringId: 0x0EDF00A9 | Guest Offset: 0x0090C8F0
int64_t DcGuarantee1Bullet(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 0D 25 F4 84 00 48 8B 01 48 85 C0 74 57 8B 49 08 85 C9
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090C8F0ull))(argc, argv);
}

// Lisp: (harvesting-enabled? ...) | StringId: 0xCCA41F6D | Guest Offset: 0x00A01AF0
int64_t DcHarvestingEnabledP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 A9 C1 70 00 0F B6 00 C3 0F 1F 44 00 00 85 FF 7E 0D 83 3E 00 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01AF0ull))(argc, argv);
}

// Lisp: (has-anim? ...) | StringId: 0x54D3F01B | Guest Offset: 0x00397A20
int64_t DcHasAnimP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 89 F3 45 31 FF 83 FF 02 7C 65 44 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00397A20ull))(argc, argv);
}

// Lisp: (heli-animate-gun ...) | StringId: 0x5DCEAFC9 | Guest Offset: 0x00A7ECD0
int64_t DcHeliAnimateGun(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 25 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7ECD0ull))(argc, argv);
}

// Lisp: (heli-enable-wind ...) | StringId: 0x2B74FD3A | Guest Offset: 0x00A7EB60
int64_t DcHeliEnableWind(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 90 C0 5C 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7EB60ull))(argc, argv);
}

// Lisp: (heli-fire-missile ...) | StringId: 0xDBD62611 | Guest Offset: 0x00A7F140
int64_t DcHeliFireMissile(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 78 49 89 F7 89 FB 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7F140ull))(argc, argv);
}

// Lisp: (heli-move-to ...) | StringId: 0xC4748BE4 | Guest Offset: 0x00A7FEE0
int64_t DcHeliMoveTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F7 89 FB 85 DB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7FEE0ull))(argc, argv);
}

// Lisp: (heli-move-to-spline ...) | StringId: 0xB5E9A606 | Guest Offset: 0x00A7FDD0
int64_t DcHeliMoveToSpline(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F5 89 FB 85 DB 7E 31 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7FDD0ull))(argc, argv);
}

// Lisp: (heli-set-base-pitch ...) | StringId: 0x28DFED19 | Guest Offset: 0x00A7EC00
int64_t DcHeliSetBasePitch(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F4 89 FB 85 DB 7E 30
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7EC00ull))(argc, argv);
}

// Lisp: (heli-set-firing-state ...) | StringId: 0x19E52453 | Guest Offset: 0x00A7F5C0
int64_t DcHeliSetFiringState(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F7 89 FB 85 DB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7F5C0ull))(argc, argv);
}

// Lisp: (heli-set-hover ...) | StringId: 0xB55B90A0 | Guest Offset: 0x00A7ED80
int64_t DcHeliSetHover(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 25 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7ED80ull))(argc, argv);
}

// Lisp: (heli-set-mg-stats ...) | StringId: 0x54AA59B5 | Guest Offset: 0x00A7F6B0
int64_t DcHeliSetMgStats(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F7 89 FB 85 DB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7F6B0ull))(argc, argv);
}

// Lisp: (heli-set-target-locator ...) | StringId: 0x8234A90B | Guest Offset: 0x00A7F2E0
int64_t DcHeliSetTargetLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F7 89 FB 85 DB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7F2E0ull))(argc, argv);
}

// Lisp: (heli-set-target-object ...) | StringId: 0xB224EDF6 | Guest Offset: 0x00A7F4A0
int64_t DcHeliSetTargetObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 89 FB 85 DB 7E 25 45 8B 27 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7F4A0ull))(argc, argv);
}

// Lisp: (hide-hint ...) | StringId: 0x4D93A099 | Guest Offset: 0x00A0CC10
int64_t DcHideHint(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D E0 DF 63 00 48 8B 03 48 89 45 F0 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0CC10ull))(argc, argv);
}

// Lisp: (hide-hud ...) | StringId: 0x44A7360E | Guest Offset: 0x00A0CD00
int64_t DcHideHud(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 01 33 72 00 48 8B 40 60 C6 80 0D 04 00 00 01 31 C0 C3 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0CD00ull))(argc, argv);
}

// Lisp: (hide-message ...) | StringId: 0x238A1DD3 | Guest Offset: 0x005DCA40
int64_t DcHideMessage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 4C 8B 05 B1 E1 A6 00 49 8B 00 48 89 45 F8 8B 36
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DCA40ull))(argc, argv);
}

// Lisp: (hide-message-deprecated ...) | StringId: 0x8987668B | Guest Offset: 0x005DC650
int64_t DcHideMessageDeprecated(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 4C 8B 05 A1 E5 A6 00 49 8B 00 48 89 45 F8 8B 36
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DC650ull))(argc, argv);
}

// Lisp: (holster-player-weapon ...) | StringId: 0x90FFF8B1 | Guest Offset: 0x0090D7C0
int64_t DcHolsterPlayerWeapon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 4C 8B 3D 25
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D7C0ull))(argc, argv);
}

// Lisp: (hud-set-counter ...) | StringId: 0x14887B7E | Guest Offset: 0x00A10470
int64_t DcHudSetCounter(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 8B 06 8B 4E 08 48 8D 15 8C FB 71 00 48 8B B2 28 03 00 00 89 46 40 89 4E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A10470ull))(argc, argv);
}

// Lisp: (hud-set-timer ...) | StringId: 0x47F1FB04 | Guest Offset: 0x00A104B0
int64_t DcHudSetTimer(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: C5 FA 10 06 C5 FA 59 05 5C 6F 45 00 C5 FA 2C C0 48 8D 0D 41 FB 71 00 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A104B0ull))(argc, argv);
}

// Lisp: (in-front-of-tank? ...) | StringId: 0x27654789 | Guest Offset: 0x00AA3E10
int64_t DcInFrontOfTankP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 48 89 F3 83 FF 02 0F 8C F1 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA3E10ull))(argc, argv);
}

// Lisp: (increment-stat ...) | StringId: 0x356D1DB1 | Guest Offset: 0x0097BDF0
int64_t DcIncrementStat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 83 FF 02 75 18 8B 16 8B 76 08 48 8D 3D 3B FF 7D 00 31 C9 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0097BDF0ull))(argc, argv);
}

// Lisp: (increment-stat-difficulty ...) | StringId: 0x07389FA9 | Guest Offset: 0x0097C010
int64_t DcIncrementStatDifficulty(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 E4 E0 FF FF FF 48 81 EC A0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0097C010ull))(argc, argv);
}

// Lisp: (increment-task-stat ...) | StringId: 0x739938AF | Guest Offset: 0x0097BE20
int64_t DcIncrementTaskStat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 83 FF 03 75 1B 8B 46 10 8B 16 8B 4E 08 48 8D 3D 08 FF 7D 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0097BE20ull))(argc, argv);
}

// Lisp: (invalid-string-id ...) | StringId: 0x99D8B976 | Guest Offset: 0x009FDE50
int64_t DcInvalidStringId(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (is-camera-stick-pressed ...) | StringId: 0x4ADFE187 | Guest Offset: 0x00A0D610
int64_t DcIsCameraStickPressed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 69 8B 70 00 C5 FA 10 90 80 0A 00 00 C5 FA 10 98 84 0A 00 00 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0D610ull))(argc, argv);
}

// Lisp: (is-difficulty-very-easy ...) | StringId: 0xA4B464C9 | Guest Offset: 0x009FCFC0
int64_t DcIsDifficultyVeryEasy(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 79 C6 65 00 83 B8 A4 48 00 00 00 0F 94 C0 83 3D A4 19 AD 00 02
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FCFC0ull))(argc, argv);
}

// Lisp: (is-fma ...) | StringId: 0x094728AF | Guest Offset: 0x00A0FE80
int64_t DcIsFma(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv;
    return *(uint8_t *)(image + 0x0105DECD);
}

// Lisp: (is-japan? ...) | StringId: 0x39DB8774 | Guest Offset: 0x009FD8B0
int64_t DcIsJapanP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D 89 BD 65 00 B0 01 83 B9 94 39 00 00 00 74 3C 48 8D 0D 4F 34 A5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD8B0ull))(argc, argv);
}

// Lisp: (is-on-screen ...) | StringId: 0x1E87DE60 | Guest Offset: 0x009FD200
int64_t DcIsOnScreen(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 90 00 00 00 4C 8B 3D E7 D9 64
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD200ull))(argc, argv);
}

// Lisp: (is-speaking ...) | StringId: 0xEC6D8062 | Guest Offset: 0x00A013F0
int64_t DcIsSpeaking(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 4C 8B 3D F9 97 64 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A013F0ull))(argc, argv);
}

// Lisp: (is-stick-pressed-towards ...) | StringId: 0x1EC8CE5C | Guest Offset: 0x00A0D6B0
int64_t DcIsStickPressedTowards(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 68 31 C0 85 FF 0F 8E 72 05 00 00 48 8B 1E 31 C0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0D6B0ull))(argc, argv);
}

// Lisp: (jetski-clear-engine-sound-override ...) | StringId: 0x639C300F | Guest Offset: 0x00A01B20
int64_t DcJetskiClearEngineSoundOverride(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (jetski-override-engine-sound ...) | StringId: 0x3983BE7D | Guest Offset: 0x00A01B30
int64_t DcJetskiOverrideEngineSound(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (jetski-start-engine-sound ...) | StringId: 0xC88EFECE | Guest Offset: 0x00A01B40
int64_t DcJetskiStartEngineSound(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (journal-clear-page ...) | StringId: 0xD933FE27 | Guest Offset: 0x009FD090
int64_t DcJournalClearPage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 A9 C5 65 00 C7 80 80 49 00 00 FF FF FF FF 31 C0 C3 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD090ull))(argc, argv);
}

// Lisp: (journal-disable ...) | StringId: 0x8781BF23 | Guest Offset: 0x00A10510
int64_t DcJournalDisable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D E0 A6 63 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A10510ull))(argc, argv);
}

// Lisp: (journal-get-page ...) | StringId: 0x2E5FD939 | Guest Offset: 0x009FD110
int64_t DcJournalGetPage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 29 C5 65 00 8B 80 80 49 00 00 FF C0 48 98 C3 66 66 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD110ull))(argc, argv);
}

// Lisp: (journal-open? ...) | StringId: 0xEA864952 | Guest Offset: 0x009FD0B0
int64_t DcJournalOpenP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D 69 EC 75 00 48 8B 11 31 C0 48 85 D2 74 3F 8B 49 08 31 C0 85 C9
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD0B0ull))(argc, argv);
}

// Lisp: (journal-set-ambient-scale ...) | StringId: 0x67311D72 | Guest Offset: 0x009FD060
int64_t DcJournalSetAmbientScale(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 85 FF 7E 06 C5 FA 10 06 EB 08 C5 FA 10 05 12 A2 46 00 48 8D 05 C7 C5 65
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD060ull))(argc, argv);
}

// Lisp: (journal-set-page ...) | StringId: 0xE9C1106D | Guest Offset: 0x009FD130
int64_t DcJournalSetPage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 81 EC 10 01 00 00 4C 8B 35 BB DA 64 00 49 8B 06
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD130ull))(argc, argv);
}

// Lisp: (jump ...) | StringId: 0xB420E516 | Guest Offset: 0x00A43ED0
int64_t DcJump(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F6 41 89 FD 45 85 ED 0F
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A43ED0ull))(argc, argv);
}

// Lisp: (kill-all-attachables ...) | StringId: 0x7B197528 | Guest Offset: 0x009F2BF0
int64_t DcKillAllAttachables(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 48 8B 05 F5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009F2BF0ull))(argc, argv);
}

// Lisp: (kill-all-breakables ...) | StringId: 0xA8024193 | Guest Offset: 0x00A0F260
int64_t DcKillAllBreakables(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 8D 05 33 FF 74 00 48 8B B8 58 0C 00 00 31 F6 EB 0D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0F260ull))(argc, argv);
}

// Lisp: (kill-all-dropped-weapons ...) | StringId: 0x50DC2915 | Guest Offset: 0x00A0F1B0
int64_t DcKillAllDroppedWeapons(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 48 8D 05 DF FF 74 00 48 8B B8 90 0C 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0F1B0ull))(argc, argv);
}

// Lisp: (kill-all-particles ...) | StringId: 0xE178EA44 | Guest Offset: 0x00A0FE60
int64_t DcKillAllParticles(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 09 20 9C 00 C6 80 54 0F 00 00 01 31 C0 C3 66 66 66 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0FE60ull))(argc, argv);
}

// Lisp: (kill-dropped-weapons-in-sphere ...) | StringId: 0x8DAC1F65 | Guest Offset: 0x00A09B10
int64_t DcKillDroppedWeaponsInSphere(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 20 85 FF 0F 8E 23 02 00 00 83 FF 02 0F 8C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A09B10ull))(argc, argv);
}

// Lisp: (kill-entity ...) | StringId: 0xDF781FBF | Guest Offset: 0x00A09D60
int64_t DcKillEntity(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 D6 49 89 F7 85 FF 7E 0E 41 8B 1F 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A09D60ull))(argc, argv);
}

// Lisp: (kill-flashlight ...) | StringId: 0x46EABE53 | Guest Offset: 0x00A0A130
int64_t DcKillFlashlight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 10 8B 1E 31 F6 89 DF E8 57 92 2D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0A130ull))(argc, argv);
}

// Lisp: (kill-message ...) | StringId: 0x4052E46E | Guest Offset: 0x005DC630
int64_t DcKillMessage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 8B 36 48 8D 3D 63 D8 B5 00 E8 3E D7 FF FF 31 C0 5D C3 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DC630ull))(argc, argv);
}

// Lisp: (kill-part-spawner ...) | StringId: 0x0AB7AFB8 | Guest Offset: 0x00A099B0
int64_t DcKillPartSpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 49 89 F7 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A099B0ull))(argc, argv);
}

// Lisp: (kill-particles ...) | StringId: 0x389CE791 | Guest Offset: 0x00A00140
int64_t DcKillParticles(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 48 89 F3 41 89 FF 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A00140ull))(argc, argv);
}

// Lisp: (kill-player ...) | StringId: 0x2FAE80C5 | Guest Offset: 0x0090C790
int64_t DcKillPlayer(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 4C 8B 3D 59 E4 73 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090C790ull))(argc, argv);
}

// Lisp: (kill-player-flashlight ...) | StringId: 0x94008BCE | Guest Offset: 0x00A0A920
int64_t DcKillPlayerFlashlight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 81 EC B0 01 00 00 48 8B 1D CB 02 64 00 48 8B 03
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0A920ull))(argc, argv);
}

// Lisp: (kill-player-no-anim ...) | StringId: 0xC63EAEFA | Guest Offset: 0x0090C5F0
int64_t DcKillPlayerNoAnim(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 90 00 00 00 4C 8B 35 F7 E5 73
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090C5F0ull))(argc, argv);
}

// Lisp: (kill-sullivan-flashlight ...) | StringId: 0xEAD294BD | Guest Offset: 0x00A0A750
int64_t DcKillSullivanFlashlight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 18 01 00 00 4C 8B 3D 99 04 64 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0A750ull))(argc, argv);
}

// Lisp: (layer-to-script-controller-indices ...) | StringId: 0xF5F06733 | Guest Offset: 0x00EA2F7E
int64_t DcLayerToScriptControllerIndices(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 6C 61 79 65 72 20 3D 3D 20 44 43 3A 3A 6B 41 6E 69 6D 61 74 65 4C 61 79
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00EA2F7Eull))(argc, argv);
}

// Lisp: (lerp-reflection-plane-of-water-units ...) | StringId: 0xEDF05AAB | Guest Offset: 0x00A02B10
int64_t DcLerpReflectionPlaneOfWaterUnits(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 02 7C 39 83 3E 00 BB 01 00 00 00 74 3F C5 FA 10
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02B10ull))(argc, argv);
}

// Lisp: (lerp-reflection-planes ...) | StringId: 0x26E8F878 | Guest Offset: 0x00A02AA0
int64_t DcLerpReflectionPlanes(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 03 7C 3C 48 8B 06 48 8B 4E 08 C5 FA 10 46 10 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02AA0ull))(argc, argv);
}

// Lisp: (lerp-reflection-planes-by-rotation ...) | StringId: 0x0EB4D1E7 | Guest Offset: 0x00A02A20
int64_t DcLerpReflectionPlanesByRotation(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 02 7C 5B 48 8B 06 C5 FA 10 46 08 C5 FA 10 4E 10
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02A20ull))(argc, argv);
}

// Lisp: (lerp-scale ...) | StringId: 0x40274C92 | Guest Offset: 0x009FDB90
int64_t DcLerpScale(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: C5 FA 10 06 C5 FA 10 4E 08 C5 FA 10 56 20 C5 EA 5C D0 C5 F2 5C C0 C5 EA
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDB90ull))(argc, argv);
}

// Lisp: (letterbox-off ...) | StringId: 0x624BF689 | Guest Offset: 0x009FFD60
int64_t DcLetterboxOff(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 0D 8B 1E 31 F6 89 DF E8 2B 36 2E 00 EB 14 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FFD60ull))(argc, argv);
}

// Lisp: (letterbox-on ...) | StringId: 0x32839068 | Guest Offset: 0x009FFE00
int64_t DcLetterboxOn(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 20 4C 8B 35 EE AD 64 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FFE00ull))(argc, argv);
}

// Lisp: (levels-fully-spawned-ignore-player? ...) | StringId: 0xA20CB8D2 | Guest Offset: 0x009FE400
int64_t DcLevelsFullySpawnedIgnorePlayerP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 38 49 89 F7 89 7D BC 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE400ull))(argc, argv);
}

// Lisp: (levels-fully-spawned? ...) | StringId: 0x4553458F | Guest Offset: 0x009FE5E0
int64_t DcLevelsFullySpawnedP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 38 49 89 F6 41 89 FF 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE5E0ull))(argc, argv);
}

// Lisp: (look-at ...) | StringId: 0x58B2D6FD | Guest Offset: 0x002E4C80
int64_t DcLookAt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC A8 00 00 00 49 89 F4 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E4C80ull))(argc, argv);
}

// Lisp: (look-at-object ...) | StringId: 0xEA4F3C96 | Guest Offset: 0x002E4A40
int64_t DcLookAtObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 49 89 F5 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E4A40ull))(argc, argv);
}

// Lisp: (make-active-character-more-wet ...) | StringId: 0x8CDD98D5 | Guest Offset: 0x00A020E0
int64_t DcMakeActiveCharacterMoreWet(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 85 FF 0F 8E D3 00 00 00 48 8D 05 EE FD
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A020E0ull))(argc, argv);
}

// Lisp: (make-character-more-wet ...) | StringId: 0x5B488D87 | Guest Offset: 0x00A023C0
int64_t DcMakeCharacterMoreWet(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 0F 8E A5 00 00 00 8B 06 BB 01 00 00 00 48 83 F8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A023C0ull))(argc, argv);
}

// Lisp: (make-inert ...) | StringId: 0x8E947C31 | Guest Offset: 0x009FD850
int64_t DcMakeInert(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 05 6D 8E 66 00 48 8D 35 8B 02 4A 00 31 FF BA 01 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD850ull))(argc, argv);
}

// Lisp: (make-vector ...) | StringId: 0xB021733F | Guest Offset: 0x009FDCF0
int64_t DcMakeVector(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 58 48 8B 1D 00 CF 64 00 48 8B 03 48 89 45 F0 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDCF0ull))(argc, argv);
}

// Lisp: (make-vector-3 ...) | StringId: 0xB90400B2 | Guest Offset: 0x009FDC20
int64_t DcMakeVector3(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 28 48 8B 1D D0 CF 64 00 48 8B 03 48 89 45 F0 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDC20ull))(argc, argv);
}

// Lisp: (message ...) | StringId: 0xC64E7E79 | Guest Offset: 0x005DC620
int64_t DcMessage(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (minmax ...) | StringId: 0x29BA2151 | Guest Offset: 0x009FDBF0
int64_t DcMinmax(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: C5 FA 10 06 C5 FA 5F 46 08 C5 FA 5D 46 10 48 C7 44 24 F8 00 00 00 00 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDBF0ull))(argc, argv);
}

// Lisp: (mirror-aiming-at ...) | StringId: 0xCFCCAA8C | Guest Offset: 0x009FC990
int64_t DcMirrorAimingAt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 90 00 00 00 49 89 F7 4C 8B 25
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FC990ull))(argc, argv);
}

// Lisp: (net-all-players-dead? ...) | StringId: 0xEB633936 | Guest Offset: 0x006694B0
int64_t DcNetAllPlayersDeadP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 40 17 9E 00 48 8B 03 48 89 45 F0 C7
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x006694B0ull))(argc, argv);
}

// Lisp: (net-all-players-in-region? ...) | StringId: 0xB9C4FE7A | Guest Offset: 0x00669500
int64_t DcNetAllPlayersInRegionP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 85 FF 7E 0F 44 8B 36 31 F6 44 89 F7 E8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00669500ull))(argc, argv);
}

// Lisp: (net-capture-flag ...) | StringId: 0x4D52E367 | Guest Offset: 0x00669960
int64_t DcNetCaptureFlag(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 49 89 F7 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00669960ull))(argc, argv);
}

// Lisp: (net-coop-host-script-id ...) | StringId: 0x7708F645 | Guest Offset: 0x00669C40
int64_t DcNetCoopHostScriptId(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (net-coop-script-id ...) | StringId: 0xC12DB877 | Guest Offset: 0x00669C30
int64_t DcNetCoopScriptId(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (net-coop-task ...) | StringId: 0xA615D1D6 | Guest Offset: 0x006697E0
int64_t DcNetCoopTask(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 BF 7D 06 9A 3B E8 82 C6 3A 00 31 C0 5D C3 66 66 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x006697E0ull))(argc, argv);
}

// Lisp: (net-coop-team-failed? ...) | StringId: 0xBFE7BA4E | Guest Offset: 0x00669C20
int64_t DcNetCoopTeamFailedP(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv;
    return *(uint8_t *)(image + 0x0113D910);
}

// Lisp: (net-current-net-task ...) | StringId: 0x601D65AB | Guest Offset: 0x00669780
int64_t DcNetCurrentNetTask(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 BF 7D 06 9A 3B E8 E2 C6 3A 00 48 8D 15 CB 1F AD 00 48 0F BF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00669780ull))(argc, argv);
}

// Lisp: (net-draw-objective-icon ...) | StringId: 0x1F8C0A58 | Guest Offset: 0x00669250
int64_t DcNetDrawObjectiveIcon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 49 89 F6 41 89 FF 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00669250ull))(argc, argv);
}

// Lisp: (net-end-wave ...) | StringId: 0xA3251A6A | Guest Offset: 0x006687E0
int64_t DcNetEndWave(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 E8 67 F5 FA FF 31 C0 5D C3 0F 1F 00 55 48 89 E5 BF DB D2 87
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x006687E0ull))(argc, argv);
}

// Lisp: (net-get-booster-amount ...) | StringId: 0x28F84ACC | Guest Offset: 0x00668820
int64_t DcNetGetBoosterAmount(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 8B 36 48 8D 3D 57 1F AD 00 E8 DA DB F8 FF 48 C7
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00668820ull))(argc, argv);
}

// Lisp: (net-get-booster-enabled ...) | StringId: 0x344C16FE | Guest Offset: 0x00668810
int64_t DcNetGetBoosterEnabled(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 8B 36 E8 65 D8 F8 FF 0F B6 C0 5D C3 55 48 89 E5 48 83 EC 10
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00668810ull))(argc, argv);
}

// Lisp: (net-get-grapple-time ...) | StringId: 0x8F971AB5 | Guest Offset: 0x006687F0
int64_t DcNetGetGrappleTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 BF DB D2 87 9D E8 72 D6 3A 00 48 8B 00 5D C3 66 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x006687F0ull))(argc, argv);
}

// Lisp: (net-heli-move-on-spline ...) | StringId: 0xEE072D9A | Guest Offset: 0x00A7FCC0
int64_t DcNetHeliMoveOnSpline(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F5 89 FB 85 DB 7E 31 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7FCC0ull))(argc, argv);
}

// Lisp: (net-hide-wave-screen ...) | StringId: 0x9EF00F55 | Guest Offset: 0x00668850
int64_t DcNetHideWaveScreen(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 3D 1F 61 E6 00 01 7E 12 48 8D 05 DE 6C 9E 00 C6 40
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00668850ull))(argc, argv);
}

// Lisp: (net-hud-set-grapple-indicator ...) | StringId: 0x6BDF859B | Guest Offset: 0x00668980
int64_t DcNetHudSetGrappleIndicator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 3D EF 5F E6 00 01 7E 55 8B 06 85 C0 74 58 C5 FA 10
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00668980ull))(argc, argv);
}

// Lisp: (net-is-cinema ...) | StringId: 0x89676506 | Guest Offset: 0x00669C50
int64_t DcNetIsCinema(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 83 3D 25 4D E6 00 02 7C 15 48 8D 05 00 1B AD 00 80 B8 9C 20 00 00 00 0F
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00669C50ull))(argc, argv);
}

// Lisp: (net-is-game-type? ...) | StringId: 0xD7BCF149 | Guest Offset: 0x00667E60
int64_t DcNetIsGameTypeP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 49 89 F6 89 FB 83 3D 09 6B E6 00 01 7E 53 BF 7D 06
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00667E60ull))(argc, argv);
}

// Lisp: (net-is-host ...) | StringId: 0x4E80EEA5 | Guest Offset: 0x00669C80
int64_t DcNetIsHost(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 83 3D F5 4C E6 00 02 7C 27 48 8D 0D 78 BE 9F 00 B0 01 48 83 79 10 00 74
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00669C80ull))(argc, argv);
}

// Lisp: (net-kill-entity ...) | StringId: 0xEF97DF8A | Guest Offset: 0x006696A0
int64_t DcNetKillEntity(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 20 48 89 F3 4C 8B 3D 47 15 9E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x006696A0ull))(argc, argv);
}

// Lisp: (net-kill-territory ...) | StringId: 0x3C7B4B72 | Guest Offset: 0x00669800
int64_t DcNetKillTerritory(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 83 3D 71 51 E6 00 02 7C 2C 48 8D 05 F4 C2 9F 00 48 83 78 10
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00669800ull))(argc, argv);
}

// Lisp: (net-npc-confirm-coop-players-contact ...) | StringId: 0x2335480A | Guest Offset: 0x006690C0
int64_t DcNetNpcConfirmCoopPlayersContact(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 44 8B 36 48 8D 3D E0 4B AA 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x006690C0ull))(argc, argv);
}

// Lisp: (net-plyaer-coop-id ...) | StringId: 0x3408A884 | Guest Offset: 0x00669240
int64_t DcNetPlyaerCoopId(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (net-respawn-flag ...) | StringId: 0xDB671BC1 | Guest Offset: 0x006698E0
int64_t DcNetRespawnFlag(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 AA 9A 67 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x006698E0ull))(argc, argv);
}

// Lisp: (net-revive-player ...) | StringId: 0x7FADF1FF | Guest Offset: 0x00667D40
int64_t DcNetRevivePlayer(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 81 EC 88 00 00 00 48 8B 1D AD 2E 9E 00 48 8B 03 48 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00667D40ull))(argc, argv);
}

// Lisp: (net-send-event ...) | StringId: 0x672AE8AA | Guest Offset: 0x004C3B60
int64_t DcNetSendEvent(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 58 49 89 F6 41 89 FC 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C3B60ull))(argc, argv);
}

// Lisp: (net-show-wave-screen ...) | StringId: 0x25C46C02 | Guest Offset: 0x00668890
int64_t DcNetShowWaveScreen(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 3D DF 60 E6 00 01 7E 50 8B 06 48 8D 1D B8 2E AD 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00668890ull))(argc, argv);
}

// Lisp: (net-spawn-flag ...) | StringId: 0x938F6AAF | Guest Offset: 0x006698C0
int64_t DcNetSpawnFlag(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 3D 95 1E AD 00 31 D2 E8 DE 48 FD FF 31 C0 5D C3 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x006698C0ull))(argc, argv);
}

// Lisp: (net-spawn-territory ...) | StringId: 0x204CBFB2 | Guest Offset: 0x00669840
int64_t DcNetSpawnTerritory(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 05 15 1F AD 00 44 8B 80 D0 1D 00 00 31 D2 BE 00 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00669840ull))(argc, argv);
}

// Lisp: (npc-add-skill ...) | StringId: 0xED41C60B | Guest Offset: 0x002E3E70
int64_t DcNpcAddSkill(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FF 45 85 FF 7E 0E 41 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3E70ull))(argc, argv);
}

// Lisp: (npc-add-traversal-skill ...) | StringId: 0x63B30199 | Guest Offset: 0x002DE790
int64_t DcNpcAddTraversalSkill(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 41 89 FF 45 85 FF 7E 0E 41 8B 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DE790ull))(argc, argv);
}

// Lisp: (npc-aim-at ...) | StringId: 0xCDDA1161 | Guest Offset: 0x002E0A50
int64_t DcNpcAimAt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 F7 89 FB 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E0A50ull))(argc, argv);
}

// Lisp: (npc-alive-count ...) | StringId: 0x1A69B5AD | Guest Offset: 0x002DF000
int64_t DcNpcAliveCount(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 B1 EC E2 00 48 63 00 C3 0F 1F 44 00 00 55 48 89 E5 41 57 41 56
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DF000ull))(argc, argv);
}

// Lisp: (npc-alive? ...) | StringId: 0xE5325BD4 | Guest Offset: 0x002DD900
int64_t DcNpcAliveP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 0D 8B 1E 31 F6 89 DF E8 8B 5A A0 00 EB 14 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD900ull))(argc, argv);
}

// Lisp: (npc-attach-to-battle-manager ...) | StringId: 0x4E3F7482 | Guest Offset: 0x002E0530
int64_t DcNpcAttachToBattleManager(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 49 89 F6 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E0530ull))(argc, argv);
}

// Lisp: (npc-can-see? ...) | StringId: 0x55345489 | Guest Offset: 0x002DA420
int64_t DcNpcCanSeeP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 41 89 FD 45 85 ED 7E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DA420ull))(argc, argv);
}

// Lisp: (npc-change-sniper-params ...) | StringId: 0xF9516DB0 | Guest Offset: 0x002DED80
int64_t DcNpcChangeSniperParams(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 2A 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DED80ull))(argc, argv);
}

// Lisp: (npc-change-vision-settings ...) | StringId: 0x1272E557 | Guest Offset: 0x002DA600
int64_t DcNpcChangeVisionSettings(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 89 FB 85 DB 7E 25 45 8B 27 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DA600ull))(argc, argv);
}

// Lisp: (npc-collect-one ...) | StringId: 0x8FF445E9 | Guest Offset: 0x002E4FB0
int64_t DcNpcCollectOne(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC D8 00 00 00 48 8B 1D 35
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E4FB0ull))(argc, argv);
}

// Lisp: (npc-combat-behavior-time ...) | StringId: 0x84BE6CF9 | Guest Offset: 0x002DD720
int64_t DcNpcCombatBehaviorTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 48 C7 45 E8 00 00 00 00 C7 45 E8 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD720ull))(argc, argv);
}

// Lisp: (npc-destroy-held-object ...) | StringId: 0xC4CAC9A0 | Guest Offset: 0x002DCB10
int64_t DcNpcDestroyHeldObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 81 EC 90 00 00 00 4C 8B 35 DB E0 D6 00 49 8B 06
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DCB10ull))(argc, argv);
}

// Lisp: (npc-die ...) | StringId: 0x238A99A6 | Guest Offset: 0x002E4680
int64_t DcNpcDie(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 89 FB 85 DB 7E 12 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E4680ull))(argc, argv);
}

// Lisp: (npc-disable-deal-no-damage-when-offscreen ...) | StringId: 0x95D8CFE0 | Guest Offset: 0x002DEF60
int64_t DcNpcDisableDealNoDamageWhenOffscreen(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 90 BC D6 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DEF60ull))(argc, argv);
}

// Lisp: (npc-disable-flinch ...) | StringId: 0x8E1197F4 | Guest Offset: 0x002DC1D0
int64_t DcNpcDisableFlinch(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 10 8B 1E 31 F6 89 DF E8 B7 71 A0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DC1D0ull))(argc, argv);
}

// Lisp: (npc-drop-object ...) | StringId: 0x2E1D3291 | Guest Offset: 0x002DCC50
int64_t DcNpcDropObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 81 EC 90 00 00 00 4C 8B 35 9B DF D6 00 49 8B 06
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DCC50ull))(argc, argv);
}

// Lisp: (npc-drop-weapon ...) | StringId: 0x94A52BA0 | Guest Offset: 0x002DB680
int64_t DcNpcDropWeapon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC A0 00 00 00 4C 8B 25 67 F5 D6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DB680ull))(argc, argv);
}

// Lisp: (npc-enable-block-first-tap-delay ...) | StringId: 0xF6CB805B | Guest Offset: 0x002E3B50
int64_t DcNpcEnableBlockFirstTapDelay(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 41 89 FF 45 85 FF 7E 0E 41 8B 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3B50ull))(argc, argv);
}

// Lisp: (npc-enable-hit-reactions ...) | StringId: 0x63E74530 | Guest Offset: 0x002DD4B0
int64_t DcNpcEnableHitReactions(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 41 89 FF 45 85 FF 7E 0E 41 8B 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD4B0ull))(argc, argv);
}

// Lisp: (npc-enable-shooting ...) | StringId: 0x2FC044DE | Guest Offset: 0x002DC680
int64_t DcNpcEnableShooting(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F4 83 FF 02 0F 8C B7 00 00 00 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DC680ull))(argc, argv);
}

// Lisp: (npc-entered-action-pack? ...) | StringId: 0x9754326D | Guest Offset: 0x002DFFB0
int64_t DcNpcEnteredActionPackP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 DA 33 A0 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DFFB0ull))(argc, argv);
}

// Lisp: (npc-face ...) | StringId: 0xF3906882 | Guest Offset: 0x002DBB30
int64_t DcNpcFace(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 20 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DBB30ull))(argc, argv);
}

// Lisp: (npc-fire-weapon ...) | StringId: 0xE3B772A8 | Guest Offset: 0x002DBE80
int64_t DcNpcFireWeapon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 12 44 8B 3E 31 F6 44 89 FF E8 05
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DBE80ull))(argc, argv);
}

// Lisp: (npc-fired-recently? ...) | StringId: 0x50B1376B | Guest Offset: 0x002DEAB0
int64_t DcNpcFiredRecentlyP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 12 44 8B 3E 31 F6 44 89 FF E8 D5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DEAB0ull))(argc, argv);
}

// Lisp: (npc-grab-object ...) | StringId: 0x91717C44 | Guest Offset: 0x002DCD90
int64_t DcNpcGrabObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 48 89 F3 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DCD90ull))(argc, argv);
}

// Lisp: (npc-group-alive-count ...) | StringId: 0xE8C6C993 | Guest Offset: 0x002DB160
int64_t DcNpcGroupAliveCount(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D 5D 5F B9 00 31 D2 E9 92 FD FF FF 66 90 55 48 89 E5 41 57 41 56
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DB160ull))(argc, argv);
}

// Lisp: (npc-group-dead-count ...) | StringId: 0x034B1021 | Guest Offset: 0x002DAEE0
int64_t DcNpcGroupDeadCount(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D 07 62 B9 00 BA 01 00 00 00 E9 0F 00 00 00 66 66 66 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DAEE0ull))(argc, argv);
}

// Lisp: (npc-group-find-first-alive ...) | StringId: 0x97E6E340 | Guest Offset: 0x002D7160
int64_t DcNpcGroupFindFirstAlive(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 4C 8B 2D 88 3A D7 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002D7160ull))(argc, argv);
}

// Lisp: (npc-has-confirmed-status? ...) | StringId: 0x2361A73D | Guest Offset: 0x002E2160
int64_t DcNpcHasConfirmedStatusP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 89 FB 85 DB 7E 25 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E2160ull))(argc, argv);
}

// Lisp: (npc-hunt ...) | StringId: 0x85D88BD8 | Guest Offset: 0x002DDA30
int64_t DcNpcHunt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 F5 89 FB 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DDA30ull))(argc, argv);
}

// Lisp: (npc-ignore-explosion-damage ...) | StringId: 0x341E10D7 | Guest Offset: 0x002DC9C0
int64_t DcNpcIgnoreExplosionDamage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 49 89 F6 83 FF 02 75 63 41 8B 1E 31 F6 89 DF E8 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DC9C0ull))(argc, argv);
}

// Lisp: (npc-in-combat? ...) | StringId: 0xEB265E4D | Guest Offset: 0x002DD830
int64_t DcNpcInCombatP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 0D 8B 1E 31 F6 89 DF E8 5A 5B A0 00 EB 14
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD830ull))(argc, argv);
}

// Lisp: (npc-in-melee-with-player? ...) | StringId: 0x56A2902B | Guest Offset: 0x002E0050
int64_t DcNpcInMeleeWithPlayerP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 3A 33 A0 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E0050ull))(argc, argv);
}

// Lisp: (npc-is-in-scripted-anim? ...) | StringId: 0x0B765DC4 | Guest Offset: 0x002DEEC0
int64_t DcNpcIsInScriptedAnimP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 12 44 8B 3E 31 F6 44 89 FF E8 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DEEC0ull))(argc, argv);
}

// Lisp: (npc-is-nav-map-position-blocked? ...) | StringId: 0xC73764DF | Guest Offset: 0x002E07A0
int64_t DcNpcIsNavMapPositionBlockedP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 20 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E07A0ull))(argc, argv);
}

// Lisp: (npc-is-player-in-front? ...) | StringId: 0x7ADD53D9 | Guest Offset: 0x002DF270
int64_t DcNpcIsPlayerInFrontP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 12 44 8B 3E 31 F6 44 89 FF E8 15
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DF270ull))(argc, argv);
}

// Lisp: (npc-killed? ...) | StringId: 0xE4FC48C3 | Guest Offset: 0x002DD990
int64_t DcNpcKilledP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 0D 8B 1E 31 F6 89 DF E8 FB 59 A0 00 EB 14 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD990ull))(argc, argv);
}

// Lisp: (npc-laz-set-param ...) | StringId: 0x8E54B88D | Guest Offset: 0x002DA290
int64_t DcNpcLazSetParam(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 89 FB 85 DB 7E 1A 45 8B 3E 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DA290ull))(argc, argv);
}

// Lisp: (npc-lower-weapon ...) | StringId: 0xD96C9F58 | Guest Offset: 0x002DBD00
int64_t DcNpcLowerWeapon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 8A 76 A0 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DBD00ull))(argc, argv);
}

// Lisp: (npc-make-others-aware-of ...) | StringId: 0x95CA7E04 | Guest Offset: 0x002DC780
int64_t DcNpcMakeOthersAwareOf(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 85 FF 7E 0F 44 8B 3E 31 F6 44
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DC780ull))(argc, argv);
}

// Lisp: (npc-mark-as-defunct ...) | StringId: 0x1A6601A0 | Guest Offset: 0x002E4F60
int64_t DcNpcMarkAsDefunct(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 8B 1E 48 8D 3D 49 8D E2 00 89 DE E8 DA CC F4 FF 48 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E4F60ull))(argc, argv);
}

// Lisp: (npc-max-count ...) | StringId: 0x1A933D69 | Guest Offset: 0x002DEFF0
int64_t DcNpcMaxCount(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: B8 0F 00 00 00 C3 66 2E 0F 1F 84 00 00 00 00 00 48 8D 05 B1 EC E2 00 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DEFF0ull))(argc, argv);
}

// Lisp: (npc-move-to ...) | StringId: 0x7CB0BB43 | Guest Offset: 0x002E1C30
int64_t DcNpcMoveTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 89 D0 31 D2 48 89 C1 E9 C3 F9 FF FF 0F 1F 00 55 48 89 E5 41 57 41 56
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E1C30ull))(argc, argv);
}

// Lisp: (npc-move-to-spawner ...) | StringId: 0xB89B4C90 | Guest Offset: 0x002E15E0
int64_t DcNpcMoveToSpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 89 D0 31 D2 48 89 C1 E9 33 F9 FF FF 0F 1F 00 48 89 D0 BA 01 00 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E15E0ull))(argc, argv);
}

// Lisp: (npc-movement-speed ...) | StringId: 0xF654643C | Guest Offset: 0x002DCA40
int64_t DcNpcMovementSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 48 89 F3 83 FF 02 75 74 44 8B 33 31 F6 44
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DCA40ull))(argc, argv);
}

// Lisp: (npc-my-battle-manager-id ...) | StringId: 0x2BBEE040 | Guest Offset: 0x002E4EF0
int64_t DcNpcMyBattleManagerId(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 8B 1E 48 8D 3D B9 8D E2 00 89 DE E8 4A CD F4 FF 48 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E4EF0ull))(argc, argv);
}

// Lisp: (npc-override-contact-status ...) | StringId: 0xBC4EAE4F | Guest Offset: 0x002DC2D0
int64_t DcNpcOverrideContactStatus(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F5 41 89 FF 45 85 FF 7E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DC2D0ull))(argc, argv);
}

// Lisp: (npc-play-facial-anim ...) | StringId: 0x01630394 | Guest Offset: 0x002DF010
int64_t DcNpcPlayFacialAnim(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC A8 00 00 00 49 89 F7 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DF010ull))(argc, argv);
}

// Lisp: (npc-playing-scripted-animation? ...) | StringId: 0x1E3D6840 | Guest Offset: 0x002DEA10
int64_t DcNpcPlayingScriptedAnimationP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 12 44 8B 3E 31 F6 44 89 FF E8 75
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DEA10ull))(argc, argv);
}

// Lisp: (npc-prefer-target ...) | StringId: 0x90362D38 | Guest Offset: 0x002DAD80
int64_t DcNpcPreferTarget(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 27 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DAD80ull))(argc, argv);
}

// Lisp: (npc-reload-weapon ...) | StringId: 0xEE5C93B6 | Guest Offset: 0x002DBDA0
int64_t DcNpcReloadWeapon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 EA 75 A0 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DBDA0ull))(argc, argv);
}

// Lisp: (npc-remove-skill ...) | StringId: 0xE7C8DB08 | Guest Offset: 0x002E3D70
int64_t DcNpcRemoveSkill(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F6 41 89 FF 45 85 FF 7E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3D70ull))(argc, argv);
}

// Lisp: (npc-remove-traversal-skill ...) | StringId: 0x56F22E2C | Guest Offset: 0x002DE6F0
int64_t DcNpcRemoveTraversalSkill(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 41 89 FF 45 85 FF 7E 0E 41 8B 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DE6F0ull))(argc, argv);
}

// Lisp: (npc-set-accuracy-multiplier ...) | StringId: 0x8715973E | Guest Offset: 0x002E00F0
int64_t DcNpcSetAccuracyMultiplier(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F4 89 FB 85 DB 7E 1F
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E00F0ull))(argc, argv);
}

// Lisp: (npc-set-allow-adjust-head-to-nav-map ...) | StringId: 0x76704022 | Guest Offset: 0x002E3C00
int64_t DcNpcSetAllowAdjustHeadToNavMap(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 41 89 FF 45 85 FF 7E 0E 41 8B 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3C00ull))(argc, argv);
}

// Lisp: (npc-set-ambient ...) | StringId: 0x2F06525E | Guest Offset: 0x002DA960
int64_t DcNpcSetAmbient(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 10 8B 1E 31 F6 89 DF E8 27 8A A0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DA960ull))(argc, argv);
}

// Lisp: (npc-set-buddy-sniper-hide-time ...) | StringId: 0x3480A7B1 | Guest Offset: 0x002DEC70
int64_t DcNpcSetBuddySniperHideTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 41 89 FD 45 85 ED 7E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DEC70ull))(argc, argv);
}

// Lisp: (npc-set-can-be-hit ...) | StringId: 0xF67CEE41 | Guest Offset: 0x002DD580
int64_t DcNpcSetCanBeHit(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 49 89 F6 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD580ull))(argc, argv);
}

// Lisp: (npc-set-can-die ...) | StringId: 0xB67490B3 | Guest Offset: 0x002DD410
int64_t DcNpcSetCanDie(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 41 89 FF 45 85 FF 7E 0E 41 8B 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD410ull))(argc, argv);
}

// Lisp: (npc-set-combat-behavior ...) | StringId: 0xBA80EF41 | Guest Offset: 0x002DE560
int64_t DcNpcSetCombatBehavior(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 89 FB 85 DB 7E 22 45 8B 3E 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DE560ull))(argc, argv);
}

// Lisp: (npc-set-comm-group-id ...) | StringId: 0x2FDF3330 | Guest Offset: 0x002E0430
int64_t DcNpcSetCommGroupId(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 89 FB 85 DB 7E 25 45 8B 27 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E0430ull))(argc, argv);
}

// Lisp: (npc-set-cover-advance-spline ...) | StringId: 0xA548A80A | Guest Offset: 0x002DA300
int64_t DcNpcSetCoverAdvanceSpline(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 89 FB 85 DB 7E 25 45 8B 27 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DA300ull))(argc, argv);
}

// Lisp: (npc-set-demeanor ...) | StringId: 0x6054BCDD | Guest Offset: 0x002DC0D0
int64_t DcNpcSetDemeanor(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 48 89 F3 41 89 FF 45 85 FF 7E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DC0D0ull))(argc, argv);
}

// Lisp: (npc-set-guard-radio-delay ...) | StringId: 0xFDD5F3A0 | Guest Offset: 0x002DEB60
int64_t DcNpcSetGuardRadioDelay(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F4 89 FB 85 DB 7E 1F
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DEB60ull))(argc, argv);
}

// Lisp: (npc-set-hunting-zone-center ...) | StringId: 0xB5AFE88E | Guest Offset: 0x002E3330
int64_t DcNpcSetHuntingZoneCenter(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 40 49 89 F6 89 FB 4C 8B 25 B5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3330ull))(argc, argv);
}

// Lisp: (npc-set-hunting-zone-marker ...) | StringId: 0x72A43DAC | Guest Offset: 0x002E3570
int64_t DcNpcSetHuntingZoneMarker(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 20 49 89 F7 89 FB 4C 8B 25 75
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3570ull))(argc, argv);
}

// Lisp: (npc-set-hunting-zone-radius ...) | StringId: 0x7CA728E6 | Guest Offset: 0x002E31C0
int64_t DcNpcSetHuntingZoneRadius(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 89 FB 85 DB 7E 1A 45 8B 37 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E31C0ull))(argc, argv);
}

// Lisp: (npc-set-hunting-zone-y-threshold ...) | StringId: 0x764D89D6 | Guest Offset: 0x002E3050
int64_t DcNpcSetHuntingZoneYThreshold(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 89 FB 85 DB 7E 1A 45 8B 37 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3050ull))(argc, argv);
}

// Lisp: (npc-set-ignore ...) | StringId: 0x2290EECD | Guest Offset: 0x002DD350
int64_t DcNpcSetIgnore(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 41 89 FF 45 85 FF 7E 0E 41 8B 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD350ull))(argc, argv);
}

// Lisp: (npc-set-in-combat ...) | StringId: 0xBD097FB5 | Guest Offset: 0x002DA720
int64_t DcNpcSetInCombat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 10 8B 1E 31 F6 89 DF E8 67 8C A0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DA720ull))(argc, argv);
}

// Lisp: (npc-set-lead-drake-spline ...) | StringId: 0x9DC7EC70 | Guest Offset: 0x002DFEE0
int64_t DcNpcSetLeadDrakeSpline(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 89 FB 85 DB 7E 22 45 8B 3E 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DFEE0ull))(argc, argv);
}

// Lisp: (npc-set-movement-speed-scale ...) | StringId: 0xFEC8D941 | Guest Offset: 0x002DD030
int64_t DcNpcSetMovementSpeedScale(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 41 89 FF 45 85 FF 7E 0E 41 8B 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD030ull))(argc, argv);
}

// Lisp: (npc-set-patrol-route ...) | StringId: 0x8070D2C0 | Guest Offset: 0x002DE620
int64_t DcNpcSetPatrolRoute(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FC 45 85 E4 7E 22 41 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DE620ull))(argc, argv);
}

// Lisp: (npc-set-perfect-aim ...) | StringId: 0x4D008BBC | Guest Offset: 0x002DC490
int64_t DcNpcSetPerfectAim(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 83 FF 02 0F 85 AF 00 00 00 41 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DC490ull))(argc, argv);
}

// Lisp: (npc-set-preoccupied ...) | StringId: 0xDE715E33 | Guest Offset: 0x002DA830
int64_t DcNpcSetPreoccupied(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 10 8B 1E 31 F6 89 DF E8 57 8B A0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DA830ull))(argc, argv);
}

// Lisp: (npc-set-shot-damage-multiplier ...) | StringId: 0x6157E60A | Guest Offset: 0x002DC570
int64_t DcNpcSetShotDamageMultiplier(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F4 83 FF 02 0F 8C B5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DC570ull))(argc, argv);
}

// Lisp: (npc-set-to-player-faction ...) | StringId: 0xEA79B72C | Guest Offset: 0x002DC960
int64_t DcNpcSetToPlayerFaction(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 01 75 4C 8B 1E 31 F6 89 DF E8 2A 6A A0 00 48 8D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DC960ull))(argc, argv);
}

// Lisp: (npc-set-weapon-visible ...) | StringId: 0xD331A5B5 | Guest Offset: 0x002DB170
int64_t DcNpcSetWeaponVisible(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 48 89 F3 83 FF 02 0F 85 F7 00 00 00 44 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DB170ull))(argc, argv);
}

// Lisp: (npc-set-zone-center ...) | StringId: 0xF87637A0 | Guest Offset: 0x002E3450
int64_t DcNpcSetZoneCenter(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 40 49 89 F6 89 FB 4C 8B 25 95
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3450ull))(argc, argv);
}

// Lisp: (npc-set-zone-marker ...) | StringId: 0x3F7DE282 | Guest Offset: 0x002E3AA0
int64_t DcNpcSetZoneMarker(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 20 49 89 F7 89 FB 4C 8B 25 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3AA0ull))(argc, argv);
}

// Lisp: (npc-set-zone-radius ...) | StringId: 0x317EF7C8 | Guest Offset: 0x002E3280
int64_t DcNpcSetZoneRadius(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 89 FB 85 DB 7E 1A 45 8B 37 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3280ull))(argc, argv);
}

// Lisp: (npc-set-zone-update-method ...) | StringId: 0x4AB54ECC | Guest Offset: 0x002DCF50
int64_t DcNpcSetZoneUpdateMethod(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 83 FF 02 0F 8C A1 00 00 00 41 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DCF50ull))(argc, argv);
}

// Lisp: (npc-set-zone-y-threshold ...) | StringId: 0x3EF5A899 | Guest Offset: 0x002E3110
int64_t DcNpcSetZoneYThreshold(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 89 FB 85 DB 7E 1A 45 8B 37 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3110ull))(argc, argv);
}

// Lisp: (npc-show-hit-effects ...) | StringId: 0x0B1421E2 | Guest Offset: 0x002DD1F0
int64_t DcNpcShowHitEffects(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 90 00 00 00 49 89 F6 41 89 FF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD1F0ull))(argc, argv);
}

// Lisp: (npc-sniper-disable-watch-area ...) | StringId: 0xF8B9878C | Guest Offset: 0x002DB890
int64_t DcNpcSniperDisableWatchArea(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 12 44 8B 3E 31 F6 44 89 FF E8 F5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DB890ull))(argc, argv);
}

// Lisp: (npc-sniper-set-watch-area ...) | StringId: 0xAEA6AF12 | Guest Offset: 0x002DB980
int64_t DcNpcSniperSetWatchArea(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 49 89 F7 89 FB 85 DB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DB980ull))(argc, argv);
}

// Lisp: (npc-stop ...) | StringId: 0x2B949798 | Guest Offset: 0x002DBF50
int64_t DcNpcStop(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 90 00 00 00 4C 8B 25 97 EC D6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DBF50ull))(argc, argv);
}

// Lisp: (npc-swap-with-ragdoll ...) | StringId: 0xC2633EBC | Guest Offset: 0x002DACE0
int64_t DcNpcSwapWithRagdoll(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 AA 86 A0 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DACE0ull))(argc, argv);
}

// Lisp: (npc-take-weapon ...) | StringId: 0xC52E64F4 | Guest Offset: 0x002DB320
int64_t DcNpcTakeWeapon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 89 FB 85 DB 7E 25 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DB320ull))(argc, argv);
}

// Lisp: (npc-target-in-zone? ...) | StringId: 0x4875B09D | Guest Offset: 0x002E2F20
int64_t DcNpcTargetInZoneP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 4C 8B 3D CC 7C D6 00 49 8B 07 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E2F20ull))(argc, argv);
}

// Lisp: (npc-throw-grenade-at-player ...) | StringId: 0x73F7057A | Guest Offset: 0x002E2350
int64_t DcNpcThrowGrenadeAtPlayer(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 68 4C 8B 3D 9C 88 D6 00 49 8B 07 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E2350ull))(argc, argv);
}

// Lisp: (npc-track-player-while-bound ...) | StringId: 0x016A8633 | Guest Offset: 0x002E1C40
int64_t DcNpcTrackPlayerWhileBound(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 25 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E1C40ull))(argc, argv);
}

// Lisp: (npc-trigger-ambush ...) | StringId: 0x2DE6BD3E | Guest Offset: 0x002DD0D0
int64_t DcNpcTriggerAmbush(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 0D 8B 1E 31 F6 89 DF E8 BA 62 A0 00 EB 14
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DD0D0ull))(argc, argv);
}

// Lisp: (npc-turn-to ...) | StringId: 0x46944D98 | Guest Offset: 0x002DFD20
int64_t DcNpcTurnTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 F7 89 FB 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DFD20ull))(argc, argv);
}

// Lisp: (npc-unbind ...) | StringId: 0x896FD647 | Guest Offset: 0x002E1D20
int64_t DcNpcUnbind(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 10 8B 1E 31 F6 89 DF E8 67 16 A0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E1D20ull))(argc, argv);
}

// Lisp: (npc-update-move-dest ...) | StringId: 0x196DB2D3 | Guest Offset: 0x002E01D0
int64_t DcNpcUpdateMoveDest(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F7 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E01D0ull))(argc, argv);
}

// Lisp: (npc-use-cap ...) | StringId: 0x1569BEFB | Guest Offset: 0x002E3F60
int64_t DcNpcUseCap(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 89 FB 85 DB 7E 22 45 8B 3E 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3F60ull))(argc, argv);
}

// Lisp: (npc-use-closest-cover ...) | StringId: 0x0DF8A402 | Guest Offset: 0x002E43B0
int64_t DcNpcUseClosestCover(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F6 41 89 FD 45 85 ED 7E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E43B0ull))(argc, argv);
}

// Lisp: (npc-use-scripted-ap ...) | StringId: 0xAF0B3CA0 | Guest Offset: 0x002DE250
int64_t DcNpcUseScriptedAp(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F6 89 FB 85 DB 7E 25 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DE250ull))(argc, argv);
}

// Lisp: (npc-using-skill? ...) | StringId: 0x4ED55C14 | Guest Offset: 0x002E3CB0
int64_t DcNpcUsingSkillP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 89 FB 85 DB 7E 1B 45 8B 37 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E3CB0ull))(argc, argv);
}

// Lisp: (npcs-get-player-position ...) | StringId: 0x69BEC34B | Guest Offset: 0x002DE830
int64_t DcNpcsGetPlayerPosition(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 48 8D 0D DB D4 E7 00 48 8B 01
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DE830ull))(argc, argv);
}

// Lisp: (object-in-region? ...) | StringId: 0x78543A1C | Guest Offset: 0x00A0FCA0
int64_t DcObjectInRegionP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 89 F3 41 89 FE 41 83 FE 01 0F 8E E1
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0FCA0ull))(argc, argv);
}

// Lisp: (object-on-object? ...) | StringId: 0x4A6F015A | Guest Offset: 0x00A04040
int64_t DcObjectOnObjectP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 89 FB 85 DB 7E 21 45 8B 37 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04040ull))(argc, argv);
}

// Lisp: (object-spawned? ...) | StringId: 0xA93D2926 | Guest Offset: 0x00A12780
int64_t DcObjectSpawnedP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 0D 8B 1E 31 F6 89 DF E8 0B 0C 2D 00 EB 14 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A12780ull))(argc, argv);
}

// Lisp: (ocean-bob-disable ...) | StringId: 0xA74871B2 | Guest Offset: 0x00A03390
int64_t DcOceanBobDisable(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (ocean-bob-enable ...) | StringId: 0x87461458 | Guest Offset: 0x00A033A0
int64_t DcOceanBobEnable(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (offer-contextual-entity ...) | StringId: 0xCCF7E165 | Guest Offset: 0x00A3FB60
int64_t DcOfferContextualEntity(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 0F 8E AF 00 00 00 44 8B 3E 45 85 FF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A3FB60ull))(argc, argv);
}

// Lisp: (offer-contextual-process ...) | StringId: 0x8BBC4B1E | Guest Offset: 0x00A3FA50
int64_t DcOfferContextualProcess(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 31 C0 41 BC 00 00 00 00 85 FF 7E 08 44
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A3FA50ull))(argc, argv);
}

// Lisp: (override-aim-fov ...) | StringId: 0x22E90916 | Guest Offset: 0x0090D7A0
int64_t DcOverrideAimFov(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: C5 FA 10 06 48 8D 05 65 20 74 00 C5 FA 11 80 D0 00 00 00 31 C0 C3 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D7A0ull))(argc, argv);
}

// Lisp: (pad-get-button-press-interval ...) | StringId: 0xD5305FD6 | Guest Offset: 0x00A0D060
int64_t DcPadGetButtonPressInterval(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 83 FF 02 0F 8C 2E 01 00 00 48 63 06 B9 01 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0D060ull))(argc, argv);
}

// Lisp: (pad-get-gesture-interval ...) | StringId: 0x14FD1F7B | Guest Offset: 0x00A0CEF0
int64_t DcPadGetGestureInterval(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 83 FF 02 0F 8C 2F 01
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0CEF0ull))(argc, argv);
}

// Lisp: (pad-get-info ...) | StringId: 0x6EFFA5EC | Guest Offset: 0x00A0D210
int64_t DcPadGetInfo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 48 89 F3 4C 8B 35 DB D9 63 00 49 8B 06
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0D210ull))(argc, argv);
}

// Lisp: (part-module-loaded? ...) | StringId: 0x77CF5D09 | Guest Offset: 0x00A00970
int64_t DcPartModuleLoadedP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 0D 8B 1E 31 F6 89 DF E8 1B 2A 2E 00 EB 14 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A00970ull))(argc, argv);
}

// Lisp: (pause-coop-combo ...) | StringId: 0x05740D1B | Guest Offset: 0x00667F60
int64_t DcPauseCoopCombo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 3D 75 20 AD 00 E8 60 68 F7 FF 31 C0 5D C3 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00667F60ull))(argc, argv);
}

// Lisp: (play-movie ...) | StringId: 0x0804EE71 | Guest Offset: 0x009FE100
int64_t DcPlayMovie(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 89 F3 4C 8D 35 CF 27 66 00 49 8B 3E 48 8B 07 FF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE100ull))(argc, argv);
}

// Lisp: (play-music-stinger ...) | StringId: 0x2F4147D8 | Guest Offset: 0x009FD3F0
int64_t DcPlayMusicStinger(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 5A 8B 1E 31 F6 89 DF E8 9A 5F 2E 00 4C 8D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD3F0ull))(argc, argv);
}

// Lisp: (play-sound ...) | StringId: 0x49B63DA6 | Guest Offset: 0x00A010C0
int64_t DcPlaySound(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 89 FB 85 DB 7E 29 45 8B 37 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A010C0ull))(argc, argv);
}

// Lisp: (play-sound-at-joint ...) | StringId: 0x3670E863 | Guest Offset: 0x00A00DA0
int64_t DcPlaySoundAtJoint(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 48 89 F3 41 89 FE 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A00DA0ull))(argc, argv);
}

// Lisp: (play-sound-at-position ...) | StringId: 0xB1969208 | Guest Offset: 0x00A00A30
int64_t DcPlaySoundAtPosition(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 20 49 89 F7 89 FB 4C 8B 25 B5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A00A30ull))(argc, argv);
}

// Lisp: (play-sound-attached ...) | StringId: 0x385747B6 | Guest Offset: 0x00A00F30
int64_t DcPlaySoundAttached(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 48 89 F3 41 89 FE 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A00F30ull))(argc, argv);
}

// Lisp: (player-allow-push-off-ledge-stealth-kill ...) | StringId: 0x6C5C0A69 | Guest Offset: 0x0090DF10
int64_t DcPlayerAllowPushOffLedgeStealthKill(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D E0 CC 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090DF10ull))(argc, argv);
}

// Lisp: (player-blind-fire-only-right ...) | StringId: 0xE253573D | Guest Offset: 0x00A0B720
int64_t DcPlayerBlindFireOnlyRight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D D0 F4 63 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0B720ull))(argc, argv);
}

// Lisp: (player-blindfire-targeting ...) | StringId: 0xA772A5A3 | Guest Offset: 0x0090F930
int64_t DcPlayerBlindfireTargeting(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 B2 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090F930ull))(argc, argv);
}

// Lisp: (player-can-melee ...) | StringId: 0xC7BB77F1 | Guest Offset: 0x00908570
int64_t DcPlayerCanMelee(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 80 26 74 00 48 8B 03 48 89 45 F0 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00908570ull))(argc, argv);
}

// Lisp: (player-can-melee-combo ...) | StringId: 0x98018A52 | Guest Offset: 0x00908310
int64_t DcPlayerCanMeleeCombo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D E0 28 74 00 48 8B 03 48 89 45 F0 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00908310ull))(argc, argv);
}

// Lisp: (player-check-for-got-hit ...) | StringId: 0x4A5E469F | Guest Offset: 0x00A0EF20
int64_t DcPlayerCheckForGotHit(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 0D F5 CD 74 00 48 8B 01 48 85 C0 74 19 8B 49 08 85 C9
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0EF20ull))(argc, argv);
}

// Lisp: (player-climb-up ...) | StringId: 0x928AC7FC | Guest Offset: 0x0090A8C0
int64_t DcPlayerClimbUp(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 98 00 00 00 48 8B 1D 29 03 74 00 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090A8C0ull))(argc, argv);
}

// Lisp: (player-climbing-up? ...) | StringId: 0xAB7B43B9 | Guest Offset: 0x0090C3D0
int64_t DcPlayerClimbingUpP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 0D 45 F9 84 00 48 8B 11 31 C0 48 85 D2 74 22 8B 49 08
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090C3D0ull))(argc, argv);
}

// Lisp: (player-disable-brutal-melee ...) | StringId: 0x8E83955A | Guest Offset: 0x0090E1F0
int64_t DcPlayerDisableBrutalMelee(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 00 CA 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E1F0ull))(argc, argv);
}

// Lisp: (player-disable-cover-exit-over ...) | StringId: 0x6F570C9D | Guest Offset: 0x0090F730
int64_t DcPlayerDisableCoverExitOver(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 B4 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090F730ull))(argc, argv);
}

// Lisp: (player-disable-cover-switch ...) | StringId: 0xE8426144 | Guest Offset: 0x0090FB30
int64_t DcPlayerDisableCoverSwitch(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 B0 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090FB30ull))(argc, argv);
}

// Lisp: (player-disable-dyno-drop-to-foreground ...) | StringId: 0xC2F6486D | Guest Offset: 0x0090EBA0
int64_t DcPlayerDisableDynoDropToForeground(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 50 C0 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090EBA0ull))(argc, argv);
}

// Lisp: (player-disable-edge-180 ...) | StringId: 0x79ECFBDC | Guest Offset: 0x0090EE30
int64_t DcPlayerDisableEdge180(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 BD 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090EE30ull))(argc, argv);
}

// Lisp: (player-disable-from-edge-jumps ...) | StringId: 0x75FDDB26 | Guest Offset: 0x0090E290
int64_t DcPlayerDisableFromEdgeJumps(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 60 C9 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E290ull))(argc, argv);
}

// Lisp: (player-disable-jump ...) | StringId: 0x4330E22D | Guest Offset: 0x0090ED90
int64_t DcPlayerDisableJump(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 60 BE 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090ED90ull))(argc, argv);
}

// Lisp: (player-disable-jump-away-from-edge ...) | StringId: 0xAC8A12D2 | Guest Offset: 0x00907030
int64_t DcPlayerDisableJumpAwayFromEdge(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 3B 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00907030ull))(argc, argv);
}

// Lisp: (player-disable-melee ...) | StringId: 0x38040704 | Guest Offset: 0x0090E9C0
int64_t DcPlayerDisableMelee(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 30 C2 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E9C0ull))(argc, argv);
}

// Lisp: (player-edge-character-relative-controls ...) | StringId: 0xCA1772C6 | Guest Offset: 0x0090DFB0
int64_t DcPlayerEdgeCharacterRelativeControls(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 40 CC 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090DFB0ull))(argc, argv);
}

// Lisp: (player-enable-climb-gear ...) | StringId: 0x56A5851C | Guest Offset: 0x00908220
int64_t DcPlayerEnableClimbGear(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D D0 29 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00908220ull))(argc, argv);
}

// Lisp: (player-enable-move-tracker ...) | StringId: 0x3CF1C20B | Guest Offset: 0x00909430
int64_t DcPlayerEnableMoveTracker(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 20 48 8B 1D BE 17 74 00 48 8B 03 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00909430ull))(argc, argv);
}

// Lisp: (player-facing-position? ...) | StringId: 0x11ACABB5 | Guest Offset: 0x0090BEE0
int64_t DcPlayerFacingPositionP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 90 00 00 00 4C 8B 35 07 ED 73
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090BEE0ull))(argc, argv);
}

// Lisp: (player-fire-at-reticle ...) | StringId: 0x0FC49C24 | Guest Offset: 0x0090E710
int64_t DcPlayerFireAtReticle(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 48 4C 8B 3D DC C4 73 00 49 8B 07 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E710ull))(argc, argv);
}

// Lisp: (player-force-aiming ...) | StringId: 0x0F13FDAA | Guest Offset: 0x0090B2F0
int64_t DcPlayerForceAiming(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 4C 8B 35 FC F8 73 00 49 8B 06 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090B2F0ull))(argc, argv);
}

// Lisp: (player-force-aiming-time ...) | StringId: 0x0FDCB8C4 | Guest Offset: 0x0090B1F0
int64_t DcPlayerForceAimingTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 00 FA 73 00 48 8B 03 48 89 45 F0 83
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090B1F0ull))(argc, argv);
}

// Lisp: (player-force-combo-light-tutorial ...) | StringId: 0xB01C8169 | Guest Offset: 0x00909150
int64_t DcPlayerForceComboLightTutorial(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D A0 1A 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00909150ull))(argc, argv);
}

// Lisp: (player-force-combo-medium-tutorial ...) | StringId: 0x3220EDD8 | Guest Offset: 0x00909240
int64_t DcPlayerForceComboMediumTutorial(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D B0 19 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00909240ull))(argc, argv);
}

// Lisp: (player-force-dyno-jump-up ...) | StringId: 0xC9B3D52A | Guest Offset: 0x009070D0
int64_t DcPlayerForceDynoJumpUp(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 20 3B 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009070D0ull))(argc, argv);
}

// Lisp: (player-force-edge-fall ...) | StringId: 0x792AC0C2 | Guest Offset: 0x0090CCC0
int64_t DcPlayerForceEdgeFall(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 8D 0D 53 F0 84 00 48 8B 01 48 85 C0 0F 84 57 01 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090CCC0ull))(argc, argv);
}

// Lisp: (player-force-fight-finisher ...) | StringId: 0x0FC620EA | Guest Offset: 0x00908C00
int64_t DcPlayerForceFightFinisher(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 49 89 F6 89 FB 4C 8B 3D E7 1F 74
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00908C00ull))(argc, argv);
}

// Lisp: (player-force-hanging ...) | StringId: 0xC3366842 | Guest Offset: 0x0090EED0
int64_t DcPlayerForceHanging(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 20 BD 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090EED0ull))(argc, argv);
}

// Lisp: (player-force-jump-at ...) | StringId: 0x3A646FB5 | Guest Offset: 0x0090B3D0
int64_t DcPlayerForceJumpAt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 20 4C 8B 35 1E F8 73 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090B3D0ull))(argc, argv);
}

// Lisp: (player-force-near-dyno-jumps ...) | StringId: 0x6694C1BA | Guest Offset: 0x00908180
int64_t DcPlayerForceNearDynoJumps(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 70 2A 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00908180ull))(argc, argv);
}

// Lisp: (player-force-no-stealth ...) | StringId: 0x15AEF90B | Guest Offset: 0x0090F830
int64_t DcPlayerForceNoStealth(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 B3 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090F830ull))(argc, argv);
}

// Lisp: (player-force-num-punches ...) | StringId: 0x87FBA57C | Guest Offset: 0x00908AF0
int64_t DcPlayerForceNumPunches(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 49 89 F6 89 FB 4C 8B 3D F7 20 74
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00908AF0ull))(argc, argv);
}

// Lisp: (player-force-stealth ...) | StringId: 0x184280D1 | Guest Offset: 0x0090FA30
int64_t DcPlayerForceStealth(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 B1 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090FA30ull))(argc, argv);
}

// Lisp: (player-foreground-edge-search ...) | StringId: 0x029BD1FC | Guest Offset: 0x0090EB00
int64_t DcPlayerForegroundEdgeSearch(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D F0 C0 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090EB00ull))(argc, argv);
}

// Lisp: (player-go-ragdoll ...) | StringId: 0xF1811405 | Guest Offset: 0x0090C4F0
int64_t DcPlayerGoRagdoll(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 81 EC 88 00 00 00 48 8B 1D FD E6 73 00 48 8B 03 48 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090C4F0ull))(argc, argv);
}

// Lisp: (player-hanging-on-bar? ...) | StringId: 0x92509722 | Guest Offset: 0x0090AB80
int64_t DcPlayerHangingOnBarP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 8D 0D 93 11 85 00 48 8B 01 48 85 C0 74 28 8B 49 08
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090AB80ull))(argc, argv);
}

// Lisp: (player-has-weapon-and-ammo? ...) | StringId: 0x548B9030 | Guest Offset: 0x00907290
int64_t DcPlayerHasWeaponAndAmmoP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 89 4A 85 00 48 8B 10 45 31 C0 B9 00 00 00 00 48 85 D2 74 1D 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00907290ull))(argc, argv);
}

// Lisp: (player-has-weapon-and-no-ammo? ...) | StringId: 0x03B61318 | Guest Offset: 0x009071B0
int64_t DcPlayerHasWeaponAndNoAmmoP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 69 4B 85 00 48 8B 10 45 31 C0 B9 00 00 00 00 48 85 D2 74 1D 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009071B0ull))(argc, argv);
}

// Lisp: (player-has-weapon? ...) | StringId: 0xF2DB441A | Guest Offset: 0x00906A50
int64_t DcPlayerHasWeaponP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 C9 52 85 00 48 8B 10 45 31 C0 B9 00 00 00 00 48 85 D2 74 1D 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00906A50ull))(argc, argv);
}

// Lisp: (player-hide-this-frame ...) | StringId: 0x3DC64B27 | Guest Offset: 0x0090E330
int64_t DcPlayerHideThisFrame(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D E9 D9 84 00 48 8B 01 48 85 C0 74 1B 8B 49 08 85 C9 74 14 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E330ull))(argc, argv);
}

// Lisp: (player-hide-weapons ...) | StringId: 0x15B08523 | Guest Offset: 0x0090E4A0
int64_t DcPlayerHideWeapons(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 50 C7 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E4A0ull))(argc, argv);
}

// Lisp: (player-in-combat? ...) | StringId: 0xC6728174 | Guest Offset: 0x0090DC20
int64_t DcPlayerInCombatP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 8D 05 84 64 85 00 B3 01 80 38 00 75 5B 83 3D 41 0D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090DC20ull))(argc, argv);
}

// Lisp: (player-in-cover? ...) | StringId: 0x97B2FD28 | Guest Offset: 0x0090BBB0
int64_t DcPlayerInCoverP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D 69 01 85 00 48 8B 01 48 85 C0 74 37 8B 49 08 85 C9 74 30 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090BBB0ull))(argc, argv);
}

// Lisp: (player-in-friendly-village ...) | StringId: 0xDD9574EB | Guest Offset: 0x0090EA60
int64_t DcPlayerInFriendlyVillage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 90 C1 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090EA60ull))(argc, argv);
}

// Lisp: (player-in-npc-way? ...) | StringId: 0xF69EB865 | Guest Offset: 0x0090F190
int64_t DcPlayerInNpcWayP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 85 FF 0F 8E 49 03 00 00 8B 36 E8 9D CC 0A 00 31 C9 48 85 C0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090F190ull))(argc, argv);
}

// Lisp: (player-in-region? ...) | StringId: 0xFAD44485 | Guest Offset: 0x0090BD70
int64_t DcPlayerInRegionP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 0F 8E C8 00 00 00 44 8B 36 31 DB 31 F6 44 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090BD70ull))(argc, argv);
}

// Lisp: (player-in-state? ...) | StringId: 0xE885C4D4 | Guest Offset: 0x0090BC10
int64_t DcPlayerInStateP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 4C 8B 3D D9 EF 73 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090BC10ull))(argc, argv);
}

// Lisp: (player-initiate-melee ...) | StringId: 0xCB550F6B | Guest Offset: 0x00909420
int64_t DcPlayerInitiateMelee(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 BA E5 84 00 C6 00 01 31 C0 C3 0F 1F 00 55 48 89 E5 41 56 53 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00909420ull))(argc, argv);
}

// Lisp: (player-instant-draw-weapon ...) | StringId: 0x033DE9F7 | Guest Offset: 0x0090DDE0
int64_t DcPlayerInstantDrawWeapon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 0F 8E 8A 00 00 00 48 8D 0D 2B DF 84 00 48 8B 01
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090DDE0ull))(argc, argv);
}

// Lisp: (player-is-aiming? ...) | StringId: 0x53838E80 | Guest Offset: 0x0090BC00
int64_t DcPlayerIsAimingP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 E8 67 4B 06 00 0F B6 C0 5D C3 66 90 55 48 89 E5 41 57 41 56
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090BC00ull))(argc, argv);
}

// Lisp: (player-lerp-anim-update-rate ...) | StringId: 0x1D9C28FC | Guest Offset: 0x003973F0
int64_t DcPlayerLerpAnimUpdateRate(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 48 8D 0D 1C 49 DC 00 48 8B 01 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x003973F0ull))(argc, argv);
}

// Lisp: (player-move-to ...) | StringId: 0x3B502BDB | Guest Offset: 0x00A0B680
int64_t DcPlayerMoveTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 85 FF 0F 8E 8A 00 00 00 48 8B 06 83 FF 02 7C 09 83 7E 08 00 0F 95 C1 EB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0B680ull))(argc, argv);
}

// Lisp: (player-no-edge-fall ...) | StringId: 0x6BDC75A8 | Guest Offset: 0x009080E0
int64_t DcPlayerNoEdgeFall(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 10 2B 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009080E0ull))(argc, argv);
}

// Lisp: (player-no-reload ...) | StringId: 0xEBFE460D | Guest Offset: 0x0090EC40
int64_t DcPlayerNoReload(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 20 4C 8B 35 AE BF 73 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090EC40ull))(argc, argv);
}

// Lisp: (player-on-jetski? ...) | StringId: 0xBB33D8A9 | Guest Offset: 0x00A01B80
int64_t DcPlayerOnJetskiP(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (player-on-narrow-edge ...) | StringId: 0x3385FA01 | Guest Offset: 0x0090E360
int64_t DcPlayerOnNarrowEdge(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 90 C8 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E360ull))(argc, argv);
}

// Lisp: (player-push-forward-speed ...) | StringId: 0x06F6D605 | Guest Offset: 0x00908750
int64_t DcPlayerPushForwardSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 8D 0D C3 35 85 00 48 8B 01 48 85 C0 74 72 8B 49 08
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00908750ull))(argc, argv);
}

// Lisp: (player-pushing? ...) | StringId: 0x68B188A0 | Guest Offset: 0x0090AC70
int64_t DcPlayerPushingP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 83 FF 01 75 5E 48 8D 0D 9D 10 85 00 48 8B 01 48 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090AC70ull))(argc, argv);
}

// Lisp: (player-pushing-speed ...) | StringId: 0x70E42A4C | Guest Offset: 0x0090ABE0
int64_t DcPlayerPushingSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 8D 0D 33 11 85 00 48 8B 01 48 85 C0 74 41 8B 49 08
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090ABE0ull))(argc, argv);
}

// Lisp: (player-remove-weapons ...) | StringId: 0x5BB8329C | Guest Offset: 0x00907170
int64_t DcPlayerRemoveWeapons(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 0D A5 4B 85 00 48 8B 01 48 85 C0 74 24 8B 49 08 85 C9
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00907170ull))(argc, argv);
}

// Lisp: (player-run-speed ...) | StringId: 0xC6E56494 | Guest Offset: 0x0090B180
int64_t DcPlayerRunSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 83 FF 02 75 4D C5 FA 10 06 C5 FA 10 4E 08 C5 E8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090B180ull))(argc, argv);
}

// Lisp: (player-save-weapon-set ...) | StringId: 0x9955E16F | Guest Offset: 0x0090B830
int64_t DcPlayerSaveWeaponSet(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 81 EC 88 00 00 00 48 8B 1D BD F3 73 00 48 8B 03 48 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090B830ull))(argc, argv);
}

// Lisp: (player-set-carry-npc-cam-offset ...) | StringId: 0x65E9551D | Guest Offset: 0x0090E170
int64_t DcPlayerSetCarryNpcCamOffset(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 48 C7 04 24 00 00 00 00 BE 80 01 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E170ull))(argc, argv);
}

// Lisp: (player-set-carry-npc-slow-walk ...) | StringId: 0x593B7A3D | Guest Offset: 0x0090FF30
int64_t DcPlayerSetCarryNpcSlowWalk(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 AC 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090FF30ull))(argc, argv);
}

// Lisp: (player-set-corner-climb-dist-scale ...) | StringId: 0x9B47D004 | Guest Offset: 0x0090DE90
int64_t DcPlayerSetCornerClimbDistScale(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 48 C7 04 24 00 00 00 00 BE 2C 01 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090DE90ull))(argc, argv);
}

// Lisp: (player-set-dyno-jump-aim-angle ...) | StringId: 0xC5DF59CE | Guest Offset: 0x0090E0F0
int64_t DcPlayerSetDynoJumpAimAngle(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 85 FF 7E 67 48 C7 04 24 00 00 00 00 BE
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E0F0ull))(argc, argv);
}

// Lisp: (player-set-prone-mode ...) | StringId: 0x88A2D5C2 | Guest Offset: 0x009089D0
int64_t DcPlayerSetProneMode(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 4C 8D 3D 3F 33 85 00 49 8B 07 48 85 C0 0F
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009089D0ull))(argc, argv);
}

// Lisp: (player-set-teaching-combo-mode ...) | StringId: 0x21E74B93 | Guest Offset: 0x00909330
int64_t DcPlayerSetTeachingComboMode(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D C0 18 74 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00909330ull))(argc, argv);
}

// Lisp: (player-take-melee-damage ...) | StringId: 0x42DBEBFE | Guest Offset: 0x009101A0
int64_t DcPlayerTakeMeleeDamage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 60 09 00 00 4C 8B 3D 47 AA 73
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009101A0ull))(argc, argv);
}

// Lisp: (player-teach-combo ...) | StringId: 0x6C6572C2 | Guest Offset: 0x005DCDB0
int64_t DcPlayerTeachCombo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 89 C8 A7 00 83 B8 B0 48 00 00 00 74 0E 48 8D 05 D9 D0 B5 00 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DCDB0ull))(argc, argv);
}

// Lisp: (player-train-wreck-dyno-jump-hack ...) | StringId: 0x2A0B2311 | Guest Offset: 0x0090E050
int64_t DcPlayerTrainWreckDynoJumpHack(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D A0 CB 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E050ull))(argc, argv);
}

// Lisp: (player-turn-to ...) | StringId: 0x0174DD00 | Guest Offset: 0x00A0B2D0
int64_t DcPlayerTurnTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 85 FF 7E 74 48 8B 06 48 85 C0 74 6C 48 8D 35 3D 0A 75 00 48 8B 16 31 C9
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0B2D0ull))(argc, argv);
}

// Lisp: (player-under-train? ...) | StringId: 0xAD70C8DF | Guest Offset: 0x0090A660
int64_t DcPlayerUnderTrainP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 8D 0D B3 16 85 00 48 8B 01 48 85 C0 0F 84 3A 02 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090A660ull))(argc, argv);
}

// Lisp: (player-unparent ...) | StringId: 0x1DC1D601 | Guest Offset: 0x0090F5F0
int64_t DcPlayerUnparent(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 0D 25 C7 84 00 48 8B 01 48 85 C0 74 1F 8B 49 08 85 C9
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090F5F0ull))(argc, argv);
}

// Lisp: (player-use-large-npc-edge-collision ...) | StringId: 0x6C6EAB6E | Guest Offset: 0x0090E400
int64_t DcPlayerUseLargeNpcEdgeCollision(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D F0 C7 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E400ull))(argc, argv);
}

// Lisp: (pow-float ...) | StringId: 0x614A6E07 | Guest Offset: 0x009FDB60
int64_t DcPowFloat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 C5 FA 10 06 C5 FA 10 4E 08 E8 0A 8E 3E 00 48 C7
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDB60ull))(argc, argv);
}

// Lisp: (predict-on-physics-driven ...) | StringId: 0x9D051196 | Guest Offset: 0x00A037C0
int64_t DcPredictOnPhysicsDriven(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 25 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A037C0ull))(argc, argv);
}

// Lisp: (print-event ...) | StringId: 0xFE2D2DC2 | Guest Offset: 0x004C1800
int64_t DcPrintEvent(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 4C 8D 35 12 EB B8 00 49 63 9E 00 02 00 00 48 83 FB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C1800ull))(argc, argv);
}

// Lisp: (print-float ...) | StringId: 0x272D183B | Guest Offset: 0x009FDF00
int64_t DcPrintFloat(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (print-integer ...) | StringId: 0x6E61504E | Guest Offset: 0x009FDEF0
int64_t DcPrintInteger(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (print-point ...) | StringId: 0xCF323EFA | Guest Offset: 0x00A10290
int64_t DcPrintPoint(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (print-string ...) | StringId: 0x8024F1BF | Guest Offset: 0x009FDEE0
int64_t DcPrintString(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (print-string-ws ...) | StringId: 0x234EDACF | Guest Offset: 0x009FDEA0
int64_t DcPrintStringWs(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 83 FF 02 75 17 48 83 7E 08 00 75 20 48 8D 35 F2 FC 49 00 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDEA0ull))(argc, argv);
}

// Lisp: (print-symbol ...) | StringId: 0x4FB6C279 | Guest Offset: 0x00A10280
int64_t DcPrintSymbol(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (print-vector ...) | StringId: 0x8A32D4A6 | Guest Offset: 0x009FDF10
int64_t DcPrintVector(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (print-vector-3 ...) | StringId: 0x45878D73 | Guest Offset: 0x009FDF20
int64_t DcPrintVector3(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (printf ...) | StringId: 0xC107125D | Guest Offset: 0x00A10110
int64_t DcPrintf(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 81 EC 28 02 00 00 48 8B 1D DD AA 63 00 48 8B 03 48 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A10110ull))(argc, argv);
}

// Lisp: (psn-demo-show-upsell ...) | StringId: 0x421618FC | Guest Offset: 0x009FD590
int64_t DcPsnDemoShowUpsell(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 A9 90 73 00 C6 80 5D 10 00 00 01 31 C0 C3 66 66 66 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD590ull))(argc, argv);
}

// Lisp: (rail-activate ...) | StringId: 0xD0EE3B5B | Guest Offset: 0x00AB98D0
int64_t DcRailActivate(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 14 48 8B 36 48 85 F6 74 15 E8 69 64 FD FF BB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB98D0ull))(argc, argv);
}

// Lisp: (rail-activate-mover ...) | StringId: 0xA57010DA | Guest Offset: 0x00AB8340
int64_t DcRailActivateMover(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 19 44 8B 3E 83 FF 01 7E 2D 8B 5E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8340ull))(argc, argv);
}

// Lisp: (rail-animate-to-mover ...) | StringId: 0x020771EF | Guest Offset: 0x00AB8130
int64_t DcRailAnimateToMover(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 20 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8130ull))(argc, argv);
}

// Lisp: (rail-deactivate ...) | StringId: 0x8DAF015E | Guest Offset: 0x00AB97D0
int64_t DcRailDeactivate(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 20 49 89 F6 4C 8B 25 17 14 59
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB97D0ull))(argc, argv);
}

// Lisp: (rail-deactivate-mover ...) | StringId: 0x7C0C3BCE | Guest Offset: 0x00AB8290
int64_t DcRailDeactivateMover(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 31 DB 85 FF 7E 3C 8B 1E 83 FB 02 72 35 48 8B 05 CE 5C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8290ull))(argc, argv);
}

// Lisp: (rail-disable-careen ...) | StringId: 0xC52F2C68 | Guest Offset: 0x00AB8B80
int64_t DcRailDisableCareen(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 89 FB 85 DB 7E 7D 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8B80ull))(argc, argv);
}

// Lisp: (rail-enable-collide-with-objects ...) | StringId: 0x17054BCB | Guest Offset: 0x00AB8EA0
int64_t DcRailEnableCollideWithObjects(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 89 FB 85 DB 7E 7D 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8EA0ull))(argc, argv);
}

// Lisp: (rail-enable-ground-hug ...) | StringId: 0x7D2FA4C6 | Guest Offset: 0x00AB8430
int64_t DcRailEnableGroundHug(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 89 FB 85 DB 7E 23 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8430ull))(argc, argv);
}

// Lisp: (rail-enable-npc-shooting ...) | StringId: 0x392231A6 | Guest Offset: 0x00AB8E40
int64_t DcRailEnableNpcShooting(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 05 6D B6 6A 00 C6 00 00 85 FF 7E 1A 83 3E 00 0F 94 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8E40ull))(argc, argv);
}

// Lisp: (rail-enable-tributary ...) | StringId: 0x1B9BC27A | Guest Offset: 0x00AB96B0
int64_t DcRailEnableTributary(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F6 41 89 FF 45 85 FF 7E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB96B0ull))(argc, argv);
}

// Lisp: (rail-enable-truck-jump ...) | StringId: 0x31873FF8 | Guest Offset: 0x00AB9BB0
int64_t DcRailEnableTruckJump(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 40 10 59 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB9BB0ull))(argc, argv);
}

// Lisp: (rail-fire-turret-grenade ...) | StringId: 0x15D2B122 | Guest Offset: 0x00AB8CD0
int64_t DcRailFireTurretGrenade(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F5 89 FB 85 DB 7E 3D 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8CD0ull))(argc, argv);
}

// Lisp: (rail-follow-mover ...) | StringId: 0x832952B6 | Guest Offset: 0x00AB81E0
int64_t DcRailFollowMover(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 20 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB81E0ull))(argc, argv);
}

// Lisp: (rail-force-careen ...) | StringId: 0xEA5B4822 | Guest Offset: 0x00AB8C40
int64_t DcRailForceCareen(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 4F 8B 1E 31 F6 89 DF E8 47 A7 22
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8C40ull))(argc, argv);
}

// Lisp: (rail-get-mover-locator ...) | StringId: 0x7D0DD7F9 | Guest Offset: 0x00AB7D00
int64_t DcRailGetMoverLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 70 49 89 F6 41 89 FC 4C 8B 3D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB7D00ull))(argc, argv);
}

// Lisp: (rail-get-spline ...) | StringId: 0x9847F5E9 | Guest Offset: 0x00AB84E0
int64_t DcRailGetSpline(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 AA AE 22 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB84E0ull))(argc, argv);
}

// Lisp: (rail-has-vehicle ...) | StringId: 0xD2FFDB06 | Guest Offset: 0x00AB94B0
int64_t DcRailHasVehicle(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 5D 8B 1E 31 F6 89 DF E8 D7 9E 22
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB94B0ull))(argc, argv);
}

// Lisp: (rail-kill-all-physics-objects ...) | StringId: 0x2066EFE9 | Guest Offset: 0x00AB8F60
int64_t DcRailKillAllPhysicsObjects(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 3D 45 B0 6A 00 48 8D 15 E1 01 3F 00 BE 01 00 00 00 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8F60ull))(argc, argv);
}

// Lisp: (rail-mode ...) | StringId: 0xEE63C2B2 | Guest Offset: 0x00AB8610
int64_t DcRailMode(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 11 8B 1E 31 F6 89 DF E8 7B AD 22 00 89 1D 8D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8610ull))(argc, argv);
}

// Lisp: (rail-override-sfx-state ...) | StringId: 0xF8026162 | Guest Offset: 0x00AB9070
int64_t DcRailOverrideSfxState(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 20 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB9070ull))(argc, argv);
}

// Lisp: (rail-override-speed-for-audio ...) | StringId: 0xD3B6CABE | Guest Offset: 0x00AB9120
int64_t DcRailOverrideSpeedForAudio(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 83 FF 02 7C 5B 41 8B 1F 31 F6 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB9120ull))(argc, argv);
}

// Lisp: (rail-scrub ...) | StringId: 0xC300E067 | Guest Offset: 0x00AB8F90
int64_t DcRailScrub(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 83 FF 02 0F 8C 86 00 00 00 41 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8F90ull))(argc, argv);
}

// Lisp: (rail-set-acceleration ...) | StringId: 0x6C65EA31 | Guest Offset: 0x00AB9590
int64_t DcRailSetAcceleration(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F7 89 FB 85 DB 7E 1D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB9590ull))(argc, argv);
}

// Lisp: (rail-set-all-vehicles-to-mover-speed ...) | StringId: 0x2EA5A660 | Guest Offset: 0x00AB7BF0
int64_t DcRailSetAllVehiclesToMoverSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 31 DB 85 FF 7E 3C 8B 1E 83 FB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB7BF0ull))(argc, argv);
}

// Lisp: (rail-set-follow-distance ...) | StringId: 0x334660E2 | Guest Offset: 0x00AB8560
int64_t DcRailSetFollowDistance(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 48 89 F3 83 FF 03 7C 4A 44 8B 3B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8560ull))(argc, argv);
}

// Lisp: (rail-set-igc-fade-out-time ...) | StringId: 0x827972A4 | Guest Offset: 0x00AB93F0
int64_t DcRailSetIgcFadeOutTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: C5 F8 57 C0 85 FF 7E 04 C5 FA 10 06 C5 FA 11 05 B0 68 A8 00 31 C0 C3 66
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB93F0ull))(argc, argv);
}

// Lisp: (rail-set-mover-follow-distance ...) | StringId: 0xD8A42677 | Guest Offset: 0x00AB7FD0
int64_t DcRailSetMoverFollowDistance(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 85 FF 7E 0B C5 FA 10 06 B8 01 00 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB7FD0ull))(argc, argv);
}

// Lisp: (rail-set-mover-intial-distance ...) | StringId: 0xF03D5B80 | Guest Offset: 0x00AB7E80
int64_t DcRailSetMoverIntialDistance(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 49 89 F7 89 FB 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB7E80ull))(argc, argv);
}

// Lisp: (rail-set-mover-speed ...) | StringId: 0xBE8978A4 | Guest Offset: 0x00AB80A0
int64_t DcRailSetMoverSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 31 DB 85 FF 7E 52 C5 FA 10 06 83 FF 02 7C 51 8B 5E 08
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB80A0ull))(argc, argv);
}

// Lisp: (rail-set-npc-archetype ...) | StringId: 0xA95B4A9B | Guest Offset: 0x00AB9660
int64_t DcRailSetNpcArchetype(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 1D 8B 1E 31 F6 89 DF E8 2B 9D 22 00 48 85 DB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB9660ull))(argc, argv);
}

// Lisp: (rail-set-speed ...) | StringId: 0x877634B0 | Guest Offset: 0x00AB9230
int64_t DcRailSetSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 83 FB 02 0F 8C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB9230ull))(argc, argv);
}

// Lisp: (rail-shutdown ...) | StringId: 0xC0E7184F | Guest Offset: 0x00AB9340
int64_t DcRailShutdown(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 8D 3D 63 AC 6A 00 48 8D 15 37 FD 3E 00 BE 01 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB9340ull))(argc, argv);
}

// Lisp: (rail-spawn ...) | StringId: 0xDE7E4738 | Guest Offset: 0x00AB9920
int64_t DcRailSpawn(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 D7 85 FF 7E 45 48 8B 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB9920ull))(argc, argv);
}

// Lisp: (rail-use-fixed-apref-in-igc ...) | StringId: 0x4A0864D1 | Guest Offset: 0x00AB9410
int64_t DcRailUseFixedAprefInIgc(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 89 FB 85 DB 7E 23 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB9410ull))(argc, argv);
}

// Lisp: (random-float ...) | StringId: 0x00DD2C2E | Guest Offset: 0x009FDA20
int64_t DcRandomFloat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 8B 05 0A 55 B4 00 8B 0D 00 55 B4 00 48 69 C9 C7
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDA20ull))(argc, argv);
}

// Lisp: (random-integer ...) | StringId: 0xC4A0E51B | Guest Offset: 0x009FD930
int64_t DcRandomInteger(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 0F 8E C3 00 00 00 8B 06 C5 F0 57 C9 83 FF 02 7C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD930ull))(argc, argv);
}

// Lisp: (regenerate-cover-action-packs-from-object ...) | StringId: 0xFBEBBCDA | Guest Offset: 0x00A04E20
int64_t DcRegenerateCoverActionPacksFromObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 44 8B 1E 31 F6 89 DF E8 6A E5 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04E20ull))(argc, argv);
}

// Lisp: (reload-module ...) | StringId: 0x0E530D18 | Guest Offset: 0x00A19200
int64_t DcReloadModule(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 4C 8B 35 EC 19 63 00 49 8B 06 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A19200ull))(argc, argv);
}

// Lisp: (reload-particle-module ...) | StringId: 0x56FB3996 | Guest Offset: 0x00A19180
int64_t DcReloadParticleModule(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 4C 8B 35 6E 1A 63 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A19180ull))(argc, argv);
}

// Lisp: (remove-contextual-entity ...) | StringId: 0xCE547C11 | Guest Offset: 0x00A3F960
int64_t DcRemoveContextualEntity(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 4C 8D 35 B2 09 61 00 49 63 9E 00 02 00 00 48 83 FB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A3F960ull))(argc, argv);
}

// Lisp: (remove-cover-action-packs-from-object ...) | StringId: 0x4BBC253B | Guest Offset: 0x00A04DC0
int64_t DcRemoveCoverActionPacksFromObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 44 8B 1E 31 F6 89 DF E8 CA E5 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04DC0ull))(argc, argv);
}

// Lisp: (remove-dynamic-light ...) | StringId: 0x28DC0AE4 | Guest Offset: 0x009FEDF0
int64_t DcRemoveDynamicLight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 12 8B 1E 31 F6 89 DF E8 9B 45 2E 00 89 DF E8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FEDF0ull))(argc, argv);
}

// Lisp: (remove-player-weapon ...) | StringId: 0x379BE9FD | Guest Offset: 0x00906E10
int64_t DcRemovePlayerWeapon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 05 05 4F 85 00 48 8B 10 45 31 C0 41 B9 00 00 00 00 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00906E10ull))(argc, argv);
}

// Lisp: (rename-object ...) | StringId: 0x3F50EB13 | Guest Offset: 0x00A04D10
int64_t DcRenameObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 48 89 F3 83 FF 02 7C 5C 44 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04D10ull))(argc, argv);
}

// Lisp: (render-settings-apply-defaults ...) | StringId: 0x9BBE8C16 | Guest Offset: 0x00A10760
int64_t DcRenderSettingsApplyDefaults(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 8B 3E 89 3D 80 F3 63 00 48 8D 05 CD 8E 64 00 BE 2D 05 DE 30
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A10760ull))(argc, argv);
}

// Lisp: (reset-actors ...) | StringId: 0xD55E5522 | Guest Offset: 0x009FD690
int64_t DcResetActors(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 E8 37 BB F6 FF E8 02 76 9F FF 31 C0 5D C3 66 66 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD690ull))(argc, argv);
}

// Lisp: (reset-aim-fov ...) | StringId: 0x6EC37668 | Guest Offset: 0x0090D780
int64_t DcResetAimFov(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 89 20 74 00 C7 80 D0 00 00 00 00 00 80 BF 31 C0 C3 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D780ull))(argc, argv);
}

// Lisp: (reset-harvesting-priority ...) | StringId: 0x2C101629 | Guest Offset: 0x00A01AE0
int64_t DcResetHarvestingPriority(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv;
    *(uint8_t *)(image + 0x0110DCB0) = 0x0Au;
    return 0;
}

// Lisp: (reset-stat ...) | StringId: 0xD6AEAC89 | Guest Offset: 0x0097BE50
int64_t DcResetStat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 BA 00 00 00 00 01 00 00 00 31 C0 83
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0097BE50ull))(argc, argv);
}

// Lisp: (restore-blur ...) | StringId: 0xDCFA292B | Guest Offset: 0x009FFF20
int64_t DcRestoreBlur(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 0D F5 BD 75 00 48 8B 01 48 85 C0 74 19 8B 49 08 85 C9
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FFF20ull))(argc, argv);
}

// Lisp: (reverse-locator ...) | StringId: 0x2711E149 | Guest Offset: 0x00A05CB0
int64_t DcReverseLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 28 4C 8B 35 3C 4F 64 00 49 8B 06 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A05CB0ull))(argc, argv);
}

// Lisp: (rope-disable-character-collision ...) | StringId: 0xA49F8426 | Guest Offset: 0x00A04340
int64_t DcRopeDisableCharacterCollision(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F7 89 FB 85 DB 7E 1D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04340ull))(argc, argv);
}

// Lisp: (rope-enable-character-collision ...) | StringId: 0xA4A7B2E6 | Guest Offset: 0x00A04270
int64_t DcRopeEnableCharacterCollision(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 7E 10 8B 1E 31 F6 89 DF E8 17 F1 2D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04270ull))(argc, argv);
}

// Lisp: (rotate-locator-angle ...) | StringId: 0x8801D9D3 | Guest Offset: 0x00A05090
int64_t DcRotateLocatorAngle(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 50 4C 8B 35 5A 5B 64 00 49 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A05090ull))(argc, argv);
}

// Lisp: (rotate-locator-lookat ...) | StringId: 0xE36D7F8E | Guest Offset: 0x00A05620
int64_t DcRotateLocatorLookat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC B8 00 00 00 4C 8B 35 C9 55 64 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A05620ull))(argc, argv);
}

// Lisp: (say ...) | StringId: 0x235CFA87 | Guest Offset: 0x00A01A80
int64_t DcSay(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: BA FF FF FF FF E9 96 17 FF FF 66 0F 1F 44 00 00 55 48 89 E5 48 8D 3D 05
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01A80ull))(argc, argv);
}

// Lisp: (say-pre-roll ...) | StringId: 0x159DCDD2 | Guest Offset: 0x00A019F0
int64_t DcSayPreRoll(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 89 F0 83 FF 02 75 5F 8B 30 BB 01 00 00 00 85 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A019F0ull))(argc, argv);
}

// Lisp: (scan-particle-directories ...) | StringId: 0x058DE911 | Guest Offset: 0x00A19360
int64_t DcScanParticleDirectories(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 E4 E0 FF FF FF 48 81 EC 20
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A19360ull))(argc, argv);
}

// Lisp: (select-best-schema-spawner ...) | StringId: 0x4895DC59 | Guest Offset: 0x00A10EB0
int64_t DcSelectBestSchemaSpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 18 02 00 00 49 89 F6 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A10EB0ull))(argc, argv);
}

// Lisp: (send-event ...) | StringId: 0xB3189857 | Guest Offset: 0x004C18F0
int64_t DcSendEvent(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F6 89 FB 83 FB 02 7C 4D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C18F0ull))(argc, argv);
}

// Lisp: (send-simple-event ...) | StringId: 0x54CB0A8D | Guest Offset: 0x004C1CF0
int64_t DcSendSimpleEvent(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 58 48 89 F3 4C 8B 3D F9 8E B8 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x004C1CF0ull))(argc, argv);
}

// Lisp: (set-animation-speed ...) | StringId: 0xAF4A581F | Guest Offset: 0x00397880
int64_t DcSetAnimationSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 49 89 F4 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00397880ull))(argc, argv);
}

// Lisp: (set-barrel-flow-multiplier ...) | StringId: 0x3FF04910 | Guest Offset: 0x00A01EF0
int64_t DcSetBarrelFlowMultiplier(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (set-black-fade ...) | StringId: 0x57505A6E | Guest Offset: 0x00A10700
int64_t DcSetBlackFade(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D F0 A4 63 00 48 8B 03 48 89 45 F0 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A10700ull))(argc, argv);
}

// Lisp: (set-boolean ...) | StringId: 0xBA711E0D | Guest Offset: 0x00A42060
int64_t DcSetBoolean(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F5 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A42060ull))(argc, argv);
}

// Lisp: (set-character-wet-mask ...) | StringId: 0x664CFB37 | Guest Offset: 0x00A02550
int64_t DcSetCharacterWetMask(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 85 FF 0F 8E C8 00 00 00 8B 3E 4C 8D 05 05
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02550ull))(argc, argv);
}

// Lisp: (set-character-wet-mask-state ...) | StringId: 0xBD0C58B9 | Guest Offset: 0x00A02690
int64_t DcSetCharacterWetMaskState(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 28 8B 06 BB 01 00 00 00 48 83 F8 05 77 2B 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02690ull))(argc, argv);
}

// Lisp: (set-character-wet-mask-value ...) | StringId: 0x591FFCD9 | Guest Offset: 0x00A026E0
int64_t DcSetCharacterWetMaskValue(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 75 8B 0E BB 01 00 00 00 48 83 F9 05 77 78 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A026E0ull))(argc, argv);
}

// Lisp: (set-clock-multiplier ...) | StringId: 0x2607C5F4 | Guest Offset: 0x00A0EE60
int64_t DcSetClockMultiplier(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 4C 8D 3D 74 30 75 00 49 8B 07 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0EE60ull))(argc, argv);
}

// Lisp: (set-collision-layer ...) | StringId: 0x1B6BB746 | Guest Offset: 0x00A03CD0
int64_t DcSetCollisionLayer(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 89 FB 85 DB 7E 27 45 8B 27 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A03CD0ull))(argc, argv);
}

// Lisp: (set-cover-push-back-dist ...) | StringId: 0x47A854A4 | Guest Offset: 0x0090B060
int64_t DcSetCoverPushBackDist(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 20 48 89 F3 4C 8B 35 8B FB 73 00 49 8B 06
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090B060ull))(argc, argv);
}

// Lisp: (set-current-by-spawner ...) | StringId: 0xB78DDD1A | Guest Offset: 0x009FD710
int64_t DcSetCurrentBySpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 28 48 8B 1D E0 D4 64 00 48 8B 03 48 89 45 F0 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD710ull))(argc, argv);
}

// Lisp: (set-default-follow-camera-settings ...) | StringId: 0x38F67D8D | Guest Offset: 0x00A0EBF0
int64_t DcSetDefaultFollowCameraSettings(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 48 C7 04 24 00 00 00 00 48 8D 35 06 0C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0EBF0ull))(argc, argv);
}

// Lisp: (set-deformation-of-water-unit ...) | StringId: 0xB010691C | Guest Offset: 0x00A02CC0
int64_t DcSetDeformationOfWaterUnit(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 05 0F 8C 99 00 00 00 48 8D 0D AA 1C 76 00 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02CC0ull))(argc, argv);
}

// Lisp: (set-dynamic-light-color ...) | StringId: 0xA1D959C1 | Guest Offset: 0x009FEE20
int64_t DcSetDynamicLightColor(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 41 89 FF 31 C0 41 83 FF 04 0F 8C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FEE20ull))(argc, argv);
}

// Lisp: (set-e-3-loading-screen ...) | StringId: 0xEB9B8D2F | Guest Offset: 0x009FD5B0
int64_t DcSetE3LoadingScreen(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (set-float ...) | StringId: 0xC1BC5183 | Guest Offset: 0x00A42630
int64_t DcSetFloat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F4 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A42630ull))(argc, argv);
}

// Lisp: (set-focusable-grappled ...) | StringId: 0xBF697B95 | Guest Offset: 0x00A0F2C0
int64_t DcSetFocusableGrappled(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 41 89 FF 45 85 FF 7E 0E 41 8B 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0F2C0ull))(argc, argv);
}

// Lisp: (set-focusable-ignore ...) | StringId: 0x472480C9 | Guest Offset: 0x00A0F3C0
int64_t DcSetFocusableIgnore(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 89 FB 85 DB 7E 20 45 8B 37 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0F3C0ull))(argc, argv);
}

// Lisp: (set-force-tension ...) | StringId: 0x58070796 | Guest Offset: 0x00667EE0
int64_t DcSetForceTension(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 83 3D 91 6A E6 00 01 7E 13 83 3E 00 48 8D 05 69 38 AD 00 0F
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00667EE0ull))(argc, argv);
}

// Lisp: (set-harvesting-enabled ...) | StringId: 0x6C742763 | Guest Offset: 0x00A01B00
int64_t DcSetHarvestingEnabled(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 85 FF 7E 0D 83 3E 00 48 8D 05 92 C1 70 00 0F 95 00 31 C0 C3 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01B00ull))(argc, argv);
}

// Lisp: (set-harvesting-priority ...) | StringId: 0x39ACE3D6 | Guest Offset: 0x00A01AC0
int64_t DcSetHarvestingPriority(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 85 FF 7E 18 0F B6 06 B1 FD 3D FD 00 00 00 77 02 88 C1 48 8D 05 C7 C1 70
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01AC0ull))(argc, argv);
}

// Lisp: (set-health-parameters ...) | StringId: 0x572D826C | Guest Offset: 0x002DAAA0
int64_t DcSetHealthParameters(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 38 49 89 F6 41 89 FC 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DAAA0ull))(argc, argv);
}

// Lisp: (set-ice-cave-stupid-gears-hack ...) | StringId: 0xF1A86EF0 | Guest Offset: 0x009FCC50
int64_t DcSetIceCaveStupidGearsHack(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D A0 DF 64 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FCC50ull))(argc, argv);
}

// Lisp: (set-int-32 ...) | StringId: 0x55A43C77 | Guest Offset: 0x00A42350
int64_t DcSetInt32(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F5 41 89 FE 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A42350ull))(argc, argv);
}

// Lisp: (set-jetski-flow-multiplier ...) | StringId: 0x4EA4FF7B | Guest Offset: 0x00A01B70
int64_t DcSetJetskiFlowMultiplier(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (set-jetski-max-throttle ...) | StringId: 0x7D981505 | Guest Offset: 0x00A01B60
int64_t DcSetJetskiMaxThrottle(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (set-jetski-near-stupid-ramp-in-ruins-2 ...) | StringId: 0x6877F1BB | Guest Offset: 0x00A01B50
int64_t DcSetJetskiNearStupidRampInRuins2(int32_t argc, uint64_t *argv, uint8_t *image) {
    (void)argc; (void)argv; (void)image;
    return 0; // No-op in Shipping build
}

// Lisp: (set-light-fov ...) | StringId: 0xBDA58A56 | Guest Offset: 0x009FFC80
int64_t DcSetLightFov(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 83 FF 02 0F 8C B4 00 00 00 45 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FFC80ull))(argc, argv);
}

// Lisp: (set-lookat-stop-phase ...) | StringId: 0xE63BAFC6 | Guest Offset: 0x00397320
int64_t DcSetLookatStopPhase(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F6 41 89 FC 41 83 FC 02 7C 33 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00397320ull))(argc, argv);
}

// Lisp: (set-max-character-wetness ...) | StringId: 0xCBAA0C97 | Guest Offset: 0x00A02510
int64_t DcSetMaxCharacterWetness(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 1A C5 FA 10 06 48 8D 05 4B 50 71 00 C5 FA 11
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02510ull))(argc, argv);
}

// Lisp: (set-mesh-visible ...) | StringId: 0x83A68745 | Guest Offset: 0x00A0BA20
int64_t DcSetMeshVisible(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 48 89 F3 83 FF 03 0F 85 32 01
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0BA20ull))(argc, argv);
}

// Lisp: (set-min-fall-dist ...) | StringId: 0xDEA27032 | Guest Offset: 0x0090E660
int64_t DcSetMinFallDist(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 48 C7 04 24 00 00 00 00 BE 20 01 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E660ull))(argc, argv);
}

// Lisp: (set-mixer-override ...) | StringId: 0xB4EE8F1D | Guest Offset: 0x009FD3B0
int64_t DcSetMixerOverride(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 1E 8B 1E 31 F6 89 DF E8 DB 5F 2E 00 48 8D 05
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD3B0ull))(argc, argv);
}

// Lisp: (set-move-mode ...) | StringId: 0x7F4EEFA5 | Guest Offset: 0x0090D4F0
int64_t DcSetMoveMode(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 49 89 F7 4C 8B 35 F9 D6 73 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D4F0ull))(argc, argv);
}

// Lisp: (set-object-faction ...) | StringId: 0xDDE27074 | Guest Offset: 0x009FCF40
int64_t DcSetObjectFaction(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 49 89 F6 83 FF 01 7F 12 48 8D 35 39 0A 4A 00 31 C0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FCF40ull))(argc, argv);
}

// Lisp: (set-object-health ...) | StringId: 0xCF2E79AE | Guest Offset: 0x00A04A20
int64_t DcSetObjectHealth(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 49 89 F7 85 FF 7E 11 41 8B 1F 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04A20ull))(argc, argv);
}

// Lisp: (set-off-balance-by-spawner ...) | StringId: 0x8694E92A | Guest Offset: 0x008E9160
int64_t DcSetOffBalanceBySpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 90 1A 76 00 48 8B 03 48 89 45 F0 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x008E9160ull))(argc, argv);
}

// Lisp: (set-player-can-only-draw-pistol ...) | StringId: 0xE6230053 | Guest Offset: 0x0090B9E0
int64_t DcSetPlayerCanOnlyDrawPistol(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 10 F2 73 00 48 8B 03 48 89 45 F0 83
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090B9E0ull))(argc, argv);
}

// Lisp: (set-player-climb-spawner ...) | StringId: 0x04E41EBC | Guest Offset: 0x0090E540
int64_t DcSetPlayerClimbSpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 48 C7 04 24 00 00 00 00 BE 88 01 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E540ull))(argc, argv);
}

// Lisp: (set-player-health ...) | StringId: 0x892E5D97 | Guest Offset: 0x009100F0
int64_t DcSetPlayerHealth(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 0D 25 BC 84 00 48 8B 01 48 85 C0 74 25 8B 49 08 85 C9
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009100F0ull))(argc, argv);
}

// Lisp: (set-player-in-combat ...) | StringId: 0x498067E7 | Guest Offset: 0x0090DB40
int64_t DcSetPlayerInCombat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 81 EC 98 00 00 00 48 8B 1D AD D0 73 00 48 8B 03 48 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090DB40ull))(argc, argv);
}

// Lisp: (set-player-infinite-ammo ...) | StringId: 0xAB1B6459 | Guest Offset: 0x0090BA80
int64_t DcSetPlayerInfiniteAmmo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 70 F1 73 00 48 8B 03 48 89 45 F0 83
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090BA80ull))(argc, argv);
}

// Lisp: (set-player-invincible ...) | StringId: 0x024B9BE2 | Guest Offset: 0x0090BB10
int64_t DcSetPlayerInvincible(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D E0 F0 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090BB10ull))(argc, argv);
}

// Lisp: (set-player-jump-spawner ...) | StringId: 0xEC27D02A | Guest Offset: 0x0090EF70
int64_t DcSetPlayerJumpSpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 48 C7 04 24 00 00 00 00 BE 84 01 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090EF70ull))(argc, argv);
}

// Lisp: (set-player-max-fall-height ...) | StringId: 0x80BAFE8C | Guest Offset: 0x00906EE0
int64_t DcSetPlayerMaxFallHeight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 10 3D 74 00 48 8B 03 48 89 45 F0 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00906EE0ull))(argc, argv);
}

// Lisp: (set-player-no-igc-kill-grenade ...) | StringId: 0xF1B16E3A | Guest Offset: 0x0090E910
int64_t DcSetPlayerNoIgcKillGrenade(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D E0 C2 73 00 48 8B 03 48 89 45 F0 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090E910ull))(argc, argv);
}

// Lisp: (set-player-on-collapsing-building ...) | StringId: 0xCE9461C8 | Guest Offset: 0x0090F0F0
int64_t DcSetPlayerOnCollapsingBuilding(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 00 BB 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090F0F0ull))(argc, argv);
}

// Lisp: (set-ragdoll-flow ...) | StringId: 0x8B588B25 | Guest Offset: 0x009FD5C0
int64_t DcSetRagdollFlow(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 48 89 F3 85 FF 0F 8E B5 00 00 00 48 C7 04 24 00 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD5C0ull))(argc, argv);
}

// Lisp: (set-rain-ripples-state ...) | StringId: 0x85D0299E | Guest Offset: 0x00A027F0
int64_t DcSetRainRipplesState(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 15 83 3E 00 48 8D 05 7C 21 76 00 0F 95 40 5F
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A027F0ull))(argc, argv);
}

// Lisp: (set-rain-state ...) | StringId: 0xB71A0088 | Guest Offset: 0x00A02830
int64_t DcSetRainState(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 4C 8B 35 BA 83 64 00 49 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02830ull))(argc, argv);
}

// Lisp: (set-reload-pistol-if-empty ...) | StringId: 0x1F4159DD | Guest Offset: 0x0090B940
int64_t DcSetReloadPistolIfEmpty(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D B0 F2 73 00 48 8B 03 48 89 45 F0 83
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090B940ull))(argc, argv);
}

// Lisp: (set-render-settings ...) | StringId: 0xFAB05D37 | Guest Offset: 0x009FFF80
int64_t DcSetRenderSettings(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F4 48 8D 0D 8B 5A 71 00 B8 01 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FFF80ull))(argc, argv);
}

// Lisp: (set-reverse-jump-enable ...) | StringId: 0x835B6FE2 | Guest Offset: 0x0090D140
int64_t DcSetReverseJumpEnable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D B0 DA 73 00 48 8B 03 48 89 45 F0 83
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D140ull))(argc, argv);
}

// Lisp: (set-run-type ...) | StringId: 0xEDE11587 | Guest Offset: 0x0090D1F0
int64_t DcSetRunType(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 49 89 F7 4C 8B 35 F9 D9 73 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D1F0ull))(argc, argv);
}

// Lisp: (set-shader-instance-param ...) | StringId: 0x34301F63 | Guest Offset: 0x00A107B0
int64_t DcSetShaderInstanceParam(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F6 85 FF 7E 11
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A107B0ull))(argc, argv);
}

// Lisp: (set-shader-material-param ...) | StringId: 0x5C817DC0 | Guest Offset: 0x00A10DD0
int64_t DcSetShaderMaterialParam(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 E4 E0 FF FF FF 48 81 EC A0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A10DD0ull))(argc, argv);
}

// Lisp: (set-single-shader-instance-param ...) | StringId: 0x48059F73 | Guest Offset: 0x00A10A50
int64_t DcSetSingleShaderInstanceParam(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 41 89 FD 45 85 ED 7E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A10A50ull))(argc, argv);
}

// Lisp: (set-spline-follow-acceleration ...) | StringId: 0x7D6C9090 | Guest Offset: 0x00A25CB0
int64_t DcSetSplineFollowAcceleration(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 89 FB 85 DB 7E 1A 45 8B 3E 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A25CB0ull))(argc, argv);
}

// Lisp: (set-spline-follow-distance ...) | StringId: 0x490DC812 | Guest Offset: 0x00A25C00
int64_t DcSetSplineFollowDistance(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 89 FB 85 DB 7E 1A 45 8B 3E 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A25C00ull))(argc, argv);
}

// Lisp: (set-spline-follow-speed ...) | StringId: 0x26344B84 | Guest Offset: 0x00A25D60
int64_t DcSetSplineFollowSpeed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F7 89 FB 85 DB 7E 30
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A25D60ull))(argc, argv);
}

// Lisp: (set-swim-drag-multiplier ...) | StringId: 0xB1DA0D91 | Guest Offset: 0x0090D650
int64_t DcSetSwimDragMultiplier(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 83 FF 01 75 0E C5 FA 10 06 C5 FA 11 05 23 07 C1 00 EB 10 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D650ull))(argc, argv);
}

// Lisp: (set-symbol ...) | StringId: 0x01D011B2 | Guest Offset: 0x00A42910
int64_t DcSetSymbol(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F5 41 89 FE 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A42910ull))(argc, argv);
}

// Lisp: (set-table-shader-material-values-param ...) | StringId: 0xDF146CA7 | Guest Offset: 0x009F3B70
int64_t DcSetTableShaderMaterialValuesParam(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 4C 8B 25 78 70 65 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009F3B70ull))(argc, argv);
}

// Lisp: (set-target-spawner-angle ...) | StringId: 0x174C4F7E | Guest Offset: 0x0090FE30
int64_t DcSetTargetSpawnerAngle(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 20 48 89 F3 4C 8B 35 BB AD 73 00 49 8B 06
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090FE30ull))(argc, argv);
}

// Lisp: (set-time-to-dry-period ...) | StringId: 0x6D2B84BA | Guest Offset: 0x00A02780
int64_t DcSetTimeToDryPeriod(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 3F C5 FA 10 06 48 8D 05 DB 4D 71 00 C5 FA 11
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02780ull))(argc, argv);
}

// Lisp: (set-visible ...) | StringId: 0x4E0D08CA | Guest Offset: 0x00A0BD30
int64_t DcSetVisible(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 89 F3 83 FF 02 0F 85 F6 00 00 00 44
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0BD30ull))(argc, argv);
}

// Lisp: (set-walk-mode ...) | StringId: 0x8855ACDB | Guest Offset: 0x0090D4B0
int64_t DcSetWalkMode(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8B 05 BD 0A C6 00 48 85 C0 74 27 48 8D 3D 6F 61 58 00 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D4B0ull))(argc, argv);
}

// Lisp: (set-walk-type ...) | StringId: 0x16EA1A1D | Guest Offset: 0x0090D350
int64_t DcSetWalkType(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 49 89 F7 4C 8B 35 99 D8 73 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D350ull))(argc, argv);
}

// Lisp: (set-water-offset ...) | StringId: 0xD89AAACD | Guest Offset: 0x00A02FF0
int64_t DcSetWaterOffset(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 28 4C 8B 35 FC 7B 64 00 49 8B 06 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02FF0ull))(argc, argv);
}

// Lisp: (set-water-state ...) | StringId: 0xF0A972D0 | Guest Offset: 0x00A02E20
int64_t DcSetWaterState(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 02 7C 44 48 8D 0D 4E 1B 76 00 48 8B 81 90 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02E20ull))(argc, argv);
}

// Lisp: (set-white-fade ...) | StringId: 0x6F684F90 | Guest Offset: 0x00A106A0
int64_t DcSetWhiteFade(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 50 A5 63 00 48 8B 03 48 89 45 F0 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A106A0ull))(argc, argv);
}

// Lisp: (show-binoculars ...) | StringId: 0x5AAF0C76 | Guest Offset: 0x00A0CB90
int64_t DcShowBinoculars(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 71 34 72 00 48 8B 48 60 81 09 00 08 00 00 8B 80 80 00 00 00 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0CB90ull))(argc, argv);
}

// Lisp: (shut-up ...) | StringId: 0x60683CCB | Guest Offset: 0x00A011E0
int64_t DcShutUp(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC 90 00 00 00 49 89 F7 4C 8B 25
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A011E0ull))(argc, argv);
}

// Lisp: (signal ...) | StringId: 0x64B0F52E | Guest Offset: 0x00A40480
int64_t DcSignal(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 68 49 89 F6 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A40480ull))(argc, argv);
}

// Lisp: (signal-raised? ...) | StringId: 0xBB99C26A | Guest Offset: 0x00A40220
int64_t DcSignalRaisedP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 F7 89 FB 4C 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A40220ull))(argc, argv);
}

// Lisp: (sla-force-tackle ...) | StringId: 0x6A3EC659 | Guest Offset: 0x00A0F4A0
int64_t DcSlaForceTackle(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 50 B7 63 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0F4A0ull))(argc, argv);
}

// Lisp: (slow-mo-effect ...) | StringId: 0x503B595E | Guest Offset: 0x00A0EE10
int64_t DcSlowMoEffect(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 89 F0 C5 FA 10 00 C5 FA 10 48 08 8B 70 18 8B 50 20 48 8D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0EE10ull))(argc, argv);
}

// Lisp: (spawn-attachable ...) | StringId: 0xA028680C | Guest Offset: 0x00A0F540
int64_t DcSpawnAttachable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC D8 00 00 00 49 89 F6 4C 8B 3D A6 B6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0F540ull))(argc, argv);
}

// Lisp: (spawn-entity ...) | StringId: 0xF43DB80A | Guest Offset: 0x00A09F40
int64_t DcSpawnEntity(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 20 49 89 D6 4C 8B 25 A7 0C 64
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A09F40ull))(argc, argv);
}

// Lisp: (spawn-entity-at ...) | StringId: 0x7417FCD6 | Guest Offset: 0x009F3900
int64_t DcSpawnEntityAt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 D6 49 89 F4 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009F3900ull))(argc, argv);
}

// Lisp: (spawn-explosion ...) | StringId: 0x6B4E7ABD | Guest Offset: 0x009FF610
int64_t DcSpawnExplosion(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 58 01 00 00 49 89 F7 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FF610ull))(argc, argv);
}

// Lisp: (spawn-explosion-npc-only ...) | StringId: 0x68F64E3C | Guest Offset: 0x009FF410
int64_t DcSpawnExplosionNpcOnly(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC B8 00 00 00 48 89 F3 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FF410ull))(argc, argv);
}

// Lisp: (spawn-floating-barrel-at-phase ...) | StringId: 0x6BF42AF8 | Guest Offset: 0x00A03E20
int64_t DcSpawnFloatingBarrelAtPhase(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 49 89 F5 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A03E20ull))(argc, argv);
}

// Lisp: (spawn-message ...) | StringId: 0xB69D2CEF | Guest Offset: 0x005DC7B0
int64_t DcSpawnMessage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 4C 8B 35 3E E4 A6 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DC7B0ull))(argc, argv);
}

// Lisp: (spawn-particles-at-joint ...) | StringId: 0xB507CCD3 | Guest Offset: 0x00A00950
int64_t DcSpawnParticlesAtJoint(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: BA 02 00 00 00 E9 26 25 FF FF 66 0F 1F 44 00 00 BA 01 00 00 00 E9 16 25
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A00950ull))(argc, argv);
}

// Lisp: (spawn-particles-at-locator ...) | StringId: 0x532A47BA | Guest Offset: 0x00A002D0
int64_t DcSpawnParticlesAtLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 E4 E0 FF FF FF 48 81 EC 20
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A002D0ull))(argc, argv);
}

// Lisp: (spawn-particles-attached ...) | StringId: 0xBB206306 | Guest Offset: 0x00A00960
int64_t DcSpawnParticlesAttached(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: BA 01 00 00 00 E9 16 25 FF FF 66 0F 1F 44 00 00 55 48 89 E5 53 50 85 FF
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A00960ull))(argc, argv);
}

// Lisp: (spawn-script ...) | StringId: 0x849A7D83 | Guest Offset: 0x00A44460
int64_t DcSpawnScript(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 68 4C 8B 25 88 67 60 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A44460ull))(argc, argv);
}

// Lisp: (spawn-snow-disturb ...) | StringId: 0x8BC6F669 | Guest Offset: 0x00A01BB0
int64_t DcSpawnSnowDisturb(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 49 89 F6 89 FB 4C 8B 3D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01BB0ull))(argc, argv);
}

// Lisp: (spline-activate ...) | StringId: 0xC61B9C4E | Guest Offset: 0x00A26180
int64_t DcSplineActivate(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 F7 85 FF 7E 12 45 8B 27 31 F6 44
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A26180ull))(argc, argv);
}

// Lisp: (spline-distance-locator ...) | StringId: 0x817AEAE5 | Guest Offset: 0x00A25F60
int64_t DcSplineDistanceLocator(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 28 49 89 F6 4C 8B 3D 89 4C 62 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A25F60ull))(argc, argv);
}

// Lisp: (spline-distance-spawner ...) | StringId: 0x4D284EEE | Guest Offset: 0x00A260E0
int64_t DcSplineDistanceSpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 18 48 89 F3 4C 8B 3D 09 4B 62 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A260E0ull))(argc, argv);
}

// Lisp: (spline-distance-vehicle-animation ...) | StringId: 0x281D8C8A | Guest Offset: 0x00AB8640
int64_t DcSplineDistanceVehicleAnimation(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC B8 00 00 00 49 89 F7 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AB8640ull))(argc, argv);
}

// Lisp: (spline-length? ...) | StringId: 0x746C2638 | Guest Offset: 0x00A256B0
int64_t DcSplineLengthP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 85 FF 7E 51 8B 36 85 F6 74 4B 48 8D 0D 67 94 62
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A256B0ull))(argc, argv);
}

// Lisp: (spline-locator-at-distance ...) | StringId: 0xB44C7CAC | Guest Offset: 0x00EA2315
int64_t DcSplineLocatorAtDistance(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 70 46 72 61 6D 65 00 44 63 53 70 6C 69 6E 65 4C 6F 63 61 74 6F 72 41 74
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00EA2315ull))(argc, argv);
}

// Lisp: (spline-transfer-pending? ...) | StringId: 0x03FB7B87 | Guest Offset: 0x00A25A20
int64_t DcSplineTransferPendingP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 0D 8B 1E 31 F6 89 DF E8 6B D9 2B 00 EB 14 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A25A20ull))(argc, argv);
}

// Lisp: (ss-error ...) | StringId: 0x77F4FEFA | Guest Offset: 0x00A44670
int64_t DcSsError(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 81 EC 28 02 00 00 48 8B 1D 7D 65 60 00 48 8B 03 48 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A44670ull))(argc, argv);
}

// Lisp: (start-movie-audio ...) | StringId: 0x0CD096E7 | Guest Offset: 0x009FE0D0
int64_t DcStartMovieAudio(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 05 05 28 66 00 48 8B 38 48 8B 06 C5 FA 2A 46 08 31 D2
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE0D0ull))(argc, argv);
}

// Lisp: (start-rumble ...) | StringId: 0x35140815 | Guest Offset: 0x009FF3D0
int64_t DcStartRumble(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 27 8B 1E 31 F6 89 DF E8 BB 3F 2E 00 C5 FA 10
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FF3D0ull))(argc, argv);
}

// Lisp: (state-character-accumulates-snow-from-ground ...) | StringId: 0x8532A3E0 | Guest Offset: 0x00A01FE0
int64_t DcStateCharacterAccumulatesSnowFromGround(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 18 83 3E 00 48 8D 05 7C 55 71 00 0F 95 80 C8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01FE0ull))(argc, argv);
}

// Lisp: (state-character-accumulates-snow-from-sky ...) | StringId: 0xB3F0137E | Guest Offset: 0x00A01F70
int64_t DcStateCharacterAccumulatesSnowFromSky(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 02 7C 4A C5 FA 10 46 08 83 3E 00 48 8D 05 E6 55
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01F70ull))(argc, argv);
}

// Lisp: (state-character-accumulates-wetness-from-water ...) | StringId: 0x5DDB29D7 | Guest Offset: 0x00A01F30
int64_t DcStateCharacterAccumulatesWetnessFromWater(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 18 83 3E 00 48 8D 05 2C 56 71 00 0F 95 80 CA
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01F30ull))(argc, argv);
}

// Lisp: (sticky-spawner ...) | StringId: 0xC6071DF8 | Guest Offset: 0x00A0B8F0
int64_t DcStickySpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 4C 8B 3D F9 F2 63 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0B8F0ull))(argc, argv);
}

// Lisp: (stop-animating ...) | StringId: 0xD9CB0C6D | Guest Offset: 0x00397AB0
int64_t DcStopAnimating(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 98 00 00 00 49 89 F4 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00397AB0ull))(argc, argv);
}

// Lisp: (stop-rumble ...) | StringId: 0xCF53AF61 | Guest Offset: 0x009FF370
int64_t DcStopRumble(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 44 8B 1E 31 F6 89 DF E8 1B 40 2E 00 89 DF E8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FF370ull))(argc, argv);
}

// Lisp: (stop-script-rumbles ...) | StringId: 0xE59BA17D | Guest Offset: 0x00A105B0
int64_t DcStopScriptRumbles(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: B8 35 00 00 00 48 8D 0D 14 F6 B1 00 0F 1F 40 00 80 7C 08 FF 00 74 0B 80
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A105B0ull))(argc, argv);
}

// Lisp: (stop-sound ...) | StringId: 0x6F26C192 | Guest Offset: 0x00A01160
int64_t DcStopSound(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 52 44 8B 36 45 85 F6 74 4A 48 8D 05 26 E0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01160ull))(argc, argv);
}

// Lisp: (stop-speaking ...) | StringId: 0x2279D61E | Guest Offset: 0x00A01570
int64_t DcStopSpeaking(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 4C 8B 3D 79 96 64 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01570ull))(argc, argv);
}

// Lisp: (string-from-float ...) | StringId: 0x5A5528D0 | Guest Offset: 0x009FDE60
int64_t DcStringFromFloat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 C5 F8 57 C0 85 FF 78 09 C5 F8 57 C0 C5 FA 5A 46 F8 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDE60ull))(argc, argv);
}

// Lisp: (string-from-string-id ...) | StringId: 0x1B2ADEA7 | Guest Offset: 0x009FDE30
int64_t DcStringFromStringId(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 85 FF 7E 09 8B 3E 31 F6 E9 63 55 2E 00 48 8D 05 1F FD 49 00 C3 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDE30ull))(argc, argv);
}

// Lisp: (string-id-from-string ...) | StringId: 0x9E1773DC | Guest Offset: 0x009FDE00
int64_t DcStringIdFromString(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 31 C0 85 FF 7E 15 48 8B 36 48 8D 05 B4 88 66 00 31 FF BA 01
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FDE00ull))(argc, argv);
}

// Lisp: (sullivan-flashlight-to-hand ...) | StringId: 0x59A3DA2C | Guest Offset: 0x00A0A6A0
int64_t DcSullivanFlashlightToHand(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D 69 51 64 00 48 8B 41 28 48 85 C0 0F 84 8D 00 00 00 8B 49 30 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0A6A0ull))(argc, argv);
}

// Lisp: (sullivan-flashlight-to-waist ...) | StringId: 0x11799CD1 | Guest Offset: 0x00A0A5F0
int64_t DcSullivanFlashlightToWaist(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 0D 19 52 64 00 48 8B 41 28 48 85 C0 0F 84 8D 00 00 00 8B 49 30 85
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0A5F0ull))(argc, argv);
}

// Lisp: (suppress-idle-speech ...) | StringId: 0xA2C6B659 | Guest Offset: 0x00A01360
int64_t DcSuppressIdleSpeech(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 85 FF 7E 60 C5 FA 10 06 48 8D 0D D9 03 71 00 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A01360ull))(argc, argv);
}

// Lisp: (swap-entity ...) | StringId: 0x326EF848 | Guest Offset: 0x00A09590
int64_t DcSwapEntity(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 48 89 F3 83 FF 02 0F 85 61 01
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A09590ull))(argc, argv);
}

// Lisp: (swim-allow-stand-on-bottom ...) | StringId: 0x5542401B | Guest Offset: 0x0090D6B0
int64_t DcSwimAllowStandOnBottom(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D 40 D5 73 00 48 8B 03 48 89 45 F0 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D6B0ull))(argc, argv);
}

// Lisp: (swim-disable ...) | StringId: 0x2C5C5A4A | Guest Offset: 0x0090D760
int64_t DcSwimDisable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 E8 D7 68 01 00 31 C0 5D C3 0F 1F 00 55 48 89 E5 E8 C7 69 01
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D760ull))(argc, argv);
}

// Lisp: (swim-disallow-stand-on-bottom ...) | StringId: 0x315FB50B | Guest Offset: 0x0090D680
int64_t DcSwimDisallowStandOnBottom(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 05 5D 48 85 00 48 8B 10 48 8D 35 7B 21 74 00 48 83 C6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D680ull))(argc, argv);
}

// Lisp: (swim-enable ...) | StringId: 0x4CBBC128 | Guest Offset: 0x0090D770
int64_t DcSwimEnable(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 E8 C7 69 01 00 31 C0 5D C3 0F 1F 00 48 8D 05 89 20 74 00 C7
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090D770ull))(argc, argv);
}

// Lisp: (symbol-equal? ...) | StringId: 0x6CDE9101 | Guest Offset: 0x00397620
int64_t DcSymbolEqualP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 8B 06 3B 46 08 0F 94 C0 0F B6 C0 C3 0F 1F 40 00 55 48 89 E5 48 83 EC 10
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00397620ull))(argc, argv);
}

// Lisp: (sync-audio-meta-data ...) | StringId: 0x6D35F28D | Guest Offset: 0x009FD560
int64_t DcSyncAudioMetaData(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8D 05 E5 41 71 00 80 78 08 00 74 0C 48 8D 3D D8 41 71 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD560ull))(argc, argv);
}

// Lisp: (sync-vox-meta-data ...) | StringId: 0x7A44FC2A | Guest Offset: 0x009FD1F0
int64_t DcSyncVoxMetaData(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 31 FF E8 C5 58 9E FF 31 C0 5D C3 90 55 48 89 E5 41 57 41 56
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD1F0ull))(argc, argv);
}

// Lisp: (sync-vox-mp-meta-data ...) | StringId: 0x41BF1CAF | Guest Offset: 0x009FD1E0
int64_t DcSyncVoxMpMetaData(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 E8 37 61 9E FF 31 C0 5D C3 0F 1F 00 55 48 89 E5 31 FF E8 C5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FD1E0ull))(argc, argv);
}

// Lisp: (tag-boolean ...) | StringId: 0x5EF1F0DB | Guest Offset: 0x00A43700
int64_t DcTagBoolean(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F6 4C 8B 25 E7 74 60
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A43700ull))(argc, argv);
}

// Lisp: (tag-float ...) | StringId: 0x4BF01CBF | Guest Offset: 0x00A439D0
int64_t DcTagFloat(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 20 49 89 F6 4C 8B 25 17 72 60
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A439D0ull))(argc, argv);
}

// Lisp: (tag-int-32 ...) | StringId: 0x5F6F364F | Guest Offset: 0x00A43880
int64_t DcTagInt32(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F6 4C 8B 25 67 73 60
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A43880ull))(argc, argv);
}

// Lisp: (tag-specified? ...) | StringId: 0xD510E933 | Guest Offset: 0x00A43DE0
int64_t DcTagSpecifiedP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F6 83 FF 01 0F 85 B1 00 00 00 4C 8D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A43DE0ull))(argc, argv);
}

// Lisp: (tag-string ...) | StringId: 0xC489284C | Guest Offset: 0x00A43B40
int64_t DcTagString(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F6 4C 8B 25 A7 70 60
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A43B40ull))(argc, argv);
}

// Lisp: (tag-symbol ...) | StringId: 0x0B1B1B8A | Guest Offset: 0x00A43C90
int64_t DcTagSymbol(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F6 4C 8B 25 57 6F 60
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A43C90ull))(argc, argv);
}

// Lisp: (tank-aim-at-obj ...) | StringId: 0xE36719FE | Guest Offset: 0x00AA2AC0
int64_t DcTankAimAtObj(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D 5A 5F 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2AC0ull))(argc, argv);
}

// Lisp: (tank-aim-at-pos ...) | StringId: 0x868E7A7B | Guest Offset: 0x00AA2E90
int64_t DcTankAimAtPos(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D 49 5C 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2E90ull))(argc, argv);
}

// Lisp: (tank-aim-at-pos-instant ...) | StringId: 0x1D39C0C0 | Guest Offset: 0x00AA2B10
int64_t DcTankAimAtPosInstant(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D C9 5F 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2B10ull))(argc, argv);
}

// Lisp: (tank-aim-forward ...) | StringId: 0x13D24475 | Guest Offset: 0x00AA2A70
int64_t DcTankAimForward(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D DD 5E 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2A70ull))(argc, argv);
}

// Lisp: (tank-dist-to-point ...) | StringId: 0x830C56D0 | Guest Offset: 0x00AA1300
int64_t DcTankDistToPoint(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 48 89 F3 83 FF 02 0F 8C C5 00 00 00 44 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA1300ull))(argc, argv);
}

// Lisp: (tank-find-best-target-mp ...) | StringId: 0xB22210CE | Guest Offset: 0x00AA4950
int64_t DcTankFindBestTargetMP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 38 49 89 F7 89 FB 45 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA4950ull))(argc, argv);
}

// Lisp: (tank-fire ...) | StringId: 0x0A96C6E3 | Guest Offset: 0x00AA2930
int64_t DcTankFire(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 4C 8B 3D B9 82 5A 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2930ull))(argc, argv);
}

// Lisp: (tank-fire-at-obj ...) | StringId: 0x9FD56E3A | Guest Offset: 0x00AA2EE0
int64_t DcTankFireAtObj(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D 6C 5C 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2EE0ull))(argc, argv);
}

// Lisp: (tank-fire-at-pos ...) | StringId: 0xFA3C0DBF | Guest Offset: 0x00AA2F30
int64_t DcTankFireAtPos(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D 55 5C 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2F30ull))(argc, argv);
}

// Lisp: (tank-force-fire-cannon ...) | StringId: 0x0FB6411A | Guest Offset: 0x00AA42C0
int64_t DcTankForceFireCannon(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 81 EC A0 00 00 00 49 89 F7 4C 8B 25
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA42C0ull))(argc, argv);
}

// Lisp: (tank-get-health ...) | StringId: 0x2666750F | Guest Offset: 0x00AA3D80
int64_t DcTankGetHealth(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 45 31 FF 85 FF 7E 70 8B 1E 45 31 FF 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA3D80ull))(argc, argv);
}

// Lisp: (tank-mg-cease-fire ...) | StringId: 0x99B74C7C | Guest Offset: 0x00AA2000
int64_t DcTankMgCeaseFire(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D 4D 69 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2000ull))(argc, argv);
}

// Lisp: (tank-mg-fire-at-loc ...) | StringId: 0x66E6E69A | Guest Offset: 0x00AA2120
int64_t DcTankMgFireAtLoc(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D 39 68 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2120ull))(argc, argv);
}

// Lisp: (tank-mg-fire-at-obj ...) | StringId: 0x5B33403B | Guest Offset: 0x00AA2680
int64_t DcTankMgFireAtObj(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D 9A 63 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2680ull))(argc, argv);
}

// Lisp: (tank-move-to ...) | StringId: 0x95A702CF | Guest Offset: 0x00AA33E0
int64_t DcTankMoveTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D C5 57 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA33E0ull))(argc, argv);
}

// Lisp: (tank-move-tracking-object ...) | StringId: 0xE30E0949 | Guest Offset: 0x00AA1550
int64_t DcTankMoveTrackingObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D E3 71 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA1550ull))(argc, argv);
}

// Lisp: (tank-reverse-to ...) | StringId: 0xAFEAD71C | Guest Offset: 0x00AA2F80
int64_t DcTankReverseTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F6 89 FB 48 8D 05 1E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA2F80ull))(argc, argv);
}

// Lisp: (tank-set-health ...) | StringId: 0x02A42435 | Guest Offset: 0x00AA3CC0
int64_t DcTankSetHealth(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 83 FF 02 0F 8C 83 00 00 00 41 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA3CC0ull))(argc, argv);
}

// Lisp: (tank-teleport ...) | StringId: 0x932ED089 | Guest Offset: 0x00AA1BA0
int64_t DcTankTeleport(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 48 89 F0 89 FE 48 8D 0D 24 6D 40 00 48 89 0C 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA1BA0ull))(argc, argv);
}

// Lisp: (tank-wait-teleport ...) | StringId: 0xB4E2D175 | Guest Offset: 0x00AA4420
int64_t DcTankWaitTeleport(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC C8 00 00 00 49 89 D6 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA4420ull))(argc, argv);
}

// Lisp: (task-elapsed-time ...) | StringId: 0x9B53A1B0 | Guest Offset: 0x00A102B0
int64_t DcTaskElapsedTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 48 89 F3 8B 3B 8B 73 08 E8 09 70 04 00 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A102B0ull))(argc, argv);
}

// Lisp: (task-failed ...) | StringId: 0x1B805494 | Guest Offset: 0x009FE900
int64_t DcTaskFailed(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 08 01 00 00 48 89 F3 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE900ull))(argc, argv);
}

// Lisp: (task-find ...) | StringId: 0x32EA7B74 | Guest Offset: 0x009FEB90
int64_t DcTaskFind(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 48 89 F3 8B 3B 8B 73 08 E8 29 87 05 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FEB90ull))(argc, argv);
}

// Lisp: (task-node-active? ...) | StringId: 0x8CE160DB | Guest Offset: 0x00A10340
int64_t DcTaskNodeActiveP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F7 4C 8B 35 A7 A8 63
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A10340ull))(argc, argv);
}

// Lisp: (task-subnode-active? ...) | StringId: 0x1A0506FC | Guest Offset: 0x009FEC80
int64_t DcTaskSubnodeActiveP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8B 3E E8 54 EA 05 00 0F B6 C0 5D C3 66 66 66 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FEC80ull))(argc, argv);
}

// Lisp: (task-subnode-complete ...) | StringId: 0xD4050C6E | Guest Offset: 0x009FECA0
int64_t DcTaskSubnodeComplete(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 81 EC 90 00 00 00 4C 8B 35 4B BF 64 00 49 8B 06
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FECA0ull))(argc, argv);
}

// Lisp: (task-subnode-play ...) | StringId: 0xC1A6D2BC | Guest Offset: 0x009FEC00
int64_t DcTaskSubnodePlay(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 18 48 8B 1D F0 BF 64 00 48 8B 03 48 89 45 F0 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FEC00ull))(argc, argv);
}

// Lisp: (throw-grenade ...) | StringId: 0x6333DA76 | Guest Offset: 0x002E2C30
int64_t DcThrowGrenade(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 40 4C 8B 3D BA 7F D6 00 49 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E2C30ull))(argc, argv);
}

// Lisp: (throw-sla-grenade ...) | StringId: 0x0EC5CFC6 | Guest Offset: 0x002E2810
int64_t DcThrowSlaGrenade(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 83 EC 68 4C 8B 3D DC 83 D6 00 49 8B 07 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E2810ull))(argc, argv);
}

// Lisp: (train-allow-unphysicalize ...) | StringId: 0x8C31728A | Guest Offset: 0x00A68430
int64_t DcTrainAllowUnphysicalize(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 89 FB 85 DB 7E 23 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A68430ull))(argc, argv);
}

// Lisp: (train-decouple ...) | StringId: 0xFA24ECD8 | Guest Offset: 0x00A67E40
int64_t DcTrainDecouple(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 24 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A67E40ull))(argc, argv);
}

// Lisp: (train-get-acceleration ...) | StringId: 0xE43BDB88 | Guest Offset: 0x00A68230
int64_t DcTrainGetAcceleration(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 85 FF 7E 10 8B 1E 31 F6 89 DF E8 56 B1
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A68230ull))(argc, argv);
}

// Lisp: (train-get-default-acceleration ...) | StringId: 0x4A413880 | Guest Offset: 0x00A682E0
int64_t DcTrainGetDefaultAcceleration(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 C7 44 24 F8 00 00 00 00 C7 44 24 F8 00 00 00 40 48 8B 44 24 F8 C3 66
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A682E0ull))(argc, argv);
}

// Lisp: (train-get-distance ...) | StringId: 0xE0D61234 | Guest Offset: 0x00A67B90
int64_t DcTrainGetDistance(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 10 85 FF 7E 10 8B 1E 31 F6 89 DF E8 F6 B7
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A67B90ull))(argc, argv);
}

// Lisp: (train-get-head-car ...) | StringId: 0xB9AD92C8 | Guest Offset: 0x00A67C40
int64_t DcTrainGetHeadCar(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 4A B7 27 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A67C40ull))(argc, argv);
}

// Lisp: (train-get-spline ...) | StringId: 0xFC1B8E09 | Guest Offset: 0x00A67AE0
int64_t DcTrainGetSpline(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 AA B8 27 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A67AE0ull))(argc, argv);
}

// Lisp: (train-kill-all-riders ...) | StringId: 0x705684E8 | Guest Offset: 0x00A68530
int64_t DcTrainKillAllRiders(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F4 89 FB 85 DB 7E 20 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A68530ull))(argc, argv);
}

// Lisp: (train-set-acceleration ...) | StringId: 0xA81CEFAE | Guest Offset: 0x00A68140
int64_t DcTrainSetAcceleration(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F4 89 FB 85 DB 0F 8E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A68140ull))(argc, argv);
}

// Lisp: (train-set-speed-mph ...) | StringId: 0xD4F03954 | Guest Offset: 0x00A67F60
int64_t DcTrainSetSpeedMph(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 89 FB 85 DB 7E 23 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A67F60ull))(argc, argv);
}

// Lisp: (train-spawn ...) | StringId: 0xCBF5C031 | Guest Offset: 0x00A67960
int64_t DcTrainSpawn(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 49 89 D6 85 FF 0F 8E 48 01 00 00 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A67960ull))(argc, argv);
}

// Lisp: (train-spawn-on-track ...) | StringId: 0x3D05016C | Guest Offset: 0x00A68740
int64_t DcTrainSpawnOnTrack(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 49 89 D6 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A68740ull))(argc, argv);
}

// Lisp: (train-teleport ...) | StringId: 0x9D5456E8 | Guest Offset: 0x00A67CD0
int64_t DcTrainTeleport(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 48 89 F3 41 89 FE 45 85 F6 7E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A67CD0ull))(argc, argv);
}

// Lisp: (train-track-player ...) | StringId: 0x011A0490 | Guest Offset: 0x00A68300
int64_t DcTrainTrackPlayer(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 F7 89 FB 85 DB 7E 23 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A68300ull))(argc, argv);
}

// Lisp: (train-transfer-pending? ...) | StringId: 0xC14382CE | Guest Offset: 0x00A685E0
int64_t DcTrainTransferPendingP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 4C 8B 3D 09 26 5E 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A685E0ull))(argc, argv);
}

// Lisp: (transfer-to-spline ...) | StringId: 0xF515BD98 | Guest Offset: 0x00A25A90
int64_t DcTransferToSpline(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 49 89 F4 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A25A90ull))(argc, argv);
}

// Lisp: (translate-y-loop ...) | StringId: 0x913BC948 | Guest Offset: 0x009FC590
int64_t DcTranslateYLoop(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 10 49 89 F4 89 FB 85 DB 7E 5D
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FC590ull))(argc, argv);
}

// Lisp: (turn-on-flashlight ...) | StringId: 0x48B1B449 | Guest Offset: 0x00A0A250
int64_t DcTurnOnFlashlight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 08 01 00 00 49 89 F7 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0A250ull))(argc, argv);
}

// Lisp: (tutorial-hints-on? ...) | StringId: 0xDF85EA60 | Guest Offset: 0x00A0F490
int64_t DcTutorialHintsOnP(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 A9 A1 64 00 48 63 80 B0 48 00 00 C3 90 55 48 89 E5 53 48 83 EC
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0F490ull))(argc, argv);
}

// Lisp: (unblock-nav-poly-under-object ...) | StringId: 0x7E5AD594 | Guest Offset: 0x00A04FC0
int64_t DcUnblockNavPolyUnderObject(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 57 8B 1E 31 F6 89 DF E8 CA E3 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A04FC0ull))(argc, argv);
}

// Lisp: (unhide-message ...) | StringId: 0xE40CA42A | Guest Offset: 0x005DCAF0
int64_t DcUnhideMessage(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 4C 8B 05 01 E1 A6 00 49 8B 00 48 89 45 F8 8B 36
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DCAF0ull))(argc, argv);
}

// Lisp: (unhide-message-deprecated ...) | StringId: 0xB2127739 | Guest Offset: 0x005DC700
int64_t DcUnhideMessageDeprecated(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 10 4C 8B 05 F1 E4 A6 00 49 8B 00 48 89 45 F8 8B 36
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x005DC700ull))(argc, argv);
}

// Lisp: (units-per-frame ...) | StringId: 0x319F4E3C | Guest Offset: 0x00A0ECF0
int64_t DcUnitsPerFrame(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 85 FF 7E 4D C5 FA 10 06 B8 01 00 00 00 83 FF 02 7C 19 48 63 4E 08 B8 01
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0ECF0ull))(argc, argv);
}

// Lisp: (units-per-second ...) | StringId: 0x0EEC468D | Guest Offset: 0x00A0EC70
int64_t DcUnitsPerSecond(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 85 FF 7E 5F C5 FA 10 06 B8 01 00 00 00 83 FF 02 7C 19 48 63 4E 08 B8 01
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0EC70ull))(argc, argv);
}

// Lisp: (unparent ...) | StringId: 0x71977AEC | Guest Offset: 0x00A03940
int64_t DcUnparent(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 85 FF 7E 10 8B 1E 31 F6 89 DF E8 4A FA 2D 00 49 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A03940ull))(argc, argv);
}

// Lisp: (unsticky-spawner ...) | StringId: 0xF8717A0A | Guest Offset: 0x00A0B7C0
int64_t DcUnstickySpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 4C 8B 3D 29 F4 63 00 49
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0B7C0ull))(argc, argv);
}

// Lisp: (update-ap-ref ...) | StringId: 0x6D7DD92D | Guest Offset: 0x00397520
int64_t DcUpdateApRef(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 48 81 EC 88 00 00 00 48 89 F3 4C 8B 3D C6 36
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00397520ull))(argc, argv);
}

// Lisp: (update-dynamic-light ...) | StringId: 0x2F5DC938 | Guest Offset: 0x009FEEE0
int64_t DcUpdateDynamicLight(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 53 50 49 89 F7 89 FB 83 FB 03 0F 8C FE 00 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FEEE0ull))(argc, argv);
}

// Lisp: (update-fight-ref ...) | StringId: 0x7347F3A6 | Guest Offset: 0x00667F80
int64_t DcUpdateFightRef(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC F8 00 00 00 89 F8 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00667F80ull))(argc, argv);
}

// Lisp: (update-task-menu ...) | StringId: 0x5047B2C8 | Guest Offset: 0x009FE8C0
int64_t DcUpdateTaskMenu(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 8B 3D AD D4 B3 00 48 85 FF 74 13 48 8B 07 FF 50 20 48 8B
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x009FE8C0ull))(argc, argv);
}

// Lisp: (use-audio-rain-level ...) | StringId: 0x00C11B61 | Guest Offset: 0x00A009F0
int64_t DcUseAudioRainLevel(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 16 8B 06 48 8D 0D 4D 0D 71 00 89 81 94 05 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A009F0ull))(argc, argv);
}

// Lisp: (use-material-from-water-unit ...) | StringId: 0x8589A890 | Guest Offset: 0x00A02020
int64_t DcUseMaterialFromWaterUnit(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 83 FF 02 0F 8C 8D 00 00 00 48 8D 15 49 29 76 00 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02020ull))(argc, argv);
}

// Lisp: (use-rain-intensity-values ...) | StringId: 0x8FCBB113 | Guest Offset: 0x00A031C0
int64_t DcUseRainIntensityValues(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 07 0F 8C 9B 00 00 00 C5 7A 10 06 C5 FA 10 4E 08
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A031C0ull))(argc, argv);
}

// Lisp: (use-reflection-plane ...) | StringId: 0x8F0342EB | Guest Offset: 0x00A029A0
int64_t DcUseReflectionPlane(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 55 C5 FA 10 06 C5 FA 10 4E 08 C4 E3 79 04 C0
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A029A0ull))(argc, argv);
}

// Lisp: (use-reflections ...) | StringId: 0x91155CD8 | Guest Offset: 0x00A02C10
int64_t DcUseReflections(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 15 83 3E 00 48 8D 05 5C 1D 76 00 0F 95 40 14
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02C10ull))(argc, argv);
}

// Lisp: (use-visible-water-unit-for-reflection ...) | StringId: 0x23D4A292 | Guest Offset: 0x00A02BC0
int64_t DcUseVisibleWaterUnitForReflection(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 85 FF 7E 23 83 3E 00 BB 01 00 00 00 74 29 48 8D 05 A5
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02BC0ull))(argc, argv);
}

// Lisp: (use-visible-water-unit-for-reflection-with-tolerance ...) | StringId: 0x969C4BC6 | Guest Offset: 0x00A02B70
int64_t DcUseVisibleWaterUnitForReflectionWithTolerance(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 02 7C 23 83 3E 00 BB 01 00 00 00 74 29 48 8D 05
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02B70ull))(argc, argv);
}

// Lisp: (use-water-unit-for-reflection ...) | StringId: 0x6FFB28A3 | Guest Offset: 0x00A02C70
int64_t DcUseWaterUnitForReflection(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 50 83 FF 02 7C 29 83 7E 08 00 BB 01 00 00 00 74 2E 8B 06
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02C70ull))(argc, argv);
}

// Lisp: (use-water-unit-for-reflection-if-visible ...) | StringId: 0xE75D4472 | Guest Offset: 0x00A02C50
int64_t DcUseWaterUnitForReflectionIfVisible(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 8D 05 29 1D 76 00 C6 40 5D 01 B8 01 00 00 00 C3 66 66 66 66 66 66 2E
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A02C50ull))(argc, argv);
}

// Lisp: (use-wind-intensity-values ...) | StringId: 0xDD75C117 | Guest Offset: 0x00A03290
int64_t DcUseWindIntensityValues(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 56 53 48 83 EC 20 4C 8B 35 5E 79 64 00 49 8B 06 48 89 45
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A03290ull))(argc, argv);
}

// Lisp: (wait ...) | StringId: 0x568EAB14 | Guest Offset: 0x00A3E690
int64_t DcWait(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 58 49 89 D7 41 89 FD 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A3E690ull))(argc, argv);
}

// Lisp: (wait-animate ...) | StringId: 0x933344D6 | Guest Offset: 0x003969A0
int64_t DcWaitAnimate(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 D7 48 89 75 B8
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x003969A0ull))(argc, argv);
}

// Lisp: (wait-boolean ...) | StringId: 0x9C2425A8 | Guest Offset: 0x00A41610
int64_t DcWaitBoolean(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 58 48 89 55 A0 49 89 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A41610ull))(argc, argv);
}

// Lisp: (wait-for ...) | StringId: 0xCA1359A3 | Guest Offset: 0x00A3FC50
int64_t DcWaitFor(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 58 41 89 FD 48 8B 1D 95
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A3FC50ull))(argc, argv);
}

// Lisp: (wait-for-npc-not-busy ...) | StringId: 0x83ADC6C2 | Guest Offset: 0x002DF6A0
int64_t DcWaitForNpcNotBusy(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 D4 85 FF 7E 10
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DF6A0ull))(argc, argv);
}

// Lisp: (wait-frames ...) | StringId: 0x6E22F8D5 | Guest Offset: 0x00A41270
int64_t DcWaitFrames(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 58 49 89 D7 41 89 FD 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A41270ull))(argc, argv);
}

// Lisp: (wait-heli-fire-missile ...) | StringId: 0x203DF587 | Guest Offset: 0x00A7EE30
int64_t DcWaitHeliFireMissile(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 81 EC 88 00 00 00 49 89 F4 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7EE30ull))(argc, argv);
}

// Lisp: (wait-heli-move-to ...) | StringId: 0xCD5F5D97 | Guest Offset: 0x00A7FA70
int64_t DcWaitHeliMoveTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 F5 89 FB 85 DB
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7FA70ull))(argc, argv);
}

// Lisp: (wait-heli-move-to-spline ...) | StringId: 0x83064E7E | Guest Offset: 0x00A7F800
int64_t DcWaitHeliMoveToSpline(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 48 89 D3 49 89 F7 41
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A7F800ull))(argc, argv);
}

// Lisp: (wait-int-32 ...) | StringId: 0xB154C41C | Guest Offset: 0x00A41D40
int64_t DcWaitInt32(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 58 48 89 55 A0 49 89 F7
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A41D40ull))(argc, argv);
}

// Lisp: (wait-npc-bind ...) | StringId: 0xA011225A | Guest Offset: 0x002E1E80
int64_t DcWaitNpcBind(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 D6 85 FF 7E 10
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E1E80ull))(argc, argv);
}

// Lisp: (wait-npc-leave-scripted-ap ...) | StringId: 0xFD09B97F | Guest Offset: 0x002DDD20
int64_t DcWaitNpcLeaveScriptedAp(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 D6 85 FF 7E 0D 8B 1E 31
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DDD20ull))(argc, argv);
}

// Lisp: (wait-npc-move-to ...) | StringId: 0x9240B713 | Guest Offset: 0x002E15F0
int64_t DcWaitNpcMoveTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 89 D0 BA 01 00 00 00 48 89 C1 E9 00 00 00 00 55 48 89 E5 41 57 41 56
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E15F0ull))(argc, argv);
}

// Lisp: (wait-npc-move-to-spawner ...) | StringId: 0x8E74A4E8 | Guest Offset: 0x002E0F10
int64_t DcWaitNpcMoveToSpawner(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 48 89 D0 BA 01 00 00 00 48 89 C1 E9 00 00 00 00 55 48 89 E5 41 57 41 56
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E0F10ull))(argc, argv);
}

// Lisp: (wait-npc-turn-to ...) | StringId: 0xA86441C8 | Guest Offset: 0x002DF9A0
int64_t DcWaitNpcTurnTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 48 49 89 F4 41 89 FD 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DF9A0ull))(argc, argv);
}

// Lisp: (wait-npc-use-closest-cover ...) | StringId: 0xCD422D55 | Guest Offset: 0x002E4130
int64_t DcWaitNpcUseClosestCover(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 48 89 55 D0 49 89 F7
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002E4130ull))(argc, argv);
}

// Lisp: (wait-npc-use-scripted-ap ...) | StringId: 0x99E4D4D8 | Guest Offset: 0x002DDF20
int64_t DcWaitNpcUseScriptedAp(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 49 89 D7 49 89 F6 89
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x002DDF20ull))(argc, argv);
}

// Lisp: (wait-one-frame ...) | StringId: 0xB7ECEEDC | Guest Offset: 0x00A40F70
int64_t DcWaitOneFrame(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 D6 4C 8D 25 98 F3 60 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A40F70ull))(argc, argv);
}

// Lisp: (wait-player-move-to ...) | StringId: 0x061AD4D4 | Guest Offset: 0x00A0B350
int64_t DcWaitPlayerMoveTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 48 89 55 C8 45 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0B350ull))(argc, argv);
}

// Lisp: (wait-player-turn-to ...) | StringId: 0x3C3E220F | Guest Offset: 0x00A0AFC0
int64_t DcWaitPlayerTurnTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 18 48 89 55 C8 45 31 F6
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A0AFC0ull))(argc, argv);
}

// Lisp: (wait-say ...) | StringId: 0x95F6B8DD | Guest Offset: 0x00A016D0
int64_t DcWaitSay(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 38 48 89 55 A8 48 89 75
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A016D0ull))(argc, argv);
}

// Lisp: (wait-spawn-entity ...) | StringId: 0xFD166E79 | Guest Offset: 0x00A09E90
int64_t DcWaitSpawnEntity(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 20 49 89 D6 4C 8B 25 57 0D 64
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A09E90ull))(argc, argv);
}

// Lisp: (wait-spawn-entity-at ...) | StringId: 0xDB171895 | Guest Offset: 0x00A09DC0
int64_t DcWaitSpawnEntityAt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 48 83 EC 28 49 89 F4 41 89 FD 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A09DC0ull))(argc, argv);
}

// Lisp: (wait-spline-distance ...) | StringId: 0x98B7BBFC | Guest Offset: 0x00A25740
int64_t DcWaitSplineDistance(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 48 89 75 D0 41 89 FF 4C 8D 35
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A25740ull))(argc, argv);
}

// Lisp: (wait-tank-aim-at-obj ...) | StringId: 0x4C67FDBD | Guest Offset: 0x00AA3A40
int64_t DcWaitTankAimAtObj(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 20 48 89 F0 89 FE 48 89 54 24 18 48 8D 0D D5 4F 40
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA3A40ull))(argc, argv);
}

// Lisp: (wait-tank-aim-at-pos ...) | StringId: 0x298E9E38 | Guest Offset: 0x00AA3AA0
int64_t DcWaitTankAimAtPos(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 20 48 89 F0 89 FE 48 89 54 24 18 48 8D 0D 34 50 40
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA3AA0ull))(argc, argv);
}

// Lisp: (wait-tank-aim-forward ...) | StringId: 0xDA56ECC6 | Guest Offset: 0x00AA3430
int64_t DcWaitTankAimForward(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 20 48 89 F0 89 FE 48 89 54 24 18 48 8D 0D 18 55 40
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA3430ull))(argc, argv);
}

// Lisp: (wait-tank-fire-at-obj ...) | StringId: 0x5651C689 | Guest Offset: 0x00AA3B00
int64_t DcWaitTankFireAtObj(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 20 48 89 F0 89 FE 48 89 54 24 18 48 8D 0D 47 50 40
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA3B00ull))(argc, argv);
}

// Lisp: (wait-tank-fire-at-pos ...) | StringId: 0x33B8A50C | Guest Offset: 0x00AA3B60
int64_t DcWaitTankFireAtPos(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 20 48 89 F0 89 FE 48 89 54 24 18 48 8D 0D 20 50 40
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA3B60ull))(argc, argv);
}

// Lisp: (wait-tank-move-to ...) | StringId: 0x9C8CD4BC | Guest Offset: 0x00AA3C60
int64_t DcWaitTankMoveTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 48 83 EC 20 48 89 F0 89 FE 48 89 54 24 18 48 8D 0D 40 4F 40
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA3C60ull))(argc, argv);
}

// Lisp: (wait-tank-reverse-to ...) | StringId: 0x00EA335F | Guest Offset: 0x00AA3BC0
int64_t DcWaitTankReverseTo(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 54 53 48 83 EC 20 49 89 F6 89 FB 48 89 54 24
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00AA3BC0ull))(argc, argv);
}

// Lisp: (wait-until ...) | StringId: 0xD1E7C609 | Guest Offset: 0x00A40E60
int64_t DcWaitUntil(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 D7 49 89 F6 41 89 FD 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A40E60ull))(argc, argv);
}

// Lisp: (wait-until-frame ...) | StringId: 0x19D1CE7D | Guest Offset: 0x00A40D40
int64_t DcWaitUntilFrame(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 D7 49 89 F6 41 89 FD 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A40D40ull))(argc, argv);
}

// Lisp: (wait-until-net-time ...) | StringId: 0xBC239ABE | Guest Offset: 0x00A40870
int64_t DcWaitUntilNetTime(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 41 57 41 56 41 55 41 54 53 50 49 89 D7 49 89 F6 41 89 FD 4C
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A40870ull))(argc, argv);
}

// Lisp: (weapon-set-reset ...) | StringId: 0x212F3614 | Guest Offset: 0x0090AD00
int64_t DcWeaponSetReset(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 BF 4E 0E 9D 45 E8 62 B1 10 00 48 8D 3D 8B 34 C3 00 B9 0F 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x0090AD00ull))(argc, argv);
}

// Lisp: (x-spawn-npc-at ...) | StringId: 0x1E7DFD23 | Guest Offset: 0x00A64C40
int64_t DcXSpawnNpcAt(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 55 48 89 E5 53 48 83 EC 28 48 8B 1D B0 5F 5E 00 48 8B 03 48 89 45 F0 48
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00A64C40ull))(argc, argv);
}

// Lisp: (y-6-u-lsa ...) | StringId: 0x73D1A78D | Guest Offset: 0x00000000
int64_t DcY6uLSA(int32_t argc, uint64_t *argv, uint8_t *image) {
    // Prologue bytes: 2F 6C 69 62 65 78 65 63 2F 6C 64 2D 65 6C 66 2E 73 6F 2E 31 00 00 00 00
    typedef int64_t (__attribute__((sysv_abi)) *GuestFn)(int32_t, uint64_t *);
    return ((GuestFn)(image + 0x00000000ull))(argc, argv);
}

