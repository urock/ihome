/**
  ******************************************************************************
  * @file    stm322xg_eval.c
  * @author  MCD Application Team
  * @version V5.0.0
  * @date    07-October-2011
  * @brief   This file provides
  *            - set of firmware functions to manage Leds, push-button and COM ports
  *            - low level initialization functions for SD card (on SDIO) and
  *              serial EEPROM (sEE)
  *          available on STM322xG-EVAL evaluation board(MB786) RevA and RevB 
  *          from STMicroelectronics.
  *
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "stm322xg_eval.h"
#include "stm32f2xx_sdio.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_i2c.h"

//GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT,
//                                 LED4_GPIO_PORT};
//const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN,
//                                 LED4_PIN};
//const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK,
//                                 LED4_GPIO_CLK};
//
//USART_TypeDef* COM_USART[COMn] = {EVAL_COM1}; 
//
//GPIO_TypeDef* COM_TX_PORT[COMn] = {EVAL_COM1_TX_GPIO_PORT};
// 
//GPIO_TypeDef* COM_RX_PORT[COMn] = {EVAL_COM1_RX_GPIO_PORT};
//
////const uint32_t COM_USART_CLK[COMn] = {EVAL_COM1_CLK};
//
//const uint32_t COM_TX_PORT_CLK[COMn] = {EVAL_COM1_TX_GPIO_CLK};
// 
//const uint32_t COM_RX_PORT_CLK[COMn] = {EVAL_COM1_RX_GPIO_CLK};
//
//const uint16_t COM_TX_PIN[COMn] = {EVAL_COM1_TX_PIN};
//
//const uint16_t COM_RX_PIN[COMn] = {EVAL_COM1_RX_PIN};
// 
//const uint8_t COM_TX_PIN_SOURCE[COMn] = {EVAL_COM1_TX_SOURCE};
//
//const uint8_t COM_RX_PIN_SOURCE[COMn] = {EVAL_COM1_RX_SOURCE};
// 
//const uint8_t COM_TX_AF[COMn] = {EVAL_COM1_TX_AF};
// 
//const uint8_t COM_RX_AF[COMn] = {EVAL_COM1_RX_AF};
//
//DMA_InitTypeDef    sEEDMA_InitStructure; 
//NVIC_InitTypeDef   NVIC_InitStructure;
//
//
///**
//  * @brief  Configures LED GPIO.
//  * @param  Led: Specifies the Led to be configured. 
//  *   This parameter can be one of following parameters:
//  *     @arg LED1
//  *     @arg LED2
//  *     @arg LED3
//  *     @arg LED4
//  * @retval None
//  */
//void LEDInit(Led_TypeDef Led)
//{
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  
//  /* Enable the GPIO_LED Clock */
//  RCC_AHB1PeriphClockCmd(GPIO_CLK[Led], ENABLE);
//
//
//  /* Configure the GPIO_LED pin */
//  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
//}
////----------------------------
///**
//* @brief Init LEDS alarm
//*/
//void LED_alarm_Init(void)
//{
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  
//  /* Enable the GPIO_LED Clock */
//  RCC_AHB1PeriphClockCmd(LED_alerm_CLK, ENABLE);
//
//
//  /* Configure the GPIO_LED pin */
//  GPIO_InitStructure.GPIO_Pin = LED_alarm;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(LED_alarm_PORT, &GPIO_InitStructure);
//}
////----------------------------------------------
///**
//* @brief Init LEDS alarm
//*/
//void LED_warning_Init(void)
//{
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  
//  /* Enable the GPIO_LED Clock */
//  RCC_AHB1PeriphClockCmd(LED_warning_CLK, ENABLE);
//
//
//  /* Configure the GPIO_LED pin */
//  GPIO_InitStructure.GPIO_Pin = LED_warning;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(LED_warning_PORT, &GPIO_InitStructure);
//}
////----------------------------------------------
///**
//* @brief Init LEDS status
//*/
//void LED_status_Init(void)
//{
//  GPIO_InitTypeDef  GPIO_InitStructure;
//  
//  /* Enable the GPIO_LED Clock */
//  RCC_AHB1PeriphClockCmd(LED_status_CLK , ENABLE);
//
//
//  /* Configure the GPIO_LED pin */
//  GPIO_InitStructure.GPIO_Pin = LED_status;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(LED_status_PORT, &GPIO_InitStructure);
//}
////----------------------------------------------
//
///**
//  * @brief  Turns selected LED On.
//  * @param  Led: Specifies the Led to be set on. 
//  *   This parameter can be one of following parameters:
//  *     @arg LED1
//  *     @arg LED2
//  *     @arg LED3
//  *     @arg LED4  
//  * @retval None
//  */
//void LEDOn(Led_TypeDef Led)
//{
//  GPIO_PORT[Led]->BSRRL = GPIO_PIN[Led];
//}
//
////------------------------------------
///**
//* @bief LED alarm On
//*/
//void LED_alarm_On(void)
//{
//  LED_alarm_PORT->BSRRL = LED_alarm ;
//}
////------------------------------------
///**
//* @bief LED warning On
//*/
//void LED_warning_On(void)
//{
//  LED_warning_PORT->BSRRL = LED_warning ;
//}
////------------------------------------
///**
//* @bief LED status On
//*/
//void LED_status_On(void)
//{
//  LED_status_PORT->BSRRL = LED_status ;
//}
////--------------------------------------
///**
//  * @brief  Turns selected LED Off.
//  * @param  Led: Specifies the Led to be set off. 
//  *   This parameter can be one of following parameters:
//  *     @arg LED1
//  *     @arg LED2
//  *     @arg LED3
//  *     @arg LED4 
//  * @retval None
//  */
//void LEDOff(Led_TypeDef Led)
//{
//  GPIO_PORT[Led]->BSRRH = GPIO_PIN[Led];  
//}
////-------------------------------------------
///**
//* @brief LED alarm Off
//*/
//void LED_alarm_Off(void)
//{
//  LED_alarm_PORT->BSRRH = LED_alarm ;  
//}
////-------------------------------------------
///**
//* @brief LED warning Off
//*/
//void LED_warning_Off(void)
//{
//  LED_warning_PORT->BSRRH = LED_warning ;  
//}
////-------------------------------------------
///**
//* @brief LED status Off
//*/
//void LED_status_Off(void)
//{
//  LED_status_PORT->BSRRH = LED_status ;  
//}
////--------------------------------------------
///**
//  * @brief  Toggles the selected LED.
//  * @param  Led: Specifies the Led to be toggled. 
//  *   This parameter can be one of following parameters:
//  *     @arg LED1
//  *     @arg LED2
//  *     @arg LED3
//  *     @arg LED4  
//  * @retval None
//  */
//void LEDToggle(Led_TypeDef Led)
//{
//  GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
//}
////-------------------------------------------------
///**
//* @brief Toggle LED alarm 
//*/
//void LED_alarm_Toggle(void)
//{
//  LED_alarm_PORT->ODR ^= LED_alarm ;
//}
////----------------------------------------------------
///**
//* @brief Toggle LED warning
//*/
//void LED_warning_Toggle(void)
//{
//  LED_warning_PORT->ODR ^= LED_warning;
//}
////----------------------------------------------------
///**
//* @brief Toggle LED status
//*/
//void LED_status_Toggle(void)
//{
//  LED_status_PORT->ODR ^= LED_status;
//}
//
//
///**
//  * @brief  Configures COM port.
//  * @param  COM: Specifies the COM port to be configured.
//  *   This parameter can be one of following parameters:    
//  *     @arg COM1
//  *     @arg COM2  
//  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
//  *   contains the configuration information for the specified USART peripheral.
//  * @retval None
//  */
//void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
//{
//  GPIO_InitTypeDef GPIO_InitStructure;
//
//  /* Enable GPIO clock */
//  RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);
//  /* Connect PXx to USARTx_Tx*/
//  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);
//
//  /* Connect PXx to USARTx_Rx*/
//  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);
//
//  /* Configure USART Tx as alternate function  */
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//
//  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);
//
//  /* Configure USART Rx as alternate function  */
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
//  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);
//
//  /* USART configuration */
//  USART_Init(COM_USART[COM], USART_InitStruct);
//    
//  /* Enable USART */
//  USART_Cmd(COM_USART[COM], ENABLE);
//}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
