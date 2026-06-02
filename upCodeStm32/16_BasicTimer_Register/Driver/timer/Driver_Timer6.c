#include "Driver_Timer6.h"
#include "Driver_Led.h"
#include "string.h"

/**
 * 基本定时器 TIM6 TIM7
 * @brief 基本定时器timer6 初始化
 * 
 * 虽然定时器挂在APB1上面，但是时钟频率仍然是72M,保持不变
 * 
 * RCC:Reset and clock control
 * 
 */
void Driver_Timer6_Init(void)
{
    /* 1 给定时器6开启时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
    /**2.设置预分频值:分频值7199表示7200分频。分频后频率10K，周期100us  
       也就是频率有原来的72M变为10K  周期有原来的1/72us变为100us
       是一个带有自动重装载的16位累加计数器
       不能设置为72000，因为会大于66535
    */
    TIM6->PSC =7200-1;
    /*
    3. 设置自动重转载寄存器的值，决定中断发生的频率。
        假设设置为9999，表示计数器计数10000次发生一次中断。
        一个周期计数一次，计数一次100us，10000次1000000us，正好1s
        当计数达到预设的值以后就会产生一次中断
    */
    TIM6->ARR=10000-1;

    /* 为了避免一启动立即进入更新中断，可以先产生一个更新事件 顺序不能错
       原因：写到预分频器里的值，等产生溢出中断以后才写入影子寄存器中，在没有写入之前
             cpu会以原来的频率(72M)进行计数，所以很快就会产生一次中断(138ms)
             本质原因就是预分频的值没有生效
             主动让它溢出一次，就会产生一次中断，预分频器里的值会立马生效
       URS：更新请求源 0：任何源都会产生中断
                      1：只有计数器上溢或下溢才会产生中断
    */
    TIM6->CR1  |= TIM_CR1_URS;
    
    /**
     * 主动产生一次更新事件，类似溢出一次，更新事件UG设置以后也会把UIF这位也置为1 
     * UG的功能：就是使预分频寄存器和重装载寄存器的值更新到影子寄存器
     * 
     * 然后立马给UIF清零
     * 
     * 必须要在使能中断之前执行这两行代码
     * UG 和UIF 用一种就可以了
    */
    TIM6->EGR |= TIM_EGR_UG;//产生了更新事件
    //TIM6->SR |= TIM_SR_UIF;//产生了更新中断  这里是用非还是或 需要待验证
   
    /* 4.使能更新中断 */
    TIM6->DIER |= TIM_DIER_UIE;

    //预重载寄存器 默认就是零 修改ARR寄存器了的值立即更新
    TIM6->CR1 &=~TIM_CR1_ARPE;

    /* NVIC 设置 */
    /* 5.设置中断优先级分组 */
    NVIC_SetPriorityGrouping(3);
    /* 6.设置中断优先级 */
    NVIC_SetPriority(TIM6_IRQn, 1);
    /* 7.使能定时器6的中断 */
    NVIC_EnableIRQ(TIM6_IRQn);

    /* 8.使能计数器 */
    TIM6->CR1 |= TIM_CR1_CEN;

    
}

/**
 * @description: TIM6的中断服务函数
 * @return {*}
 */
uint8_t isReady=0;
void TIM6_IRQHandler(void)
{
  //清除相应的中断标志位，不清发现灯是亮的 
  TIM6->SR &= ~TIM_SR_UIF;
  
  //逻辑代码不要写在中断函数里 可以设置标志位
  Dri_Led_Toggle(LED_GREEN);
  //isReady=1;
  //只能用于测试中断函数可执行
  //printf("1s\r\n");
}
