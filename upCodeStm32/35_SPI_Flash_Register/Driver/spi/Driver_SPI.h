#ifndef __DRIVER_SPI_H__
#define __DRIVER_SPI_H__
#include "stm32f10x.h"
#include"Delay.h"

#define CS_HIGH (GPIOC->ODR |= GPIO_ODR_ODR13)
#define CS_LOW (GPIOC->ODR &= ~GPIO_ODR_ODR13)

#define SCK_HIGH (GPIOA->ODR |= GPIO_ODR_ODR5)
#define SCK_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR5)

#define MOSI_HIGH (GPIOA->ODR |= GPIO_ODR_ODR7)
#define MOSI_LOW (GPIOA->ODR &= ~GPIO_ODR_ODR7)

#define MISO_READ (GPIOA->IDR & GPIO_IDR_IDR6)

#define SPI_DELAY Delay_us(5)

void Driver_SPI_Init(void);
void Driver_SPI_Start(void);
void Driver_SPI_Stop(void);
uint8_t Driver_SPI_SwapByte(uint8_t byte);

#endif // __DRIVER_SPI_H__

/**
 * @brief SPI
 * 高速的 全双工 同步 串行通讯协议
 * 四根线:SCLK MOSI MISO SS(NSS/CS):Neganative SS,低电平有效
 * STM32的SPI的时钟频率最大位fpclk/2
 * 两个设备之间通讯时，通讯速率受限于低速设备
 * 主机的MOSI和从机的MOSI相连 主机的MISO和从机的MISO相连 SCLK和SCLK相连
 * 
 * spi通讯时是没有地址的，主机和哪个从机通讯，就把谁的SS置为低电平
 * 
 * 时钟的极性和相位
 * NOR:或非 NAND:与非
 */