#ifndef __INF_ESP32_H__
#define __INF_ESP32_H__
#include "usart.h"
#include "string.h"
void Inf_ESP32_Init(void);
void Inf_SendAtCmd(uint8_t *atCmd);
void Inf_ESP32_WifiInit(void);
void Inf_ESP32_TCPServerInit(void);
//port 为什么用 uint16_t 不行
void Inf_ESP32_ReadTcpData(uint16_t *id,uint8_t ip[],uint32_t *port,uint16_t *dataLen,uint8_t data[]);
void Inf_ESP32_SendTcpData(uint16_t id,uint16_t dataLen,uint8_t data[]);
#endif // !__INF_ESP32_H__




