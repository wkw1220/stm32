#include "Driver_DMA.h"
/*
 *DMA具有12个独立可编程的通道，其中 DMA1有7个通道，DMA2有5个通道
 */

void Dri_DMA1_Channel1_Init(void)
{
    /* 1. 开启 DAM 时钟 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    /* 2. DMA相关的配置 */
    /* 2.1 DMA方向: 从存储器(ROM)到存储器(RAM) . 通道随意*/
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR; // 0: 从外设读 1:从存储器读
    DMA1_Channel1->CCR |= DMA_CCR1_MEM2MEM;
    /* 2.2 设置存储器和外设的数据宽度:  8位(一个字节) 16位(半字) 32位(字) */
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE; /* 00:8位 01:16位 10:32位 11:保留 */
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE;
    /* 2.3 设置外设和存储器的地址是否自增 */
    DMA1_Channel1->CCR |= DMA_CCR1_PINC; /* 外设地址自增 */
    DMA1_Channel1->CCR |= DMA_CCR1_MINC; /* 存储器地址自增 */
    /* 2.4 开启传输完成的中断 */
    DMA1_Channel1->CCR |= DMA_CCR1_TCIE;
    /* 2.5 NVIC的设置 */
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel1_IRQn, 3);
    NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

void Dri_DMA1_Channel4_Init(void)
{
    /* 1. 开启 DAM 时钟 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    /* 2. DMA相关的配置 */
    /* 2.1 DMA方向: 从存储器(RAM)到串口(外设) . 通道4*/
    DMA1_Channel4->CCR |= DMA_CCR4_DIR; // 0: 从外设读 1:从存储器读

    /* 2.2 设置存储器和外设的数据宽度:  8位(一个字节) 16位(半字) 32位(字) */
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE; /* 00:8位 01:16位 10:32位 11:保留 */
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;
    /* 2.3 设置外设和存储器的地址是否自增 */
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC; /* 串口外设地址不能自增 */
    DMA1_Channel4->CCR |= DMA_CCR4_MINC;  /* 存储器地址自增 */
    /* 2.4 开启传输完成的中断 */
    DMA1_Channel4->CCR |= DMA_CCR4_TCIE;

    /* 2.5 使能串口的DMA传输 */
    USART1->CR3 |= USART_CR3_DMAT;
    /* 2.6 开启循环模式 */
    DMA1_Channel4->CCR |= DMA_CCR4_CIRC;

    /* 2.5 NVIC的设置 */
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 3);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}

void Dri_DMA1_Channel5_Init(void)
{
    /* 1. 开启 DAM 时钟 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    /* 2. DMA相关的配置 */
    /* 2.1 DMA方向: 从存储器(RAM)到串口(外设) . 通道5*/
    DMA1_Channel5->CCR &= ~DMA_CCR5_DIR; // 0: 从外设读 1:从存储器读

    /* 2.2 设置存储器和外设的数据宽度:  8位(一个字节) 16位(半字) 32位(字) */
    DMA1_Channel5->CCR &= ~DMA_CCR5_PSIZE; /* 00:8位 01:16位 10:32位 11:保留 */
    DMA1_Channel5->CCR &= ~DMA_CCR5_MSIZE;
    /* 2.3 设置外设和存储器的地址是否自增 */
    DMA1_Channel5->CCR &= ~DMA_CCR5_PINC; /* 串口外设地址不能自增 */
    DMA1_Channel5->CCR |= DMA_CCR5_MINC;  /* 存储器地址自增 */
    /* 2.4 开启传输完成的中断 */
    DMA1_Channel5->CCR |= DMA_CCR5_TCIE;

    /* 2.5 使能串口的DMA传输 */
    USART1->CR3 |= USART_CR3_DMAR;
    /* 2.6 开启循环模式 */
    DMA1_Channel5->CCR |= DMA_CCR5_CIRC;

    /* 2.5 NVIC的设置 */
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel5_IRQn, 3);
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

/**
 * @brief DMA传输不涉及外设，所以通道随便选。我们选DMA1的1通道。
 *   只能从ROM到RAM， 不能从RAM到ROM
 * @param srcAddr
 * @param desAddr
 * @param dataLen
 */
void Dri_DMA1_TransimitDataFromRomToRam(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen)
{
    /* 1. 设置外设地址 */
    DMA1_Channel1->CPAR = srcAddr;
    /* 2. 设置存储器地址 */
    DMA1_Channel1->CMAR = desAddr;

    /* 3. 设置要传输的数据量 */
    DMA1_Channel1->CNDTR = dataLen;
    /* 4. 开启通道,开始传输 */
    DMA1_Channel1->CCR |= DMA_CCR1_EN;
}

/**
 * @brief 从RAM读 然后传输到外设
 *
 * @param srcAddr
 * @param desAddr
 * @param dataLen
 */
void Dri_DMA1_TransimitDataFromRamToPerpherial(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen)
{
    /* 1. 设置外设地址 */
    DMA1_Channel4->CPAR = desAddr;
    /* 2. 设置存储器地址 */
    DMA1_Channel4->CMAR = srcAddr;

    /* 3. 设置要传输的数据量 */
    DMA1_Channel4->CNDTR = dataLen;
    /* 4. 开启通道,开始传输 */
    DMA1_Channel4->CCR |= DMA_CCR4_EN;
}

/**
 * @brief 从外设读取到RAM
 *
 * @param srcAddr
 * @param desAddr
 * @param dataLen
 *
 * 把W24C02的数据通过DMA的方式读入到RAM 中
 * Inf_W24C02_Init();
    //Inf_W24C02_WriteBytes(0x00,str1,6);
    Inf_W24C02_ReadBytes(0x00,des11,6);
    printf("%s\r\n", des11);
 * uint8_t des11[100]={0};
 *  Dri_DMA1_Channel5_Init();
    Delay_ms(1000);
    Dri_DMA1_TransimitDataFromPerpherialToRam(des11, ADDR, 6);
    printf("%s\r\n", des11);
 */
void Dri_DMA1_TransimitDataFromPerpherialToRam(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen)
{
    /* 1. 设置外设地址 */
    DMA1_Channel5->CPAR = srcAddr;
    /* 2. 设置存储器地址 */
    DMA1_Channel5->CMAR = desAddr;

    /* 3. 设置要传输的数据量 */
    DMA1_Channel5->CNDTR = dataLen;
    /* 4. 开启通道,开始传输 */
    DMA1_Channel5->CCR |= DMA_CCR5_EN;
}

uint8_t isTransmitFinished = 0;
void DMA1_Channel1_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF1)
    {                                  // 说明已经传输完成
        DMA1->IFCR |= DMA_IFCR_CTCIF1; // 清除中断传输完成标志
        /* 通道不需要了,可以关闭 */
        DMA1_Channel1->CCR &= ~DMA_CCR1_EN;
        isTransmitFinished = 1;
    }
}

void DMA1_Channel4_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF4)
    {                                  // 说明已经传输完成
        DMA1->IFCR |= DMA_IFCR_CTCIF4; // 清除中断传输完成标志
        /* 通道不需要了,可以关闭 */
        DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
        isTransmitFinished = 1;
    }
}

void DMA1_Channel5_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF5)
    {                                  // 说明已经传输完成
        DMA1->IFCR |= DMA_IFCR_CTCIF5; // 清除中断传输完成标志
        /* 通道不需要了,可以关闭 */
        DMA1_Channel5->CCR &= ~DMA_CCR5_EN;
        isTransmitFinished = 1;
    }
}
