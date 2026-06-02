#include "Driver_Led.h"
#include "stm32f10x.h" 

void Dri_Led_Init(){
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
	  //GPIOA->CRL|=0x33;
    GPIOA->CRL|=(GPIO_CRL_MODE0|GPIO_CRL_MODE1);
    GPIOA->CRL&=~(GPIO_CRL_CNF0|GPIO_CRL_CNF1);
    
    GPIOA->CRH|=GPIO_CRH_MODE8;
    GPIOA->CRH&=~GPIO_CRH_CNF8;
    Dri_Led_Off(LED1);
    Dri_Led_Off(LED2);
    Dri_Led_Off(LED3);
}
 

void Dri_Led_On(uint32_t Led){  
	GPIOA ->ODR &= ~Led;  
}


void Dri_Led_Off(uint32_t Led){
  GPIOA ->ODR |= Led;
}

void Dri_Led_OnAll(uint32_t Leds[],uint8_t size){
  uint8_t i;
  for (i = 0; i < size; i++)
  {
    Dri_Led_On(Leds[i]);
  }  
}

void Dri_Led_OffAll(uint32_t Leds[],uint8_t size){
  uint8_t i;
  for (i = 0; i < size; i++)
  {
    Dri_Led_Off(Leds[i]);
  }  
}


void Dri_Led_Toggle(uint32_t Led){
  if((GPIOA->IDR&Led)==0){//说明灯是开着的
      Dri_Led_Off(Led);
  }else{
      Dri_Led_On(Led);
  }
}


void Dri_Led_On_BSRR(uint32_t LED_BSRR_ON){
    GPIOA->BSRR |=LED_BSRR_ON;
}


void Dri_Led_Off_BSRR(uint32_t LED_BSRR_OFF){
    GPIOA->BSRR |=LED_BSRR_OFF;
}

/**
 * @brief LED3 闪烁的时候触发蜂鸣器
 * 
 */
void Dri_Led_Init_WithInterrupt(void)
{
    /**1.开始时钟 **/
    /**1.1 开启GPIOA */
    RCC->APB2ENR |=RCC_APB2ENR_IOPAEN;

    /** 1.2 AFIO */
    RCC->APB2ENR |=RCC_APB2ENR_AFIOEN;

   /** 2.配置PA8 的输出模式  */
   GPIOA->CRH|=GPIO_CRH_MODE8;
   GPIOA->CRH&=~GPIO_CRH_CNF8;
   /* 3. 配置AFIO 配置PA8引脚为外部中断  EXTICR3  0101*/
   AFIO->EXTICR[2] &= ~AFIO_EXTICR3_EXTI8;
   AFIO->EXTICR[2] |= AFIO_EXTICR3_EXTI8_PA;
   /* 4. 配置EXTI */
   /* 4.1. 配置上升沿触发 RTSR TR8=1 说明电流即将变为1，灯关闭*/
   EXTI->FTSR |=EXTI_FTSR_TR8;
   EXTI->RTSR |=EXTI_RTSR_TR8;
   /* 4.2 开启 LINE8, 配置的中断屏蔽寄存器 */
   EXTI->IMR |=EXTI_IMR_MR8;

   /* 5. 配置 NVIC */
   /* 5.1 配置优先级组 (3-7) 配置3表示4个二进制位全部用于表示抢占优先级*/
   NVIC_SetPriorityGrouping(3);
   /* 5.2 配置优先级 参数1:中断号*/
   NVIC_SetPriority(EXTI9_5_IRQn, 3);
   /* 5.3 使能Line8 */
   NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void EXTI9_5_IRQHandler(){
  /* 务必一定必须要清除中断标志位 */
    EXTI->PR |= EXTI_PR_PR8;
    Delay_ms(5);
    //只为零 说明灯是亮着的 为1是灯熄灭的 
    if ((GPIOA->IDR & GPIO_IDR_IDR8) != 0)
    {
      Dri_Buzzer_Start();
      Dri_Led_On(LED1_ORANGE);
    }else{
      Dri_Buzzer_Stop();
      Dri_Led_Off(LED1_ORANGE);
    }
    
}

void Dri_xxx_Init(){
    RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;
	  
    GPIOC->CRH|=GPIO_CRH_MODE9;
    GPIOC->CRH&=~GPIO_CRH_CNF9;
}

void Dri_xxx_start(){
    GPIOC ->ODR &= ~GPIO_ODR_ODR9;
}

void Dri_xxx_stop(){
    GPIOC ->ODR |= GPIO_ODR_ODR9;
}