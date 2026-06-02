#include "Inf_LoRa.h"

void Inf_LoRa_init()
{
    Ebyte_RF.Init();
}

void Inf_LoRa_ReadData()
{
    Ebyte_RF.StartPollTask();
}

void Inf_LoRa_SendData(uint8_t *data,uint16_t dataLen)
{
    /**超时时间必须是0 意义何在??? 超时时间不限?? */
    Ebyte_RF.Send(data,dataLen,0);
}

void LoRa_TransmitSuccessCallBack()
{
    printf("LoRa send sucess\r\n");
}

void LoRa_TransmitTimeOutCallBack()
{
    printf("LoRa send time out\r\n");
}

void LoRa_TransmitOtherCaseCallBack()
{
}

void LoRa_ReceiveSuccessCallBack(uint8e_t *buffer, uint8e_t length)
{
    printf("It's received data:%.*s\r\n",length,buffer);
    Inf_LoRa_SendData("ok\r\n",4);
}

void LoRa_ReceiveTimeOutCallBack()
{
}

void LoRa_ReceiveOtherCaseCallBack()
{
}
