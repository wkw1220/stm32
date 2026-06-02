
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
}

void Inf_SendAtCmd(uint8_t *atCmd)
{
   memset(responseBuff,0,responseDataSize);
   responseDataSize=0;
   rxDataSize=0;
   HAL_UART_Transmit(&huart2,atCmd,strlen((char *)atCmd),2000);
   uint8_t count=6;
   do{
    uint32_t timeOut=0xfffff;
     while(rxDataSize==0 && timeOut--)
     {
       //printf("111111............\r\n");
     }
     /* 新收到的数据拷贝到一次响应的缓冲区中 */
     memcpy(&responseBuff[responseDataSize],rxBuff,rxDataSize);
     responseDataSize += rxDataSize;
     rxDataSize=0; 
   } while(strstr((char *)responseBuff,"OK")==NULL && strstr((char *)responseBuff,"ERROR") && count--);
    printf("%.*s",responseDataSize,responseBuff);
    printf("\r\n============11111===========\r\n");
}


void Inf_ESP32_BlurTeethInit()
{
    /* 1. 初始化ESP32 */
    Inf_ESP32_Init();

    /* 2. 设置蓝牙角色  0: 注销 Bluetooth LE 1: client 角色 2: server 角色*/
    Inf_SendAtCmd("AT+BLEINIT=2\r\n");

    /* 3. 服务端创建服务*/
    printf("3--服务端创建服务\r\n");
    Inf_SendAtCmd("AT+BLEGATTSSRVCRE\r\n");

    /* 4. 服务端开启服务*/
    printf("4--服务端开启服务\r\n");
    Inf_SendAtCmd("AT+BLEGATTSSRVSTART\r\n");

    /* 5. 设置设备名称 */
    Inf_SendAtCmd("AT+BLENAME=\"future-bt\"\r\n");

    /*
        6. 设置蓝牙广播参数
            AT+BLEADVPARAM=<adv_int_min>,<adv_int_max>,<adv_type>,<own_addr_type>,<channel_map>

               a： adv_int_min adv_int_max 广播的最小间隔和最大间隔。 等于该值乘上0.625ms
               b： adv_type                广播类型   0：通用广播
               c： own_addr_type           Bluetooth LE 地址类型  0：公共地址 1：随机地址
               d： channel_map             广播信道   1: ADV_CHNL_37 2: ADV_CHNL_38
                                                     4: ADV_CHNL_39 7: ADV_CHNL_ALL

    */
    
  Inf_SendAtCmd("AT+BLEADVPARAM=50,50,0,0,7,0,,\r\n");
  /*
        7. 设置广播数据并开始广播
            AT+BLEADVDATAEX=<dev_name>,<uuid>,<manufacturer_data>,<include_power>
  */
  Inf_SendAtCmd("AT+BLEADVDATAEX=\"future-bt\",\"A123\",\"0102030405\",1\r\n");
  printf("8--\r\n");
  Inf_SendAtCmd("AT+BLEADVSTART\r\n");

  /*
       8. 配置 Bluetooth LE SPP(Serial Port Profile)
           选择支持 notify 或者 indicate 的 characteristic 作为写通道发送数据，
           选择支持写操作的 characteristic 作为读通道接收数据。
    */
  Inf_SendAtCmd("AT+BLESPPCFG=1,1,7,1,5\r\n");

  /*
        9. 透传模式下，Wi-Fi、socket、Bluetooth LE 或 Bluetooth 状态改变时会打印提示信息
    */
 printf("9--end\r\n");
Inf_SendAtCmd("AT+SYSMSG=4\r\n");

}

void Inf_ESP32_BLEReadData()
{
    if(rxDataSize==0)
    {
        return;
    }
    printf("%.*s\r\n",rxDataSize,rxBuff);
    rxDataSize=0;
}
