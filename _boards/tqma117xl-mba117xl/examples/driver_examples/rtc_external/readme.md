# External RTC Demo

## Overview

The RTC project is a simple demonstration program for onboard pcf85063a RTC connected to I2C bus 6.
For I2C communication NXP I2C driver is used. It sets up the RTC hardware block to trigger an alarm
after a user specified time period. The test will set the current date and time to a predefined value.
The alarm will be set with reference to this predefined date and time.

## Demo Output

The log below shows the output of the external_rtc demo in the Cortex-M7 terminal:

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
