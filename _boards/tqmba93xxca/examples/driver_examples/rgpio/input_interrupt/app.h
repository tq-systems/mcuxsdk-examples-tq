/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
 * D-82229 Seefeld, Germany.
 * Author: Markus Niebel
 */
 
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define BOARD_SW_GPIO        GPIO2
#define BOARD_SW_GPIO_PIN    0U
#define BOARD_SW_IRQ         GPIO2_0_IRQn
#define BOARD_SW_IRQ_HANDLER Reserved73_IRQHandler /* GPIO handler */
#define BOARD_SW_NAME        "X1:27 (GPIO2_IO0)"

#define BOARD_SW_INT_OUTPUT kRGPIO_InterruptOutput0

#define EXAMPLE_GPIO_CLOCK_ROOT kCLOCK_Root_BusWakeup
#define EXAMPLE_GPIO_CLOCK_GATE kCLOCK_Gpio2
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
