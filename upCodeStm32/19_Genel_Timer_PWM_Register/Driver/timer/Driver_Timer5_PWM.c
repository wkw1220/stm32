#include "Driver_Timer5_PWM.h"
/**
 * @brief 灯为什么会变暗和变亮?
 *  本质上是流过灯的电流大小导致的。 在额定范围内，电流越大灯越亮，电流越小灯越暗
 *  但是电阻一定，根据欧姆定律，电阻不变我们可以调节电压的大小改变电流的大小。
 *  可以用正弦波电压实现。
 *  但是输入电压一定，我们可以改变得到电压的占空比取的有效电压
 *  PWM:一定要有惯性的设备。像电机，LED灯 启动快，停止慢
 * 
 *  捕获比较功能:只能输出方波
 *   捕获比较寄存器:要么捕获，要么比较不能同时捕获又比较
 *   比较：四个通道都可以，主要是和CNT计数器比较。
 *         假设小于这个数就输出低电平，大于这个数就输出高电平
 */

u8 flag=2;// 2:是开启2通道 3:开启3通道
void Dri_Timer5_Pwm_Init(void){
    if(flag==2){
      Dri_Timer5_Pwm_Init_Channel2();
    }else if(flag==3){
      Dri_Timer5_Pwm_Init_Channel3();
    }
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
    /* 3.1 预分频器的配置 频率变为72000000/7200=10000=10K 时钟周期变为1s/10K=100us */
    TIM5->PSC= 7200-1;
    /**  
     * 3.2 自动重装载寄存器的配置 因为周期变为100us,
     * 说明100*100=10000us=10ms才会触发一次中断，会把CNT计数器清零
     * 时钟周期变为100us了，计100次数就是10ms,也就是会产生时长为10ms的方波
     * 通过改变捕获比较寄存器的值来改变高电平和低电平的输出时间
     * 
     * */
    TIM5->ARR= 100-1;
    /* 3.3 计数器的方向 */
    TIM5->CR1&=~TIM_CR1_DIR;
    /* 3.4 配置通道2的捕获比较寄存器 */
    TIM5->CCR2=50;
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
 * @brief 这个是使能3通道 如何要让PA8灯，那么PA2接口和PA8接口需要接一个跳线
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
   if(flag==2){
       TIM5->CCR2 = dutyCycle;
    }else if(flag==3){
      TIM5->CCR3 = dutyCycle;
    }
}

