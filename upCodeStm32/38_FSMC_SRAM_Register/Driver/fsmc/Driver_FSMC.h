#ifndef __DRIVER_FSMC_H__
#define __DRIVER_FSMC_H__

#include "stm32f10x.h"
void Driver_FSMC_Init(void);
#endif

/**
 * @brief E2PROM 和Flash
 * 这两种设备都是掉电以后不丢失
 * 主要区别E2PROM可以按字节为单位修改数据，但是FLash一般以多个字节为单位
 * 两种设备在写入数据之前都是先擦除然后再写入，E2PROM一般时自动完成，所以中途加了一个步骤所以慢
 * Flash是人为控制擦除的区域，再写入之前就做了擦除工作，相对E2PROM要快一些
 * 通讯一般用I2C或者SPI方式
 *
 * STM32芯片自带的有内存(RAM)和FLASH,通过数据总线控制
 *
 *
 * RAM:掉电数据会丢失
 * SRAM:静态存储器，MOS管锁存的方式，造价昂贵，
 * DRAM:动态存储器,电容充电的方式，缺点是会漏电(电容的特性)，需要一套电路不停的扫描
 *      读写速度要慢一些，因为要对电容充电
 *
 * 本案例中:
 * STM32:内存用的是SRAM(64K),Flash(512K),最大可以达到2M 没有DRAM
 *
 * MMAP：memory map
 * 一共是32位 4个G =4096M=1024M*4 内M部分成8块，以一块就相对相当于512M,两个512M就是1G
 *
 * 第一块 地址0x00000000-0x0007FFFF:Aliasd to Flash or system memory depending on Boot pins
 *   作用是启动时是把引脚引向Flash启功还是内存system memory(让后再跳到SRAM)启动
 *  Flash启动:Boot0拉低
 *  SRAM启动:Boot01拉低
 *  从bootLoader启动
 *
 * FSMC就是扩展RAM和FLASH的，只能扩展静态的内存，不能扩展动态的内存 但是FMC可以扩展动态的内存
 * 外面不是有存储芯片和FLash了吗？通讯方式都是串口通信(I2C或SPI),串口通信都很慢
 *
 * FSMC:都是并口对外通讯的，传输速度更快
 *
 * NOR Flash：可以随机访问
 * NAND FLash：只能固定的地址访问
 * 
 * fsmc把AHB总线上的数据转换位对应外设的通讯协议，控制外设的访问时序，以至于我们可以直接再程序中寻址访问
 */
