#include "Driver_GPIO.h"

void Dri_GPIO_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPGEN;

    /* RST 通用推挽 */
    GPIOG->CRL |= GPIO_CRL_MODE7;
    GPIOG->CRL &= ~GPIO_CRL_CNF7;

    /* INT PG6上拉输入 MODE=00 CNF=10*/
    GPIOG->CRL &= ~(GPIO_CRL_MODE6|GPIO_CRL_CNF6_0);
    GPIOG->CRL |= GPIO_CRL_CNF6_1;
    GPIOG->ODR |= GPIO_ODR_ODR6;
}

