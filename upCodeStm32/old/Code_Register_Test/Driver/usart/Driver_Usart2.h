#ifndef __DRI_USART2_H__
#define __DRI_USART2_H__
#include "stm32f10x.h" 

/**
 * @brief USART1初始化
 * 
 */
void Dri_USART2_Init(void);

/**
 * @brief 通过中断的当方式
 * 
 */
void Dri_USART2_Init_WithInterruput(void);

/**
 * @brief 发送一个字节数据
 * 
 * @param byteChar 
 */
void Dri_USART2_SendChar(uint8_t byteChar);

/**
 * @brief 发送一个字符串
 * 
 * @param str 
 */
void Dri_USART2_SendStr(uint8_t *str);

uint8_t Dri_USART2_ReceiveChar(void);

/**
 * @brief 接收字符串 利用空闲帧实现
 * 
 * @return 
 */
void Dri_USART2_ReceiveStr(u8* buff);


#endif // !__DRI_USTAR2_H__

