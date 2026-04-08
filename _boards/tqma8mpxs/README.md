<!-- 
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany. 
-->

# TQMa8MPxS Board

## Overview

This README provides a guide for using the M7 core on TQMa8MPxS SoM on MB-SMARC-2 and
compatible mainboards. It includes demo applications that showcase the capabilities of the
board and the MCUXpresso SDK software.

## Hardware Requirements

| Hardware            | Description                                                                           | Connector     |
| ------------------  | ------------------------------------------------------------------------------------- | ------------- |
| Micro USB cable     | Debug UART Cortex-A                                                                   | X20           |
| USB to RS232 cable  | Debug UART Cortex-M                                                                   | X40           |
| Personal Computer   | Required for programming and interacting with the board.                              | -             |
| Power Supply        | A power supply with a voltage of 24V is required to power the board.                  | X12           |

## Board Preparation

1.  Connect 24V power supply at X12
2.  Connect a Micro USB cable between the host PC and the X20 USB port on the target board.
3.  Connect a USB to RS232 cable between the host PC and the X40 port on the target board.
4.  Open two serial terminals for the first (Cortex-A55, LPUART1) and the second (Cortex-M7 LPUART3)
    USB UART port with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

5.  Set the board to SD boot mode and insert an SD card with BSP supporting Cortex-M demos.
    | S-1  | S-2  | S-3  | S-4  |
    | ---- | ---- | ---- | ---- |
    | OFF  | ON   | ON   | OFF  |


## Build

In order to build the example for this board use the following command:

```bash
west build mcuxsdk-examples-tq/_boards/tqma8mpxs/<app_location> --board tqma8mpxs -Dcore_id=cm7 -DBINARY_DIR=mcuxsdk-examples-tq/build -DCUSTOM_BOARD_ROOT="mcuxsdk-examples-tq/_boards" --config=debug --pristine
```

## Demos

The demos that are included are listed as directories in the examples folder.

Each demo has its own README that provides more detailed information about the demo, including its
purpose, how to prepare for the demo, and what to expect when running the demo.

## Debugging

For debugging, hardware modifications on the SoM are needed. Debugging is currently not supported by
default. Please contact TQ-Systems support.
