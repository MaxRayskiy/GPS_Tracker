/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "gps.h"
#include "gprs.h"

//delete!!
#include <stdio.h>
#include <stdlib.h>

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

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */
  sim800_init();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  output_buffer_num = 1;
  output_buffer_pointer = 0;
  gps_buffer_pointer = 0;
  gps_data_ready = 0;
  gps_dump = 0;
  gprs_dump = 0;
  //
  start_message = 0;
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_7);
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  LL_USART_EnableIT_RXNE(USART1);
  LL_USART_EnableIT_ERROR(USART1);

  //LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_7);
  LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
  LL_mDelay(3000);
  LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);

  LL_USART_Disable(USART3);
  LL_mDelay(200);
  LL_USART_Enable(USART3);

  LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_7);
 // Enable GPS messages

 LL_mDelay(5000);
 GPRS_connect_TCP();
 LL_mDelay(500);
 while(1) {
	 for (int i = 0; i < 100; ++ i) {
		 if (gps_data_ready == 1) {
			 gps_data_ready = 0;
			 GPS_parse();
			 GPS_dump_buffer();
		 }
		 LL_mDelay(100);
	 }

	 GPRS_send_tcp();
	 LL_mDelay(2000); // 10 sec

 }
  //LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_8);
 /*
  if (!GPRS_check_power_up() ) {
  	  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_6);
  	  sprintf(debug_buffer, "power FAIL\r\n");
  	  LL_USART_EnableIT_TXE(USART1);
  	  LL_mDelay(3000);
  	  while(1);
  }

  if (!GPRS_init() ) {
	  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_6);
      sprintf(debug_buffer, "init FAIL\r\n");
   	  LL_USART_EnableIT_TXE(USART1);
   	  LL_mDelay(3000);
   	  while(1);
   }
*/
  LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_9);

/*
  if(GPRS_send_SMS_debug()) {
	  sprintf(debug_buffer, "SMS OK\r\n");
	  LL_USART_EnableIT_TXE(USART1);
  }*/
  LL_mDelay(500);
  //GPRS_connect_TCP();
  // Enable GPS messages
  //LL_USART_EnableIT_RXNE(USART1);
  //LL_USART_EnableIT_ERROR(USART1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
  {
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_6, LL_RCC_PREDIV_DIV_1);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(48000000);
  LL_SetSystemCoreClock(48000000);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
