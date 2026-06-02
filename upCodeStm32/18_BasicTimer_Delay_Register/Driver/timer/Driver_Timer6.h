#ifndef __DRIVER_TIMER6_H__
#define __DRIVER_TIMER6_H__
#include "stm32f10x.h" 

/**
 * 基本定时器 TIM6 TIM7
 * @brief 基本定时器timer6 初始化
 *  16位自动装载计数器
 *  只能向上计数，没有外部IO，不能对外不脉冲进行计数
 */
void Driver_Timer6_Init(void);

#endif // __DRIVER_TIMER6_H__
