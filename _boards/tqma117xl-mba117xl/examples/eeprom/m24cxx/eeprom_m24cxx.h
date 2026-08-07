//*****************************************************************************
/*!
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2026 TQ-Systems GmbH <oss@ew.tq-group.com>,
 * D-82229 Seefeld, Germany.
 * Author: Mohammad Tawsif Ahmad Ansari
 */
//******************************************************************************

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "fsl_lpi2c.h"
#include "stdint.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Structure Definition
 ******************************************************************************/

/**
 * @brief Structure to store base address of I2C bus and slave.
 *
 * This structure contains the base address of I2C bus connected to EEPROM
 * and the slave address of the EEPROM.
 */
typedef struct m24cxx
{
    LPI2C_Type *base; /* I2C bus base address */
    uint8_t eepromAddress; /* EEPROM M24CXX address on the bus */
    uint32_t eepromSizeTotal;
    uint16_t eepromPageSize;
} M24CXX_Type;

/*******************************************************************************
 * API
 ******************************************************************************/

status_t M24CXX_Read(const M24CXX_Type *const eeprom, const uint16_t addr, uint8_t *const data, const uint16_t length);

status_t M24CXX_Write(const M24CXX_Type *const eeprom, uint16_t addr, const uint8_t *data, uint16_t length);
