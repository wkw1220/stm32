#ifndef __DRIVER_LED_H__
#define __DRIVER_LED_H__
#include "stm32f10x.h" 
#define LED1 GPIO_ODR_ODR0
#define LED2 GPIO_ODR_ODR1
#define LED3 GPIO_ODR_ODR8

/**
 * @brief Led 初始化
 * 
 */
void Dri_Led_Init(uint32_t Leds[],uint8_t size);

/**
 * @brief Led 打开
 * 
 */
void Dri_Led_On(uint32_t Led);

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




