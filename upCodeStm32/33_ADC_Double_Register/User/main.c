#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Delay.h"
#include "Driver_ADC.h"
uint16_t data[2]={0};
uint8_t dataLen=2;
void initADC(){
    Dri_ADC1_Init_MultiChannel();
    Dri_ADC1_DMA_Init();
    Driver_ADC1_DMA_Start((uint32_t)data, dataLen);
}
int main(){
	Dri_USART_Init();
    printf("ADC多通道转换实验.....读电压\n");
    initADC();
	while (1){
      printf("滑动变阻器=%.4f,电源电压=%.4f\r\n",(data[0]*3.3)/4095,(data[1]*3.3)/4095);
      //printf("滑动变阻器=%.4f,电源电压=%.4f\r\n",(data[2]*3.3)/4095,(data[3]*3.3)/4095);
      Delay_s(1);
    }
}



