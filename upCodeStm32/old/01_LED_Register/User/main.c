
#include "stdint.h"
#include "stm32f10x.h"                  // Device header

int main(){
	// 结构体指针是箭头 结构体变量是点
	/** 
	// 1.开启GPIOA的时钟
	*(uint32_t *)(0x40021000+0x18) =4;

	//2.给GPIO设置工作模式
	//给PA0设置为通用推挽输出 0000 0011=0x3   PA1 0011 0000 =0x30
	*(uint32_t *)(0x40010800+0x00) =0x03; //0x30
	
	//3.给输出寄存器赋值
	 *(uint32_t *)(0x40010800+0x0c) =0xfffe;// 0xfffd
	*/
	
	//进化1 地址换成st定义好的宏
	/** 
	RCC->APB2ENR=4;
	GPIOA->CRL=3;
	GPIOA ->ODR=0xFFFE;
	*/
	
	
	//进化2：只操作需要的位  位运算
	//RCC->APB2ENR|=4;
	//GPIOA->CRL|=1;
	//GPIOA->CRL|=2;
	//GPIOA->CRL&=~4;
	//GPIOA->CRL&=~8;
	//GPIOA ->ODR &= ~1;
	
	
	
	//RCC->APB2ENR|=1<<2;
	//GPIOA->CRL|=1;
	//GPIOA->CRL|=1<<2;
	//GPIOA->CRL&=~(1<<2);
	//GPIOA->CRL&=~(1<<3);
	//GPIOA ->ODR &= ~1;
	
	
	
	RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
	//黄灯亮
	GPIOA->CRL|=GPIO_CRL_MODE0_0;
	GPIOA->CRL|=GPIO_CRL_MODE0_1;
	GPIOA->CRL&=~(GPIO_CRL_CNF0_0);
	GPIOA->CRL&=~(GPIO_CRL_CNF0_1);
	GPIOA ->ODR &= ~GPIO_ODR_ODR0;
    
	//蓝灯亮
	GPIOA->CRL|=GPIO_CRL_MODE1_0;
	GPIOA->CRL|=GPIO_CRL_MODE1_1;
	GPIOA->CRL&=~(GPIO_CRL_CNF1_0);
	GPIOA->CRL&=~(GPIO_CRL_CNF1_1);
	GPIOA ->ODR &= ~GPIO_ODR_ODR1;

	//绿灯亮  在高位
	/**GPIOA->CRH|=GPIO_CRH_MODE8_0;
	GPIOA->CRH|=GPIO_CRH_MODE8_1;
	GPIOA->CRH&=~(GPIO_CRH_CNF8_0);
	GPIOA->CRH&=~(GPIO_CRH_CNF8_1);
	GPIOA ->ODR &= ~GPIO_ODR_ODR8;*/

	
	
	////////////////////////////////////////////////////////////
	
	//RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
	//GPIOA->CRL|=0x33;
	//GPIOA ->ODR &= ~3;
	
	//RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;
	//GPIOB->CRH|=0x33;
	//GPIOB ->ODR &= ~4;
	
	
	
	
}
