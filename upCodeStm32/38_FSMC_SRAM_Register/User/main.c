#include "Dri_USART.h"
#include <stdio.h>
#include "Delay.h"
#include "Driver_FSMC.h"
/**
 * @brief 存储数据的时候不会自动把数据扩展到外部SRAM，需要特别申明才可以
 * 
 * @return int 
 */

/**定义一个变量存储到外部SRAM */
//方式一:全部变量 变量v1的值为10，存储到外部SRAM地址是0x68000000的位置 
uint8_t v1 __attribute__((at(0x68000000)));//定义变量时如果还没有使能不能初始化
const u8 x=20;//全局常量会放到Flash中 局部常量不会放到FLash中，而是放到sram中
int main()
{
    Dri_USART_Init();
    printf("FSMC.......\r\n");
    Driver_FSMC_Init();
    uint8_t v1 __attribute__((at(0x68000000)))=102;//已经使能可以初始化
    u8 v2=20;
    printf("v1Address=%p v1=%d\r\n",&v1,v1);
    printf("v2Address=%p v2=%d\r\n",&v2,v2);
    //方式二:定义指针
    uint32_t* address= 0x68000004;
    *address=30;
    printf("address=%p v1=%d\r\n",address,*address);
    
    
    printf("xAddress=%p x=%d\r\n",&x,x);

    const u8 y=20;//局部常量不会放到FLash中，而是放到sram中
    printf("yAddress=%p y=%d\r\n",&y,y);
    //memset(buffer,0,255);
    while (1)
    {
      
    }
}
