#include "Driver_Vibrator.h"
#include "Driver_Led.h"
//#include "Driver_Buzzer.h"

void Dri_Vibrator_Init(void)
{
    /**1.开始时钟 **/
    /**1.1 开启GPIOF */
    RCC->APB2ENR |=RCC_APB2ENR_IOPFEN;

    /** 1.2 AFIO */
    RCC->APB2ENR |=RCC_APB2ENR_AFIOEN;

   /** 2.配置PF10 的输入模式 MODE：输入模式00 CNF CNF:10(上拉或者下拉) ODR：0下拉 1：上拉 */
   GPIOF->CRH &= ~GPIO_CRH_MODE12;
   GPIOF->CRH &= ~GPIO_CRH_CNF12_0;
   GPIOF->CRH |= GPIO_CRH_CNF12_1;
   GPIOF->ODR &= ~GPIO_ODR_ODR12; //下拉
   /* 3. 配置AFIO 配置PF11引脚为外部中断 ?EXTICR3 ?0101*/
   AFIO->EXTICR[3] &= ~AFIO_EXTICR4_EXTI12;
   AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI12_PF;
   /* 4. 配置EXTI */
   /* 4.1. 配置上升沿触发 RTSR TR11=1*/
   EXTI->RTSR |=EXTI_RTSR_TR12;
   /* 4.2 开启 LINE11, 配置的中断屏蔽寄存器 */
   EXTI->IMR |=EXTI_IMR_MR12;

   /* 5. 配置 NVIC */
   /* 5.1 配置优先级组 (3-7) 配置3表示4个二进制位全部用于表示抢占优先级*/
   NVIC_SetPriorityGrouping(3);
   /* 5.2 配置优先级 参数1:中断号*/
   NVIC_SetPriority(EXTI15_10_IRQn, 3);
   /* 5.3 使能Line11 */
   NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
?* @description: line 15-10的中断服务函数.
?* ?一旦按键下按键1,则会执行一次这个函数
?* @return 
?*/
//uint8_t flagVibrator=0;
void EXTI15_10_IRQHandler1(void)//
{
    /* 务必一定必须要清除中断标志位 */
    EXTI->PR |= EXTI_PR_PR12;
    //Delay_ms(5);
    if ((GPIOF->IDR & GPIO_IDR_IDR12) != 0)
    {
      //flagVibrator=1;
    }
}
