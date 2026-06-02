#include "Driver_Led.h"


void Dri_Led_On(uint16_t led){  
	HAL_GPIO_WritePin(GPIOA,led,GPIO_PIN_RESET);  
}


void Dri_Led_Off(uint16_t led){
  HAL_GPIO_WritePin(GPIOA,led,GPIO_PIN_SET); 
}

void Dri_Led_OnAll(uint16_t Leds[],uint8_t size){
  uint8_t i;
  for (i = 0; i < size; i++)
  {
    Dri_Led_On(Leds[i]);
  }  
}

void Dri_Led_OffAll(uint16_t Leds[],uint8_t size){
  uint8_t i;
  for (i = 0; i < size; i++)
  {
    Dri_Led_Off(Leds[i]);
  }  
}


void Dri_Led_Toggle(uint16_t Led){
   HAL_GPIO_TogglePin(GPIOA,Led);
}
