#ifndef __DRIVER_KEY_H__
#define __DRIVER_KEY_H__

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
 */