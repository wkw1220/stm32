#include "Driver_I2C2_HardWare.h"

/**
 * @brief 初始化
 * 
 */
void Dri_I2C2_Init_Hard(void)
{
     /* 1. 开启时钟 */
     /* 1.1 i2c硬件的时钟 */
     RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
     /* 1.2 GPIO时钟 */
     RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

     /* 2. 设置gpio的引脚的工作模式 */
    /**
     * @brief 
     * PB10->SCL
     * PB11->SDA
     * 开漏输出：既可以用于输出也可以用于输入，外界要有上拉电阻
     * 用于输入的时候，最好先输出一个1，把线的控制权交给外界
     * MODE=11 CNF=01 这是软件模式
     * MODE=11 CNF=11 硬件这个模式一定要改 
     */
    //GPIOB->CRH |=(GPIO_CRH_MODE10|GPIO_CRH_MODE11|GPIO_CRH_CNF10_0|GPIO_CRH_CNF11_0);
    //GPIOB->CRH &=~(GPIO_CRH_CNF10_1|GPIO_CRH_CNF11_1);
    GPIOB->CRH |= (GPIO_CRH_MODE10 | GPIO_CRH_MODE11 | GPIO_CRH_CNF10 | GPIO_CRH_CNF11);
    /* 3. 设置I2C2 */
    /* 3.1 配置硬件的工作模式  I2C  */
    I2C2->CR1 &= ~I2C_CR1_SMBUS;

    /* 3.2 配置给I2C设备提供的时钟的频率 36MHz*/
    I2C2->CR2 |= 36 << 0;

    /* 3.3 设置标准模式=0 or 快速模式=1 */
    I2C2->CCR &= ~I2C_CCR_FS;

    /* 3.3 配置I2C产生时钟的频率 100K or 400K
      Thigh=CCR * Tcplk1
       ccr = Thigh/=Tcplk1 = 5us / (1/36)us = 180
    */
   I2C2->CCR |= 180 << 0;
   /* 3.4 时钟信号的上升沿
      100KHz的时候要求最大上升沿不超过1us(手册)。
      时钟频率是36MHz则 写入：1 /（1/36） + 1 = 37
      其实就是计算的 最大上升沿时间/时钟周期 + 1
    */
   I2C2->TRISE |= 37;
   /* 3.4 使能I2C */
   I2C2->CR1 |= I2C_CR1_PE;
}

/**
 * @brief 开始信号
 * 
 */
uint8_t Dri_I2C2_Start_Hard(void)
{
     I2C2->CR1 |= I2C_CR1_START;
     uint16_t timeout = 0xffff;
     while (((I2C2->SR1 & I2C_SR1_SB) == 0) && timeout)
     {
       timeout--; 
     }
     return timeout ? OK : FAIL;
}

/**
 * @brief 停止信号
 * 
 */
void Dri_I2C2_Stop_Hard(void)
{
    /* 产生终止条件 */
    I2C2->CR1 |= I2C_CR1_STOP;
}

/**
 * @brief 接收方产生应答信号
 * 
 */
void Dri_I2C2_Ack_Hard(void)
{
   /* 产生应答信号 */
   I2C2->CR1 |= I2C_CR1_ACK;
}

/**
 * @brief 接收方产生非应答信号
 * 
 */
void Dri_I2C2_NoAck_Hard(void)
{
   I2C2->CR1 &= ~I2C_CR1_ACK;
}

/**
 * @description: 发送一个设备地址
 * @param {uint8_t} byte
 */
uint8_t Driver_I2C_SendAddr(uint8_t addr)
{
   // 把要发送的数据写入到数据寄存器
   I2C2->DR = addr;
   uint16_t timeout = 0xffff;
   while (((I2C2->SR1 & I2C_SR1_ADDR) == 0) && timeout)
   {
      timeout--;
   }
   if (timeout)
   {
    I2C2->SR2;
   }
   return timeout ? OK : FAIL;
}
/**
 * @brief 发送一个字节的数据
 * 
 * @param byte 要发送的字节
 */
uint8_t Dri_I2C2_SendByte_Hard(uint8_t byte)
{
    uint16_t timeout = 0xffff;
    while (((I2C2->SR1 & I2C_SR1_TXE) == 0) && timeout)
    {
        timeout--;
    }
    I2C2->DR = byte;
    timeout = 0xffff;
    while (((I2C2->SR1 & I2C_SR1_BTF) == 0) && timeout)
    {
       timeout--;
    }
    return timeout ? OK : FAIL;
}

/**
 * @brief 读一个字节的数据
 * 
 * @return uint8_t data 要读取的字节
 */
uint8_t Dri_I2C2_ReceiveByte_Hard()
{
    uint16_t timeout = 0xffff;
    while (((I2C2->SR1 & I2C_SR1_RXNE) == 0) && timeout)
    {
        timeout--;
    }
    uint8_t data=timeout ? I2C2->DR : 0;
    return data;
}
