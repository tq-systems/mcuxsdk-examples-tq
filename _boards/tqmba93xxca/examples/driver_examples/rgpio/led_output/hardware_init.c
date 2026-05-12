/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * Copyright (c) 2025-2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
 * D-82229 Seefeld, Germany.
 * Author: Markus Niebel
 */

/*${header:start}*/
#include "app.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_rgpio.h"
/*${header:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    /* clang-format off */

    const clock_root_config_t rgpioClkCfg = {
        .clockOff = false,
        .mux = 0, // 24Mhz Mcore root buswake clock
        .div = 1
    };
    /* clang-format on */
    BOARD_InitBootPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    CLOCK_SetRootClock(EXAMPLE_GPIO_CLOCK_ROOT, &rgpioClkCfg);
    CLOCK_EnableClock(EXAMPLE_GPIO_CLOCK_GATE);
    CLOCK_EnableClock(kCLOCK_Gpio2);
    /* Set PCNS/ICNS register value to 0x0 to prepare the RGPIO initialization */
    BOARD_LED_RGPIO->PCNS = 0x0;
    BOARD_LED_RGPIO->ICNS = 0x0;

#if 0
    /* Workaround: Disable interrupt which might be enabled by ROM. */
    RGPIO_SetPinInterruptConfig(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN,
                                kRGPIO_InterruptOutput0,
                                kRGPIO_InterruptOrDMADisabled);
    NVIC_ClearPendingIRQ(BOARD_SW_IRQ);
#endif
}
/*${function:end}*/
