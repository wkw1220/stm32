#include "Driver_bkp.h"

void Dri_BKP_Init(void){
    /*1.使能时钟 bkp 和 pwr*/
    RCC->APB1ENR|=RCC_APB1ENR_BKPEN;
    RCC->APB1ENR|=RCC_APB1ENR_PWREN;
    /*2.允许访问后备寄存器*/
    PWR->CR|=PWR_CR_DBP;

    /** 备份域的软件复位和 恢复服务 执行这个会清除BKP上所有数据 */
    //RCC->BDCR|=RCC_BDCR_BDRST;
    //RCC->BDCR&=~RCC_BDCR_BDRST;

    /**3.使能RTC时钟 */
    RCC->BDCR|=RCC_BDCR_RTCEN;

     /**4.选择RTC时钟源 LSE */
    RCC->BDCR&=~RCC_BDCR_LSEON;

    /**5.配置LSE作为RTC时钟 */
    RCC->BDCR&=~RCC_BDCR_RTCSEL;
    RCC->BDCR|=(0x01<<8);
}

void Dri_BKP_WriteData(u16 data){
    BKP->DR1=data;

}
u16 Dri_BKP_ReadData(){
    return BKP->DR1;
}