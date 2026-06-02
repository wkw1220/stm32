#ifndef __DRIVER_I2C2_SOFT_H__
#define __DRIVER_I2C2_SOFT_H__
#include "Delay.h"
#include "stm32f10x.h"
#include "Driver_Usart.h"

#define ACK 0
#define NACK 1
#define SDA_HIGH (GPIOB->ODR |=GPIO_ODR_ODR11)
#define SDA_LOW  (GPIOB->ODR &=~GPIO_ODR_ODR11)
#define SCL_HIGH (GPIOB->ODR |=GPIO_ODR_ODR10)
#define SCL_LOW  (GPIOB->ODR &=~GPIO_ODR_ODR10)
#define READ_SDA (GPIOB->IDR & GPIO_ODR_ODR11)

#define I2C_DELAY Delay_us(10)

void Dri_I2C2_Init_Soft(void);
void Dri_I2C2_Start_Soft(void);
void Dri_I2C2_Stop_Soft(void);
void Dri_I2C2_Ack_Soft(void);
void Dri_I2C2_NoAck_Soft(void);
uint8_t Dri_I2C2_WaitAck_Soft(void);
void Dri_I2C2_SendByte_Soft(uint8_t byte);
uint8_t Dri_I2C2_ReceiveByte_Soft(void);
#endif // __DRIVER_I2C2_H__
