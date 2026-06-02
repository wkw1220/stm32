#ifndef __INF_ESP32_H__
#define __INF_ESP32_H__
#include "usart.h"
#include "string.h"
void Inf_ESP32_Init(void);
void Inf_SendAtCmd(uint8_t *atCmd);

void Inf_ESP32_BlurTeethInit();
void Inf_ESP32_BLEReadData(void);
#endif // !__INF_ESP32_H__




