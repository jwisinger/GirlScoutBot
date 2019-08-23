/**
  ******************************************************************************
  * @file  : custom.c
  * @brief : Source file for the BSP Common driver
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

/* Includes ------------------------------------------------------------------*/ 
#include "stm32f1_custom.h"

#include "stm32f1xx_hal_exti.h"

/** @defgroup BSP BSP
 * @{
 */ 

/** @defgroup CUSTOM CUSTOM
 * @{
 */   
    
/** @defgroup CUSTOM_LOW_LEVEL CUSTOM LOW LEVEL
 *  @brief This file provides set of firmware functions to manage Leds and push-button
 *         available on STM32F1xx-Nucleo Kit from STMicroelectronics.
 * @{
 */ 

/**
 * @}
 */ 

/** @defgroup CUSTOM_LOW_LEVEL_Private_Defines CUSTOM LOW LEVEL Private Defines
 * @{
 */ 

/**
 * @brief STM32F1XX NUCLEO BSP Driver version number V1.2.6
 */  
#define __CUSTOM_BSP_VERSION_MAIN   (0x01) /*!< [31:24] main version */
#define __CUSTOM_BSP_VERSION_SUB1   (0x02) /*!< [23:16] sub1 version */
#define __CUSTOM_BSP_VERSION_SUB2   (0x06) /*!< [15:8]  sub2 version */
#define __CUSTOM_BSP_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __CUSTOM_BSP_VERSION        ((__CUSTOM_BSP_VERSION_MAIN << 24)\
                                                    |(__CUSTOM_BSP_VERSION_SUB1 << 16)\
                                                    |(__CUSTOM_BSP_VERSION_SUB2 << 8 )\
                                                    |(__CUSTOM_BSP_VERSION_RC))
/**
 * @}
 */ 

/** @defgroup CUSTOM_LOW_LEVEL_FunctionPrototypes CUSTOM LOW LEVEL Private Function Prototypes
 * @{
 */
typedef void (* BSP_EXTI_LineCallback) (void);

/**
 * @}
 */ 

/** @defgroup CUSTOM_LOW_LEVEL_Private_Variables CUSTOM LOW LEVEL Private Variables
 * @{
 */
static GPIO_TypeDef*  LED_PORT[LEDn] = {LED2_GPIO_PORT};
static const uint16_t LED_PIN[LEDn]  = {LED2_PIN};
static GPIO_TypeDef*   BUTTON_PORT[BUTTONn] = {KEY_BUTTON_GPIO_PORT}; 
static const uint16_t  BUTTON_PIN[BUTTONn]  = {KEY_BUTTON_PIN}; 
static const IRQn_Type BUTTON_IRQn[BUTTONn] = {KEY_BUTTON_EXTI_IRQn};
EXTI_HandleTypeDef* hExtiButtonHandle[BUTTONn];
EXTI_HandleTypeDef hexti0 = {.Line = EXTI_LINE_0};
/**
 * @}
 */ 

/** @defgroup CUSTOM_LOW_LEVEL_Private_Functions CUSTOM LOW LEVEL Private Functions
 * @{
 */ 
static void BUTTON_KEY_EXTI_Callback(void);
/**
 * @brief  This method returns the STM32F1xx NUCLEO BSP Driver revision
 * @retval version: 0xXYZR (8bits for each decimal, R for RC)
 */
int32_t BSP_GetVersion(void)
{
  return (int32_t)__CUSTOM_BSP_VERSION;
}

/**
 * @brief  Configures LED on GPIO and/or on MFX.
 * @param  Led: LED to be configured. 
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @retval HAL status
 */
int32_t BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* LED2 is on the same GPIO Port */
  LED2_GPIO_CLK_ENABLE();    
  
  /* Configure the GPIO_LED pin */
  GPIO_InitStruct.Pin   = LED_PIN [Led];
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
  
  HAL_GPIO_Init(LED_PORT [Led], &GPIO_InitStruct); 
  HAL_GPIO_WritePin(LED_PORT [Led], LED_PIN [Led], GPIO_PIN_RESET);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  DeInit LEDs.
 * @param  Led: LED to be configured. 
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @note Led DeInit does not disable the GPIO clock nor disable the Mfx 
 * @retval HAL status
 */
int32_t BSP_LED_DeInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /* Turn off LED */ 
  HAL_GPIO_WritePin(LED_PORT [Led], (uint16_t)LED_PIN[Led], GPIO_PIN_RESET);
  
  /* DeInit the GPIO_LED pin */ 
  GPIO_InitStruct.Pin = LED_PIN [Led];    
  HAL_GPIO_DeInit(LED_PORT [Led], GPIO_InitStruct.Pin);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Turns selected LED On.
 * @param  Led: LED to be set on 
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @retval HAL status
 */
int32_t BSP_LED_On(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT [Led], LED_PIN [Led], GPIO_PIN_SET);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Turns selected LED Off. 
 * @param  Led: LED to be set off
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @retval HAL status
 */
int32_t BSP_LED_Off(Led_TypeDef Led)
{
  HAL_GPIO_WritePin(LED_PORT [Led], LED_PIN [Led], GPIO_PIN_RESET);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Toggles the selected LED.
 * @param  Led: LED to be toggled
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @retval HAL status
 */
int32_t BSP_LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], (uint16_t)LED_PIN[Led]);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Get the status of the LED.
 * @param  Led: LED for which get the status
 *              This parameter can be one of the following values:
 *              @arg  LED1
 *              @arg  LED2
 *              @arg  LED3
 *              @arg  LED4
 * @retval HAL status (1=high, 0=low)
 */
int32_t BSP_LED_GetState(Led_TypeDef Led) 
{ 
  return (int32_t)HAL_GPIO_ReadPin(LED_PORT [Led], LED_PIN [Led]); 
}
/**
  * @brief  Configures button GPIO and EXTI Line.
  * @param  Button: Button to be configured
  *                 This parameter can be one of the following values: 
  *                 @arg  BUTTON_KEY: Key Push Button
  * @param  ButtonMode Button mode
  *                    This parameter can be one of the following values:
  *                    @arg  BUTTON_MODE_GPIO: Button will be used as simple IO
  *                    @arg  BUTTON_MODE_EXTI: Button will be connected to EXTI line 
  *                                            with interrupt generation capability
  * @retval BSP status
  */
int32_t BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef ButtonMode)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  hExtiButtonHandle[Button] = &hexti0;
  
  static BSP_EXTI_LineCallback ButtonCallback[BUTTONn] ={BUTTON_KEY_EXTI_Callback};                                                
  static uint32_t  BSP_BUTTON_PRIO [BUTTONn] ={BSP_BUTTON_KEY_IT_PRIORITY};  											     
  static const uint32_t BUTTON_EXTI_LINE[BUTTONn] ={KEY_BUTTON_EXTI_LINE};
  
  /* Enable the BUTTON clock*/ 
  KEY_BUTTON_GPIO_CLK_ENABLE();
  GPIO_InitStruct.Pin = BUTTON_PIN [Button];
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  
  if(ButtonMode == BUTTON_MODE_GPIO)
  {
    /* Configure Button pin as input */
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;    
    HAL_GPIO_Init(BUTTON_PORT [Button], &GPIO_InitStruct);
  }
  else /* (ButtonMode == BUTTON_MODE_EXTI) */
  {      
    /* Configure Button pin as input with External interrupt */    
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; 
    
    HAL_GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStruct);
    
    (void)HAL_EXTI_GetHandle(hExtiButtonHandle[Button], BUTTON_EXTI_LINE[Button]);  
    (void)HAL_EXTI_RegisterCallback(hExtiButtonHandle[Button],  HAL_EXTI_COMMON_CB_ID, ButtonCallback[Button]);
      
    /* Enable and set Button EXTI Interrupt to the lowest priority */
    HAL_NVIC_SetPriority((BUTTON_IRQn[Button]), BSP_BUTTON_PRIO[Button], 0x00);
    HAL_NVIC_EnableIRQ((BUTTON_IRQn[Button]));
  }
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Push Button DeInit.
 * @param  Button Button to be configured
 *                This parameter can be one of the following values:
 *                @arg  BUTTON_KEY: Key Push Button
 * @note PB DeInit does not disable the GPIO clock
 * @retval BSP status
 */
int32_t BSP_PB_DeInit(Button_TypeDef Button)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  GPIO_InitStruct.Pin = BUTTON_PIN[Button];
  HAL_NVIC_DisableIRQ((IRQn_Type)(BUTTON_IRQn[Button]));
  HAL_GPIO_DeInit(BUTTON_PORT[Button], GPIO_InitStruct.Pin);
  
  return BSP_ERROR_NONE;
}

/**
 * @brief  Returns the selected button state.
 * @param  Button Button to be checked
 *                This parameter can be one of the following values:
 *                @arg  BUTTON_KEY: Key Push Button
 * @retval The Button GPIO pin value
 */
int32_t BSP_PB_GetState(Button_TypeDef Button)
{
  return (int32_t)(HAL_GPIO_ReadPin(BUTTON_PORT[Button], BUTTON_PIN[Button]));
}

/**
 * @brief  Key EXTI line detection callbacks.
 * @retval None
 */
void BSP_PB_IRQHandler (Button_TypeDef Button)
{
  HAL_EXTI_IRQHandler( hExtiButtonHandle[Button] );   
}

/**
 * @brief  BSP Push Button callback
 * @param  Button Specifies the pin connected EXTI line
 * @retval None.
 */
__weak void BSP_PB_Callback(Button_TypeDef Button)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(Button);
  
  /* This function should be implemented by the user application.
     It is called into this driver when an event on Button is triggered. */   
}

/**
  * @brief  Key EXTI line detection callbacks.
  * @retval None
  */
static void BUTTON_KEY_EXTI_Callback(void)
{   
  BSP_PB_Callback(BUTTON_KEY);
}

/**
 * @}
 */ 

/**
 * @}
 */

/**
 * @}
 */    

/**
 * @}
 */  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

