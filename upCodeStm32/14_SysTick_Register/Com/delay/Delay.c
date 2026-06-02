#include "Delay.h" // Device header

/** 
 * 这个延时函数千万不能用于中断里
 */

void Delay_us1(uint16_t us)
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

/**
 * @brief systick是Cotex-m3芯片内部提供的定时器，叫滴答定时器，
 * 而不是片上外设提供的Timx定时器，所以参考手册要看M3编程手册英文版,
 * 因为本身是芯片内部的，天然带有时钟，不需要开时钟。
 *   systick内部是做减法的
 *   我们这里选择频率是72M,也就是1s要震动72*10^6 =72000000次，
 *   专业一点是72000000个时钟周期，也就是震动一次是1/72000000秒=1/72000ms=1/72us,
 *   也就是一个时钟周期是1/72us
 *   对于滴答定时器也就是1/72us(也就是一个时钟周期)计一次数, 那么计72次数正好是1us
 *   SysTick->LOAD:重装载值,也就是及多少次数 
 *   SysTick->VAL:当前值寄存器
 * 
 * 工作过程:STK_CTRL中EN使能位为1，然后把LOAD寄存器中的值放到VAL寄存器中，每个时钟周期
 *         都会把VAL中的值减去1，也就是VAL中的值一直是在不断发生变化的，等到VAL寄存器的值为0以后
 *         下一个时钟周期到来之前的那一刻发生溢出(是否产生中断?),然后重新把LOAD寄存器中的
 *         值加载到VAL寄存器.
 * 
 * 
 * @param us 
 */
void Delay_us(uint16_t us)
{
    /**  
    * 定时器重装值 是整体减1 load寄存器是24位 72*us有没有可能溢出呢?
    * 2^24/2^16=2^8=256 用uint16_t的最大值*72 也不会溢出 但是传入参数
    * 不能用uint32_t
    * 
    * 时钟有个72M,还有一个9M,为什么选择频率高72M的而不是选择低的9M的?
    *   精度高一些
    * 
    */
    SysTick->LOAD = 72 * us;
    
    /**  清除当前计数值 
     *   如果在使能之前,如果上一次寄存器中有残余值(上一次不在为0的时候发生了中断而停止的场景)
     *   但是根据工作流程，在使能之后LOAD里的值会立马写入到VAL寄存器中
     *   所以这行代码可有可无 
     *   但是加上这行代码会对SysTick->CTRL这个寄存器中这一位SysTick_CTRL_COUNTFLAG清零
     * */
    //SysTick->VAL = 0;

    /*设置内部时钟源（2位->1）,不需要中断（1位->0），并启动定时器(0位->1)*/
    
    //SysTick->CTRL |=(SysTick_CTRL_ENABLE|SysTick_CTRL_CLKSOURCE);
    //SysTick->CTRL |=SysTick_CTRL_ENABLE;
    SysTick->CTRL |=SysTick_CTRL_ENABLE_Msk;
    //SysTick->CTRL |=SysTick_CTRL_CLKSOURCE;
    SysTick->CTRL |=SysTick_CTRL_CLKSOURCE_Msk;
    //SysTick->CTRL = 0x5;
    
    /**
     * 等待计数到0,如果计数到0则寄存器第16位会置为1
     * 问题:等这个位变为1以后，何时变为0?
     * 方式一:如果在上次读取本寄存器后， SysTick 已经计到了 0，则该位为 1。
     *       如果读取该位，该位将自动清零,
     *      本案例中就是用这种方式实现
     *     注:参考文档为CM3权威指南CnR2中chapter8-->8.7章节
     * 方式二: 1.读取SysTick->VAL寄存器中的值时返回当前倒计数的值;
     *        2.写它则使之清零，在写入值的时候同时还会清除在SysTick控制及状态寄存器中的COUNTFLAG 标志 
     *    
     * 
     * */
    //while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG));
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
    /* 关闭定时器 */
    //SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  
    
}


//UINT16_MAX
void Delay_BadWay(void){
    for (u16 i = 0; i < UINT16_MAX; i++)
    {
        for (u8 j = 0; j < 30; j++)
        {
            
        }
    }
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

