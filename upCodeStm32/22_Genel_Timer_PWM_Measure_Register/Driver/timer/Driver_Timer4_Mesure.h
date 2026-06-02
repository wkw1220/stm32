#ifndef __DRIVER_TIMER4_MESURE_H__
#define __DRIVER_TIMER4_MESURE_H__
#include "stm32f10x.h" 

void Dri_Timer4_Init(void);
void Dri_Timer4_Start(void);
void Dri_Timer4_Stop(void);
double Dri_Tim4_getCycle(void);
double Dri_Tim4_getFrequency(void);
#endif
