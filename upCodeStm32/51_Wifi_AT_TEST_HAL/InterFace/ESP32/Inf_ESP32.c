#include "Inf_ESP32.h"

//接收响应缓冲区和Inf_长度
//接收响应缓冲区和Inf_长度
uint8_t respBuff[1024];
uint16_t respLen;

uint8_t rxBuff[128];/**缓冲一次中断接收的数据 */
uint8_t responseBuff[512];//一次响应
uint16_t rxDataSize;//一次中断接收的数据量
uint16_t responseDataSize;//一次响应接收到数据量

void Inf_ESP32_ReadResp(uint8_t buff[],uint16_t *len){
    //串口2接收变长数据
    HAL_UARTEx_ReceiveToIdle(&huart2,buff,1024,len,1000);
}

void Inf_ESP32_SendCmd(uint8_t *cmd){
    //直接通过串口2发送命令
    HAL_UART_Transmit(&huart2,cmd,strlen((char*)cmd),1000);

    //读取响应
    do{
       Inf_ESP32_ReadResp(respBuff,&respLen);
    }while(strstr((char *)respBuff,"OK")==NULL);

    //将响应打印输出 不用加\r\n指令中本身就有
    printf("%.*s",respLen,respBuff);
    memset(respBuff,0,1024);
}

void Inf_ESP32_Init_NoIT(void){
   //1.初始化串口2
   MX_USART2_UART_Init();
   //2.ESP32复位 硬件复位 软件复位
   uint8_t *cmd="AT+RST=0\r\n";
   Inf_ESP32_SendCmd(cmd);

   //3.延时 确保重启成功 一般是在3s钟以后
   HAL_Delay(3000);
}

void Inf_ESP32_WifiInit(void){
    /**1.设置wifi的工作模式 */
    uint8_t *cmd="AT+CWMODE=1\r\n";
    Inf_ESP32_SendCmd(cmd);
    /**2.设置wifi要连接的AP */
    cmd="AT+CWJAP=\"future\",\"12201220..\"\r\n";
    Inf_ESP32_SendCmd(cmd);
    //HAL_Delay(3000);
    /**3.查看IP地址*/
    cmd="AT+CIPSTA?\r\n";
    Inf_ESP32_SendCmd(cmd);

}

////////////////////////////////////////////////////////
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t size){
    if(huart->Instance==USART2){
       rxDataSize=size;
       //printf("rxBuff=%s\r\n",rxBuff);
       HAL_UARTEx_ReceiveToIdle_IT(&huart2,rxBuff,128);
    }
}

void Inf_ESP32_HandleResponse(){
    //清除缓冲区很关键
    memset(responseBuff,0,responseDataSize);
    responseDataSize=0;
    uint8_t count=3;
    do{
       uint32_t timeOut =0xffffff;
       while(rxDataSize==0&&timeOut--){
        //printf("1\r\n");
       }
       //新收到的数据拷贝到一次响应额缓冲区钟
       memcpy(&responseBuff[responseDataSize],rxBuff,rxDataSize);
       responseDataSize+=rxDataSize;//计算一次响应的数据量
       rxDataSize=0;
    }while(strstr((char *)responseBuff,"OK")==NULL 
           &&strstr((char *)responseBuff,"ERROR")==NULL&&count--);
    printf("%.*s",responseDataSize,responseBuff);
    printf("\r\n=====================\r\n");

    //测试代码      
    /* while(rxDataSize==0);    
    uint16_t tempSize=rxDataSize;
    rxDataSize=0;  
    printf("tempSize=%.*s",tempSize,rxBuff);  */
}

void Inf_ESP32_SendCmd_IT(uint8_t *cmd){
    HAL_UART_Transmit(&huart2,cmd,strlen((char *)cmd),2000);
    Inf_ESP32_HandleResponse();
}

void Inf_ESP32_Init_IT(void){
   //1.初始化串口2
   MX_USART2_UART_Init();

   //使用中断的方式从串口方式接收数据
   HAL_UARTEx_ReceiveToIdle_IT(&huart2,rxBuff,128);

   //2.ESP32复位 硬件复位 软件复位
   Inf_ESP32_SendCmd_IT("AT+RST=0\r\n");

   //3.延时 确保重启成功 一般是在3s钟以后
   HAL_Delay(3000);
}

void Inf_ESP32_WifiInit_IT(void){
    /**1.设置wifi的工作模式 */
    //uint8_t *cmd="AT+CWMODE=1\r\n";
    Inf_ESP32_SendCmd_IT("AT+CWMODE=1\r\n");
    /**2.设置wifi要连接的AP */
    //cmd="AT+CWJAP=\"future\",\"12201220..\"\r\n";
    Inf_ESP32_SendCmd_IT("AT+CWJAP=\"future\",\"12201220..\"\r\n");
    /**3.查看IP地址*/
    //cmd="AT+CIPSTA?\r\n";
    Inf_ESP32_SendCmd_IT("AT+CIPSTA?\r\n");

}
///////////////////////////////////////////////////////


void Inf_ESP32_Init(uint8_t mode){
    if(mode==ESP32_NOIT){
        Inf_ESP32_Init_NoIT();
    }else if(mode==ESP32_IT){
        Inf_ESP32_Init_IT();
    }
}

void Inf_ESP32_TcpServer(){
    /*1.允许多连接*/
    Inf_ESP32_SendCmd_IT("AT+CIPMUX=1\r\n");
    /*2.开启服务器 */
    Inf_ESP32_SendCmd_IT("AT+CIPSERVER=1,8080\r\n");

    /*3.设置IPD的格式  +IPD,0,9,"192.168.168.29",64932:网速快吗?*/
    Inf_ESP32_SendCmd_IT("AT+CIPDINFO=1\r\n");
}

void Inf_ESP32_ReadTcpData(uint16_t *id,uint8_t ip[],uint16_t *port,uint16_t *dataLen,uint8_t data[]){
    /**本质上也是通过串口把数据发送过来   +IPD,0,9,"192.168.168.29",64932:网速快吗?*/
    if(rxDataSize>0){
        if(strstr(rxBuff,"+IPD")){//说明收到TCP传输的数据
            memset(data,0,strlen(data));
            sscanf(rxBuff,"%*[\r\n]+IPD,%d,%d,\"%[^\"]\",%d",id,dataLen,ip,port);
            strtok(rxBuff,":");
            memcpy(data,strtok(NULL,":"),*dataLen);
            printf("Read:id=%d,ip=%s,port=%d,len=%d,data=%.*s\r\n",*id,ip,*port,*dataLen,*dataLen,data);
        }
        //printf("%.*s\r\n",rxDataSize,rxBuff);
        rxDataSize=0;
    }
}

uint8_t txBuff[100];
void Inf_ESP32_SendTcpData(uint16_t id,uint8_t data[],uint16_t dataLen){
    if(dataLen==0){
        return;
    } 
    memset(txBuff,0,strlen(txBuff));
    sprintf(txBuff,"AT+CIPSEND=%d,%d\r\n",id,dataLen);
    printf("txbuff=%s",txBuff);
    Inf_ESP32_SendCmd_IT(txBuff);
    
    /**立即发送数据 就和发送命令一样 */
    //Inf_ESP32_SendCmd_IT("AT+CIPSTA?\r\n");
    HAL_UART_Transmit(&huart2,data,dataLen,2000);
    Inf_ESP32_HandleResponse();
}

