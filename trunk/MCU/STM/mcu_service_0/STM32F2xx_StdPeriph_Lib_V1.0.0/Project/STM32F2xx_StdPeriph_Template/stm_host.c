/** \file stm_host.c
 *  \brief File containing functions servicing interaction with USB and PCIe hosts,
	of RDM and performing delayed actions scheduled in different ISRs

	
	General Idea is that lowest priority program spins in endless while loop in stm_host_comm
	function and waits for asserting flags. These flags are asserted in ISRs: either in
	servicing requests from USB, PCIe host or RDM module, or in Timer ISR (every 1 sec). 

	

 */

#include "main.h"
#include "user_uart.h"
#include "stm_host.h"
#include "jtag/ports.h"
#include "jtag/micro.h"
#include "user_timer.h"
#include "gpio_low_level.h"

extern stm_status_t		stm_status;





/************ global vars for USART ************************/
extern struct cmd_frame_s uframe; // cmd from host received by USART
extern __IO uint8_t need_uframe_dpc; // delayed procedure call for USART command

// Buffer_Block_Rx is used to store data buffers while data transfer with host by USART
extern uint8_t Buffer_Block_Rx[RX_BLOCK_SIZE];  

extern __IO uint8_t usart_stm_ready;				// flag set in DPC when smth is done; 
extern __IO uint8_t usart_ready_to_send_data;	// flag set in ISR to tell DPC to finish data transfer




/** 
*  \brief Endless loop where program waits for assetions of flags either in
	servicing requests from USB, PCIe host or RDM module, or in Timer ISR (every 1 sec). 
 	
USB - USART:
	Flag need_uframe_dpc is set in USART ISR when STM received the whole command Frame 
	Commads supported by this interface:
		<ul>
			<li> CMD_READ_STATUS
			<li> CMD_WRITE_XSVF
			<li> CMD_READ_XSVF
			<li> CMD_PROG_CPLD

	\return none
 */
void stm_host_comm(void)
{
  	int ii; 

	while(1) {
	  
		if (need_uframe_dpc	== 1) {
		  
		  	// timer interrupt was disabled in intrrupt service routine for usart rx
		  
			need_uframe_dpc = 0; 
			
			switch 	(uframe.cmd_code) {
			  
				case CMD_READ_STATUS: 	read_status_handler(); 	break;				  
			  
					
				case CMD_READ_XSVF: 	read_xsvf_handler();		break; 
					
				case CMD_PROG_CPLD:		prog_cpld_handler(); 		break; 
					
				default:
					user_put_char(CMD_NACK); 
					break; 							
			  
			}
			
			// enable timer interrupt after transfer data
			TIM_ITConfig(TIM3, TIM_IT_Update , ENABLE);					
			
			
			
						
		} 
	} // while
 
  
}





void read_status_handler(void)
{
	  
	
	user_put_char(CMD_ACK); 
		
	user_tx_buf_usart((uint8_t*) &stm_status, sizeof(stm_status_t));
		
	
}


void read_xsvf_handler(void)
{
 	uint16_t i;
	uint16_t	xsvf_position;	
	uint16_t	xsvf_len;

	

	  
		xsvf_position = uframe.cmd_data.xsvf_data.xsvf_position; 	  
		xsvf_len = uframe.cmd_data.xsvf_data.xsvf_len; 
		
		user_put_char(CMD_ACK); 
			
		for (i=0; i<xsvf_len; i++) 
			user_put_char(Buffer_Block_Rx[i + xsvf_position]);
				
	

}

void prog_cpld_handler(void)
{
  
	int 	xsvf_error = 0; 
	int		status; 
	
	unsigned char	cpld_selector;
	
	cpld_selector =  uframe.cmd_data.prog_cpld_data.cpld_selector;
	
	user_put_char(CMD_ACK); 
	
	if (cpld_selector == 0) {
		
		// init mcu pins for jtag opp
		sys_jtag_init(); 
		
		xsvf_data_init(); 
		
		xsvf_error = xsvfExecute();
		
		if (xsvf_error == 0) { // OK
			user_put_char(CMD_CMLP); 
		} else {
			user_put_char(CMD_ERROR); 		
			user_put_char(xsvf_error); 
		} 
		
		// deinit mcu pins for jtag opp
		sys_jtag_deinit();
		


		led_on(LED0); 
		
		
	} else {

		// init mcu pins for jtag opp
		board_jtag_init(); 
		
		// sel mode = 1
		set_cpld_mode_high();
		// sel channel 
		set_cpld_channel(cpld_selector); 
	/*	
		while(1) {
    setPort(TCK,0);  
		TIM_SetCounter(TIM2,0); 
		while(TIM_GetCounter(TIM2) < 1000); // dt = 50 ns; 	
    setPort(TCK,1); 
		TIM_SetCounter(TIM2,0); 
		while(TIM_GetCounter(TIM2) < 1000); // dt = 50 ns; 	
		}
		
	*/
		xsvf_data_init(); 
		
		xsvf_error = xsvfExecute();
		
		if (xsvf_error == 0) { // OK
			user_put_char(CMD_CMLP); 
		} else {
			user_put_char(CMD_ERROR); 		
			user_put_char(xsvf_error); 
		} 
		
		// deinit mcu pins for jtag opp
		board_jtag_deinit();
		


		led_on(LED1); 	  
	}
  
}

