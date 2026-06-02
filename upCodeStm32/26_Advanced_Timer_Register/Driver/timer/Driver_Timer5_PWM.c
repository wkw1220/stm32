#include "Driver_Timer5_PWM.h"

void Dri_Timer5_Pwm_Init(void){
    Dri_Timer5_Pwm_Init_Channel2();
    //Dri_Timer5_Pwm_Init_Channel3();
}

/**
 * @brief 定时器5 选择通道2
 *   通道1:PA0
 *   通道2:PA1
 *   通道3:PA2
 *   通道4:PA3
 * 
 */
void Dri_Timer5_Pwm_Init_Channel2()
{
    /* 1.开启时钟 */
    /* 1.1定时器5的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    /* 1.2 GPIO的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    /* 2.设置GPIO的复用推挽输出 PA1 MODE=11 CNF=10*/
    GPIOA->CRL |=(GPIO_CRL_MODE1|GPIO_CRL_CNF1_1);
    GPIOA->CRL &=~GPIO_CRL_CNF1_0;
    /* 3.定时器配置 */
    /* 3.1 预分频器的配置 */
    TIM5->PSC= 7200-1;
    /* 3.2 自动重装载寄存器的配置 */
    TIM5->ARR= 100-1;
    /* 3.3 计数器的方向 */
    TIM5->CR1&=~TIM_CR1_DIR;
    /* 3.4 配置通道2的捕获比较寄存器 */
    TIM5->CCR2=20;
    /* 3.5 把配置2配置为输出 */
    TIM5->CCMR1 &= ~TIM_CCMR1_CC2S;
    /* 3.6 配置通道的输出比较模式 CCMR1_OC2M=110*/
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_2;
    TIM5->CCMR1 |= TIM_CCMR1_OC2M_1;
    TIM5->CCMR1 &= ~TIM_CCMR1_OC2M_0;

    /* 3.7 使能通道2 */
    TIM5->CCER |= TIM_CCER_CC2E;
    /* 3.8 设置通道的极性 0=高电平有效  1=低电平有效  对于呼吸灯来说是无所谓的*/
    TIM5->CCER &= ~TIM_CCER_CC2P;
}

/**
 * @brief 这个是使能3通道 PA2 接口和PA8接口需要接一个跳线
 * 
 */
void Dri_Timer5_Pwm_Init_Channel3()
{
    /* 1.开启时钟 */
    /* 1.1定时器5的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    /* 1.2 GPIO的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    /* 2.设置GPIO的复用推挽输出 PA2 MODE=11 CNF=10*/
    GPIOA->CRL |=(GPIO_CRL_MODE2|GPIO_CRL_CNF2_1);
    GPIOA->CRL &=~GPIO_CRL_CNF2_0;
    /* 3.定时器配置 */
    /* 3.1 预分频器的配置 */
    TIM5->PSC= 7200-1;
    /* 3.2 自动重装载寄存器的配置 */
    TIM5->ARR= 100-1;
    /* 3.3 计数器的方向 */
    TIM5->CR1&=~TIM_CR1_DIR;
    /* 3.4 配置通道3的捕获比较寄存器 */
    TIM5->CCR3=50;
    /* 3.5 把配置3配置为输出 */
    TIM5->CCMR2 &= ~TIM_CCMR2_CC3S;
    /* 3.6 配置通道的输出比较模式 CCMR2_OC3M=110*/
    TIM5->CCMR2 |= TIM_CCMR2_OC3M_2;
    TIM5->CCMR2 |= TIM_CCMR2_OC3M_1;
    TIM5->CCMR2 &= ~TIM_CCMR2_OC3M_0;

    /* 3.7 使能通道3 */
    TIM5->CCER |= TIM_CCER_CC3E;
    /* 3.8 设置通道的极性 0=高电平有效  1=低电平有效  对于呼吸灯来说是无所谓的*/
    TIM5->CCER &= ~TIM_CCER_CC3P;
}

void Dri_Timer5_Pwm_Start()
{
    /* 使能计数器 */
    TIM5->CR1 |= TIM_CR1_CEN;
}

void Dri_Timer5_Pwm_Stop()
{
   TIM5->CR1 &= ~TIM_CR1_CEN;
}

void Dri_TIM5_Pwm_SetDutyCycle(uint8_t dutyCycle)
{
   TIM5->CCR2 = dutyCycle;
   //TIM5->CCR3 = dutyCycle;
}

