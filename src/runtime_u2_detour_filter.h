#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* BB_U2_DETOUR_ALLOW is a comma-separated list of complete registry names. */
static inline bool u2_detour_name_allowed(const char *allow, const char *name) {
    if (!allow || !*allow) return true;
    const size_t name_len = strlen(name);
    for (const char *token = allow; *token;) {
        const size_t token_len = strcspn(token, ",");
        if (token_len == name_len && memcmp(token, name, name_len) == 0) return true;
        token += token_len;
        if (*token) ++token;
    }
    return false;
}
