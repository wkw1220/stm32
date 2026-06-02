#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include <string.h>
#include "Driver_Led.h"
#include "Driver_bkp.h"

int main(){
	Dri_USART_Init_WithInterruput();
	Dri_Led_Init();
	Dri_Led_Off(LED1|LED2|LED3);
	Dri_Led_On(LED3);
	Dri_BKP_Init();
	//Dri_BKP_WriteData(88);
	u16 data=Dri_BKP_ReadData();
	printf("BKP DR1 data=%d\r\n",data);
	while (1){
		
	}
}



