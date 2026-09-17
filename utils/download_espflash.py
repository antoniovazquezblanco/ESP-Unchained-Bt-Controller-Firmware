#!/usr/bin/env python3
"""Download the espflash binaries bundled into release packages.

espflash is a single static binary per platform; release packages ship one per
desktop platform so end users need nothing installed. This fetches them from the
espflash GitHub releases and lays them out as

    <output-dir>/<platform>/espflash[.exe]

It is the single source of truth for the pinned espflash version and asset list,
driven by the `espflash-download` CMake target (which the `distribution` target
depends on to assemble release packages).
"""

from __future__ import annotations

import argparse
import io
import sys
import urllib.request
import zipfile
from pathlib import Path

# Pinned so packages are reproducible; bump when a newer espflash is desired.
ESPFLASH_VERSION = "4.6.0"
ESPFLASH_BASE = "https://github.com/esp-rs/espflash/releases/download"

# platform key -> (release asset name, binary name inside the asset)
ESPFLASH_ASSETS = {
    "windows-x86_64": ("espflash-x86_64-pc-windows-msvc.zip", "espflash.exe"),
    "linux-x86_64": ("espflash-x86_64-unknown-linux-gnu.zip", "espflash"),
    "linux-aarch64": ("espflash-aarch64-unknown-linux-gnu.zip", "espflash"),
    "macos-x86_64": ("espflash-x86_64-apple-darwin.zip", "espflash"),
    "macos-aarch64": ("espflash-aarch64-apple-darwin.zip", "espflash"),
}


def fetch(platform: str, version: str = ESPFLASH_VERSION,
          cache_dir: Path | None = None) -> bytes:
    """Return the espflash binary bytes for a platform, downloading once."""
    asset, member = ESPFLASH_ASSETS[platform]
    cached = cache_dir / version / platform / member if cache_dir else None
    if cached and cached.is_file():
        return cached.read_bytes()

    url = f"{ESPFLASH_BASE}/v{version}/{asset}"
    with urllib.request.urlopen(url) as resp:  # noqa: S310 (trusted host)
        blob = resp.read()
    with zipfile.ZipFile(io.BytesIO(blob)) as zf:
        # The binary sits at the archive root; match by basename to be safe.
        names = [n for n in zf.namelist() if Path(n).name == member]
        if not names:
            raise SystemExit(f"error: {member} not found inside {asset}")
        data = zf.read(names[0])

    if cached:
        cached.parent.mkdir(parents=True, exist_ok=True)
        cached.write_bytes(data)
    return data


def download_to(output_dir: Path, platforms, version: str = ESPFLASH_VERSION,
                cache_dir: Path | None = None) -> None:
    """Write <output-dir>/<platform>/espflash[.exe] for each platform."""
    for platform in platforms:
        _, member = ESPFLASH_ASSETS[platform]
        dest = output_dir / platform / member
        if dest.is_file():
            print(f"[espflash] {platform}: up to date")
            continue
        print(f"[espflash] fetching {platform} (espflash {version})")
        dest.parent.mkdir(parents=True, exist_ok=True)
        dest.write_bytes(fetch(platform, version, cache_dir))
        if not member.endswith(".exe"):
            dest.chmod(0o755)


def main() -> None:
    p = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--output-dir", type=Path, required=True,
                   help="directory to write <platform>/espflash into")
    p.add_argument("--version", default=ESPFLASH_VERSION,
                   help=f"espflash version (default: {ESPFLASH_VERSION})")
    p.add_argument("--platforms", default=",".join(ESPFLASH_ASSETS),
                   help="comma-separated platforms to fetch (default: all)")
    p.add_argument("--cache-dir", type=Path, default=None,
                   help="optional download cache shared with the packager")
    args = p.parse_args()

    platforms = [x.strip() for x in args.platforms.split(",") if x.strip()]
    unknown = [x for x in platforms if x not in ESPFLASH_ASSETS]
    if unknown:
        sys.exit(f"error: unknown platform(s): {', '.join(unknown)}. "
                 f"Choose from: {', '.join(ESPFLASH_ASSETS)}")

    download_to(args.output_dir, platforms, args.version, args.cache_dir)


if __name__ == "__main__":
    main()
