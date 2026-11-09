#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
"""Automated Quick Runner for Uncharted PC Recomp.

Runs the game with launcher settings for a fixed duration (default 10s),
captures live stdout/stderr, updates user/last_run.log, and summarizes results.
Usage:
    python scratch/auto_test.py [timeout_seconds]
"""

import os
import sys
import json
import time
import shutil
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / 'launcher'))
sys.path.insert(0, str(ROOT / 'scripts'))

from uncharted_launcher import game_environment, CONFIG_FILE, load_json, APP_DEFAULTS

def main():
    duration = int(sys.argv[1]) if len(sys.argv) > 1 else 10
    try:
        subprocess.run(["taskkill", "/F", "/IM", "bb-probe.exe"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, timeout=2)
    except Exception:
        pass
    gpu_exe = ROOT / "out" / "gpu" / "bb-probe.exe"
    out_exe = ROOT / "out" / "bb-probe.exe"
    if gpu_exe.is_file():
        try:
            shutil.copy2(gpu_exe, out_exe)
        except OSError as e:
            print(f"[AutoTest] Warning: could not sync {gpu_exe} -> {out_exe}: {e}")
    print(f"[AutoTest] Loading launcher settings from {CONFIG_FILE} ...")
    settings = load_json(CONFIG_FILE, APP_DEFAULTS)
    
    # Force windowed mode for headless/test runs if desired, or keep launcher settings
    env = game_environment(settings)
    env.setdefault("BB_U2_SELFTEST", "1")
    for k, v in os.environ.items():
        if k.startswith("BB_U2_"):
            env[k] = v
    
    # Ensure MSYS2 Clang64 is in PATH for DLLs
    clang64_bin = Path(r"C:\msys64\clang64\bin")
    if clang64_bin.is_dir() and str(clang64_bin) not in env.get("PATH", ""):
        env["PATH"] = f"{clang64_bin};{env.get('PATH', '')}"
        
    user_dir = ROOT / 'user'
    user_dir.mkdir(parents=True, exist_ok=True)
    log_file = user_dir / 'last_run.log'
    
    print(f"[AutoTest] Launching Uncharted ({env.get('BB_EXE', 'u2')}) for {duration} seconds...")
    print(f"[AutoTest] Game dir: {env.get('BB_GAME_DIR')}")
    print(f"[AutoTest] Log output: {log_file}")
    
    cmd = [sys.executable, str(ROOT / 'run.py')]
    
    with open(log_file, 'wb') as lf:
        proc = subprocess.Popen(
            cmd,
            cwd=str(ROOT),
            env=env,
            stdin=subprocess.DEVNULL,
            stdout=lf,
            stderr=subprocess.STDOUT
        )
        
        start_time = time.time()
        crashed = False
        
        while True:
            elapsed = time.time() - start_time
            if proc.poll() is not None:
                crashed = True
                break
            if elapsed >= duration:
                print(f"[AutoTest] Reached timeout of {duration}s without crash. Terminating test run cleanly...")
                subprocess.run(["taskkill", "/F", "/T", "/PID", str(proc.pid)], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                try:
                    proc.wait(timeout=3)
                except subprocess.TimeoutExpired:
                    proc.kill()
                break
            time.sleep(0.1)

    lines = log_file.read_text(encoding='utf-8', errors='replace').splitlines()
    for decoded in lines:
        if any(k in decoded for k in ("State Transition", "[U2 Loop]", "[U2 Recomp]", "[U2 SelfTest]", "[U2 EngineTest]", "HeapArena", "ASSERTION", "Fault:", "Error", "HALT:")):
            print(f"  {decoded}")

    print("\n" + "=" * 60)
    print(f"[AutoTest] Run completed. Exit code: {proc.returncode if crashed else 'STOPPED_AFTER_TIMEOUT'}")
    print("=" * 60)
    print("Last log lines:")
    for l in lines[-25:]:
        print(f"  {l}")
    print("=" * 60)

if __name__ == '__main__':
    main()

