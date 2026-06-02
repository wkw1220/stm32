#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include <string.h>
#include "Driver_Led.h"
#include "Driver_Key.h"

/**
 * @brief 内核控制的 在CM3编程文档里
 * 
 */
void EnterStandByMode(void){
  //设置待机模式
  SCB->SCR|=SCB_SCR_SLEEPDEEP_Msk; //使能深度睡眠模式
  //RCC->APB1ENR|=RCC_APB1ENR_PWREN; //使能电源接口时钟 这行代码不能写在这里
  PWR->CR|=PWR_CR_PDDS;          //设置待机模式

  //使能PA0唤醒引脚，给PA0一个上升沿就会唤醒待机
  PWR->CSR|=PWR_CSR_EWUP;     
  
  __WFI(); //执行等待中断指令，进入待机模式
}


/**
 * @brief 进入睡眠模式以后，再次唤醒以后，程序会从上次睡眠的地方处继续执行
 */
int main(){
	Dri_USART_Init_WithInterruput();
	Dri_Key_Init();
	Dri_Led_Init();
	Dri_Led_Off(LED1|LED2|LED3);
	Dri_Led_On(LED3);
    
	/**
	 * 使能电源接口时钟 这行代码必须写在main函数里
	 * 因为进入待机模式以后，所有的外设时钟都会被关闭，包括RCC时钟
	*/
	RCC->APB1ENR|=RCC_APB1ENR_PWREN; 
	//判断是否是从待机模式唤醒的
	if((PWR->CSR&PWR_CSR_SBF)!=0){ 
		//清除待机唤醒标志位
		PWR->CR|=PWR_CR_CSBF; 
		printf("Wake up from Standby Mode\r\n");
	}else{
		printf("Wake up from normal\r\n");
	}

	if((PWR->CSR&PWR_CSR_WUF)!=0){ 
		//清除待机唤醒标志位
		PWR->CR|=PWR_CR_CWUF; 
		printf("Wake up from PA0 \r\n");
	}else{
		printf("Power on Reset\r\n");
	}
	while (1){
		printf("Enter standby mode......\n");
		Delay_s(5);
		EnterStandByMode();
		//Delay_s(1);
		printf("Wake up......\n");
	}
}



