#ifndef __DRIVER_CAN_H
#define __DRIVER_CAN_H
#include "stm32f10x.h"
#include "Dri_USART.h"
#include "string.h"

/**
?* @description: 存储接收到的数据
?* @return {*}
?*/
typedef struct
{
    uint16_t stdId;//报文ID
    uint8_t data[8];//数据长度最大为8
    uint8_t length;//数据实际长度
} RxDataStruct;

void Driver_Can_Init(void);
void CAN_SendMsg(uint16_t stdId, uint8_t *data, uint8_t length);
void CAN_ReceiveMsg(RxDataStruct rxDataStruct[], uint8_t *msgCount);
#endif // !__DRIVER_CAN_H

