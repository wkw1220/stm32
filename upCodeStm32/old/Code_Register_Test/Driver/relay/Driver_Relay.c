#include "Driver_Relay.h"

/**
 * @brief 继电器相关操作 低电平导通
 * 
 */
void Dri_Relay_Init(){
    RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;
    GPIOC->CRH|=GPIO_CRH_MODE9;
    GPIOC->CRH&=~GPIO_CRH_CNF9;
}

void Dri_Relay_start(){
    GPIOC ->ODR &= ~GPIO_ODR_ODR9;
}

void Dri_Relay_stop(){
    GPIOC ->ODR |= GPIO_ODR_ODR9;
}