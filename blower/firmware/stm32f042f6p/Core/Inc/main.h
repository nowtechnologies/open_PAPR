/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define STARTUP_MODE_SELECT_Pin GPIO_PIN_0
#define STARTUP_MODE_SELECT_GPIO_Port GPIOF
#define BEEPER_Pin GPIO_PIN_1
#define BEEPER_GPIO_Port GPIOF
#define MODE_LED_Pin GPIO_PIN_4
#define MODE_LED_GPIO_Port GPIOA
#define ADC_BATTERY_Pin GPIO_PIN_5
#define ADC_BATTERY_GPIO_Port GPIOA
#define ADC_USER_POT_Pin GPIO_PIN_6
#define ADC_USER_POT_GPIO_Port GPIOA
#define ADC_MAX_POT_Pin GPIO_PIN_7
#define ADC_MAX_POT_GPIO_Port GPIOA
#define ADC_RATIO_POT_Pin GPIO_PIN_1
#define ADC_RATIO_POT_GPIO_Port GPIOB
#define PWM_MOTOR_IN_Pin GPIO_PIN_9
#define PWM_MOTOR_IN_GPIO_Port GPIOA
#define PWM_MOTOR_OUT_Pin GPIO_PIN_10
#define PWM_MOTOR_OUT_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/