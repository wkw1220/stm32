#ifndef __DRIVER_DMA_H__
#define __DRIVER_DMA_H__
#include "stm32f10x.h" 
extern uint8_t isTransmitFinished;

/**
 * @brief 这个通道的作用是从ROM读取数据到RAM
 * 
 */
void Dri_DMA1_Channel1_Init(void);

/**
 * @brief 这个通道主要用于USART1_TX
 * 
 */
void Dri_DMA1_Channel4_Init(void);

/**
 * @brief 这个通道主要用于USART1_RX
 * 
 */
void Dri_DMA1_Channel5_Init(void);

void Dri_DMA1_TransimitDataFromRomToRam(uint32_t srcAddr, uint32_t desAddr, uint16_t dataLen);

void Dri_DMA1_TransimitDataFromMemToPerpherial(uint32_t memoryAddr, uint32_t perpherialAddr, uint16_t dataLen);

void Dri_DMA1_TransimitDataFromPerpherialToMem(uint32_t perpherialAddr, uint32_t memoryAddr, uint16_t dataLen);

void Dri_DMA1_TransimitDataFromPerpherialToRam(uint32_t perpherialAddr, uint32_t memoryAddr, uint16_t dataLen);
#endif 

/**
 * @brief 存储器映像说明
 * STM32是32位地址 寻址空间是4G,如果把这个4G分成8个块(block0---block7)，那么每个块就是512M(注意单位是兆)
 *  可以查看数据手册了解存储器映像地址对应的存储器
 *  我们写的程序会下载到存储器中，其中在block0中会留出512K(注意单位是K)的容量用于我们字节写的程序，这块内容
 *  同时存储程序中定义的const常量,这就是FLash
 * 
 *  其中另外划出64k的容量用作动态内存SRAM，运行的变量全部都放到这个存储区域
 * 
 * DMA：首先有两个，每个对应的读取的内容都不一样。到底是读取APB1，还是APH2下的，
 *      去参考手册参考图59，60
 *   DMA1有七个通道 DMA有5个通道,每个通道专门用来管理来自于一个或多个外设对存储器访问的请求。
 *     还有一个仲裁器来协调各个DMA请求的优先权。
 *   要注意的是DMA2只存在于大容量产品和互联型产品中。虽然每个通道可以接收多个外设的请求，但是同一时间只能接收一个，不能同时接收多个。
 *  C8T6芯片就只有DMA1
 * 
 */


/**
 * @brief DMA 配置逻辑
 * 1->确定传输方向(外设到存储器还是存储器到外设，还是存储器到存储器)，这样就可以根据手册
 *    确定哪个通道，存储器到存储器可以选择任意通道
 * 2->初始化通道
 * 3->设置地址
 * 如果对串口不要忘记对串口进行使能
 * 
 */