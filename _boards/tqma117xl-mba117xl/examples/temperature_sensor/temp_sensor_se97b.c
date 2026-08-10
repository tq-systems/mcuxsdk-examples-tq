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

#include "temp_sensor_se97b.h"
#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

 /*******************************************************************************
 * Local Functions
 ******************************************************************************/

/*!
 * @brief Check if register address is valid.
 *
 * @param addr        address of the register to validate.
 *
 * @retval true       valid register address.
 * @retval false      not valid register address.
 */
static bool SE97B_Sensor_IsValidReg(uint8_t addr)
{
    if (addr > (NUMBER_OF_REGISTERS - 1) && (addr != SE97B_SMBUS_REG))
    {
        return false;
    }
    else
    {
        return true;
    }
}

/*******************************************************************************
 * Global Functions
 ******************************************************************************/

/*!
 * @brief Write one Register to the SE97B.
 *
 * Performs an I2C transfer using the MCUXpresso LPI2C driver.
 * The register address is sent first followed by the data bytes.
 *
 * @param sensor  Pointer to the SE97B device instance.
 * @param addr    Register address to write.
 * @param data    Pointer to transmit buffer.
 *
 * @retval kStatus_Success Write completed successfully.
 * @retval Other           LPI2C driver error code.
 */
status_t SE97B_Sensor_WriteReg(const SE97B_SENSOR_Type *const sensor, const uint8_t addr, uint16_t *const data)
{
    uint8_t dataBuffer8Bit[REGISTER_SIZE];

    if (SE97B_Sensor_IsValidReg(addr))
    {
        dataBuffer8Bit[0] = (*data >> 8) & 0x00FF;
        dataBuffer8Bit[1] = *data & 0x00FF;

        lpi2c_master_transfer_t xfer;
        xfer.slaveAddress   = sensor->sensorAddress;
        xfer.direction      = kLPI2C_Write;
        xfer.subaddress     = addr;
        xfer.subaddressSize = 1;
        xfer.data           = dataBuffer8Bit;
        xfer.dataSize       = REGISTER_SIZE;
        xfer.flags          = kLPI2C_TransferDefaultFlag;

        return LPI2C_MasterTransferBlocking(sensor->base, &xfer);
    }
    else
    {
        return kStatus_InvalidArgument;
    }
}

/*!
 * @brief Read one Register from the SE97B.
 *
 * Performs an I2C transfer using the MCUXpresso LPI2C driver.
 * The function sends the address and then reads the requested number of bytes.
 *
 * @param sensor  Pointer to the SE97B device instance.
 * @param addr    Address of register to read.
 * @param data    Pointer to receive buffer.
 *
 * @retval kStatus_Success Read completed successfully.
 * @retval Other           LPI2C driver error code.
 */
status_t SE97B_Sensor_ReadReg(const SE97B_SENSOR_Type *const sensor, const uint8_t addr, uint16_t *const data)
{
    uint8_t dataBuffer8Bit[REGISTER_SIZE] = { 0, 0 };
    status_t status;

    if (SE97B_Sensor_IsValidReg(addr))
    {

        lpi2c_master_transfer_t xfer;
        xfer.slaveAddress   = sensor->sensorAddress;
        xfer.direction      = kLPI2C_Read;
        xfer.subaddress     = addr;
        xfer.subaddressSize = 1;
        xfer.data           = dataBuffer8Bit;
        xfer.dataSize       = REGISTER_SIZE;
        xfer.flags          = kLPI2C_TransferDefaultFlag;

        status = LPI2C_MasterTransferBlocking(sensor->base, &xfer);
        if (status == kStatus_Success)
        {
            *data = (dataBuffer8Bit[0] << 8) | dataBuffer8Bit[1];
        }
        return status;
    }
    else
    {
        return kStatus_InvalidArgument;
    }
}

/*!
 * @brief Read Temperature in milli Degree Celsius.
 *
 * This function reads temperature in milli Degree Celsius.
 *
 * @param sensor         Pointer to the SE97B device instance.
 * @param temperature    Pointer to the variable where temperature value is copied.
 *
 * @retval kStatus_Success Read completed successfully.
 * @retval Other           LPI2C driver error code.
 */
status_t SE97B_Sensor_Get_Temp_Milli_Degree_Celsius(const SE97B_SENSOR_Type *const sensor, int32_t *const temperature)
{
    uint16_t temp = 0;
    uint16_t readData;
    status_t status = SE97B_Sensor_ReadReg(sensor, SE97B_TEMP_REG, &readData);
    if (status != kStatus_Success)
    {
        return status;
    }
    temp = readData & 0x0fff; /* Remove the upper 4 flag bits */
    temp = temp >> 1; /* Remove LSB RFU bit and left shift to get temperature value */
    /* SE97B resolution is 0.125 Degree Celsius per bit */
    *temperature = temp * 125; /* Temperature in milli Degree Celsius */
    /* Check 12th flag bit for temperature sign */
    if (readData & 0x1000)
    {
        *temperature = *temperature * -1;
    }
    return status;
}
