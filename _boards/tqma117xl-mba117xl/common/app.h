//*****************************************************************************
/*!
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * Copyright (c) 2021-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
 * D-82229 Seefeld, Germany.
 * Author: Isaac Lucas de Lima Yuki
 */
//******************************************************************************

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * SNVS RTC defines
 ******************************************************************************/

/* The RTC is in the Secure Non-Volatile Storage (SNVS) domain */

#define kCLOCK_SnvsHp0          kCLOCK_SnvsHp
#define EXAMPLE_SNVS_IRQn       SNVS_HP_NON_TZ_IRQn
#define EXAMPLE_SNVS_IRQHandler SNVS_HP_NON_TZ_IRQHandler

/*******************************************************************************
 * Flex CAN defines
 ******************************************************************************/
 
#define EXAMPLE_CAN                   CAN3
#define EXAMPLE_FLEXCAN_IRQn          CAN3_IRQn
#define EXAMPLE_FLEXCAN_RxWarningIRQn CAN3_IRQn
#define EXAMPLE_FLEXCAN_BusOffIRQn    CAN3_IRQn
#define EXAMPLE_FLEXCAN_ErrorIRQn     CAN3_IRQn
#define EXAMPLE_FLEXCAN_MBIRQn        CAN3_IRQn
#define EXAMPLE_FLEXCAN_IRQHandler    CAN3_IRQHandler

#define TX_MESSAGE_BUFFER_NUM      (8)
#define RX_MESSAGE_BUFFER_NUM      (9)
#define RX_QUEUE_BUFFER_BASE       (1U)
#define RX_QUEUE_BUFFER_SIZE       (4U)

/* Select OSC24Mhz as master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master flexcan clock source */
#define FLEXCAN_CLOCK_SOURCE_DIVIDER (1U)
/*
 * Get frequency of flexcan clock used in FLEXCAN_FDCalculateImprovedTimingValues() function 
 * in file flexcan_ping_pong_buffer_transfer.c that runs the demo.
 */
#define EXAMPLE_CAN_CLK_FREQ ((CLOCK_GetRootClockFreq(kCLOCK_Root_Can3) / 100000U) * 100000U)
/* Set USE_IMPROVED_TIMING_CONFIG macro to use api to calculates the improved CAN / CAN FD timing values. */
#define USE_IMPROVED_TIMING_CONFIG (1U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void BOARD_startupHook(void);
extern void BOARD_InitHardware(void);

#endif /* _APP_H_ */
