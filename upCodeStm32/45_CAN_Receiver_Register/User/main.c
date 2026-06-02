#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include <string.h>
#include "Driver_Can.h"
#include "Driver_Led.h"

/* 1. 接收数据 */
RxDataStruct rxDataStruct[3];//最多为3
uint8_t rxMsgCount;
int main(){
	  Dri_USART_Init();
    printf("CAN 通讯实验: 接收 寄存器版\r\n");
    Driver_Can_Init();
    printf("CAN 初始化配置完成...\r\n");
    Dri_Led_Init();
    //Dri_Led_Off(LED3|LED2|LED1);
    //Dri_Led_On(LED2);
	while (1){
      CAN_ReceiveMsg(rxDataStruct, &rxMsgCount);
      /* 2. 输出消息 */
      uint8_t i;
      for (i = 0; i < rxMsgCount; i++)
      {
        RxDataStruct msg = rxDataStruct[i];
        //printf("stdId = 0x%x, length = %d, msgData = %s\r\n", msg.stdId, msg.length, msg.data);
         int num1 = atoi(msg.data);
         int temp=num1%2;
         printf("num1=%d,temp=%d\r\n",num1,temp);
         if(num1 % 2 == 0){
          printf("LED2 Toggle\r\n");
         }else if(num1 % 2 == 1){
          printf("LED3 Toggle\r\n");
         }
      }
	}
}



