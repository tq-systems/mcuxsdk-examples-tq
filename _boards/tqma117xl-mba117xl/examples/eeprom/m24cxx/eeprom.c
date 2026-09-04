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
#include "eeprom_m24cxx.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define DATA_BUFFER_SIZE             500
#define I2C_MASTER                   ((LPI2C_Type *)I2C_MASTER_BASE)
#define I2C_MASTER_BASE              (LPI2C6_BASE)
#define LPI2C_BAUDRATE               100000U
#define LPI2C_MASTER_CLOCK_FREQUENCY BOARD_BOOTCLOCKRUN_LPI2C6_CLK_ROOT

#define M24C64_I2C_ADDR              0x54
#define EEPROM_M24C64_SIZE           8192
#define EEPROM_M24C64_PAGE_COUNT     256
#define EEPROM_M24C64_PAGE_SIZE      (EEPROM_M24C64_SIZE / EEPROM_M24C64_PAGE_COUNT)

#define READ_WRITE_SIZE_BYTES        200 /* Number of bytes to be written and read from EEPROM */
#define READ_WRITE_OFFSET_ADDRESS    500 /* Offset adddress to read and write from EEPROM */

/*******************************************************************************
 * Variables
 ******************************************************************************/

static uint8_t writeBuffer[DATA_BUFFER_SIZE];
static uint8_t readBuffer[DATA_BUFFER_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/

 /*!
 * \brief  This is the main application entry point where system is
 * initialized and all system components are started.
 */
int main(void)
{
    status_t status;
    uint16_t err = 0;

    M24CXX_Type eeprom;
    eeprom.base = LPI2C6; /* EEPROM is connected to I2C bus 6 */
    /* EEPROM is 24C64 from ST. It is 8 KB in size */
    eeprom.eepromAddress = M24C64_I2C_ADDR; /* EEPROM Slave address */
    eeprom.eepromSizeTotal = EEPROM_M24C64_SIZE;
    eeprom.eepromPageSize = EEPROM_M24C64_PAGE_SIZE;

    lpi2c_master_config_t masterConfig;
    BOARD_InitHardware();
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = LPI2C_BAUDRATE;
    LPI2C_MasterInit(I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

    PRINTF("This is a EEPROM M24C64 Demo.\n");

    for (int i = 0; i < READ_WRITE_SIZE_BYTES; ++i)
    {
        writeBuffer[i] = i;
    }

    PRINTF("Writing %u bytes at offset %u.\n", READ_WRITE_SIZE_BYTES, READ_WRITE_OFFSET_ADDRESS);
    status = M24CXX_Write(&eeprom, READ_WRITE_OFFSET_ADDRESS, (uint8_t *) writeBuffer, READ_WRITE_SIZE_BYTES);
    if (status != kStatus_Success)
    {
        PRINTF("WRITE ERROR STATUS: %u \n", status);
        err++;
    }

    PRINTF("Reading %u bytes from offset %u.\n", READ_WRITE_SIZE_BYTES, READ_WRITE_OFFSET_ADDRESS);
    status = M24CXX_Read(&eeprom, READ_WRITE_OFFSET_ADDRESS, (uint8_t *) readBuffer, READ_WRITE_SIZE_BYTES);
    if (status != kStatus_Success)
    {
        PRINTF("READ ERROR STATUS: %u \n", status);
        err++;
    }

    PRINTF("Comparing read and write %u bytes from offset %u.\n", READ_WRITE_SIZE_BYTES, READ_WRITE_OFFSET_ADDRESS);
    for (int i = 0; i < READ_WRITE_SIZE_BYTES; ++i)
    {
        if (writeBuffer[i] != readBuffer[i])
        {
            err++;
        }
    }

    if (err == 0)
    {
        PRINTF("Read and write %u bytes from offset %u match! Demo Success.\n", READ_WRITE_SIZE_BYTES, READ_WRITE_OFFSET_ADDRESS);
        return EXIT_SUCCESS;
    }
    else
    {
        PRINTF("Demo Failed! %u Errors Found\n", err);
        return EXIT_FAILURE;
    }
}
