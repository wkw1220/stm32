#ifndef __INTEFACE_W24C02_HARD_H__
#define __INTEFACE_W24C02_HARD_H__
#include "Driver_I2C2_HardWare.h"
#include "string.h"
#define ADDR 0xA0
void Inf_W24C02_Init_Hard(void);
void Inf_W24C02_WriteByte_Hard(uint8_t innerAddr, uint8_t byte);
uint8_t Inf_W24C02_ReadByte_Hard(uint8_t innerAddr);
void Inf_W24C02_WriteBytes_Hard(uint8_t innerAddr, uint8_t *bytes, uint8_t len);
void Inf_W24C02_ReadBytes_Hard(uint8_t innerAddr, uint8_t *bytes, uint8_t len);
#endif // __INTEFACE_W24C02_HARD_H__
