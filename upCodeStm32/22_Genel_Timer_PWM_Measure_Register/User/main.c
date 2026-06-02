#include "Dri_USART.h"
#include <stdio.h>
#include "Driver_Led.h"
#include "Driver_Timer5_PWM.h"
#include "Driver_Timer4_Mesure.h"
#include "Delay.h"

/**
 * @brief 一定不能忘了把PA1和PB6引脚连接起来！！！！
 * 定时器5通道2:PA1,会产生一共方波
 * 定时器4通道1:PB6,开启的是输入捕获功能
 *    PWM是开启输入比较功能
 * @return int 
 */

int main(){
	Dri_USART_Init();
	printf("获取定时器4的周期和频率!\r\n");
	Dri_Timer5_Pwm_Init();
	Dri_Timer5_Pwm_Start();
	Dri_Timer4_Init();
	Dri_Timer4_Start();
    double t, f;
	//u8 count=1;
	//u8 dir =0;
    while(1){
		t = Dri_Tim4_getCycle();
        f = Dri_Tim4_getFrequency();
        printf("t=%.4fms, f=%.4fHz\r\n", t, f);
        Delay_ms(2000);
		/* Dri_TIM5_Pwm_SetDutyCycle(count);
		if(dir==0){
           count=count+5;
		}else if(dir==1){
           count=count-5;
		}
		if(count>200){
		   dir=1;
		}else if (count<=5)
		{
		   dir=0;
		}
		Delay_ms(20);  */
	}
}



