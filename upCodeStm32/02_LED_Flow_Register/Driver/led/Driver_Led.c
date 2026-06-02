#include "Driver_Led.h"
#include "stm32f10x.h"
/**
 * @brief 初始化
 *  通用推挽输出
 *  mode:11 50Hz
 *  conf:00
 *  PA0 PA1 PA8
 *  推挽输出总结：
 *  1.向设置/清除寄存器(BSRR)对应的设置位或清除位写1
 *  2.或者直接向输出寄存器(ODR)对应位写1或0
 *  3.也可以从输入寄存器读取引脚状态
 *  4.工作时两个MOS管交替工作来驱动负载.输出1时，上部晶体管导通将电流推向负载;
 *    输出0时，下部晶体管导通，将电流拉回地线。所以叫推挽输出
 *  应用场景：1.驱动能力需求较高的场合
 *           2.高速信号传输
 *           3.无需共用信号线的场合
 *
 *
 * 开漏输出总结:
 * 1.上部晶体管永远关闭
 * 2.可以输出0
 * 3.如果要输出1必须接上拉电阻
 * 应用场景:1.多个设备共用信号线
 *         2.不同电压系统之间的接口
 *         3.需要外部上拉电阻来确定逻辑高电平场合
 *
 * 注意：根据电路图，改变ODR的值可以通过BSRR或者直接向ODR写入值，如果信号不经过ODR
 *        直接来自片上外设到输出控制器,例如串口这种模式就是复用，严格意义上这时接口
 *        不能叫做GPIO了(说白了GPIO控制器来自两个线路，一个是ODR,这时就是GPIO;
 *        一个是来自片上外设，这是就是复用了。就看控制权在谁手上)
 */

/**
 * @brief 施密特触发器的作用
 *    1.把模拟量转化成数字量
 *    2.消抖
 *   如何消抖:
 *    1.当输入电压高于上限的时候，会输出高电平
 *    2.当输入电压低于下限的时候，会输出低电平
 *    3.当输入电压在下限和上限之间的时候，电平不会发生变化
 * 引脚采集的高电平或者低电平其实就是采集引脚的电压
 *
 *
 * ADC模块其实就是一个电压表
 *
 */

/**
 * @brief GPIO输入模式 MODE=00
 * 在输入引脚处有个钳位电路 ，会把电压钳位在一定范围，保证输入电压的安全
 * 这个要配合施密特触发器使用
 * 1.模拟输入:施密特触发器关闭，直接输出模拟量(也就是电压的大小) CNF=00
 *  除了模拟输入，施密特触发器会打开
 *  打开以后都会给IDR寄存器和复用功能输入存放值，所以不像输出那样分通用输入和复用输入
 * 2.上拉输入/下拉输入:CNF=10 ODR:0下拉 ODR:1 上拉 参考手册106页
 *     
 * 3.浮空输入：外面的信号会直接到施密特触发器，电压多少直接是由接入这个IO设备输出的电压
 *             决定。如果这个IO设备是开漏输出，输出的不是低电平，如果没有接上拉电阻
 *             一定处于高阻态，有可能会导致IDR里的值在0和1直接跳变
 *           如果外面的设备是推挽输出，这里就可以配浮空输入
 *           CNF=01
 * 
 * 注意：配置成浮空输入,上拉或者下拉输入，发现模拟量信号输入也是导通，不受影响，所以在模拟量
 *      采集的时候如果给配置成浮空输入也是能够采集到模拟量的
 */
void Dri_Led_Init()
{
  /** 
   * 时钟使能要放在最前面
   * 运算电路不需要时钟信号
   * 存储电路需要时钟信号
  */
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  // GPIOA->CRL|=0x33;这里配置为推挽输出，其实也可以配置成开漏输出
  GPIOA->CRL |= (GPIO_CRL_MODE0 | GPIO_CRL_MODE1);
  GPIOA->CRL &= ~(GPIO_CRL_CNF0 | GPIO_CRL_CNF1);

  GPIOA->CRH |= GPIO_CRH_MODE8;
  GPIOA->CRH &= ~GPIO_CRH_CNF8;
  
}

void Dri_Led_On(uint32_t Led)
{
  GPIOA->ODR &= ~Led;
}

void Dri_Led_Off(uint32_t Led)
{
  GPIOA->ODR |= Led;
}

void Dri_Led_OnAll(uint32_t Leds[], uint8_t size)
{
  uint8_t i;
  for (i = 0; i < size; i++)
  {
    Dri_Led_On(Leds[i]);
  }
}

void Dri_Led_OffAll(uint32_t Leds[], uint8_t size)
{
  uint8_t i;
  for (i = 0; i < size; i++)
  {
    Dri_Led_Off(Leds[i]);
  }
}

void Dri_Led_Toggle(uint32_t Led)
{
  if ((GPIOA->IDR & Led) == 0)
  {
    Dri_Led_Off(Led);
  }
  else
  {
    Dri_Led_On(Led);
  }
}

void Dri_Led_Toggle_Xor(uint32_t Led)
{
  GPIOA->ODR ^= Led;
}

/**
 * @brief 在 STM32 中，对GPIOx_BSRR寄存器的写操作不会让寄存器自身自动置 0，
 *        也不会在操作后自动清除写入的位，但该寄存器的只写属性和操作的一次性特性
 *        GPIOx_BSRR本身是只写寄存器，无存储功能
 *        注：如果同时设置了BSy和BRy的对应位， BSy位起作用。
 * 
 * @param br 
 */
void Dri_Led_On_Off_BSRR(uint32_t br){   
  GPIOA ->BSRR = br;
  //GPIOA ->BSRR |= br;
}
