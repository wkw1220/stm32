#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Delay.h"
#include "Driver_ADC.h"
int main(){
	Dri_USART_Init();
    printf("ADC转换实验.....读电压");
    Dri_ADC1_Init();
    Dri_ADC1_StartConvert();
	while (1){
      double v=Dri_ADC1_ReadVolate();
      printf("v=%.2f\r\n",v);
      Delay_s(1);
    }
}



