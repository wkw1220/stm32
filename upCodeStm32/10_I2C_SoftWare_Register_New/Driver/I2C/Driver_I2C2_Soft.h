/**
 * @file Driver_I2C2_Soft.h
 * @author windear
 * @brief W24C02:是一款EE2PROM芯片型号 本案例是MCU芯片通过I2C通信的方式对芯片进行存储和读写数据
 *        此芯片是外设硬件，驱动程序存入inf接口文件中
 * @version 0.1
 * @date 2025-07-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __DRIVER_I2C2_SOFT_H__
#define __DRIVER_I2C2_SOFT_H__
#include "Delay.h"
#include "stm32f10x.h"
#include "Dri_Usart.h"

#define ACK 0
#define NACK 1

#define SCL_HIGH (GPIOB->ODR |=GPIO_ODR_ODR10)
#define SCL_LOW  (GPIOB->ODR &=~GPIO_ODR_ODR10)
#define SDA_HIGH (GPIOB->ODR |=GPIO_ODR_ODR11)
#define SDA_LOW  (GPIOB->ODR &=~GPIO_ODR_ODR11)
#define READ_SDA (GPIOB->IDR & GPIO_ODR_ODR11)

#define I2C_DELAY Delay_us(10)

/**
 * @brief 初始化
 * 
 */
void Dri_I2C2_Init_Soft(void);
/**
 * @brief 开始信号
 * 
 */
void Dri_I2C2_Start_Soft(void);
/**
 * @brief 停止信号
 * 
 */
void Dri_I2C2_Stop_Soft(void);
/**
 * @brief  接收方产生应答信号
 * 
 */
void Dri_I2C2_Ack_Soft(void);
/**
 * @brief 接收方产生非应答信号
 * 
 */
void Dri_I2C2_NoAck_Soft(void);
/**
 * @brief 等待接收方的应答
 * 
 * @return uint8_t 应答或者非应答
 */
uint8_t Dri_I2C2_WaitAck_Soft(void);

/**
 * @brief 发送一个字节的数据
 * 
 * @param byte 要发送的字节
 */
void Dri_I2C2_SendByte_Soft(uint8_t byte);

/**
 * @brief 读一个字节的数据
 * 
 * @return uint8_t 返回读取的字节
 */
uint8_t Dri_I2C2_ReceiveByte_Soft(void);
#endif 
// __DRIVER_I2C2_H__

