/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "usb_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <math.h>
#include "midi.h"
#include "synth.h"
#include "../../Drivers/USBH_midi_class/Inc/usbh_MIDI.h"

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
 CRC_HandleTypeDef hcrc;

I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_spi3_tx;

SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_spi2_tx;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
extern USBH_HandleTypeDef hUsbHostFS;
extern ApplicationTypeDef Appli_state;

#define PI 3.14159f



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S3_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_SPI2_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM3_Init(void);
void MX_USB_HOST_Process(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int CLKPrevious = 0;
int refreshLCD = 1;
volatile int menu_counter = 0;
volatile int menu2_counter = 0;
volatile int menu3_counter = 0;
volatile int Attack_counter = 0;
volatile int Decay_counter = 0;
volatile int Sustain_counter = 0;
volatile int Release_counter = 0;
volatile int Scale_counter = 0;
volatile int menu2_running = 0;
volatile int menu1_running = 1;
volatile int menu3_running = 0;
volatile int Attack_running = 0;
volatile int Decay_running = 0;
volatile int Sustain_running = 0;
volatile int Release_running = 0;
volatile int Scale_running = 0;
char str[16];
volatile int button_pressed = 0;
volatile int ATT_BACK = 0;
volatile int DEC_BACK = 0;
volatile int SUS_BACK = 0;
volatile int REL_BACK = 0;
volatile int SCL_BACK = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  ApplicationTypeDef last_Appli_state = Appli_state;
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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2S3_Init();
  MX_USART2_UART_Init();
  MX_USB_HOST_Init();
  MX_SPI2_Init();
  MX_CRC_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  synth_init();
  Displ_Init(Displ_Orientat_0);			// initialize display controller - set orientation parameter as per your needs
  Displ_CLS(BLACK);						// clear the screen - BLACK or any other color you prefer
  Displ_BackLight('I');  					// initialize backlight
  InitMenu();





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



    /* USER CODE END WHILE */
    MX_USB_HOST_Process();

    /* USER CODE BEGIN 3 */


    if(last_Appli_state != Appli_state) {
      last_Appli_state = Appli_state;

      if(Appli_state == APPLICATION_READY) {
        start_midi(); // Initialize midi controller
        callDrawMenu(1);

      }

    }
    if(last_Appli_state != APPLICATION_READY) {
    	continue;
    	// Don't do RunMenu1() when Appli_state != APPLICATION_READY
    }

    if((refreshLCD==1||button_pressed==1)&&menu1_running == 1){
                	RunMenu1(menu_counter, button_pressed);
                	button_pressed = 0;
                	refreshLCD = 0;
            }
            else if((refreshLCD==1||button_pressed==1)&&menu2_running == 1){
            	RunMenu2(menu2_counter, button_pressed);
            	button_pressed = 0;
            	refreshLCD = 0;
            }
            else if((refreshLCD==1||button_pressed==1)&&menu3_running == 1){


                       	RunMenu3(menu3_counter, button_pressed);
                       	button_pressed = 0;
                       	refreshLCD = 0;
            }

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S3_Init(void)
{

  /* USER CODE BEGIN I2S3_Init 0 */

  /* USER CODE END I2S3_Init 0 */

  /* USER CODE BEGIN I2S3_Init 1 */

  /* USER CODE END I2S3_Init 1 */
  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_8K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S3_Init 2 */

  /* USER CODE END I2S3_Init 2 */

}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 10000;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|DISPL_DC_Pin|DISPL_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_Power_GPIO_Port, OTG_FS_Power_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, DISPL_CS_Pin|TOUCH_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(DISPL_LED_GPIO_Port, DISPL_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : CS_I2C_SPI_Pin DISPL_RST_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|DISPL_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_Power_Pin */
  GPIO_InitStruct.Pin = OTG_FS_Power_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_Power_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Button_IN_Pin */
  GPIO_InitStruct.Pin = Button_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Button_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Rotary_CLK_Pin */
  GPIO_InitStruct.Pin = Rotary_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Rotary_CLK_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Rotary_DT_Pin */
  GPIO_InitStruct.Pin = Rotary_DT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Rotary_DT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TOUCH_INT_Pin */
  GPIO_InitStruct.Pin = TOUCH_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(TOUCH_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISPL_CS_Pin TOUCH_CS_Pin DISPL_DC_Pin */
  GPIO_InitStruct.Pin = DISPL_CS_Pin|TOUCH_CS_Pin|DISPL_DC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : DISPL_LED_Pin */
  GPIO_InitStruct.Pin = DISPL_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DISPL_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15
                           PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_OC_Pin */
  GPIO_InitStruct.Pin = OTG_FS_OC_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(OTG_FS_OC_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MEMS_INT2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void readEncoder()
{
if(menu1_running ==1){
	if(Scale_running==0){
		int CLKNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4); // Read the state of the CLK pin
		if (CLKNow != CLKPrevious)
		{
			int DTNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5); // Read the state of the DT pin
			if (DTNow != CLKNow) // If DT state is different from CLK state, encoder is rotating in A direction
			{
				if (menu_counter < 2)
					menu_counter++;
				else
					menu_counter = 0;
			}
			else // Encoder is rotating in B direction
			{
				if (menu_counter < 1)
					menu_counter = 2;
				else
					menu_counter--;
			}
			refreshLCD = 1;
		}
		CLKPrevious = CLKNow; // Store last state of CLK
	}
	else if(Scale_running==1){
		int CLKNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4); // Read the state of the CLK pin
				if (CLKNow != CLKPrevious)
				{
					int DTNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5); // Read the state of the DT pin
					if (DTNow != CLKNow) // If DT state is different from CLK state, encoder is rotating in A direction
					{
						if (Scale_counter < 10)
							Scale_counter++;
						else
							Scale_counter = 0;
					}
					else // Encoder is rotating in B direction
					{
						if (Scale_counter < 1)
							Scale_counter = 10;
						else
							Scale_counter--;
					}
					refreshLCD = 1;
				}
				CLKPrevious = CLKNow; // Store last state of CLK
	}
}

else if (menu2_running ==1){
	int CLKNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4); // Read the state of the CLK pin
	    if (CLKNow != CLKPrevious)
	    {
	        int DTNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5); // Read the state of the DT pin
	        if (DTNow != CLKNow) // If DT state is different from CLK state, encoder is rotating in A direction
	        {
	            if (menu2_counter < 4)
	                menu2_counter++;
	            else
	                menu2_counter = 0;
	        }
	        else // Encoder is rotating in B direction
	        {
	            if (menu2_counter < 1)
	                menu2_counter = 4;
	            else
	                menu2_counter--;
	        }
	        refreshLCD = 1;
	    }
	    CLKPrevious = CLKNow; // Store last state of CLK
}
else if (menu3_running ==1){
	if(Attack_running == 0 && Decay_running == 0 && Sustain_running == 0 && Release_running == 0){
			int CLKNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4); // Read the state of the CLK pin
	    	if (CLKNow != CLKPrevious)
	    	{
	    		int DTNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5); // Read the state of the DT pin
	    		if (DTNow != CLKNow) // If DT state is different from CLK state, encoder is rotating in A direction
	    		{
	    			if (menu3_counter < 4)
	    				menu3_counter++;
	    			else
	    				menu3_counter = 0;
	    		}
	    		else // Encoder is rotating in B direction
	    		{
	    			if (menu3_counter < 1)
	    				menu3_counter = 4;
	    			else
	    				menu3_counter--;
	    		}
	    		refreshLCD = 1;
	    	}
	    CLKPrevious = CLKNow; // Store last state of CLK
	}
	else if(Attack_running == 1){
		int CLKNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4); // Read the state of the CLK pin
			    	if (CLKNow != CLKPrevious)
			    	{
			    		int DTNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5); // Read the state of the DT pin
			    		if (DTNow != CLKNow) // If DT state is different from CLK state, encoder is rotating in A direction
			    		{
			    			if (Attack_counter < 10)
			    				Attack_counter++;
			    			else
			    				Attack_counter = 0;
			    		}
			    		else // Encoder is rotating in B direction
			    		{
			    			if (Attack_counter < 1)
			    				Attack_counter = 10;
			    			else
			    				Attack_counter--;
			    		}
			    		refreshLCD = 1;
			    	}
			    CLKPrevious = CLKNow; // Store last state of CLK
	}
	else if(Decay_running == 1){
		int CLKNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4); // Read the state of the CLK pin
					    	if (CLKNow != CLKPrevious)
					    	{
					    		int DTNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5); // Read the state of the DT pin
					    		if (DTNow != CLKNow) // If DT state is different from CLK state, encoder is rotating in A direction
					    		{
					    			if (Decay_counter < 10)
					    				Decay_counter++;
					    			else
					    				Decay_counter = 0;
					    		}
					    		else // Encoder is rotating in B direction
					    		{
					    			if (Decay_counter < 1)
					    				Decay_counter = 10;
					    			else
					    				Decay_counter--;
					    		}
					    		refreshLCD = 1;
					    	}
					    CLKPrevious = CLKNow; // Store last state of CLK
	}
	else if(Sustain_running == 1){
		int CLKNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4); // Read the state of the CLK pin
					    	if (CLKNow != CLKPrevious)
					    	{
					    		int DTNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5); // Read the state of the DT pin
					    		if (DTNow != CLKNow) // If DT state is different from CLK state, encoder is rotating in A direction
					    		{
					    			if (Sustain_counter < 10)
					    				Sustain_counter++;
					    			else
					    				Sustain_counter = 0;
					    		}
					    		else // Encoder is rotating in B direction
					    		{
					    			if (Sustain_counter < 1)
					    				Sustain_counter = 10;
					    			else
					    				Sustain_counter--;
					    		}
					    		refreshLCD = 1;
					    	}
					    CLKPrevious = CLKNow; // Store last state of CLK
	}
	else if(Release_running == 1){
		int CLKNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_4); // Read the state of the CLK pin
							    	if (CLKNow != CLKPrevious)
							    	{
							    		int DTNow = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_5); // Read the state of the DT pin
							    		if (DTNow != CLKNow) // If DT state is different from CLK state, encoder is rotating in A direction
							    		{
							    			if (Release_counter < 10)
							    				Release_counter++;
							    			else
							    				Release_counter = 0;
							    		}
							    		else // Encoder is rotating in B direction
							    		{
							    			if (Release_counter < 1)
							    				Release_counter = 10;
							    			else
							    				Release_counter--;
							    		}
							    		refreshLCD = 1;
							    	}
							    CLKPrevious = CLKNow; // Store last state of CLK
	}
}
}

void Button_Control(){
	button_pressed = 1;
	if(Attack_running ==1){
		ATT_BACK = 1;
	}
	if(Decay_running ==1){
			DEC_BACK = 1;
	}
	if(Sustain_running ==1){
			SUS_BACK = 1;
	}
	if(Release_running ==1){
			REL_BACK = 1;
	}
	if(Scale_running ==1){
			SCL_BACK = 1;
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
