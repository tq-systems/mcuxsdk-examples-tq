<!--
SPDX-License-Identifier: CC-BY-4.0

Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# Hello World Demo

## Overview

This 'hello_world' demo application serves as a sanity check for new SDK build environments and board bring-up.
This demo prints the "hello world." string to the terminal using the SDK UART drivers. The purpose of this demo is to
demonstrate the use of the UART and provide a simple project for debugging and further development.

A guide for building and running demos can be found [here](../../../README.md#building-demos-for-tqma117xl).

## Demo Output

The log below shows the output of the Hello World demo in the terminal window:

``` txt
TQ-Systems GmbH
Version:
Branch: main
This is a demo application for the TQMa1176L-MBa117xL device.
MCUX SDK version: 2026.06.00-pvw1
hello world.
```

Any given characters on the Cortex-M7 terminal will get echoed back.
