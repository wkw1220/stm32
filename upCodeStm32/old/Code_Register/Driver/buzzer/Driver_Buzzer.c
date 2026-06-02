#include "Driver_Buzzer.h"


/**
 * @brief Buzzer Init Function
 * Use GPIOP C9
 * 
 */
void Dri_Buzzer_Init(){
   RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;
   GPIOC->CRH|=GPIO_CRH_MODE8;
   GPIOC->CRH&=~GPIO_CRH_CNF8;
}


void Dri_Buzzer_Start(){
   GPIOC->ODR &=~GPIO_ODR_ODR8;
}

void Dri_Buzzer_Stop(){
   GPIOC->ODR |=GPIO_ODR_ODR8;
}

void Dri_Buzzer_Toggle()
{
   if((GPIOC->ODR&GPIO_ODR_ODR8)==0)
   {
      Dri_Buzzer_Stop();
   }else{
      Dri_Buzzer_Start();
   }
}
