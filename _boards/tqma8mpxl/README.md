<!--
SPDX-License-Identifier: CC-BY-4.0

Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# TQMa8MPxL Board

## Overview

This README provides a guide for using the M7 core on TQMa8MPxL SoM on MBa8MPxL and
compatible mainboards. It includes demo applications that showcase the capabilities of the
board and the MCUXpresso SDK software.

## Trivial Hardware Requirements

| Hardware           | Description                                                                           | Pin Reference |
| ------------------ | ------------------------------------------------------------------------------------- | ------------- |
| Micro USB cable    | Required for connecting the board to your computer for programming and communication. | X28           |
| J-Link Debug Probe | Used for debugging and programming the board.                                         | X22           |
| Personal Computer  | Required for programming and interacting with the board.                              | -             |
| Power Supply       | A power supply with a voltage of 24V is required to power the board.                  | X13           |

## Board Preparation

1.  Connect 24V power supply at X13
2.  Connect a USB cable between the host PC and the X28 USB port on the target board.
3.  Connect the J-Link Debug Probe between the host PC and the X22 port on the target board.
4.  Open two serial terminals for the first (Cortex-A55, LPUART1) and the second (Cortex-M7, LPUART3)
    USB UART port with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

5.  Set the board to SD boot mode and insert an SD card with BSP supporting Cortex-M demos.
    | S-1  | S-2  | S-3  | S-4  |
    | ---- | ---- | ---- | ---- |
    | ON   | ON   | OFF  | OFF  |


## Build

In order to build the examples for this board use the following command:

```bash
west build mcuxsdk-examples-tq/_boards/tqma8mpxl/<app_location> --board tqma8mpxl -Dcore_id=cm7 -DBINARY_DIR=mcuxsdk-examples-tq/build  -DCUSTOM_BOARD_ROOT="mcuxsdk-examples-tq/_boards" --config=debug --pristine
```

## Demos

The demos that are included are listed as directories in the examples folder.

Each demo has its own README that provides more detailed information about the demo, including its
purpose, how to prepare for the demo, and what to expect when running the demo.

## Debugging

### west

Debugging support using `west` command is configured for SEGGER J-Link. To debug with `west` use
the following command:

```
west debug -r jlink
```

### VS-Code

Before debugging, specify the appropriate device configuration in the [launch.json](../templates/launch.json) file.
Refer to the [launch.json](../templates/launch.json) example provided. Follow the instructions
on [debugging with VS-Code](../README.md/#debugging-with-vs-code).

### GDB-Server

Use GDB for independent loading and debugging targets from your IDE:

- Start GDB server.
- Select the connection via USB.
- Select device `MIMX8ML8_M7` and set the endian to `Little Endian`.
- Select `JTAG` as the interface with a fixed speed of 4000 kHz.
  - __NOTE__: If debugging doesn't work as expected, you can try changing the interface to SWD and/or varying the
  interface speed.
- Further options are optional.
- Then start gdb via CLI.
- Follow the command for booting:

  ``` bat
  file <PATH.elf>
  target remote localhost:<port>
  monitor reset
  monitor halt
  load
  monitor go
  ```
