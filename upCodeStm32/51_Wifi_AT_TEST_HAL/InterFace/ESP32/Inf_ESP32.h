#ifndef __INF_ESP32_H__
#define __INF_ESP32_H__
#include "usart.h"
#include "string.h"
#include "stdio.h"
#include "main.h"
enum{
    ESP32_NOIT=1,
    ESP32_IT=2,
};

/**
 * @brief 初始化函数
 *
 */
void Inf_ESP32_Init(uint8_t mode);

/**
 * @brief 向ESP32发送命令
 *
 * @param cmd
 * @param cmdLen
 */
void Inf_ESP32_SendCmd(uint8_t *cmd);

void Inf_ESP32_SendCmd_IT(uint8_t *cmd);

/**
 * @brief 读取响应
 *
 * @param buff
 * @param len
 */
void Inf_ESP32_ReadResp(uint8_t buff[], uint16_t *len);

void Inf_ESP32_WifiInit(void);

void Inf_ESP32_WifiInit_IT(void);

void Inf_ESP32_TcpServer(void);

void Inf_ESP32_ReadTcpData(uint16_t *id,uint8_t ip[],uint16_t *port,uint16_t *dataLen,uint8_t data[]);

void Inf_ESP32_SendTcpData(uint16_t id,uint8_t data[],uint16_t dataLen);

#endif // !__INF_ESP32_H__
