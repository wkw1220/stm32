/**
 * @file Int_W24C02.h
 * @author winder
 * @brief W24C02:是一款EE2PROM芯片型号 本案例是MCU芯片通过I2C通信的方式对存储芯片进行存储和读写数据
 *        C02芯片:是2Kbit，一共256个字节，一页是16个字节，也就是256/16=16页
 *        只有主机主动发送请求，从机是不会发送请求的
 * 
 * @version 0.1
 * @date 2025-07-29
 * 
 * @copyright Copyright (c) 2025
 *  
 */
#ifndef __INTEFACE_W24C02_H__
#define __INTEFACE_W24C02_H__
#include "Driver_I2C2_Soft.h"
#include "string.h"
/**
 * @brief //10100000 从设备的地址(移动过后的地址)
 *  七位地址 1010000(0x50) 第八位最后一位是读(1)或者写(0)
 *     
 */
#define ADDR 0xA0  

/**
 * @brief 存储芯片初始化
 * 
 */
void Inf_W24C02_Init(void);
/**
 * @brief cpu向存储器innerAddr这个地址写入一个字节
 *        结果是这个字节会存储在存储器的这个innerAddr地址
 * 
 * @param innerAddr 存储器的内部地址
 * @param byte 要向存储器存放的内容 按字节存储
 */
void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte);

/**
 * @brief CPU从存储器按字节读取内容
 * 
 * @param innerAddr:存储器的内部地址
 * @return uint8_t:读取的内容
 */
uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr);

/**
 * @brief 页写入:CPU向存储器写入多个字节
 * 
 * @param innerAddr:存储器内部地址 
 * @param bytes:存储内容，多个字节 
 * @param len:存储内容的字节长度 对于这个芯片如果len=16正好是一页
 * 00-0f
 * 10-1f
 * 20-2f
 * 如果写入的长度大于16，假设是17，此芯片不会自动换页，第17个字符会覆盖第一个字符 
 */
void Inf_W24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len);

/**
 * @brief 页读取:CPU向存储器读取多个字节
 * 
 * @param innerAddr:存储器内部地址 
 * @param bytes:读取到的存储内容保存的地址，多个字节 
 * @param len:读取内容的字节长度 对于这个芯片如果len=16正好是一页
 *  读的时候没有页的限制，但是写的时候有页的限制
 */
void Inf_W24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len);
#endif 
// __INTEFACE_W24C02_H__
