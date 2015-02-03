/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"
#include "stm32_eval_sdio_sd.h"

#include "main.h"
#include "user_timer.h"
#include "user_uart.h"




/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/






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
  
    TimingDelay_Decrement();
}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/


/**
  * @brief  This function handles SDIO global interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
  /* Process All SDIO Interrupt Sources */
  //SD_ProcessIRQSrc();
}

void TIM3_IRQHandler(void)
{
  Timer_Interrupt_Service(); 
}




/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
//  if(EXTI_GetITStatus(EXTI_Line3) != RESET)
//  {
//	
//	wbus_irq_handler();
//    
//    /* Clear the EXTI line 0 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line3);
//  }
}


void USART1_IRQHandler(void)
{
  
	user_usart_irq_handler();  
}



/*EDGE_I2C*/
void I2C1_EV_IRQHandler(void)
{
  
//	Edge_i2c_EV_IRQ_handler();
  
   
}

/**
  * @brief  This function handles I2C1 Error interrupt request.
  * @param  None
  * @retval : None
  */
void I2C1_ER_IRQHandler(void)
{
  /* Read SR1 register to get I2C error */
//  if ((I2C_ReadRegister(EDGE_I2C, I2C_Register_SR1) & 0xFF00) != 0x00)
//  {
//    /* Clears error flags */
//    I2C1->SR1 &= 0x00FF;
//  }

}



//LOCAL I2C
void LOCAL_I2C_ER_IRQHANDLER(void)
{

  
//  /* Read SR1 register to get I2C error */
//  if ((I2C_ReadRegister(LOCAL_I2C, I2C_Register_SR1) & 0xFF00) != 0x00)
//  {
//    /* Clears error flags */
//    LOCAL_I2C->SR1 &= 0x00FF;
//    
//  }

}




void LOCAL_I2C_EV_IRQHANDLER(void)
{
  
	//();  
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
