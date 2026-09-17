#!/usr/bin/env bash
# ESP Unchained Bluetooth Controller
# Firmware flasher script

set -e

# Banner
if [ -t 1 ] && [ -z "${NO_COLOR:-}" ]; then
  BT=$'\033[38;2;0;130;252m'   # Bluetooth blue
  WHITE=$'\033[1;37m'
  RST=$'\033[0m'
else
  BT=''; WHITE=''; RST=''
fi
printf '%s' "$BT"
cat <<'BANNER'
 _   _         _         _             _
| | | |_ _  __| |_  __ _(_)_ _  ___ __| |
| |_| | ' \/ _| ' \/ _` | | ' \/ -_) _` |
 \___/|_||_\__|_||_\__,_|_|_||_\___\__,_|
  ___ _          _            _   _
 | _ ) |_  _ ___| |_ ___  ___| |_| |_
 | _ \ | || / -_)  _/ _ \/ _ \  _| ' \
 |___/_|\_,_\___|\__\___/\___/\__|_||_|
BANNER
printf '\n            %sFlasher tool%s\n\n' "$WHITE" "$RST"

# OS detection (MINGW/MSYS/Cygwin all run the native Windows espflash.exe)
case "$(uname -s)" in
  Linux)                OS=linux;   EXE= ;;
  Darwin)               OS=macos;   EXE= ;;
  MINGW*|MSYS*|CYGWIN*) OS=windows; EXE=.exe ;;
  *) echo "[!] Unsupported OS: $(uname -s)" >&2; exit 1 ;;
esac
echo "[+] Detected OS: $OS"

# CPU architecture detection
case "$(uname -m)" in
  x86_64|amd64)  ARCH=x86_64 ;;
  arm64|aarch64) ARCH=aarch64 ;;
  *) echo "[!] Unsupported CPU: $(uname -m)" >&2; exit 1 ;;
esac
echo "[+] Detected CPU: $ARCH"

# Locate the espflash tool for the detected OS and architecture
DIR="$(cd "$(dirname "$0")" && pwd)"
TOOL="$DIR/tools/$OS-$ARCH/espflash$EXE"
if [ ! -x "$TOOL" ]; then chmod +x "$TOOL" 2>/dev/null || true; fi
if ! "$TOOL" --version >/dev/null 2>&1; then
  echo "[!] Could not execute the espflash tool." >&2
  exit 1
fi
echo "[+] Using espflash tool: $TOOL"

# Pick the first firmware image (.bin) found next to this script.
FW=""
for f in "$DIR"/*.bin; do FW="$f"; break; done
if [ ! -f "$FW" ]; then
  echo "[!] No firmware (.bin) image found next to this script." >&2
  exit 1
fi
echo "[+] Firmware image: $(basename "$FW")"

# Prompt the user to plug in their device if they haven't already done so
read -r -p "Connect ONLY the board you want to flash, then press Enter to continue... " _ || true

# Print device information using espflash (also confirms the board is reachable)
echo "[*] Target device:"
if ! "$TOOL" board-info "$@"; then
  echo "[!] Could not read device information. Is the board connected?" >&2
  exit 1
fi

# Ask for the user's confirmation before proceeding with the flash
read -r -p "Are you sure you want to proceed with flashing? (y/N) " CONFIRM || CONFIRM=""
if [[ ! "$CONFIRM" =~ ^[Yy]$ ]]; then
  echo "Aborted."
  exit 1
fi

# Flash
"$TOOL" write-bin "$@" 0x0 "$FW"
