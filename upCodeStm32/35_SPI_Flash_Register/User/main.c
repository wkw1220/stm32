#include "Dri_USART.h"
#include <stdio.h>
#include "Delay.h"
#include "Int_W25Q32.h"
uint8_t mid; 
uint16_t did;
//uint8_t data[255]={'s','2','3','t','u','z'};
uint8_t data[255]="你好啊1234";
uint8_t buffer[255]={0};
int main()
{
    Dri_USART_Init();
    printf("SPI_FLash_Soft.......\r\n");
    Inf_W25Q32_Init();
    Inf_W25Q32_ReadId(&mid,&did);
    printf("mid=0x%X,did=0x%X\r\n",mid,did);

    Inf_W25Q32_EraseSector(0,0);
    Inf_W25Q32_WritePage(0,0,0,data,10);//写之前把擦除功能嵌入进去
    Inf_W25Q32_Read(0,0,0,buffer,10);
    printf("buff=%s\r\n",buffer);
    memset(buffer,0,255);
    while (1)
    {
      
    }
}
