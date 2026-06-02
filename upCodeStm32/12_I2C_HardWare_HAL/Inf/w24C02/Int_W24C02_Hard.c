#include "Int_W24C02_Hard.h"

void Inf_W24C02_Init(void)
{
   MX_I2C2_Init();
}

void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
   HAL_I2C_Mem_Write(&hi2c2,ADDR,innerAddr,I2C_MEMADD_SIZE_8BIT,&byte,1,2000);
   HAL_Delay(5);
}

uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr)
{
  uint8_t byteChar;
  HAL_I2C_Mem_Read(&hi2c2,ADDR+1,innerAddr,I2C_MEMADD_SIZE_8BIT,&byteChar,1,2000);
  return byteChar;
    
}

void Inf_W24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
  HAL_I2C_Mem_Write(&hi2c2,ADDR,innerAddr,I2C_MEMADD_SIZE_8BIT,bytes,len,2000);
  /**
   * @brief 延时一定要加
   * 
   */
  HAL_Delay(5);
}

void Inf_W24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
  HAL_I2C_Mem_Read(&hi2c2,ADDR+1,innerAddr,I2C_MEMADD_SIZE_8BIT,bytes,len,2000);
}
