
#include "stdint.h"
#include "stm32f10x.h" 
#include "Driver_Led.h" 
#include "Delay.h"  
#include "Driver_Usart.h"
#include "Driver_Usart2.h" 

#include "stdio.h" 

uint32_t Leds[]={LED1_ORANGE,LED2_BLUE,LED3_GREEN};
//extern  buff[100];
//extern  len;
//extern  isReciveOver;
uint8_t buffTemp[100];

extern uint8_t buff_Usart2[100];
extern uint8_t len_Usart2;
extern uint8_t isReciveOver_Usart2;
int main(){
	//Dri_USART1_Init();
	Dri_USART2_Init_WithInterruput();
	printf("1111\r\n");
	Dri_USART2_SendStr("buff_Usart2");
	//Dri_USART2_Init_WithInterruput();
	while(1){
		//Dri_USART1_ReceiveStr(buff_Usart2);
		//Dri_USART1_SendStr(buff_Usart2);
		//memset(buff_Usart2, 0, sizeof(buff_Usart2));
		/* if(isReciveOver){
          Dri_USART1_SendStr(buff);
          memset(buff, 0, sizeof(buff));
		  isReciveOver=0;
		  len=0; 
		} */
        if(isReciveOver_Usart2){
          Dri_USART2_SendStr(buff_Usart2);
          memset(buff_Usart2, 0, sizeof(buff_Usart2));
		  isReciveOver_Usart2=0;
		  len_Usart2=0; 
		}

	}


}


