#include "stm32f10x.h"                  
#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_SysTick.h"



/**
 * @brief 所有初始化统一放在这里处理
 * 
 */
void init(void){
    Dri_USART_Init();
	Dri_Led_Init();
	//Driver_SysTick_Init();
	Driver_SysTick_Init_V2();
}

/**
 * @brief 通过系统定时器sysTick 实现Led灯闪烁功能
 * 
 * @return int 
 */
int main(){
	init();
	printf("sysTick success!\r\n");
    while(1);
}



