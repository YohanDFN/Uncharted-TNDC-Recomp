// Uncharted 2: Among Thieves — Audio Subsystems & Driver
// Clean C++23 Reimplementation (ndlib/audio/)
#pragma once

#ifndef ABI
#if defined(_WIN32)
#define ABI __attribute__((sysv_abi))
#else
#define ABI
#endif
#endif

extern "C" {
void ABI u2_cpp_AudioFile_Initialize(void *self);
}
