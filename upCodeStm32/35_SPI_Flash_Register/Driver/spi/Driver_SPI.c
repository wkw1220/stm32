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
    Driver_SPI_Init_Soft();
}

void Driver_SPI_Start(void)
{
    Driver_SPI_Start_Soft();
}

void Driver_SPI_Stop(void)
{
    Driver_SPI_Stop_Soft();
}

uint8_t Driver_SPI_SwapByte(uint8_t byte)
{
    return Driver_SPI_SwapByte_Soft(byte);
}

/**
 * @brief 使能四个引脚并配置输入输出模式
 * 配置mode 选择模式0
 * 初始时SCLK:时钟拉低
 * CS:低电平使能,所以在初始化的时候 需要把CS电平拉高
 * 时钟信号第一个上升沿就开始有效
 * 
 */
void Driver_SPI_Init_Soft(void)
{
    /* 1. 开启GPIO时钟 PA和PC*/
    RCC->APB2ENR |= (RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN);
    //生成环境使能以后就拉高拉低,防止模式改变后有一个跳变
    //SCK_LOW;
    //CS_HIGH;

    /* 2. 设置引脚的工作模式 */
    /* 2.1 cs(NSS/SS): 推挽输出 PC13*  CNF=00 MODE=11 */
    GPIOC->CRH &= ~GPIO_CRH_CNF13;
    GPIOC->CRH |= GPIO_CRH_MODE13;
    /* 2.2 sclk: 推挽输出 PA5*/
    /* 2.3 mosi: 推挽输出 PA7*/
    GPIOA->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_CNF7);
    GPIOA->CRL |= (GPIO_CRL_MODE5 | GPIO_CRL_MODE7);
    /* 2.4 miso: 浮空输入 PA6  CNF=01 MODE=00*/
    GPIOA->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);
    GPIOA->CRL |= GPIO_CRL_CNF6_0;
    /* 3. spi的模式0  sck空闲状态是0*/
    SCK_LOW;
    /* 4. 片选默认不选中 */
    CS_HIGH;
    /* 5. 延时 */
    SPI_DELAY;
}

/**
 * @brief 通讯开始使能拉低
 * 
 */
void Driver_SPI_Start_Soft(void)
{
    CS_LOW;
    //SPI_DELAY;//遇到速度低的最好加一下
}

/**
 * @brief 通讯开始使能拉高
 * 
 */
void Driver_SPI_Stop_Soft(void)
{
    CS_HIGH;
    //SPI_DELAY;//遇到速度低的最好加一下
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
        /* 1. 先把数据放入到MOSI上 高位先行 */
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


