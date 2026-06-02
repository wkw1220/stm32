#include "Inf_W5500_Ping.h"
uint8_t macAddress[]={110,120,130,140,150,160};
uint8_t ip[4] = {192, 168, 1, 99};  // ip地址(前三段和电脑保持一致)
uint8_t subMask[4] = {255, 255, 255, 0};
uint8_t gateWay[4] = {192, 168, 1, 1}; // 默认网关(和电脑保持一致)

void Inf_W5500_Restart()
{
   RST_LOW;
   Delay_ms(1000);//重启引脚拉低至少500ms可以使芯片重启
   RST_HIGH;
   Delay_ms(500);
}

void Inf_W5500_ConfigInfo()
{
   printf("start set IP \r\n");
   setSIPR(ip);
   printf("completed set IP \r\n");
   printf("start set sub \r\n");
   setSUBR(subMask);
   printf("completed set sub \r\n");
   printf("start set wg \r\n");
   setGAR(gateWay);
   printf("completed set wg \r\n");
   /*4.设置物理地址（MAC地址）*/
   printf("start set mac address \r\n");
   setSHAR(macAddress);
   printf("completed set mac address \r\n");
   printf("W5500 network config completed \r\n");
   printf("ip地址: %d.%d.%d.%d \r\n", ip[0], ip[1], ip[2], ip[3]);
}

void Inf_W5500_Init(void)
{
    /* 1.给w5500注册底层函数 */
    user_register_function();

    /* 2.初始化SPI */
    Driver_SPI_Init();
    
    /**初始化复位和中断引脚并使能 主要给后面软件重启提供相关功能 */
    Dri_GPIO_ResetAndInt_Init();

    /* 3.软件重启w5500芯片 */
    Inf_W5500_Restart();
    
    /* 4.设置IP地址,掩码,网关,MAC地址相关信息 */
    Inf_W5500_ConfigInfo();
}
