#ifndef __Dri_DHT11_H__
#define __Dri_DHT11_H__
#include "stm32f10x.h" 
#include <stdio.h> 
#include <string.h> 
#define DHT11_PIN GPIO_ODR_ODR1
#define SCL_HIGH (GPIOA->ODR |=GPIO_ODR_ODR1)
#define SCL_LOW  (GPIOA->ODR &=~GPIO_ODR_ODR1)

uint8_t receiveOneByte(void);
void receiveOneBytes(uint8_t* result);
void start(void);
#endif

