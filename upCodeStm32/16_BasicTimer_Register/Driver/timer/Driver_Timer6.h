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

/**
 *  stm32中 以1和8结尾的一般的都是高级的
 *          以6或者7结尾的一般是基本的
 *         其他的基本都是通用的
 * 
 * 基本定时器只能用来计时
 * 通用计时器具有基本定时的功能 还能输出pwm方波
 * 高级定时器具有通用计时器的功能，同时还具备刹车时间和死区时间
 */