#ifndef __DRIVER_SYSTICK_H__
#define __DRIVER_SYSTICK_H__
#include "stm32f10x.h" 
#define LOAD_VLUE 72000
/**
 * @brief 时钟定时器初始化
 * 
 */
void Driver_SysTick_Init(void);
void Driver_SysTick_Init_V2(void);
#endif // __DRIVER_SYSTICK_H__

