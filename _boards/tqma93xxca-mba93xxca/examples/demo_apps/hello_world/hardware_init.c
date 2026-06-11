/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
 * D-82229 Seefeld, Germany.
 * Author: Isaac Lucas de Lima Yuki
 */

#include "board.h"
#include "pin_mux.h"

/***********************************************************************************************************************
 * Code
 **********************************************************************************************************************/

void BOARD_InitHardware(void)
{
  BOARD_InitBootPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
}
