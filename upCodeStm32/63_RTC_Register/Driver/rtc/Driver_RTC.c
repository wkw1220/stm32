#include "Driver_RTC.h"
void Dri_RTC_Init(void){
    /*1.使能时钟 bkp 和 pwr*/
    RCC->APB1ENR|=RCC_APB1ENR_BKPEN;
    RCC->APB1ENR|=RCC_APB1ENR_PWREN;
    /*2.允许访问后备域寄存器*/
    PWR->CR|=PWR_CR_DBP;
    
    /**3.选择RTC时钟源 LSE */
    RCC->BDCR|=RCC_BDCR_LSEON;
    while(!(RCC->BDCR & RCC_BDCR_LSERDY));

    /**4.使能RTC时钟 */
    RCC->BDCR|=RCC_BDCR_RTCEN;
    
    /**
     * 5.判断RTOFF是否允许写入
     */    
    while(!(RTC->CRL & RTC_CRL_RTOFF));
    /*6.进入配置模式*/
    RTC->CRL|=RTC_CRL_CNF;
    /*7.设置预分频值 设置RTC预分频值 32768Hz/32768=1Hz */
    RTC->PRLH=0x0000;
    RTC->PRLL=(0x7FFF&0xFFFF);

    /*8.退出配置模式*/
    RTC->CRL&=~RTC_CRL_CNF;
}

void Driver_RTC_SetUinxTimestamp(uint32_t value){
    /** 5.判断RTOFF是否允许写入 */
    while(!(RTC->CRL & RTC_CRL_RTOFF));

    /*6.进入配置模式*/
    RTC->CRL|=RTC_CRL_CNF;

     //等待秒标志
    while(!(RTC->CRL & RTC_CRL_SECF));
    /*7.设置RTC计数器*/
    RTC->CNTH=(value>>16)&0xFFFF;
    RTC->CNTL=value&0xFFFF;

    /*8.退出配置模式*/
    RTC->CRL&=~RTC_CRL_CNF;
}

uint32_t Driver_RTC_readUnixTimestamp(){
    uint32_t value=0;
    /** 1.等待寄存器数据同步完成 */
    while(!(RTC->CRL & RTC_CRL_RSF));
    RTC->CRL &=~RTC_CRL_RSF;
    
    /*7.读取RTC计数器*/
    value=((uint32_t)(RTC->CNTH)<<16)&0xFFFF0000;
    value|=((uint32_t)(RTC->CNTL))&0x0000FFFF;

    return value;
}

 void Driver_RTC_getDateTime(RTC_DateTime_Struct *dateTime){
    
    uint32_t value=0;
    /** 1.等待寄存器数据同步完成 */
    while(!(RTC->CRL & RTC_CRL_RSF));
    RTC->CRL &=~RTC_CRL_RSF;
    
    /*7.读取RTC计数器*/
    value=((uint32_t)(RTC->CNTH)<<16)&0xFFFF0000;
    value|=((uint32_t)(RTC->CNTL))&0x0000FFFF;
    struct tm *timeinfo= localtime((const time_t*)&value);
    dateTime->year=timeinfo->tm_year + 1900;
    dateTime->month=timeinfo->tm_mon + 1;
    dateTime->day=timeinfo->tm_mday;
    dateTime->hour=timeinfo->tm_hour;
    dateTime->minute=timeinfo->tm_min;
    dateTime->second=timeinfo->tm_sec;
}

void Driver_RTC_getDateTimeOther(uint16_t *year,uint16_t *month,uint16_t *day,
    uint16_t *hour,uint16_t *minute,uint16_t *second){
    uint32_t value=0;
    /** 1.等待寄存器数据同步完成 */
    while(!(RTC->CRL & RTC_CRL_RSF));
    RTC->CRL &=~RTC_CRL_RSF;
    
    /*2.读取RTC计数器*/
    value=((uint32_t)(RTC->CNTH)<<16)&0xFFFF0000;
    value|=((uint32_t)(RTC->CNTL))&0x0000FFFF;
    struct tm *timeinfo= localtime((const time_t*)&value);
    /* printf("year=%d, month=%d, day=%d, hour=%d, minute=%d, second=%d\n",
           timeinfo->tm_year + 1900,
           timeinfo->tm_mon + 1,
           timeinfo->tm_mday,
           timeinfo->tm_hour,
           timeinfo->tm_min,
           timeinfo->tm_sec); */
    *year=timeinfo->tm_year + 1900;
    *month=timeinfo->tm_mon + 1;
    *day=timeinfo->tm_mday;
    *hour=timeinfo->tm_hour;
    *minute=timeinfo->tm_min;
    *second=timeinfo->tm_sec;
    /* RTC_DateTime_Struct struct1;
    RTC_DateTime_Struct *struct2;
    struct1.year=timeinfo->tm_year + 1900;
    struct2->year=timeinfo->tm_year + 1900; */
    //printf("struct1 year= %d\n",struct1.year);
    //printf("struct2 year= %d\n",struct2->year);
    //printf("year=%d %d\n",*year,timeinfo->tm_year + 1900);
     /* printf("DateTime: %04d-%02d-%02d %02d:%02d:%02d\r\n",
               *year,
               *month,
               *day,
               *hour,
               *minute,
               *second);  */
    }
