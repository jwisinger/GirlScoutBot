/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#undef USE_BSP_COM_FEATURE
#undef USE_COM_LOG
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define ENTER_RTN_Pin GPIO_PIN_0
#define ENTER_RTN_GPIO_Port GPIOA
#define ENTER_Pin GPIO_PIN_2
#define ENTER_GPIO_Port GPIOA
#define ENTER_EXTI_IRQn EXTI2_IRQn
#define UP_Pin GPIO_PIN_3
#define UP_GPIO_Port GPIOA
#define UP_EXTI_IRQn EXTI3_IRQn
#define UP_RTN_Pin GPIO_PIN_5
#define UP_RTN_GPIO_Port GPIOA
#define RIGHT_RTN_Pin GPIO_PIN_6
#define RIGHT_RTN_GPIO_Port GPIOA
#define RIGHT_Pin GPIO_PIN_0
#define RIGHT_GPIO_Port GPIOB
#define RIGHT_EXTI_IRQn EXTI0_IRQn
#define LEFT_RET_Pin GPIO_PIN_1
#define LEFT_RET_GPIO_Port GPIOB
#define LEFT_Pin GPIO_PIN_11
#define LEFT_GPIO_Port GPIOB
#define LEFT_EXTI_IRQn EXTI15_10_IRQn
#define RADIO_INT_Pin GPIO_PIN_8
#define RADIO_INT_GPIO_Port GPIOA
#define RADIO_INT_EXTI_IRQn EXTI9_5_IRQn
#define RADIO_CS_Pin GPIO_PIN_9
#define RADIO_CS_GPIO_Port GPIOA
#define RADIO_RESET_Pin GPIO_PIN_10
#define RADIO_RESET_GPIO_Port GPIOA
#define RIGHT_MOTOR_Pin GPIO_PIN_5
#define RIGHT_MOTOR_GPIO_Port GPIOB
#define LEFT_MOTOR_Pin GPIO_PIN_7
#define LEFT_MOTOR_GPIO_Port GPIOB
#define DOWN_Pin GPIO_PIN_9
#define DOWN_GPIO_Port GPIOB
#define DOWN_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */
extern osTimerId singleShotTimerHandle;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
