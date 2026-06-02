#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include <string.h>
#include "Driver_Led.h"

/**
 * @brief 内核控制的 在CM3编程文档里
 * 
 */
void EnterSleepMode(void){
  /* 1. 设置普通睡眠模式 默认值就是零 可以不配置*/
  SCB->SCR &=~SCB_SCR_SLEEPDEEP;
  SCB->SCR &=~SCB_SCR_SLEEPONEXIT;

  /* 2. 进入睡眠模式，等待中断唤醒，可以靠任意中断唤醒 这是一条汇编语言 */
  __WFI();
}

/**
 * @brief 进入睡眠模式以后，再次唤醒以后，程序会从上次睡眠的地方处继续执行
 */
extern  buff[100];
extern  len;
extern  isReadySend;
int main(){
	Dri_USART_Init_WithInterruput();
	Dri_Led_Init();
	Dri_Led_Off(LED1|LED2|LED3);
	//Dri_Led_On(LED1);
	uint8_t flag=1;
	while (1){
		/**
		 * @brief 本来是想通过发送字符串来控制睡眠和唤醒，但是真正执行时可能由于还每唤醒导致
		 *        在睡眠状态下收到字符串有异常无法唤醒。这种方式无法实现
		 * 
		 */
		printf("start.....\n");
		if(isReadySend){
          Dri_USART_SendStr(buff);
		  if(strcmp(buff,"sleep")==0){
			printf("sleep.....\n");
			flag=1;
		  }else if(strcmp(buff,"wake")==0){
			printf("wake.....\n");
			flag=2;
		  }
          memset(buff, 0, sizeof(buff));
		  isReadySend=0;
		  len=0; 
		}

        if(flag==1){
		  printf("Enter sleep mode.....\n");
          Delay_s(1);
		  Dri_Led_Off(LED2);
		  //EnterSleepMode();
		}else if(flag==2){
          Delay_s(1);
		  printf("Wake up sleep mode....\n");
		  Dri_Led_On(LED2);
		}
		
	}
}



