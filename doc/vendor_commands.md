# Vendor-specific HCI commands

Vendor-specific HCI commands and events of controllers reporting Company Id `0xF00D`.

All commands use OGF `0x3F` and reply with a Command Complete whose first return byte is a status.
Any other opcode in the group replies `0x01` Unknown HCI Command.

## Commands

| Opcode   | Name                  | Description                                     |
| :------- | :-------------------- | :---------------------------------------------- |
| `0xFC00` | `INFO`                | Firmware name, firmware version and board name. |
| `0xFC01` | `SUPPORTED_CMDS`      | Which of these commands are implemented.        |
| `0xFC02` | `SET_BDADDR`          | Override the controller public address.         |
| `0xFC03` | `SET_TRAFFIC_MONITOR` | Enable low-level link traffic capture.          |
| `0xFC04` | `SET_SCAN_CHANNEL`    | Pin advertising reception to one primary channel. |
| `0xFC05` | `INJECT_PDU`          | Transmit a crafted LL/LMP PDU on an active link. |

### `0xFC00` INFO

No parameters.

Returns status, then three fields, each a `uint8` length followed by that many bytes: firmware name, firmware version, board name.
The strings are not NUL-terminated.

```
00                                            status
1b 65 73 70 5f 75 6e ... 72              (27) "esp_unchained_bt_controller"
18 76 30 2e 30 2e 31 ... 79              (24) "v0.0.1-75-g1744b47-dirty"
0c 65 73 70 33 32 64 65 76 6b 69 74 63   (12) "esp32devkitc"
```

### `0xFC01` SUPPORTED_CMDS

No parameters.

Returns status, then a little-endian `uint64` bitfield where bit N is set when the command with OCF N is implemented.
`INFO` is bit 0.
Commands past OCF 63 read as absent.

A build with all six commands returns `00 3f 00 00 00 00 00 00 00`.

### `0xFC02` SET_BDADDR

| Offset | Size | Field                                   |
| :----- | :--- | :-------------------------------------- |
| 0      | 6    | `BD_ADDR`, least-significant byte first |

Sets both the BR/EDR and the LE public address.
Not persistent; a reset restores the factory address.

Returns status.
Fewer than 6 bytes returns `0x12` Invalid HCI Command Parameters.

### `0xFC03` SET_TRAFFIC_MONITOR

| Offset | Size | Field |
| :----- | :--- | :---- |
| 0      | 1    | Flags |

| Flag   | Source                        | State       |
| :----- | :---------------------------- | :---------- |
| `0x01` | Outgoing BR/EDR LMP PDUs      | implemented |
| `0x02` | Incoming BR/EDR LMP PDUs      | implemented |
| `0x04` | Outgoing BLE LL data PDUs     | implemented |
| `0x08` | Incoming BLE LL PDUs          | implemented |

Flags combine; `0x00` disables every source.
Each captured PDU is reported as a `0xFF` event.

`0x08` captures every incoming LL PDU, data and control (LLCP). `0x04` captures
outgoing LL PDUs. On the C3 and C5 this includes outgoing control (LLCP); on the
classic ESP32 only outgoing *data* is captured, because its outgoing LLCP is queued
to the baseband below the tap — the peer's replies still arrive on `0x08`.

Returns status.
An empty parameter returns `0x12` Invalid HCI Command Parameters.
A reserved flag returns `0x11` Unsupported Feature or Parameter Value.

### `0xFC04` SET_SCAN_CHANNEL

| Offset | Size | Field   |
| :----- | :--- | :------ |
| 0      | 1    | Channel |

Channel 37, 38 or 39 pins advertising reception (scanning) to that single
primary channel; 0 restores the normal three-channel hop.

Scanning normally sweeps all three primary channels. Pinning keeps the radio on
one, so you only receive advertisements sent on it. It takes effect on the
running scan and on scans started afterwards, and persists until changed or the
controller resets.

Returns status.
An empty parameter, or a channel other than 0/37/38/39, returns `0x12` Invalid
HCI Command Parameters.

### `0xFC05` INJECT_PDU

Transmit one attacker-crafted link-layer PDU on a connection the controller owns
-- the write twin of `SET_TRAFFIC_MONITOR`. The LL PDU is encoded exactly as the
`0xFF` capture event carries it, so a captured PDU can be replayed, tweaked or
malformed. Injecting a raw LL/LMP PDU **bypasses the link-controller state
machine**: that is the point for testing, but it can desync or wedge the
controller (recover with `HCI_Reset`). Authorised testing only.

| Offset | Size | Field                                                       |
| :----- | :--- | :--------------------------------------------------------- |
| 0      | 1    | Kind: `0x01` BR/EDR LMP, `0x02` BLE LL                     |
| 1      | 2    | Link id / connection handle, little-endian                 |
| 3      | *n*  | The PDU (see below)                                        |

The link id / handle is the controller link the PDU goes out on. The host learns
it from a **TX** capture event (`0xFF`, direction `0x00`); RX captures report link
id `0xFF` and cannot be used to source it.

**BLE LL** (`kind 0x02`) -- the PDU mirrors the capture event's LL body:

| Offset | Size | Field                                                  |
| :----- | :--- | :---------------------------------------------------- |
| 3      | 1    | LLID: `0x01` data continuation, `0x02` data start, `0x03` control (LLCP) |
| 4      | 1    | Payload length                                        |
| 5      | *n*  | Payload (for control, the LLCP opcode first)         |

Control (LLCP) payload is capped at 27 bytes (26 on the classic ESP32); data at
251 (27 on the classic -- a single PDU, see below). On every target the data LLID
start/continuation choice is made by the controller at transmit time, so LLID
`0x01` and `0x02` behave the same.

On the C3/S3, data longer than the link's current maximum TX octets (27 without
Data Length Extension) is fragmented by the controller into a start PDU plus
continuations, exactly like host ACL, and each injected data PDU is reported in
Number Of Completed Packets like host ACL. Only one control PDU is in flight per
link: while one (injected or the controller's own) awaits its air ack, a control
injection returns `0x0C`; retry after a connection event.

The classic ESP32 injects a single LL PDU without fragmenting, so an LL payload is
capped at 27 octets (data) / 26 (control); a longer one returns `0x12`. Data goes
through the controller's own send path (reported in Number Of Completed Packets);
control goes out verbatim through the controller's raw-LLCP test path, so any
opcode -- known, unknown or malformed -- is transmitted exactly as given.

**BR/EDR LMP** (`kind 0x01`) -- the PDU is the raw LMP bytes, opcode first
(`byte0 = opcode<<1 | TID`; `0x7F` escapes to a 1-byte extended opcode in
`byte1`), up to 17 bytes.

Per-target support: the BLE-only targets (ESP32-C3, ESP32-S3, ESP32-C5) implement
`kind 0x02` BLE LL and reject `kind 0x01` LMP; the classic ESP32 implements both
`kind 0x01` LMP and `kind 0x02` BLE LL.

Returns status:

| Status | Meaning |
| :----- | :------ |
| `0x00` | Success -- the PDU was queued for transmission. |
| `0x11` | Unsupported Feature -- this target has no path for that kind. |
| `0x12` | Invalid HCI Command Parameters -- too short, length mismatch, or a bad LLID / oversized payload. |
| `0x07` | Memory Capacity Exceeded -- the controller TX buffer pool is empty. |
| `0x0C` | Command Disallowed -- no such active link, a control PDU is already in flight on it (retry), or the controller refused the submit. |

## Events

| Code   | Subcode | Description              |
| :----- | :------ | :----------------------- |
| `0xFF` | `0x01`  | One captured LMP/LL PDU. |

### `0xFF` event, subcode `0x01`

| Offset | Size | Field                                                    |
| :----- | :--- | :------------------------------------------------------- |
| 0      | 1    | Subcode, `0x01`                                          |
| 1      | 1    | Direction: `0x00` TX, `0x01` RX                          |
| 2      | 1    | Link id, `0xFF` when unknown                             |
| 3      | 4    | Controller clock at capture, little-endian `uint32`      |
| 7      | 1    | PDU length                                               |
| 8      | *n*  | The PDU bytes (see below)                                |

For BR/EDR LMP the PDU is the raw LMP bytes, opcode first, and the length is the
on-air length. For BLE LL the PDU is a reconstructed 2-byte header — LLID then
length — followed by the payload; for a control PDU the payload begins with the
LLCP opcode.

The clock unit is the controller's native tick: 312.5 us on the classic ESP32 and
C3 (the BT clock), the link-layer timer tick on the C5.
RX captures report link id `0xFF`.
Only PDUs the controller accepted are captured.
