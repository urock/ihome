/**
  ******************************************************************************
  * @file    main.c
  * @author  urock
  * @version V0.1.0
  * @date    07-Jan-2014
  * @brief   Main program body
  ******************************************************************************

  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio_low_level.h"
#include "uart_low_level.h"
#include "spi_low_level.h"
#include "tcp_server_netconn.h"
#include "tcp_client_netconn.h"
#include "power_cntrl_thread.h"
#include "uart_thread.h"
#include "flash.h"
#include "udp_server.h"

mcu_config_t	mcu_config;

 
int CONNECTED_BOARD_KRM[8] = {0,0,0,0,0,0,0,0};

xQueueHandle power_control_queue ;
xQueueHandle tcp_client_queue ;

xSemaphoreHandle xUSARTSemaphore;


/*Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
 int main(void)
{ 

	 portBASE_TYPE xReturn;  //  TaskCreate Error

	 uint8_t	wr_val=0;
	 uint8_t	rd_val=1;
	 
	 uint16_t	count = 0, cycles=0;
	 
	 uint32_t	errors = 0; 
  
#ifdef SERIAL_DEBUG
	 DebugComPort_Init();

	 vSemaphoreCreateBinary(xUSARTSemaphore);
	 if (xUSARTSemaphore == NULL) {
		msg_dbg(("vSemaphoreCreateBinary failed\n"));
		while(1);
	 }
#endif
	 
  
//	 msg_dbg(("DBG: Program start, base type size is %d\n\n\r",sizeof(portBASE_TYPE)));
  
	 init_leds();

	 init_spi();
         for (int i = 1; i < NUM_RELAY_BOARDS; i++){
            rd_val = spi_read_id(i);
            if (rd_val != 255){
              CONNECTED_BOARD_KRM[i-1] = 1;
              
              msg_dbg(("board present %d\n",i));
            }
            else{
              CONNECTED_BOARD_KRM[i-1] = 0;
            }
         }
/*         
         relay_write_reg_spi(1, 0xA1);
         relay_write_reg_spi(3, 0xA3);
         
         rd_val = relay_read_reg_spi(1);
         msg_dbg(("board 1 state %d\n",rd_val));

         rd_val = relay_read_reg_spi(3);
         msg_dbg(("board 3 state %d\n",rd_val));
         
         relay_write_reg_spi(1, 0xFF);
         relay_write_reg_spi(3, 0xFF);
         
         rd_val = relay_read_reg_spi(1);
         msg_dbg(("board 1 state %d\n",rd_val));

         rd_val = relay_read_reg_spi(3);
         msg_dbg(("board 3 state %d\n",rd_val));
*/
         
  //       while(1);
/*	 
	 if (rd_val != rd_val) {
	 	errors++;
		printf("errors = %d\n",errors);
	 }
	 
	 if (count == 0) {
	 	printf("cycles = %d, errors = %d\n",cycles, errors);
		cycles++;
	 }
	 count++;
	 wr_val++;
	 
	 }
*/
	 //TODO: add check of connected devices


	 led_on(LED0);
	 led_on(LED1);
	 led_on(LED2);


	 init_config_t(&mcu_config);

  
 
/* configure Ethernet (GPIOs, clocks, MAC, DMA) */ 
	 ETH_BSP_Config();
    
/* Initilaize the LwIP stack */
	 LwIP_Init();
  

	 UDP_server_init();
	  
	  //  
//  /* Initialize webserver demo */
  tcp_server_netconn_init(); //   // DEFAULT_THREAD_STACKSIZE = 500B

  
  
	msg_dbg(("Done Init, Start tasks\n\r"));
  
	power_control_queue 	= xQueueCreate( 8, sizeof(uint16_t) );
	tcp_client_queue 	= xQueueCreate( 8, sizeof(uint16_t) );
	
	if ( (power_control_queue != NULL) && (tcp_client_queue != NULL) ) {

		xReturn = xTaskCreate(power_control_thread, (signed portCHAR *)"power_control_thread", configMINIMAL_STACK_SIZE * 2, NULL, POWER_CNTRL_PRIO, NULL);
		if (xReturn != pdPASS){
			msg_dbg(("Can't create thread_relay. Error = %d \n", (int)xReturn));
		}

//		xReturn = xTaskCreate(thread_comport_out, ( signed portCHAR * )"thread_comport_out", configMINIMAL_STACK_SIZE * 2, NULL , thread_comport_out_PRIO, NULL);
//		if (xReturn != pdPASS){
//			msg_dbg(("Can't create thread_comport_out. Error = %d \n", xReturn));
//		}

		xReturn = xTaskCreate(thread_comport_in, ( signed portCHAR * )"thread_comport_in", configMINIMAL_STACK_SIZE * 2, NULL , thread_comport_in_PRIO, NULL);
		if (xReturn != pdPASS){
			msg_dbg(("Can't create thread_comport_in. Error = %d \n", xReturn));
		}

		tcp_client_netconn_init();
				
	} else {
		msg_dbg(("Can't create xQueue_relay_x \n"));    
	}

          

	/* Start scheduler */
	vTaskStartScheduler();
  
	while(1);

	return 0;

}






/** ----------------------------------------------------------------------------
* @brief В этой функции выполняются какие либо действия если программный стек переполнился
* например можно зажечь светодиод
* @param
* @return void
*/
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName ){
// Если переполняется Stack то вызываем загорается индикатор alarm  
//LED_alarm_On();   
for (;;){

  portCHAR PAGE_BODY[1024];
  printf("\n\n");
#ifdef configUSE_TRACE_FACILITY  
  strcat((char *)PAGE_BODY, "Name          State  Priority  Stack   Num\n" );
  strcat((char *)PAGE_BODY, "---------------------------------------------\n");
  
    
  /* The list of tasks and their status */
  vTaskList((signed char *)(PAGE_BODY + strlen(PAGE_BODY)));
  printf("\n%s\n", "task name status priority HWMTS ID");
  strcat((char *)PAGE_BODY, "---------------------------------------------\n");
  strcat((char *)PAGE_BODY, "B : Blocked, R : Ready, D : Deleted, S : Suspended\n\n");
   
  printf ("%s\n", PAGE_BODY);
#endif
  
  } //end for
}



/**-----------------------------------------------------------------------------
* @brief Этот таймер запускает функцию опроса датчиков температуры на  шине 1-wire каждую минуту
* 
*/
void vAutoReloadTimerFunction(xTimerHandle xTimer) {  
// Вставить функцию запуска опроса датчика  
  
  
  return;
}//--------------------------------------------------------------------------
//---------------------------------------------------------------------------
/**
* @brief Функция интервальных таймеров. 
* 
* 
*/
void vQneShotTimersFunction(xTimerHandle xTimer) {
  //end_timer = 1;  // Одиночный таймер
  return;
}
//------------------------------------------------------------------------------

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
