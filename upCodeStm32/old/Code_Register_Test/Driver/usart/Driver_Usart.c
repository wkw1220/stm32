#include "Driver_Usart.h"
#include <string.h>
#include <stdio.h>

void Intenel_Init(){
    /* 1 .开启时钟 */
    /* 1.1 串口1外设时钟 */
    RCC->APB2ENR |=RCC_APB2ENR_USART1EN;
    /* 1.2 GPIO时钟 */
    RCC->APB2ENR |=RCC_APB2ENR_IOPAEN;
    /* 2.配置GPIO引脚的工作模式 PA9=TX(复用推挽)10 11 PA10=RX(浮空输入) 0100  */
    GPIOA->CRH |=GPIO_CRH_MODE9;
    GPIOA->CRH |=GPIO_CRH_CNF9_1;
    GPIOA->CRH &=~GPIO_CRH_CNF9_0;

    GPIOA->CRH &=~GPIO_CRH_MODE10;
    GPIOA->CRH |=GPIO_CRH_CNF10_0;
    GPIOA->CRH &=~GPIO_CRH_CNF10_1;
    
   
    /* 3.串口的参数配置 */
    /* 3.1 配置波特率 115200 */
    USART1->BRR=0x271;
    /* 3.2 配置一个字的长度 8位 */
    USART1->CR1 &=~USART_CR1_M;
    /* 3.3 配置不需要校验位 */
    USART1->CR1 &=~USART_CR1_PCE;
    /* 3.4 配置停止位的长度 */
    USART1->CR2 &=~USART_CR2_STOP;
    /* 3.5 使能接收和发送 */
    USART1->CR1 |=USART_CR1_RE;
    USART1->CR1 |=USART_CR1_TE;
}

/**
 * @brief USART1初始化
 * UART1 RX:PA10 TX:PA9  可以使用逻辑分析仪器接这两个引脚
 * 
 */
void Dri_USART1_Init(void){
    Intenel_Init();
    /* 4.使能串口 */ 
    USART1->CR1 |=USART_CR1_UE;
}

/**
 * @brief 通过中断的当方式接收数据
 * 
 */
void Dri_USART1_Init_WithInterruput(void){
    Intenel_Init();

    /**
     * @brief 接收数据需要哪些条件？
     * 1.接收数据就绪可读 事件标志：RXNE  使能位：RXNEIE(中文文档有错)
     * 2.检测到空闲线路 事件标志：IDLE 使能位：IDLEIE  这个中断说明数据以及传输完成
     */
    USART1->CR1 |=USART_CR1_RXNEIE;
    USART1->CR1 |=USART_CR1_IDLEIE;

    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(USART1_IRQn,2);
    NVIC_EnableIRQ(USART1_IRQn);
    /* 4.使能串口 */ 
    USART1->CR1 |=USART_CR1_UE;
}

/**
 * @brief 发送一个字节数据
 * 
 * @param byteChar 
 */
void Dri_USART1_SendChar(uint8_t byteChar){
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
 * @brief 发送一个字符串
 * 
 * @param str 
 */
void Dri_USART1_SendStr(uint8_t *str){
   uint8_t i;
    for (i = 0; i < strlen(str); i++)
    {
        Dri_USART1_SendChar(str[i]);
    }
}

/**
 * @brief 这种是通过阻塞的方式来接收，使用这个函数其它程序只能等待，所有要用中断的方式来实现
 * 
 * @return uint8_t 
 */
uint8_t Dri_USART1_ReceiveChar(){
    while((USART1->SR&USART_SR_RXNE)==0);
    return USART1->DR;
}

/**
 * @brief 接收字符串 利用空闲帧实现
 * 
 * @return 
 */
void Dri_USART1_ReceiveStr(u8* buff){
   u8 i=0;
   while(1){
    //判断寄存器中是否有数据，有数据就读取
     while((USART1->SR&USART_SR_RXNE)!=0) //0 没有收到数据 1 收到数据 可以读取
     { 
       buff[i]=USART1->DR;
       i++;
     }
     //检测到空闲帧说明数据全部传完
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

/**
 * @brief 这些变量 再main 函数中使用extern 引用
 * extern  buff[100];
   extern  len;
   extern  isReadySend;
int main(){
	Dri_USART_Init_WithInterruput();
	while (1){
		if(isReadySend){
          Dri_USART_SendStr(buff);
          memset(buff, 0, sizeof(buff));
		  isReadySend=0;
		  len=0; 
		}
	}
  }
 * 
 */
uint8_t buff[100]={0};
uint8_t len=0;
uint8_t isReciveOver=0;

void USART1_IRQHandler(void){

  //设置了两个中断使能 1.当接收到数据时  2.检测到空闲数据线时

  //接收到数据时产生的中断
  if(USART1->SR&USART_SR_RXNE){
    buff[len]=USART1->DR;
    len++;
    //一定要清除中断位
    //USART1->SR &=~USART_SR_RXNE
  }else if(USART1->SR&USART_SR_IDLE){
    //非常耗时，不能再中断中操作
    //Dri_USART1_ReceiveStr(buff);
    //memset(buff, 0, sizeof(buff));
    isReciveOver=1;
    //清除中断标志位
    USART1->SR;
    USART1->DR;
  }
}


int fputc1(int c,FILE *file){
  Dri_USART1_SendChar(c);
  return c;
}
