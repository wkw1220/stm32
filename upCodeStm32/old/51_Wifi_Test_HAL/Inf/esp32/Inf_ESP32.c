
#include "Inf_ESP32.h"
uint8_t rxBuff[128];//缓冲一次中断接收的数据
uint8_t responseBuff[512];/* 一次响应 */
uint16_t rxDataSize;/* 一次中断接收的数据 */
uint16_t responseDataSize;/* 一次响应接收的数据 */

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t size)
{
   if(huart->Instance ==USART2)
   {
      rxDataSize=size;
      HAL_UARTEx_ReceiveToIdle_IT(&huart2,rxBuff,128);
   }
}
void Inf_ESP32_Init()
{
    /* 1.初始化串口2 */
    MX_USART2_UART_Init();

    /* 2.用中断的方式从串口2接收数据 */
    HAL_UARTEx_ReceiveToIdle_IT(&huart2,rxBuff,128);

    /* 2.重启ESP32 */
    Inf_SendAtCmd("AT+RST=0\r\n");

    HAL_Delay(3000);

    /* 3. 测试AT启动*/
    Inf_SendAtCmd("AT\r\n");
    //Inf_SendAtCmd("AT+GMR\r\n");

    Inf_ESP32_WifiInit();

    Inf_ESP32_TCPServerInit();
  
}

void Inf_SendAtCmd(uint8_t *atCmd)
{
   memset(responseBuff,0,responseDataSize);
   responseDataSize=0;
   rxDataSize=0;
   HAL_UART_Transmit(&huart2,atCmd,strlen((char *)atCmd),2000);
   //rxDataSize==0 ，表示没有响应 ，一直等待
   /*while(rxDataSize==0)
   {
       //printf("111111............\r\n");
   }

   uint16_t tempSize=rxDataSize;
   rxDataSize=0;
   printf("%.*s",tempSize,rxBuff);
   printf("\r\n=======================\r\n");*/
   uint8_t count=6;
   do{
    uint32_t timeOut=0xffffff;
     while(rxDataSize==0 && timeOut--)
     {
       printf("111111............\r\n");
     }
     /* 新收到的数据拷贝到一次响应的缓冲区中 */
     memcpy(&responseBuff[responseDataSize],rxBuff,rxDataSize);
     responseDataSize += rxDataSize;
     rxDataSize=0; 
   } while(strstr((char *)responseBuff,"OK")==NULL && strstr((char *)responseBuff,"ERROR") && count--);
    printf("%.*s",responseDataSize,responseBuff);
    printf("\r\n============11111===========\r\n");
}

void Inf_ESP32_WifiInit()
{
    /* 1.设置工作模式 */
    Inf_SendAtCmd("AT+CWMODE=1\r\n");
    /* 2.设置要连接的AP */
    Inf_SendAtCmd("AT+CWJAP=\"future\",\"wobuzhidao703.\"\r\n");
    /* 3.查看IP地址 */
    HAL_Delay(5000);
    Inf_SendAtCmd("AT+CIPSTA?\r\n");
}

void Inf_ESP32_TCPServerInit()
{
    /* 1.允许多连接 */
    Inf_SendAtCmd("AT+CIPMUX=1\r\n");
    /* 2.开启TCP服务器 */
    Inf_SendAtCmd("AT+CIPSERVER=1,8080\r\n");

    /* 3.设置IPD的格式 */
    Inf_SendAtCmd("AT+CIPDINFO=1\r\n");
}

//+IPD,0,4,"192.168.0.102",54236:56
void Inf_ESP32_ReadTcpData(uint16_t *id,uint8_t ip[],uint32_t *port,uint16_t *dataLen,uint8_t data[])
{
    if(rxDataSize>0)
    {
        printf("%.*s",rxDataSize,rxBuff);
        
        if(strstr(rxBuff,"+IPD"))
        {    
            memset(data,0,strlen(data));
            sscanf(rxBuff,"%*[\r\n]+IPD,%d,%d,\"%[^\"]\",%d",id,dataLen,ip,port); //port 为什么用 uint16_t 不行????
            //sscanf(rxBuff,"%*[\r\n]+IPD,%d,%d,\"%[^\"]\"",id,dataLen,ip);
            printf("dataLen=%d\r\n",*dataLen);
            strtok(rxBuff,":");
            memcpy(data,strtok(NULL,":"),*dataLen);
            printf("id=%d,ip=%s,port=%d,len=%d,data=%.*s\r\n",*id,ip,*port,*dataLen,*dataLen,data);
        }
        //printf("%.*s",rxDataSize,rxBuff);
        rxDataSize=0;
    }
}

void Inf_ESP32_SendTcpData(uint16_t id,uint16_t dataLen,uint8_t data[])
{
   uint8_t sendCmd[100];
   sprintf((char *)sendCmd, "AT+CIPSEND=%d,%d\r\n", id, dataLen);
   Inf_SendAtCmd(sendCmd);
   HAL_UART_Transmit(&huart2, data, dataLen, 1000);
}

