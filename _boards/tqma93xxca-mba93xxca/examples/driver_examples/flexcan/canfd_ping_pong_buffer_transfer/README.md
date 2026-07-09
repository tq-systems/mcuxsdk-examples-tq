<!--
SPDX-License-Identifier: CC-BY-4.0

Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
D-82229 Seefeld, Germany.
-->

# CANFD Ping Pong Demo

## Overview

The CANFD ping pong buffer transfer example shows how to use the FlexCAN queue feature to create
two simulated FIFOs that can receive CANFD frames.

In this example, two boards are connected through the CAN bus. Endpoint A (Board A) sends CANFD messages to
endpoint B (Board B) when the user inputs the number of CAN messages to be sent in the terminal. Endpoint B uses
two receiving queues to receive messages and then prints the message content and the receiving queue
number to the terminal after any queue is full.

This example enables the FlexCAN Individual RX Masking and Queue features. This configuration forces
FlexCAN to search for the first free-to-receive matched Rx Message Buffer (MB) and the last
non-free-to-receive Rx MB. Otherwise, the matching process would default to the first matched Rx MB.

Node A transmits frames with the ID `0x321`. Node B's message buffer ID is set to `0x21` (which
normally only matches ID `0x21` frames).
However, Node B's message buffer is masked by `0xFF` in the Receive Individual Mask register, which means, Node B will also receive ID `0x321` frames.

When queue 1 finishes receiving, the application updates queue 1's individual ID mask to `0x7FF`. At this point, it can only receive frames with the ID `0x21`. Therefore, queue 1 ignores any following messages and queue 2 will start to receive frames.
When queue 2 finishes receiving, the application restores queue 1's individual ID mask to `0xFF`, thus allowing it to receive ID `0x321` frames again.

This example also demonstrates how to create a custom FlexCAN IRQ handler.

This example limits the maximum number of CAN messages to be sent each time to 256.

## Hardware Setup

To set up the demo correctly, the following prerequisites are required:

- External board (TQMa93xx[C/L]A or TQMa117xL) containing the CAN FD Ping Pong Demo
- Physical CAN connection between the two boards on X19/CAN3 on the MBa117xL (or X9/CAN2 on MBa93xx[C/L]A)

The demo is now ready to be run. One board needs to be Node A (Transmit) and the other needs to be Node B (Receive).

__Attention__: The CAN Controller must be disabled in the Linux device tree to use this demo.

## Running the Demo

Copy the compiled binary file (canfd_ping_pong_buffer_transfer_cm33.bin) to bootable drive of SD card.
Boot the board up and in UBoot run the following command.

```
load mmc 1:1 0x80000000 canfd_ping_pong_buffer_transfer_cm33.bin; cp.b 0x80000000 0x201e0000 0x30000; bootaux 0x1ffe0000 0
```

## Output on serial terminal of Cortex-M33 as show below for transmitter node A

```
Version:
Branch: tqma93xxxa-can-bus-ping-pong
This is a demo application for the MBa93xxCA device.
MCUX SDK version: 2026.06.00-pvw1
********* FLEXCAN PingPong Buffer Example *********
    Message format: Standard (11 bit id)
    Node B Message buffer 1 to 4 used as Rx queue 1.
    Node B Message buffer 5 to 8 used as Rx queue 2.
    Node A Message buffer 8 used as Tx.
*********************************************

Please select local node as A or B:
Note: Node B should start first.
Node:a
Please input the number of CAN/CANFD messages to be send and end with enter.
4
Transmission done.
```

## Output on serial terminal of Cortex-M33 as show below for reciever node B

```
Version:
Branch: tqma93xxxa-can-bus-ping-pong
This is a demo application for the MBa93xxCA device.
MCUX SDK version: 2026.06.00-pvw1
********* FLEXCAN PingPong Buffer Example *********
    Message format: Standard (11 bit id)
    Node B Message buffer 1 to 4 used as Rx queue 1.
    Node B Message buffer 5 to 8 used as Rx queue 2.
    Node A Message buffer 8 used as Tx.
*********************************************

Please select local node as A or B:
Note: Node B should start first.
Node:b
Start to Wait data from Node A

Read Rx MB from Queue 1.
Rx MB ID: 0x321, Rx MB data: 0x0, Time stamp: 17000
Rx MB ID: 0x321, Rx MB data: 0x1, Time stamp: 17123
Rx MB ID: 0x321, Rx MB data: 0x2, Time stamp: 17246
Rx MB ID: 0x321, Rx MB data: 0x3, Time stamp: 17370
Read Rx MB from Queue 2.
Rx MB ID: 0x321, Rx MB data: 0x4, Time stamp: 29655
Rx MB ID: 0x321, Rx MB data: 0x5, Time stamp: 29777
Rx MB ID: 0x321, Rx MB data: 0x6, Time stamp: 29900
Rx MB ID: 0x321, Rx MB data: 0x7, Time stamp: 30024
Wait Node A to trigger the next 8 messages!
```
