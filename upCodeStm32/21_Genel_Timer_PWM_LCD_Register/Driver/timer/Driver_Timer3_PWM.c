#include "Driver_Timer3_PWM.h"

void Dri_Timer3_Pwm_Init(void){
    Dri_Timer3_Pwm_Init_Channel3();
}

/**
 * @brief 定时器3 选择通道3
 *   通道3:PB0 这根引脚和LCD背光灯相连接
 *   LCD背光调节
 */

void Dri_Timer3_Pwm_Init_Channel3()
{
    /* 1.开启时钟 */
    /* 1.1定时器3的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    /* 1.2 GPIO的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    /* 2.设置GPIO的复用推挽输出 PB0 MODE=11 CNF=10*/
    GPIOB->CRL |=(GPIO_CRL_MODE0|GPIO_CRL_CNF0_1);
    GPIOB->CRL &=~GPIO_CRL_CNF0_0;
    /* 3.定时器配置 */
    /* 3.1 预分频器的配置 */
    TIM3->PSC= 7200-1;
    /* 3.2 自动重装载寄存器的配置 */
    TIM3->ARR= 100-1;
    /* 3.3 计数器的方向 */
    TIM3->CR1&=~TIM_CR1_DIR;
    /* 3.4 配置通道3的捕获比较寄存器 */
    TIM3->CCR3=50;
    /* 3.5 把配置3配置为输出 */
    TIM3->CCMR2 &= ~TIM_CCMR2_CC3S;
    /* 3.6 配置通道的输出比较模式 CCMR2_OC3M=110*/
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_2;
    TIM3->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M_0;

    /* 3.7 使能通道3 */
    TIM3->CCER |= TIM_CCER_CC3E;
    /* 3.8 设置通道的极性 0=高电平有效  1=低电平有效  对于呼吸灯来说是无所谓的*/
    TIM3->CCER &= ~TIM_CCER_CC3P;
}

void Dri_Timer3_Pwm_Start()
{
    /* 使能计数器 */
    TIM3->CR1 |= TIM_CR1_CEN;
}

void Dri_Timer3_Pwm_Stop()
{
   TIM3->CR1 &= ~TIM_CR1_CEN;
}

void Dri_Tim3_Pwm_SetDutyCycle(uint8_t dutyCycle)
{
   TIM3->CCR3 = dutyCycle;
}

