#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_Timer6.h"


int main(){
	Dri_USART_Init_WithInterruput();
	printf("send success!\r\n");
	Dri_Led_Init();
	Driver_Timer6_Init();
    while(1);
}



