/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/**
 * 架构图:
 * 被动单元:在整个电路中，这部分电路不会主动做事情
 * 三个被动单元：
 *  1. 内部SRAM:存储程序执行时用到的变量
 *  2. 内部闪存存储器FLASH:存储程序执行时用到的变量
 *     a. 存储下载的程序
 *     b. 程序执行时用到的常量
 *  3.AHB到APB的桥(AHB到APBx) AHB就相当于扩展坞,例如自己用到的一个usb分出多个usb接口
 *    桥1:连接APB2 72MHz 高速外设
 *    桥2:连接APB1 36MHz 低速外设
 */

/**
 * 四个驱动(主动)单元
 * 计算机总一般有三大总线:地址总线 数据总线 控制总线
 * 1.DCode:相当于数据总线
 * 2.system:地址总线和控制总线的合二为一
 * 3.DMA1
 * 4.DMA2
 * 这四个驱动单元都和总线矩阵相连
 *   总线矩阵就相当于转接员的角色。相当于网络中的路由器或交换机的功能
 *
 * 其它单元
 * 1.ICode:连接Flash,实现指令的读取
 * 2.FSMC:用来扩展外部的SRAM,Flash,连接LCD屏幕等
 *
 */

/**
 * 时钟树:时钟树是一套硬件电路，控制时钟的生成和时钟的供给。
 *  问:为什么芯片内部有时钟源却不用?
 *   首先晶振是没办法做到小型化的，芯片内部时钟是不可能用晶振做的，是用RC震荡电路实现的，
 *   但是RC震荡电路对温度很敏感，容易得到一个不稳定的时钟信号，例如在串口通讯或者时序要求
 *   比较严格的地方就非常不合适。但是晶振对温度就不是很敏感。如果从节约成本上又不影响性能
 *   的产品就可以用内部时钟。例如就简单的做个闹钟。
 */

/**
 * 如果构建HAL项目:
 * 1.sys系统工作模式:烧录程序的方式
 *   no debug中选择serial wire串行线
 *   如果没有选择是no bug，也可以烧录，但是芯片是没办法再烧录第二次
 *   如果想再重新烧录，必须修改启动模式为bootLoader模式
 * 2.RCC设置:HSE和LSE都选择Crystal/Ceramic Resonator
 * 3.Clock Configuration：因为再RCC设置了，所以HSE和LSE两个值都是蓝色并可用
 * 4.GPOI引脚配置
 *   GPIO Output Level：表示一上电时的电平
 *                      High：高电平--->没电
 *                      Low：低电平--->有电
 * 
 * 不要再MDK-ARM文件里建新文件
 */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
    // HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
    // HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint32_t leds[] = {LED1_Pin, LED2_Pin, LED3_Pin};
  while (1)
  {
    // 流水灯效果 start------------------
    /**for(uint8_t i=0;i<sizeof(leds)/sizeof(uint32_t);i++){
      HAL_GPIO_WritePin(GPIOA,leds[i],GPIO_PIN_RESET);
      HAL_Delay(500);
    }

    for(uint8_t i=0;i<sizeof(leds)/sizeof(uint32_t);i++){
      HAL_GPIO_WritePin(GPIOA,leds[2-i],GPIO_PIN_SET);
      HAL_Delay(500);
    }
    */
    // 流水灯效果 end------------------
    HAL_GPIO_TogglePin(GPIOA, leds[0]);
    HAL_Delay(500);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
