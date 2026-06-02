#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include <string.h>

/**
 * @brief 使用中断的方式接收字符串
 *  一般情况下使用中断的方式来实现字符串的接收
 * 
 */
extern  buff[100];
extern  len;
extern  isReadySend;
int main(){
	Dri_USART_Init_WithInterruput();
	printf("usart1 test.....\n");
	while (1){
		if(isReadySend){
          Dri_USART_SendStr(buff);
          memset(buff, 0, sizeof(buff));
		  isReadySend=0;
		  len=0; 
		}
	}
}



