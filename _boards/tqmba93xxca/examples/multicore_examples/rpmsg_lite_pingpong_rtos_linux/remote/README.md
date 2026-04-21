<!-- 
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany. 
-->

# RPMSG Ping Ping Demo

## Overview

The RPMSG pingpong demo shows communication between the Cortex-A and Cortex-M core by sending a message back and forth for a set amount of times.
It does not need any special hardware configurations.

## Building the Demo

This command builds the RPMSG Lite Pingpong example for Cortex-M33. The artefacts will be stored in the mcuxsdk-examples-tq/build folder.
```bash
west build mcuxsdk-examples-tq/_boards/tqmba93xxca/examples/multicore_examples/rpmsg_lite_pingpong_rtos_linux/remote --board tqmba93xxca -Dcore_id=cm33 -DBINARY_DIR=mcuxsdk-examples-tq/build -DCUSTOM_BOARD_ROOT="mcuxsdk-examples-tq/_boards" --config=debug --pristine
```

## Running the Demo

1. Prepare the board as described [here](../../../../README.md#board-preparartion)
2. Power on the target board
3. Select the correct device tree (`setenv fdtfile imx93-tqma9352-mba93xxca-rpmsg.dtb`)
4. Boot to U-Boot and load the demo (e.g. via USB)
    ```
    => load usb 0:1 ${loadaddr} /rpmsg_lite_pingpong_rtos_linux_remote.bin
    => cp.b ${loadaddr} 0x201e0000 ${filesize}
    => bootaux 0x1ffe0000 0
    ```
5. Append `clk-imx93.mcore_booted=1` to the `bootargs` environment variable.
   `setenv bootargs '${bootargs} clk-imx93.mcore_booted=1'`
6. Boot to Linux.
7. After login, make sure `imx_rpmsg_pingpong` kernel module is inserted (`lsmod`) or insert it (`modprobe imx_rpmsg_pingpong`).

## Demo Output

After the boot process succeeds, the ARM Cortex-M33 terminal displays the following information:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
RPMSG Ping-Pong FreeRTOS RTOS API Demo...
RPMSG Share Base Addr is 0xa4010000
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
During the Kernel boot sequence, the ARM Cortex-M33 terminal displays the following information:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Link is up!
Nameservice announce sent.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
After the Linux RPMsg pingpong module was installed, the ARM Cortex-M33 terminal displays the following information:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Waiting for ping...
Sending pong...
Waiting for ping...
Sending pong...
Waiting for ping...
Sending pong...
......
Waiting for ping...
Sending pong...
Ping pong done, deinitializing...
Looping forever...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The Cortex-A terminal displays the following information: see it via `dmesg | tail -n 75`
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
imx_rpmsg_pingpong virtio1.rpmsg-openamp-demo-channel.-1.30: new channel: 0x400 -> 0x1e!
get 1 (src: 0x1e)
get 3 (src: 0x1e)
......
get 99 (src: 0x1e)
get 101 (src: 0x1e)
imx_rpmsg_pingpong virtio1.rpmsg-openamp-demo-channel.-1.30: goodbye!
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
