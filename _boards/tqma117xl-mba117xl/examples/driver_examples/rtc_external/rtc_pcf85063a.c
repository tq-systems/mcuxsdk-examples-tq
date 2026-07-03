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

#include "rtc_pcf85063a.h"
#include "fsl_common.h"

/*******************************************************************************
 * Local Static Functions
 ******************************************************************************/

/*!
 * @brief Convert a BCD value to decimal.
 *
 * The PCF85063A stores time and date registers in Binary Coded Decimal (BCD)
 * format. This function converts a BCD byte into a normal decimal value.
 *
 * @param val BCD encoded value.
 *
 * @return Decimal representation of the input value.
 */
static uint8_t bcd2dec(uint8_t val)
{
    return ((val >> 4U) * 10U) + (val & 0x0FU);
}

/*!
 * @brief Convert a decimal value to BCD.
 *
 * The PCF85063A requires time and date values to be written in Binary
 * Coded Decimal (BCD) format. This function converts a decimal value
 * into BCD.
 *
 * @param val Decimal value.
 *
 * @return BCD representation of the input value.
 */
static uint8_t dec2bcd(uint8_t val)
{
    return ((val / 10U) << 4U) | (val % 10U);
}

/*******************************************************************************
 * Global Functions
 ******************************************************************************/

/*!
 * @brief Read one or more registers from the PCF85063A.
 *
 * Performs an I2C transfer using the MCUXpresso LPI2C driver.
 * The function sends the register address and then reads the
 * requested number of bytes.
 *
 * @param rtc Pointer to the PCF85063A device instance.
 * @param reg     First register address to read.
 * @param data    Pointer to receive buffer.
 * @param length  Number of bytes to read.
 *
 * @retval kStatus_Success Read completed successfully.
 * @retval Other           LPI2C driver error code.
 */
status_t PCF85063A_ReadRegisters(const PCF85063A_Type *const rtc,
                                 const uint8_t reg,
                                 uint8_t *const data,
                                 const uint8_t length)
{
    lpi2c_master_transfer_t xfer;
    xfer.slaveAddress   = rtc->rtcAddress;
    xfer.direction      = kLPI2C_Read;
    xfer.subaddress     = reg;
    xfer.subaddressSize = 1;
    xfer.data           = data;
    xfer.dataSize       = length;
    xfer.flags          = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(rtc->base, &xfer);
}

/*!
 * @brief Write one or more registers to the PCF85063A.
 *
 * Performs an I2C transfer using the MCUXpresso LPI2C driver.
 * The register address is sent first followed by the data bytes.
 *
 * @param rtc Pointer to the PCF85063A device instance.
 * @param reg     First register address to write.
 * @param data    Pointer to transmit buffer.
 * @param length  Number of bytes to write.
 *
 * @retval kStatus_Success Write completed successfully.
 * @retval Other           LPI2C driver error code.
 */
status_t PCF85063A_WriteRegisters(const PCF85063A_Type *const rtc,
                                  const uint8_t reg,
                                  uint8_t *const data,
                                  const uint8_t length)
{
    lpi2c_master_transfer_t xfer;
    xfer.slaveAddress   = rtc->rtcAddress;
    xfer.direction      = kLPI2C_Write;
    xfer.subaddress     = reg;
    xfer.subaddressSize = 1;
    xfer.data           = data;
    xfer.dataSize       = length;
    xfer.flags          = kLPI2C_TransferDefaultFlag;

    return LPI2C_MasterTransferBlocking(rtc->base, &xfer);
}

/*!
 * @brief Set the RTC date and time.
 *
 * This example programs the RTC with a fixed date and time.
 * Modify the values in this function to set a different date.
 *
 * Example:
 *     03-Jul-2026 14:30:00
 *
 * @param rtc Pointer to the PCF85063A device instance.
 * @param setCurrentTime Pointer to variable containing time to set.
 *
 * @retval kStatus_Success Time was written successfully.
 * @retval Other           I2C communication error.
 */
status_t PCF85063A_SetTime(const PCF85063A_Type *const rtc, const timedate_t *const setCurrentTime)
{
    uint8_t rtcData[PCF85063_TIME_DATE_SIZE];

    rtcData[0] = dec2bcd(setCurrentTime->sec & 0x7F);
    rtcData[1] = dec2bcd(setCurrentTime->min & 0x7F);
    rtcData[2] = dec2bcd(setCurrentTime->hour & 0x3F);
    rtcData[3] = dec2bcd(setCurrentTime->day & 0x3F);
    rtcData[4] = dec2bcd(setCurrentTime->dow & 0x07);
    rtcData[5] = dec2bcd(setCurrentTime->mon & 0x1F);
    rtcData[6] = dec2bcd(setCurrentTime->year);

    return PCF85063A_WriteRegisters(rtc, PCF85063_REG_SECONDS, rtcData, ARRAY_SIZE(rtcData));
}

/*!
 * @brief Configure the RTC alarm.
 *
 * Programs the alarm registers and enables the alarm interrupt.
 * The alarm is configured as a daily repeating alarm by ignoring
 * the day comparison field.
 *
 * @param rtc Pointer to the PCF85063A device instance.
 * @param setAlarmTime Pointer to variable containing alarm time.
 *
 * @retval kStatus_Success Alarm configured successfully.
 * @retval Other           I2C communication error.
 */
status_t PCF85063A_SetAlarm(const PCF85063A_Type *const rtc, const alarmtime_t *const setAlarmTime)
{
    uint8_t alarm[PCF85063_ALARM_TIME_SIZE];

    PCF85063A_ClearAlarmFlag(rtc);

    /*
     * Daily alarm:
     * compare seconds, minutes, hours
     * ignore day field (bit7 = 1)
     */
    alarm[0] = dec2bcd(setAlarmTime->sec & 0x7F);
    alarm[1] = dec2bcd(setAlarmTime->min & 0x7F);
    alarm[2] = dec2bcd(setAlarmTime->hour & 0x3F);
    alarm[3] = PCF85063_DAY_FIELD_ALARM_OFF;

    if (PCF85063A_WriteRegisters(rtc, PCF85063_REG_SECOND_ALARM, alarm, ARRAY_SIZE(alarm)) != kStatus_Success)
    {
        PRINTF("RTC write failed\n");
        return kStatus_Fail;
    }

    return PCF85063A_SetFlags(rtc, PCF85063_REG_CTRL2, PCF85063_ALARM_ENABLE, PCF85063_CTRL2_AIE);
}

/*!
 * @brief Check whether an alarm event has occurred.
 *
 * Reads the Alarm Flag (AF) bit in the CTRL2 register.
 *
 * @param rtc Pointer to the PCF85063A device instance.
 *
 * @retval true   Alarm event occurred.
 * @retval false  No alarm event pending.
 */
bool PCF85063A_AlarmOccurred(const PCF85063A_Type *const rtc)
{
    uint8_t ctrl2;

    if (PCF85063A_ReadRegisters(rtc, PCF85063_REG_CTRL2, &ctrl2, 1) != kStatus_Success)
    {
        PRINTF("RTC read failed\n");
        return false;
    }

    return ((ctrl2 & PCF85063_CTRL2_AF) != 0U);
}

/*!
 * @brief Clear the alarm flag.
 *
 * Clears the Alarm Flag (AF) bit in the CTRL2 register.
 * If the INT pin is connected to the MCU, clearing this flag
 * releases the interrupt output.
 *
 * @param rtc Pointer to the PCF85063A device instance.
 */
void PCF85063A_ClearAlarmFlag(const PCF85063A_Type *const rtc)
{
    PCF85063A_SetFlags(rtc, PCF85063_REG_CTRL2, PCF85063_ALARM_OFF, PCF85063_CTRL2_AF);
}

/*!
 * @brief Print the current date and time.
 *
 * Prints the current date and time to the debug console.
 *
 * Printed format:
 *     Date : DD/MM/YYYY
 *     Time : HH:MM:SS
 *
 * @param currentTime Pointer to variable containing time to print.
 */
void PCF85063A_PrintTime(const timedate_t *const currentTime)
{
    PRINTF("Date : %02d/%02d/20%02d\n", currentTime->day, currentTime->mon, currentTime->year);
    PRINTF("Time : %02d:%02d:%02d\n", currentTime->hour, currentTime->min, currentTime->sec);
    PRINTF("Weekday : %d\n\n", currentTime->dow);
}

/*!
 * @brief Read current date and time.
 *
 * Reads the RTC time registers beginning at the Seconds register,
 * converts the values from BCD to decimal, and returns in the structure.
 *
 * @param getCurrentTime Pointer to variable to store current time and date.
 * @param rtc Pointer to the PCF85063A device instance.
 */
status_t PCF85063A_GetTime(const PCF85063A_Type *const rtc, timedate_t *const getCurrentTime)
{
    uint8_t rtcData[PCF85063_TIME_DATE_SIZE];

    if (PCF85063A_ReadRegisters(rtc, PCF85063_REG_SECONDS, rtcData, ARRAY_SIZE(rtcData)) != kStatus_Success)
    {
        PRINTF("RTC read failed\n");
        return kStatus_Fail;
    }

    getCurrentTime->sec     = bcd2dec(rtcData[0] & 0x7F);
    getCurrentTime->min     = bcd2dec(rtcData[1] & 0x7F);
    getCurrentTime->hour    = bcd2dec(rtcData[2] & 0x3F);
    getCurrentTime->day     = bcd2dec(rtcData[3] & 0x3F);
    getCurrentTime->dow     = bcd2dec(rtcData[4] & 0x07);
    getCurrentTime->mon     = bcd2dec(rtcData[5] & 0x1F);
    getCurrentTime->year    = bcd2dec(rtcData[6]);

    return kStatus_Success;
}

/**
 * @brief Updates selected bits in a PCF85063 RTC register.
 *
 * This function performs a read-modify-write operation on the specified
 * register of the PCF85063 RTC. It first reads the current register value,
 * then updates only the bits specified by the mask while leaving all other
 * bits unchanged, and finally writes the modified value back to the register.
 *
 * @param rtc Pointer to the PCF85063A device instance.
 * @param registerAdd Register address to modify.
 * @param bits New bit values to be applied.
 * @param mask Bit mask indicating which bits should be updated.
 */
status_t PCF85063A_SetFlags(const PCF85063A_Type *const rtc,
                            const uint8_t registerAdd,
                            const uint8_t bits,
                            const uint8_t mask)
{
    uint8_t data;

    if (PCF85063A_ReadRegisters(rtc, registerAdd, &data, 1) != kStatus_Success)
    {
        PRINTF("RTC read failed\n");
        return kStatus_Fail;
    }

    data = (bits & mask) | (data & (~mask));

    if (PCF85063A_WriteRegisters(rtc, registerAdd, &data, 1) != kStatus_Success)
    {
        PRINTF("RTC write failed\n");
        return kStatus_Fail;
    }

    return kStatus_Success;
}
