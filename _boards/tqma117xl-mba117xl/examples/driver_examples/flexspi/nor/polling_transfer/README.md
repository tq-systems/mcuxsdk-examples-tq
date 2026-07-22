<!--
SPDX-License-Identifier: CC-BY-4.0

Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# FlexSPI NOR Polling Transfer

## Overview

This FlexSPI NOR polling transfer example shows how to use the FlexSPI driver in polling mode to interact with external NOR flash memory.

The example erases a sector, programs a page with sample data into the flash and finally reads back the data from the flash to verify integrity.

__Attention__: This demo runs only from RAM. Executing code via XIP while reading, writing or erasing from the same NOR flash will cause memory conflicts.
Make sure to use the `debug` and `release` build configurations.

## Demo Output

The log below shows the output of the FlexSPI NOR polling transfer example in the terminal window:

```
TQ-Systems GmbH
Version:
Branch: main
This is a demo application for the TQMa1176L-MBa117xL device.
MCUX SDK version: 2026.06.00

FLEXSPI example started!
Vendor ID: 0xef
Erasing whole chip over FlexSPI...
Erase finished !
Erasing Serial NOR over FlexSPI...
Erase data - successfully.
Program data - successfully.
```
