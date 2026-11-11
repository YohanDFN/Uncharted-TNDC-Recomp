#include "../src/runtime_u2_detour_filter.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    const char *base = "ScriptManager_LookupPointer";
    const char *module = "ScriptManager_LookupPointerInModule";
    assert(u2_detour_name_allowed(NULL, base));
    assert(u2_detour_name_allowed("", base));
    assert(u2_detour_name_allowed(module, module));
    assert(!u2_detour_name_allowed(module, base));
    assert(!u2_detour_name_allowed(base, module));
    assert(u2_detour_name_allowed("A,ScriptManager_LookupPointer,Z", base));
    assert(u2_detour_name_allowed("ScriptManager_LookupPointer,Z", base));
    assert(u2_detour_name_allowed("A,ScriptManager_LookupPointer", base));
    assert(!u2_detour_name_allowed("A,ScriptManager_LookupPointerInModule,Z", base));
    assert(!u2_detour_name_allowed("PrefixScriptManager_LookupPointer", base));
    assert(!u2_detour_name_allowed(",,ScriptManager_LookupPointerInModule,,", base));
    assert(u2_detour_name_allowed(",,ScriptManager_LookupPointer,,", base));
    puts("U2 detour filter: PASSED (12 assertions)");
    return 0;
}
