#include "Delay.h" // Device header
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
    SysTick->CTRL |=SysTick_CTRL_CLKSOURCE;  //时钟频率：72M 
    //SysTick->CTRL &=~SysTick_CTRL_CLKSOURCE_Msk;  //时钟频率:9M
    //SysTick->CTRL = 0x5;
    
    /**
     * 等待计数到0,如果计数到0则寄存器第16位会置为1
     * 问题:等这个位变为1以后，何时变为0?
     * 方式一:如果在上次读取本寄存器后， SysTick 已经计到了 0，则该位为 1。
     *       如果读取该位(确切的说是读取CTRL寄存器就可以了)，该位将自动清零,
     *       本案例中就是用这种方式实现
     *       注:参考文档为CM3权威指南CnR2中chapter8-->8.7章节
     * 方式二: 1.读取SysTick->VAL寄存器中的值时返回当前倒计数的值;
     *        2.写它则使之清零，在写入值的时候同时还会清除在SysTick控制及状态寄存器中的COUNTFLAG 标志 
     * 
     * Returns 1 if timer counted to 0 since last time this was read.
     * 定时器一到 0，countflag 就 = 1
     * 你一读它，它就自动变回 0
     *    
     * 
     * */
    //while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG));
    while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)){
        //SysTick->VAL = 5000;  //每执行一次，SysTick->VAL会变为0，但是SysTick_CTRL_COUNTFLAG会清零，重新新的一轮开始
    }
    /* 关闭定时器 */
    //SysTick->CTRL &= ~SysTick_CTRL_ENABLE;
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  
    
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

//UINT16_MAX
void Delay_BadWay(void){
    for (u16 i = 0; i < UINT16_MAX; i++)
    {
        for (u8 j = 0; j < 30; j++)
        {
            
        }
    }
}


/**
 * 使 COUNTFLAG 从 1 变回 0 的方式
主要有两种：
1. 读取 SysTick->CTRL
  SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk
  读取 CTRL 寄存器后，COUNTFLAG 会自动清零 准确的说只要读取CTRL 寄存器就会清零COUNTFLAG，不管你读的是哪个位，还是读整个寄存器，都会清零COUNTFLAG,而不是读取该位
2. 写 SysTick->VAL
  任何向 SysTick->VAL 写入值都会清除当前计数，并同时清除 COUNTFLAG
这两种方式是 Cortex-M SysTick 的规范行为。
 */


 /**
  * Load 寄存器  
  * Val 寄存器 
  * SysTick_CTRL_ENABLE 使能位
  * 
  *  1.Load 寄存器里写入值 
  *      ---> 2.使能位为1，(只要使能了Load寄存器中的值就会立马加载到Val寄存器中，一旦使能计时器立刻开始工作)
  *      --> 3.把Load寄存器中的值加载到Val寄存器中,Val寄存器中的值会不断发生变化
  *      --> 4.Val寄存器中的值每个时钟周期减1 
  *      -->5.当VAL从1变成0的那个时刻，COUNTFLAG置1,如果同时打开了TICKINT=1，这时会挂起 SysTick 中断请求
  *      -->6.当 VAL=0 后，下一个时钟周期不是继续减成负数或普通意义上的溢出值，而是硬件自动把 LOAD 重新装入 VAL，然后继续倒计数
  *        重新把Load寄存器中的值加载到Val寄存器中
  *        COUNTFLAG 不是因为自动重装就立刻清零。一般是读取 CTRL/CSR，或者写 VAL 时，COUNTFLAG 才会被清掉
  * 
  */


  /**
   * 
   * while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)){
        //SysTick->VAL = 5000;  //每执行一次，SysTick->VAL会变为0，但是SysTick_CTRL_COUNTFLAG会清零，重新新的一轮开始
    }

   * 如果在计时的过程中向Val寄存器里写值了 会发生什么？
   * 不会把你写进去的那个数当成新的计数值。
   对 STM32F103 的 SysTick 来说，在计时过程中向 VAL 寄存器写任意值，会发生这几件事：
    1. VAL 当前值被直接清成 0。
    2. COUNTFLAG 同时被清 0。
    3. 当前这一轮倒计时被打断。
    4. 根据 SysTick 在 VAL=0 后会自动从 LOAD 重装的机制，可以推出：下一次 SysTick 时钟到来时，会把 LOAD的值重新装入 VAL，然后从头开始计数。
    所以，本质上它相当于：
    强制把这次 SysTick 重新开始计时
    比如：
    • 设 LOAD = 71999
    • 本来已经数到 VAL = 30000
    • 这时你去写 VAL
    结果不是把 VAL 改成你写入的数，而是：
    • VAL 立刻清零
    • 剩下那 30000 个计数作废
    • 下一拍重新装入 71999
    • 再从 71999 开始往下减
    还有一个容易混淆的点：
    • 写 VAL 不会因为“清零了”就立刻产生中断
    • 相反，它会把 COUNTFLAG 清掉
    总结:向 SysTick 的 VAL 寄存器写任意值，并不是写入新的计数值，而是把当前计数器清零，同时清除 COUNTFLAG，使计数器在下一时钟周期从 LOAD 重新开始计数。
   */

        /**
         * 也就是会重新下一轮计时，如果开启中断，不会产生中断吗？
         * 不会，因为写 VAL 会清除 COUNTFLAG，而 COUNTFLAG 是产生中断的条件之一。
         * 写 VAL 这件事本身，不会产生 SysTick 中断，即使你已经打开了 TICKINT=1。

        更准确地说：

        TICKINT=1 的作用是：当计数器自然地从 1 变成 0 时，产生 SysTick 异常请求。
        但你向 VAL 写任意值时，硬件只是把当前值清成 0，并清掉 COUNTFLAG。
        这不属于“计数器从 1 递减到 0”这个事件，所以不会因此触发中断。
        下一次 SysTick 时钟到来后，计数器会从 LOAD 重新装载，然后开始下一轮计时。
        你可以直接记成一句：

        SysTick 中断是在计数器正常倒计数到 0 时触发的，不是因为软件写 VAL 把它清零而触发的。

        再分两种情况看会更稳：

        还没到 0，你中途写了 VAL
        结果是这轮计时被打断，从下一拍开始按 LOAD 重新计时。
        这次写操作不会立刻进中断。

        它之前已经计到 0，已经产生了 SysTick 挂起请求
        这时你再写 VAL，也不等于清除这个挂起中断。
        VAL 只影响当前计数值和 COUNTFLAG。
        挂起状态对应的是 SCB_ICSR.PENDSTCLR 这个机制，和写 VAL 不是一回事。

        所以最后结论就是：

        写 VAL = 重新开始下一轮计时
        写 VAL ≠ 触发中断
        写 VAL ≠ 清除已经 pending 的 SysTick 中断  
        */

        /**
         * 一旦SysTick->CTRL |=SysTick_CTRL_ENABLE;开始使能 硬件就开始工作，计时器就开始工作，这完全是硬件行为
         * 不会影响主程序的执行 这个要注意
         */