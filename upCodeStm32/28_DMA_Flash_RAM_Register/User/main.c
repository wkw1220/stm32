#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_DMA.h"
#include "Delay.h"
#include "Int_W24C02.h"
const char *str = "xyt123";
uint8_t *str1 = "123456789";
uint8_t buffer[100] = {0};
uint8_t src[] = {'a', 'b', 'c', 'd'};
int main()
{
    
    Dri_USART_Init();
    printf("start.......\n");
    //........通道1代码start 数据从ROM(Flash硬盘) 到 RAM(内存) 传输完成以后把RAM的数据打印到控制台.......
    /**
    const uint8_t src[]={10,20,30,40};
    uint8_t dest[4]={0};
    Dri_USART_Init();
    Dri_DMA1_Channel1_Init();
    printf("DMA实验,从ROM到RAM.....\r\n");
    Dri_DMA1_TransimitDataFromRomToRam(&src[0],&dest[0],4);
    while(isTransmitFinished==0);
    printf("%d,%d,%d,%d\r\n",dest[0],dest[1],dest[2],dest[3]);**/
    //........通道1代码end.......

    //........通道4代码start 把ROM或者RAM中的数据通过DMA传输到外设串口
    //const uint8_t src[]={'a','b','e','d'};//这些数据是存放在ROM中
    //uint8_t src[]={'a','b','c','d'};//这些数据存放在RAM中
    //Dri_DMA1_Channel4_Init();
    //需要到串口的地方去配置DMA 在Channel4 中代码已经加了
    //Dri_DMA1_TransimitDataFromMemToPerpherial(src,(uint32_t)(&(USART1->DR)),4);
    //........通道4代码end

    // 通道5  把串口1的数据通过DMA的方式读入到RAM 中
    //Dri_DMA1_Channel5_Init(); 
    //memset(buffer, 0, 100);
    //Delay_ms(1000);
    // 一定要清零重置
    //Dri_DMA1_TransimitDataFromPerpherialToMem(&(USART1->DR),buffer,6);
    /* while (1){
        if(isTransmitFinished){
            isTransmitFinished=0;
            printf("DMA: %s\r\n", buffer);
            memset(buffer, 0, strlen(buffer));
        }
    } */
    // 通道5  end.....................................................
    while(1);

}
