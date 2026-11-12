"""Archive CURRENT_WORK before replacing it with a nonempty UTF-8 file."""
import argparse
import hashlib
import os
from datetime import datetime, timezone
from pathlib import Path
import tempfile


def rotate(current: Path, journal: Path, replacement: Path):
    if replacement.resolve() in (current.resolve(), journal.resolve()):
        raise ValueError('Replacement must be a separate file')
    new = replacement.read_bytes()
    if not new.decode('utf-8-sig').strip():
        raise ValueError('Replacement must contain a nonempty UTF-8 handoff')
    lock = current.with_suffix('.rotation.lock')
    # A stale lock after a crash requires inspection, never automatic removal.
    handle = lock.open('xb')
    try:
        old = current.read_bytes()
        old.decode('utf-8-sig')
        digest = hashlib.sha256(old).hexdigest()
        stamp = datetime.now(timezone.utc).isoformat()
        entry = f'\n\n---\n\n## {stamp}\n\nSHA-256: `{digest}`\n\n'.encode() + old + b'\n<!-- End of archived handoff -->\n'
        with journal.open('ab') as archive:
            archive.write(entry)
            archive.flush()
            os.fsync(archive.fileno())
        with journal.open('rb') as archive:
            archive.seek(-len(entry), os.SEEK_END)
            if archive.read() != entry:
                raise OSError('Archive verification failed')
        if current.read_bytes() != old:
            raise OSError('CURRENT_WORK changed during archival; replacement refused')
        temp = None
        try:
            with tempfile.NamedTemporaryFile(dir=current.parent, delete=False) as output:
                temp = Path(output.name)
                output.write(new)
                output.flush()
                os.fsync(output.fileno())
            os.replace(temp, current)
        finally:
            if temp is not None and temp.exists():
                temp.unlink()
    finally:
        handle.close()
        lock.unlink()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--next', required=True, type=Path)
    args = parser.parse_args()
    docs = Path(__file__).resolve().parent.parent / 'docs'
    rotate(docs / 'CURRENT_WORK.md', docs / 'WORK_LOG.md', args.next)
    print('Previous handoff archived and verified; CURRENT_WORK replaced.')


if __name__ == '__main__':
    main()
