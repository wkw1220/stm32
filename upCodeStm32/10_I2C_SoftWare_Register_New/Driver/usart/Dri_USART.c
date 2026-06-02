#include "Dri_USART.h"
#include "string.h"


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

   /** 3.6 使能串口的各种中断 查看手册25.4的资料 */
   //接收缓冲区非空中断使能，检测到接收缓冲去不为空，我们就可以从缓冲区里读数据，当USART_SR中的ORE或者RXNE为’1’时，产生USART中断。
   USART1->CR1 |= USART_CR1_RXNEIE;
   //检测到空闲线路会产生中断
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
void Dri_USART_SendByte(uint8_t byteChar)
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
        Dri_USART_SendByte(str[i]);
    }
    
}

uint8_t Dri_USART_ReceiveChar(void)
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


/**
 * @brief 接收缓冲区非空中断使能 RXNEIE
 * 如果接收缓冲区不为空，会产生这个中断，也就是会执行下面的中断函数
 * 如果IDLE：监测到总线空闲 (IDLE line detected)，也会产生中断
 * 所以下面中断处理函数会对这个两个中断分别处理
 * 所以处理完成以后 有两个中断，一定要把中断位清除(RXNE 和)
 * 
 */
u8 buff[100]={'0'};
u8 len=0;
u8 isReadySend=0;
void USART1_IRQHandler(void)
{

    //RXNE:读数据寄存器非空 (Read data register not empty)产生的中断的处理逻辑
    if(USART1->SR&USART_SR_RXNE) 
    {
      /**
        * @brief 读过以后要对中断位清零，通过读手册
        * 对USART_DR的读操作可以将该位清零。RXNE位也可以通过写入0来清除
        * 所以清除中断位可以省略
        */
       //USART1->SR &= ~USART_SR_RXNE;

       //以下是处理字节的业务逻辑
       buff[len]=USART1->DR;
       len++;

    }

    //监测到总线空闲 产生的中断处理逻辑
    if(USART1->SR&USART_SR_IDLE)
     {
       //Dri_USART_SendByte('r');//发现空闲发送一个字符，用于测试

       /**
        * @brief 清除空闲中断标志位
        * 先读USART_SR，然后读USART_DR
        * 
       */
       USART1->SR;
       USART1->DR;

       /**
        * @brief 下面代码就可以处理自己的业务逻辑
        * 
        */
       //下面两行代码非常耗时，建议你不要写在中断里面，用一个状态位表示
       //Dri_USART_SendStr(buff);
       //memset(buff, 0, sizeof(buff));
       isReadySend=1;
    }
}

int fputc(int c, FILE *file)
{
  Dri_USART_SendByte(c);
  return c;
}


