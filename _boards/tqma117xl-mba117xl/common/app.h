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
 * Prototypes
 ******************************************************************************/

extern void BOARD_startupHook(void);
extern void BOARD_InitHardware(void);

#endif /* _APP_H_ */
