/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  *  此案例用两种方式实现
     1.轮询的方式
     2.中断的方式
     真正的项目是通过第二种中断的方式
  * 
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
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
  /**
   * 此案例用两种方式实现
   *  1.轮询的方式
   *  2.中断的方式
   * 真正的项目是通过第二种中断的方式
   * 
   */
  uint16_t realLen;
  uint16_t wantLen=100;
  uint8_t buff[100]={0};
  uint32_t timeOut=1000;//HAL_MAX_DELAY
  uint8_t isComplete=0;

  /**中断回调函数 */
  void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart,uint16_t Size){
      //是否是串口1发生的中断
      if(huart->Instance==USART1){
        //处理相应的业务逻辑  这里最好给个标志位放到主程序中去处理
        //HAL_UART_Transmit(&huart1,buff,Size,1000);
        //HAL_UARTEx_ReceiveToIdle_IT(&huart1,buff,wantLen);

        //使用标志位的方式，但是没办法获取接收字符的实际长度
        isComplete=1;
      }
  }
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
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  /**
   * 中断的方式
   * 1.接收的数据长度=wantLen
   * 2.遇到空闲帧
   * 
   * 中断产生以后会调用回调函数HAL_UARTEx_RxEventCallback()
   * 
  */
  HAL_UARTEx_ReceiveToIdle_IT(&huart1,buff,wantLen);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  uint8_t test_idle[]="test_idle";
  HAL_UART_Transmit(&huart1,test_idle,strlen(test_idle),1000);
  
  while (1)
  {
    //printf("hello atguigu\r\n");
    /**
     * @brief Construct a new hal uartex receivetoidle object
     * 这是一种阻塞式的接收方式
     * 1.接收数据的长度=wantLen
     * 2.检测到了空闲帧
     * 3.接收超时时间达到了timeOut
     * 满足这三个条件阻塞终止
     */
    //HAL_UARTEx_ReceiveToIdle(&huart1,buff,wantLen,&realLen,timeOut);
    //HAL_UART_Transmit(&huart1,buff,realLen,1000);

    //HAL_Delay(1);
    //HAL_UART_Transmit(&huart1,"main...\r\n",9,1000);
    /* USER CODE END WHILE */
    
    if(isComplete==1){
      isComplete=0;
      /**
       * @brief wantLen:期望接收的长度 会复制给 huart1.RxXferSize
       *     huart1.RxXferSize:期望接收的长度
       *     huart1.RxXferCount:还剩多少没接收的长度
       *     huart1.RxXferSize - huart1.RxXferCount :表示实际接收到的长度
       *     通过这种方式可以获取实际接收的长度
       *     回调方法里可以直接读取，但是通过在外面只能用这种方式
       */
      //printf("huart1.RxXferSize:%d\r\n",huart1.RxXferSize);
      //printf("huart1.RxXferCount:%d\r\n",huart1.RxXferCount);
      HAL_UART_Transmit(&huart1,buff,huart1.RxXferSize-huart1.RxXferCount,1000);
      HAL_UARTEx_ReceiveToIdle_IT(&huart1,buff,wantLen);
    }



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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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

#ifdef  USE_FULL_ASSERT
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
