#include "Driver_Timer4_Mesure.h"

/**
 * @brief 定时器4 选择通道1
 *   通道1:PB6
 *   统一按照向上计数，捕获上升沿
 *   正常情况下，每经过一共时钟周期CNT就会计一个数，捕获到一个上升沿，CNT的值就会
 *   理解写入到CCR比较寄存器当中。
 *   由于分频以后 周期是1us,所以计算到的数字除以1000,就是ms单位
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
    /** 3.1 预分频器的配置 分频后计数器的的时钟是1Mz,周期:1us
     *  这里要设置频率高一些，因为使用的是输入捕获功能，如果频率比捕获对象的频率还低
     *  那么就没办法捕获
    */
    TIM4->PSC = 72 - 1;
    /* 3.2 自动重装载寄存器的配置:设置最大，尽量避免溢出，防止频繁产生溢出中断*/
    TIM4->ARR = 65535;
    /* 3.3 计数器的方向 */
    TIM4->CR1 &= ~TIM_CR1_DIR;

    /**4.输入捕获部分 */
    /**4.1 TI1的选择：把CH1直接连到TI1 */
    TIM4->CR2 &= ~TIM_CR2_TI1S;
    /** 4.2 信号的输入滤波器:不滤波 */
    TIM4->CCMR1&=~TIM_CCMR1_IC1F;
    /** 4.3 配置边缘检测器：上升沿:0   下降沿:1*/
    TIM4->CCER &=~TIM_CCER_CC1P;
    /**4.4 通道1配置为输入，把信号映射到IC1 */
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    /** 4.5 对信号做分频(信号频率比较高的时候) */
    TIM4->CCMR1 &=~TIM_CCMR1_IC1PSC;
    /* 4.6 通道1输入捕获使能 */
    TIM4->CCER |= TIM_CCER_CC1E;
    /**4.7 开启捕获中断 */
    TIM4->DIER |=TIM_DIER_CC1IE;

    /** 5.NVIC中断的配置 */
    /** 5.1 优先级组 */
    NVIC_SetPriorityGrouping(3);
    /** 5.1 优先级 */
    NVIC_SetPriority(TIM4_IRQn,3);
    /** 5.1 使能中断 */
    NVIC_EnableIRQ(TIM4_IRQn);
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

/**
 * 中断服务函数 
 * 捕获到上升沿的时候会产生中断
 * */

uint8_t raiseEdgrCounter=0; //上升沿个数
uint16_t cycle=0;//存储的周期的值
void TIM4_IRQHandler(void){
   /**判断是否是通断1发生了捕获中断 */
   if(TIM4->SR&TIM_SR_CC1IF){

      /**清除中断标志位 */
      TIM4->SR&=~TIM_SR_CC1IF;
     /**如果是第一个上升沿，则清零计数器，让计数器从0开始计数 怎么知道是第一个上升沿?*/
     raiseEdgrCounter++;
     if(raiseEdgrCounter==1){
        TIM4->CNT=0;//计数器清零
     }else if(raiseEdgrCounter==2){
        /**存储信号的周期 单位是us*/
        cycle=TIM4->CCR1;
        /**上升沿的计数从0开始计数 */
        raiseEdgrCounter=0;
     }

   }
}

double Dri_Tim4_getCycle(void){
    //由于分频以后 周期是1us,所以计算到的数字除以1000,就是ms单位
    return cycle/1000.0;
}

double Dri_Tim4_getFrequency(void){
    //cycle的单位是us
    return 1000000/cycle;
}

void Dri_Timer4_Init(void)
{
    Dri_Timer4_Init_Channel1();
}
