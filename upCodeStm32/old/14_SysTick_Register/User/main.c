#include "stm32f10x.h"                  
#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_SysTick.h"

int main(){
	Dri_USART_Init_WithInterruput();
	printf("send success!");
	Dri_Led_Init();
	Driver_SysTick_Init();
    while(1);
}



