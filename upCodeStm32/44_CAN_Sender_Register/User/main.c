#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include <string.h>
#include "Driver_Can.h"


int main(){
	Dri_USART_Init();
    printf("CAN 通讯实验: 静默回环 寄存器版\r\n");
    Driver_Can_Init();
    printf("CAN 初始化配置完成...\r\n");

    uint16_t stdId = 0x066;
    uint8_t *tData = "abcdefg";//长度最多为8
	//uint8_t tData[8]={'a','b','c','d','e','f','g','h'};
    CAN_SendMsg(stdId, tData, strlen((char *)tData));
    printf("发送完毕...%d\r\n",strlen((char *)tData));

    tData = "123";
    CAN_SendMsg(stdId, tData, strlen((char *)tData));
    printf("发送完毕...\r\n");
 
	tData = "7890";
    CAN_SendMsg(stdId, tData, strlen((char *)tData));
    printf("发送完毕...\r\n");

    stdId = 0x067;
    tData = "xyz";
    CAN_SendMsg(stdId, tData, strlen((char *)tData));
    printf("发送完毕...\r\n");   

    /* 1. 接收数据 */
    RxDataStruct rxDataStruct[3];//最多为3
    uint8_t rxMsgCount;
    CAN_ReceiveMsg(rxDataStruct, &rxMsgCount);
    printf("接收完毕 rxMsgCount = %d...\r\n", rxMsgCount);

    /* 2. 输出消息 */
    uint8_t i;
    for (i = 0; i < rxMsgCount; i++)
    {
      RxDataStruct msg = rxDataStruct[i];
      printf("stdId = 0x%x, length = %d, msgData = %s\r\n", msg.stdId, msg.length, msg.data);
    }
	while (1){

	}
}



