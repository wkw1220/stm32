#ifndef __DRIVER_I2C2_HARDWARE_H__
#define __DRIVER_I2C2_HARDWARE_H__
#include "Delay.h"
#include "stm32f10x.h"
#include "Driver_Usart.h"

#define ACK 0
#define NACK 1
#define OK 1
#define FAIL 0

#define I2C_DELAY Delay_us(10)

void Dri_I2C2_Init_Hard(void);
uint8_t Dri_I2C2_Start_Hard(void);
void Dri_I2C2_Stop_Hard(void);
void Dri_I2C2_Ack_Hard(void);
void Dri_I2C2_NoAck_Hard(void);
uint8_t Driver_I2C_SendAddr(uint8_t addr);
uint8_t Dri_I2C2_SendByte_Hard(uint8_t byte);
uint8_t Dri_I2C2_ReceiveByte_Hard(void);
#endif // __DRIVER_I2C2_HARDWARE_H__
