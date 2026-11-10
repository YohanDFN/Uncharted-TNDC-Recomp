// Uncharted 2: Among Thieves — Boot State Machine & Game Loading
// Clean C++23 Reimplementation (game/loading/)
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
void     ABI u2_cpp_Settings_Init(void);
void *   ABI u2_cpp_Settings_NewSetting(void *self, uint64_t key, const char *name, uint32_t type, void *parent);
void     ABI u2_cpp_ProcessList_StartUp(void);
void     ABI u2_cpp_ProcessMgr_Init(void *mgr);
uint32_t ABI u2_cpp_GameLoading_MountInitialArchives(void);
uint32_t ABI u2_cpp_GameLoading_PlayGoWait(int32_t timeout_ms);
int32_t  ABI u2_cpp_IntroState_Update(void *self);
int32_t  ABI u2_cpp_UCCApp_StateMachineUpdate(void *self, float dt);
}
