# Usage

## What this is

The firmware turns the board into a **bare Bluetooth HCI controller** that speaks the standard **HCI** transport.
There is no on-chip host stack meaning that to the computer it is simply another Bluetooth controller.

The advantages is that this Bluetooth controller has some vendor commands that allow us to do things such as changing the MAC address of the device.

This means that you will be able to use this as any other serial Bluetooth controller in your machine.

## Flashing the firmware

Prebuilt, ready-to-flash packages are published on the [Releases](https://github.com/antoniovazquezblanco/ESP-Unchained-Bt-Controller-Firmware/releases) page, one zip per supported board (named `esp-unchained-<board>-<version>.zip`).
Each package bundles the firmware, the flashing tool and a small script, so you do **not** need Python, ESP-IDF or any toolchain installed.

To flash your device:

1. Download the zip that matches your board (for example `esp-unchained-scapycon2026badge-...` for the ScapyCon 2026 badge).
2. Unzip it anywhere.
3. Connect **only** the board you want to flash, then run the script for your operating system:
   - **Windows**: double-click `flash.bat`.
   - **Linux / macOS**: run `./flash.sh` from a terminal.

The script shows the detected device, asks for confirmation and then flashes it.

The serial port is detected automatically. If several serial devices are connected and the wrong one is picked, pass the port explicitly:

- **Windows**: `flash.bat --port COM5`
- **Linux**: `./flash.sh --port /dev/ttyUSB0`
- **macOS**: `./flash.sh --port /dev/cu.usbserial-XXXX`

## Locating the serial port

Once flashed, the board shows up on your computer as a serial (UART) port.

The exact device name cannot be always set.
It depends on the board and its USB-to-UART interface and on your operating system.
Different boards may enumerate differently, so don't assume a particular name.

The reliable way to find it is to list the serial ports before and after plugging the board in, and take the one that appears.

Here are some hints on how to locate your device on different operative systems:

- **Linux**: Device will commonly show up under `/dev/ttyACM*` or `/dev/ttyUSB*`. You may use the command `ls -1 /dev/ttyACM* /dev/ttyUSB*` to list all ports.
- **Windows**: Look in the `Device Manager`, under `Ports (COM & LPT)` for a new `COMx` entry.
- **macOS**: Device will commonly show up under `/dev/cu.usbmodem*` or `/dev/cu.usbserial-*`. You may use the command `ls -1 /dev/cu.*` to list all ports.
