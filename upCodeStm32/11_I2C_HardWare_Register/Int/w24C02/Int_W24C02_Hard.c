#include "Int_W24C02_Hard.h"

void Inf_W24C02_Init(void)
{
    Dri_I2C2_Init_Hard();
}

void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    //1.开始
    Dri_I2C2_Start_Hard();
    //2.发送写地址(设备地址)
    Driver_I2C_SendAddr(ADDR);
    //3.发送内部地址
    Dri_I2C2_SendByte_Hard(innerAddr);
    //4.发送具体数据
    Dri_I2C2_SendByte_Hard(byte);
    //5.停止信号
    Dri_I2C2_Stop_Hard();
    
   Delay_ms(5); 
}

uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr)
{
    //1.开始
    Dri_I2C2_Start_Hard();
    //2.发送写地址(设备地址)
    Driver_I2C_SendAddr(ADDR);
   
      //4.发送内部地址
      Dri_I2C2_SendByte_Hard(innerAddr);
      //6.开始
      Dri_I2C2_Start_Hard();
      //7.发送写地址(设备地址)
      Driver_I2C_SendAddr(ADDR+1);
      //10.给对方一个非应答
      Dri_I2C2_NoAck_Hard();
      //10.停止信号
      Dri_I2C2_Stop_Hard();
      //9.读取一个字节
      uint8_t byte=Dri_I2C2_ReceiveByte_Hard();
      return byte;
    
}

void Inf_W24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
  //1.开始
  Dri_I2C2_Start_Hard();
  //2.发送写地址(设备地址)
  Driver_I2C_SendAddr(ADDR);
  //4.发送内部地址
   Dri_I2C2_SendByte_Hard(innerAddr);
  for(uint8_t i=0;i<len;i++){
    //发送具体数据
    //Dri_I2C2_SendByte_Hard(*(bytes+i));
    Dri_I2C2_SendByte_Hard(bytes[i]);
  }
   //10.停止信号
   Dri_I2C2_Stop_Hard();
   Delay_ms(5);
}

void Inf_W24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
  //1.开始
  Dri_I2C2_Start_Hard();
  //2.发送写地址(设备地址)
  Driver_I2C_SendAddr(ADDR);
  //4.发送内部地址
  Dri_I2C2_SendByte_Hard(innerAddr);
  //6.开始
  Dri_I2C2_Start_Hard();
  //7.发送读地址(设备地址)
  Driver_I2C_SendAddr(ADDR+1);
  for(uint8_t i=0;i<len;i++){
   
    //等待响应
    if(i<len-1){
      Dri_I2C2_Ack_Hard();
    }else{
      Dri_I2C2_NoAck_Hard();
      Dri_I2C2_Stop_Hard();
    }
       
   //发送具体数据
   //*(bytes+i)=Dri_I2C2_ReceiveByte_Hard();
   bytes[i]=Dri_I2C2_ReceiveByte_Hard();
  }
   
}
