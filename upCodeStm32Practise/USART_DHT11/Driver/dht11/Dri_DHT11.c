#include "Dri_DHT11.h"
#include "Delay.h"
/**
 * @brief Set the pin output object
 * 推挽输出
 */
void set_pin_output(void){
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
    GPIOA->CRL|=GPIO_CRL_MODE1;
    GPIOA->CRL&=~GPIO_CRL_CNF1;
}

/**
 * @brief Set the pin input object
 * 浮空输入
 */
void set_pin_input(void){
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
    GPIOA->CRL&=~GPIO_CRL_MODE1;
    GPIOA->CRL&=~GPIO_CRL_CNF1_1;
    GPIOA->CRL|=GPIO_CRL_CNF1_0;
}

void start(void){
  //MCU准备发出信号 先把模式切换到输出
  set_pin_output();
  //拉高电平
  SCL_HIGH;
  SCL_LOW;
  //至少要拉低18ms
  Delay_ms(20);
  //MCU再拉高信号 20-40us
  SCL_HIGH;
  Delay_us(20);
  //主动权要交给DHT11
  set_pin_input();
  //等待DHT11把信号拉低
  while(GPIOA->IDR&GPIO_IDR_IDR1);//！=1 说明为高 ==0 说明为低 拉低
  while((GPIOA->IDR&GPIO_IDR_IDR1)==0);//拉高
  while(GPIOA->IDR&GPIO_IDR_IDR1);//拉低
  //这时开始接收数据
}

uint8_t receiveOneByte(void){
    uint8_t data=0;
    for(uint8_t i=0;i<8;i++){
      //while(GPIOA->IDR&GPIO_IDR_IDR1);//本来就是拉低的 这行可以不要
      while((GPIOA->IDR&GPIO_IDR_IDR1)==0);//DHT11是否拉高电压，没拉高继续等
     //能执行到这里说明 已经开始传送数据
     Delay_us(50);
     data<<=1;
     if(GPIOA->IDR&GPIO_IDR_IDR1){
       data|=0x1;
       while(GPIOA->IDR&GPIO_IDR_IDR1);//拉低
     }else{
       data|=0x0;//可以不写
     }
    }
    //printf("data=%d\n",data);
    return data;
}

void receiveOneBytes(uint8_t* result){
  uint8_t i=0;
  start();
  //set_pin_input();
  uint8_t buff[5]={0};
  for(i=0;i<5;i++){
      buff[i]=receiveOneByte();
  }
  if(buff[0]+buff[1]+buff[2]+buff[3]==buff[4]){
     memcpy(result,buff,4);
  }
  //printf("%d,%d,%d,%d,%d\n",buff[0],buff[1],buff[2],buff[3],buff[4]);
}

