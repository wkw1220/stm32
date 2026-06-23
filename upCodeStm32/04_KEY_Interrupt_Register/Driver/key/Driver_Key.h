#ifndef __DRIVER_KEY_H__
#define __DRIVER_KEY_H__

#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bits for pre-emption priority, 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bits for pre-emption priority, 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority, 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority, 1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority, 0 bits for subpriority */


/**
 * @brief 按键定义初始化
 *
 */
void Dri_Key_Init(void);

#endif // !__DRIVER_KEY_H__

/**
 * @brief
 * key1 PF8 sw3 外面是低电平 接上拉电阻 下降沿
 * key2 PF9 sw4 外面是低电平 接上拉电阻 下降沿
 * key3 PF10 sw5 外面是高电平 接下拉电阻 上升沿
 * key4 PF11 sw6 外面是高电平 接下拉电阻 上升沿
 * 
 * key1 和 key2 接通以后是0v,在没有接通前接一个上拉电阻,没有接通时是3.3v,接通时是0v
 *    接通的瞬间有3.3v到0v的跳变,会产生一个下降沿  稳定后引脚是0v
 * key3 和 key4 接通以后是3.3v,在没有接通前接一个下拉电阻,没有接通时是0v,接通时是3.3v
 *    接通的瞬间有0v到3.3v的跳变,会产生一个上升沿  稳定后引脚是3.3v
 * 
 * 为什么时按键拉高了引脚的电压,而不是引脚电压拉低了按键电压,因为引脚外面输入电路是一个钳位电路
 */