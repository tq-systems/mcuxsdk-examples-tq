<!-- 
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# RTC Demo

## Overview

This RTC example demonstrates the SDK's RTC driver.
The test initializes the RTC with a predefined date and time and then configures the hardware block to trigger an alarm after a user-specified time period.

## Demo Output

The log below shows the output of the internal RTC demo in the Cortex-M7 terminal:

```
TQ-Systems GmbH
Version:
Branch: tqma117xl-flexcan-loopback
This is a demo application for the TQMa1176L-MBa117xL device.
MCUX SDK version: 2026.06.00-pvw1
SNVS HP example:
Set up time to wake up an alarm.
Current datetime: 2014-12-25 19:00:00
Please input the number of second to wait for alarm and press enter
The second must be positive value
5
Alarm will occur at: 2014-12-25 19:00:22

Alarm occurs !!!! Current datetime: 2014-12-25 19:00:22
Please input the number of second to wait for alarm and press enter
The second must be positive value
```
