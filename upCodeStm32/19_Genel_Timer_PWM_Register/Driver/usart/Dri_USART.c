#include "Dri_USART.h"
#include "string.h"
#include <stdio.h>


void Init_InternelFun(){
    /* 1 .开启时钟 */
    /* 1.1 串口1外设时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    /* 1.2 GPIO时钟 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    /* 2.配置GPIO引脚的工作模式 PA9=TX(复用推挽)10 11 PA10=RX(浮空输入) 0100  */
    GPIOA->CRH |= GPIO_CRH_MODE9;
    GPIOA->CRH |= GPIO_CRH_CNF9_1;
    GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
    GPIOA->CRH &= ~GPIO_CRH_MODE10;
    GPIOA->CRH |= GPIO_CRH_CNF10_0;
    GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
    /* 3.串口的参数配置 */
    /* 3.1 配置波特率 115200 */
    USART1->BRR=0x271;
    /* 3.2 配置一个字的长度 8位 */
    USART1->CR1 &= ~USART_CR1_M;
    /* 3.3 配置不需要校验位 */
    USART1->CR1 &= ~USART_CR1_PCE;
    /* 3.4 配置停止位的长度 */
    USART1->CR2 &= ~USART_CR2_STOP;
    /* 3.5 使能接收和发送 */
     USART1->CR1 |= USART_CR1_RE;
     USART1->CR1 |= USART_CR1_TE;
}
/**
 * @brief USART1初始化
 * 
 */
void Dri_USART_Init(void)
{
    Init_InternelFun();
    /* 4.使能串口 */
    USART1->CR1 |= USART_CR1_UE;
}

void Dri_USART_Init_WithInterruput(void){
   Init_InternelFun();
   USART1->CR1 |= USART_CR1_RXNEIE;
   USART1->CR1 |= USART_CR1_IDLEIE;
   NVIC_SetPriorityGrouping(3);
   NVIC_SetPriority(USART1_IRQn, 3);
   NVIC_EnableIRQ(USART1_IRQn); 
   /* 4.使能串口 */
   USART1->CR1 |= USART_CR1_UE;
}



/**
 * @brief 发送一个字节数据
 * 
 * @param byteChar 
 */
void Dri_USART_SendChar(uint8_t byteChar)
{

    /** 
     * USART_SR_TXE 
     * =0 说明数据还没有转移到寄存器
     * =1 说明数据已经转移到寄存器
    */

    //if((USART1->SR&USART_SR_TXE)!=0) 
    while((USART1->SR&USART_SR_TXE)==0);
    USART1->DR=byteChar;
     
}

/**
 * @brief 发送字符串
 * 
 * @param str 
 */
void Dri_USART_SendStr(u8 *str)
{   
    uint8_t i;
    for (i = 0; i < strlen(str); i++)
    {
        Dri_USART_SendChar(str[i]);
    }
    
}

uint8_t Dri_USART_ReceiveChar()
{
    while((USART1->SR&USART_SR_RXNE)==0);
    return USART1->DR;
}

void Dri_USART_ReceiveStr(u8* buff)
{ 
   u8 i=0;
   while(1){
    //判断寄存器中是否有数据，有数据就读取
     while((USART1->SR&USART_SR_RXNE)!=0) //0 没有收到数据 1 收到数据 可以读取
     { 
       buff[i]=USART1->DR;
       i++;
     }
     if(USART1->SR&USART_SR_IDLE){
       return;
     } 
    /* while((USART1->SR&USART_SR_RXNE)==0)
    {
      if(USART1->SR&USART_SR_IDLE)
      {
         return;
      }  
    }
    buff[i]=USART1->DR;
    i++; */
   }
}

u8 buff[100]={'0'};
u8 len=0;
u8 isReadySend=0;
void USART1_IRQHandler(void)
{
    if(USART1->SR&USART_SR_RXNE) //。对USART_DR的读操作可以将该位清零。RXNE位也可以通过写入0来清除
    {
       buff[len]=USART1->DR;
       len++;
    }
     if(USART1->SR&USART_SR_IDLE)
     {
       //先读USART_SR，然后读USART_DR)
       USART1->SR;
       USART1->DR;
       //下面两行代码非常耗时，建议你要写在中断里面
       //Dri_USART_SendStr(buff);
       //memset(buff, 0, sizeof(buff));
       isReadySend=1;
       //Dri_USART_SendChar('r');
     }
}

int fputc(int c,FILE *file){
  Dri_USART_SendChar(c);
  return c;
}


