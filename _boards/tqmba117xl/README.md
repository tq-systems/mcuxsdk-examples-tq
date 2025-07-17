# MBa117xL Board

## Table of Contents

[[_TOC_]]

## Overview

This README provides a comprehensive guide for using the MBa117xL board with the TQMBa1176L module using the M4-Cortex
and the M7-Cortex. It includes several demo applications that showcase the capabilities of the board and the MCUXpresso
SDK software. The demos range from simple sanity checks like the "Hello World" demo to more complex demonstrations.

## Software Requirements

- General requirements
- MCUXpresso [Secure Provisioning Tool](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-secure-provisioning-tool:MCUXPRESSO-SECURE-PROVISIONING#downloads) v7

## Hardware Requirements

| Hardware           | Description                                                                           | Pin Reference |
| ------------------ | ------------------------------------------------------------------------------------- | ------------- |
| Micro USB cable    | Required for connecting the board to your computer for programming and communication. | X1            |
| J-Link Debug Probe | Used for debugging and programming the board.                                         | X39           |
| Personal Computer  | Required for programming and interacting with the board.                              | -             |
| Power Supply       | A power supply with a voltage of 24V is required to power the board.                  | X13           |

Please note that these requirements are specific to the MBa117xL board using the TQMBa1176L module. Always refer to the
specific READMEs of the demo you are running for any additional hardware requirements.

## Serial Port Configuration

Configure the serial terminal with the following settings:

- 115200 baud rate
- 8 data bits
- No parity
- One stop bit
- No flow control
- Select the appropriate COM interface for the two cores.

## Booting

Refer to [GDB-Server](#gdb-server) or follow the instructions for [VS-Code debugging](#debugging) to load the targets
after setting up the proper booting configuration.

### Booting from Internal RAM

Load the desired target `.bin` file into address 0x00 or use a `.elf` file linked directly to the internal RAM.

### Booting from Flash

When using J-Link for debugging or firmware loading, ensure the FLEXSPI1 pin multiplexing is correctly configured for the
MBa117xL.

These are the settings for JLink-GDB related programs:

``` json
"-device",
 "MIMXRT1176xxxA_M7?BankAddr=0x30000000&Loader=nCS@AD18_CLK@AD19_D0@AD20_D1@AD21_D2@AD22_D3@AD23&BankAddr=0x60000000&Loader=nCS@SDB100_CLK@SDB101_D0@SDB102_D1@SDB103_D2@SDB104_D3@SDB105"
```

__Attention__: Before booting, it is **mandatory** to set the right [fuses](#setting-fuses).

After setting fuses, set the dip switch as follows:

| S3-1 | S3-2 | S3-3 | S3-4 |
| ---- | ---- | ---- | ---- |
| OFF  | OFF  | OFF  | OFF  |

| S4-1 | S4-2 | S4-3 | S4-4 |
| ---- | ---- | ---- | ---- |
| OFF  | OFF  | OFF  | OFF  |

| S5-1 | S5-2 | S5-3 | S5-4 |
| ---- | ---- | ---- | ---- |
| OFF  | OFF  | OFF  | OFF  |

| S6-1 | S6-2 | S6-3 | S6-4 |
| ---- | ---- | ---- | ---- |
| OFF  | ON   | ON   | ON   |

## Debugging

Before debugging, specify the appropriate device configuration in the [launch.json](../templates/launch.json) file.
Refer to the [launch.json](../templates/launch.json) example provided. Follow the instructions on [debugging with VS-Code](../README.md/#debugging-with-vs-code).

## GDB-Server

Use GDB for independent loading and debugging targets from your IDE:

- Start GDB server.
- Select the connection via USB.
- Select device `MIMXRT1176xxxA_M7` and set the endian to `Little Endian`.
- Select `JTAG` as the interface with a fixed speed of 4000 kHz.
  - __NOTE__: If debugging doesn't work as expected, you can try changing the interface to SWD and/or varying the
  interface speed.
- Select the right flash bank for FlexSPI1:
  - `nCS@AD18_CLK@AD19_D0@AD20_D1@AD21_D2@AD22_D3@AD23&BankAddr=0x60000000&Loader=nCS@SDB100_CLK@SDB101_D0@SDB102_D1@SDB103_D2@SDB104_D3@SDB105`
- Further options are optional.
- Then start GDB via CLI.
- Follow the command for booting:

  ``` bat
  file <PATH.elf>
  target remote localhost:<port>
  monitor reset
  monitor halt
  load
  monitor go
  ```

## Setting Fuses

To boot from the flash device, follow these steps:

- Power off the board.
- Set the dip switch as follows:

| S3-1 | S3-2 | S3-3 | S3-4 |
| ---- | ---- | ---- | ---- |
| OFF  | OFF  | OFF  | OFF  |

| S4-1 | S4-2 | S4-3 | S4-4 |
| ---- | ---- | ---- | ---- |
| OFF  | OFF  | OFF  | OFF  |

| S5-1 | S5-2 | S5-3 | S5-4 |
| ---- | ---- | ---- | ---- |
| OFF  | OFF  | OFF  | OFF  |

| S6-1   | S6-2    | S6-3 | S6-4 |
| ------ | ------- | ---- | ---- |
| **ON** | **OFF** | ON   | ON   |

- After setting up the boot mode, establish a connection between the host computer and the target hardware via UART1.
- Open the [Secure Provisioning Tool](#software-requirements).
- Open the processor menu in the upper left corner and select the target processor `MIMXRT1176`.
- Select UART as the connection type and set the COM port to the one used for the target hardware connection.
  The default baud rate setting of 115200 can be kept. Check the connection with the "Test Connection" button afterward.
- Now open the `Build image` tab right below the target processor in the upper left corner.
- Select `OTP Configuration`.
- The OTP Configuration menu opens and asks to read the current values from the target processor, select "Yes" here.
- If the fuses can be read correctly, the status message "Successfully updated fuses values" is shown in the message
  area at the bottom of the "OTP Configuration" window.
- If something went wrong, please check the connection to the target hardware and try to read the fuses again by
  clicking the "Read" button at the window bottom.
- Now select the `fuse 0x9A0` from the Boot param section and select the wildcard on bit 10.
- When clicking on the wildcard it changes from "*" to "1". The "Required value" should now be `0x400`.
  **Double-check these values**.
  - __ATTENTION__: A **wrong setting** on `fuse 0x9A0` **leads to bricked target hardware** that can no longer boot
  from the QSPI NOR Flash.

- Select `Advanced Mode` in the bottom left corner.
- Now click on `Burn`. Confirm.
- You should have got a success message.

## License

SPDX-License-Identifier: BSD-3-Clause

For Documentation:
SPDX-License-Identifier: CC-BY-4.0 OR BSD-3-Clause

Copyright of this File:
Copyright 2025 TQ-Systems GmbH <oss@ew.tq-group.com>, D-82229 Seefeld, Germany.
Author: Isaac L. L. Yuki
