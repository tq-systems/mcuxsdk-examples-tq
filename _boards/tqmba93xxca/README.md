# MBa9xxxCA.XX Board

## Overview

This README provides a comprehensive guide for using the MBa9xxx board with the TQMa93xxXX module
using the Cortex-M33. It includes several demo applications that showcase the capabilities of the
board and the MCUXpresso SDK software. The demos range from simple sanity checks like the
"Hello World" demo to more complex demonstrations of inter-core communication in a multicore system.

## Trivial Hardware Requirements

| Hardware           | Description                                                                           | Pin Reference |
| ------------------ | ------------------------------------------------------------------------------------- | ------------- |
| Micro USB cable    | Required for connecting the board to your computer for programming and communication. | X22           |
| J-Link Debug Probe | Used for debugging and programming the board.                                         | X4            |
| Personal Computer  | Required for programming and interacting with the board.                              | -             |
| Power Supply       | A power supply with a voltage of 24V is required to power the board.                  | X23           |

Please note that these requirements are specific to the MBa9xxx board using the TQMa93xxXX module.
Always refer to the specific READMEs of the demo you are running for any additional hardware
requirements. Further information can be found in the documentation directory.

## Serial port configuration

- Serial terminal has the following settings:
  - 115200 baud rate
  - 8 data bits
  - No parity
  - One stop bit
  - No flow control
  - Select the COM interface for the two cores.

## Build

In order to build the example for this board use the following command:

```bash
west build --board <mcuxsdk-examples-tq-repo>/_boards/tqmba93xxca/<app_location> \
-Dcore_id=cm33 --build-dir <mcuxsdk-examples-tq-repo>/build \
-DCUSTOM_BOARD_ROOT="<mcuxsdk-examples-tq-repo>/_boards" \
--config=lpa_ram --pristine --target guiconfig
```

> [!NOTE]
> The `config` must be set to the desired linking location. `lpa_ram` is examplary for
> linking to RAM.

## Booting

Refer to [GDB-Server](#gdb-server) or follow the instructions for [VS-Code debugging](#debugging) to load the targets
after setting up the proper booting configuration.

### Booting from Internal RAM

Load the desired target `.bin` file into address 0x00 or use a `.elf` file linked directly to the internal RAM.

Set the dip switch as follows:

| S-1  | S-2  | S-3  | S-4  |
| ---- | ---- | ---- | ---- |
| OFF  | OFF  | OFF  | ON   |

## Demos

The demos that are included are listed as directories in the examples folder.

Each demo has its own README that provides more detailed information about the demo, including its
purpose, how to prepare for the demo, and what to expect when running the demo.

## Debugging

Before debugging, specify the appropriate device configuration in the [launch.json](../templates/launch.json) file.
Refer to the [launch.json](../templates/launch.json) example provided. Follow the instructions on [debugging with VS-Code](../README.md/#debugging-with-vs-code).

## GDB-Server

Use GDB for independent loading and debugging targets from your IDE:

- Start GDB server.
- Select the connection via USB.
- Select device `MIMX9352_M33` and set the endian to `Little Endian`.
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

## License

SPDX-License-Identifier: BSD-3-Clause

For Documentation:
SPDX-License-Identifier: CC-BY-4.0 OR BSD-3-Clause

Copyright of this File:
Copyright 2025 TQ-Systems GmbH <oss@ew.tq-group.com>, D-82229 Seefeld, Germany.
Author: Isaac L. L. Yuki
