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

/* SE97B Temperature Sensor Register Definitions  */
#define SE97B_CAPABILITY_REG           0x00
#define SE97B_CONFIGURATION_REG        0x01
#define SE97B_UPPER_ALARM_REG          0x02
#define SE97B_LOWER_ALARM_REG          0x03
#define SE97B_CRITICAL_TEMP_REG        0x04
#define SE97B_TEMP_REG                 0x05
#define SE97B_MANUFACTURER_ID_REG      0x06
#define SE97B_DEVICE_ID_REG            0x07
#define SE97B_SMBUS_REG                0x22
#define REGISTER_SIZE                  0x02
#define NUMBER_OF_REGISTERS            0x09

/*******************************************************************************
 * Structure Definition
 ******************************************************************************/

/**
 * @brief Structure to store base address of I2C bus and slave.
 *
 * This structure contains the base address of I2C bus connected to SE97B
 * and the slave address of the SE97B.
 */
typedef struct se97bSensor
{
    LPI2C_Type *base;   /* I2C bus base address */
    uint8_t sensorAddress;
} SE97B_SENSOR_Type;

/*******************************************************************************
 * API
 ******************************************************************************/

status_t SE97B_Sensor_ReadReg(const SE97B_SENSOR_Type *const sensor, const uint8_t addr, uint16_t *const data);

status_t SE97B_Sensor_WriteReg(const SE97B_SENSOR_Type *const sensor, const uint8_t addr, uint16_t *const data);

status_t SE97B_Sensor_Get_Temp_Milli_Degree_Celsius(const SE97B_SENSOR_Type *const sensor, int32_t *const temperature);
