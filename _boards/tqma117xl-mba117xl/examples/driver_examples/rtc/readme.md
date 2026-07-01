# RTC Demo

## Overview

The RTC project is a simple demonstration program of the SDK RTC driver. It sets up the RTC
hardware block to trigger an alarm after a user specified time period. The test will set the current
date and time to a predefined value. The alarm will be set with reference to this predefined date
and time.

## Demo Output

The log below shows the output of the canfd_loopback demo in the Cortex-M7 terminal:

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
