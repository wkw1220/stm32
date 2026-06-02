#include "Driver_Iwdg.h"

void Dri_Iwdg_Feed(void){
    //喂狗
    IWDG->KR = 0xAAAA;  
}


/**
 * @brief 
 * 
 * @param prer 预分频系数值
 * @param rlr  重装载寄存器值
 */
void Dri_Iwdg_Init(uint8_t prer,uint16_t rlr){
    //启动看门狗 看门狗一旦启动就无法停止，IWDG->KR = 0xCCCC会自动开启LSI
    IWDG->KR = 0xCCCC;   
    
    //允许访问重装载计数器和预分频器寄存器后
    IWDG->KR = 0x5555;               
   
    // 设置预分频器系数
    IWDG->PR = prer & 0x07;  
    
    //设置重装载寄存器       
    IWDG->RLR = rlr & 0x0FFF;       
    //IWDG->KR = 0xAAAA;   

    Dri_Iwdg_Feed(); //喂狗，重新装载计数器的值                       
}

