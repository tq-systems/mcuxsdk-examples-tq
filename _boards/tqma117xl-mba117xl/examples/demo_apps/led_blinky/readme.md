<!-- 
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany. 
-->

# Led Blinky Demo

## Overview

The LED Blinky demo application provides a sanity check for the new SDK build environments and board bring up. The LED Blinky demo 
uses the systick interrupt to realize the function of timing delay. The example continuosly toggles the green user led on MBa117xL.
The purpose of this demo is to provide a simple project for debugging and further development.

## Building the Demo

In order to build the example for this board use the following command:

```bash
west build mcuxsdk-examples-tq/_boards/tqma117xl-mba117xl/examples/demo_apps/led_blinky --board tqma117xl-mba117xl -Dcore_id=cm7 -DBINARY_DIR=mcuxsdk-examples-tq/build  -DCUSTOM_BOARD_ROOT="mcuxsdk-examples-tq/_boards" --config=debug --pristine
```

The above command will build the Led Blinky example for Cortex-M7 and the output will be stored in the build folder.

## Running the Demo and debugging

1. Prepare the board as described [here](../../../README.md#board-preparartion)

2. After the compilation is complete  run the following command to load the file into the board to debug and run it.

```bash
west debug
```

3. Use GDB commands to debug and run the demo.

__Attention__: Only the demo compiled with setting `--config=debug` will be able to debug from the Jlink debugger and GDB commands.

__Note__: Demo can be compiled for SPI Flash but it needs to be flashed with MCUXpresso [Secure Provisioning Tool](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-secure-provisioning-tool:MCUXPRESSO-SECURE-PROVISIONING#downloads)

## Demo Output

The User Led 1 will blink in Green colour.
