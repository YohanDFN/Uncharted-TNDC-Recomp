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
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / 'launcher'))
sys.path.insert(0, str(ROOT / 'scripts'))

from uncharted_launcher import game_environment, CONFIG_FILE, load_json, APP_DEFAULTS

def main():
    duration = int(sys.argv[1]) if len(sys.argv) > 1 else 10
    print(f"[AutoTest] Loading launcher settings from {CONFIG_FILE} ...")
    settings = load_json(CONFIG_FILE, APP_DEFAULTS)
    
    # Force windowed mode for headless/test runs if desired, or keep launcher settings
    env = game_environment(settings)
    
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
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT
        )
        
        start_time = time.time()
        crashed = False
        lines = []
        
        # Read stream non-blocking or until timeout
        while True:
            elapsed = time.time() - start_time
            if proc.poll() is not None:
                # Process exited on its own (crash or normal exit)
                crashed = True
                remaining = proc.stdout.read()
                if remaining:
                    lf.write(remaining)
                    lines.extend(remaining.decode('utf-8', errors='replace').splitlines())
                break
                
            line = proc.stdout.readline()
            if line:
                lf.write(line)
                lf.flush()
                decoded = line.decode('utf-8', errors='replace').rstrip()
                lines.append(decoded)
                if len(lines) > 200:
                    lines.pop(0)
                # Print key milestones to console in real-time
                if any(k in decoded for k in ("State Transition", "[U2 Loop]", "HeapArena", "ASSERTION", "Fault:", "Error", "HALT:")):
                    print(f"  {decoded}")
            else:
                time.sleep(0.01)
                
            if elapsed >= duration:
                print(f"[AutoTest] Reached timeout of {duration}s without crash. Terminating test run cleanly...")
                proc.terminate()
                try:
                    proc.wait(timeout=3)
                except subprocess.TimeoutExpired:
                    proc.kill()
                break

    print("\n" + "=" * 60)
    print(f"[AutoTest] Run completed. Exit code: {proc.returncode if crashed else 'STOPPED_AFTER_TIMEOUT'}")
    print("=" * 60)
    print("Last log lines:")
    for l in lines[-25:]:
        print(f"  {l}")
    print("=" * 60)

if __name__ == '__main__':
    main()

