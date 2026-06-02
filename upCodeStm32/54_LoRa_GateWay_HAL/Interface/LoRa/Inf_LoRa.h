#ifndef __INF_LOAR_H__
#define __INF_LOAR_H__
#include "usart.h"
#include "ebyte_core.h"

void Inf_LoRa_init();
void Inf_LoRa_ReadData();
void Inf_LoRa_SendData(uint8_t *data,uint16_t dataLen);

/* void LoRa_TransmitSuccessCallBack();
void LoRa_TransmitTimeOutCallBack();
void LoRa_TransmitOtherCaseCallBack();
void LoRa_ReceiveSuccessCallBack(uint8e_t *buffer, uint8e_t length);
void LoRa_ReceiveTimeOutCallBack();
void LoRa_ReceiveOtherCaseCallBack(); */
#endif 

