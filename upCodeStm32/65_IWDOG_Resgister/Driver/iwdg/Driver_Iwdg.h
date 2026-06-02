#ifndef __DRIVER_IWDG_H__
#define __DRIVER_IWDG_H__
#include "stm32f10x.h" 
// IWDG 初始化
void Dri_Iwdg_Init(uint8_t prer,uint16_t rlr);

// 喂狗 刷新计数器的值
void Dri_Iwdg_Feed(void);
#endif

