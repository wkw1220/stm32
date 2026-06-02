#include "Inf_key.h"

uint8_t key3IsPressed=0;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == KEY3_Pin)
    {
        HAL_Delay(7);
        if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_SET)
        {
            //HAL_GPIO_TogglePin(LED_0_GPIO_Port, LED_0_Pin);
            //printf("key3╟╢обак...\r\n");
            key3IsPressed=1;
        }
    } 
}

