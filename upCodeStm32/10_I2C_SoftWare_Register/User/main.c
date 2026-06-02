#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include "Int_W24C02.h"
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
 * 
 *    上拉电阻的阻值设定问题(本芯片中上拉电阻阻值是10千欧姆)
 *     电平是弱上拉强下拉。总线电压是VCC,中间和从设备之间有一个上拉电阻
 *     强下拉:从设备接低和总线上上拉电阻连接，这段线路近似于导线，所以上拉电阻的的下部
 *           立马被拉低为低电平。
 *     弱上拉:从设备和主线断开，根据硬件的设计，板子上相当于电容，就相当于VCC对电容充电
 *            充电需要时间，所以上拉的时间会长
 *     上拉电阻越小，电流就越大，上拉的就越快，翻转的速度也就越快，这样通讯速率也就越快
 *     但是通讯速率越快会导致功率加大，发热量也就越大。
 *     速率和上拉电阻的通用匹配关系
 *     100K->10K欧姆
 *     400K->4.7K欧姆
 *     3.xM->500到1000欧姆
 * 
 * SDA:Serial Data SCL:Serial Clock
 */

int main(){
	Dri_USART_Init();
	Inf_W24C02_Init();
	printf("Hello success!\r\n");

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




