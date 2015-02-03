/** \file main.c
 *  \brief 
 */

/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include "user_uart.h"
#include "user_timer.h"
#include "stm_host.h"
#include "gpio_low_level.h"
#include "jtag/ports.h"
#include "jtag/micro.h"

stm_status_t	stm_status;



int main(void)
{
 
	int		status, i;
        
	uint8_t *ptr8;
	
	
	
	// init stm_status struct
	ptr8 = (uint8_t*)&stm_status; 
	for(i=0;i<sizeof(stm_status_t); i++)
	  *ptr8++ = 0;   
	
	stm_status.fversion = STM_FIRMWARE_VER; 
    

        
	user_usart1_init(FTDI_BAUDRATE); 
	
	user_timer_init1();

	printf("\n\r\n\r\n\r\n\r\r************** RC47 Test Started ************************\n\r\r\n\r");
	
	init_leds();


	// enable 1 sec timer interrupts at the end of init
	TIM_ITConfig(TIM3, TIM_IT_Update , ENABLE); 
	
/*	
			// init mcu pins for jtag opp
		board_jtag_init(); 
		
		// sel mode = 1
		set_cpld_mode_high();
		// sel channel 
		set_cpld_channel(1); 
		
		while(1) {
    setPort(TCK,0);  
		TIM_SetCounter(TIM2,0); 
		while(TIM_GetCounter(TIM2) < 1000); // dt = 50 ns; 	
    setPort(TCK,1);  
		TIM_SetCounter(TIM2,0); 
		while(TIM_GetCounter(TIM2) < 1000); // dt = 50 ns; 	
		}
 */
   
	stm_host_comm(); 
	
}




/** \fn Timer_Interrupt_Service
 *  \brief 

	\return none

 */
void Timer_Interrupt_Service(void)	
{
	TIM_ITConfig(TIM3, TIM_IT_Update , DISABLE); 
	
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
	led_toggle(LED_STATUS); 


	TIM_ITConfig(TIM3, TIM_IT_Update , ENABLE); 
	
}
