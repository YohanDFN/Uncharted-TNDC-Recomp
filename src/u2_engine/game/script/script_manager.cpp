// Uncharted 2: Among Thieves — ScriptManager & DC Script Preloader
// Clean C++23 Reimplementation of big2-ps4/src/game/script-manager.cpp
#include "u2_engine.h"
#include "game/script/script_manager.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <memory>
#include <unordered_map>

#ifdef _WIN32
#include <windows.h>
#endif

// Forward declaration from runtime
extern "C" int runtime_file_translate(const char *guest, char *out, size_t size);

namespace u2::engine {

static std::unordered_map<uint32_t, void *> s_dc_symbol_map;
static std::vector<std::unique_ptr<std::vector<uint8_t>>> s_dc_buffers;
static bool s_dc_loaded = false;

void dc_script_loader_init() {
    if (s_dc_loaded) return;
    s_dc_loaded = true;

    char host_dir[1024] = {};
    if (runtime_file_translate("/app0/u2data/build/main/dc1/ps4", host_dir, sizeof(host_dir)) != 0) {
        std::snprintf(host_dir, sizeof(host_dir), "u2data/build/main/dc1/ps4");
    }

#ifdef _WIN32
    char search_pattern[1024];
    std::snprintf(search_pattern, sizeof(search_pattern), "%s/*.bin", host_dir);
    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(search_pattern, &fd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                char full_path[1024];
                std::snprintf(full_path, sizeof(full_path), "%s/%s", host_dir, fd.cFileName);
                FILE *fp = std::fopen(full_path, "rb");
                if (fp) {
                    std::fseek(fp, 0, SEEK_END);
                    long sz = std::ftell(fp);
                    std::fseek(fp, 0, SEEK_SET);
                    if (sz > 0x28) {
                        auto pbuf = std::make_unique<std::vector<uint8_t>>(static_cast<size_t>(sz));
                        if (std::fread(pbuf->data(), 1, pbuf->size(), fp) == pbuf->size()) {
                            const uint8_t *data = pbuf->data();
                            // Validate '00CD' magic
                            if (data[0] == '0' && data[1] == '0' && data[2] == 'C' && data[3] == 'D') {
                                uint8_t *raw_buf = pbuf->data();
                                uintptr_t base_addr = reinterpret_cast<uintptr_t>(raw_buf);

                                // 1. Apply Naughty Dog DC 64-bit pointer relocation bitmap (0x00A19A27)
                                uint64_t reloc_off = *reinterpret_cast<const uint64_t *>(raw_buf + 0x08);
                                if (reloc_off + 4 <= pbuf->size()) {
                                    uint32_t num_bitmap_bytes = *reinterpret_cast<const uint32_t *>(raw_buf + reloc_off);
                                    const uint8_t *bitmap = raw_buf + reloc_off + 4;
                                    if (reloc_off + 4 + num_bitmap_bytes <= pbuf->size()) {
                                        uint64_t total_qwords = static_cast<uint64_t>(num_bitmap_bytes) * 8;
                                        uint64_t max_qwords = reloc_off / 8;
                                        if (total_qwords > max_qwords) total_qwords = max_qwords;

                                        for (uint64_t qw = 0; qw < total_qwords; ++qw) {
                                            uint32_t byte_idx = static_cast<uint32_t>(qw / 8);
                                            uint32_t bit_idx  = static_cast<uint32_t>(qw % 8);
                                            if ((bitmap[byte_idx] & (1u << bit_idx)) != 0) {
                                                *reinterpret_cast<uint64_t *>(raw_buf + qw * 8) += base_addr;
                                            }
                                        }
                                    }
                                }

                                // 2. Ingest symbol table (offset +0x10 was relocated into direct memory pointer)
                                const uint8_t *symtab = *reinterpret_cast<const uint8_t **>(raw_buf + 0x10);
                                if (symtab >= raw_buf && symtab + 8 <= raw_buf + pbuf->size()) {
                                    uint32_t num_syms = *reinterpret_cast<const uint32_t *>(symtab + 4);
                                    for (uint32_t i = 0; i < num_syms; ++i) {
                                        const uint8_t *entry = symtab + 8 + i * 16;
                                        if (entry + 16 > raw_buf + pbuf->size()) break;
                                        uint32_t sym_hash = *reinterpret_cast<const uint32_t *>(entry);
                                        void *sym_ptr = *reinterpret_cast<void * const *>(entry + 8);
                                        if (sym_ptr >= raw_buf && sym_ptr < raw_buf + pbuf->size()) {
                                            s_dc_symbol_map[sym_hash] = sym_ptr;
                                        }
                                    }
                                }
                            }
                        }
                        s_dc_buffers.push_back(std::move(pbuf));
                    }
                    std::fclose(fp);
                }
            }
        } while (FindNextFileA(hFind, &fd));
        FindClose(hFind);
    }
#endif

    std::printf("[U2 DC ScriptLoader] Ingested %zu DC symbols across %zu .bin files from '%s'\n",
                s_dc_symbol_map.size(), s_dc_buffers.size(), host_dir);
    std::fflush(stdout);
}

void *dc_lookup_symbol(uint32_t symbol_hash) {
    if (!s_dc_loaded) {
        dc_script_loader_init();
    }
    auto it = s_dc_symbol_map.find(symbol_hash);
    if (it != s_dc_symbol_map.end()) {
        return it->second;
    }
    return nullptr;
}

void *dc_lookup_symbol_in_module(uint32_t symbol_hash, uint32_t module_hash) {
    (void)module_hash;
    return dc_lookup_symbol(symbol_hash);
}

} // namespace u2::engine

extern "C" {

static void *get_safe_fallback() {
    static uint8_t *s_fallback = nullptr;
    if (!s_fallback) {
#ifdef _WIN32
        s_fallback = reinterpret_cast<uint8_t *>(
            VirtualAlloc(nullptr, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)
        );
#else
        s_fallback = reinterpret_cast<uint8_t *>(std::malloc(4096));
#endif
        if (s_fallback) {
            // Fill with SysV ret stubs: xor eax, eax; xorps xmm0, xmm0; ret (31 c0 0f 57 c0 c3)
            for (size_t i = 0; i < 4096; i += 16) {
                s_fallback[i + 0] = 0x31; // xor eax, eax
                s_fallback[i + 1] = 0xc0;
                s_fallback[i + 2] = 0x0f; // xorps xmm0, xmm0
                s_fallback[i + 3] = 0x57;
                s_fallback[i + 4] = 0xc0;
                s_fallback[i + 5] = 0xc3; // ret
            }
        }
    }
    return s_fallback;
}

/* 0x00A15E70 — ScriptManager::LookupPointer(uint32_t symbol_hash)
 * Primary symbol resolver used across all Naughty Dog engine gameplay subsystems.
 * If symbol is not yet loaded in guest ELF, we resolve directly from our preloaded DC binary scripts.
 * If completely absent, returns an executable safe buffer to prevent NULL dereferences and execution faults.
 */
void * ABI u2_cpp_ScriptManager_LookupPointer(uint32_t symbol_hash) {
    void *sym = u2::engine::dc_lookup_symbol(symbol_hash);
    if (sym) {
        return sym;
    }

    std::printf("[U2 ScriptManager] LookupPointer: MISSING symbol hash 0x%08X\n", symbol_hash);
    std::fflush(stdout);

    return get_safe_fallback();
}

/* 0x00A17280 — ScriptManager::LookupPointerInModule(uint32_t symbol_hash, uint32_t module_hash)
 * Module-scoped symbol resolver.
 */
void * ABI u2_cpp_ScriptManager_LookupPointerInModule(uint32_t symbol_hash, uint32_t module_hash) {
    void *sym = u2::engine::dc_lookup_symbol_in_module(symbol_hash, module_hash);
    if (sym) {
        return sym;
    }

    std::printf("[U2 ScriptManager] LookupPointerInModule: MISSING symbol 0x%08X in module 0x%08X\n",
                symbol_hash, module_hash);
    std::fflush(stdout);

    return get_safe_fallback();
}

} // extern "C"
