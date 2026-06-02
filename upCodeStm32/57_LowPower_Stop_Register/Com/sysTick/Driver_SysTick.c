#include "Driver_SysTick.h"
#include "Driver_Led.h"
/**
 * @brief 系统时钟(嘀嗒定时器)时钟定时器初始化
 * 注意：滴答定时器
 * 
 */
void Driver_SysTick_Init(void)
{
    /* 1.配置时钟源 1=AHB(72MHZ) 0=AHB/8 */
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE;
    /* 2.使能中断 */
    SysTick->CTRL |= SysTick_CTRL_TICKINT;

    /**
     * 中断优先级没有配置:使用默认值，可以设置
     * 没有使能中断:系统定时器在内核，不用使能
     */
    
    /* 3.定时器1ms产生一次中断 72M=72000000(把这个值写到寄存里会溢出，
    所以要变通),也就是1次是1/72微秒,72次是1us，72000是一毫秒，Load里的值就是，
    这个值递减到零时就会产生一次中断，然后重装载这个这个值S*/
    SysTick->LOAD=LOAD_VLUE-1;
    /* 4使能定时器 */
    SysTick->CTRL |= SysTick_CTRL_ENABLE;

    //NVIC是不需要配置的，模式时开启的
    //NVIC_SetPriorityGrouping(3);
    //NVIC_SetPriority(SysTick_IRQn,3);
    //NVIC_EnableIRQ(SysTick_IRQn);//这行代码是不能放开的。
}

/**
 * @brief 系统提供的一行代码搞定
 * 
 */
void Driver_SysTick_Init_V2(void){
  SysTick_Config(LOAD_VLUE);
}

/**
 * @brief SysTick中断服务函数
 * 每1ms产生一次中断，计数1000次就是ls
 * 
 * 中断标志位是：COUNTFLAG,重装载的时候会自动清除，不需要手动清除，所以这里代码不需要体现
 * 
 */
uint16_t count=0;
void SysTick_Handler(void){
   count++;
   if(count==1000){
     count=0;
     Dri_Led_Toggle(LED1);
   }
}

