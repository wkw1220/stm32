#include "Driver_Timer4_PWM.h"

void Dri_Timer4_Init(void)
{
    Dri_Timer4_Init_Channel1();
}

/**
 * @brief 定时器4 选择通道1
 *   通道1:PB6
 *
 */
void Dri_Timer4_Init_Channel1()
{
    /* 1.开启时钟 */
    /* 1.1定时器4的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    /* 1.2 GPIO的时钟(PB6) */
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    /* 2.设置GPIO的浮空输入 PB6 MODE=00 CNF=01*/
    GPIOB->CRL &= ~(GPIO_CRL_MODE6 | GPIO_CRL_CNF6_1);
    GPIOB->CRL |= GPIO_CRL_CNF6_0;
    /* 3.定时器配置 */
    /* 3.1 预分频器的配置 分频后计数器的的时钟是1Mz,z周期:1us*/
    TIM4->PSC = 72 - 1;
    /* 3.2 自动重装载寄存器的配置:设置最大，尽量避免溢出*/
    TIM4->ARR = 65535;
    /* 3.3 计数器的方向 */
    TIM4->CR1 &= ~TIM_CR1_DIR;

    /**4.输入捕获部分 */
    /**4.1 输入信号不滤波 */
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
    /**4.2 配置边缘检测 IC1：上升沿  IC2:下降沿 */
    TIM4->CCER &= ~TIM_CCER_CC1P;
    TIM4->CCER |= TIM_CCER_CC2P;
    /**4.3 把IC1映射到TI1,上升沿 */
    TIM4->CCMR1&= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1|= TIM_CCMR1_CC1S_0;
    /**4.4 把IC2映射到TI1,下降沿 */
    TIM4->CCMR1&= ~TIM_CCMR1_CC2S_0;
    TIM4->CCMR1|= TIM_CCMR1_CC2S_1;
    /**4.5 IC1和IC2的预分频:都不分频 */
    TIM4->CCMR1&= ~TIM_CCMR1_IC1PSC;
    TIM4->CCMR1&= ~TIM_CCMR1_IC2PSC;
   
    /**4.6配置TRGI信号:TI1P1  */
    TIM4->SMCR |=(TIM_SMCR_TS_0|TIM_SMCR_TS_2);
    TIM4->SMCR &=~TIM_SMCR_TS_1;
    /**4.6配置从模式:为复位模式 */
    TIM4->SMCR |=TIM_SMCR_SMS_2;
    TIM4->SMCR &=~(TIM_SMCR_SMS_0|TIM_SMCR_SMS_1);
    /**4.6 使能通道1和通道2捕获 */
    TIM4->CCER |=TIM_CCER_CC1E;
    TIM4->CCER |=TIM_CCER_CC2E;

}

void Dri_Timer4_Start()
{
    /* 使能计数器 */
    TIM4->CR1 |= TIM_CR1_CEN;
}

void Dri_Timer4_Stop()
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
}

/**周期:ms 
 * 不加1 会缺少一个周期
*/
double Dri_Tim4_getCycle(void){
    return ((TIM4->CCR1)+1) /1000.0;
}

double Dri_Tim4_getFrequency(void){
    return 1000000/((TIM4->CCR1)+1);
}

double Dri_Tim4_getDutyCycle(void){
    return (((TIM4->CCR2)+1)*1.0)/((TIM4->CCR1)+1);
}
