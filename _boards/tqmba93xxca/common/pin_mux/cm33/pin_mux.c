/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
 * D-82229 Seefeld, Germany.
 *
 * Author: Markus Niebel
 */

#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO14__LPUART3_TX, 0U);
    IOMUXC_SetPinMux(IOMUXC_PAD_GPIO_IO15__LPUART3_RX, 0U);

    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO14__LPUART3_TX,
                        IOMUXC_PAD_DSE(15U) | IOMUXC_PAD_FSEL1(2U));
    IOMUXC_SetPinConfig(IOMUXC_PAD_GPIO_IO15__LPUART3_RX,
                        IOMUXC_PAD_HYS_MASK);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
