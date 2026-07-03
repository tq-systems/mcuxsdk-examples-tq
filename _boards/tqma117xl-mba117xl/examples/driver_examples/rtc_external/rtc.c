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
#include "board.h"
#include "app.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SECOND            20U
#define MINUTE            30U
#define HOUR              14U
#define DAY               3U
#define WEEKDAY           5U
#define MONTH             7U
#define YEAR              26U
#define ALARM_SECOND      40U
#define ALARM_MINUTE      30U
#define ALARM_HOUR        14U
#define LOAD_CAP_SEL_12_5 0x01
#define ENABLE_CAP_12_5   0x01
#define WAIT_TIME_1_SEC   1000000U

/* I2C Bus configuration definitions */

#define I2C_MASTER                   ((LPI2C_Type *)I2C_MASTER_BASE)
#define I2C_MASTER_BASE              (LPI2C6_BASE)
#define LPI2C_BAUDRATE               100000U
#define LPI2C_MASTER_CLOCK_FREQUENCY BOARD_BOOTCLOCKRUN_LPI2C6_CLK_ROOT

/*******************************************************************************
 * Main
 ******************************************************************************/

int main(void)
{
    timedate_t setTime;
    alarmtime_t alarmTimeSet;
    timedate_t getTime;
    PCF85063A_Type rtc;
    rtc.base = LPI2C6; /* RTC is connected to I2C bus 6 */
    rtc.rtcAddress = PCF85063_I2C_ADDR; /* PCF85063A Slave address */
    lpi2c_master_config_t masterConfig;
    BOARD_InitHardware();
    LPI2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Hz = LPI2C_BAUDRATE;
    LPI2C_MasterInit(I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

    /*
    * Select 12.5 Pico farad Integrated load capacitance for rtc crystal.
    * Refer datasheet and schematic for more information.
    */
    PCF85063A_SetFlags(&rtc, PCF85063_REG_CTRL1, ENABLE_CAP_12_5, LOAD_CAP_SEL_12_5);

    PRINTF("Current RTC value:\n");
    PCF85063A_GetTime(&rtc, &getTime);
    PCF85063A_PrintTime(&getTime);
    PRINTF("Setting RTC...\n");

    setTime.sec  = SECOND;
    setTime.min  = MINUTE;
    setTime.hour = HOUR;
    setTime.day  = DAY;
    setTime.dow  = WEEKDAY;
    setTime.mon  = MONTH;
    setTime.year = YEAR;

    if (PCF85063A_SetTime(&rtc, &setTime) == kStatus_Success)
    {
        PRINTF("RTC updated successfully.\n");
    }
    else
    {
        PRINTF("RTC update failed.\n");
    }

    PRINTF("Setting alarm for %02d:%02d:%02d every day...\n\n", ALARM_HOUR, ALARM_MINUTE, ALARM_SECOND);

    alarmTimeSet.sec = ALARM_SECOND;
    alarmTimeSet.min = ALARM_MINUTE;
    alarmTimeSet.hour = ALARM_HOUR;
    if (PCF85063A_SetAlarm(&rtc, &alarmTimeSet) == kStatus_Success)
    {
        PRINTF("Alarm configured.\n");
    }
    else
    {
        PRINTF("Alarm configuration failed.\n");
    }

    SDK_DelayAtLeastUs(WAIT_TIME_1_SEC, CLOCK_GetFreq(kCLOCK_CoreSysClk));
    PRINTF("RTC after update:\n");

    /* Demostrate PCF85063A_GetTime function */
    PCF85063A_GetTime(&rtc, &getTime);
    PCF85063A_PrintTime(&getTime);

    while (1)
    {
        SDK_DelayAtLeastUs(WAIT_TIME_1_SEC, CLOCK_GetFreq(kCLOCK_CoreSysClk));
        PCF85063A_GetTime(&rtc, &getTime);
        PCF85063A_PrintTime(&getTime);
        if (PCF85063A_AlarmOccurred(&rtc))
        {
            PRINTF("====================================\n");
            PRINTF("==            ALARM..!            ==\n");
            PRINTF("====================================\n\n");
            PCF85063A_ClearAlarmFlag(&rtc);
            PCF85063A_GetTime(&rtc, &getTime);
            PCF85063A_PrintTime(&getTime);
        }
    }
}
