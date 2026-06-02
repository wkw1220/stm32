#ifndef __DRIVER_USART_H__
#define __DRIVER_USART_H__
#include "stm32f10x.h"
#include <stdio.h>

/**
 * @brief USART1初始化
 * 
 */
void Dri_USART_Init(void);

/**
 * @brief 通过中断的当方式
 * 
 */
void Dri_USART_Init_WithInterruput(void);

/**
 * @brief 发送一个字节数据
 * 
 * @param byteChar 
 */
void Dri_USART_SendByte(uint8_t byteChar);

/**
 * @brief 发送一个字符串
 * 
 * @param str 
 */
void Dri_USART_SendStr(uint8_t *str);

uint8_t Dri_USART_ReceiveByte(void);

/**
 * @brief 接收字符串 利用空闲帧实现
 * 
 * @return 
 */
void Dri_USART_ReceiveStr(u8* buff);


#endif // !__DRIVER_USTAR_H__

/**
 * 串口通信
 * 发送方的发送接接收方的接收，发送方的接收接接收方的发送，相对于比较是高电平还是低电平，所以他们还有
 * 一根共同的接地线。 所以一共有三根线
 * Universal Synchronous Asynchronous Receiver and Transmitter  
 * USART(通用同步异步收发器)：既可以同步也可以异步   串行 同步/异步 全双工
 * UART：只支持异步通信
 * 
 * 
 * 本芯片有五个串口，可以查阅相关手册
 * 
 * CH430芯片 是USB转串口的芯片
 */

