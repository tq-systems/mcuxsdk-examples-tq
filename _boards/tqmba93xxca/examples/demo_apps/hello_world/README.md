<!--
SPDX-License-Identifier: CC-BY-4.0

Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# Hello World Demo

## Overview

This 'hello_world' demo application serves as a sanity check for new SDK build environments and board bring-up.
This demo prints the "hello world." string to the terminal using the SDK UART drivers. The purpose of this demo is to
demonstrate the use of the UART and provide a simple project for debugging and further development.

## Running the Demo in U-Boot

1. Power on the target board and halt in U-Boot
2. Select the correct device tree (`setenv fdtfile imx93-tqma9352-mba93xxca-rpmsg.dtb`)
3. Load the demo (e.g. via USB)
    ```
    => load usb 0:1 ${loadaddr} /hello_world_cm33.bin
    => cp.b ${loadaddr} 0x201e0000 ${filesize}
    => bootaux 0x1ffe0000 0
    ```

## Running the Demo in Linux

1. Power on the target board and halt in U-Boot
2. Select the correct device tree (`setenv fdtfile imx93-tqma9352-mba93xxca-rpmsg.dtb`)
3. Boot Linux. Verify that the correct device tree is used by checking if `/sys/class/remoteproc/remoteproc0/` exists
4. Load the built `hello_world_cm33.elf` into `/lib/firmware`
5. Adjust the content of `/sys/class/remoteproc/remoteproc0/firmware` to the name of the demo file:
6. ```
   echo hello_world_cm33.elf > /sys/class/remoteproc/remoteproc0/firmware
   ```
7. Start the Demo:
   ```
   echo start > /sys/class/remoteproc/remoteproc0/state
   ```
8. Stop the Demo:
   ```
   echo stop > /sys/class/remoteproc/remoteproc0/state
   ```

## Running the Demo via J-Link

1. Power on the target board
2. Connect the Segger J-Link Debug Probe between the host PC and X20 of MBa9xxxCA.
3. Open two separate terminals.
4. Start a GDB server in the first terminal:
   `JLinkGDBServerCLExe -device MIMX9332_M33 -if JTAG -speed 4000 -port 50000`
   Note: On Windows, GDB server might need to be called via: `JLinkGDBServerCL.exe`.
5. In the other terminal, start GDB via CLI:
   `<path to arm-none-eabi-gdb(.exe)>/arm-none-eabi-gdb`
6. In GDB, run:
    ```
    file <PATH.elf>
    target remote localhost:50000
    monitor reset
    monitor halt
    load 
    monitor go
   ```

## Demo Output

The log below shows the output of the Hello World demo in the terminal window:

```
hello world.
```
