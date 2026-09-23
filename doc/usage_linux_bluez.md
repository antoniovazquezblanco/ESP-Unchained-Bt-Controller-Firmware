# Usage on Linux (BlueZ)

On Linux the controller is used through **BlueZ**. Once the serial port is attached to the stack it shows up as a normal `hciX` adapter, and every BlueZ tool (`bluetoothctl`, `btmgmt`, Wireshark, ...) works against it.

This assumes the board is already flashed and you know its serial port (see [Usage](usage.md) for how to locate it).

## Attach manually

`btattach` (shipped with BlueZ) binds a serial port to the stack using the standard HCI protocol.
Keep it running: the adapter disappears when you kill this process.

```
# ESP32-C3 / ESP32-C5 (native USB-Serial/JTAG). Baud is irrelevant over USB.
sudo btattach -B /dev/ttyACM0 -P h4

# Classic ESP32 (USB-UART bridge, HCI at 921600, no flow control wired).
sudo btattach -B /dev/ttyUSB0 -P h4 -S 921600 -N
```

A new adapter appears. Confirm it and bring it up:

```
hciconfig -a           # or: bluetoothctl list
sudo hciconfig hci0 up
```

From here use it like any adapter, for example:

```
bluetoothctl
```

Stop `btattach` (`Ctrl-C`) to detach.

## Attach automatically (udev)

To attach the controller the moment it is plugged in, have udev start a `btattach` service for it.
`btattach` must stay running, so trigger a systemd unit instead of calling it from `RUN+` (udev kills long-running `RUN+` processes).

Find your board's USB ids:

```
udevadm info -a -n /dev/ttyACM0 | grep -E 'idVendor|idProduct'
```

Native USB-Serial/JTAG boards report `303a:1001`; the classic board shows its bridge instead (CP2102 `10c4:ea60`, CH340 `1a86:7523`).

Service template `/etc/systemd/system/btattach@.service`:

```
[Unit]
Description=Attach ESP Unchained HCI controller on %I

[Service]
ExecStart=/usr/bin/btattach -B /dev/%I -P h4
```

Rule `/etc/udev/rules.d/99-esp-unchained.rules` (native USB boards):

```
ACTION=="add", SUBSYSTEM=="tty", ATTRS{idVendor}=="303a", ATTRS{idProduct}=="1001", \
  TAG+="systemd", ENV{SYSTEMD_WANTS}="btattach@%k.service"
```

Reload; it applies on the next plug-in:

```
sudo udevadm control --reload
```

For the classic ESP32, match its bridge ids instead and add its speed to the service line (`-S 921600 -N`).
