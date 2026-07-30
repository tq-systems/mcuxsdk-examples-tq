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
#if (defined NOR_FLASH_DEMO) && (NOR_FLASH_DEMO == 1)
#include "fsl_flexspi.h"
#endif

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
 * NOR Flash Flexspi defines
 ******************************************************************************/

#define EXAMPLE_FLEXSPI                 FLEXSPI1
#define FLASH_SIZE                      0x40000 /* 128 Mb W25Q128, Size in KB */
#define EXAMPLE_FLEXSPI_AMBA_BASE       FlexSPI1_AMBA_BASE
#define FLASH_PAGE_SIZE                 256
#define EXAMPLE_SECTOR                  500
#define SECTOR_SIZE                     0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK           kCLOCK_Flexspi1
#define FLASH_PORT                      kFLEXSPI_PortA1
#define EXAMPLE_FLEXSPI_RX_SAMPLE_CLOCK kFLEXSPI_ReadSampleClkLoopbackFromDqsPad

#define CUSTOM_LUT_LENGTH            64U
#define FLASH_QUAD_ENABLE            0x02
#define FLASH_BUSY_STATUS_POL        1U
#define FLASH_BUSY_STATUS_OFFSET     0U

#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD     0
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE        1
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR        2
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD   3
#define NOR_CMD_LUT_SEQ_IDX_ERASECHIP          4
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 5
#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL        6
#define NOR_CMD_LUT_SEQ_IDX_READID             7
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG     8
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG      9
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST          10

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void BOARD_startupHook(void);
extern void BOARD_InitHardware(void);

#if (defined NOR_FLASH_DEMO) && (NOR_FLASH_DEMO == 1)

static inline void flexspi_clock_init(void)
{
  /*Clock setting for flexspi1*/
  CLOCK_SetRootClockDiv(kCLOCK_Root_Flexspi1, 2);
  CLOCK_SetRootClockMux(kCLOCK_Root_Flexspi1, 0);
}

#endif

#endif /* _APP_H_ */
