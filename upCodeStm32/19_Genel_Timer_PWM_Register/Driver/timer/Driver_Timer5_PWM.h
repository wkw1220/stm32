#ifndef __DRIVER_TIMER5_PWM_H__
#define __DRIVER_TIMER5_PWM_H__
#include "stm32f10x.h" 

void Dri_Timer5_Pwm_Init(void);
void Dri_Timer5_Pwm_Start(void);
void Dri_Timer5_Pwm_Stop(void);
void Dri_TIM5_Pwm_SetDutyCycle(uint8_t dutyCycle);
#endif
/**
 * @brief PWM方波生成思路
 * 主要通过捕获比较寄存器中的比较功能   这是输出模式(关键点)！！！！
 *  后面的测量周期用的是捕获功能是输入模式
 *  首先通用定时器有四个通道(关键点) 作为PWM方波生成，任何一个通道都可以
 *  1.首先设置分频->主要是改变时钟周期
 *  2->设置自动重装载寄存器的值，说白了就是多长时间发生一次中断 好把后续的CNT计数器清零
 *  3->CNT计数器就开始计数 和前面定时器计数器功能是一模一样的
 *  4->核心就是设置捕获比较寄存器里的值 时刻和CNT里的值比较 知道结果要么大于等于
 *     要么小于  大于等于就输出高电平(or low voltage),小于就输出低电平(或高电平)
 *  5->然后修改每隔一段时间修改一下捕获比较器里的值
 *     假设周期是100us,自动重装载器里的值是100,也就是10ms,如果时间间隔大于10ms,呼吸灯的效果变频率就越小
 *     如果小于10ms 呼吸的效率频率就越大
 * 
 *   注意:这里没有用到中断 一定要配置通道的输出比较模式
 * 
 */