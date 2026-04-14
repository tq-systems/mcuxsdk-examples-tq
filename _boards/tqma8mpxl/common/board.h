//*****************************************************************************
/*!
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
 * D-82229 Seefeld, Germany.
 * Author: Markus Niebel
 */
//******************************************************************************

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define BOARD_NAME "TQMa8MPxL"

#define BOARD_DOMAIN_ID   (1U)
/* The UART to use for debug messages. */
#define BOARD_DEBUG_UART_TYPE     kSerialPort_Uart
#define BOARD_DEBUG_UART_BAUDRATE (115200U)
#define BOARD_DEBUG_UART_BASEADDR UART3_BASE
#define BOARD_DEBUG_UART_INSTANCE (3U)
#define BOARD_DEBUG_UART_CLK	  kCLOCK_Uart3
#define BOARD_DEBUG_UART_CLK_ROOT kCLOCK_RootUart3
#define BOARD_DEBUG_UART_CLK_FREQ                                                           \
    CLOCK_GetPllFreq(kCLOCK_SystemPll1Ctrl) / (CLOCK_GetRootPreDivider(BOARD_DEBUG_UART_CLK_ROOT)) / \
        (CLOCK_GetRootPostDivider(BOARD_DEBUG_UART_CLK_ROOT)) / 10
#define BOARD_UART_IRQ         UART3_IRQn
#define BOARD_UART_IRQ_HANDLER UART3_IRQHandler

#define BOARD_GPC_BASEADDR GPC
#define BOARD_MU_IRQ       MU1_M7_IRQn

/* Shared memory base for RPMsg communication. */
#define VDEV0_VRING_BASE      (0x55000000U)
#define RESOURCE_TABLE_OFFSET (0xFF000)

#define BOARD_IS_XIP_FLEXSPI()                                                                                 \
    (((uint32_t)BOARD_InitDebugConsole >= 0x08000000U) && ((uint32_t)BOARD_InitDebugConsole < 0x10000000U))

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
/*******************************************************************************
 * API
 ******************************************************************************/
void BOARD_InitDebugConsole(void);
void BOARD_InitMemory(void);
void BOARD_RdcInit(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus */
#endif /* _BOARD_H_ */
