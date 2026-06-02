#ifndef __INTEFACE_W25Q32_H__
#define __INTEFACE_W25Q32_H__

#include "Driver_SPI.h"
void Inf_W25Q32_Init(void);
void Inf_W25Q32_ReadId(uint8_t *mid, uint16_t *did);

void Inf_W25Q32_WiteEnable(void);
void Inf_W25Q32_WiteDisable(void);
void Inf_W25Q32_EraseSector(uint8_t block, uint8_t sector);
void Inf_W25Q32_WritePage(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint16_t len);
void Inf_W25Q32_Read(uint8_t block, uint8_t sector, uint8_t page, uint8_t *data, uint16_t len);
#endif
/**
 * @brief 芯片W25Q32介绍
 * 芯片先擦除后写入 擦除以后所有位全部变为1
 * 如果不擦除就写入，只能把1写成0,不能把0写成1
 * 
 * 每一页有256字节，一共有16384页 总容量是16384*256/1024=4096K 4096k/1024=4M
 * 一共有16384页(page)，16384/16=1024个扇区(sector), 1024/16=64块(block)
 * 一共有64块(block),一块有16扇区(sector)，1个扇区有16页(page)，1页有256个字节(byte)
 * 不允许一页一页的擦除，要么擦除一页，要么擦除一块，要么整个擦除
 * 一页:256字节,一个扇区(sector):4KB,一块(block):64KB
 * 
 */
