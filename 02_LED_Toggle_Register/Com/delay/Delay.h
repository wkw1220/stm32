#ifndef __delay_h__
#define __delay_h__
#include "stm32f10x.h" // Device header

/**
 * @brief 这三个参数为什么是uint16_t，而不是uint8_t或者uint32_t?
 * 
 * @param us 
 * 准确的说这个函数不是延时多长时间 是至少多长时间
 */
void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);
void Delay_s(uint16_t s);

void Delay_BadWay(void);

#endif
