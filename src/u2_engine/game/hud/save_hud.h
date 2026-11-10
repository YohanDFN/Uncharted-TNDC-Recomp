// Uncharted 2: Among Thieves — SaveHUD Subsystem
// Clean C++23 Reimplementation (game/hud/)
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
void         ABI u2_cpp_Hud_ReadConfigFile(void *self);
void         ABI u2_cpp_SaveBuffer_InitExternal(void *self, uint32_t magic, uint32_t size, void *buf);
void         ABI u2_cpp_SaveBuffer_DebugSetBuffer(void *self, void *buf);
void         ABI u2_cpp_SaveBuffer_CopyFrom(void *self, const void *src);
void         ABI u2_cpp_SaveBuffer_WriteBytes(void *self, const void *data, uint32_t size);
void         ABI u2_cpp_SaveBuffer_WriteU64Count0(void *self, const uint64_t *val);
void         ABI u2_cpp_SaveBuffer_WriteU64Count1(void *self, const uint64_t *val);
void         ABI u2_cpp_Hud_FreeGrenadeIndicator(void *self, uint32_t idx);
void         ABI u2_cpp_Hud_ShowGrenadeIndicator(void *self, uint32_t idx, uint8_t show);
int32_t      ABI u2_cpp_GameTask_cbCollectTaskNode(void *task, uint32_t key, void *collection);
void         ABI u2_cpp_Settings_FreeSetting(void *setting);
uint8_t      ABI u2_cpp_GameLoading_IsChunkSetEntirelyWithin(const char *chunk_set_name, const uint64_t *target_mask);
uint32_t     ABI u2_cpp_GameLoading_GetPlayGoSpeed(void);
}
