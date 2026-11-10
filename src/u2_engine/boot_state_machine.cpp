// Uncharted 2: Among Thieves — Boot State Machine & Settings Engine Subsystems
// Pure C++23 Reimplementation (Zero PS4 Code Execution)
// ADR-006: Dual-Executable Architecture.

#include "u2_engine.h"
#include <cstdlib>
#include <cstring>
#include <cstdint>

namespace {

// Memory pools allocated dynamically for Settings and ProcessList
static uint8_t *s_settings_pool = nullptr;
static uint8_t **s_settings_hash_buckets = nullptr;
static uint8_t *s_settings_nodes = nullptr;
static void *s_process_node_pool = nullptr;

} // namespace

/* 0x00A1C710 — Settings::Init(void)
 * Allocates the 8192-slot settings pool (0xE0000 bytes = 8192 * 0x70),
 * clears the slot bitmap at 0x01534CE0 (0x400 bytes), sets up the bucket
 * hash table at 0x01535100, initializes the node free list at 0x01535108,
 * and sets the initialized flag at 0x01535170.
 */
void ABI u2_cpp_Settings_Init(void) {
    if (!g_u2_image_base) return;

    constexpr size_t kPoolSize = 0x2000 * 0x70; // 8192 settings * 112 bytes = 917,504 bytes
    if (!s_settings_pool) {
        s_settings_pool = static_cast<uint8_t *>(std::malloc(kPoolSize));
    }
    if (s_settings_pool) {
        std::memset(s_settings_pool, 0, kPoolSize);
    }
    *u2::engine::elf_ptr<uint8_t *>(0x015350E0) = s_settings_pool;

    // Clear allocation bitmap (1024 bytes = 8192 bits)
    uint64_t *bitmap = u2::engine::elf_ptr<uint64_t>(0x01534CE0);
    std::memset(bitmap, 0, 0x400);

    // Set modulo hash count
    *u2::engine::elf_ptr<int32_t>(0x0153513C) = 8191;

    // Allocate hash buckets if needed
    constexpr size_t kHashSize = sizeof(void *) * 8192;
    if (!s_settings_hash_buckets) {
        s_settings_hash_buckets = static_cast<uint8_t **>(std::malloc(kHashSize));
    }
    if (s_settings_hash_buckets) {
        std::memset(s_settings_hash_buckets, 0, kHashSize);
    }
    *u2::engine::elf_ptr<uint8_t **>(0x01535100) = s_settings_hash_buckets;

    // Allocate and link 8192 hash nodes (32 bytes each: +0 next, +8 prev, +16 setting)
    constexpr size_t kNumNodes = 8192;
    if (!s_settings_nodes) {
        s_settings_nodes = static_cast<uint8_t *>(std::malloc(kNumNodes * 32));
    }
    if (s_settings_nodes) {
        std::memset(s_settings_nodes, 0, kNumNodes * 32);
        for (size_t i = 0; i < kNumNodes - 1; ++i) {
            uint8_t *curr = s_settings_nodes + i * 32;
            uint8_t *next = s_settings_nodes + (i + 1) * 32;
            *reinterpret_cast<uint8_t **>(curr + 0x00) = next;
        }
        *reinterpret_cast<uint8_t **>(s_settings_nodes + (kNumNodes - 1) * 32) = nullptr;
    }
    *u2::engine::elf_ptr<uint8_t *>(0x01535108) = s_settings_nodes;

    // Set initialized flag
    *u2::engine::elf_ptr<uint8_t>(0x01535170) = 1;
}

/* 0x00A1CD50 — Settings::NewSetting(void *self, uint64_t key, const char *name, uint32_t type, void *parent)
 * Allocates a slot from the settings pool via the slot bitmap, initializes
 * the Setting struct, and inserts it into the bucket hash table.
 */
void * ABI u2_cpp_Settings_NewSetting(void *self, uint64_t key, const char *name, uint32_t type, void *parent) {
    if (!g_u2_image_base) return nullptr;

    uint8_t *settings_base = *u2::engine::elf_ptr<uint8_t *>(0x015350E0);
    if (!settings_base) {
        u2_cpp_Settings_Init();
        settings_base = *u2::engine::elf_ptr<uint8_t *>(0x015350E0);
        if (!settings_base) return nullptr;
    }

    uint64_t *bitmap = u2::engine::elf_ptr<uint64_t>(0x01534CE0);
    int32_t slot = -1;

    // Find first free slot in 64-bit bitmap
    for (int32_t i = 0; i < 128; ++i) {
        if (bitmap[i] != 0xFFFFFFFFFFFFFFFFULL) {
            for (int32_t b = 0; b < 64; ++b) {
                if (!(bitmap[i] & (1ULL << b))) {
                    bitmap[i] |= (1ULL << b);
                    slot = i * 64 + b;
                    break;
                }
            }
            break;
        }
    }

    if (slot < 0 || slot >= 0x2000) return nullptr;

    uint8_t *s = settings_base + static_cast<size_t>(slot) * 0x70;
    std::memset(s, 0, 0x70);
    *reinterpret_cast<void **>(s + 0x00) = parent ? parent : self;
    *reinterpret_cast<uint64_t *>(s + 0x18) = key;
    *reinterpret_cast<uint32_t *>(s + 0x20) = type;
    *reinterpret_cast<const char **>(s + 0x28) = name;
    s[0x61] = 0x03; // Bit 0 (active) | Bit 1 (allocated)

    // Allocate node from free list and insert into bucket
    uint8_t **free_head = u2::engine::elf_ptr<uint8_t *>(0x01535108);
    int32_t mod = *u2::engine::elf_ptr<const int32_t>(0x0153513C);
    uint8_t **buckets = *u2::engine::elf_ptr<uint8_t **>(0x01535100);

    if (free_head && *free_head && mod > 0 && buckets) {
        uint8_t *node = *free_head;
        *free_head = *reinterpret_cast<uint8_t **>(node + 0x00);

        *reinterpret_cast<uint8_t **>(node + 0x10) = s;
        uint32_t b_idx = static_cast<uint32_t>(key) % static_cast<uint32_t>(mod);
        uint8_t *first = buckets[b_idx];

        *reinterpret_cast<uint8_t **>(node + 0x00) = first;
        *reinterpret_cast<uint8_t **>(node + 0x08) = nullptr;
        if (first && reinterpret_cast<uintptr_t>(first) >= 0x10000) {
            *reinterpret_cast<uint8_t **>(first + 0x08) = node;
        }
        buckets[b_idx] = node;
    }

    return s;
}

/* 0x009B0A30 — ProcessList::StartUp(void)
 * Initializes g_pProcessListNodePool (0x0151FF70) with a pool of process list nodes.
 */
void ABI u2_cpp_ProcessList_StartUp(void) {
    if (!g_u2_image_base) return;

    void **pool_ptr = u2::engine::elf_ptr<void *>(0x0151FF70);
    if (!*pool_ptr) {
        constexpr size_t kPoolSize = 512 * 32; // 512 nodes of 32 bytes = 16 KiB
        if (!s_process_node_pool) {
            s_process_node_pool = std::malloc(kPoolSize);
        }
        if (s_process_node_pool) {
            std::memset(s_process_node_pool, 0, kPoolSize);
        }
        *pool_ptr = s_process_node_pool;
    }
}

/* 0x009B9530 — ProcessMgr::Init(void *mgr)
 * Initializes the ProcessMgr root structure (0x0115F1A0) and boots process list.
 */
void ABI u2_cpp_ProcessMgr_Init(void *mgr) {
    if (!g_u2_image_base) return;

    u2_cpp_ProcessList_StartUp();
    void *target_mgr = mgr ? mgr : u2::engine::elf_ptr<void>(0x0115F1A0);
    auto *m = static_cast<uint8_t *>(target_mgr);
    std::memset(m, 0, 0xD00);
    *reinterpret_cast<uint32_t *>(m + 0x00) = 1; // initialized
}

/* 0x00B783F0 — GameLoadingMountInitialArchives(void)
 * Registers core archive (u2data/build/main/bin.psarc) into g_CoreArchives (0x015AF450).
 */
uint32_t ABI u2_cpp_GameLoading_MountInitialArchives(void) {
    if (!g_u2_image_base) return 0;

    auto *num_archives = u2::engine::elf_ptr<uint64_t>(0x015AF448);
    auto *archives = u2::engine::elf_ptr<uint8_t>(0x015AF450);
    if (*num_archives < 32) {
        size_t idx = static_cast<size_t>(*num_archives);
        uint8_t *entry = archives + idx * 24;
        static const char s_archive_path[] = "u2data/build/main/bin.psarc";
        static const char s_mount_point[]  = "build/main/";
        *reinterpret_cast<const char **>(entry + 0x00) = s_archive_path;
        *reinterpret_cast<const char **>(entry + 0x08) = s_mount_point;
        *reinterpret_cast<uint64_t *>(entry + 0x10) = 1; // Mounted flag
        *num_archives = idx + 1;
    }
    return 1;
}

/* 0x00B78790 — GameLoadingPlayGoWait(int32_t timeout_ms)
 * Simulates optical/PlayGo wait for streaming chunk ready. Returns 0 immediately on PC.
 */
uint32_t ABI u2_cpp_GameLoading_PlayGoWait(int32_t timeout_ms) {
    (void)timeout_ms;
    return 0;
}

/* 0x00A551B0 — IntroState::Update(void *self)
 * Intro logo presentation (sce-ndi-logos, bp-logos). Returns 1 when ready to enter Menu.
 */
int32_t ABI u2_cpp_IntroState_Update(void *self) {
    (void)self;
    return 1;
}

/* 0x00D59A50 — UCCApp::StateMachineUpdate(void *self, float dt)
 * Core Uncharted 2 state machine. Transitions from State 1 (I/O Mount)
 * through States 2-6 up to State 7 (Main Menu).
 */
int32_t ABI u2_cpp_UCCApp_StateMachineUpdate(void *self, float dt) {
    (void)dt;
    if (!self) return 0;

    auto *app = static_cast<uint8_t *>(self);
    int32_t current_state = *reinterpret_cast<int32_t *>(app + 0x00);

    switch (current_state) {
    case 0:
    case 1: { // Initial loading: mount core archives
        u2_cpp_GameLoading_MountInitialArchives();
        *reinterpret_cast<int32_t *>(app + 0x00) = 2;
        break;
    }
    case 2: { // Archives ready (sceFiosOpIsDone complete)
        *reinterpret_cast<int32_t *>(app + 0x00) = 3;
        break;
    }
    case 3: { // Mount auxiliary archives (core.psarc, uiextra.psarc)
        *reinterpret_cast<int32_t *>(app + 0x00) = 4;
        break;
    }
    case 4: { // Initialize settings and process system
        u2_cpp_Settings_Init();
        u2_cpp_ProcessMgr_Init(nullptr);
        *reinterpret_cast<int32_t *>(app + 0x00) = 5;
        break;
    }
    case 5: { // Check save game data / user profile
        *reinterpret_cast<int32_t *>(app + 0x00) = 6;
        break;
    }
    case 6: { // Hide splash screen, transition to Menu State
        *reinterpret_cast<int32_t *>(app + 0x00) = 7;
        break;
    }
    case 7: { // MAIN MENU STATE
        // Configure menu camera floats matching 0x00D59ECE
        *reinterpret_cast<int32_t *>(app + 0x04) = 0;
        *reinterpret_cast<float *>(app + 0x08) = 100.0f;
        *reinterpret_cast<float *>(app + 0xE0) = -80.0f;
        *reinterpret_cast<float *>(app + 0xE4) = 4.0f;
        *reinterpret_cast<float *>(app + 0x0C) = -80.0f;
        *reinterpret_cast<float *>(app + 0x10) = 1.0f;
        if (g_u2_image_base) {
            *u2::engine::elf_ptr<uint32_t>(0x0147DA0C) = 0;
        }
        break;
    }
    default:
        break;
    }

    return *reinterpret_cast<int32_t *>(app + 0x00);
}
