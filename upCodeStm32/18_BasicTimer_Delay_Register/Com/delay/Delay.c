#include "Delay.h" // Device header

void Delay_us(uint16_t us)
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

void Delay_ms(uint16_t ms)
{
    while (ms--)
    {
        Delay_us(1000);
    }
}

void Delay_s(uint16_t s)
{
    while (s--)
    {
        Delay_ms(1000);
    }
}


void Delay_BasicTimer_us(uint16_t us){
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC =72-1;
    TIM6->ARR=us-1;
    TIM6->EGR |= TIM_EGR_UG;
    TIM6->SR  &= TIM_SR_UIF;
    /* 4.使能更新中断 */
    TIM6->DIER |= TIM_DIER_UIE;

    TIM6->CR1 |= TIM_CR1_CEN;
    
    //while(TIM6->CNT<us-1);
    while((TIM6->SR&TIM_SR_UIF)==0);
    TIM6->SR &=~TIM_SR_UIF;
    TIM6->CR1 &= ~TIM_CR1_CEN;

}
void Delay__BasicTimer_ms(uint16_t ms){
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    TIM6->PSC =7200-1;
    TIM6->ARR=10*ms-1;
    TIM6->EGR |= TIM_EGR_UG;
    TIM6->SR  &= ~TIM_SR_UIF;
    /* 4.使能更新中断 */
    TIM6->DIER |= TIM_DIER_UIE;

    TIM6->CR1 |= TIM_CR1_CEN;
    
    //while(TIM6->CNT<us-1);
    while((TIM6->SR&TIM_SR_UIF)==0);
    TIM6->SR &=~TIM_SR_UIF;
    TIM6->CR1 &= ~TIM_CR1_CEN;
}
void Delay__BasicTimer_s(uint16_t s){
    while (s--)
    {
        Delay__BasicTimer_ms(1000);
    }
}
