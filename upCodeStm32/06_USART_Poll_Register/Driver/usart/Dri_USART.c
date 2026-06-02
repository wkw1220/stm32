#include "Dri_USART.h"


void Init_InternelFun()
{
  /* 1 .开启时钟 */
  /* 1.1 串口1外设时钟 */
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  /* 1.2 GPIO时钟 */
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  /* 2.配置GPIO引脚的工作模式 PA9=TX(输出复用推挽CNF=10 MODE=11) PA10=RX(浮空输入 CNF=01 MODE=00)  */
  GPIOA->CRH |= GPIO_CRH_MODE9;
  GPIOA->CRH |= GPIO_CRH_CNF9_1;
  GPIOA->CRH &= ~GPIO_CRH_CNF9_0;
  GPIOA->CRH &= ~GPIO_CRH_MODE10;
  GPIOA->CRH |= GPIO_CRH_CNF10_0;
  GPIOA->CRH &= ~GPIO_CRH_CNF10_1;
  /* 3.串口的参数配置 */
  
  /** 
   *  3.1 配置波特率 115200 
   *  直接对照参照表，整数在高12位，小数在低四位
   *  小数一个单位是0.0625,小数部分除以0.0625得到的整数放在低四位
   */
  USART1->BRR = 0x271;
  /* 3.2 配置一个字的长度 8位 */
  USART1->CR1 &= ~USART_CR1_M;
  /* 3.3 配置不需要校验位 */
  USART1->CR1 &= ~USART_CR1_PCE;
  /* 3.4 配置停止位的长度 1个停止位 */
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
   * USART_SR_TXE:发送数据寄存器为空标志位
   * =0 说明数据还没有转移到寄存器
   * =1 说明数据已经转移到寄存器
   */

  /**
   * @brief 如果用if判断，存在逻辑问题
   * 调用发送，如果发现寄存器里的值非空，条件是flase，会导致if里的语句不能执行
   * 会导致数据漏发，这次数据就丢失了
   */
  //if((USART1->SR&USART_SR_TXE)!=0){
      //USART1->DR = oneByte;
  //}

  /**
   * @brief 一直等到发送寄存器为空
   * while (!(USART1->SR & USART_SR_TXE)); 和下面语句等同
   */
  while ((USART1->SR & USART_SR_TXE) == 0);
  USART1->DR = oneByte;
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
 * 
 * RXNE：读数据寄存器非空 (Read data register not empty)
 *   =0 说明没有收到数据
 *   =1 说明收到数据，可以读取
 */
uint8_t Dri_USART_ReceiveByte(void)
{
  while ((USART1->SR & USART_SR_RXNE) == 0);
  return USART1->DR;
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
    while ((USART1->SR & USART_SR_RXNE) != 0) //0 没有收到数据 1 收到数据 可以读取
    {
      buff[i] = USART1->DR;//读取操作可以将RCNE位清零,说明没有数据
      i++;
    }

    /**
     * @brief IDLE到底多长才算空闲 一个数据帧的长度
     *  假设配置为八位，1个起始位 八个数据位 1个结束位 都为高电平
     * 
     */
    if (USART1->SR & USART_SR_IDLE)
    {
      /**
       * 检测到空闲总线为1，说明是空闲的
       * 如果读取该操作 会使RXNE的值为0说明有数据，会使后面发送的数据丢失
       * 先读USART_SR，然后读USART_DR 可以清除IDLE位
      */
      USART1->DR;
      return;
    }
    /* while((USART1->SR&USART_SR_RXNE)==0)
    {
      if(USART1->SR&USART_SR_IDLE)
      {
         USART1->DR;
         return;
      }
    }
    buff[i]=USART1->DR;
    i++; */
  }
}

/**当调用printf的时候，会自动调用这个方法执行，只需要调用一个通过串口发送字符的函数 */
int fputc(int c, FILE *file)
{
  Dri_USART_SendByte(c);
  return c;
}

/**
 * @brief 接收和发送数据说明
 *  低位优先
 *  接收数据:一开始RXNE为0,接收移位寄存器会根据配置数据的长度，例如配置接收数据的长度是八位，当八位
 *          接收成功后会自动把数据移动到RDR(接收数据寄存器)。移动到RDR以后RXNE为1
 *          只要数据到RDR,cpu就可以读取数据了。只要一读取RXNE自动清零变为0
 *  发送数据:一开始TXE就为1，cpu或DMA把数据写入到发送数据寄存器(TDR)，只要一写入TXE就变为0，一旦写入达到设定长度，会自动把数据
 *          移动到发送移位寄存器,同时设置TXE变为1，
 *  TDR和RDR背后有发送控制器和接收控制来配置相关操作
 *  
 *  三类寄存器: CR：控制寄存器  DR:数据寄存器 SR:状态寄存器
 * 
 *  DB9：两排九根线，RXD,TXD用到，CTS和RTS属于硬件流控制
 *  通讯方式：1.先发送信号 2. 对方回复信号会用到RI,DSR等其它机耕线，现在基本不用了
 *  VGA:三排
 * 
 * 问题:usart1这里的复用引脚为什么不用配值AFIO
 *     数据手册->引脚定义查看表格中有的引脚有默认复位功能，所以就不用配置AFIO
 *     关注一下PA13复位后不一样,这就是能下载的原因,自己画板就可以参考
 *     但是有的引脚有多个复用功能，例如PA9就有两个默认复用功能，这里就涉及到
 *     IO重定向，用了一个，另外一个就不能同时用，如果同时用发现其它引脚也有这些功能
 *     这时就需要配置AFIO。
 */