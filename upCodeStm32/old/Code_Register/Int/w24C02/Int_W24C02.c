#include "Int_W24C02.h"
#include "Driver_I2C2_Soft.h"

void Inf_W24C02_Init(void)
{
    Dri_I2C2_Init_Soft();
}

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
  Dri_I2C2_WaitAck_Soft();
  for(uint8_t i=0;i<len;i++){
    //发送具体数据
    Dri_I2C2_SendByte_Soft(*(bytes+i));
    //Dri_I2C2_SendByte_Soft(bytes[i]);
    //等待响应
     Dri_I2C2_WaitAck_Soft();
  }
   //10.停止信号
   Dri_I2C2_Stop_Soft();
   Delay_ms(5);
}

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
    //bytes[i]=Dri_I2C2_ReceiveByte_Soft();
    //等待响应
    /* if(i<len-1){
      Dri_I2C2_Ack_Soft();
    }else{
      Dri_I2C2_NoAck_Soft();
    }
    */    
   i!=len-1?Dri_I2C2_Ack_Soft():Dri_I2C2_NoAck_Soft();
  }
   //10.停止信号
   Dri_I2C2_Stop_Soft();
}
