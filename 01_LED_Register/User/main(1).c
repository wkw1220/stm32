
#include "stdint.h"
#include "stm32f10x.h"  
/**
 * arm公司简介
 *   一开始是英国公司，然后卖给日本软银，arm架构处理器及相关外围组件的电路设计方案，简单的说
 *   就是设计芯片图纸
 *  几乎所以移动端处理器均采用arm授权方案设计
 *  整个芯片CPU(通用计算单元)只是整个芯片的一部分，还包括GPU(图形处理单元),NPU(神经处理单元),I/O
 *  接口等 总称为SoC:system on chip,片上系统
 * 
 *   ARM出图纸
 *   公司(枭龙，苹果，联发科，海思等)买图纸-->给代工厂(台积电)加工
 *   问题：图纸都是从arm买来的，为什么每家公司生产的芯片不一样,因为有很多公司自己设计芯片图纸
 *   那么arm就有两种授权方式:
 *   1.内核授权:图纸和指令集都授权给公司(枭龙，联发科，海思，stm32等)
 *   2.架构授权:只授权指令集(苹果公司等)
 *   为什么不自己设计一套指令集：生态护城河问题，考虑到应用程序的兼容性
 *   新的指令集:Risc-V
 * 
 */

/**
 * @brief 下载器说明
 * 地阔星是嘉立创开源的最小stm32开发板
 * type-c一般不能用于下载
 * STLink是专门为了给stm32芯片下载程序开发的一种下载器，它内部其实也是一个stm32C8T6芯片+LDO芯片
 * JLink:可以不止烧录st公司的芯片，也可以下载别的公司芯片
 */

/**
 * @brief 供电方式
 * stlink也有3.3v和GND,说明内部也有一个LDO,这种方式也可以直接给芯片供电
 * LDO:低压差线性稳压器。
 * 如果传统的低压设备想获取稳定的电压为3.3v,输入设备至少为8v以上
 * ASM1117是实现LDO技术的一个芯片
 * 
 * type-c的供电方式
 *   5v------->ASM1117----->3.3v
 * 
 */

/**
 * @brief 晶振
 * 51--->11.0592Mhz
 * stm32 -->8Mhz
 *       -->32.768kHz
 * 1.stm32晶振小于51,是不是stm32要比51慢?
 *   不是,51是拿外部晶振直接用作主频
 *   32内部有一个特殊的电路倍频器，最高可以是9倍 达到72M
 * 晶振的物理属性使得晶振的震动频率已经达到了极限，现在只能提高工艺和倍频提高震动频率。
 * 一般电脑主频率达到5.5Ghz已经是极限了。
 * 2.为什么需要两个晶振?
 *  32.768kHz=2^15,容易得到1hz的分频，容易实现低功耗模式
 */

/**
 * @brief boot启动模式选择
 * 1.Flash启动:Boot0直接接GND低电平,目前就用这种方式
 * 2.SRAM启动
 * 3.bootLoader启动
 * 
 * 复位信号:1.stlink可以给
 *         2.物理按键Nrst可以发送
 * 
 */

/**
 * @brief stlink安装
 * x86:32位
 * amd64：位
 * x86(32位) Intel --->升级-->x86 64位 但是不兼容32位，导致32位写的程序要重新写
 * amd64位(amd公司)反过来兼容x86(32位的)
 * 
 * 英特尔到了I7-9代以后又反过来兼容AMD64位的 
 * 
 * 安装stlink驱动是为了能让电脑识别到stlink，不然电脑会把它识别成普通股的硬盘 
 * stlink-upgrade固件升级:是方便识别到最新的芯片
 */

/**
 * @brief 最小支持文件选择及其含义
 * CMSIS：Cortex Microcontroller Software Interface Standard，Cortex 微控制器软件接口标准
 *        是对ARM内核CPU进行支持的--->也就是对系统架构图中Cortex-M3作支持的分为两块
 *        1.CoreSupport:Core_cm3.c/.h
 *        2.DeviceSupport  -->startup --arm -->选择对应设备的汇编文件.s(最先执行的文件，再执行main)
 *                         |
 *                         -->stm32公司做对应支持的文件：stm3210x.h system_stm3210x.c/.h
 * 
 *       要选择一共六个文件
 * stm32f10x_stdperiph_Driver:这个文件主要是对片上外设做支持的标准库,但是我们不基于标准库开发
 *                            这个文件夹里所有东西不用
 */

 /*  32位指的是地址是32位
 *  cortex-A advance 高性能
 *        -R realTime  实时
 *        -M micro 嵌入式
 * 2.smt 32位
 *   cortex-M 地址总线是32位
 * 3.应用场景
 *   物联网 工业自动化 汽车电子 消费电子 医疗设备 智能家居 航空航天 安全系统 可穿戴设备 智能交通
 * 4.命名规则
 *   DSP：数字信号处理器
 * 5.存储器说明
 *   芯片内部集成内存和外存 内存-->SRAM(64K)  外存-->flash(512K)
 *   如果芯片内部内存或者外存不够用可以扩展 扩展的内存叫SPRAM  
 *   EEPROM 是个什么鬼东西
 * 
 * 每一个芯片都有对应的型号，要想开发程序，
 *   1.必须首先先下载开发工具KeilMDK
 *   2.下载对应的芯片型号的开发支持包
 *     (1).通过KeilMDK在线下载并安装
 *     (2).从官网下载离线安装
 *       上st公司网站，选择cortex-M3系列-->103ZE系列 下载以后点击安装
 * 6.CMOS电压(平)：3.3V
 *   TTL电压(平)：5V
 * TTL转CMOS电压可使用AMS1117芯片
 * 
 * 7.开发程序
 *   芯片支持包安装:https://www.keil.arm.com/devices/ 
 *   1.在线下载：直接从keil
 *   2.先从官网下载，然后离线安装
 *   芯片支持包安装好以后，要求官网下载外设标准库，里面有提供标准的启动文件
 *   https://www.st.com/zh/embedded-software/stm32-standard-peripheral-libraries/products.html
 *   
 *   1.添加新的文件夹，魔法棒右边的品字形图案
 *   2.如何把新添加的文件夹路径添加进来
 *      魔法棒-->c/c++-->Include Paths
 *              target-->选择对应的编译器
 *              output-->Create Hex File
 *              C++ level 0
 *                  C99
 *             Debug -->stlink
 *                     reset and run 
 *                     run勾选给去掉
 * 
 * 
 *
 * 1.先让对应的GPIO引脚使能（RCC 寄存器）
 *    本系统中有7组引脚（A B C D E F G ）每一组有16个引脚，共16*7=112个引脚
 *    如果设置A组，A组这16个（0到15)都获得使能。也就是使能是一组一组的赋能
 * 2.配置IO引脚工作模式(输入模式/输入模式)
 *     MODE：配置工作模式
 *          输入模式：00
 *          输出模式：01(10HZ) 10(2HZ) 11(50HZ)
 *     CNF：设置对应这种工作模式下的那种模式
 *          输入模式：00：模拟输入模式  01：浮空输入模式 10：上拉/下来输入模式 11：保留
 *          输出模式：00：通用推挽输出模式 01：通用开漏输出模式 10：复用推挽输出模式 11：复用开漏输出模式
 * 3.给对应的IO设置值
 * 
 * 某一位设置1  直接和这位或
 * 某一位设置0  这位先取反再与
 */
int main(){

    // 结构体指针是箭头 结构体变量是点
	//(0x40021000+0x18)数字:地址值 -->(uint32_t *)数字:强转为指针-->*(uint32_t *)数字对当前地址赋值
	 
	// 1.开启GPIOA的时钟
	*(uint32_t *)(0x40021000+0x18) =4;

	//2.给GPIO设置工作模式
	//给PA0设置为通用推挽输出 0000 0011=0x3   PA1 0011 0000 =0x30
	*(uint32_t *)(0x40010800+0x00) =0x03; //0x30
	
	//3.给输出寄存器赋值
	*(uint32_t *)(0x40010800+0x0c) =0;
	 *(uint32_t *)(0x40010800+0x0c) =0xfffe;// 0xfffd
	
	
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
	GPIOA->CRH|=GPIO_CRH_MODE8_0;
	GPIOA->CRH|=GPIO_CRH_MODE8_1;
	GPIOA->CRH&=~(GPIO_CRH_CNF8_0);
	GPIOA->CRH&=~(GPIO_CRH_CNF8_1);
	GPIOA ->ODR &= ~GPIO_ODR_ODR8;

}
