#ifndef __DRIVER_TIMER4_MESURE_H__
#define __DRIVER_TIMER4_MESURE_H__
#include "stm32f10x.h" 

void Dri_Timer4_Init(void);
void Dri_Timer4_Start(void);
void Dri_Timer4_Stop(void);
double Dri_Tim4_getCycle(void);
double Dri_Tim4_getFrequency(void);
double Dri_Tim4_getDutyCycle(void);
#endif

/**
 * @brief 核心思想 要理解从模式 非常重要 实现自动测量
 * 
 * 触发器的触发信号
 * 1->触发输入信号(TRGI):主要通过外部的信号来控制定时器的一些动作 这时候定时器
 *    就处于主从模式中的从模式
 * 2->触发输出信号(TRGO):本定时器输出到其它定时器或者其它外设信号，一般用于其它定时器的级联
 *    或触发其它一些外设的工作
 * 
 * 主要理解TRGI
 * 有专门的从模式控制寄存器中TS[2:0]来描述
 * 
 * 
 * 
 */

