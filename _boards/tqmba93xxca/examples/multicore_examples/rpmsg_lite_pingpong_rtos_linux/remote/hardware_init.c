/*
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * Copyright 2022-2024 NXP
 *
 * Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>, D-82229 Seefeld, Germany.
 * Author: Markus Niebel
 */

/*${header:start}*/
#include "board.h"
#include "pin_mux.h"
#include "rsc_table.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    /* copy resource table to destination address(TCM) */
    copyResourceTable();
}
/*${function:end}*/
