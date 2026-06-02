#ifndef __DRIVER_LED_H__
#define __DRIVER_LED_H__
#include "main.h"

/**
 * @brief Led 打开
 * 
 */
void Dri_Led_On(uint16_t Led);

/**
 * @brief 打开所有灯
 * 
 * @param Leds 
 * @param size 
 */
void Dri_Led_OnAll(uint16_t Leds[],uint8_t size);

/**
 * @brief Led 关闭
 * 
 */

void Dri_Led_Off(uint16_t Led);

/**
 * @brief 关闭所有灯
 * 
 * @param Leds 
 * @param size 
 */
void Dri_Led_OffAll(uint16_t Leds[],uint8_t size);

/**
 * @brief 灯光闪烁
 * 
 * @param Led 
 */
void Dri_Led_Toggle(uint16_t Led);

#endif // !__DRIVER_LED_H__





