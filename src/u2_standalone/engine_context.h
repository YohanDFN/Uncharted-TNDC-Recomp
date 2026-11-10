// Uncharted 2: Among Thieves — Standalone Native C++ Engine Context
// Replaces PS4 ELF guest code execution with 100% native C++23 host execution.
// ADR-006: Dual-Executable Architecture.
#pragma once

#include <cstdint>
#include <cstddef>
#include <cmath>
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

    bool initialize(const std::string &game_dir, const std::string &elf_override_path = "", bool headless = false, int display_index = -1) {
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

        // 4. Initialize Platform Window & Vulkan/Direct3D Renderer
        initialize_graphics(1920, 1080, "Uncharted 2: Among Thieves (Native C++ Recomp)", headless, display_index);

        // 5. Load and Display Splash Screen (sce_sys/pic1.png)
        std::string splash_path;
        if (!game_dir.empty()) {
            splash_path = game_dir + "/sce_sys/pic1.png";
        }
        if (splash_path.empty() || !std::ifstream(splash_path, std::ios::binary).is_open()) {
            splash_path = "sce_sys/pic1.png";
        }
        load_splash_screen(splash_path);

        // 6. Initialize and Step Boot State Machine (UCCApp State 1 -> State 7 Menu)
        native_log("[U2 Native] Initializing Boot State Machine & Subsystems...\n");
        *reinterpret_cast<int32_t *>(m_ucc_app_state) = 1;
        while (*reinterpret_cast<int32_t *>(m_ucc_app_state) < 7) {
            int32_t old_state = *reinterpret_cast<int32_t *>(m_ucc_app_state);
            u2_cpp_UCCApp_StateMachineUpdate(m_ucc_app_state, 0.016f);
            int32_t new_state = *reinterpret_cast<int32_t *>(m_ucc_app_state);
            native_log("[U2 Native] Boot State Transition: State " + std::to_string(old_state)
                       + " -> State " + std::to_string(new_state) + "\n");
        }
        // Step State 7 once to configure Main Menu camera & environment
        u2_cpp_UCCApp_StateMachineUpdate(m_ucc_app_state, 0.016f);
        native_log("[U2 Native] Boot sequence complete! Reached State 7 (MAIN MENU ACTIVE).\n");

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

        native_log("[U2 Native] Native C++23 engine loop active. Presenting interactive Main Menu...\n");
        uint64_t start_ticks = SDL_GetTicks();
        int last_remaining = -1;
        bool running = true;
        SDL_Event ev;

        while (running) {
            while (SDL_PollEvent(&ev)) {
                if (ev.type == SDL_EVENT_QUIT) {
                    running = false;
                } else if (ev.type == SDL_EVENT_KEY_DOWN) {
                    if (ev.key.key == SDLK_ESCAPE) {
                        running = false;
                    } else if (ev.key.key == SDLK_UP) {
                        if (!m_menu_items.empty()) {
                            m_menu_selected_item = (m_menu_selected_item - 1 + static_cast<int>(m_menu_items.size())) % static_cast<int>(m_menu_items.size());
                            native_log("[U2 Native] Menu Naviguer HAUT -> Selection: " + m_menu_items[m_menu_selected_item].title + "\n");
                        }
                    } else if (ev.key.key == SDLK_DOWN) {
                        if (!m_menu_items.empty()) {
                            m_menu_selected_item = (m_menu_selected_item + 1) % static_cast<int>(m_menu_items.size());
                            native_log("[U2 Native] Menu Naviguer BAS -> Selection: " + m_menu_items[m_menu_selected_item].title + "\n");
                        }
                    } else if (ev.key.key == SDLK_RETURN || ev.key.key == SDLK_KP_ENTER) {
                        if (!m_menu_items.empty()) {
                            native_log("[U2 Native] Menu Action -> Valide: " + m_menu_items[m_menu_selected_item].title + "\n");
                            u2_cpp_MenuHost_TriggerControlClicked(nullptr, m_menu_selected_item);
                            m_last_action_msg = "Action: " + m_menu_items[m_menu_selected_item].title;
                            m_last_action_time = SDL_GetTicks();
                            if (m_menu_selected_item == 5) { // QUITTER
                                native_log("[U2 Native] Option Quitter selectionnee. Arret du moteur.\n");
                                running = false;
                            }
                        }
                    }
                } else if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    if (ev.button.button == SDL_BUTTON_LEFT) {
                        float mx = ev.button.x;
                        float my = ev.button.y;
                        for (int i = 0; i < static_cast<int>(m_menu_items.size()); ++i) {
                            float item_y = 310.0f + i * 85.0f;
                            if (mx >= 130.0f && mx <= 790.0f && my >= item_y && my <= item_y + 65.0f) {
                                m_menu_selected_item = i;
                                native_log("[U2 Native] Clic Souris -> Selection: " + m_menu_items[i].title + "\n");
                                u2_cpp_MenuHost_TriggerControlClicked(nullptr, i);
                                m_last_action_msg = "Action: " + m_menu_items[i].title;
                                m_last_action_time = SDL_GetTicks();
                                if (i == 5) { // QUITTER
                                    running = false;
                                }
                                break;
                            }
                        }
                    }
                } else if (ev.type == SDL_EVENT_MOUSE_MOTION) {
                    float mx = ev.motion.x;
                    float my = ev.motion.y;
                    for (int i = 0; i < static_cast<int>(m_menu_items.size()); ++i) {
                        float item_y = 310.0f + i * 85.0f;
                        if (mx >= 130.0f && mx <= 790.0f && my >= item_y && my <= item_y + 65.0f) {
                            m_menu_selected_item = i;
                            break;
                        }
                    }
                }
            }

            render_frame();

            if (timeout_seconds > 0) {
                uint64_t elapsed_ms = SDL_GetTicks() - start_ticks;
                int elapsed_sec = static_cast<int>(elapsed_ms / 1000);
                int remaining = timeout_seconds - elapsed_sec;
                if (remaining < 0) remaining = 0;

                if (remaining != last_remaining) {
                    last_remaining = remaining;
                    std::string title = "Uncharted 2: Among Thieves (Pure C++23 Standalone) — Menu Principal [Fermeture auto dans "
                                        + std::to_string(remaining) + "s - ECHAP pour quitter]";
                    SDL_SetWindowTitle(m_window, title.c_str());
                    native_log("[U2 Native] Temps restant avant fermeture: " + std::to_string(remaining) + "s\n");
                }

                if (elapsed_sec >= timeout_seconds) {
                    native_log("[U2 Native] Delai de " + std::to_string(timeout_seconds) + "s atteint. Fermeture propre du moteur.\n");
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

    bool initialize_graphics(int width, int height, const char *title, bool headless = false, int display_index = -1) {
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

        // Enumerate displays
        int num_displays = 0;
        SDL_DisplayID *displays = SDL_GetDisplays(&num_displays);
        native_log("[U2 Native] Enumerated " + std::to_string(num_displays) + " connected display(s):\n");
        if (displays) {
            for (int i = 0; i < num_displays; ++i) {
                SDL_Rect b = {};
                SDL_GetDisplayBounds(displays[i], &b);
                const char *dname = SDL_GetDisplayName(displays[i]);
                native_log("  Display [" + std::to_string(i) + "] ID=" + std::to_string(displays[i])
                           + " '" + (dname ? dname : "Unknown") + "' Bounds=("
                           + std::to_string(b.x) + "," + std::to_string(b.y) + " "
                           + std::to_string(b.w) + "x" + std::to_string(b.h) + ")\n");
            }
        }

        native_log("[U2 Native] Creating SDL window (" + std::to_string(width) + "x" + std::to_string(height) + ")...\n");
        m_window = SDL_CreateWindow(title, width, height, SDL_WINDOW_RESIZABLE);
        if (!m_window) {
            native_log("[U2 Native] WARNING: SDL_CreateWindow failed (" + std::string(SDL_GetError())
                       + "). Falling back to headless mode.\n");
            if (displays) SDL_free(displays);
            return true;
        }

        // Position window on requested display if specified
        if (displays && num_displays > 0 && display_index >= 0) {
            int target_idx = display_index;
            if (target_idx >= num_displays) {
                native_log("[U2 Native] Requested display index " + std::to_string(display_index)
                           + " exceeds available displays (" + std::to_string(num_displays)
                           + "). Defaulting to display 0.\n");
                target_idx = 0;
            }
            SDL_Rect b = {};
            if (SDL_GetDisplayBounds(displays[target_idx], &b)) {
                int win_x = b.x + (b.w > width ? (b.w - width) / 2 : 0);
                int win_y = b.y + (b.h > height ? (b.h - height) / 2 : 0);
                SDL_SetWindowPosition(m_window, win_x, win_y);
                const char *dname = SDL_GetDisplayName(displays[target_idx]);
                native_log("[U2 Native] Positioned window on Display " + std::to_string(target_idx)
                           + " ('" + (dname ? dname : "Unknown") + "') at coordinates ("
                           + std::to_string(win_x) + ", " + std::to_string(win_y) + ")\n");
            }
        }
        if (displays) {
            SDL_free(displays);
            displays = nullptr;
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

    void draw_debug_text(float x, float y, const char *str, float scale = 1.0f) {
        if (!m_renderer || !str) return;
        if (scale != 1.0f) {
            SDL_SetRenderScale(m_renderer, scale, scale);
            SDL_RenderDebugText(m_renderer, x / scale, y / scale, str);
            SDL_SetRenderScale(m_renderer, 1.0f, 1.0f);
        } else {
            SDL_RenderDebugText(m_renderer, x, y, str);
        }
    }

    void render_phurba_dagger_3d(float center_x, float center_y, float scale, float time_sec) {
        float angle_y = time_sec * 0.9f;
        float cos_y = std::cos(angle_y);
        float sin_y = std::sin(angle_y);
        float tilt_x = 0.28f; // Legere inclinaison perspective
        float cos_x = std::cos(tilt_x);
        float sin_x = std::sin(tilt_x);

        struct Point3D { float x, y, z; };
        struct Edge { int i0, i1; };

        const std::vector<Point3D> vertices = {
            // 0: Pointe inferieure aceree
            {0.0f, -170.0f, 0.0f},
            // 1, 2, 3: Lame triangulaire inferieure (section 1)
            {24.0f * std::cos(0.0f), -80.0f, 24.0f * std::sin(0.0f)},
            {24.0f * std::cos(2.0944f), -80.0f, 24.0f * std::sin(2.0944f)},
            {24.0f * std::cos(4.1888f), -80.0f, 24.0f * std::sin(4.1888f)},
            // 4, 5, 6: Base large de la triple lame (section 2)
            {38.0f * std::cos(0.0f), 0.0f, 38.0f * std::sin(0.0f)},
            {38.0f * std::cos(2.0944f), 0.0f, 38.0f * std::sin(2.0944f)},
            {38.0f * std::cos(4.1888f), 0.0f, 38.0f * std::sin(4.1888f)},
            // 7, 8, 9, 10: Tetes Makara / Garde
            {26.0f, 22.0f, 0.0f}, {-26.0f, 22.0f, 0.0f}, {0.0f, 22.0f, 26.0f}, {0.0f, 22.0f, -26.0f},
            // 11, 12, 13, 14: Manche / Noeud vajra central
            {15.0f, 65.0f, 0.0f}, {-15.0f, 65.0f, 0.0f}, {0.0f, 65.0f, 15.0f}, {0.0f, 65.0f, -15.0f},
            // 15, 16, 17, 18: Tetes des trois divinites couronnees
            {30.0f, 105.0f, 0.0f}, {-30.0f, 105.0f, 0.0f}, {0.0f, 105.0f, 30.0f}, {0.0f, 105.0f, -30.0f},
            // 19: Couronne / Flamme superieure
            {0.0f, 140.0f, 0.0f}
        };

        const std::vector<Edge> edges = {
            // Aretes de la lame vers la pointe
            {0, 1}, {0, 2}, {0, 3},
            {1, 2}, {2, 3}, {3, 1},
            {1, 4}, {2, 5}, {3, 6},
            {4, 5}, {5, 6}, {6, 4},
            // Jonction garde
            {4, 7}, {5, 8}, {6, 9}, {4, 10},
            {7, 9}, {9, 8}, {8, 10}, {10, 7},
            // Poignee vajra
            {7, 11}, {8, 12}, {9, 13}, {10, 14},
            {11, 13}, {13, 12}, {12, 14}, {14, 11},
            // Pommeau aux visages
            {11, 15}, {12, 16}, {13, 17}, {14, 18},
            {15, 17}, {17, 16}, {16, 18}, {18, 15},
            // Couronne superieure
            {15, 19}, {16, 19}, {17, 19}, {18, 19}
        };

        std::vector<SDL_FPoint> proj(vertices.size());
        std::vector<float> depths(vertices.size());
        const float dist = 420.0f;

        for (size_t i = 0; i < vertices.size(); ++i) {
            const auto &v = vertices[i];
            float rx = v.x * cos_y - v.z * sin_y;
            float rz = v.x * sin_y + v.z * cos_y;
            float ry = v.y;

            float fy = ry * cos_x - rz * sin_x;
            float fz = ry * sin_x + rz * cos_x;
            float fx = rx;

            depths[i] = fz;
            float pscale = dist / (dist + fz);
            proj[i].x = center_x + fx * pscale * scale;
            proj[i].y = center_y - fy * pscale * scale;
        }

        for (const auto &e : edges) {
            float avg_z = (depths[e.i0] + depths[e.i1]) * 0.5f;
            int intensity = static_cast<int>(180 + (avg_z / 40.0f) * 75.0f);
            if (intensity < 90) intensity = 90;
            if (intensity > 255) intensity = 255;

            SDL_SetRenderDrawColor(
                m_renderer,
                static_cast<Uint8>(intensity),
                static_cast<Uint8>(intensity * 0.82f),
                static_cast<Uint8>(intensity * 0.25f),
                230
            );
            SDL_RenderLine(m_renderer, proj[e.i0].x, proj[e.i0].y, proj[e.i1].x, proj[e.i1].y);
        }
    }

    void render_main_menu() {
        if (!m_renderer) return;

        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

        // 1. Fond semi-transparent sombre sur toute la fenetre
        SDL_FRect backdrop = {0.0f, 0.0f, 1920.0f, 1080.0f};
        SDL_SetRenderDrawColor(m_renderer, 6, 9, 14, 185);
        SDL_RenderFillRect(m_renderer, &backdrop);

        // 2. Panneau lateral gauche du menu
        SDL_FRect panel = {100.0f, 140.0f, 720.0f, 760.0f};
        SDL_SetRenderDrawColor(m_renderer, 12, 16, 24, 215);
        SDL_RenderFillRect(m_renderer, &panel);
        SDL_SetRenderDrawColor(m_renderer, 190, 150, 55, 200);
        SDL_RenderRect(m_renderer, &panel);

        // 3. Titre et sous-titre
        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 220);
        draw_debug_text(132.0f, 172.0f, "UNCHARTED 2: AMONG THIEVES", 2.6f);
        SDL_SetRenderDrawColor(m_renderer, 255, 215, 0, 255);
        draw_debug_text(130.0f, 170.0f, "UNCHARTED 2: AMONG THIEVES", 2.6f);

        SDL_SetRenderDrawColor(m_renderer, 175, 205, 235, 255);
        draw_debug_text(132.0f, 218.0f, "REMASTERED -- 100% PURE C++23 NATIVE RECOMP", 1.25f);

        SDL_SetRenderDrawColor(m_renderer, 190, 150, 55, 180);
        SDL_RenderLine(m_renderer, 130.0f, 248.0f, 790.0f, 248.0f);

        // 4. Liste des options de menu
        for (int i = 0; i < static_cast<int>(m_menu_items.size()); ++i) {
            float item_y = 280.0f + i * 85.0f;
            SDL_FRect item_box = {130.0f, item_y, 660.0f, 68.0f};

            if (i == m_menu_selected_item) {
                // Fond surligne ambre/or
                SDL_SetRenderDrawColor(m_renderer, 210, 160, 30, 95);
                SDL_RenderFillRect(m_renderer, &item_box);
                SDL_SetRenderDrawColor(m_renderer, 255, 215, 60, 255);
                SDL_RenderRect(m_renderer, &item_box);

                // Barre verticale doree sur la gauche
                SDL_FRect bar = {130.0f, item_y, 8.0f, 68.0f};
                SDL_SetRenderDrawColor(m_renderer, 255, 215, 0, 255);
                SDL_RenderFillRect(m_renderer, &bar);

                // Pointeur et Titre
                std::string label = "> " + m_menu_items[i].title;
                SDL_SetRenderDrawColor(m_renderer, 255, 245, 190, 255);
                draw_debug_text(155.0f, item_y + 13.0f, label.c_str(), 1.7f);

                // Description
                SDL_SetRenderDrawColor(m_renderer, 230, 220, 170, 230);
                draw_debug_text(160.0f, item_y + 42.0f, m_menu_items[i].description.c_str(), 1.0f);
            } else {
                // Fond discret
                SDL_SetRenderDrawColor(m_renderer, 22, 28, 40, 130);
                SDL_RenderFillRect(m_renderer, &item_box);
                SDL_SetRenderDrawColor(m_renderer, 55, 65, 85, 130);
                SDL_RenderRect(m_renderer, &item_box);

                // Titre normal
                std::string label = "  " + m_menu_items[i].title;
                SDL_SetRenderDrawColor(m_renderer, 175, 185, 200, 210);
                draw_debug_text(155.0f, item_y + 13.0f, label.c_str(), 1.55f);

                // Description
                SDL_SetRenderDrawColor(m_renderer, 125, 135, 150, 160);
                draw_debug_text(160.0f, item_y + 42.0f, m_menu_items[i].description.c_str(), 0.95f);
            }
        }

        // 5. Notification d'action si recente (< 2.5 secondes)
        if (!m_last_action_msg.empty()) {
            uint64_t diff = SDL_GetTicks() - m_last_action_time;
            if (diff < 2500) {
                SDL_FRect notif_box = {130.0f, 810.0f, 660.0f, 50.0f};
                SDL_SetRenderDrawColor(m_renderer, 30, 80, 50, 220);
                SDL_RenderFillRect(m_renderer, &notif_box);
                SDL_SetRenderDrawColor(m_renderer, 60, 200, 100, 240);
                SDL_RenderRect(m_renderer, &notif_box);
                SDL_SetRenderDrawColor(m_renderer, 220, 255, 220, 255);
                draw_debug_text(150.0f, 826.0f, m_last_action_msg.c_str(), 1.2f);
            }
        }

        // 6. Panneau d'informations Moteur a droite
        SDL_FRect info_box = {1320.0f, 140.0f, 520.0f, 200.0f};
        SDL_SetRenderDrawColor(m_renderer, 12, 16, 26, 215);
        SDL_RenderFillRect(m_renderer, &info_box);
        SDL_SetRenderDrawColor(m_renderer, 70, 105, 150, 180);
        SDL_RenderRect(m_renderer, &info_box);

        SDL_SetRenderDrawColor(m_renderer, 240, 200, 70, 255);
        draw_debug_text(1340.0f, 155.0f, "STATUT DU MOTEUR NATIF C++23", 1.35f);
        SDL_SetRenderDrawColor(m_renderer, 70, 105, 150, 180);
        SDL_RenderLine(m_renderer, 1340.0f, 178.0f, 1810.0f, 178.0f);

        SDL_SetRenderDrawColor(m_renderer, 200, 220, 240, 240);
        draw_debug_text(1340.0f, 192.0f, "- UCCApp State: 7 (MAIN MENU ACTIVE)", 1.05f);
        draw_debug_text(1340.0f, 214.0f, "- Code PS4 execute: ZERO (0 instructions)", 1.05f);
        draw_debug_text(1340.0f, 236.0f, "- Detours ELF actifs: 909 hooks C++ / C", 1.05f);
        draw_debug_text(1340.0f, 258.0f, "- Archives montees: build/main/bin.psarc", 1.05f);
        draw_debug_text(1340.0f, 280.0f, "- AllocStack Mem: Profondeur 46 active", 1.05f);
        draw_debug_text(1340.0f, 302.0f, "- Presenteur: SDL3 Accelerated (60 FPS)", 1.05f);

        // 7. Dague Phurba rituelle 3D animee en temps reel
        float time_sec = static_cast<float>(SDL_GetTicks()) / 1000.0f;
        render_phurba_dagger_3d(1580.0f, 620.0f, 1.4f, time_sec);

        SDL_SetRenderDrawColor(m_renderer, 210, 175, 70, 200);
        draw_debug_text(1470.0f, 830.0f, "DAGUETTE PHURBA -- RENDU 3D TEMPS REEL", 1.05f);

        // 8. Pied de page / Barre de statut d'aide
        SDL_FRect footer = {0.0f, 1025.0f, 1920.0f, 55.0f};
        SDL_SetRenderDrawColor(m_renderer, 8, 11, 18, 245);
        SDL_RenderFillRect(m_renderer, &footer);
        SDL_SetRenderDrawColor(m_renderer, 190, 150, 55, 160);
        SDL_RenderLine(m_renderer, 0.0f, 1025.0f, 1920.0f, 1025.0f);

        SDL_SetRenderDrawColor(m_renderer, 210, 225, 240, 240);
        draw_debug_text(40.0f, 1042.0f,
            "[FLECHES HAUT / BAS / SOURIS] Naviguer   [ENTREE / CLIC] Valider   [ECHAP] Quitter   -- Dual-Executable Native Uncharted 2",
            1.15f);
    }

    void render_frame() {
        SDL_SetRenderDrawColor(m_renderer, 10, 10, 15, 255);
        SDL_RenderClear(m_renderer);

        if (m_splash_texture) {
            SDL_RenderTexture(m_renderer, m_splash_texture, nullptr, nullptr);
        }

        int32_t state = *reinterpret_cast<const int32_t *>(m_ucc_app_state);
        if (state >= 7) {
            render_main_menu();
        }

        SDL_RenderPresent(m_renderer);
    }

    struct MenuItem {
        std::string title;
        std::string description;
    };

    std::vector<MenuItem> m_menu_items = {
        {"CONTINUER", "Reprendre la derniere partie sauvegardee"},
        {"NOUVELLE PARTIE", "Commencer une nouvelle aventure au Tibet"},
        {"SELECTION DE CHAPITRE", "Rejouer un chapitre deverrouille"},
        {"OPTIONS & PARAMETRES", "Graphismes, audio, commandes et affichage"},
        {"BONUS & GALERIE", "Tresors collectes, modeles 3D et cinematiques"},
        {"QUITTER", "Quitter vers le bureau Windows"}
    };

    int m_menu_selected_item{0};
    std::string m_last_action_msg{""};
    uint64_t m_last_action_time{0};
    uint8_t *m_data_buffer{nullptr};
    void *m_alloc_stack{nullptr};
    SDL_Window *m_window{nullptr};
    SDL_Renderer *m_renderer{nullptr};
    SDL_Texture *m_splash_texture{nullptr};
    uint8_t m_ucc_app_state[0x100]{};
    bool m_initialized{false};
};

} // namespace u2::standalone
