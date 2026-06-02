#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_Timer5.h"

extern uint8_t isReady;

int main(){
	Dri_USART_Init_WithInterruput();
	printf("通用定时器基本功能--计数!\r\n");
	Dri_Led_Init();
	Driver_Timer5_Init();
    while(1){
		if(isReady){
			Dri_Led_Toggle(LED2);
			isReady=0;
			//printf("1s\r\n");
		}
	}
}



