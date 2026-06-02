/**
 * 程序功能:按键按下,按键产生中断，通过中断的方式控制LED的翻转
 */
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Driver_Led.h"
#include "Driver_Key.h"
#include "Driver_Iwdg.h"
#include "Dri_usart.h"
uint32_t Leds[]={LED1,LED2,LED3};

void EnterStandByMode(void){
  //设置待机模式
  SCB->SCR|=SCB_SCR_SLEEPDEEP_Msk; //使能深度睡眠模式
  RCC->APB1ENR|=RCC_APB1ENR_PWREN; 
  PWR->CR|=PWR_CR_PDDS;          //设置待机模式

  //使能PA0唤醒引脚，给PA0一个上升沿就会唤醒待机
  PWR->CSR|=PWR_CSR_EWUP;     
  
  __WFI(); //执行等待中断指令，进入待机模式
}
int main(){
	Dri_USART_Init();
	printf("start........\r\n");
	Dri_Led_Init(Leds,3);
	Dri_Key_Init();
	Dri_Iwdg_Init(4,2499); //看门狗初始化 设置1秒溢出
	
	extern uint8_t flag;
  while (1){
	Dri_Led_Toggle(LED1);
	Dri_Led_Toggle(LED2);
	Dri_Led_Toggle(LED3);
	printf("进入待机模式\r\n");
	Delay_ms(1000);
	//EnterStandByMode();
	printf("Main Running ending....\r\n");
	//Dri_Iwdg_Feed(); //喂狗
	
  }
}
/**
 * @brief 程序启动说明及文件顺序
 * 
 *  程序启动加载startup_stm3210x_hd.s汇编文件,在148行，系统一上电或者服务
 *  执行Reset_Handler 过程调用。可以看到先执行SystemInit系统初始化函数然后
 *  再执行__main函数
 *  SystemInit函数在哪?
 *  在system_stm32f10x.c文件中,(这个文件一定要仔细阅读文件说明),提供了两个函数SystemInit()
 *  和SystemCoreClockUpdate()和一个全局变量SystemCoreClock,通过阅读函数，发现这里就是
 *  默认设置了时钟，没有让我们去配置。
 *  系统初始化函数执行完了,会调用core_cm3.c中的内容：这里面是内核的宏和内核函数，例如systick，NVIC
 *  然后stm3210x.h全是用到的引用头文件 然后这些初始化好了就执行main函数
 *  文件执行的大致流程startup_stm3210x_hd.s--->system_stm32f10x.c--->core_cm3.c--->stm3210x.h--->main.c
 */




