#ifndef __DRIVER_ADC_H__
#define __DRIVER_ADC_H__
#include "stm32f10x.h" 
void Dri_ADC1_Init(void);
void Dri_ADC1_StartConvert(void);
double Dri_ADC1_ReadVolate(void);
void Dri_ADC1_Init_MultiChannel(void);
void Dri_ADC1_DMA_Init(void);
void Driver_ADC1_DMA_Start(uint32_t desAddr, uint8_t dataLen);
#endif // !__DRIVER_ADC_H__