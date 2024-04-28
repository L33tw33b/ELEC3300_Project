/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "z_displ_ILI9XXX.h"
#include "z_touch_XPT2046.h"
#include "z_touch_XPT2046_menu.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void readEncoder(void);
void Button_Control();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CS_I2C_SPI_Pin GPIO_PIN_3
#define CS_I2C_SPI_GPIO_Port GPIOE
#define OTG_FS_Power_Pin GPIO_PIN_0
#define OTG_FS_Power_GPIO_Port GPIOC
#define TOUCH_MISO_Pin GPIO_PIN_2
#define TOUCH_MISO_GPIO_Port GPIOC
#define Button_IN_Pin GPIO_PIN_5
#define Button_IN_GPIO_Port GPIOA
#define Button_IN_EXTI_IRQn EXTI9_5_IRQn
#define Rotary_CLK_Pin GPIO_PIN_4
#define Rotary_CLK_GPIO_Port GPIOC
#define Rotary_CLK_EXTI_IRQn EXTI4_IRQn
#define Rotary_DT_Pin GPIO_PIN_5
#define Rotary_DT_GPIO_Port GPIOC
#define BOOT1_Pin GPIO_PIN_2
#define BOOT1_GPIO_Port GPIOB
#define TOUCH_INT_Pin GPIO_PIN_11
#define TOUCH_INT_GPIO_Port GPIOE
#define TOUCH_INT_EXTI_IRQn EXTI15_10_IRQn
#define DISPL_CS_Pin GPIO_PIN_12
#define DISPL_CS_GPIO_Port GPIOE
#define TOUCH_CS_Pin GPIO_PIN_13
#define TOUCH_CS_GPIO_Port GPIOE
#define DISPL_DC_Pin GPIO_PIN_14
#define DISPL_DC_GPIO_Port GPIOE
#define DISPL_RST_Pin GPIO_PIN_15
#define DISPL_RST_GPIO_Port GPIOE
#define DISPL_SCK_Pin GPIO_PIN_13
#define DISPL_SCK_GPIO_Port GPIOB
#define DISPL_LED_Pin GPIO_PIN_14
#define DISPL_LED_GPIO_Port GPIOB
#define DISPL_MOSI_Pin GPIO_PIN_15
#define DISPL_MOSI_GPIO_Port GPIOB
#define OTG_FS_OC_Pin GPIO_PIN_5
#define OTG_FS_OC_GPIO_Port GPIOD
#define Audio_SDL_Pin GPIO_PIN_6
#define Audio_SDL_GPIO_Port GPIOB
#define Audio_SDA_Pin GPIO_PIN_9
#define Audio_SDA_GPIO_Port GPIOB
#define MEMS_INT2_Pin GPIO_PIN_1
#define MEMS_INT2_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
