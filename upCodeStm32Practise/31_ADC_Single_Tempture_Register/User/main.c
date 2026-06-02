#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Delay.h"
#include "Driver_ADC.h"
int main(){
	Dri_USART_Init();
    printf("ADC¶ÁÈ¡CPUÎÂ¶È");
    Dri_ADC1_Init();
    Dri_ADC1_StartConvert();
	while (1){
      double CPU_Temp=Dri_ADC1_ReadTemprature();
      printf("CPUÎÂ¶È: %.2f\n",CPU_Temp);
      Delay_s(1);
    }
}



