/**
 * @file Driver_SPI.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-08
 * 
 * @copyright Copyright (c) 2025
 * MOSI:Master Output Slave Input PB15
 * MISO:Master Input Slave Output PB14
 * SS:片选线或者使能线. 有时候也称NSS或者CS PD3
 * SCLK:System Clock PB13
 * 只能主机和从机之间进行通信，不能从机和从机进行通信
 */
#include "Driver_SPI.h"

//////////Internet start..//////////
void Driver_SPI_Init_HardWare_Internet(void)
{
    /* 1. 开启GPIO时钟 PG PB和PD 开启SPI2的时钟*/
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    RCC->APB2ENR |= (RCC_APB2ENR_IOPBEN| RCC_APB2ENR_IOPDEN);
    /* 2. 设置引脚的工作模式 */
    /* 2.1 cs: 推挽输出 PD3*  CNF=00 MODE=11 */
    GPIOD->CRL &= ~GPIO_CRL_CNF3;
    GPIOD->CRL |= GPIO_CRL_MODE3;
    /* 2.2 sck: 复用推挽输出 PB13  CNF=10 MODE=11*/
    /* 2.3 mosi: 复用推挽输出 PB15*/
    GPIOB->CRH |= (GPIO_CRH_MODE13 | GPIO_CRH_MODE15| GPIO_CRH_CNF13_1|GPIO_CRH_CNF15_1);
    GPIOB->CRH &= ~(GPIO_CRH_CNF13_0|GPIO_CRH_CNF15_0);
    /* 2.4 miso: 浮空输入 PB14  CNF=01 MODE=00*/
    GPIOB->CRH &= ~(GPIO_CRH_CNF14_1 | GPIO_CRH_MODE14);
    GPIOB->CRH |= GPIO_CRH_CNF14_0;
    /* 3. SPI相关的配置 */
    /* 3.1 配置SPI2为主模式 */
    SPI2->CR1 |= SPI_CR1_MSTR;
    /* 3.2 NSS禁用, 从设备的片选使用普通的GPIO控制*/
    SPI2->CR1 |= SPI_CR1_SSM;
    SPI2->CR2 &= ~SPI_CR2_SSOE;
    SPI2->CR1 |= SPI_CR1_SSI;
    /* 3.3 配置SPI的工作模式 模式0   时钟极性和相位*/
    SPI2->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
    /* 3.4 配置波特率的分频系数 0=2分频 1=4分频 2=8分频 ....*/
    SPI2->CR1 &= ~SPI_CR1_BR;
    SPI2->CR1 |= SPI_CR1_BR_1;
    /* 3.5 配置数据帧的格式: 8为或16位 */
    SPI2->CR1 &= ~SPI_CR1_DFF;
    /* 3.6 配置LSB 或 MSB*/
    SPI2->CR1 &= ~SPI_CR1_LSBFIRST;
    /* 3.7 使能SPI */
    SPI2->CR1 |= SPI_CR1_SPE;
}

uint8_t Driver_SPI_SwapByte_Internet(uint8_t byte)
{
    /* 1. 写数据到发送缓冲区 */
    /* 1.1 判断发送缓冲功区为空 */
     while ((SPI2->SR & SPI_SR_TXE) == 0);
    /* 1.2 把数据放入DR寄存器 */
    SPI2->DR = byte;
    /* 2. 读数据 */
    /* 2.1 先判断接收缓冲区非空 */
    while ((SPI2->SR & SPI_SR_RXNE) == 0);
    /* 2.1 从接收缓冲区读取数据 */
    return (uint8_t)(SPI2->DR & 0xff);
}

void Driver_SPI_Start_Internet(void)
{
    CS_LOW;
}

void Driver_SPI_Stop_Internet(void)
{
    CS_HIGH;
}

void Driver_SPI_Init(void)
{
    Driver_SPI_Init_HardWare_Internet();
}

void Driver_SPI_Start(void)
{
    Driver_SPI_Start_Internet();
}

void Driver_SPI_Stop(void)
{
    Driver_SPI_Stop_Internet();
}

uint8_t Driver_SPI_SwapByte(uint8_t byte)
{
    return Driver_SPI_SwapByte_Internet(byte);
}

//////////Internet end..//////////



