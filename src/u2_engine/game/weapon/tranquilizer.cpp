// Uncharted 2: Among Thieves — Tranquilizer Weapon Subsystem
// Reimplementation of 0x009E0CC0 (game/weapon/tranquilizer.cpp) with safe NULL checks.

#include "u2_engine.h"
#include <cstdio>
#include <cstdint>
#include <cstring>

extern "C" {

/* 0x009E0CC0: TranquilizerSystem::Init()
 * Iterates through 5 weapon effect entries at 0x015296F0 (spaced by 0x48 bytes).
 * Looks up symbol at +0x24 (0xCD0FBE5A).
 * Resolves particle/texture strings if symbol exists, and binds variables via 0x00A1CD50.
 */
void ABI u2_cpp_Tranquilizer_Init() {
    if (!g_u2_image_base) return;

    uint8_t *table = u2::engine::elf_ptr<uint8_t>(0x015296F0);
    if (!table) return;

    typedef void * (ABI *LookupSymbolFn)(uint32_t hash);
    auto lookup_fn = u2::engine::elf_fn<LookupSymbolFn>(0x00A15E70);

    typedef uint64_t (ABI *ConvertNameFn)(const char *str, int32_t unused);
    auto convert_fn = u2::engine::elf_fn<ConvertNameFn>(0x00555140);

    typedef void * (ABI *BindVarFn)(uint32_t id, void *out_handle, int32_t *val_ptr, uint32_t count, void *r8, void *r9, float xmm0, uint64_t stack_arg);
    auto bind_fn = u2::engine::elf_fn<BindVarFn>(0x00A1CD50);

    void *r14_arg = u2::engine::elf_ptr<void>(0x00A1D290);
    void *r9_arg  = u2::engine::elf_ptr<void>(0x00A1D2C0);
    float float_val = *u2::engine::elf_ptr<float>(0x00E6715C);

    for (uint32_t i = 0; i < 5; ++i) {
        uint8_t *entry = table + (i * 0x48);
        uint32_t sym_hash = *reinterpret_cast<uint32_t *>(entry + 0x24);

        void *sym = lookup_fn ? lookup_fn(sym_hash) : nullptr;
        if (sym) {
            uint8_t *sym_ptr = reinterpret_cast<uint8_t *>(sym);

            // 1. Field at +0x48
            void **f48 = *reinterpret_cast<void ***>(sym_ptr + 0x48);
            if (f48 && *f48 && convert_fn) {
                const char *s = reinterpret_cast<const char *>(*f48);
                uint64_t res = convert_fn(s, 0);
                *reinterpret_cast<uint64_t *>(entry + 0x00) = res;
            }

            // 2. Field at +0x50
            void **f50 = *reinterpret_cast<void ***>(sym_ptr + 0x50);
            if (f50 && *f50 && convert_fn) {
                const char *s = reinterpret_cast<const char *>(*f50);
                uint64_t res = convert_fn(s, 0);
                *reinterpret_cast<uint64_t *>(entry + 0x0C) = res;
            }

            // 3. Field at +0x60
            void **f60 = *reinterpret_cast<void ***>(sym_ptr + 0x60);
            if (f60 && *f60 && convert_fn) {
                const char *s = reinterpret_cast<const char *>(*f60);
                uint64_t res = convert_fn(s, 0);
                *reinterpret_cast<uint64_t *>(entry + 0x18) = res;
            }
        }

        // Variable 1: +0x38
        if (bind_fn) {
            int32_t val1 = 0;
            void *handle1 = bind_fn(0x309861E2u, entry + 0x38, &val1, 4, r14_arg, r9_arg, float_val, 0);
            if (handle1) {
                *reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t *>(handle1) + 0x20) = 0;
                uint8_t *flags = reinterpret_cast<uint8_t *>(handle1) + 0x61;
                *flags = (*flags & ~0x0B) | 0x08;
            }

            // Variable 2: +0x3C
            int32_t val2 = 0;
            void *handle2 = bind_fn(0x309861E2u, entry + 0x3C, &val2, 4, r14_arg, r9_arg, float_val, 0);
            if (handle2) {
                *reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t *>(handle2) + 0x20) = 0;
                uint8_t *flags = reinterpret_cast<uint8_t *>(handle2) + 0x61;
                *flags = (*flags & ~0x0B) | 0x08;
            }

            // Variable 3: +0x40
            int32_t val3 = 0;
            void *handle3 = bind_fn(0x309861E2u, entry + 0x40, &val3, 4, r14_arg, r9_arg, float_val, 0);
            if (handle3) {
                *reinterpret_cast<uint32_t *>(reinterpret_cast<uint8_t *>(handle3) + 0x20) = 0;
                uint8_t *flags = reinterpret_cast<uint8_t *>(handle3) + 0x61;
                *flags = (*flags & ~0x0B) | 0x08;
            }
        }
    }
}

} // extern "C"
