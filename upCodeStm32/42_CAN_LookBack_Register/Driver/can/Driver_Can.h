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

/**
 * @brief 
 * 在电路板上，差分信号走线必须是等长，等宽，紧密靠近，且在同一层面的两根线
 * 
 * CAN发送信息(数据帧)，所有结点都会收到信息，无法将报文单独发送给指定的节点，所有
 * 节点都将捕获所有报文
 * 
 * 在stm32芯片内部会集成一个CAN控制器，查看数据手册的引脚定义，CAN_RX==>PA11
 * CAN_TX==>PA12,如果直接使用PA11和PA12，直接使用默认的引脚复用功能就可以了
 * 但是查看手册 我们使用了PB8和PB9 
 *  解决办法有两种:
 * 1.PA11和PA12 用杜邦线和PB8和PB9连接 
 * 2.学习新知识点，引脚重映射功能
 * 
 * 
 * 寄存器版本移植的时候注意的地方
 * 1.引脚的配置
 * 2.工作模式的配置
 * 3.过滤器组的配置
 * 
 */
