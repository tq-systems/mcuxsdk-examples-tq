<!--
SPDX-License-Identifier: CC-BY-4.0

Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# M24C64 EEPROM Demo

## Overview

This EEPROM example demonstrates the read and write operations of the onboard M24C64 8 KB EEPROM.
The demo initializes the I2C bus at a speed of 100 kHz and allows the user to read data from or write data to a specific memory address.

The page size of the M24C64 EEPROM is 32 bytes. This demo writes 200 bytes across multiple pages starting at offset 500 in a single operation.
It also reads back all 200 bytes in one operation.

The write operation handles page and offset management. Bytes written past a page boundary are automatically written to the next page.

## Demo Output

The log below shows the output of the M24C64 EEPROM demo in the Cortex-M7 terminal:

```
TQ-Systems GmbH
Version:
Branch: tqma117x_M24C64_eeprom
This is a demo application for the TQMa1176L-MBa117xL device.
This is a EEPROM M24C64 Demo.
Writing 200 bytes at offset 500.
Reading 200 bytes from offset 500.
Comparing read and write 200 bytes from offset 500.
Read and write 200 bytes from offset 500 match! Demo Success.
```
