#include "Driver_ADC.h"

/**
 * @brief 初始化 单通道
 *
 */
void Dri_ADC1_Init(void)
{
    /* 1. 时钟配置 */
    /* 1.1 adc时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // 分配要加上 6分频 12M
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;
        /* 1.2 gpio的时钟 */
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    /* 2. gpio工作模式: PC0 模拟输入  CNF=00 MODE=00 */
    GPIOC->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);

    /* 2. ADC相关配置 */
    /* 2.1 禁用扫描模式. 只有一个通道不用扫描 */
    ADC1->CR1 &= ~ADC_CR1_SCAN;
    /* 2.2 启用连续转换模式 CR2=CONT 1*/
    ADC1->CR2 |= ADC_CR2_CONT;
    /* 2.3 数据对齐方式: 右对齐 左对齐 */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;
    /* 2.4 设置采样时间 ADC_SMPR1  010=13.5周期 第10通道采样*/
    ADC1->SMPR1 &= (ADC_SMPR1_SMP10_0 | ADC_SMPR1_SMP10_2);
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;
    /* 2.6 通道组的配置 */
    /* 2.6.1 配置几个通道需要转换 */
    ADC1->SQR1 &= ~ADC_SQR1_L;
    /* 2.6.2 把通道号配置到组里面.  */
    ADC1->SQR3 &= ~ADC_SQR3_SQ1; /* 先把5位清零 */
    ADC1->SQR3 |= 10 << 0;       /* 设置最后5位 */
    /* 2.7 选择软件触发 */
    ADC1->CR2 &= ~ADC_CR2_EXTTRIG; /* 禁用规则组的外部转换 */
    ADC1->CR2 |= ADC_CR2_EXTSEL;   /* 选择使用软件触发ADC */
}

void Dri_ADC1_StartConvert(void)
{
    /* 1. 上电: 把ADC从休眠模式唤醒 */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 2. 执行校准 */
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL)
        ;

    /* 3. ADON = 1, 开始转换 0>1 从休眠模式唤醒 1->1 开始 */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 4. 使用软件开始转换规则通道 */
    ADC1->CR2 |= ADC_CR2_SWSTART;

    /* 5. 等待首次转换完成 */
    while ((ADC1->SR & ADC_SR_EOC) == 0);
}

double Dri_ADC1_ReadVolate(void)
{
    // 12位的ADC 范围 [0, 4095]
    return ADC1->DR * 3.3 / 4095;
}



/**
 * @brief 初始化 多通道
 *  PC0 PC2
 */
void Dri_ADC1_Init_MultiChannel(void)
{
    /* 1. 时钟配置 */
    /* 1.1 adc时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    // 分配要加上 6分频 12M
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;
    /* 1.2 gpio的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    /* 2. gpio工作模式: PC0 PC2 模拟输入  CNF=00 MODE=00 */
    GPIOC->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);
    GPIOC->CRL &= ~(GPIO_CRL_MODE2 | GPIO_CRL_CNF2);

    /* 2. ADC相关配置 */
    /* 2.1 启用扫描模式. */
    ADC1->CR1 |= ADC_CR1_SCAN;
    /* 2.2 启用连续转换模式 CR2=CONT 1*/
    ADC1->CR2 |= ADC_CR2_CONT;
    /* 2.3 数据对齐方式: 右对齐 左对齐 */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;
    /* 2.4 设置采样时间 ADC_SMPR1  010=13.5周期 第10通道采样*/
    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP10_0 | ADC_SMPR1_SMP10_2);
    ADC1->SMPR1 |= ADC_SMPR1_SMP10_1;

    ADC1->SMPR1 &= ~(ADC_SMPR1_SMP12_0 | ADC_SMPR1_SMP12_2);
    ADC1->SMPR1 |= ADC_SMPR1_SMP12_1;
    /* 2.6 通道组的配置 */
    /* 2.6.1 配置两个个通道需要转换 */
    ADC1->SQR1 &= ~ADC_SQR1_L;
    ADC1->SQR1 |= ADC_SQR1_L_0;
    /* 2.6.2 把通道号配置到组里面.  */
    ADC1->SQR3 &= ~ADC_SQR3_SQ1; /* 先把5位清零 */
    ADC1->SQR3 |= 10 << 0;       /* 设置最后5位 */

    ADC1->SQR3 &= ~ADC_SQR3_SQ2; /* 先把5位清零 */
    ADC1->SQR3 |= 12 << 5;
    /* 2.7 选择软件触发 */
    ADC1->CR2 &= ~ADC_CR2_EXTTRIG; /* 禁用规则组的外部转换 */
    ADC1->CR2 |= ADC_CR2_EXTSEL;   /* 选择使用软件触发ADC */
}

/**
 * @brief 多通道的时候一定要配置DMA
 * 查看文档，通道1对应ADC1
 * @return double 
 */
void Dri_ADC1_DMA_Init(void)
{
    /* 1. 开启 DAM 时钟 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    /* 2. DMA相关的配置 */
    /* 2.1 DMA方向: 从存储器(ROM)到存储器(RAM) . 通道随意*/
    DMA1_Channel1->CCR &= ~DMA_CCR1_DIR; // 0: 从外设读 1:从存储器读
    /* 2.2 设置存储器和外设的数据宽度:  8位(一个字节) 16位(半字) 32位(字) */
    DMA1_Channel1->CCR &= ~DMA_CCR1_PSIZE_1; /* 00:8位 01:16位 10:32位 11:保留 */
    DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0;
    
    DMA1_Channel1->CCR &= ~DMA_CCR1_MSIZE_1;
    DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0;
    /* 2.3 设置外设和存储器的地址是否自增 */
    DMA1_Channel1->CCR &= ~DMA_CCR1_PINC; /* 外设地址自增 */
    DMA1_Channel1->CCR |= DMA_CCR1_MINC; /* 存储器地址自增 */
    /* 2.4 开启循环模式 */
    DMA1_Channel1->CCR |=DMA_CCR1_CIRC;

    /* 2.5 给ADC1开启DMA模式 */
    ADC1->CR2|=ADC_CR2_DMA;
}

void Driver_ADC1_DMA_Start(uint32_t desAddr, uint8_t dataLen)
{
    /* 1. 设置外设地址 */
    DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR);
    /* 2. 设置存储器地址 */
    DMA1_Channel1->CMAR = desAddr;

    /* 3. 设置要传输的数据量 */
    DMA1_Channel1->CNDTR = dataLen;
    /* 4. 开启通道,开始传输 */
    DMA1_Channel1->CCR |= DMA_CCR1_EN;

    Dri_ADC1_StartConvert();
}
