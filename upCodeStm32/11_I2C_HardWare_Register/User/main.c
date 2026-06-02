#include "stm32f10x.h"                  
#include "Delay.h"
#include "Driver_USART.h"
#include "Int_W24C02_Hard.h"
#include <string.h>

/**
 * @brief I2C:不需要USART，CAN等通讯协议的外部收发设备，现在被广泛的使用在系统内多个集成电路(IC)
 *        之间的的通讯
 *        双向两线制 同步串行半双工 低速通讯
 *    只能主机和从机之间通信，不能从机和从机之间通信
 *  
 *    此项目功能描述：
 *    W24C02:是一款EE2PROM芯片型号 本案例是MCU芯片通过I2C通信的方式对芯片进行存储和读写数据
 *           硬件的驱动存放在inf文件夹中
 *    I2C:通讯方式，驱动存放在Driver文件夹中
 */

int main(){
	Dri_USART_Init();
	Inf_W24C02_Init();
	printf("Hello success HardWare I2C!\r\n");

    /**向存储器0位置存储a这个字符 */
	Inf_W24C02_WriteByte(0x00,'a');
	/**读取存储器0这个位置的字符 并打印在工作台 */
	u8 byteChar=Inf_W24C02_ReadByte(0x00);
	printf("%c\r\n",byteChar);
    Inf_W24C02_WriteBytes(0x00,"123456",6);
	u8 byteChar1=Inf_W24C02_ReadByte(0x03);
	u8 byteChar2=Inf_W24C02_ReadByte(0x05);
	printf("%c\r\n",byteChar1);
	printf("%c\r\n",byteChar2);
    u8 buff[255]={0};
	Inf_W24C02_ReadBytes(0x01,buff,3);
	printf("%s\r\n",buff);
	memset(buff,0,sizeof(buff));
	while (1){
		
	}
}




