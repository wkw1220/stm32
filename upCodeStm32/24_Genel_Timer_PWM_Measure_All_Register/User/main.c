#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_Timer5_PWM.h"
#include "Driver_Timer4_Mesure.h"
#include "Delay.h"



int main(){
	Dri_USART_Init();
	/**
	 * 因为复位模式:触发输入的上升沿会重新初始化计数器(让计数器重新从0开始计数)，这样
	 * 就不需要手动清零计数器，这不需要去数上升沿的个数，从而可以硬件自动完成测量
	 */
	printf("硬件获取获取定时器4的周期和频率寄存器版本!\r\n");
	Dri_Timer5_Pwm_Init();
	Dri_Timer5_Pwm_Start();
	Dri_Timer4_Init();
	Dri_Timer4_Start();
    double t, f,dutyCycle;
    while(1){
		t = Dri_Tim4_getCycle();
        f = Dri_Tim4_getFrequency();
		dutyCycle=Dri_Tim4_getDutyCycle();
        printf("t=%.4fms, f=%.4fHz,d=%0.2f%%\r\n", t, f,dutyCycle*100);
        Delay_ms(1000);
	}
}



