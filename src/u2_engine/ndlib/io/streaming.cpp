// Uncharted 2: Among Thieves — File Loading Subsystems
// Pure C++23 Reimplementation (Zero PS4 Code Execution)
// ADR-006: Dual-Executable Architecture.

#include "u2_engine.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

struct ShaderFileData {
    void    *buffer{nullptr};
    uint64_t size{0};
};

/* 0x00CED020 — FileLoader::LoadFileHelper(void *file_data_out, const char *path)
 * Loads a file into memory allocated from the active thread AllocStack (or HostAllocator).
 * Returns true if successfully loaded, false otherwise.
 * Populates file_data_out:
 *   +0x00: void *buffer
 *   +0x08: uint64_t size
 */
extern "C" bool ABI u2_cpp_LoadFileHelper(void *file_data_out, const char *path) {
    if (!file_data_out) return false;

    auto *data = static_cast<ShaderFileData *>(file_data_out);
    data->buffer = nullptr;
    data->size = 0;

    if (!path || !*path) return false;

    // 1. Try runtime_file_open first (translates guest path e.g. /app0/...)
    int64_t fd = runtime_file_open(path, 0, 0);
    FILE *fp = nullptr;
    int64_t file_size = -1;

    if (fd >= 0) {
        file_size = runtime_file_lseek(static_cast<int>(fd), 0, 2 /* SEEK_END */);
        runtime_file_lseek(static_cast<int>(fd), 0, 0 /* SEEK_SET */);
    } else {
        // Fallback: translate guest path to host path, or open directly
        char translated[1024];
        if (runtime_file_translate(path, translated, sizeof(translated)) == 0) {
            fp = std::fopen(translated, "rb");
        }
        if (!fp) {
            if (std::strncmp(path, "/app0/", 6) == 0) {
                fp = std::fopen(path + 6, "rb");
            }
        }
        if (!fp) {
            fp = std::fopen(path, "rb");
        }
        if (fp) {
            std::fseek(fp, 0, SEEK_END);
            file_size = std::ftell(fp);
            std::fseek(fp, 0, SEEK_SET);
        }
    }

    if (fd < 0 && !fp) {
        return false;
    }

    if (file_size < 0) file_size = 0;

    void *buf = u2::engine::allocate_from_current_or_host(static_cast<size_t>(file_size), 16, "UNKNOWN");
    if (!buf) {
        if (fd >= 0) runtime_file_close(static_cast<int>(fd));
        if (fp) std::fclose(fp);
        return false;
    }

    if (file_size > 0) {
        if (fd >= 0) {
            int64_t read_bytes = runtime_file_pread(static_cast<int>(fd), buf, static_cast<uint64_t>(file_size), 0);
            if (read_bytes < file_size) {
                runtime_file_lseek(static_cast<int>(fd), 0, 0);
                runtime_file_read(static_cast<int>(fd), buf, static_cast<uint64_t>(file_size));
            }
        } else if (fp) {
            size_t read_bytes = std::fread(buf, 1, static_cast<size_t>(file_size), fp);
            (void)read_bytes;
        }
    }

    if (fd >= 0) runtime_file_close(static_cast<int>(fd));
    if (fp) std::fclose(fp);

    data->buffer = buf;
    data->size = static_cast<uint64_t>(file_size);

    printf("[U2 FileLoader] Loaded '%s' (%lld bytes, buf=%p)\n",
           path, static_cast<long long>(file_size), buf);
    fflush(stdout);

    return true;
}

/* 0x00B70270 — Streaming::ResolvePath(const char *original_path, char *resolved_out)
 * Normalizes paths for PC VFS:
 * - If original_path starts with "/host/Z:/" or "/host/", strips the host prefix.
 * - If path doesn't start with "/", prepends "/app0/".
 * - If path is relative like "u2data/..." or "build/main/...", prepends "/app0/".
 * - Ensures Windows backslashes are normalized to forward slashes.
 */
extern "C" void ABI u2_cpp_Streaming_ResolvePath(const char *original_path, char *resolved_out) {
    if (!resolved_out) return;
    resolved_out[0] = '\0';
    if (!original_path || !*original_path) return;

    const char *p = original_path;
    // Strip "/host/Z:/" or "/host/z:/" or "/host/"
    if (std::strncmp(p, "/host/Z:/", 9) == 0 || std::strncmp(p, "/host/z:/", 9) == 0) {
        p += 9;
    } else if (std::strncmp(p, "/host/", 6) == 0) {
        p += 6;
    }

    // Skip any leading drive letter e.g. "Z:/"
    if (p[0] && p[1] == ':' && (p[2] == '/' || p[2] == '\\')) {
        p += 3;
    }

    char clean_path[1024];
    size_t len = 0;
    while (*p && len < sizeof(clean_path) - 1) {
        clean_path[len++] = (*p == '\\') ? '/' : *p;
        p++;
    }
    clean_path[len] = '\0';

    if (clean_path[0] == '/') {
        // Already absolute guest path e.g. /app0/...
        std::snprintf(resolved_out, 1024, "%s", clean_path);
    } else if (std::strncmp(clean_path, "u2data/", 7) == 0) {
        std::snprintf(resolved_out, 1024, "/app0/%s", clean_path);
    } else if (std::strncmp(clean_path, "build/main/", 11) == 0) {
        std::snprintf(resolved_out, 1024, "/app0/u2data/%s", clean_path);
    } else {
        std::snprintf(resolved_out, 1024, "/app0/u2data/build/main/%s", clean_path);
    }
}
