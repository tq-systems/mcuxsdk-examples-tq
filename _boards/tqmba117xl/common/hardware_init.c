//*****************************************************************************
/*!
 * \copyright
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * \copyright
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * \copyright
 * Copyright (c) 2021-2025 TQ-Systems GmbH <oss@ew.tq-group.com>,
 * D-82229 Seefeld, Germany.
 * Author: Isaac L. L. Yuki
 */
//******************************************************************************

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

__attribute__((weak)) void BOARD_startupHook(void) {}

void BOARD_InitHardware(void)
{
  BOARD_Initialize();
  BOARD_startupHook();
}
