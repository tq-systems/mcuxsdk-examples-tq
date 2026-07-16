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

#define PCF85063_I2C_ADDR            0x51
#define PCF85063_REG_CTRL1           0x00
#define PCF85063_REG_CTRL2           0x01
#define PCF85063_REG_OFFSET          0x02
#define PCF85063_REG_RAM             0x03
#define PCF85063_REG_SECONDS         0x04
#define PCF85063_REG_MINUTES         0x05
#define PCF85063_REG_HOURS           0x06
#define PCF85063_REG_DAYS            0x07
#define PCF85063_REG_WEEKDAYS        0x08
#define PCF85063_REG_MONTHS          0x09
#define PCF85063_REG_YEARS           0x0A
#define PCF85063_REG_SECOND_ALARM    0x0B
#define PCF85063_REG_MINUTE_ALARM    0x0C
#define PCF85063_REG_HOUR_ALARM      0x0D
#define PCF85063_REG_DAY_ALARM       0x0E
#define PCF85063_TIME_DATE_SIZE      0x07
#define PCF85063_ALARM_TIME_SIZE     0x04
#define PCF85063_DAY_FIELD_ALARM_OFF 0x80 /* ignore day field (bit7 = 1) */
#define PCF85063_ALARM_OFF           0xBF
#define PCF85063_ALARM_ENABLE        0x80
#define PCF85063_CTRL2_AIE           (1U << 7)
#define PCF85063_CTRL2_AF            (1U << 6)

/*******************************************************************************
 * Structure Definition
 ******************************************************************************/

/**
 * @brief Structure to store date and time information.
 *
 * This structure contains the individual components of a calendar date
 * and 24-hour time.
 */
typedef struct timedate
{
    uint8_t sec;  /* second (0 - 59) */
    uint8_t min;  /* minute (0 - 59) */
    uint8_t hour; /* hour (0 - 23) */
    uint8_t day;  /* day (1 - 31) */
    uint8_t dow;  /* day of week (0 - 6; 0 = Sunday) */
    uint8_t mon;  /* month (1 - 12) */
    uint8_t year; /* year (4 digit year) */
} timedate_t;

/**
 * @brief Structure to store date and time information.
 *
 * This structure contains the individual components 24-hour time.
 */
typedef struct alarmtime
{
    uint8_t sec;  /* second (0 - 59) */
    uint8_t min;  /* minute (0 - 59) */
    uint8_t hour; /* hour (0 - 23) */
} alarmtime_t;

/**
 * @brief Structure to store base address of I2C bus and slave.
 *
 * This structure contains the base address of I2C bus connected to rtc 
 * and the slave address of the rtc.
 */
typedef struct pcf85063a
{
    LPI2C_Type *base;   /* I2C bus base address */
    uint8_t rtcAddress; /* RTC PCF85063A address on the bus */
} PCF85063A_Type;

/*******************************************************************************
 * API
 ******************************************************************************/

bool PCF85063A_AlarmOccurred(const PCF85063A_Type *const rtc);

void PCF85063A_ClearAlarmFlag(const PCF85063A_Type *const rtc);

void  PCF85063A_PrintTime(const timedate_t *const currentTime);

status_t PCF85063A_GetTime(const PCF85063A_Type *const rtc, timedate_t *const getCurrentTime);

status_t PCF85063A_SetAlarm(const PCF85063A_Type *const rtc, const alarmtime_t *const setAlarmTime);

status_t PCF85063A_SetTime(const PCF85063A_Type *const rtc, const timedate_t *const setCurrentTime);

status_t PCF85063A_SetFlags(const PCF85063A_Type *const rtc,
                            const uint8_t registerAdd,
                            const uint8_t bits,
                            const uint8_t mask);

status_t PCF85063A_ReadRegisters(const PCF85063A_Type *const rtc,
                                 const uint8_t reg,
                                 uint8_t *const data,
                                 const uint8_t length);

status_t PCF85063A_WriteRegisters(const PCF85063A_Type *const rtc,
                                  const uint8_t reg,
                                  uint8_t *const data,
                                  const uint8_t length);
