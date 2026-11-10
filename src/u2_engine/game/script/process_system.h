// Uncharted 2: Among Thieves — Process & Coroutine System
// Clean C++23 Reimplementation (game/script/)
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
void *   ABI u2_cpp_ProcessMgr_LookupProcessByPid(void *mgr, uint32_t pid);
void *   ABI u2_cpp_Process_LookupProcessByPid(void *root, uint32_t pid);
void     ABI u2_cpp_Process_ValidateInternalPointer(void *self);
void     ABI u2_cpp_ProcessMgr_AddProcessForScriptUpdate(void *mgr, void *proc);
void *   ABI u2_cpp_ProcessList_Alloc(void *proc);
uint8_t  ABI u2_cpp_Process_DMenuStateProp(void *self, int32_t cmd);
float    ABI u2_cpp_Process_DMenuFloatProp_354(void *self, int32_t cmd, float val);
uint8_t  ABI u2_cpp_Process_DMenuBoolProp_43A(void *self, int32_t cmd);
uint8_t  ABI u2_cpp_Process_DMenuBoolProp_43F(void *self, int32_t cmd);
uint8_t  ABI u2_cpp_Process_DMenuBoolProp_439(void *self, int32_t cmd);
uint8_t  ABI u2_cpp_Process_DMenuBoolProp_438(void *self, int32_t cmd);
uint8_t  ABI u2_cpp_Process_DMenuBoolProp_43B(void *self, int32_t cmd);
}
