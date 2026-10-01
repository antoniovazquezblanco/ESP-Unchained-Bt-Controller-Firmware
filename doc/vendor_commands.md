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

A build with all four commands returns `00 0f 00 00 00 00 00 00 00`.

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

| Flag   | Source                   | State       |
| :----- | :----------------------- | :---------- |
| `0x01` | Outgoing BR/EDR LMP PDUs | implemented |
| `0x02` | Incoming BR/EDR LMP PDUs | implemented |
| `0x04` | Outgoing BLE LL PDUs     | reserved    |
| `0x08` | Incoming BLE LL PDUs     | reserved    |

Flags combine; `0x00` disables every source.
Each captured PDU is reported as a `0xFF` event.

Returns status.
An empty parameter returns `0x12` Invalid HCI Command Parameters.
A reserved flag returns `0x11` Unsupported Feature or Parameter Value.

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
| 3      | 4    | Controller clock, little-endian `uint32`, 312.5 us ticks |
| 7      | 1    | PDU length                                               |
| 8      | *n*  | The PDU, opcode byte first                               |

The PDU length is the on-air length.
RX captures report link id `0xFF`.
Only PDUs the controller accepted are captured.
