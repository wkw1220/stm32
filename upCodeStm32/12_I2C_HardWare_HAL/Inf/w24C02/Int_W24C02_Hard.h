#ifndef __INTEFACE_W24C02_HARD_H__
#define __INTEFACE_W24C02_HARD_H__
#include "string.h"
#include "i2c.h"
#define ADDR 0xA0
void Inf_W24C02_Init(void);
void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte);
uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr);
void Inf_W24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len);
void Inf_W24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len);
#endif // __INTEFACE_W24C02_HARD_H__
