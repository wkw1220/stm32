#include "Dri_USART.h"
#include "string.h"

void Init_InternelFun()
{
  /* 1 .开启时钟 */
  /* 1.1 串口2外设时钟 */
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
  /* 1.2 GPIO时钟 */
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  /* 2.配置GPIO引脚的工作模式 PA2=TX(输出复用推挽CNF=10 MODE=11) PA3=RX(浮空输入 CNF=01 MODE=00)  */
  GPIOA->CRL |= GPIO_CRL_MODE2;
  GPIOA->CRL |= GPIO_CRL_CNF2_1;
  GPIOA->CRL &= ~GPIO_CRL_CNF2_0;
  GPIOA->CRL &= ~GPIO_CRL_MODE3;
  GPIOA->CRL |= GPIO_CRL_CNF3_0;
  GPIOA->CRL &= ~GPIO_CRL_CNF3_1;
  /* 3.串口的参数配置 */
  /* 3.1 配置波特率 9600-->0xEA6  0x138*/
  USART2->BRR = 0xEA6;// 
  /* 3.2 配置一个字的长度 8位 */
  USART2->CR1 &= ~USART_CR1_M;
  /* 3.3 配置不需要校验位 */
  USART2->CR1 &= ~USART_CR1_PCE;
  /* 3.4 配置停止位的长度 1个停止位 */
  USART2->CR2 &= ~USART_CR2_STOP;
  /* 3.5 使能接收和发送 */
  USART2->CR1 |= USART_CR1_RE;
  USART2->CR1 |= USART_CR1_TE;
}
/**
 * @brief USART2初始化
 *
 */
void Dri_USART_Init(void)
{
  Init_InternelFun();
  /* 4.使能串口 */
  USART2->CR1 |= USART_CR1_UE;
}

/**
 * @brief 发送一个字节数据
 * 发送数据寄存器：不能判断是不是为0，有可能发送的数据就是0，应判断这个寄存器是否有值的标志位
 * 发送移位寄存器
 *
 * @param oneByte
 */
void Dri_USART_SendByte(uint8_t oneByte)
{

  /**
   * USART_SR_TXE
   * =0 说明数据还没有转移到寄存器
   * =1 说明数据已经转移到寄存器
   */

  /**
   * @brief 如果用if判断，存在逻辑问题
   * 调用发送，如果发现寄存器里的值非空，条件是flase，会导致if里的语句不能执行
   * 会导致数据漏发，这次数据就丢失了
   */
  //if((USART2->SR&USART_SR_TXE)!=0){
      //USART2->DR = oneByte;
  //}

  /**
   * @brief 一直等到发送寄存器为空
   * while (!(USART2->SR & USART_SR_TXE)); 和下面语句等同
   */
  while ((USART2->SR & USART_SR_TXE) == 0);
  USART2->DR = oneByte;
}

/**
 * @brief 发送字符串
 *
 * @param str
 */
void Dri_USART_SendStr(uint8_t *str)
{
  uint8_t i;
  for (i = 0; i < strlen(str); i++)
  {
    Dri_USART_SendByte(str[i]);
  }
}

/**
 * @brief 接收一个字节
 * 
 * @return uint8_t 
 * 如果在程序中使用了这个方法，会导致程序一直阻塞 
 */
uint8_t Dri_USART_ReceiveByte(void)
{
  printf("准备接收数...\r\n");
  while ((USART2->SR & USART_SR_RXNE) == 0);
  uint8_t data=USART2->DR;
  printf("接收数据已完成:%c\r\n",data);
  return data;
}

/**
 * @brief 用于接收一个字符串，由于对IDLE时间进行了判断，所以在主程序中调用看似没有阻塞
 * 但是本质上还是阻塞的
 * @param buff 
 */
void Dri_USART_ReceiveStr(u8 *buff)
{
  u8 i = 0;
  while (1)
  {
    // 判断寄存器中是否有数据，有数据就读取
    while ((USART2->SR & USART_SR_RXNE) != 0) // 0 没有收到数据 1 收到数据 可以读取
    {
      buff[i] = USART2->DR;
      i++;
    }
    if (USART2->SR & USART_SR_IDLE)
    {
      USART2->DR;
      return;
    }
  }
}

/**当调用printf的时候，会自动调用这个方法执行，只需要调用一个通过串口发送字符的函数 */
int fputc(int c, FILE *file)
{
  Dri_USART_SendByte(c);
  return c;
}
