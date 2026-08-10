<!--
SPDX-License-Identifier: CC-BY-4.0

Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# SE97B Temperature Sensor Demo

## Overview

This temperature sensor example demonstrates how to read the onboard SE97B temperature sensor.
The demo initializes the I2C bus with a bus speed of 100 kHz and allows the user to read the current temperature
from the onboard SE97B sensor.

The demo runs in an interactive loop where the current temperature is read each time a key is pressed.

## Demo Output

The log below shows the output of the SE97B Temperature Sensor Demo in the Cortex-M7 terminal:

```
TQ-Systems GmbH
Version:  
Branch: tqma117x_SE97B_temp_sensor 
This is a demo application for the TQMa1176L-MBa117xL device.
This is a SE97B Temperature Sensor Demo. Press a key to start.

----------------- READ REGISTER VALUES -----------------
Register Read Success. CAPABILITY: 0x00f7
Register Read Success. CONFIGURATION: 0x0000
Register Read Success. UPPER ALARM: 0x0000
Register Read Success. LOWER ALARM: 0x0000
Register Read Success. CRITICAL TEMPERATURE: 0x0000
Register Read Success. TEMPERATURE: 0xc268
Register Read Success. MANUFACTURER ID: 0x1131
Register Read Success. DEVICE ID: 0xa203
Register Read Success. SMBUS: 0x0031
--------------------------------------------------------

Current Temperature is 38.750 °C.
Current Temperature is 38.375 °C.
Current Temperature is 38.500 °C.
Current Temperature is 38.500 °C.
Current Temperature is 38.500 °C.
Current Temperature is 38.500 °C.
Current Temperature is 38.500 °C.
```
