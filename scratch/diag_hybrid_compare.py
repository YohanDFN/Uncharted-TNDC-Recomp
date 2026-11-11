"""Run an explicitly selected probe through unchanged launcher preparation.

No executable is copied over the working build. Logs are kept per run and
only the process tree created by this invocation is terminated at timeout.
"""
import argparse
import os
from pathlib import Path
import subprocess
import sys
import time
import re

ROOT = Path(__file__).resolve().parent.parent


def child(probe, excluded, hooks, dump):
    sys.path.insert(0, str(ROOT))
    sys.path.insert(0, str(ROOT / "launcher"))
    from uncharted_launcher import game_environment, CONFIG_FILE, load_json, APP_DEFAULTS
    os.environ.update(game_environment(load_json(CONFIG_FILE, APP_DEFAULTS)))
    os.environ["PATH"] = "C:/msys64/clang64/bin;" + os.environ.get("PATH", "")
    os.environ["BB_U2_SELFTEST"] = "1"
    os.environ["BB_PROBE"] = str(probe)
    for key in ("BB_U2_MODULES", "BB_U2_DISABLE_DETOURS", "BB_U2_DETOUR_ALLOW",
                "BB_U2_DETOUR_RANGE", "BB_GUEST_HOOKS", "BB_HOOK_DUMP", "BB_HOOK_DUMP_BEFORE"):
        os.environ.pop(key, None)
    if hooks:
        os.environ["BB_GUEST_HOOKS"] = hooks
    if dump:
        os.environ["BB_HOOK_DUMP"] = dump
    if excluded:
        tree = probe.parents[2]
        files = [tree / "src/u2_engine/u2_engine_registry.cpp"]
        files += list((tree / "src").glob("runtime_u2_*.c"))
        names = []
        for file in files:
            names.extend(re.findall(r'\{\s*"([^"]+)"\s*,\s*0x', file.read_text(encoding="utf-8")))
        os.environ["BB_U2_DETOUR_ALLOW"] = ",".join(name for name in names if name not in excluded)
    import run
    sys.argv = [str(ROOT / "run.py")]
    run.main()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--probe", type=Path, required=True)
    parser.add_argument("--label", default="baseline")
    parser.add_argument("--seconds", type=int, default=20)
    parser.add_argument("--child", action="store_true")
    parser.add_argument("--exclude", action="append", default=[])
    parser.add_argument("--hooks", default="", help="explicit BB_GUEST_HOOKS specification")
    parser.add_argument("--dump", default="", help="explicit BB_HOOK_DUMP offset:bytes:file specification")
    args = parser.parse_args()
    probe = args.probe.resolve(strict=True)
    if args.child:
        child(probe, args.exclude, args.hooks, args.dump)
        return
    if not args.label.replace("_", "").replace("-", "").isalnum():
        parser.error("label must contain letters, digits, hyphens or underscores")
    log = ROOT / "user" / ("diag_compare_" + args.label + ".log")
    started = time.monotonic()
    with log.open("wb") as output:
        command = [sys.executable, str(Path(__file__).resolve()), "--probe", str(probe), "--child"]
        for name in args.exclude:
            command += ["--exclude", name]
        if args.hooks:
            command += ["--hooks", args.hooks]
        if args.dump:
            command += ["--dump", args.dump]
        proc = subprocess.Popen(command,
                                cwd=ROOT, stdout=output, stderr=subprocess.STDOUT,
                                stdin=subprocess.DEVNULL)
        try:
            code = proc.wait(timeout=args.seconds)
            outcome = "EXIT " + str(code)
        except subprocess.TimeoutExpired:
            stopped = subprocess.run(["taskkill", "/F", "/T", "/PID", str(proc.pid)],
                                     capture_output=True, text=True)
            if stopped.returncode:
                raise RuntimeError("Cannot stop diagnostic process tree: " + stopped.stderr)
            proc.wait(timeout=10)
            outcome = "STOPPED_AFTER_TIMEOUT"
    lines = log.read_text(encoding="utf-8", errors="replace").splitlines()
    faults = [line for line in lines if any(word in line for word in
              ("Fault:", "Guest fault", "[U2 Trap]", "FATAL", "cannot create"))]
    print(f"{args.label}: {outcome}, {time.monotonic()-started:.1f}s, log={log}")
    for line in lines:
        if any(word in line for word in ("PASSED", "Installed ", "State Transition", "Hook 0x")):
            print(line)
    print("Fault/assertion/allocation-error lines:", len(faults))
    for line in faults[:12]:
        print(line)


if __name__ == "__main__":
    main()
