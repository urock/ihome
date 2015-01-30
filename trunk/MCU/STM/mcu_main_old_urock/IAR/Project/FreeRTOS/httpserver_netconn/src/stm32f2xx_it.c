/**
  ******************************************************************************
  * @file    stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
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


#include "stm32f2xx_it.h"
#include "main.h"
#include "stm32f2x7_eth.h"



/* Scheduler includes */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* lwip includes */
#include "lwip/sys.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern xSemaphoreHandle s_xSemaphore;
extern volatile xSemaphoreHandle xSemaphore_inputs[16];
/* Private function prototypes -----------------------------------------------*/
extern void xPortSysTickHandler(void); 
/* Private functions ---------------------------------------------------------*/
extern volatile struct PORT_struct PORTs[40];
//volatile 
//struct sendText_t send2host_cmd;

 /******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
//  portCHAR PAGE_BODY[1024];
//  printf("\n\n");
//
//  strcat((char *)PAGE_BODY, "Name          State  Priority  Stack   Num\n" );
//  strcat((char *)PAGE_BODY, "---------------------------------------------\n");
//  
//    
//  /* The list of tasks and their status */
//  vTaskList((signed char *)(PAGE_BODY + strlen(PAGE_BODY)));
//  printf("\n%s\n", "task name status priority HWMTS ID");
//  strcat((char *)PAGE_BODY, "---------------------------------------------\n");
//  strcat((char *)PAGE_BODY, "B : Blocked, R : Ready, D : Deleted, S : Suspended\n\n");
//   
//  printf ("%s\n", PAGE_BODY);    
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  xPortSysTickHandler(); 
}

/**
  * @brief  This function handles External line 10 interrupt request.
  * @param  None
  * @retval None
  */

void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(ETH_LINK_EXTI_LINE) != RESET)
  {
    Eth_Link_ITHandler(DP83848_PHY_ADDRESS);
    /* Clear interrupt pending bit */
    EXTI_ClearITPendingBit(ETH_LINK_EXTI_LINE);
  }
//  ------------------------------------------------------
//  if(EXTI_GetITStatus(EXTI_Line12) != RESET)    
//  {
//    if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_12)== RESET){
//      // Port7 
//       LED_alarm_Toggle();
//    }
//    else if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)== RESET){
//      // Port5      
//      LED_warning_Toggle();
//    }    
//    /* Clear the EXTI line 3 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line12);
//  }
////--------------------------------------------------------
//  if(EXTI_GetITStatus(EXTI_Line11) != RESET)    
//  {
//    if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)== RESET){
//      // Port7 
//       LED_alarm_Toggle();
//    }
//    else if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)== RESET){
//      // Port5      
//      LED_warning_Toggle();
//    }    
//    /* Clear the EXTI line 3 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line11);
//  }
//  
  
//------------------  
//  if(EXTI_GetITStatus(EXTI_Line11) != RESET)  
//  {  
//
//    EXTI_ClearITPendingBit(EXTI_Line11);
//  }
////------------------
//  if(EXTI_GetITStatus(EXTI_Line12) != RESET)  
//  {  
//
//    EXTI_ClearITPendingBit(EXTI_Line12);
//  }
//------------------
//  if(EXTI_GetITStatus(EXTI_Line13) != RESET)  //Button temper
//  {  
//    LEDOff(LED3); 
//    EXTI_ClearITPendingBit(EXTI_Line13);
//  }
//------------------  
//  if(EXTI_GetITStatus(EXTI_Line14) != RESET)  
//  {  
//
//    EXTI_ClearITPendingBit(EXTI_Line14);
//  }
//------------------  
//  if(EXTI_GetITStatus(EXTI_Line15) != RESET) //Button key  допустим что это порт 0
//  {      
//  }    
}

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{ 
//  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
//  {
//    
//    /* Clear the EXTI line 0 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line0);
//  }
}

/**
  * @brief  This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */

void EXTI1_IRQHandler(void)
{
//  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
//  {
//    
//    /* Clear the EXTI line 1 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line1);
//  }
}

/**
  * @brief  This function handles External line 2 interrupt request.
  * @param  None
  * @retval None
  */

void EXTI2_IRQHandler(void)
{ 
//  if(EXTI_GetITStatus(EXTI_Line2) != RESET)
//  {
//    
//    /* Clear the EXTI line 2 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line2);
//  }
}

/**
  * @brief  This function handles External line 3 interrupt request.
  * @param  None
  * @retval None
  */

void EXTI3_IRQHandler(void)
{
//  if(EXTI_GetITStatus(EXTI_Line3) != RESET)    
//  {
//    if (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)== RESET){
//      // Port7 
//      LED_alarm_Toggle();
//    }
//    else if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)== RESET){
//      // Port5      
//      LED_warning_Toggle();
//    }    
//    /* Clear the EXTI line 3 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line3);
//  }
}

/**
  * @brief  This function handles External line 4 interrupt request.
  * @param  None
  * @retval None
  */

void EXTI4_IRQHandler(void)
{
// 
//  if(EXTI_GetITStatus(EXTI_Line4) != RESET)
//  {
//    
//    /* Clear the EXTI line 4 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line4);
//  }
}

void EXTI9_5_IRQHandler(void)
{  
//  if(EXTI_GetITStatus(Port19_EXTI_LINE) != RESET)
//  {
//    static portBASE_TYPE xHigherPriorityTaskWoken;
//
//    xHigherPriorityTaskWoken = pdFALSE;
//    
//    xSemaphoreGiveFromISR( xSemaphore_inputs[7], &xHigherPriorityTaskWoken );
//    if( xHigherPriorityTaskWoken == pdTRUE )
//    {
//     /* Выдача семафора разблокировала задачу, и приоритет разблокированной
//        задачи выше, чем у текущей задачи - принудительно переключаем
//        контекст выполнения, чтобы обеспечить возврат из ISR напрямую
//        в разблокированную (которая имеет самый высокий приоритет) задачу.
//        ВНИМАНИЕ: макрос, реально используемый для переключения контекста
//        из ISR, зависит от конкретного порта FreeRTOS. Здесь указано имя
//        макроса, корректное для порта Open Watcom DOS. Другие порты FreeRTOS
//        могут использовать другой синтаксис. Для определения используемого
//        синтаксиса обратитесь к примерам, предоставленным вместе с портом
//        FreeRTOS. */
////        my_tasks_vReceiveButton_init ((void*)&PORTs[8 + MAX_PORTs_in_shift]);  //!!!!!!!!!!!!
//        taskYIELD();     
//        EXTI_ClearITPendingBit(Port19_EXTI_LINE);  
//    }
//  }  
//  
  
//  if(EXTI_GetITStatus(Port18_EXTI_LINE) != RESET)
//  {
//    LED_warning_Toggle();
//    EXTI_ClearITPendingBit(Port18_EXTI_LINE);  
//  }  
//
//  if(EXTI_GetITStatus(Port17_EXTI_LINE) != RESET)
//  {
//    LED_warning_Toggle();
//    EXTI_ClearITPendingBit(Port17_EXTI_LINE);  
//  }  
//  
//  if(EXTI_GetITStatus(Port16_EXTI_LINE) != RESET)
//  {
//    LED_warning_Toggle();
//    EXTI_ClearITPendingBit(Port16_EXTI_LINE);  
//  }     
  
}

/**
  * @brief  This function handles ethernet DMA interrupt request.
  * @param  None
  * @retval None
  */
void ETH_IRQHandler(void)
{
  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

  /* Frame received */
  if ( ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R) == SET) 
  {
    /* Give the semaphore to wakeup LwIP task */
    xSemaphoreGiveFromISR( s_xSemaphore, &xHigherPriorityTaskWoken );   
  }
	
  /* Clear the interrupt flags. */
  /* Clear the Eth DMA Rx IT pending bits */
  ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
  ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
	
  /* Switch tasks if necessary. */	
  if( xHigherPriorityTaskWoken != pdFALSE )
  {
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
  }
}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
