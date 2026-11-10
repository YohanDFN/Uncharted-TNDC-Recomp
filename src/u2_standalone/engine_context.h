// Uncharted 2: Among Thieves — Standalone Native C++ Engine Context
// Replaces PS4 ELF guest code execution with 100% native C++23 host execution.
// ADR-006: Dual-Executable Architecture.
#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <SDL3/SDL.h>
#include "../src/runtime.h"
#include "../src/u2_engine/u2_engine.h"
#include "png_loader.h"

namespace u2::standalone {

inline void native_log(const std::string &msg) {
    std::printf("%s", msg.c_str());
    std::fflush(stdout);
    FILE *log_f = std::fopen("user/native_run.log", "a");
    if (log_f) {
        std::fputs(msg.c_str(), log_f);
        std::fflush(log_f);
        std::fclose(log_f);
    }
}

constexpr size_t kEngineDataBufferSize = 24 * 1024 * 1024; // 24 MiB data & rodata buffer

class EngineContext {
public:
    EngineContext() = default;
    ~EngineContext() {
        shutdown();
    }

    bool initialize(const std::string &game_dir, const std::string &elf_override_path = "", bool headless = false) {
        native_log("============================================================\n");
        native_log("[U2 Native] Uncharted 2: Among Thieves — Native C++23 Recomp\n");
        native_log("[U2 Native] Starting Standalone Engine (Zero PS4 Code Execution)\n");
        native_log("============================================================\n");

        // 1. Allocate non-executable guest data buffer (PAGE_READWRITE only)
        if (!allocate_memory_buffer()) {
            native_log("[U2 Native] FATAL: Failed to allocate 24 MiB non-executable data buffer.\n");
            return false;
        }

        // 2. Load .rodata & static tables from ELF into memory
        std::string elf_path = elf_override_path;
        if (elf_path.empty()) {
            elf_path = "out/big2/eboot.elf";
            std::ifstream test_f(elf_path, std::ios::binary);
            if (!test_f.is_open() && !game_dir.empty()) {
                elf_path = game_dir + "/big2-ps4_Shipping.elf";
            }
        }
        load_elf_data(elf_path);

        // 2b. Install native detours into the data buffer for trampoline calling
        runtime_u2_install_detours(m_data_buffer, kEngineDataBufferSize);

        // 3. Initialize Memory Subsystem (C++23 AllocStack depth 46)
        m_alloc_stack = u2_cpp_AllocateAllocStack();
        if (!m_alloc_stack) {
            native_log("[U2 Native] FATAL: Failed to initialize AllocStack.\n");
            return false;
        }
        native_log("[U2 Native] Memory Subsystem initialized (AllocStack depth 46, thread-local active)\n");

        // 4. Initialize Platform Window & Vulkan Renderer
        initialize_graphics(1920, 1080, "Uncharted 2: Among Thieves (Native C++ Recomp)", headless);

        // 5. Load and Display Splash Screen (sce_sys/pic1.png)
        std::string splash_path;
        if (!game_dir.empty()) {
            splash_path = game_dir + "/sce_sys/pic1.png";
        }
        if (splash_path.empty() || !std::ifstream(splash_path, std::ios::binary).is_open()) {
            splash_path = "sce_sys/pic1.png";
        }
        load_splash_screen(splash_path);

        m_initialized = true;
        return true;
    }

    void run_loop(int timeout_seconds = 0) {
        if (!m_window || !m_renderer) {
            native_log("[U2 Native] Headless loop running...\n");
            if (timeout_seconds > 0) {
                SDL_Delay(timeout_seconds * 1000);
            }
            return;
        }

        native_log("[U2 Native] Native C++23 engine loop active. Presenting splash screen...\n");
        uint64_t start_ticks = SDL_GetTicks();
        bool running = true;
        SDL_Event ev;

        while (running) {
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_EVENT_QUIT) {
                    running = false;
                } else if (ev.type == SDL_EVENT_KEY_DOWN) {
                    if (ev.key.key == SDLK_ESCAPE) {
                        running = false;
                    }
                }
            }

            render_frame();

            if (timeout_seconds > 0) {
                uint64_t elapsed = (SDL_GetTicks() - start_ticks) / 1000;
                if (elapsed >= static_cast<uint64_t>(timeout_seconds)) {
                    native_log("[U2 Native] Reached timeout of " + std::to_string(timeout_seconds) + "s. Terminating cleanly.\n");
                    break;
                }
            }

            SDL_Delay(16); // ~60 FPS presentation
        }
    }

    void shutdown() {
        if (m_splash_texture) {
            SDL_DestroyTexture(m_splash_texture);
            m_splash_texture = nullptr;
        }
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        SDL_Quit();

        if (m_data_buffer) {
            VirtualFree(m_data_buffer, 0, MEM_RELEASE);
            m_data_buffer = nullptr;
            g_u2_image_base = nullptr;
        }
        m_initialized = false;
    }

    uint8_t *data_buffer() const noexcept {
        return m_data_buffer;
    }

private:
    bool allocate_memory_buffer() {
        // Strict Security Constraint: PAGE_READWRITE only, NEVER PAGE_EXECUTE_*
        m_data_buffer = static_cast<uint8_t *>(VirtualAlloc(
            nullptr, kEngineDataBufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE
        ));
        if (!m_data_buffer) return false;

        std::memset(m_data_buffer, 0, kEngineDataBufferSize);
        g_u2_image_base = m_data_buffer;
        native_log("[U2 Native] Engine Context initialized (24 MiB non-executable data buffer, 0 guest code pages)\n");
        return true;
    }

    struct Elf64Phdr {
        uint32_t p_type;
        uint32_t p_flags;
        uint64_t p_offset;
        uint64_t p_vaddr;
        uint64_t p_paddr;
        uint64_t p_filesz;
        uint64_t p_memsz;
        uint64_t p_align;
    };
    static_assert(sizeof(Elf64Phdr) == 56, "Elf64Phdr size must be 56 bytes");

    void load_elf_data(const std::string &path) {
        native_log("[U2 Native] Loading ELF data from: " + path + "\n");
        FILE *f = std::fopen(path.c_str(), "rb");
        if (!f) {
            native_log("[U2 Native] Note: ELF file not found at '" + path + "'. Initialized clean empty data space.\n");
            return;
        }

        // Parse ELF64 header
        uint8_t hdr[64] = {};
        if (std::fread(hdr, 1, 64, f) != 64 || hdr[0] != 0x7F || hdr[1] != 'E' || hdr[2] != 'L' || hdr[3] != 'F') {
            native_log("[U2 Native] Note: File is not a valid ELF binary.\n");
            std::fclose(f);
            return;
        }

        uint64_t phoff = *reinterpret_cast<const uint64_t *>(hdr + 0x20);
        uint16_t phentsize = *reinterpret_cast<const uint16_t *>(hdr + 0x36);
        uint16_t phnum = *reinterpret_cast<const uint16_t *>(hdr + 0x38);

        size_t loaded_segments = 0;
        size_t loaded_bytes = 0;

        for (uint16_t i = 0; i < phnum; ++i) {
            std::fseek(f, static_cast<long>(phoff + i * phentsize), SEEK_SET);
            Elf64Phdr phdr = {};
            if (std::fread(&phdr, sizeof(phdr), 1, f) != 1) break;

            // PT_LOAD = 1
            if (phdr.p_type == 1 && phdr.p_vaddr < kEngineDataBufferSize) {
                size_t copy_sz = static_cast<size_t>(phdr.p_filesz);
                if (phdr.p_vaddr + copy_sz > kEngineDataBufferSize) {
                    copy_sz = kEngineDataBufferSize - phdr.p_vaddr;
                }
                std::fseek(f, static_cast<long>(phdr.p_offset), SEEK_SET);
                size_t nread = std::fread(m_data_buffer + phdr.p_vaddr, 1, copy_sz, f);
                loaded_bytes += nread;
                ++loaded_segments;
            }
        }
        std::fclose(f);

        native_log("[U2 Native] Loaded " + std::to_string(loaded_segments) + " data/rodata segments ("
                   + std::to_string(loaded_bytes / 1024) + " KiB) from '" + path + "'\n");
    }

    bool initialize_graphics(int width, int height, const char *title, bool headless = false) {
        if (headless) {
            native_log("[U2 Native] Headless mode active: window and surface creation skipped.\n");
            return true;
        }

        const char *env_headless = std::getenv("BB_HEADLESS");
        if (env_headless && *env_headless && *env_headless != '0') {
            native_log("[U2 Native] BB_HEADLESS active: window and surface creation skipped.\n");
            return true;
        }

        native_log("[U2 Native] Initializing SDL video subsystem...\n");
        if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            native_log("[U2 Native] WARNING: SDL_InitSubSystem failed (" + std::string(SDL_GetError())
                       + "). Falling back to headless mode.\n");
            return true;
        }

        native_log("[U2 Native] Creating SDL window (" + std::to_string(width) + "x" + std::to_string(height) + ")...\n");
        m_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
        if (!m_window) {
            native_log("[U2 Native] WARNING: SDL_CreateWindow failed (" + std::string(SDL_GetError())
                       + "). Falling back to headless mode.\n");
            return true;
        }

        native_log("[U2 Native] Creating accelerated renderer...\n");
        m_renderer = SDL_CreateRenderer(m_window, nullptr);
        if (!m_renderer) {
            native_log("[U2 Native] WARNING: SDL_CreateRenderer failed (" + std::string(SDL_GetError())
                       + "). Falling back to headless mode.\n");
            return true;
        }

        native_log("[U2 Native] Graphics & Window initialized (" + std::to_string(width) + "x"
                   + std::to_string(height) + ", Presenter: " + std::string(SDL_GetRendererName(m_renderer)) + ")\n");
        return true;
    }

    void load_splash_screen(const std::string &path) {
        if (!m_renderer) return;

        DecodedImage img;
        if (load_png(path, img)) {
            m_splash_texture = SDL_CreateTexture(
                m_renderer,
                SDL_PIXELFORMAT_RGBA32,
                SDL_TEXTUREACCESS_STATIC,
                img.width,
                img.height
            );
            if (m_splash_texture) {
                SDL_UpdateTexture(m_splash_texture, nullptr, img.pixels.data(), img.width * 4);
                native_log("[U2 Native] Loaded Splash Screen '" + path + "' ("
                           + std::to_string(img.width) + "x" + std::to_string(img.height) + ")\n");
                return;
            }
        }

        native_log("[U2 Native] Note: Splash screen '" + path + "' not found. Using procedural fallback splash.\n");
        create_procedural_splash(1920, 1080);
    }

    void create_procedural_splash(int width, int height) {
        m_splash_texture = SDL_CreateTexture(
            m_renderer,
            SDL_PIXELFORMAT_RGBA32,
            SDL_TEXTUREACCESS_STATIC,
            width,
            height
        );
        if (!m_splash_texture) return;

        std::vector<uint8_t> pixels(static_cast<size_t>(width) * height * 4, 0);
        for (int y = 0; y < height; ++y) {
            uint8_t gradient = static_cast<uint8_t>(20 + (y * 40 / height));
            for (int x = 0; x < width; ++x) {
                size_t idx = (static_cast<size_t>(y) * width + x) * 4;
                pixels[idx + 0] = 12;            // R
                pixels[idx + 1] = gradient;      // G
                pixels[idx + 2] = gradient + 20; // B
                pixels[idx + 3] = 255;           // A
            }
        }
        SDL_UpdateTexture(m_splash_texture, nullptr, pixels.data(), width * 4);
    }

    void render_frame() {
        SDL_SetRenderDrawColor(m_renderer, 10, 10, 15, 255);
        SDL_RenderClear(m_renderer);

        if (m_splash_texture) {
            SDL_RenderTexture(m_renderer, m_splash_texture, nullptr, nullptr);
        }

        SDL_RenderPresent(m_renderer);
    }

    uint8_t *m_data_buffer{nullptr};
    void *m_alloc_stack{nullptr};
    SDL_Window *m_window{nullptr};
    SDL_Renderer *m_renderer{nullptr};
    SDL_Texture *m_splash_texture{nullptr};
    bool m_initialized{false};
};

} // namespace u2::standalone
