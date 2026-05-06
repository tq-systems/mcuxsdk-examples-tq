<!-- 
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany. 
-->

# Hello World Demo

## Overview

This 'hello_world' demo application serves as a sanity check for new SDK build environments and board bring-up.
The demo prints the "hello world." string to the terminal using the SDK UART drivers. The purpose of this demo is to
demonstrate the use of the UART and provide a simple project for debugging and further development.

## Building the Demo

In order to build the example for this board use the following command:
```bash
west build mcuxsdk-examples-tq/_boards/tqma8mpxl/examples/demo_apps/hello_world --board tqma8mpxl -Dcore_id=cm7 -DBINARY_DIR=mcuxsdk-examples-tq/build -DCUSTOM_BOARD_ROOT="mcuxsdk-examples-tq/_boards" --config=debug --pristine
```

The above command will build the Hello World example for Cortex-M7 and the output will be in mcuxsdk-examples-tq/build folder.

## Running the Demo

1. Prepare the board as described [here](../../../README.md#board-preparartion)
2. Copy the hello_world_cm7.bin from the build folder to SD card.
3. Power on the target board and halt in U-Boot
4. Load the demo
   ```
   fatload mmc 1:1 0x48000000 hello_world_cm7.bin
   cp.b 0x48000000 0x7e0000 20000
   bootaux 0x7e0000
   ```
5. After starting the demo, characters can be entered into the Cortex-M7 terminal. The input is processed by the M7-Core and echoed back to the terminal.

## Demo Output

The log below shows the output of the Hello World demo in the Cortex-M7 terminal:

```
MCUX SDK version: 2026.03.00
hello world.
```

Furthermore, any given characters on the Cortex-M7 terminal will get echoed back.
