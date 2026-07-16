<!-- 
SPDX-License-Identifier: CC-BY-4.0
 
Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# External RTC Demo

## Overview

This RTC example is a simple demonstration program for the onboard PCF85063A RTC connected to I2C bus 6.
For I2C communication, NXP's I2C driver is used. The test initializes the RTC with a predefined date and time and then configures the hardware block to trigger an alarm after a user-specified time period.

## Demo Output

The log below shows the output of the external RTC demo in the Cortex-M7 terminal:

```
TQ-Systems GmbH
Version:
Branch: tqma117xl-rtc-external
This is a demo application for the TQMa1176L-MBa117xL device.

Current RTC value:
Date :  3/ 7/2026
Time : 14:40:13
Weekday : 5


Setting RTC...
RTC updated successfully.
Setting alarm for 14:31:20 every day...
Alarm configured.

RTC after update:
Date :  3/ 7/2026
Time : 14:30:20
Weekday : 5
```

On occurrence of alarm following message will be seen with time stamp.

```
====================================
==            ALARM..!            ==
====================================

Date :  3/ 7/2026
Time : 14:31:20
Weekday : 5
```
