# Usage with Scapy (usbbluetooth)

[scapy-usbbluetooth](https://github.com/usbbluetooth/scapy-usbbluetooth) exposes the controller as a [Scapy](https://scapy.net/) socket, so you can build and send raw HCI packets from Python.
It works the same on Linux, Windows and macOS.

These boards are **serial** HCI controllers, so open them with `SerialController`.

This assumes the board is flashed and you know its serial port (see [Usage](usage.md) for how to locate it).

## Install

```
pip install scapy-usbbluetooth
```

This pulls in `usbbluetooth` and `scapy` as dependencies.

## Send an HCI command

```python
from usbbluetooth import SerialController
from scapy_usbbluetooth import UsbBluetoothSocket
from scapy.layers.bluetooth import HCI_Hdr, HCI_Command_Hdr, HCI_Cmd_Reset

# ESP32-C3 / ESP32-C5 (native USB-Serial/JTAG). Baud is irrelevant over USB.
dev = SerialController("/dev/ttyACM0", rtscts=False)

# Classic ESP32 (USB-UART bridge): 921600 baud, no flow control wired.
# dev = SerialController("/dev/ttyUSB0", baudrate=921600, rtscts=False)

sock = UsbBluetoothSocket(dev)          # opens the port for you
resp = sock.sr1(HCI_Hdr() / HCI_Command_Hdr() / HCI_Cmd_Reset())
resp.show()
sock.close()
```

Use the port name for your OS: `/dev/ttyACM*` or `/dev/ttyUSB*` on Linux, `COMx` on Windows, `/dev/cu.*` on macOS.

Pass an **unopened** `SerialController`; the socket opens it. These controllers do not wire hardware flow control, so always pass `rtscts=False` (the default is `True`).

## Call a vendor command

The classic ESP32 build answers our own vendor commands (see the capability matrix in the [README](../README.md)). This reads INFO (`0xFC00` = OGF `0x3F`, OCF `0x000`), which returns the firmware name, version and board name:

```python
resp = sock.sr1(HCI_Hdr() / HCI_Command_Hdr(ogf=0x3f, ocf=0x000))
resp.show()
```

## Finding the port automatically

`usbbluetooth` cannot discover these boards (they are not USB Bluetooth-class devices), but pyserial can match them by USB id:

```python
from serial.tools import list_ports

# Native USB-Serial/JTAG boards (ESP32-C3 / ESP32-C5) report 303a:1001.
for p in list_ports.comports():
    if (p.vid, p.pid) == (0x303A, 0x1001):
        print("ESP native USB controller on", p.device)
```

This matches any ESP32 native USB-Serial/JTAG device and cannot tell a C3 from a C5 (same ids).
The classic board's USB-UART bridge uses generic ids (CH340 `1a86:7523`, CP2102 `10c4:ea60`) shared with unrelated hardware, so pick it by the port you plugged in.

## Platform notes

On Windows the boards show up as `COMx` and need no special driver setup: unlike the library's USB-class path (which may need WinUSB installed with Zadig), the serial path uses the normal COM-port driver.
See the project's [platform quirks](https://usbbluetooth.github.io/quirks/) for the rest.
