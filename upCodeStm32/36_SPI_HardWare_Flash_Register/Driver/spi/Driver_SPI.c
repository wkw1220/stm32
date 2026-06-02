/**
 * @file Driver_SPI.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-08
 * 
 * @copyright Copyright (c) 2025
 * MOSI:Master Output Slave Input
 * MISO:Master Input Slave Output
 * SS:片选线或者使能线. 有时候也称NSS或者CS
 * SCLK:System Clock
 * 只能主机和从机之间进行通信，不能从机和从机进行通信
 */
#include "Driver_SPI.h"

void Driver_SPI_Init(void)
{
    //Driver_SPI_Init_Soft();
    Driver_SPI_Init_HardWare();
    //Driver_SPI_Init_HardWare_Internet();
}

void Driver_SPI_Start(void)
{
    //Driver_SPI_Start_Soft();
    Driver_SPI_Start_HardWare();
    //Driver_SPI_Start_Internet();
}

void Driver_SPI_Stop(void)
{
    //Driver_SPI_Stop_Soft();
    Driver_SPI_Stop_HardWare();
    //Driver_SPI_Stop_Internet();
}

uint8_t Driver_SPI_SwapByte(uint8_t byte)
{
    //return Driver_SPI_SwapByte_Soft(byte);
    return Driver_SPI_SwapByte_HardWare(byte);
    //return Driver_SPI_SwapByte_Internet(byte);
}

//////////soft start..//////////
void Driver_SPI_Init_Soft(void)
{
    /* 1. 开启GPIO时钟 PA和PC*/
    RCC->APB2ENR |= (RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN);
    /* 2. 设置引脚的工作模式 */
    /* 2.1 cs: 推挽输出 PC13*  CNF=00 MODE=11 */
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13;
    /* 2.2 sck: 推挽输出 PA5*/
    /* 2.3 mosi: 推挽输出 PA7*/
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
    GPIOA->CRL |= (GPIO_CRL_MODE5 | GPIO_CRL_MODE7);
    /* 2.4 miso: 浮空输入 PA6  CNF=01 MODE=00*/
    GPIOA->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);
    GPIOA->CRL |= GPIO_CRL_CNF6_0;
    /* 3. spi的模式0  sck空闲状态是 0*/
    SCK_LOW;
    /* 4. 片选默认不选中 */
    CS_HIGH;
    /* 5. 延时 */
    SPI_DELAY;
}

/**
 * @brief 
 * 
 * @param byte:Master设备的数据
 *        rByte:Slave设备的数据
 * @return uint8_t 
 */
uint8_t Driver_SPI_SwapByte_Soft(uint8_t byte)
{
    uint8_t rByte = 0x00;
    for (uint8_t i = 0; i < 8; i++)
    {
        /* 1. 先把数据放入到MOSI上 */
        (byte & 0x80) ? MOSI_HIGH : MOSI_LOW;
        byte <<= 1;
        //SPI_DELAY;

        /* 2. 拉高时钟 (第一个跳变)*/
        SCK_HIGH;
        //SPI_DELAY;
        /* 3. 读取miso  (第一个跳变采样)*/
        rByte <<= 1;
        MISO_READ ? rByte |= 0x01 : rByte;
        /* 4. 拉低时钟 */
        SCK_LOW;
        //SPI_DELAY;
    }
    return rByte;
}

void Driver_SPI_Start_Soft(void)
{
    CS_LOW;
    //SPI_DELAY;//遇到速度低的最好加一下
}

void Driver_SPI_Stop_Soft(void)
{
    CS_HIGH;
    //SPI_DELAY;//遇到速度低的最好加一下
}
//////////soft end..//////////


//////////HardWare start..//////////
void Driver_SPI_Init_HardWare(void)
{
    /* 1. 开启GPIO时钟 PA和PC 开启SPI1的时钟*/
    RCC->APB2ENR |= (RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN| RCC_APB2ENR_SPI1EN);
    /* 2. 设置引脚的工作模式 */
    /* 2.1 cs: 通用推挽输出 PC13*  CNF=00 MODE=11 */
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13;
    /* 2.2 sck: 复用推挽输出 PA5  CNF=10 MODE=11*/
    /* 2.3 mosi: 复用推挽输出 PA7*/
    GPIOA->CRL &= ~(GPIO_CRL_CNF7_0|GPIO_CRL_CNF5_0);
    GPIOA->CRL |= (GPIO_CRL_MODE5 | GPIO_CRL_MODE7| GPIO_CRL_CNF7_1|GPIO_CRL_CNF5_1);
    /* 2.4 miso: 浮空输入 PA6  CNF=01 MODE=00*/
    GPIOA->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);
    GPIOA->CRL |= GPIO_CRL_CNF6_0;
    
    /* 3. SPI相关的配置 */
    /* 3.1 配置SPI1为主模式 */
    SPI1->CR1 |= SPI_CR1_MSTR;
    /* 3.2 NSS禁用, 从设备的片选使用普通的GPIO控制*/
    SPI1->CR1 |= SPI_CR1_SSM;
    SPI1->CR2 &= ~SPI_CR2_SSOE;
    SPI1->CR1 |= SPI_CR1_SSI;
    /* 3.3 配置SPI的工作模式 模式0   时钟极性和相位*/
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
    /* 3.4 配置波特率的分频系数 0=2分频 1=4分频 2=8分频 .... 最高只能到达18M*/
    SPI1->CR1 &= ~SPI_CR1_BR;
    SPI1->CR1 |= SPI_CR1_BR_1;
    /* 3.5 配置数据帧的格式: 8为或16位 */
    SPI1->CR1 &= ~SPI_CR1_DFF;
    /* 3.6 配置LSB 或 MSB*/
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    /* 3.7 使能SPI */
    SPI1->CR1 |= SPI_CR1_SPE;
    
    //默认不通讯，先不要使能
    CS_HIGH;
}

uint8_t Driver_SPI_SwapByte_HardWare(uint8_t byte)
{
    /* 1. 写数据到发送缓冲区 */
    /* 1.1 判断发送缓冲功区为空 */
     while ((SPI1->SR & SPI_SR_TXE) == 0);
    /* 1.2 把数据放入DR寄存器 */
    SPI1->DR = byte;
    /* 2. 读数据 */
    /* 2.1 先判断接收缓冲区非空 */
    while ((SPI1->SR & SPI_SR_RXNE) == 0);
    /* 2.1 从接收缓冲区读取数据 */
    return (uint8_t)(SPI1->DR & 0xff);
}

void Driver_SPI_Start_HardWare(void)
{
    CS_LOW;
}

void Driver_SPI_Stop_HardWare(void)
{
    CS_HIGH;
}
//////////HardWare end..//////////

//////////Internet start..//////////
void Driver_SPI_Init_HardWare_Internet(void)
{
    /* 1. 开启GPIO时钟 PG PB和PD 开启SPI2的时钟*/
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    RCC->APB2ENR |= (RCC_APB2ENR_IOPGEN | RCC_APB2ENR_IOPBEN| RCC_APB2ENR_IOPDEN);
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
    CS_LOW_INTERNET;
}

void Driver_SPI_Stop_Internet(void)
{
    CS_HIGH_INTERNET;
}
//////////Internet end..//////////



