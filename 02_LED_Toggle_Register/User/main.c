#include "Delay.h"
#include "Driver_Led.h"
uint32_t Leds[]={LED1_YELLOW,LED2_BLUE,LED3_GREEN};
//UINT16_MAX
/**
 * @brief 
 * 汉字拼音里没有以U开头的文字
 * @return int 
 */
int main(){
	u8 size=3;
    Dri_Led_Init();
	Dri_Led_OffAll(Leds,size);

	while(1){
		/* Dri_Led_On(Leds[0]);
		Delay_ms(1000);
		Dri_Led_Off(Leds[0]);
		Delay_ms(1000); */
		
		Dri_Led_Toggle(LED2_BLUE);
		Delay_ms(1000);
		//Delay_BadWay();
	}
}





