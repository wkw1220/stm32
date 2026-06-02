#include "Driver_M433.h"
/**
 * @brief 433M相关初始化
 *
 */
void Dri_M433_Init(void)
{
    /* 配置引脚F11 -->按键D */
    /**1.开始时钟 **/
    /**1.1 开启GPIOF */
    RCC->APB2ENR |= RCC_APB2ENR_IOPFEN;

    /** 1.2 AFIO */
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    /** 2.配置PF10 的输入模式 MODE：输入模式00 CNF CNF:10(上拉或者下拉) ODR：0下拉 1：上拉 */
    GPIOF->CRH &= ~(GPIO_CRH_MODE11 | GPIO_CRH_MODE10);
    GPIOF->CRH &= ~(GPIO_CRH_CNF11_0 | GPIO_CRH_CNF10_0);
    GPIOF->CRH |= (GPIO_CRH_CNF11_1 | GPIO_CRH_CNF10_1);
    GPIOF->ODR &= ~(GPIO_ODR_ODR11 | GPIO_ODR_ODR10); // 下拉
    /* 3. 配置AFIO 配置PF11引脚为外部中断 ?EXTICR3 ?0101*/
    AFIO->EXTICR[2] &= ~(AFIO_EXTICR3_EXTI11 | AFIO_EXTICR3_EXTI10);
    AFIO->EXTICR[2] |= (AFIO_EXTICR3_EXTI11_PF | AFIO_EXTICR3_EXTI10_PF);
    /* 4. 配置EXTI */
    /* 4.1. 配置上升沿触发 RTSR TR11=1*/
    /*
      开始设备电压为低电压，按下后电压有低变高，上升沿，松开后电压一直保持在高电压。
      寄存器IDR中的值也为1. 再按一下，电压有高电压变为低电压，松开后，电压保持在低电压，IDR中的值为0
      所以 按下后电压值会保留的状态，下降沿中断也需要触发
     */
    EXTI->RTSR |= (EXTI_RTSR_TR11|EXTI_RTSR_TR10);
    EXTI->FTSR |= (EXTI_FTSR_TR11|EXTI_FTSR_TR10);
    /* 4.2 开启 LINE11, 配置的中断屏蔽寄存器 */
    EXTI->IMR |= (EXTI_IMR_MR11|EXTI_IMR_MR10);

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
uint8_t flagM433 = 0;
uint8_t flagM433D1 = 0;
uint8_t flagVibrator=0;
void EXTI15_10_IRQHandler(void)
{
    /* 务必一定必须要清除中断标志位 */
    EXTI->PR |= EXTI_PR_PR11;
    if ((GPIOF->IDR & GPIO_IDR_IDR11) != 0)
    {
        flagM433 = 1;
    }
    else
    {
        flagM433 = 0;
    }

    EXTI->PR |= EXTI_PR_PR10;
    if ((GPIOF->IDR & GPIO_IDR_IDR10) != 0)
    {
        flagM433D1 = 1;
    }
    else
    {
        flagM433D1 = 0;
    }

    EXTI->PR |= EXTI_PR_PR12;
    if ((GPIOF->IDR & GPIO_IDR_IDR12) != 0)
    {
        flagVibrator = 1;
    }
    
}
