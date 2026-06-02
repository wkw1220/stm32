#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include <string.h>
#include "Driver_Led.h"
#include "Driver_Key.h"

/**
 * @brief 内核控制的 在CM3编程文档里
 * 
 */
void EnterStopMode(void){
  /* 1. 设置普通睡眠模式 默认值就是零 可以不配置*/
  SCB->SCR |=SCB_SCR_SLEEPDEEP;

  //用到PWR外设，所以要开启PWR外设的时钟
  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  
  //深度睡眠下进入停机模式
  PWR->CR &=~PWR_CR_PDDS; 

  /**配置停机模式下电压调节器的的工作：0：开启 1：低功耗 */ 
  PWR->CR &=~PWR_CR_LPDS;
  /* 2. 进入睡眠模式，等待中断唤醒，可以靠任意中断唤醒 这是一条汇编语言 */
  __WFI();
}


void config_clock_after_stopmode(){
	/* 1.开启HSE */
	RCC->CR |= RCC_CR_HSEON;
	/* 2.等待HSE就绪 */
	while((RCC->CR & RCC_CR_HSERDY)==0);

	//3.使能PLL
	 RCC->CR |= RCC_CR_PLLON;
	//4.等待PLL就绪
	while((RCC->CR & RCC_CR_PLLRDY)==0);
	/* 5.配置PLL作为时钟 */
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= (0x2<<0);
	/* 6.等待PLL设置成功 */
	while((RCC->CFGR & RCC_CFGR_SWS)==0);
}

void getClock(uint32_t *sysclk,uint32_t* ahbClk,uint32_t* apb1Clk,uint32_t *apb2Clk){
	//获取 HSE HSI的system clock
	uint32_t sysClkSource=(RCC->CFGR & RCC_CFGR_SWS);
	if(sysClkSource==RCC_CFGR_SWS_HSI){
		*sysclk=HSI_VALUE;
	}else if(sysClkSource==RCC_CFGR_SWS_HSE){
		*sysclk=HSE_VALUE;
	}else if(sysClkSource==RCC_CFGR_SWS_PLL){
		uint32_t mull = ((RCC->CFGR & RCC_CFGR_PLLMULL)>>18)+2;
		*sysclk=HSE_VALUE * mull;
	}
	/**计算AHB时钟 */
	uint32_t ahbPrescaler=(RCC->CFGR & RCC_CFGR_HPRE)>>4;
	switch(ahbPrescaler){
		case 0x0: *ahbClk=*sysclk;break;
		case 0x8: *ahbClk=*sysclk>>1;break;
		case 0x9: *ahbClk=*sysclk>>2;break;
		case 0xA: *ahbClk=*sysclk>>3;break;
		case 0xB: *ahbClk=*sysclk>>4;break;
		case 0xC: *ahbClk=*sysclk>>6;break;
		case 0xD: *ahbClk=*sysclk>>8;break;
		case 0xE: *ahbClk=*sysclk>>10;break;
		default: *ahbClk=*sysclk;break;
	}

	/**计算APB1时钟 */
	uint32_t apb1Prescaler=((RCC->CFGR & RCC_CFGR_PPRE1)>>8);
	switch(apb1Prescaler){
		case 0x0: *apb1Clk=*ahbClk;break;
		case 0x4: *apb1Clk=*ahbClk>>1;break;
		case 0x5: *apb1Clk=*ahbClk>>2;break;
		case 0x6: *apb1Clk=*ahbClk>>3;break;
		case 0x7: *apb1Clk=*ahbClk>>4;break;
		default: *apb1Clk=*ahbClk;break;
	}

	/**计算APB2时钟 */
	uint32_t apb2Prescaler=((RCC->CFGR & RCC_CFGR_PPRE2)>>11);
	switch(apb2Prescaler){
		case 0x0: *apb2Clk=*ahbClk;break;
		case 0x4: *apb2Clk=*ahbClk>>1;break;
		case 0x5: *apb2Clk=*ahbClk>>2;break;
		case 0x6: *apb2Clk=*ahbClk>>3;break;
		case 0x7: *apb2Clk=*ahbClk>>4;break;
		default: *apb2Clk=ahbClk;break;
	}
}

/**
 * @brief 进入睡眠模式以后，再次唤醒以后，程序会从上次睡眠的地方处继续执行
 */
extern  buff[100];
extern  len;
extern  isReadySend;
int main(){
	Dri_USART_Init_WithInterruput();
	Dri_Key_Init();
	Dri_Led_Init();
	Dri_Led_Off(LED1|LED2|LED3);
	Dri_Led_On(LED3);
	while (1){
		uint32_t sysclk_temp,ahbClk_temp,apb1Clk_temp,apb2Clk_temp;
		getClock(&sysclk_temp,&ahbClk_temp,&apb1Clk_temp,&apb2Clk_temp);
		printf("Enter stop mode......\n");
		Delay_s(5);
		//EnterStopMode();

        uint32_t sysclk_temp1,ahbClk_temp1,apb1Clk_temp1,apb2Clk_temp1;
		getClock(&sysclk_temp1,&ahbClk_temp1,&apb1Clk_temp1,&apb2Clk_temp1);
		//需要开启时钟
		config_clock_after_stopmode();
		//Delay_s(1);
		printf("Wake up......\n");

        printf("System Clock=%d MHz, AHB Clock=%d MHz, APB1 Clock=%d MHz, APB2 Clock=%d MHz\n",
			sysclk_temp/1000000,
			ahbClk_temp/1000000,
			apb1Clk_temp/1000000,
			apb2Clk_temp/1000000
		); 

		printf("after......System Clock=%d MHz, AHB Clock=%d MHz, APB1 Clock=%d MHz, APB2 Clock=%d MHz\n",
			sysclk_temp1/1000000,
			ahbClk_temp1/1000000,
			apb1Clk_temp1/1000000,
			apb2Clk_temp1/1000000
		); 
		
	}
}



