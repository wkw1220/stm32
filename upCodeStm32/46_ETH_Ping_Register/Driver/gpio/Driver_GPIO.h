#ifndef __DRIVER_GPIO_H__
#define __DRIVER_GPIO_H__
#include "stm32f10x.h"
#define RST_HIGH (GPIOG->ODR |= GPIO_ODR_ODR7)
#define RST_LOW (GPIOG->ODR &= ~GPIO_ODR_ODR7)
void Dri_GPIO_ResetAndInt_Init(void);
#endif // __DRIVER_GPIO_H__



