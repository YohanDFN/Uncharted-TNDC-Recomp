// Uncharted 2: Among Thieves — AudioFile & Engine Audio Subsystems
// Pure C++23 Reimplementation (Zero PS4 Code Execution)
// ADR-006: Dual-Executable Architecture.

#include "u2_engine.h"
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstdio>

/* 0x003CE4A0 — AudioFile::Initialize(void *self)
 * Initializes the AudioFile singleton (0x01111FD0) with three memory buffers:
 * - Buffer 1: 0x21000 bytes (135,168), 128-byte aligned at +0x08
 * - Buffer 2: 0x0E000 bytes (57,344), 128-byte aligned at +0x28
 * - Buffer 3: 16 pointer slots (128 bytes) at +0x50, each pointing to a 1024-byte slot buffer
 * Marks the AudioFile as initialized (self[0] = 1).
 */
void ABI u2_cpp_AudioFile_Initialize(void *self) {
    if (!self) return;
    auto *a = static_cast<uint8_t *>(self);
    if (a[0] != 0) return; // Already initialized

    *reinterpret_cast<uint32_t *>(a + 0x04) = 0;

    // Buffer 1: 0x21000 bytes (135,168), align 0x80
    void *buf1 = u2::engine::allocate_from_current_or_host(0x21000, 0x80, "AudioFile_Buf1");
    if (buf1) std::memset(buf1, 0, 0x21000);
    *reinterpret_cast<void **>(a + 0x08) = buf1;
    *reinterpret_cast<uint64_t *>(a + 0x10) = 0;
    *reinterpret_cast<uint64_t *>(a + 0x18) = 0;
    *reinterpret_cast<uint32_t *>(a + 0x20) = 0;

    // Buffer 2: 0xE000 bytes (57,344), align 0x80
    void *buf2 = u2::engine::allocate_from_current_or_host(0xE000, 0x80, "AudioFile_Buf2");
    if (buf2) std::memset(buf2, 0, 0xE000);
    *reinterpret_cast<void **>(a + 0x28) = buf2;
    *reinterpret_cast<uint64_t *>(a + 0x30) = 0;
    *reinterpret_cast<uint64_t *>(a + 0x38) = 0;
    *reinterpret_cast<uint64_t *>(a + 0x40) = 0;
    *reinterpret_cast<uint64_t *>(a + 0x48) = 0;

    // Buffer 3: 16 pointer slots (0x80 bytes, align 0x10)
    auto **slots = static_cast<void **>(u2::engine::allocate_from_current_or_host(0x80, 0x10, "AudioFile_Slots"));
    *reinterpret_cast<void **>(a + 0x50) = slots;
    if (slots) {
        std::memset(slots, 0, 0x80);
        for (size_t i = 0; i < 16; ++i) {
            void *slot_buf = u2::engine::allocate_from_current_or_host(0x400, 0x10, "AudioFile_SlotBuf");
            if (slot_buf) std::memset(slot_buf, 0, 0x400);
            slots[i] = slot_buf;
        }
    }

    a[0] = 1; // Mark initialized!
    printf("[U2 Engine] AudioFile_Initialize: success (self=%p, buf1=%p, buf2=%p, slots=%p)\n",
           self, buf1, buf2, slots);
    fflush(stdout);
}
