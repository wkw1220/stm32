#ifndef __delay_h__
#define __delay_h__
#include "stm32f10x.h" // Device header

void Delay_us(uint16_t us);
void Delay_ms(uint16_t ms);
void Delay_s(uint16_t s);

void Delay_us_SysTick(uint16_t us);

void Delay_ms_SysTick(uint16_t ms);

void Delay_s_SysTick(uint16_t s);

void Delay_us_BasicTimer(uint16_t us);

void Delay_ms_BasicTimer(uint16_t ms);

void Delay_s_BasicTimer(uint16_t s);

#endif
