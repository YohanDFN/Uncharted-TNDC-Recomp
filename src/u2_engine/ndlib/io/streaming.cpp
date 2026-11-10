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
