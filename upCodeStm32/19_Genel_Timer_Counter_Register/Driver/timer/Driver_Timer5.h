#ifndef __DRIVER_TIMER5_H__
#define __DRIVER_TIMER5_H__
#include "stm32f10x.h" 
/**
 * @brief 基本定时器timer6 初始化
 * 
 */
void Driver_Timer5_Init(void);
#endif // __DRIVER_TIMER5_H__

/**
 * @brief 通用定时器如何计数
 *  先分频，假设分频后时钟周期是100us,自动重装载寄存器中断值是10000
 *  CNT计数器 每一时钟周期会增加1, 达到自动重装寄存器中的值(10000)就触发一次中断,
 *  说明过了1s  
 *  这里只涉及到CNT计数器，是有时钟频率触发的。和自重装载寄存器
 * 
 */