/**
 * @file Int_W24C02.c
 * @author windear
 * @brief 
 * @version 0.1
 * @date 2025-07-29
 * 
 * @copyright Copyright (c) 2025
 * W24C02:是一款EE2PROM芯片型号 本案例是MCU芯片通过I2C通信的方式对芯片进行存储和读写数据
 *        C02芯片:是2KB，一共256个字节，一页是16个字节，也就是256/16=16页
 *        只有主机主动发送请求，从机是不会发送请求的
 */

#include "Int_W24C02.h"
#include "Driver_I2C2_Soft.h"

/**
 * @brief 存储芯片初始化
 * 
 */
void Inf_W24C02_Init(void)
{
    Dri_I2C2_Init_Soft();
}

/**
 * @brief cpu向存储器innerAddr这个地址写入一个字节
 *        结果是这个字节会存储在存储器的这个innerAddr地址
 * 
 * @param innerAddr 存储器的内部地址
 * @param byte 要向存储器存放的内容 按字节存储
 */
void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    //1.开始
    Dri_I2C2_Start_Soft();
    //2.发送写地址(设备地址)
    Dri_I2C2_SendByte_Soft(ADDR);
    //3.等待响应
    uint8_t ack=Dri_I2C2_WaitAck_Soft();
    if(ack==ACK){
      //4.发送内部地址
      Dri_I2C2_SendByte_Soft(innerAddr);
      //5.等待响应
      Dri_I2C2_WaitAck_Soft();
      //6.发送具体数据
      Dri_I2C2_SendByte_Soft(byte);
      //7.等待响应
      Dri_I2C2_WaitAck_Soft();
      //8.停止信号
      Dri_I2C2_Stop_Soft();
    }
   Delay_ms(5); 
}

/**
 * @brief CPU从存储器按字节读取内容
 * 
 * @param innerAddr:存储器的内部地址
 * @return uint8_t:读取的内容
 */
uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr)
{
    //1.开始
    Dri_I2C2_Start_Soft();
    //2.发送写地址(设备地址)
    Dri_I2C2_SendByte_Soft(ADDR);
    //3.等待响应
    uint8_t ack=Dri_I2C2_WaitAck_Soft();
    if(ack==ACK){
      //4.发送内部地址
      Dri_I2C2_SendByte_Soft(innerAddr);
      //5.等待响应
      Dri_I2C2_WaitAck_Soft();
      //6.开始
      Dri_I2C2_Start_Soft();
      //7.发送写地址(设备地址)
      Dri_I2C2_SendByte_Soft(ADDR+1);
      //8.等待响应
      Dri_I2C2_WaitAck_Soft();
      //9.读取一个字节
      uint8_t byte=Dri_I2C2_ReceiveByte_Soft();
      //10.给对方一个非应答
      Dri_I2C2_NoAck_Soft();
      //10.停止信号
      Dri_I2C2_Stop_Soft();
      return byte;
    }
    return 0;
}

/**
 * @brief 页写入:CPU向存储器写入多个字节
 * 
 * @param innerAddr:存储器内部地址 
 * @param bytes:存储内容，多个字节 
 * @param len:存储内容的字节长度 对于这个芯片如果len=16正好是一页
 * 00-0f
 * 10-1f
 * 20-2f
 * 如果写入的长度大于16，假设是17，此芯片不会自动换页，第17个字符会覆盖第一个字符 
 */
void Inf_W24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
  //1.开始
  Dri_I2C2_Start_Soft();
  //2.发送写地址(设备地址)
  Dri_I2C2_SendByte_Soft(ADDR);
  //3.等待响应
  Dri_I2C2_WaitAck_Soft();
  //4.发送内部地址
  Dri_I2C2_SendByte_Soft(innerAddr);
   //等待响应
  uint16_t ack=Dri_I2C2_WaitAck_Soft();
  if(ack==ACK){
   for(uint8_t i=0;i<len;i++){
      //发送具体数据
      Dri_I2C2_SendByte_Soft(*(bytes+i));
      //Dri_I2C2_SendByte_Soft(bytes[i]);
      //等待响应
      Dri_I2C2_WaitAck_Soft();
    }
   //10.停止信号
   Dri_I2C2_Stop_Soft();
  }
   Delay_ms(5);
}

/**
 * @brief 页读取:CPU向存储器读取多个字节
 * 
 * @param innerAddr:存储器内部地址 
 * @param bytes:读取到的存储内容保存的地址，多个字节 
 * @param len:读取内容的字节长度 对于这个芯片如果len=16正好是一页
 *     读的时候没有页的限制，但是写的时候有页的限制
 */
void Inf_W24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
  //1.开始
  Dri_I2C2_Start_Soft();
  //2.发送写地址(设备地址)
  Dri_I2C2_SendByte_Soft(ADDR);
  //3.等待响应
  Dri_I2C2_WaitAck_Soft();
  //4.发送内部地址
  Dri_I2C2_SendByte_Soft(innerAddr);
  //5.等待响应
  Dri_I2C2_WaitAck_Soft();
  //6.开始
  Dri_I2C2_Start_Soft();
  //7.发送写地址(设备地址)
  Dri_I2C2_SendByte_Soft(ADDR+1);
   //8.等待响应
  Dri_I2C2_WaitAck_Soft();
  for(uint8_t i=0;i<len;i++){
   
    //发送具体数据
    *(bytes+i)=Dri_I2C2_ReceiveByte_Soft();
    i!=len-1?Dri_I2C2_Ack_Soft():Dri_I2C2_NoAck_Soft();

    //bytes[i]=Dri_I2C2_ReceiveByte_Soft();
    //等待响应
    /* if(i<len-1){
      Dri_I2C2_Ack_Soft();
    }else{
      Dri_I2C2_NoAck_Soft();
    }
    */    
   
  }
   //10.停止信号
   Dri_I2C2_Stop_Soft();
}
