#include "Driver_Led.h"

/**
 * @brief 初始化以后默认值会使灯全亮
 * 
 */
void Dri_Led_Init(){
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
	  //GPIOA->CRL|=0x33;
    GPIOA->CRL|=(GPIO_CRL_MODE0|GPIO_CRL_MODE1);
    GPIOA->CRL&=~(GPIO_CRL_CNF0|GPIO_CRL_CNF1);
    
    GPIOA->CRH|=GPIO_CRH_MODE8;
    GPIOA->CRH&=~GPIO_CRH_CNF8;
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
  if((GPIOA->IDR&Led)==0){
      Dri_Led_Off(Led);
  }else{
      Dri_Led_On(Led);
  }
}
