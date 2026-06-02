#include "Driver_Timer1.h"

/**
 * @brief 定时器1 选择通道1
 *   通道1:PA8
 *
 */
void Dri_Timer1_Init_Channel1()
{
    /* 1.开启时钟 */
    /* 1.1定时器1的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    /* 1.2 GPIO的时钟(PA8) 因为定时器1的TIM1_CH1的复用通道就是PA8 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    /* 2.设置GPIO的复用推挽输出 PA8 MODE=11 CNF=10*/
    GPIOA->CRH |= (GPIO_CRH_MODE8 | GPIO_CRH_CNF8_1);
    GPIOA->CRH &= ~GPIO_CRH_CNF8_0;
    /* 3.定时器配置 */
    /* 3.1 预分频器的配置 分频后计数器的的时钟频率是2Hz*/
    TIM1->PSC = 7200 - 1;
    /* 3.2 自动重装载寄存器的配置:1s中溢出两次*/
    TIM1->ARR = 5000;
    /* 3.3 重复计数器的配置 RCR=4表示CNT计数器溢出5次，才会产生更新事件(中断) */
    TIM1->RCR=4;
    /* 3.4 计数器的方向 */
    TIM1->CR1 &= ~TIM_CR1_DIR;

    /**4.输入捕获部分 */
    /**4.1 把通道1配置为输出 */
    TIM1->CCMR1&= ~TIM_CCMR1_CC1S;
    /**4.2 配置捕获比较寄存器的值  */ 
    TIM1->CCR1=2500; //占空比 50%
    /**4.3 输出的极性 0=高电平有效 1=低电平有效 */
    TIM1->CCER &= ~TIM_CCER_CC1P;
    /**4.4 使能通道1 */
    TIM1->CCER |=TIM_CCER_CC1E;
    /**4.5 配置通道的输出比较模式 CCMR1_OC1M=110*/
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_2;
    TIM1->CCMR1 |= TIM_CCMR1_OC1M_1;
    TIM1->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    /**4.6 主输出使能，只有高级定时器需要配置 */
    TIM1->BDTR|=TIM_BDTR_MOE;
    /**4.7 防止一启动就进入更新中断：1.使用UG位产生一个更新事件 2.预分频和重复计数器更新到影子寄存器 */
    TIM1->EGR |=TIM_EGR_UG;
    TIM1->SR &=~TIM_SR_UIF;//清除中断标志位
   
   /**5 配置中断 */
   /**5.1 定时器使能中断 */
   TIM1->DIER |=TIM_DIER_UIE;
   /**5.2 NVIC的配置 */
   NVIC_SetPriorityGrouping(3);
   NVIC_SetPriority(TIM1_UP_IRQn,3);
   NVIC_EnableIRQ(TIM1_UP_IRQn);
}

void Dri_Timer1_Start()
{
    /* 使能计数器 */
    TIM1->CR1 |= TIM_CR1_CEN;
}

void Dri_Timer1_Stop()
{
    TIM1->CR1 &= ~TIM_CR1_CEN;
}

/**一启动就会进入中断
 * 来一次更新事件才会把TIM1->RCR=4设置的值写入到影子寄存器 但是我们在中断里已经停掉了计数器
 * 所以，一上电没有反应
 */
void TIM1_UP_IRQHandler(void){
    //printf("11\r\n");
    //清除中断标志位
    TIM1->SR &=~TIM_SR_UIF;
    //停掉计数器
    Dri_Timer1_Stop();
}

void Dri_Timer1_Init(void)
{
    Dri_Timer1_Init_Channel1();
}

