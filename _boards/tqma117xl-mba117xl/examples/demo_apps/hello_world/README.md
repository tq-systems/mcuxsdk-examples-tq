# hello_world

## Overview

The 'hello_world' demo application serves as a sanity check for new SDK build environments and board bring-up.
This demo prints the "Hello World" string to the terminal using the SDK UART drivers. The purpose of this demo is to
demonstrate the use of the UART and provide a simple project for debugging and further development.

## Demo Preparation

1. Connect the J-Link Debug Probe to your board.
2. Connect a USB cable between the host PC and the J-Link-debugger.
3. Connect a USB-Cable between the host PC and the board.
4. Open two serial terminal with the settings specified for the board.
5. Download the program to the target board.
6. Launch the debugger in your IDE to begin running the demo.

## Running the Demo

The log below shows the output of the Hello World demo in the terminal window:

``` txt
hello world.
```

## License

SPDX-License-Identifier: BSD-3-Clause

For Documentation:
SPDX-License-Identifier: CC-BY-4.0 OR BSD-3-Clause

Copyright of this File:
Copyright 2025 TQ-Systems GmbH <oss@ew.tq-group.com>, D-82229 Seefeld, Germany.
