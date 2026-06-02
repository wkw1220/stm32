#ifndef __DRIVER_LED_H__
#define __DRIVER_LED_H__
#include "stm32f10x.h"
#include "Delay.h" 
#define LED1 GPIO_ODR_ODR0
#define LED2 GPIO_ODR_ODR1
#define LED3 GPIO_ODR_ODR8

#define LED1_ORANGE  LED1
#define LED2_BLUE LED2 
#define LED3_GREEN LED3 

#define LED1_BSRR_OFF GPIO_BSRR_BS0
#define LED2_BSRR_OFF GPIO_BSRR_BS1
#define LED3_BSRR_OFF GPIO_BSRR_BS8

#define LED1_BSRR_ON GPIO_BSRR_BR0
#define LED2_BSRR_ON GPIO_BSRR_BR1
#define LED3_BSRR_ON GPIO_BSRR_BR8

/**
 * @brief Led 初始化
 * 
 */
void Dri_Led_Init(void);

void Dri_Led_Init_WithInterrupt(void);

/**
 * @brief Led 打开
 * 
 */
void Dri_Led_On(uint32_t Led);

/**
 * @brief Led 打开 通过BSRR 端口设置清除寄存器
 * 
 */
void Dri_Led_On_BSRR(uint32_t LED_BSRR_ON);

/**
 * @brief 打开所有灯
 * 
 * @param Leds 
 * @param size 
 */
void Dri_Led_OnAll(uint32_t Leds[],uint8_t size);

/**
 * @brief Led 关闭
 * 
 */

void Dri_Led_Off(uint32_t Led);

/**
 * @brief Led 关闭 通过BSRR寄存器
 * 
 */

void Dri_Led_Off_BSRR(uint32_t LED_BSRR_OFF);

/**
 * @brief 关闭所有灯
 * 
 * @param Leds 
 * @param size 
 */
void Dri_Led_OffAll(uint32_t Leds[],uint8_t size);

/**
 * @brief 灯光闪烁
 * 
 * @param Led 
 */
void Dri_Led_Toggle(uint32_t Led);

#endif // !__DRIVER_LED_H__




