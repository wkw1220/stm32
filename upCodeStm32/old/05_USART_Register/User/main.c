#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include <string.h>

extern  buff[100];
extern  len;
extern  isReadySend;
int main(){
	Dri_USART_Init_WithInterruput();
	while (1){
		if(isReadySend){
          Dri_USART_SendStr(buff);
          memset(buff, 0, sizeof(buff));
		  isReadySend=0;
		  len=0; 
		}
	}

	//uint8_t* str="Hello World";
	//u8 buff[100]={0};
	//while (1)
	//{
		//Dri_USART_SendStr(str);
		//uint8_t c=Dri_USART_ReceiveChar();
		//Dri_USART_SendChar(c);

		//Dri_USART_ReceiveStr(buff);
		//Dri_USART_SendStr(buff);
		//memset(buff, 0, sizeof(buff));
	//}
	
}



