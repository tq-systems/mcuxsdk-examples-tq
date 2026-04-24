<!-- 
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany. 
-->

# MBa93xx Board Setup

## Overview

This README provides a comprehensive guide for using the MBa9xxx board with the TQMa93xxXX module
using the Cortex-M33. It includes several demo applications that showcase the capabilities of the
board and the MCUXpresso SDK software. The demos range from simple sanity checks like the
"Hello World" demo to more complex demonstrations of inter-core communication in a multicore system.

## Hardware Requirements

| Hardware                      | Description                                                                           | Pin Reference |
| ----------------------------- | ------------------------------------------------------------------------------------- | ------------- |
| Micro USB cable               | Required for connecting the board to your computer for programming and communication. | X22           |
| J-Link Debug Probe (optional) | Used for debugging and programming the board.                                         | X20           |
| Personal Computer             | Required for programming and interacting with the board.                              | -             |
| Power Supply                  | A power supply with a voltage of 24V is required to power the board.                  | X23, X24      |

Please note that these requirements are specific to the MBa9xxx board using the TQMa93xxXX module.
Always refer to the specific READMEs of the demo you are running for any additional hardware
requirements. Further information can be found in the documentation directory.

## Board Preparation

1.  Connect 24V power supply at X23 or X24
2.  Connect a USB cable between the host PC and the X22 USB port on the target board.
3.  Open two serial terminals for the first (Cortex-A55, LPUART1) and the second (Cortex-M33, LPUART3)
    USB UART port with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

4.  Set the board to SD boot mode and insert an SD card with BSP supporting Cortex-M demos.
    | S-1  | S-2  | S-3  | S-4  |
    | ---- | ---- | ---- | ---- |
    | ON   | ON   | OFF  | OFF  |


## Building Demos for TQMa93xxXX

In order to build a demo for TQMa93xxXX use the following command:

```bash
west build _boards/tqmba93xxca/<app_location> --board tqmba93xxca \
-Dcore_id=cm33 --build-dir build \
-DCUSTOM_BOARD_ROOT="<workspace>/mcuxsdk-examples-tq/_boards" \
--config=<configuration> --pristine
```

## Available Demos

|  Demo                                                                                            |  Description                                                              |
| ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------- |
|  [Hello World](examples/demo_apps/hello_world/README.md)                                         |  Hello World demo, to make sure everything has been set up correctly      |
|  [RPMSG Ping Pong](examples/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote/README.md)  |  Demonstrates inter-core communication between Cortex-A55 and Cortex-M33  |
|  [RGPIO LED Output Demo](examples/driver_examples/rgpio/input_interrupt/README.md)               |  Uses the Cortex-M33 to toggle a GPIO pin periodically                    |


