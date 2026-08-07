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

#include "eeprom_m24cxx.h"

/*******************************************************************************
 * Global Functions
 ******************************************************************************/

/*!
 * @brief Read one or more bytes from the M24CXX.
 *
 * Performs an I2C transfer using the MCUXpresso LPI2C driver.
 * The function sends the address and then reads the requested number of bytes.
 *
 * @param eeprom  Pointer to the M24CXX device instance.
 * @param addr    First address to read.
 * @param data    Pointer to receive buffer.
 * @param length  Number of bytes to read.
 *
 * @retval kStatus_Success Read completed successfully.
 * @retval Other           LPI2C driver error code.
 */
status_t M24CXX_Read(const M24CXX_Type *const eeprom, const uint16_t addr, uint8_t *const data, const uint16_t length)
{
    if ((addr >= eeprom->eepromSizeTotal) || ((addr + length) >= eeprom->eepromSizeTotal))
    {
        return kStatus_OutOfRange;
    }

    lpi2c_master_transfer_t xfer;
    xfer.slaveAddress   = eeprom->eepromAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = addr;
    xfer.subaddressSize = 2;
    xfer.data           = data;
    xfer.dataSize       = length;
    xfer.flags          = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(eeprom->base, &xfer);
}

/*!
 * @brief Write one or more bytes to the M24CXX.
 *
 * Performs an I2C transfer using the MCUXpresso LPI2C driver.
 * The register address is sent first followed by the data bytes.
 *
 * @param eeprom  Pointer to the M24CXX device instance.
 * @param addr    First address to write.
 * @param data    Pointer to transmit buffer.
 * @param length  Number of bytes to write.
 *
 * @retval kStatus_Success Write completed successfully.
 * @retval Other           LPI2C driver error code.
 */
static status_t M24CXX_WritePage(const M24CXX_Type *const eeprom, const uint16_t addr, uint8_t *const data, const uint16_t length)
{
    lpi2c_master_transfer_t xfer;
    xfer.slaveAddress   = eeprom->eepromAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = addr;
    xfer.subaddressSize = 2;
    xfer.data           = data;
    xfer.dataSize       = length;
    xfer.flags          = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(eeprom->base, &xfer);
}

static uint16_t M24CXX_Get_Length_To_Write(uint16_t offset, uint16_t length, uint16_t pagesize)
{
    uint16_t pageOffset = offset & (pagesize - 1);
    uint16_t maxLen = pagesize - pageOffset;

    if (length > maxLen)
    {
        length = maxLen;
    }
    return length;
}

/*!
 * @brief Write Multiple Bytes to M24CXX.
 *
 * This function write multiple bytes across page boundry. It can write more than
 * one page at a time. It can also write to any offset non-alligned with page boundry.
 * The write operation takes care of page and offset handling.
 * Writing bytes past the page boundry automatically gets written on the next page.
 *
 * @param eeprom  Pointer to the M24CXX device instance.
 * @param addr    The offset address where to write.
 * @param data    Pointer to data buffer.
 * @param length  Number of data bytes to be written.
 *
 * @retval kStatus_Success Read completed successfully.
 * @retval Other           LPI2C driver error code.
 */
status_t M24CXX_Write(const M24CXX_Type *const eeprom, uint16_t addr, const uint8_t *data, uint16_t length)
{
    status_t status;

    if ((addr >= eeprom->eepromSizeTotal) || ((addr + length) >= eeprom->eepromSizeTotal))
    {
        return kStatus_OutOfRange;
    }

    while (length > 0)
    {
        uint16_t writeSize = M24CXX_Get_Length_To_Write(addr, length, eeprom->eepromPageSize);
        status = M24CXX_WritePage(eeprom, addr, (uint8_t *) data, writeSize);
        if (status != kStatus_Success)
        {
            return status;
        }
        addr += writeSize;
        data += writeSize;
        length -= writeSize;
        SDK_DelayAtLeastUs(10000, SystemCoreClock);
    }
    return status;
}
