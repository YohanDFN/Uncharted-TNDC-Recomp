// Uncharted 2: Among Thieves — Native C++ Standalone Executable
// Pure C++23 Reimplementation (Zero PS4 Code Execution)
// Entry point for uncharted2-native.exe (ADR-006).

#include "engine_context.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>

extern "C" void runtime_restart(void) {
    const char *restart_cmd = std::getenv("BB_RESTART_COMMAND");
    if (restart_cmd && *restart_cmd) {
        STARTUPINFOA si = {};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi = {};
        if (CreateProcessA(nullptr, const_cast<char *>(restart_cmd), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            _exit(0);
        }
    }
    _exit(0);
}

int main(int argc, char **argv) {
    std::string game_dir;
    std::string elf_path;
    int timeout_seconds = 0;
    int display_index = -1;
    bool headless = false;

    // Check environment variables
    const char *env_game = std::getenv("BB_GAME_DIR");
    if (env_game && *env_game) game_dir = env_game;

    const char *env_timeout = std::getenv("BB_TIMEOUT");
    if (env_timeout && *env_timeout) timeout_seconds = std::atoi(env_timeout);

    const char *env_display = std::getenv("BB_DISPLAY");
    if (env_display && *env_display) display_index = std::atoi(env_display);

    const char *env_headless = std::getenv("BB_HEADLESS");
    if (env_headless && *env_headless && *env_headless != '0') headless = true;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--game" || arg == "--app0") {
            if (i + 1 < argc) game_dir = argv[++i];
        } else if (arg == "--elf") {
            if (i + 1 < argc) elf_path = argv[++i];
        } else if (arg == "--timeout") {
            if (i + 1 < argc) timeout_seconds = std::atoi(argv[++i]);
        } else if (arg == "--display" || arg == "-d" || arg == "--screen") {
            if (i + 1 < argc) display_index = std::atoi(argv[++i]);
        } else if (arg == "--headless") {
            headless = true;
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: uncharted2-native.exe [options]\n";
            std::cout << "  --game <path>     Path to CUSA02343 game folder\n";
            std::cout << "  --elf <path>      Path to eboot.elf/big2-ps4_Shipping.elf\n";
            std::cout << "  --display <idx>   Target monitor index (0=primary, 1=second screen)\n";
            std::cout << "  --timeout <sec>   Run for N seconds then exit cleanly\n";
            std::cout << "  --headless        Run without interactive window/GPU\n";
            return 0;
        }
    }

    if (game_dir.empty()) {
        game_dir = "C:/Users/Yohan/Desktop/Uncharted/CUSA02343";
    }

    u2::standalone::EngineContext engine;
    if (!engine.initialize(game_dir, elf_path, headless, display_index)) {
        u2::standalone::native_log("[U2 Native] Failed to initialize engine context.\n");
        return 1;
    }

    // Run engine unit tests to verify native subsystems
    u2::standalone::native_log("[U2 Native] Validating C++23 engine subsystems...\n");
    int test_failures = u2_run_engine_unit_tests(engine.data_buffer());
    if (test_failures != 0) {
        u2::standalone::native_log("[U2 Native] WARNING: " + std::to_string(test_failures) + " assertions failed in unit tests.\n");
    } else {
        u2::standalone::native_log("[U2 Native] All C++23 native subsystem assertions verified successfully.\n");
    }

    u2::standalone::native_log("[U2 Native] Engine running. (Press ESC or close window to exit)\n");
    engine.run_loop(timeout_seconds);

    u2::standalone::native_log("[U2 Native] Engine shutdown complete. Goodbye!\n");
    return 0;
}
