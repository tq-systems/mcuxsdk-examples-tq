/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2022 NXP
 *
 * Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
 * D-82229 Seefeld, Germany.
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
void BOARD_InitPins(void) {                                /*!< Function assigned for the core: Cortex-M7F[m7] */
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA6_UART3_TX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA6_UART3_TX,
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PE_MASK);
    IOMUXC_SetPinMux(IOMUXC_SD1_DATA7_UART3_RX, 0U);
    IOMUXC_SetPinConfig(IOMUXC_SD1_DATA7_UART3_RX,
                        IOMUXC_SW_PAD_CTL_PAD_PUE_MASK |
                        IOMUXC_SW_PAD_CTL_PAD_PE_MASK);
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
