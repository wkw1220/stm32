#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_Timer3_PWM.h"
#include "Delay.h"



int main(){
	Dri_USART_Init();
	printf("µ÷½ÚLCD±³¹â!\r\n");
	//Dri_Led_Init();
	Dri_Timer3_Pwm_Init();
	Dri_Timer3_Pwm_Start();
    uint8_t dutyCycle=0;
	uint8_t dir=0;
    while(1){
		if(dir==0){
		 dutyCycle=dutyCycle+1;
		 if(dutyCycle>=98){
			dir=1;
		 }
		}else if(dir==1){
		 dutyCycle=dutyCycle-1;
		 if(dutyCycle<=1){
			dir=0;
		 }
		} 
		Dri_Tim3_Pwm_SetDutyCycle(dutyCycle);
		Delay_ms(15);
	}
}



