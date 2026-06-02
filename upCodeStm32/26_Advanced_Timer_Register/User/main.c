#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_Timer5_PWM.h"
#include "Driver_Timer4_PWM.h"
#include "Delay.h"



int main(){
	Dri_USART_Init();
	printf("高级定时器\r\n");
    Dri_Timer1_Init();
	Dri_Timer1_Start();
    while(1){

	}
}



