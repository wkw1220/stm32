#include "Driver_SysTick.h"
#include "Driver_Led.h"
/**
 * @brief 时钟定时器初始化
 * 
 */
void Driver_SysTick_Init(void)
{
    /* 1.配置时钟源 1=AHB(72MHZ) 0=AHB/8 */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
    /* 2.使能中断 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT;
    /* 3.定时器1ms产生一次中断 */
    SysTick->LOAD=72000-1;
    /* 4使能定时器 */
    SysTick->CTRL |= SysTick_CTRL_ENABLE;
}

/**
 * @brief 
 * 
 */
uint16_t count=0;
uint8_t isReadySystick=0;
void SysTick_Handler(void){
   count++;
   if(count==1000){
     count=0;
     Dri_Led_Toggle(LED_BLUE);
     //isReadySystick=1;
   }
}