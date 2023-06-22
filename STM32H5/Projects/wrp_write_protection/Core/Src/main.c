/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define WRP_ON

#ifdef WRP_ON

/* Write-protected sectors can be modified through option bytes only when
   PRODUCT_STATE = OPEN */

#define OB_WRP_STATE 			OB_WRPSTATE_ENABLE
#define OB_WRP_FLASH_BANK 		FLASH_BANK_1

/*
 * Enable defines in order i.e. Test 1 followed by Test 2
 * WARNING or NOTE: Flash sectors 0 and 1 contain the application code.
 	 	 So if those sectors are used in OB_WRP_SECTORS define, they will be write protected
 	 	 and you'll need to remove the WRP protection (e.g. using STM32CubeProgrammer) if you
 	 	 need to re-program the application.

 */
/* Un-comment only for TEST 1: Program sectors 2,3 and 5 */
#define OB_WRP_SECTORS 			(OB_WRP_SECTOR_2 | OB_WRP_SECTOR_3 | OB_WRP_SECTOR_5)
/* Un-comment only for TEST 2: Program sector 6) */
//#define OB_WRP_SECTORS (OB_WRP_SECTOR_6)

#endif /* WRP_ON */


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
static void MX_GPIO_Init(void);
static void MX_ICACHE_Init(void);

/* USER CODE BEGIN PFP */
#ifdef WRP_ON

HAL_StatusTypeDef set_WRP(uint32_t bank_num, uint32_t state);
static void WRP_Config_Error(void);

#endif
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
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */
#ifdef WRP_ON
  HAL_StatusTypeDef err;
  /* FLASH_BANK_1 and OB_WRPSTATE_ENABLE from stm32h7xx_hal_flash_ex.h file */
  /* OB_WRP_SECTORS defined in main.c */
  err = set_WRP(OB_WRP_FLASH_BANK, OB_WRP_STATE);

  if (err != HAL_OK)
  {
	  /* Toggle error indicator with USER_LED */
	  WRP_Config_Error();
  }
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
	  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Function to set OB (WRP)
  * @param  bank_num: Flash Bank Number
  * @param  state: WRP state enable/disable
  * @retval HAL_Status
  */
  HAL_StatusTypeDef set_WRP(uint32_t bank_num, uint32_t state)
{
	HAL_StatusTypeDef err;
	static FLASH_OBProgramInitTypeDef OB;


	OB.OptionType = OPTIONBYTE_WRP;
	OB.WRPState = state;
	OB.Banks = bank_num;

	/* Fetch all OB type configuration (we need WRP sectors)
	 * This step is to check if protection are already set
	 */
	HAL_FLASHEx_OBGetConfig(&OB);

	/* Retrieve WRPSector information from Option Bytes */
	unsigned int ob_wrp_sectors = OB.WRPSector;

	/* Check if user defined WRP sectors are pre-programmed */
	if ((ob_wrp_sectors & OB_WRP_SECTORS) == OB_WRP_SECTORS)
	{
		HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

		/* blink USER_LED (toggle ON & OFF) once */
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		HAL_Delay(500);
		HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
		HAL_Delay(500);

		return HAL_OK;
	}
	else
	{
		/* Check if certain sectors have been pre-programmed */
		if(ob_wrp_sectors & OB_WRP_SECTORS)
		{
			HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

			/* Toggle user-LED thrice and leave it ON */
			int max_toggle_count = 6;
			for(int i = 0; i < max_toggle_count; i++)
			{
				HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
				HAL_Delay(500);
			}
		}
		else
		{
			/* If Flash sectors are not pre-programmed, disable USER_LED and proceed to program WRP OB */
			HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);
		}

		OB.OptionType = OPTIONBYTE_WRP;
		OB.WRPState = state;
		OB.Banks = bank_num;
		OB.WRPSector = OB_WRP_SECTORS;	/* Program new sectors - Managed through #define above */

		err = HAL_FLASH_Unlock();
		err |= HAL_FLASH_OB_Unlock();
		err |=  HAL_FLASHEx_OBProgram(&OB);

		/* If all is okay, next command triggers a system reset with updated OB */
		if (err == HAL_OK)
		{
			HAL_FLASH_OB_Launch();
			return(HAL_OK);
		}
		else
		{
		  HAL_FLASH_OB_Lock();
		  HAL_FLASH_Lock();
		  return HAL_ERROR;
		}
	}
}

  /**
    * @brief  Error handler called on OB (WRP)configuration setting
    * @param  None
    * @retval None
    *         Continuous USER_LED toggle
    */
static void WRP_Config_Error(void)
{
	HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_SET);
  	while (1)
  	{
	  HAL_GPIO_TogglePin(USER_LED_GPIO_Port, USER_LED_Pin);
  	  HAL_Delay (100);
  	}
}


/* USER CODE END 4 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 250;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache (default 2-ways set associative cache)
  */
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : USER_LED_Pin */
  GPIO_InitStruct.Pin = USER_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USER_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USB_FS_OVCR_Pin */
  GPIO_InitStruct.Pin = USB_FS_OVCR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USB_FS_OVCR_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

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
