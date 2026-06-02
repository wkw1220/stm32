#include "Driver_I2C2_Soft.h"

/**
 * @brief 初始化
 * 本质上是对GPIO的初始化
 * 用软件去模拟I2C通讯
 * 
 * 注意:在I2C开始通讯前,SDA和SCL应该是什么状态? 高电平
 *      在初始化以后，应该默认是高阻态 表示空闲
 * 
 * SCL的控制权一直在主设备,当SCL为高电平时才能开始对SDA进行采样
 * SDA的控制权有可能在主设备，也有可能在从设备
 * 
 */
void Dri_I2C2_Init_Soft(void)
{
    /**
     * @brief 
     * PB10和PB11引脚的模式是相同的
     * PB10->SCL
     * PB11->SDA
     * 开漏输出：既可以用于输出也可以用于输入，外界要有上拉电阻
     * 用于输入的时候，最好先输出一个1，把线的控制权交给外界
     * MODE=11 CNF=01
     */
    RCC->APB2ENR |=RCC_APB2ENR_IOPBEN;
    
    //new:在初始化以后，应该默认是高阻态 表示空闲
    SDA_HIGH;
    SCL_HIGH;
   
    GPIOB->CRH |=(GPIO_CRH_MODE10|GPIO_CRH_MODE11|GPIO_CRH_CNF10_0|GPIO_CRH_CNF11_0);
    GPIOB->CRH &=~(GPIO_CRH_CNF10_1|GPIO_CRH_CNF11_1);
}

/**
 * @brief 开始信号
 * 约定:每一个函数后，都必须维护好状态
 * 除了STOP,SCL必须低 SDA必须高
 */
void Dri_I2C2_Start_Soft(void)
{
     /* 1. 拉高sda和scl */
     SDA_HIGH;
     SCL_HIGH;
     /* 2. 延时 */
     I2C_DELAY;
     /* 3. 拉低sda */
     SDA_LOW;
     /* 4. 延时 */
     I2C_DELAY;

     //new：为了防止误采样，必须要保证结束时SCL处于低电平
     SCL_LOW;//防止误采样，时钟占用时钟线
     SDA_HIGH;//保证SDA是高电平 这样别的设备可以占用

    //保证时钟线为低电平的时间足够长，也是为了照顾低速的设备
     I2C_DELAY;

}

/**
 * @brief 停止信号
 * 
 */
void Dri_I2C2_Stop_Soft(void)
{
    //前面已经约定好了 其它函数必须以SDA_HIGH结束
    /* 1. scl拉高 sda拉低 */
    SDA_LOW;
    SCL_HIGH;
    
    /*2.延时*/
    I2C_DELAY;
    /* 3.拉高sda */
     SDA_HIGH;
    /*4.延时*/
    I2C_DELAY;
}

/**
 * @brief 接收方产生应答信号
 * 
 */
void Dri_I2C2_Ack_Soft(void)
{
    /* 1.sda拉高 scl拉低 */
    //SDA_HIGH;
    //SCL_LOW;
    /* 2. 延时*/
    //I2C_DELAY;
    /* 3. sda拉低*/
    SDA_LOW;
    /* 4. 延时*/
    I2C_DELAY;
    /* 5. scl拉高*/
    SCL_HIGH;
    /* 6. 延时*/
    I2C_DELAY;
    /* 7. scl拉低*/
    SCL_LOW;
    /* 8. 延时*/
    I2C_DELAY;
    /* 9. sda拉高*/
    SDA_HIGH;
    /* 10. 延时*/
    I2C_DELAY;
    //结束时 SCL低 SDA高
}

/**
 * @brief 接收方产生非应答信号
 * 
 */
void Dri_I2C2_NoAck_Soft(void)
{
    /* 1.sda拉高 scl拉低 */
    //SDA_HIGH;
    //SCL_LOW;
    /* 2. 延时*/
    //I2C_DELAY;
    /* 3. scl拉高*/
    SCL_HIGH;
    /* 4. 延时*/
    I2C_DELAY;
    /* 5. scl拉低*/
    SCL_LOW;
    /* 6. 延时*/
    I2C_DELAY;
    //结束时 SCL低 SDA高
}

/**
 * @brief 等待接收方法的应答
 * 
 */
uint8_t Dri_I2C2_WaitAck_Soft(void)
{
    /* 1. 把sda拉高, sda的主动权交给对方(e2prom芯片) */
    //SDA_HIGH;

    /* 2. scl拉低  */
    //SCL_LOW;
    /* 3. 延时*/
    //I2C_DELAY;
    /* 4. scl拉高*/
    SCL_HIGH;
    /* 5. 延时*/
    I2C_DELAY;
    /* 6.读取sda电平 */
    uint8_t ack=ACK;
    if(READ_SDA){
        ack=NACK;
    }
    /* 7. scl拉低  */
    SCL_LOW;
    /* 8. 延时*/
    I2C_DELAY;
    //结束时 SCL低 SDA高
    return ack;
}

/**
 * @brief 发送一个字节的数据,高位先行
 * 
 * @param byte 要发送的字节
 */
void Dri_I2C2_SendByte_Soft(uint8_t byte)
{
    for(uint8_t i=0;i<8;i++)
    {
       /* 1.拉低sda 和 scl */
       //SDA_LOW;
       //SCL_LOW;
       /* 延时 */
       //I2C_DELAY;
       /* 2.向sda写数据 要找到最高位 1.可以向左移动七位 2.和0x80  1000 0000 与得到*/
       if(byte&0x80){
        SDA_HIGH;
       }else
       {
        SDA_LOW;
       }
       I2C_DELAY;
       /* 3.拉高scl */
       SCL_HIGH;
       /* 延时 */
       I2C_DELAY;
       /* 4.拉高sda 和 和拉低scl */
       SCL_LOW;

       //new:最好把SDA拉高
       SDA_HIGH;//最后一次发送 也能保持前面的约定

       I2C_DELAY;

       /** 5.左移1位，为下一次发送做准备 */
       byte<<=1;

       //结束时 SCL低 SDA高
    }
}

/**
 * @brief 读一个字节的数据
 * 
 * @return uint8_t data 要读取的字节
 */
uint8_t Dri_I2C2_ReceiveByte_Soft(void)
{
    uint8_t data=0;
    for(uint8_t i=0;i<8;i++)
    {
        /* 1.拉低scl */
       SCL_LOW;
       /* 2.延时 */
       I2C_DELAY;
       /* 3.拉高scl */
       SCL_HIGH;
       /* 4.延时 */
       I2C_DELAY;
       /* 5.向sda写数据 */
       data<<=1;
       if(READ_SDA){
        data|=0x01;
       }
       /* 6.拉低scl */
       SCL_LOW;
       /* 7.延时 */
       I2C_DELAY;
    }
    //结束时 SCL低 SDA高
    return data;
}
