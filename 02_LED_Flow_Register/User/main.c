
/**
 * @file main.com
 * @author windear
 * @brief 
 * @version 0.1
 * @date 2024-10-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/**
 * 专业术语汇总:
 * ARMCC: ARM Complier for C 支持C语言的arm编译器
 * RCC: Reset and Clock Control 复位和时钟控制器
 * AHB: Advanced High-performance Bus 高级高性能总线
 * APB: Advanced Peripheral Bus 高级外设总线
 * ODR: Output Data Regiseter 输出数据寄存器
 */

/**
 * CPU(72M)---->AHB(72M)----------->APB1(36M)
 *                       |     |
 *                      RCC    ----->APB2(72M)  为什么要挂两个外设总线。驱动能力强的
 *                                              挂在APB2上，驱动能力弱的挂在APB1上
 */

/** 
 * 掩码:mask，在编程和计算机科学中是一个二进制模式，用于选择性地操作数据的某些位。
 *      掩码本质上是一个二进制数，它的每一位通过设置为1或0来控制我们是否要操作
 *      目标数据中的对应位
 * 掩码的名字来源于其功能:它像一张"面具"或"过滤器",只让我们想要的位通过，并屏蔽其它位
 *  
 * 1.想取某位状态:按位与& 取出某位状态
 * 2.某位置为1,只需 |=
 * 3.某位置为0,只需 &= ~
 * 4.某位翻转:异或 ^=
 *  
 */

/**
 * GPIO的输入输出
 *   输出:stm32芯片能够主动决定引脚的电平
 *   输入:外界能够决定stm32引脚芯片的电平
 */
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Driver_Led.h"
uint32_t Leds[]={LED1,LED2,LED3};

int main(){
	
    Dri_Led_Init();
	while(1){
		Delay_ms(500);
		Dri_Led_Off(Leds[0]);
		Delay_ms(1000);
		Dri_Led_On(Leds[0]);
	}
	/* Delay_s(1);
	Dri_Led_Off(Leds[0]);
	while(1){
	  u8 i;
	  u8 temp;
	  temp=size-2;
	  for (i = 1; i < size; i++)
	  {
		if(fangXiang==0){
          Dri_Led_On(Leds[i]);
		}else{
		  Dri_Led_On(Leds[temp--]);
		}
		Delay_s(1);
		Dri_Led_OffAll(Leds,3);
	  }
	  fangXiang=~fangXiang;
	  tempCloseRcc++;
	  if(tempCloseRcc>=10){
		RCC->APB2ENR &= ~RCC_APB2ENR_IOPAEN;
	  }
	}  */
}



