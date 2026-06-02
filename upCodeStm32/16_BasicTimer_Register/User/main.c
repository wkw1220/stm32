#include "Driver_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_Timer6.h"

extern uint8_t isReady;
extern uint8_t isReadySystick;
int main(){
	Dri_USART_Init_WithInterruput();
	printf("send success!\r\n");
	Dri_Led_Init();
	Driver_SysTick_Init();
	Driver_Timer6_Init();
    while(1){
		/* if(isReady){
			Dri_Led_Toggle(LED_GREEN);
			isReady=0;
		}
		if(isReadySystick){
			Dri_Led_Toggle(LED_BLUE);
			isReadySystick=0;
		} */
	}
}



