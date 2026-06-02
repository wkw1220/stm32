#ifndef __DRIVER_ADC_H__
#define __DRIVER_ADC_H__
#include "stm32f10x.h"

void Dri_ADC1_Init(void);
void Dri_ADC1_StartConvert(void);
double Dri_ADC1_ReadVolate(void);
void Dri_ADC1_Init_MultiChannel(void);
void Dri_ADC1_DMA_Init(void);
void Driver_ADC1_DMA_Start(uint32_t desAddr, uint8_t dataLen);

#endif // !__DRIVER_ADC_H__

/**
 * @brief 规则通道多通道连续采集模式
 * 扫描完一个通道并采集，然后把数据存入规则通道数据寄存器 然后再扫描第二个通道并采集，然后把
 * 采集到的数据存入到规则通道寄存器中，这样数就会立即覆盖掉上一个通道采集的数据。那么如何读取数据?
 * 1->利用中断的方式:中断产生的条件是遍历完规则通道里所有通道完以后才会产生中断，此时数据已经全部都被
 *    覆盖，就剩最后一次通道采集的数据 所以不行。
 * 2->DMA方式:每一个通道采集完以后都会发生一个DMA请求
 *    如果各个通道的采集时间不一样会导致什么情况????
 * 
 * 单通道可以通过循环采样把电容充满
 * 但是多通道不行，测量不准需要改变采样时间
 */