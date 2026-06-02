#ifndef __DRIVER_BKP_H__
#define __DRIVER_BKP_H__
#include "stm32f10x.h"
void Dri_BKP_Init(void);
void Dri_BKP_WriteData(u16 data);
u16 Dri_BKP_ReadData();
#endif
