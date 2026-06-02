
#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Driver_Led.h"
uint32_t Leds[]={LED1,LED2,LED3};

int main(){
	u8 size=3;
	u8 fangXiang=0;
    Dri_Led_Init();
	Dri_Led_OffAll(Leds,size);
	Dri_Led_On(Leds[0]);

	/*while(1){
		Delay_ms(500);
		Dri_Led_Off(Leds[0]);
		Delay_ms(1000);
		Dri_Led_On(Leds[0]);
	}*/
	Delay_s(1);
	Dri_Led_Off(Leds[0]);
	while(1){
	  u8 i;
	  u8 temp;
	  temp=size-2;
	  for (i = 1; i < size; i++)
	  {
		if(fangXiang==0){
          Dri_Led_On(Leds[i]);
		}else{
		  Dri_Led_On(Leds[temp--]);
		}
		Delay_s(1);
		Dri_Led_OffAll(Leds,3);
	  }
	  fangXiang=~fangXiang;
	} 
}



