#ifndef __INTFACE_W5500_H__
#define __INTFACE_W5500_H_
 
#include "w5500.h"
#include "Driver_SPI.h"
#include "stdio.h"
#include "Driver_GPIO.h"


void Inf_W5500_Init(void);
void Inf_W5500_Restart(void);
#endif

/**
 * @brief 配置项目要注意的地方
 * 
 * 1->在wizchip_conf.h中定义芯片型号为W5500 第75行
 *  #define _WIZCHIP_  W5500
 * 2->在wizchip_conf.h中定义芯片的接口模式为SPI VDM模式 第154行
 *   #define _WIZCHIP_IO_MODE_           _WIZCHIP_IO_MODE_SPI_VDM_
 * 3->路径的修改
 *   #include "w5500_base/w5500.h"  第157行
 * 4->在wizchip_conf.c中实现SPI的底层读写函数，让w5500和自己定义的SPI驱动相关联
 *   四个函数需要自己实现，要引入头文件Driver_SPI.h
 *   wizchip_spi_readbyte   
 *   wizchip_spi_writebyte
 *   wizchip_cs_select
 *   wizchip_cs_deselect
 * 5->要注册这些底层函数，在wizchip_conf.c中user_register_function()函数中注册
 *    函数写在wizchip_conf.c中的最后面，并且要在wizchip_conf.h中声明函数原型
 *   
 */