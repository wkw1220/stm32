
#include <string.h>
#include "Delay.h"
#include "Driver_Buzzer.h"
#include "Driver_Led.h"
#include "Driver_Usart.h"
#include "Int_W24C02_Hard.h"
#include "Driver_TIM6.h"
#include "Driver_Timer5_PWM.h"
int main(){
	Dri_USART1_Init();
	printf("Start comminucation..\r\n");
	Inf_W24C02_Init_Hard();
	Inf_W24C02_WriteByte_Hard(0x00,'w');
	uint8_t byte=Inf_W24C02_ReadByte_Hard(0x00);
	printf("%c\r\n",byte);
	Inf_W24C02_WriteBytes_Hard(0x01,"hello123",8);
	uint8_t buff[100]={0};
	Inf_W24C02_ReadBytes_Hard(0x01,buff,8);
	printf("%s",buff);
	while(1); 

    /** 
	Dri_USART1_Init();
	printf("send success!\r\n");
	//Delay_s(2);
	//printf("1\r\n");
	//Dri_Led_Init();
	//Driver_SysTick_Init();
	//Dri_Timer6_Init();
    
	//Dri_Led_Init();
    Dri_Timer5_Pwm_Init();
	Dri_Timer5_Pwm_Start();
	uint8_t dutyCycle = 0;
    uint8_t dir = 0; // 变大
	Dri_TIM5_Pwm_SetDutyCycle(dutyCycle);
    while(1)
	{
        if (dir == 0)
		{
			dutyCycle ++;
			if(dutyCycle>=99)
			{
			  dir=1;
			}

		}else{
			dutyCycle--;
			if(dutyCycle<=1){
			 dir=0;
			}
		}

		Dri_TIM5_Pwm_SetDutyCycle(dutyCycle);
		Delay_ms(15);
	}
	*/

}




