#ifndef __DRIVER_TIMER5_PWM_H__
#define __DRIVER_TIMER5_PWM_H__
#include "stm32f10x.h" 

void Dri_Timer5_Pwm_Init(void);
void Dri_Timer5_Pwm_Start(void);
void Dri_Timer5_Pwm_Stop(void);
void Dri_TIM5_Pwm_SetDutyCycle(uint8_t dutyCycle);
#endif