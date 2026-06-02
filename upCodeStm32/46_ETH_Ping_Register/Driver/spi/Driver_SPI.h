#ifndef __DRIVER_SPI_H__
#define __DRIVER_SPI_H__
#include "stm32f10x.h"
#include"Delay.h"

#define CS_HIGH (GPIOD->ODR |= GPIO_ODR_ODR3)
#define CS_LOW (GPIOD->ODR &= ~GPIO_ODR_ODR3)

#define SCK_HIGH (GPIOB->ODR |= GPIO_ODR_ODR13)
#define SCK_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR13)

#define MOSI_HIGH (GPIOB->ODR |= GPIO_ODR_ODR15)
#define MOSI_LOW (GPIOB->ODR &= ~GPIO_ODR_ODR15)

#define MISO_READ (GPIOB->IDR & GPIO_IDR_IDR14)

#define SPI_DELAY Delay_us(5)

void Driver_SPI_Init(void);
void Driver_SPI_Start(void);
void Driver_SPI_Stop(void);
uint8_t Driver_SPI_SwapByte(uint8_t byte);

#endif // __DRIVER_SPI_H__

