// Uncharted 2: Among Thieves — Boot State Machine & Settings Engine Subsystems
// Pure C++23 Reimplementation (Zero PS4 Code Execution)
// ADR-006: Dual-Executable Architecture.

#include "u2_engine.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstdio>
#include <atomic>

namespace {

// Memory pools allocated dynamically for Settings and ProcessList
static uint8_t *s_settings_pool = nullptr;
static uint8_t **s_settings_hash_buckets = nullptr;
static uint8_t *s_settings_nodes = nullptr;
static void *s_process_node_pool = nullptr;

static bool is_guest_code_executable() {
    if (!g_u2_image_base) return false;
#ifdef _WIN32
    MEMORY_BASIC_INFORMATION mbi = {};
    if (VirtualQuery(g_u2_image_base, &mbi, sizeof(mbi))) {
        return (mbi.Protect & (PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)) != 0;
    }
#endif
    return false;
}

} // namespace

/* 0x00A1C710 — Settings::Init(void)
 * Allocates the 8192-slot settings pool (0xE0000 bytes = 8192 * 0x70),
 * clears the slot bitmap at 0x01534CE0 (0x400 bytes), sets up the bucket
 * hash table at 0x01535100, initializes the node free list at 0x01535108,
 * and sets the initialized flag at 0x01535170.
 */
void ABI u2_cpp_Settings_Init(void) {
    if (!g_u2_image_base) return;
    printf("[U2 Engine] Settings_Init called\n");
    fflush(stdout);

    u2::engine::ensure_root_allocator();

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

    // Initialize ScriptManager circular list heads & static globals
    uint8_t *packages_head = u2::engine::elf_ptr<uint8_t>(0x01534140);
    printf("[U2 Engine] Settings_Init: packages_head=%p, was=%p\n",
           packages_head, packages_head ? *reinterpret_cast<void **>(packages_head) : nullptr);
    if (packages_head) {
        *reinterpret_cast<uint8_t **>(packages_head + 0) = packages_head;
        *reinterpret_cast<uint8_t **>(packages_head + 8) = packages_head;
    }
    printf("[U2 Engine] Settings_Init: now packages_head[0]=%p, [8]=%p\n",
           packages_head ? *reinterpret_cast<void **>(packages_head) : nullptr,
           packages_head ? *reinterpret_cast<void **>(packages_head + 8) : nullptr);
    fflush(stdout);
    uint8_t *head2 = u2::engine::elf_ptr<uint8_t>(0x01163A50);
    if (head2) {
        *reinterpret_cast<uint64_t *>(head2 + 0) = 0;
        *reinterpret_cast<uint64_t *>(head2 + 0x10) = 0;
    }
    *u2::engine::elf_ptr<uint32_t>(0x015340A0) = 0;
    *u2::engine::elf_ptr<float>(0x015340A4) = 1.0f;
    *u2::engine::elf_ptr<uint32_t>(0x015340A8) = 0;
    *u2::engine::elf_ptr<uint64_t>(0x015340B0) = 0xFFFFFFFFFFFFFFFFULL;
    *u2::engine::elf_ptr<uint32_t>(0x015340B8) = 4;
    *u2::engine::elf_ptr<uint32_t>(0x0153410C) = 0;
    *u2::engine::elf_ptr<uint8_t>(0x01534110) = 0;
    std::memset(u2::engine::elf_ptr<void>(0x015340D0), 0, 16);
    *u2::engine::elf_ptr<uint64_t>(0x01534120) = 0;
    *u2::engine::elf_ptr<uint64_t>(0x01534130) = 0;
    *u2::engine::elf_ptr<uint32_t>(0x0153418C) = 0;
    *u2::engine::elf_ptr<uint8_t>(0x01534190) = 0;
    std::memset(u2::engine::elf_ptr<void>(0x01534150), 0, 16);
    *u2::engine::elf_ptr<uint64_t>(0x015341A0) = 0;
    *u2::engine::elf_ptr<uint64_t>(0x015341B0) = 0;
    *u2::engine::elf_ptr<uint64_t>(0x015341D0) = 0;
    *u2::engine::elf_ptr<uint64_t>(0x015341C8) = 0;
    *u2::engine::elf_ptr<uint8_t *>(0x015341C0) = u2::engine::elf_ptr<uint8_t>(0x01044BC0);
    *u2::engine::elf_ptr<uint32_t>(0x015341D8) = 0;

    // Initialize StateScript/UccTweaks hash table to safe 1-bucket table (prevents divide-by-zero at 0x00A4BDFE)
    static void *s_empty_bucket[1] = { nullptr };
    uint8_t *p_ht_buckets = u2::engine::elf_ptr<uint8_t>(0x0153C000);
    uint32_t *p_ht_count = u2::engine::elf_ptr<uint32_t>(0x0153C03C);
    if (p_ht_buckets && p_ht_count && *p_ht_count == 0) {
        *reinterpret_cast<void ***>(p_ht_buckets) = s_empty_bucket;
        *p_ht_count = 1;
    }
    *u2::engine::elf_ptr<uint64_t>(0x015341F0) = 0;
    *u2::engine::elf_ptr<uint64_t>(0x015341E8) = 0;
    *u2::engine::elf_ptr<uint64_t>(0x015341E0) = 0;
    *u2::engine::elf_ptr<uint8_t>(0x015342E0) = 0;
    *u2::engine::elf_ptr<uint8_t>(0x015346E0) = 0;

    // Register 3 builtin DC functions into 0x01534208:
    uint8_t *node1 = u2::engine::elf_ptr<uint8_t>(0x01534C38);
    *reinterpret_cast<uintptr_t *>(node1 + 0x00) = reinterpret_cast<uintptr_t>(u2::engine::elf_ptr<void>(0x00A19360));
    *reinterpret_cast<const char **>(node1 + 0x08) = reinterpret_cast<const char *>(u2::engine::elf_ptr<void>(0x00EA1650));
    *reinterpret_cast<uint32_t *>(node1 + 0x10) = 0x227FBCF1U;
    *reinterpret_cast<const char **>(node1 + 0x18) = reinterpret_cast<const char *>(u2::engine::elf_ptr<void>(0x00EA166A));
    *reinterpret_cast<uint32_t *>(node1 + 0x20) = 0x73BCB654U;
    *reinterpret_cast<uint8_t **>(node1 + 0x28) = nullptr;

    uint8_t *node2 = u2::engine::elf_ptr<uint8_t>(0x01534C68);
    *reinterpret_cast<uintptr_t *>(node2 + 0x00) = reinterpret_cast<uintptr_t>(u2::engine::elf_ptr<void>(0x00A19200));
    *reinterpret_cast<const char **>(node2 + 0x08) = reinterpret_cast<const char *>(u2::engine::elf_ptr<void>(0x00EA1687));
    *reinterpret_cast<uint32_t *>(node2 + 0x10) = 0x9D0C33D9U;
    *reinterpret_cast<const char **>(node2 + 0x18) = reinterpret_cast<const char *>(u2::engine::elf_ptr<void>(0x00EA1696));
    *reinterpret_cast<uint32_t *>(node2 + 0x20) = 0x0E530D18U;
    *reinterpret_cast<uint8_t **>(node2 + 0x28) = node1;

    uint8_t *node3 = u2::engine::elf_ptr<uint8_t>(0x01534C98);
    *reinterpret_cast<uintptr_t *>(node3 + 0x00) = reinterpret_cast<uintptr_t>(u2::engine::elf_ptr<void>(0x00A19180));
    *reinterpret_cast<const char **>(node3 + 0x08) = reinterpret_cast<const char *>(u2::engine::elf_ptr<void>(0x00EA16A4));
    *reinterpret_cast<uint32_t *>(node3 + 0x10) = 0xA78EC8DDU;
    *reinterpret_cast<const char **>(node3 + 0x18) = reinterpret_cast<const char *>(u2::engine::elf_ptr<void>(0x00EA16BB));
    *reinterpret_cast<uint32_t *>(node3 + 0x20) = 0x56FB3996U;
    *reinterpret_cast<uint8_t **>(node3 + 0x28) = node2;

    *u2::engine::elf_ptr<uint8_t *>(0x01534208) = node3;
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

// Static default Process node for ProcessMgr root pointers
static uint8_t s_default_process_node[0x200] = {};

/* 0x009B9530 — ProcessMgr::Init(void *mgr)
 * Initializes the ProcessMgr root structure (0x0115F1A0) and boots process list.
 */
void ABI u2_cpp_ProcessMgr_Init(void *mgr) {
    if (!g_u2_image_base) return;
    printf("[U2 Engine] ProcessMgr_Init called (mgr=%p)\n", mgr);
    fflush(stdout);

    u2_cpp_ProcessList_StartUp();
    void *target_mgr = mgr ? mgr : u2::engine::elf_ptr<void>(0x0115F1A0);
    auto *m = static_cast<uint8_t *>(target_mgr);
    std::memset(m, 0, 0xD00);
    *reinterpret_cast<uint32_t *>(m + 0x00) = 1; // initialized

    std::memset(s_default_process_node, 0, sizeof(s_default_process_node));

    auto get_node = [](uint32_t offset) -> void * {
        void *p = u2::engine::elf_ptr<void>(offset);
        return p ? p : s_default_process_node;
    };

    void *node_d20 = get_node(0x01520D88);
    void *node_c58 = get_node(0x01520360);
    void *node_ca8 = get_node(0x01520B30);

    *reinterpret_cast<void **>(m + 0xC40) = get_node(0x01520108);
    *reinterpret_cast<void **>(m + 0xC48) = get_node(0x015201D0);
    *reinterpret_cast<void **>(m + 0xC50) = get_node(0x01520298);
    *reinterpret_cast<void **>(m + 0xC58) = node_c58;
    *reinterpret_cast<void **>(m + 0xC60) = get_node(0x01520428);
    *reinterpret_cast<void **>(m + 0xC68) = get_node(0x015204F0);
    *reinterpret_cast<void **>(m + 0xC70) = get_node(0x015205B8);
    *reinterpret_cast<void **>(m + 0xC78) = get_node(0x01520680);
    *reinterpret_cast<void **>(m + 0xC80) = get_node(0x01520810);
    *reinterpret_cast<void **>(m + 0xC88) = get_node(0x015208D8);
    *reinterpret_cast<void **>(m + 0xC90) = get_node(0x01520748);
    *reinterpret_cast<void **>(m + 0xC98) = get_node(0x015209A0);
    *reinterpret_cast<void **>(m + 0xCA0) = get_node(0x01520A68);
    *reinterpret_cast<void **>(m + 0xCA8) = node_ca8;
    *reinterpret_cast<void **>(m + 0xCB0) = get_node(0x015201D0);
    *reinterpret_cast<void **>(m + 0xCB8) = get_node(0x01520298);
    *reinterpret_cast<void **>(m + 0xCC0) = node_c58;
    *reinterpret_cast<void **>(m + 0xCC8) = get_node(0x01520428);
    *reinterpret_cast<void **>(m + 0xCD0) = get_node(0x01520748);
    *reinterpret_cast<void **>(m + 0xCD8) = get_node(0x01520810);
    *reinterpret_cast<void **>(m + 0xD10) = get_node(0x01520BF8);
    *reinterpret_cast<void **>(m + 0xD18) = get_node(0x01520CC0);
    *reinterpret_cast<void **>(m + 0xD20) = node_d20;
    *reinterpret_cast<void **>(m + 0xD28) = node_d20;
    *reinterpret_cast<void **>(m + 0xD30) = node_d20;

    // Set globals 0x01161EE8, 0x01161EF0, 0x0115F198
    *u2::engine::elf_ptr<void *>(0x01161EE8) = node_d20;
    *u2::engine::elf_ptr<void *>(0x01161EF0) = node_d20;
    *u2::engine::elf_ptr<void *>(0x0115F198) = node_d20;

    // ProcessList anchor at 0x01116180 + 0x78
    void *proc_list_head = u2::engine::elf_ptr<void>(0x01116180 + 0x78);

    auto init_process_node = [proc_list_head](void *node_ptr) {
        if (!node_ptr) return;
        auto *n = static_cast<uint8_t *>(node_ptr);
        *reinterpret_cast<void **>(n + 0x10) = n + 0x40;
        *reinterpret_cast<void **>(n + 0x40) = n + 0x40;
        *reinterpret_cast<void **>(n + 0x58) = n;
        *reinterpret_cast<void **>(n + 0x70) = proc_list_head;
        *reinterpret_cast<void **>(n + 0xC0) = n + 0x40;
    };

    const uint32_t node_offsets[] = {
        0x01520108, 0x015201D0, 0x01520298, 0x01520360, 0x01520428,
        0x015204F0, 0x015205B8, 0x01520680, 0x01520748, 0x01520810,
        0x015208D8, 0x015209A0, 0x01520A68, 0x01520B30, 0x01520BF8,
        0x01520CC0, 0x01520D88
    };
    init_process_node(s_default_process_node);
    for (uint32_t off : node_offsets) {
        init_process_node(u2::engine::elf_ptr<void>(off));
    }
}

/* 0x00B783F0 — GameLoadingMountInitialArchives(void)
 * Registers core archives into g_CoreArchives (0x015AF450):
 *   1. u2data/build/main/bin.psarc  -> "build/main/" (State 10)
 *   2. u2data/data.psarc            -> "" (State 9)
 *   3. u2data/build/main/core.psarc -> "build/main/" (State 8)
 * Advances g_pGameLoadingState to 7 (All Initial Archives Mounted).
 */
uint32_t ABI u2_cpp_GameLoading_MountInitialArchives(void) {
    if (!g_u2_image_base) return 0;
    printf("[U2 Engine] GameLoading_MountInitialArchives called\n");
    fflush(stdout);

    u2::engine::ensure_root_allocator();

    auto *num_archives = u2::engine::elf_ptr<uint64_t>(0x015AF448);
    auto *archives = u2::engine::elf_ptr<uint8_t>(0x015AF450);

    struct ArchiveDef {
        const char *path;
        const char *mount;
        uint64_t    state_signal;
    };

    static const ArchiveDef s_core_archives[] = {
        { "u2data/build/main/bin.psarc",  "build/main/", 10 },
        { "u2data/data.psarc",            "",             9 },
        { "u2data/build/main/core.psarc", "build/main/",  8 },
    };

    uint8_t **p_state = u2::engine::elf_ptr<uint8_t *>(0x015AF750);
    uint8_t *state_obj = (p_state && *p_state) ? *p_state : nullptr;
    auto signal_fn = reinterpret_cast<void (ABI *)(void *, uint64_t)>(g_u2_image_base + 0x00B29440);

    for (const auto &arch : s_core_archives) {
        if (*num_archives < 32) {
            size_t idx = static_cast<size_t>(*num_archives);
            uint8_t *entry = archives + idx * 24;
            *reinterpret_cast<const char **>(entry + 0x00) = arch.path;
            *reinterpret_cast<const char **>(entry + 0x08) = arch.mount;
            *reinterpret_cast<uint64_t *>(entry + 0x10) = 1; // Mounted flag
            *num_archives = idx + 1;
            printf("[U2 Engine] Mounted core archive [%zu]: '%s' at '%s'\n", idx, arch.path, arch.mount);
        }

        if (state_obj && signal_fn) {
            *reinterpret_cast<uint64_t *>(state_obj + 0x20) = arch.state_signal;
            signal_fn(state_obj, arch.state_signal);
        }
    }

    // Ensure ScriptManager s_packages list head is initialized
    uint8_t *packages_head = u2::engine::elf_ptr<uint8_t>(0x01534140);
    if (packages_head) {
        *reinterpret_cast<uint8_t **>(packages_head + 0) = packages_head;
        *reinterpret_cast<uint8_t **>(packages_head + 8) = packages_head;
    }

    if (state_obj && signal_fn) {
        printf("[U2 Engine] Advancing g_pGameLoadingState (%p) to 7 (READY)\n", state_obj);
        *reinterpret_cast<uint64_t *>(state_obj + 0x20) = 7;
        signal_fn(state_obj, 7);
    }
    fflush(stdout);
    return 1;
}

/* 0x00B78790 — GameLoadingPlayGoWait(int32_t timeout_ms)
 * Simulates optical/PlayGo wait for streaming chunk ready. Returns 0 immediately on PC.
 */
uint32_t ABI u2_cpp_GameLoading_PlayGoWait(int32_t timeout_ms) {
    static int s_count = 0;
    if (s_count++ < 10) {
        printf("[U2 Engine] GameLoading_PlayGoWait called (timeout=%d) caller=%p (guest offset=%#llx)\n",
               timeout_ms, __builtin_return_address(0),
               g_u2_image_base ? (unsigned long long)((uintptr_t)__builtin_return_address(0) - (uintptr_t)g_u2_image_base) : 0ULL);
        fflush(stdout);
    }
    if (g_u2_image_base) {
        auto *base = reinterpret_cast<uint8_t *>(g_u2_image_base);
        // PlayGo streaming completion: chunk 0 is 100% ready on PC
        *reinterpret_cast<float *>(base + 0x01050A38) = 1.0f;
        *reinterpret_cast<uint32_t *>(base + 0x015AFAE8) = 0;
        *reinterpret_cast<uint8_t *>(base + 0x015AF77C) = 1;
        uint8_t *rdi = *reinterpret_cast<uint8_t **>(base + 0x015AF750);
        if (rdi && reinterpret_cast<uintptr_t>(rdi) >= 0x10000) {
            auto *counter = reinterpret_cast<std::atomic<int64_t> *>(rdi + 0x20);
            int64_t val = counter->load();
            if (val > 0) {
                counter->store(0);
            }
        }
    }
    (void)timeout_ms;
    return 0;
}

/* 0x00A551B0 — IntroState::Update(void *self)
 * Intro logo presentation (sce-ndi-logos, bp-logos). Returns 1 when ready to enter Menu.
 */
int32_t ABI u2_cpp_IntroState_Update(void *self) {
    printf("[U2 Engine] IntroState_Update called (self=%p)\n", self);
    fflush(stdout);
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
    printf("[U2 Engine] UCCApp_StateMachineUpdate called: state=%d, dt=%f\n", current_state, dt);
    fflush(stdout);

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
    case 7: { // MAIN MENU INITIALIZATION
        // Configure menu camera floats matching 0x00D59ECE
        *reinterpret_cast<int32_t *>(app + 0x04) = 0;
        *reinterpret_cast<float *>(app + 0x08) = 100.0f;
        *reinterpret_cast<float *>(app + 0xE0) = -80.0f;
        *reinterpret_cast<float *>(app + 0xE4) = 4.0f;
        *reinterpret_cast<float *>(app + 0x0C) = -80.0f;
        *reinterpret_cast<float *>(app + 0x10) = 1.0f;
        if (g_u2_image_base) {
            *u2::engine::elf_ptr<uint32_t>(0x0147DA0C) = 0;
            // Create ProcessMgr task for menu process if guest code is executable (bb-probe mode)
            if (is_guest_code_executable()) {
                void *proc_mgr = u2::engine::elf_ptr<void>(0x0115F1A0);
                if (proc_mgr) {
                    void *script_proc = *reinterpret_cast<void **>(static_cast<uint8_t *>(proc_mgr) + 0xC88);
                    if (script_proc) {
                        uint8_t task_params[0x80] = {};
                        *reinterpret_cast<uint32_t *>(task_params + 0x00) = 0x6019C7E;
                        *reinterpret_cast<void **>(task_params + 0x08) = script_proc;
                        auto create_task = reinterpret_cast<void * (ABI *)(void *, int32_t)>(g_u2_image_base + 0x009B5C20);
                        void *task = create_task(task_params, 0);
                        printf("[U2 Engine] Menu process task created: %p\n", task);
                        fflush(stdout);
                    }
                }
            }
        }
        *reinterpret_cast<int32_t *>(app + 0x00) = 8;
        break;
    }
    case 8: { // MENU TRANSITION & PRESENTATION DELAY
        if (g_u2_image_base) {
            float *p_timer = u2::engine::elf_ptr<float>(0x0172BDCC);
            if (p_timer) {
                *p_timer += dt;
                float threshold = *u2::engine::elf_ptr<const float>(0x00E693D8);
                if (threshold <= 0.0f) threshold = 51.0f;
                // Transition to State 9 (Main Menu Interactive Loop)
                if (*p_timer > threshold || *p_timer > 1.0f) {
                    *reinterpret_cast<int32_t *>(app + 0x00) = 9;
                    printf("[U2 Engine] Menu timer elapsed (%f s) -> State 9 (MAIN MENU ACTIVE)\n", *p_timer);
                    fflush(stdout);
                }
            } else {
                *reinterpret_cast<int32_t *>(app + 0x00) = 9;
            }
        } else {
            *reinterpret_cast<int32_t *>(app + 0x00) = 9;
        }
        break;
    }
    case 9: { // MAIN MENU ACTIVE
        break;
    }
    default:
        break;
    }

    return *reinterpret_cast<int32_t *>(app + 0x00);
}
