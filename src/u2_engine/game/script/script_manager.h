// Uncharted 2: Among Thieves — ScriptManager & DC Script Preloader
// Clean C++23 Reimplementation (game/script/)
#pragma once

#include <cstdint>
#include <cstddef>

#ifndef ABI
#if defined(_WIN32)
#define ABI __attribute__((sysv_abi))
#else
#define ABI
#endif
#endif

namespace u2::engine {

void  dc_script_loader_init();
void *dc_lookup_symbol(uint32_t symbol_hash);
void *dc_lookup_symbol_in_module(uint32_t symbol_hash, uint32_t module_hash);

} // namespace u2::engine

extern "C" {
void * ABI u2_cpp_ScriptManager_LookupPointer(uint32_t symbol_hash);
void * ABI u2_cpp_ScriptManager_LookupPointerInModule(uint32_t symbol_hash, uint32_t module_hash);
}
