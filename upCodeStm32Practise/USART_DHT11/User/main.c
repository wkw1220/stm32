#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include <string.h>
#include "Dri_DHT11.h"
uint8_t result[4]={0};
int main(){
	Dri_USART_Init();
	printf("star....\n");
	while (1)
	{
		//receiveOneByte();
		receiveOneBytes(result);
		printf("Êª¶È:%d.%dRH  ",result[0],result[1]);
		printf("ÎÂ¶È:%d.%d¡æ\n",result[2],result[3]);
		memset(result, 0, sizeof(result));
		Delay_s(2);
	} 
}



