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

#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define RPMSG_LITE_LINK_ID            (RL_PLATFORM_IMX93_M33_A55_USER_LINK_ID)
#define RPMSG_LITE_SHMEM_BASE         (VDEV1_VRING_BASE)
#define RPMSG_LITE_NS_ANNOUNCE_STRING "rpmsg-openamp-demo-channel"
#define RPMSG_LITE_MASTER_IS_LINUX

#define APP_DEBUG_UART_BAUDRATE (115200U) /* Debug console baud rate. */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
