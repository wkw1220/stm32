#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_Timer5_PWM.h"
#include "Delay.h"



int main(){
	Dri_USART_Init();
	printf("通用定时器PWM功能!\r\n");
	Dri_Led_Init();
	Dri_Timer5_Pwm_Init();
	Dri_Timer5_Pwm_Start();
    uint8_t dutyCycle=0;
	uint8_t dir=0;
	uint8_t step=1;
    while(1){
		if(dir==0){
		 dutyCycle=dutyCycle+step;
		 if(dutyCycle>=99){
			dir=1;
		 }
		}else if(dir==1){
		 dutyCycle=dutyCycle-step;
		 if(dutyCycle<=1){
			dir=0;
		 }
		} 
		Dri_TIM5_Pwm_SetDutyCycle(dutyCycle);
		Delay_ms(20);
	}
}



