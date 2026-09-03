//******************************************************************************
/*!
 * SPDX-License-Identifier: BSD-3-Clause
 * 
 * Copyright (c) 2026 TQ-Systems GmbH <license@tq-group.com>,
 * D-82229 Seefeld, Germany.
 * Author: Mohammad Tawsif Ahmad Ansari
 */
//******************************************************************************

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "app.h"
#include "board.h"
#include "fsl_debug_console.h"
#include <stdlib.h>
#include <stdio.h>
#include "temp_sensor_se97b.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* SE97B Temperature Sensor I2C Slave Address */
#define SE97B_TEMP_SENSOR_I2C_ADDR   0x1B

#define I2C_MASTER                   ((LPI2C_Type *)I2C_MASTER_BASE)
#define I2C_MASTER_BASE              (LPI2C6_BASE)
#define LPI2C_BAUDRATE               100000U
#define LPI2C_MASTER_CLOCK_FREQUENCY BOARD_BOOTCLOCKRUN_LPI2C6_CLK_ROOT

typedef struct HW_Reg
{
    uint8_t index;
    const char* name;
} HW_Reg_Type;

static const HW_Reg_Type se97b_registers[] =
{
    { SE97B_CAPABILITY_REG, "CAPABILITY" },
    { SE97B_CONFIGURATION_REG, "CONFIGURATION" },
    { SE97B_UPPER_ALARM_REG, "UPPER ALARM" },
    { SE97B_LOWER_ALARM_REG, "LOWER ALARM" },
    { SE97B_CRITICAL_TEMP_REG, "CRITICAL TEMPERATURE" },
    { SE97B_TEMP_REG, "TEMPERATURE" },
    { SE97B_MANUFACTURER_ID_REG, "MANUFACTURER ID" },
    { SE97B_DEVICE_ID_REG, "DEVICE ID" },
    { SE97B_SMBUS_REG, "SMBUS" }
};

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Read and print the register.
 *
 * This function reads and prints the value of register. If there is error then error is also printed.
 *
 * @param sensor         Pointer to the SE97B device instance.
 * @param reg            Pointer to the Address of Register data.
 *
 * @retval None
 */
static void SE97B_Sensor_PrintReg(const SE97B_SENSOR_Type *const sensor, const HW_Reg_Type *const reg)
{
    uint16_t dataBuffer;
    status_t status = SE97B_Sensor_ReadReg(sensor, reg->index, &dataBuffer);
    if (status == kStatus_Success)
    {
        PRINTF("Register Read Success. %s: 0x%04x\n", reg->name, dataBuffer);
    }
    else
    {
        PRINTF("Register %s Read Failed. ERROR: %u\n", reg->name, status);
    }
}

 /*!
 * \brief  This is the main application entry point where system is
 * initialized and all system components are started.
 */
int main(void)
{
    status_t status;
    int32_t milliDegreeCelsius = 0;
    SE97B_SENSOR_Type tempSensor;
    tempSensor.base = LPI2C6; /* SE97B is connected to I2C bus 6 */
    tempSensor.sensorAddress = SE97B_TEMP_SENSOR_I2C_ADDR;
    lpi2c_master_config_t masterConfig;
    BOARD_InitHardware();
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = LPI2C_BAUDRATE;
    LPI2C_MasterInit(I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

    PRINTF("This is a SE97B Temperature Sensor Demo. Press a key to start.\n\n");
    GETCHAR();

    PRINTF("----------------- READ REGISTER VALUES -----------------\n");
    for (int i = 0; i < ARRAY_SIZE(se97b_registers); i++)
    {
        SE97B_Sensor_PrintReg(&tempSensor, &se97b_registers[i]);
    }
    PRINTF("--------------------------------------------------------\n\n");

    while (1)
    {
        status = SE97B_Sensor_Get_Temp_Milli_Degree_Celsius(&tempSensor, &milliDegreeCelsius);
        if (status != kStatus_Success)
        {
            PRINTF("ERROR STATUS: Read Failed %u \n", status);
            return EXIT_FAILURE;
        }
        PRINTF("Current Temperature is %i.%i °C.\n", milliDegreeCelsius / 1000, abs(milliDegreeCelsius % 1000));
        GETCHAR();
    }
    return EXIT_SUCCESS;
}
