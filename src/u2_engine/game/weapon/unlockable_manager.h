// Uncharted 2: Among Thieves — UnlockableManager Subsystem
// Clean C++23 Reimplementation (game/weapon/)
#pragma once

#include <cstdint>

#ifndef ABI
#if defined(_WIN32)
#define ABI __attribute__((sysv_abi))
#else
#define ABI
#endif
#endif

extern "C" {
void ABI u2_cpp_UnlockableManager_SetDefaultWeapon(void *self, uint8_t flag);
}
