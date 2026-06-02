/**
 * 程序功能:按键按下,按键产生中断，通过中断的方式控制LED的翻转
 */
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Driver_Led.h"
#include "Driver_Key.h"
uint32_t Leds[]={LED1,LED2,LED3};
uint8_t counter=0;
extern uint8_t direct_flag;
int main(){
	Dri_Led_Init(Leds,3);
	Dri_Key_Init();
	while(1){
      Dri_Led_On(Leds[counter]);
	  /**
	   * @brief 这里使用定时器，但是在key中断函数里也使用了定时器，
	   *        中断函数定时器执行完毕会关闭定时功能，会导致这里的定时器
	   *        一直在循环，会卡死。所以要取消定时器里关闭定时的功能。
	   *   hal库就没有这个问题，自己分析.... 
	   */
	  Delay_s(1);
	  Dri_Led_Off(Leds[counter]);
	  if(direct_flag){
        counter=(counter+2)%3;//取值为 n-1
	  }else{
		counter=(counter+1)%3;
	  }

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




