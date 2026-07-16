<!-- 
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany. 
-->

# MBa117xL Board Setup

## Overview

This README provides a comprehensive guide for using the MBa117xL board with the TQMa1176L module using the Cortex-M7
and the Cortex-M4. It includes several demo applications that showcase the capabilities of the board and the MCUXpresso
SDK software. The demos range from simple sanity checks like the "Hello World" demo to more complex demonstrations.

## Software Requirements

- MCUXpresso [Secure Provisioning Tool](https://www.nxp.com/design/design-center/software/development-software/mcuxpresso-software-and-tools-/mcuxpresso-secure-provisioning-tool:MCUXPRESSO-SECURE-PROVISIONING#downloads), used for writing a demo to the SPI flash.

## Hardware Requirements

| Hardware           | Description                                                                           | Pin Reference |
| ------------------ | ------------------------------------------------------------------------------------- | ------------- |
| Micro USB Cable    | Required for connecting the board to your computer for programming and communication. | X1            |
| J-Link Debug Probe | Used for debugging and programming the board.                                         | X48           |
| Personal Computer  | Required for programming and interacting with the board.                              | -             |
| Power Supply       | A power supply with a voltage of 24V is required to power the board.                  | X13           |

Please note that these requirements are specific to the MBa117xL board using the TQMa1176L module. Always refer to the
specific READMEs of the demo you are running for any additional requirements.

## Board Preparation

1.  Connect 24V power supply at X13
2.  Connect a USB cable between the host PC and the X1 USB port on the target board.
3.  Open a serial terminal for the Cortex-M7 with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control

4.  Set the board to Serial downloader mode by setting the DIP-Switches on S6:
    | S-1  | S-2  | S-3  | S-4  |
    | ---- | ---- | ---- | ---- |
    | ON   | OFF  | ON   | ON   |

    The DIP-Switches S3-S5 can all be turned OFF

## Building Demos for TQMa117xL

In order to build a demo for TQMa117xL use the following command:

```bash
west build mcuxsdk-examples-tq/_boards/tqma117xl-mba117xl/<app_location> --board tqma117xl-mba117xl -Dcore_id=cm7 -DBINARY_DIR=mcuxsdk-examples-tq/build -DCUSTOM_BOARD_ROOT="mcuxsdk-examples-tq/_boards" --config=<configuration> --pristine
```

Available build configurations are:

|   Build Configuration       | Description                                                                                                                             |
| --------------------------- | --------------------------------------------------------------------------------------------------------------------------------------- |
|  debug                      |  Build for SRAM with full debug symbols and no optimization                                                                             |
|  flexspi_nor_debug          |  Build for NOR Flash with full debug symbols and no optimization (XIP possible). Data, Heap, and Stack are allocated in internal SRAM   |
|  flexspi_nor_sdram_debug    |  Build for NOR Flash with full debug symbols and no optimization (XIP possible). Data, Heap, and Stack are offloaded to external SDRAM  |
|  release                    |  Speed- and size-optimized build for internal memory                                                                                    |
|  flexspi_nor_release        |  Speed- and size-optimized build for NOR Flash (XIP possible). Data, Heap, and Stack are allocated in internal SRAM                     |
|  flexspi_nor_sdram_release  |  Speed- and size-optimized build for NOR Flash (XIP possible). Data, Heap, and Stack are offloaded to external SDRAM                    |

## Running and Debugging a Demo

1. Prepare the board as described [here](./README.md#board-preparation)
2. Power on the target board
3. Connect the Segger J-Link Debug Probe between the host PC and X48 of MBa117xL.
4. After the compilation is complete, run the following command to load the file into the board to debug and run it.

```bash
west debug
```

5. Use GDB commands to debug and run the demo.

__Attention__: Debugging via J-Link and GDB requires the demo to be compiled with the `--config=debug` flag.

## Writing a Demo to Flash

It is also possible to write the demo to the flash of TQMa117xL using NXP's Secure Provisioning Tool.
Download the [Secure Provisioning Tool](../../../README.md#software-requirements) and build a demo with the "flexspi_nor_debug" or "flexspi_nor_release" build configuration.

```bash
west build mcuxsdk-examples-tq/_boards/tqma117xl-mba117xl/<app_location> --board tqma117xl-mba117xl -Dcore_id=cm7 -DBINARY_DIR=mcuxsdk-examples-tq/build -DCUSTOM_BOARD_ROOT="mcuxsdk-examples-tq/_boards" --config=flexspi_nor_debug --pristine
```

- Power off the board.
- Set the DIP-Switches into serial downloader mode:

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

- After configuring the boot mode, establish a connection between the host computer and the target hardware via UART1.

----

- Open the Secure Provisioning Tool.
- Open the processor menu in the upper left corner and select the target processor `MIMXRT1176`.
- Select UART as the connection type and set the COM port to the one used for the target hardware connection.
  The default baud rate setting of 115200 can be kept. Check the connection with the "Test Connection" button afterwards.
- Select `FlexSPI NOR - simplified` and set the following values in the opened window:

```
Boot memory configuration parameters
FlexSPI NOR instance: FLEXSPI1

#NOR option0
Device type: QuadSPI SDR NOR
Query pads: 1
Cmd pads: 1
Quad mode setting: Set StatusReg2[1] by cmd 0x31
Misc mode: Disabled
Max frequency: 120MHz
Has option1: Yes
Resulting word: 0xC1000406

#NOR option1
Flash connection: Single port A
Drive strength: 0
DQS pinmux group: 0
Enable second pinmux: Yes
Status override: 0
Dummy cycles: 0
Resulting word: 0x00010000

Memory size [bytes]: 0x10000000
```
__Attention__: Before flashing, make sure the [Fuses are set correctly to support QSPI Boot](./README.md#setting-the-qspi-boot-fuses).

- When the Fuses are set correctly, go back to the `Build image` tab and select the image in `Source executable image`, then click the `Build image` button on the right.
- Go to the `Write image` section and click `Write image` on the right.
- Open a serial terminal, select the correct COM Port and a baud rate of 115200.
- Finally, power off the board and set the DIP-Switches into SPI boot mode:

| S3-1 | S3-2 | S3-3 | S3-4 |
| ---- | ---- | ---- | ---- |
| OFF  | OFF  | OFF  | OFF  |

| S4-1 | S4-2 | S4-3 | S4-4 |
| ---- | ---- | ---- | ---- |
| OFF  | OFF  | OFF  | OFF  |

| S5-1 | S5-2 | S5-3 | S5-4 |
| ---- | ---- | ---- | ---- |
| OFF  | OFF  | OFF  | OFF  |

| S6-1     | S6-2     | S6-3 | S6-4 |
| -------- | -------- | ---- | ---- |
| **OFF**  | **ON**   | ON   | ON   |

When powering on the board now, the serial terminal should print the banner message of the flashed demo.

### Setting the QSPI Boot Fuses

__Attention__: The fuse required for QSPI boot is typically configured by default on standard TQ modules.
This guide describes how to manually program the `fuse 0x9A0` to "1" using the NXP Secure Provisioning Tool. Other fuses are not covered in this guide.

- After configuring processor and UART, open the `Build image` tab right below the target processor in the upper left corner.
- Select `OTP Configuration`.
- The OTP Configuration menu opens and asks to read the current values from the target processor, select "Yes" here.
- If the fuses can be read correctly, the status message "Successfully updated fuses values" is shown in the message
  area at the bottom of the "OTP Configuration" window.
- If something went wrong, please check the connection to the target hardware and try to read the fuses again by
  clicking the "Read" button at the bottom of the window.
- Select the `fuse 0x9A0` from the Boot param section and select the wildcard on bit 10.
- When clicking on the wildcard it changes from "*" to "1". The "Required value" should now be `0x400`.
  **Double-check these values**.
  - __Attention__: **Wrong settings** of `fuse 0x9A0` **lead to bricked target hardware** that can no longer boot
  from the QSPI NOR Flash.

- Select `Advanced Mode` in the bottom left corner.
- Click `Burn` and confirm the action. A success message confirms execution.

## Available Demos

|  Demo                                                                                            |  Description                                                              |
| ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------- |
|  [Hello World](examples/demo_apps/hello_world/README.md)                                         |  Hello World demo, to make sure everything has been set up correctly.     |
|  [LED Blinky](examples/demo_apps/led_blinky/README.md)                                           |  Uses the Cortex-M7 to toggle a green LED on a GPIO pin periodically.     |
|  [RTC Internal](examples/driver_examples/rtc/README.md)                                          |  Demonstrate how to use internal SNVS RTC with Alarm function.            |
|  [RTC External](examples/driver_examples/rtc_external/README.md)                                 |  Demonstrate how to use RTC on I2C bus with Alarm function.               |

## Support Wiki

See [TQ Embedded Wiki for TQMa117xL](https://support.tq-group.com/en/arm/tqma117xl)
