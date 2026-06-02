#include "stm32f10x.h"                  
#include "Delay.h"
#include "Dri_USART.h"
#include "Driver_Led.h"
#include <string.h>

/**
 * @brief 使用轮询的方式接收和发送字符串
 * 
 * 波特率对通讯距离有很大影响 115200 在50m到200m
 *                          9600   在1200m
 * UART:VCC表示1 GND表示0 抗干扰能力弱，一般不会超过3m
 *       三根线:RX TX GND 
 *       异步全双工串口通信
 * RS232:直接把电压拉高,例如12V表示1,GND表示0. 抗干扰能力强(苏联军用电压在96V)
 *       一般也是三根线:RX TX GND
 *       异步全双工串口通信
 * RS485:A线和B线，通过差分信号传输数据，压差可以在9~16v之间
 *       半双工（主流）：2 根核心线（A、B），推荐 3 根（加 GND）
 *       全双工（少见）：4 根核心线（TX+、TX-、RX+、RX-），推荐 5 根（加 GND）。
 *       异步串口通信
 * 
 * 为什么USB线也是差分信号，传输的距离短:一般小于3m，因为压差比较小在500mv到1v之间
 * USB 虽然是串行通信，但通过差分信号、高速编码（如 NRZI 编码）和
 * 多通道复用（如 USB 3.0 的发送 / 接收通道分离），实现了远超传统串行接口的传输速率，
 * 甚至超过部分早期并行接口，因此可能被误认为是并行通信。但本质上，其 “逐位传输” 的核心机制仍属于串行通信范畴。
 */
int main(){
	Dri_USART_Init();
	Dri_Led_Init();
	Dri_Led_Off(LED1|LED2|LED3);
	printf("start....");
	/**
	 * @brief 发送一个字符
	 * 
	 */
	Dri_USART_SendByte('a');

	/**
	 * @brief 发送一个字符串
	 * 
	 */
	uint8_t* str="使用的是串口3\r\n";
	Dri_USART_SendStr(str);
	u8 buff[100]={0};
	while (1)
	{
		/**
		 * @brief 接收字节是通过轮询的方式，
		 * 缺点就是会使程序一直阻塞在这个地方
		 * 
		 */
		/*uint8_t c=Dri_USART_ReceiveByte();
		Dri_USART_SendByte('b');
		Dri_USART_SendByte(c);  */
        Dri_Led_Toggle(LED1);

		//Dri_USART_ReceiveStr(buff);
		/**
		 * 这是一条测试语句，验证上条语句会不会在程序中发生阻塞,发现ReceiveStr好像不会在程序中阻塞
		 * 但是本质上还是阻塞的，只不过是阻塞的时间比较短
		 * 但是有个问题，如果在接收之后里面输出一个字符，在发送接收的字符串有时会乱码
		 * 
		 * 原因是：先读USART_SR，然后读USART_DR 虽然检测到了空闲，
		 *         但是返回之前少操作了USART_DR，导致RNXE的值一直为1，这样就会有误差。
		 */
		Delay_s(1);
		Dri_USART_SendByte('b');
		Dri_USART_SendByte('\n');

		Dri_USART_SendStr(buff); 
	} 
}



