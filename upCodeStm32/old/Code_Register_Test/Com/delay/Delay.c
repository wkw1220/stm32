#include "Delay.h" // Device header

/**
 * @brief 这个延时函数千万不能用于中断里
 * 
 * @param us 
 */

void Delay_us(uint16_t us)
{
    //Delay_us_BasicTimer(us);
    Delay_us_SysTick(us);
}

void Delay_ms(uint16_t ms)
{
    Delay_ms_BasicTimer(ms);
    //Delay_ms_SysTick(ms);
}

void Delay_s(uint16_t s)
{
   Delay_s_BasicTimer(s);
   //Delay_s_SysTick(s);
}

void Delay_us_SysTick(uint16_t us)
{
    /* 定时器重装值 */
    SysTick->LOAD = 72 * us;
    /* 清除当前计数值 */
    SysTick->VAL = 0;
    /*设置内部时钟源（2位->1）,不需要中断（1位->0），并启动定时器(0位->1)*/
    SysTick->CTRL = 0x5;
    /*等待计数到0， 如果计数到0则16位会置为1*/
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG));
    /* 关闭定时器 */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE; 
}

void Delay_ms_SysTick(uint16_t ms)
{
    while (ms--)
    {
        Delay_us(1000);
    }
}

void Delay_s_SysTick(uint16_t s)
{
    while (s--)
    {
        Delay_ms(1000);
    }
}


void Delay_us_BasicTimer(uint16_t us)
{
    /* 1. 给定时器7开启时钟*/
    RCC->APB1ENR |=RCC_APB1ENR_TIM7EN;
    /* 2. 设置预分频值: 分频值71表示72分频。分频后频率1000K，周期1us */
    TIM7->PSC= 72-1;
    
    TIM7->ARR= us-1;

    TIM7->EGR |= TIM_EGR_UG;
    TIM7->SR  &= ~TIM_SR_UIF;

    TIM7->CR1 |= TIM_CR1_CEN;
    while(TIM7->CNT<us-1);
    //while((TIM7->SR & TIM_SR_UIF)==0);
    //TIM7->SR  &= ~TIM_SR_UIF;
    TIM7->CR1 &= ~TIM_CR1_CEN;
}


void Delay_ms_BasicTimer(uint16_t ms)
{
    /* 1. 给定时器7开启时钟*/
    RCC->APB1ENR |=RCC_APB1ENR_TIM7EN;
    /* 2. 设置预分频值: 分频值71表示72分频。分频后频率1000K，周期1us */
    TIM7->PSC= 7200-1;
    
    TIM7->ARR= 10*ms-1;

    TIM7->EGR |= TIM_EGR_UG;
    TIM7->SR  &= ~TIM_SR_UIF;

    TIM7->CR1 |= TIM_CR1_CEN;
    while(TIM7->CNT<10*ms-1);
    //while((TIM7->SR & TIM_SR_UIF)==0);
    //TIM7->SR  &= ~TIM_SR_UIF;
    TIM7->CR1 &= ~TIM_CR1_CEN;
}


void Delay_s_BasicTimer(uint16_t s)
{
    while (s--)
    {
        Delay_ms_BasicTimer(1000);
    }
}
