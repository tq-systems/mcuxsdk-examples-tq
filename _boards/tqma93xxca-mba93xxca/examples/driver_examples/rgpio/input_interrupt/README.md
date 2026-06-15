<!-- 
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany. 
-->

# RGPIO Input Interrupt Demo

## Overview
This demo will make use of the Cortex-M33 to detect falling edges on pin X1:27 (GPIO2_IO0) on MBa93xxCA. When detecting a falling edge, a message will be printed on the terminal of the M7.

## Preface
Software on Cortex-A must not use the GPIO2 bank. Running the demo will occupy the GPIO2 bank, which might cause unexpected errors, if the system tries to use GPIO2 simultaneously.

GPIO2:0 (X1:27) is used as interrupt source.
Edges at this pin are generated when connecting / disconnecting with X1:2 (3.3V)
As X1:27 (GPIO2_IO0) is configured as InterruptFallingEdge, the pin should be held high via pull-up to register a falling edge.

## Building the Demo

This command builds the RGPIO Input Interrupt example for Cortex-M33. The artefacts will be stored in the mcuxsdk-examples-tq/build folder.
```bash
west build mcuxsdk-examples-tq/_boards/tqma93xxca-mba93xxca/examples/driver_examples/rgpio/input_interrupt --board tqma93xxca-mba93xxca -Dcore_id=cm33 -DBINARY_DIR=mcuxsdk-examples-tq/build -DCUSTOM_BOARD_ROOT="mcuxsdk-examples-tq/_boards" --config=debug --pristine
```

## Running the Demo in U-Boot

1. Prepare the board as described [here](../../../README.md#board-preparartion)
2. Power on the target board and halt in U-Boot
3. Select the correct device tree (`setenv fdtfile imx93-tqma9352-mba93xxca-rpmsg.dtb`)
4. Load the demo (e.g. via USB)
    ```
    => load usb 0:1 ${loadaddr} /rgpio_input_interrupt_cm33.bin
    => cp.b ${loadaddr} 0x201e0000 ${filesize}
    => bootaux 0x1ffe0000 0
    ```

## Running the Demo in Linux

1. Prepare the board as described [here](../../../README.md#board-preparartion)
2. Power on the target board and halt in U-Boot
3. Select the correct device tree (`setenv fdtfile imx93-tqma9352-mba93xxca-rpmsg.dtb`)
4. Boot Linux. Verify that the correct device tree is used by checking if `/sys/class/remoteproc/remoteproc0/` exists
5. Load the built `rgpio_input_interrupt_cm33.elf` into `/lib/firmware`
6. Adjust the content of `/sys/class/remoteproc/remoteproc0/firmware` to the name of the demo file:
7. ```
   echo rgpio_input_interrupt_cm33.elf > /sys/class/remoteproc/remoteproc0/firmware
   ```
8. Start the Demo:
   ```
   echo start > /sys/class/remoteproc/remoteproc0/state
   ```
9. Stop the Demo:
   ```
   echo stop > /sys/class/remoteproc/remoteproc0/state
   ```

## Running the Demo via J-Link

1. Prepare the board as described [here](../../../README.md#board-preparartion)
2. Power on the target and halt in U-Boot to select the correct device tree (`setenv fdtfile imx93-tqma9352-mba93xxca-rpmsg.dtb`)
3. Boot Linux
4. Connect the Segger J-Link Debug Probe between the host PC and X20 of MBa9xxxCA
5. Open two separate terminals
6. Start a GDB server in the first terminal:
   `JLinkGDBServerCLExe -device MIMX9332_M33 -if JTAG -speed 4000 -port 50000`
   Note: On Windows, use `JLinkGDBServerCL.exe` instead.
   Depending on the module, the "-device" might be `MIMX9352_M33`
7. In the other terminal, start GDB via CLI:
   `<path to arm-none-eabi-gdb(.exe)>/arm-none-eabi-gdb`
8. In GDB, run:
    ```
    file <PATH.elf>
    target remote localhost:50000
    monitor reset
    monitor halt
    load 
    monitor go
   ```

## Demo Output

When the example runs successfully, the following message is displayed in the terminal:
```
 RGPIO Driver example

 Press X1:27 (GPIO2_IO0)
 X1:27 (GPIO2_IO0) is pressed
 ...
```

When creating an interrupt by connecting the X1:27 pin to X1:02 (3.3V) and then releasing it, the demo logs each detected falling edge.