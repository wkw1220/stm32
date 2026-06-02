#include "Driver_ADC.h"

/**
 * @brief 初始化 单通道
 * 查手册 PC0对应的是10通道
 *
 */
void Dri_ADC1_Init(void)
{
    /* 1. 时钟配置 */
    /* 1.1 adc时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    //分配要加上 6分频 12M ADC输入时钟不得超过14MHz
    RCC->CFGR |= RCC_CFGR_ADCPRE_1;
    RCC->CFGR &= ~RCC_CFGR_ADCPRE_0;
    /* 1.2 GPIO的时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    /* 2. GPIO工作模式: PC0 模拟输入  CNF=00 MODE=00 */
    GPIOC->CRL &= ~(GPIO_CRL_MODE0 | GPIO_CRL_CNF0);

    /* 2. ADC相关配置 */
    /* 2.1 禁用扫描模式. 只有一个通道不用扫描 */
    ADC1->CR1 &= ~ADC_CR1_SCAN;
    /* 2.2 启用连续转换模式 CR2=CONT 1*/
    ADC1->CR2 |= ADC_CR2_CONT;
    /* 2.3 数据对齐方式: 右对齐:0 左对齐:1 */
    ADC1->CR2 &= ~ADC_CR2_ALIGN;
    /** 2.4 设置采样时间 ADC_SMPR1  010=13.5周期 
     *  第10通道采样 PC0是对应的10通道
     *  采样需要13.5个周期，后面还要加12.5周期 也就是26个周期才能由采样到转换完成
     *  一个周期是1/12us，相当于26/12us 完成转换
     * */
    ADC1->SMPR1 |= (ADC_SMPR1_SMP16_0 | ADC_SMPR1_SMP10_1);
    ADC1->SMPR1 &= ~ADC_SMPR1_SMP10_2;
    /* 2.6 通道组的配置 */
    /* 2.6.1 配置几个通道需要转换 一个需要转换*/
    ADC1->SQR1 &= ~ADC_SQR1_L;
    /**  
     * 2.6.2 把通道号配置到组里面. 
     * 观察SQR1，SQR2，SQR3
     * 首先确定需要转换几个通道，有一个通道就放在装换通道1里，有两个就把第二个放在
     * 转换通道2里，三个就放在第三个通道里 依次类推
     */
    ADC1->SQR3 &= ~ADC_SQR3_SQ1; /* 先把5位清零 */
    ADC1->SQR3 |= 0x10 << 0;       /* 设置最后5位 用的是10通道所以配置值为10 */
    /* 2.7 选择软件触发 */
    ADC1->CR2 &= ~ADC_CR2_EXTTRIG; /* 禁用规则组的外部转换 */
    ADC1->CR2 |= ADC_CR2_EXTSEL;   /* 选择使用软件触发ADC */
}

void Dri_ADC1_StartConvert(void)
{
    /* 1. 上电: 把ADC从休眠模式唤醒:由0到1 配置以后就变为1了 */
    ADC1->CR2 |= ADC_CR2_TSVREFE;
    ADC1->CR2 |= ADC_CR2_ADON;
    /* 2. 执行校准 ADC_CR2_CAL校准完成以后这位会变为0*/
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL);

    /* 3. ADON = 1(此时已经是1了), 前面已配置开始转换 0>1:从休眠模式唤醒   这步骤再设置1->1:开始 */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* 4. 使用软件开始转换规则通道 */
    ADC1->CR2 |= ADC_CR2_SWSTART;//用的是规则通道 而不是注入通道模式

    /* 5. 等待首次转换完成 */
    while ((ADC1->SR & ADC_SR_EOC) == 0);
}

// 读取ADC1_IN16通道原始值（CPU温度传感器）
uint16_t ADC1_Read_Temp_Raw(void) {
    ADC1->CR2 |= ADC_CR2_SWSTART;  // 软件触发ADC转换
    while (!(ADC1->SR & ADC_SR_EOC)); // 等待转换完成（EOC位=1）
    return ADC1->DR;               // 返回ADC原始值（12位，0~4095）
}
float CPU_Temp = 0.0f;
// 计算CPU温度（从ADC原始值换算为℃）
double Dri_ADC1_ReadTemprature(void) {
    uint16_t adc_raw = ADC1_Read_Temp_Raw(); // 读取IN16通道ADC值
    float v_sense;                           // 温度传感器电压（V）

    // 1. 换算传感器电压：ADC原始值 → 实际电压（VREF+=3.3V）
    v_sense = (float)adc_raw * 3.3f / 4095.0f;

    // 2. 温度公式计算（STM32F1手册标准公式）
    const float V25 = 1.43f;          // 25℃时参考电压（典型值）
    const float Avg_Slope = 0.0043f;  // 温度系数（4.3mV/℃，典型值）
    CPU_Temp = ((V25-v_sense) / Avg_Slope) + 25.0f;
    return CPU_Temp;
}







