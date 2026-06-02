#include "Driver_Lcd.h"

void Dri_FSMC_GPIO_LCD_Init(void)
{
    /* 1 配置 A0-A18 地址端口的输出模式 复用推挽输出CNF:10 50MHz速度 MODE:11*/
    /* =============MODE=============== */
    GPIOG->CRL |= GPIO_CRL_MODE0;
    /* =============CNF=============== */
    GPIOG->CRL |= GPIO_CRL_CNF0_1 ;
    GPIOG->CRL &= ~GPIO_CRL_CNF0_0;
    /*
        2 数据端口 复用推挽输出
            在实际应用中，即使数据线被配置为输出模式，FSMC控制器仍然能够管理数据线的方向，使其在需要时成为输入线。
            这种自动切换是由FSMC控制器硬件管理的，不需要软件干预。
            因此，即使GPIO配置为复用推挽输出，FSMC依然可以实现读取操作。
    */
    /* =============MODE=============== */
    GPIOD->CRL |= (GPIO_CRL_MODE0 |
                   GPIO_CRL_MODE1);
    GPIOD->CRH |= (GPIO_CRH_MODE8 |
                   GPIO_CRH_MODE9 |
                   GPIO_CRH_MODE10 |
                   GPIO_CRH_MODE14 |
                   GPIO_CRH_MODE15);

    GPIOE->CRL |= (GPIO_CRL_MODE7);
    GPIOE->CRH |= (GPIO_CRH_MODE8 |
                   GPIO_CRH_MODE9 |
                   GPIO_CRH_MODE10 |
                   GPIO_CRH_MODE11 |
                   GPIO_CRH_MODE12 |
                   GPIO_CRH_MODE13 |
                   GPIO_CRH_MODE14 |
                   GPIO_CRH_MODE15);

    /* =============CNF=============== */
    GPIOD->CRL |= (GPIO_CRL_CNF0_1 |
                   GPIO_CRL_CNF1_1);
    GPIOD->CRL &= ~(GPIO_CRL_CNF0_0 |
                    GPIO_CRL_CNF1_0);

    GPIOD->CRH |= (GPIO_CRH_CNF8_1 |
                   GPIO_CRH_CNF9_1 |
                   GPIO_CRH_CNF10_1 |
                   GPIO_CRH_CNF14_1 |
                   GPIO_CRH_CNF15_1);
    GPIOD->CRH &= ~(GPIO_CRH_CNF8_0 |
                    GPIO_CRH_CNF9_0 |
                    GPIO_CRH_CNF10_0 |
                    GPIO_CRH_CNF14_0 |
                    GPIO_CRH_CNF15_0);

    GPIOE->CRL |= (GPIO_CRL_CNF7_1);
    GPIOE->CRL &= ~(GPIO_CRL_CNF7_0);

    GPIOE->CRH |= (GPIO_CRH_CNF8_1 |
                   GPIO_CRH_CNF9_1 |
                   GPIO_CRH_CNF10_1 |
                   GPIO_CRH_CNF11_1 |
                   GPIO_CRH_CNF12_1 |
                   GPIO_CRH_CNF13_1 |
                   GPIO_CRH_CNF14_1 |
                   GPIO_CRH_CNF15_1);
    GPIOE->CRH &= ~(GPIO_CRH_CNF8_0 |
                    GPIO_CRH_CNF9_0 |
                    GPIO_CRH_CNF10_0 |
                    GPIO_CRH_CNF11_0 |
                    GPIO_CRH_CNF12_0 |
                    GPIO_CRH_CNF13_0 |
                    GPIO_CRH_CNF14_0 |
                    GPIO_CRH_CNF15_0);


    /* 3 其他控制端口  复用推挽输出 */
    /* 3.1 PD4: 读控制引脚  PD5： 写控制引脚 */
    GPIOD->CRL |= (GPIO_CRL_MODE4 |
                   GPIO_CRL_MODE5);
    GPIOD->CRL |= (GPIO_CRL_CNF4_1 |
                   GPIO_CRL_CNF5_1);
    GPIOD->CRL &= ~(GPIO_CRL_CNF4_0 |
                    GPIO_CRL_CNF5_0);

    /* 3.2  PG12：NE4*/
    GPIOG->CRH |= (GPIO_CRH_MODE12);
    GPIOG->CRH |= (GPIO_CRH_CNF12_1);
    GPIOG->CRH &= ~(GPIO_CRH_CNF12_0);

    /* 3.3  背光引脚PB0：通用推挽输出*/
    GPIOB->CRL |= GPIO_CRL_MODE0;
    GPIOB->CRL &= ~GPIO_CRL_CNF0;

    /* 3.4  重置引脚PG15：通用推挽输出*/
    GPIOG->CRH |= GPIO_CRH_MODE15;
    GPIOG->CRH &= ~GPIO_CRH_CNF15;

}

void Dri_FSMC_LCD_Init(void)
{
    /* 1. 时钟开启 */
    RCC->APB2ENR |= (RCC_APB2ENR_IOPDEN |
                     RCC_APB2ENR_IOPBEN |
                     RCC_APB2ENR_IOPEEN |
                     RCC_APB2ENR_IOPFEN |
                     RCC_APB2ENR_IOPGEN |
                     RCC_APB2ENR_AFIOEN);

    RCC->AHBENR |= RCC_AHBENR_FSMCEN;
    /* 2. 各个引脚的模式的配置 */
    Dri_FSMC_GPIO_LCD_Init();
    /* 3. fsmc的配置 Bank1的 4区 BCR4 */
    /* 3.1 存储块使能 */
    FSMC_Bank1->BTCR[6] |= FSMC_BCR4_MBKEN;
    /* 3.2 设置存储类型 00=SRAM ROM*/
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_MTYP;
    /* 3.3 禁止闪存访问 */
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_FACCEN;
    /* 3.4 地址和数据复用: 不复用 */
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_MUXEN;
    /* 3.5 数据总线的宽度 16位宽度=01 */
    FSMC_Bank1->BTCR[6] &= ~FSMC_BCR4_MWID_1;
    FSMC_Bank1->BTCR[6] |= FSMC_BCR4_MWID_0;
    /* 3.6 写使能 */;
    FSMC_Bank1->BTCR[6] |= FSMC_BCR4_WREN;

    /* 3. fsmc的 时序 */
    /* 3.1 地址建立时间 对同步读写来说,永远一个周期 */
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR4_ADDSET;
    /* 3.2 地址保持时间 对同步读写来说,永远一个周期 */
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR4_ADDHLD;
    /* 3.3 数据保持时间 手册不能低于55ns 我们设置1us*/
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR4_DATAST;
    FSMC_Bank1->BTCR[7] |= (71 << 8);
    /* 3.4 设置时序模式 */
    FSMC_Bank1->BTCR[7] &= ~FSMC_BTR4_ACCMOD;
}
