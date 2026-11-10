// Uncharted 2: Among Thieves — Tranquilizer Weapon System
// Clean C++23 Reimplementation (game/weapon/)
#pragma once

#ifndef ABI
#if defined(_WIN32)
#define ABI __attribute__((sysv_abi))
#else
#define ABI
#endif
#endif

extern "C" {
void ABI u2_cpp_Tranquilizer_Init(void);
}
