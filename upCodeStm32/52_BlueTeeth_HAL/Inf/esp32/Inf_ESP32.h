#ifndef __INF_ESP32_H__
#define __INF_ESP32_H__
#include "usart.h"
#include "string.h"

/* typedef enum{
    WIFI_CHANGE=0,//wifi变化
    BLE_DISCONN,//蓝牙连接上 
    BLE_SPP,//蓝牙进入透传模式
    BLE_DATA   //蓝色数据
} ESP32State; */
void Inf_ESP32_Init(void);
void Inf_SendAtCmd(uint8_t *atCmd);

void Inf_ESP32_BlurTeethInit(void);
void Inf_ESP32_BLEReadData(void);
#endif // !__INF_ESP32_H__




