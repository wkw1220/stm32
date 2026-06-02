#ifndef __DRIVER_RTC_H__
#define __DRIVER_RTC_H__
#include "stm32f10x.h"
#include "time.h"
#include <stdio.h>

typedef struct {
    uint16_t year;
    uint16_t month;
    uint16_t day;
    uint16_t hour;
    uint16_t minute;
    uint16_t second;
} RTC_DateTime_Struct;

void Dri_RTC_Init(void);
void Driver_RTC_SetUinxTimestamp(uint32_t value);
uint32_t Driver_RTC_readUnixTimestamp(void);
void Driver_RTC_getDateTime(RTC_DateTime_Struct *dateTime);

void Driver_RTC_getDateTimeOther(uint16_t *year,uint16_t *month,uint16_t *day,
    uint16_t *hour,uint16_t *minute,uint16_t *second);
#endif


