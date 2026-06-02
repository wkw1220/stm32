#include "Driver_Timer5.h"
#include "Driver_Led.h"
#include "string.h"

/**
 * @brief 通用定时器timer5 初始化
 *   实现基本计数功能
 *  TIM2 TIM3 TIM4 TIM5一共四个通用定时器
 *  通用定时器一般对应四个通道，分别对应四个物理引脚。
 *  可以接外部时钟源，但是只有通道1和通道2做外部时钟源，其它两个不行
 *  四个通道即能做输入，也能做输出，但同一时间只能选择其中一个
 */
void Driver_Timer5_Init(void)
{
    /* 1 给定时器5开启时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    /* 2.设置预分频值:分频值7199表示7200分频。分频后频率10K，周期100us  */
    TIM5->PSC =7200-1;
    /*
    3. 设置自动重转载寄存器的值，决定中断发生的频率。
        假设设置为9999，表示计数器计数10000次发生一次中断。
        计数一次100us，10000次1000000us，正好1s
    */
    TIM5->ARR=10000-1;

    /* 为了避免一启动立即进入更新中断，可以先产生一个更新事件 顺序不能错*/
    TIM5->CR1  |= TIM_CR1_URS;
    TIM5->EGR |= TIM_EGR_UG;
    //TIM5->SR  &= ~TIM_SR_UIF;
   
    /* 4.使能更新中断 */
    TIM5->DIER |= TIM_DIER_UIE;

    /* NVIC 设置 */
    /* 5.设置中断优先级分组 */
    NVIC_SetPriorityGrouping(3);
    /* 6.设置中断优先级 */
    NVIC_SetPriority(TIM5_IRQn, 1);
    /* 7.使能定时器6的中断 */
    NVIC_EnableIRQ(TIM5_IRQn);

    /* 8.使能计数器 */
    TIM5->CR1 |= TIM_CR1_CEN;

    
}

/**
 * @description: TIM5的中断服务函数
 * @return {*}
 */
uint8_t isReady=0;
void TIM5_IRQHandler(void)
{
  TIM5->SR &= ~TIM_SR_UIF;
  isReady=1;
  //Dri_Led_Toggle(LED2);
  //纯测试代码
  //printf("1s\r\n");
}
