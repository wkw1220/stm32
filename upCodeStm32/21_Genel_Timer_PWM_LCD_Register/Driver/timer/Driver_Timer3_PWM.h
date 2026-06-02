#ifndef __DRIVER_TIMER3_PWM_H__
#define __DRIVER_TIMER3_PWM_H__
#include "stm32f10x.h" 

void Dri_Timer3_Pwm_Init(void);
void Dri_Timer3_Pwm_Start(void);
void Dri_Timer3_Pwm_Stop(void);
void Dri_Tim3_Pwm_SetDutyCycle(uint8_t dutyCycle);
#endif
