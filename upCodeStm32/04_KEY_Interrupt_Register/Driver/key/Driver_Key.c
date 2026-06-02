#include "stm32f10x.h"
#include "Driver_Key.h"
#include "Driver_Led.h"
#include "Delay.h"
/**
 * 中断的种类:
 *  1. 内核内部中断:例如滴答定时器产生的中断
 *  2. 片上外设产生的中断:例如串口，DMA等产生的中断
 *  3. 外部中断:来自GPIO的中断
 *
 *  NVIC:嵌套向量中断器，负责给所有中断信号分配优先级，专门控制优先级
 *  EXTI:专门负责来自芯片外部的中断(不是片上外设)
 *  AFIO:每一组GPIO有16个引脚，负责筛选每组某一位的引脚全部发送给EXTI
 *       它是一个选择器，具体功能一定要参考参考手册
 *  流程关系:AFIO->EXTI->NVIC
 * 每个优先级用四位表示，最大值是16，但是它的优先级种类数量远大于16，如何用四位来
 * 表示更多的优先级种类数量？
 *
 *
 * 引脚复用的具体涵义:
 *  在查看芯片手册框图时发现，有的引脚后面还有其它内容,例如PA10/usart1-tx/tim1-ch3
 *  PA10默认有一条线路和CPU相连，usart1-tx发送线也和cpu相连，默认情况下各自运行，但是片上
 *  外设在芯片内部，想把数据发送到芯片外，有没有线路，那么usart1-tx在AFIO的作用下就和PA10
 *  连接，PA10通用引脚就变成了usart1-tx发送引脚。其它复用引脚和这类似
 */

/**
 *  信号中断和事件中断
 *  信号中断:这种中断时中断cpu执行的main函数，中断信号会传递给cpu
 *  事件中断:中断信号不会传给cpu,而是直接发送给片上外设
 *  例如按下按键PA0,让串口发送数据
 *   信号中断:PA10会利用AFIO产生中断，然后传递给EXTI,然后再经过NVIC到CPU,CPU调用中断
 *           处理函数EXTI0_Handler,中断处理函数调用硬件USART串口，然后发送数据。
 *   事件中断机制:PA10会利用AFIO产生中断，然后传递给EXTI,然后信号直接到USART串口，串口
 *               直接发送数据。
 */

/**
 * @brief 按键定义初始化
 * 1.给按键对应的io口设置工作模式：下拉输入
 *   按键处的电压是3.3V，如果在引脚接上拉，会使引脚的电压为3.3V，按下按键以后引脚电压和按键处的电压
 *   是等位电压，不会产生上升沿或下降沿。如果引脚下拉，会使引脚的电压变为0V，按键按下，引脚的电压0V
 *   会被按键处的电压拉高至3.3V，瞬间就会产生一个上升沿。
 *   为什么时按键拉高了引脚的电压，而不是引脚电压拉低了按键电压，因为引脚外面输入电路是一个钳位电路
 * 2.配置复用为外部中断
 * 3.配置外部中断控制器 EXTI
 * 4.配置NVIC
 */
void Dri_Key_Init_PF10(void)
{
  /**1.开始时钟 **/
  /**1.1 开启GPIOF */
  RCC->APB2ENR |= RCC_APB2ENR_IOPFEN; // 在stm32f10x.h文件的第69行注释放开

  /** 1.2 AFIO */
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

  /** 2.配置PF10 的输入模式 MODE：输入模式00 CNF CNF:10(上拉或者下拉) ODR：0下拉 1：上拉 */
  GPIOF->CRH &= ~GPIO_CRH_MODE10;
  GPIOF->CRH |= GPIO_CRH_CNF10_1;
  GPIOF->CRH &= ~GPIO_CRH_CNF10_0;
  GPIOF->ODR &= ~GPIO_ODR_ODR10;//下拉
  /**
   * 寄存器在AFIO里去找 
   * 
   * 要启用引脚的中断服务功能
   * 3. 配置AFIO 配置PF10引脚为外部中断  EXTICR3  0101
   * 
   * 总共16个引脚 分成4组 每组4个引脚
   * EXTICR有四个，序号是 1 2 3 4 但是AFIO->EXTICR用的是数组实现
   * 0->3 4->7 8->11 12->15
   * 下标是0到3，所以下面代码下标是2
   */
  //先把这四位清零 然后再或
  AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI10;
  AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI10_PF;

  /* 4. 配置EXTI 片上外设唯一个例外不需要放开时钟信号的*/
  /* 4.1. 配置上升沿触发 RTSR TR10=1*/
  EXTI->RTSR |= EXTI_RTSR_TR10;
  /* 4.2 开启 LINE10, 配置的中断屏蔽寄存器，因为是与门连接的NVIC */
  EXTI->IMR |= EXTI_IMR_MR10;

  /* 5. 配置 NVIC */
  /* 5.1 配置优先级组 (3-7) 配置3表示4个二进制位全部用于表示抢占优先级
   * 4个二进制位全部用于表示抢占优先级
   */
  NVIC_SetPriorityGrouping(2);
  /* 5.2 配置优先级 参数1:中断号*/
  NVIC_SetPriority(EXTI15_10_IRQn, 3);
  /* 5.3 使能Line10 */
  NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void Dri_Key_Init_PF8(void)
{
  /**1.开始时钟 **/
  /**1.1 开启GPIOF */
  RCC->APB2ENR |= RCC_APB2ENR_IOPFEN; // 在stm32f10x.h文件的第69行注释放开

  /** 1.2 AFIO */
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

  /** 2.配置PF8 的输入模式 MODE：输入模式00 CNF CNF:10(上拉或者下拉) ODR：0下拉 1：上拉 */
  GPIOF->CRH &= ~GPIO_CRH_MODE8;
  GPIOF->CRH |= GPIO_CRH_CNF8_1;
  GPIOF->CRH &= ~GPIO_CRH_CNF8_0;
  GPIOF->ODR |= GPIO_ODR_ODR8;//上拉
  /**
   *
   * 要启用引脚的中断服务功能
   *3. 配置AFIO 配置PF10引脚为外部中断  EXTICR3  0101
   *EXTICR有四个，序号是 1 2 3 4 但是AFIO->EXTICR用的是数组实现
   *下标是0到3，所以下面代码下标是2
   */
  // 先把这四位清零 然后再或 PF  0101 可以先统一设置为0 再设置为0101
  AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI8;
  AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PF;

  /* 4. 配置EXTI 片上外设唯一个例外不需要放开时钟信号的*/
  /* 4.1. 配置下降沿触发 FTSR TR8=1*/
  EXTI->FTSR |= EXTI_FTSR_TR8;
  /* 4.2 开启 LINE8, 配置的中断屏蔽寄存器，因为是与门连接的NVIC */
  EXTI->IMR |= EXTI_IMR_MR8;

  /* 5. 配置 NVIC */
  /* 5.1 配置优先级组 (3-7) 配置3表示4个二进制位全部用于表示抢占优先级
   * 4个二进制位全部用于表示抢占优先级
   */
  NVIC_SetPriorityGrouping(3);
  /* 5.2 配置优先级 参数1:中断号*/
  NVIC_SetPriority(EXTI9_5_IRQn, 3);
  /* 5.3 使能Line8 */
  NVIC_EnableIRQ(EXTI9_5_IRQn);
}


void Dri_Key_Init_PF9(void)
{
  /**1.开始时钟 **/
  /**1.1 开启GPIOF */
  RCC->APB2ENR |= RCC_APB2ENR_IOPFEN; // 在stm32f10x.h文件的第69行注释放开

  /** 1.2 AFIO */
  RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

  /** 2.配置PF9 的输入模式 MODE：输入模式00 CNF CNF:10(上拉或者下拉) ODR：0下拉 1：上拉 */
  GPIOF->CRH &= ~GPIO_CRH_MODE9;
  GPIOF->CRH |= GPIO_CRH_CNF9_1;
  GPIOF->CRH &= ~GPIO_CRH_CNF9_0;
  GPIOF->ODR |= GPIO_ODR_ODR9;//上拉
  /**
   *
   * 要启用引脚的中断服务功能
   *3. 配置AFIO 配置PF10引脚为外部中断  EXTICR3  0101
   *EXTICR有四个，序号是 1 2 3 4 但是AFIO->EXTICR用的是数组实现
   *下标是0到3，所以下面代码下标是2
   */
  // 先把这四位清零 然后再或 PF  0101 可以先统一设置为0 再设置为0101
  AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI9;
  AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI9_PF;

  /* 4. 配置EXTI 片上外设唯一个例外不需要放开时钟信号的*/
  /* 4.1. 配置下降沿触发 FTSR TR8=1*/
  EXTI->FTSR |= EXTI_FTSR_TR9;
  /* 4.2 开启 LINE8, 配置的中断屏蔽寄存器，因为是与门连接的NVIC */
  EXTI->IMR |= EXTI_IMR_MR9;

  /* 5. 配置 NVIC */
  /* 5.1 配置优先级组 (3-7) 配置3表示4个二进制位全部用于表示抢占优先级
   * 4个二进制位全部用于表示抢占优先级
   */
  NVIC_SetPriorityGrouping(3);
  /* 5.2 配置优先级 参数1:中断号*/
  NVIC_SetPriority(EXTI9_5_IRQn, 3);
  /* 5.3 使能Line8 */
  NVIC_EnableIRQ(EXTI9_5_IRQn);
}



void Dri_Key_Init(void){
  Dri_Key_Init_PF9();
  Dri_Key_Init_PF8();//这个是有问题的 测试的时候可以重新验证
  Dri_Key_Init_PF10();
  
}


/**
 * 中断向量表:可以查看手册,对应的中断函数在startup_stm32f10x_hd.s汇编文件里
 *
 * 中断服务函数没有返回值，也没有参数
 *
 * @description: line 15-10的中断服务函数.
 *  一旦按下按键(线路板上是sw5),会触发一次上升沿，则会执行一次这个函数
 * @return
 *
 *只针对GPIO的EXTI触发中断服务函数只有7个:5->9对应同一个,10->15对应同一个。
  0，1，2，3，4分别自己独立的
 */
void EXTI15_10_IRQHandler(void)
{
  // 判断当前是那种EXTI
  if (EXTI->PR & EXTI_PR_PR10)
  {
    /**
     * 务必一定必须要清除中断标志位
     * 如果想捕捉每一次的中断，把清除中断标志位放在最前，如果想已经进入中断，不想在
     * 中断执行的时候再产生中断，可以把清除中断标志位放在最后。主要是根据需求而定。
     * */
    // EXTI->PR |= EXTI_PR_PR10;
    EXTI->PR = EXTI_PR_PR10; // 这样写的效率更高，查看手册
    /**
     * 消抖：
     * 硬件消抖：加电容 电容越大，消抖能力越强，但是反应就越慢
     * 软件消抖：延时
     * 中断里加延时，在生产环境中是禁止的
     */
    Delay_ms(10); // 10到15ms消抖

    // 判断按键是否是真的按下
    if ((GPIOF->IDR & GPIO_IDR_IDR10) != 0)//稳定后引脚是3.3v
    {
      Dri_Led_Toggle(LED1);
    }
  }
}

void EXTI9_5_IRQHandler(void)
{
  // 判断当前是那种EXTI
  if (EXTI->PR & EXTI_PR_PR8)
  {
    /**
     * 务必一定必须要清除中断标志位
     * 如果想捕捉每一次的中断，把清除中断标志位放在最前，如果想已经进入中断，不想在
     * 中断执行的时候再产生中断，可以把清除中断标志位放在最后。主要是根据需求而定。
     * */
    // EXTI->PR |= EXTI_PR_PR8;
    EXTI->PR = EXTI_PR_PR8; // 这样写的效率更高，查看手册
    /**
     * 消抖：
     * 硬件消抖：加电容 电容越大，消抖能力越强，但是反应就越慢
     * 软件消抖：延时
     * 中断里加延时，在生产环境中是禁止的
     */
    Delay_ms(10); // 10到15ms消抖

    // 判断按键是否是真的按下
    if ((GPIOF->IDR & GPIO_IDR_IDR8) == 0) //稳定后引脚是0v
    {
      Dri_Led_Toggle(LED2);
    }
  }else if (EXTI->PR & EXTI_PR_PR9)
  {
    EXTI->PR = EXTI_PR_PR9; // 这样写的效率更高，查看手册
    Delay_ms(10); // 10到15ms消抖

    // 判断按键是否是真的按下
    if ((GPIOF->IDR & GPIO_IDR_IDR9) == 0) //稳定后引脚是0v
    {
      Dri_Led_Toggle(LED3);
    }
  }
}
