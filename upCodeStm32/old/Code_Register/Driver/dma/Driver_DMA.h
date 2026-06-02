#ifndef __DRIVER_DMA_H__
#define __DRIVER_DMA_H__
#include "stm32f10x.h" 
extern uint8_t isTransmitFinished;

/**
 * @brief 这个通道的作用是从ROM读取数据到RAM
 * 
 */
void Dri_DMA1_Channel1_Init(void);

/**
 * @brief 这个通道主要用于USART1_TX
 * 
 */
void Dri_DMA1_Channel4_Init(void);

/**
 * @brief 这个通道主要用于USART1_RX
 * 
 */
void Dri_DMA1_Channel5_Init(void);

void Dri_DMA1_TransimitDataFromRomToRam(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen);

void Dri_DMA1_TransimitDataFromRamToPerpherial(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen);

void Dri_DMA1_TransimitDataFromPerpherialToRam(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen);


#endif 