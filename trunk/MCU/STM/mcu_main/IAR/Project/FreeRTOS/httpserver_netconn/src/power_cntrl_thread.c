#include "main.h"
#include "power_cntrl_thread.h"
#include "spi_low_level.h"
#include "tcp_protocol.h"

// Внешние переменные
// Очереди
extern xQueueHandle power_control_queue ;
extern xQueueHandle tcp_client_queue ;
extern xQueueHandle tcp_tempsensor_queue;

//Симафоры
extern xSemaphoreHandle xUSARTSemaphore;

// переменные
extern int CONNECTED_BOARD_KRM[8];
//--------------------------------------------------
// Массив состояний реле платы KRM
uint8_t	current_relay_state[NUM_RELAY_BOARDS];

/**
 * 	 *
 * 	@brief Main thread function. Executed every RELAY_SCAN_PERIOD_MS millicesonds.
 *
 *
 * 	@param  *pParams
 *
 * 	@return never
 *
 */
void power_control_thread(void *pParams)
{
	uint8_t			board;
	uint8_t 		new_relay_state;

	uint8_t         port_num;
	uint8_t 		new_port_status;
        
    uint8_t         tmp_xor;
        
    int k;
        
    struct relay_port_status_t     port_status;
	struct relay_data_t	relay_data; 

	portTickType 	xLastWakeTime;

	msg_dbg_tsk(("Entering power_control_thread...\n\r"));

	xLastWakeTime = xTaskGetTickCount();

	// start endless loop
	while(1) {
	  
	  	// read data from input queue until it is empty
	  	while( xQueueReceive(power_control_queue, &relay_data, 0) == pdTRUE) {
			
		  if (relay_data.MsgType == 0) {	// write new relay state
		    
		    board = relay_data.byte1;
			new_relay_state = relay_data.byte2; 
		    
			// check input parameter
			if (board >=NUM_RELAY_BOARDS) {                          
                                msg_dbg_tsk(("Error in power_control_thread: board >= NUM_RELAY_BOARDS\n\r"));
				continue;
			}
			
			// for every read value update power register in relay board
			if (new_relay_state != current_relay_state[board]) {

				current_relay_state[board] = new_relay_state;

				// relay_write_reg_spi need board index in [1..8] range, so + 1
				relay_write_reg_spi(board+1, new_relay_state);
			}			
			
		    
		  } else if (relay_data.MsgType == 1) {	// update only one bit in relay state
		    
		    board = relay_data.byte1; 
		    port_num = relay_data.byte2;
		    new_port_status = relay_data.byte3;
		    
		    if (new_port_status == 0) 
		      current_relay_state[board] &= ~(1<<port_num); 
		    else
		      current_relay_state[board] |= (1<<port_num); 
		    
		    relay_write_reg_spi(board+1, current_relay_state[board]);
		    
		  }
			  
		}


		// scan through all relay boards and read it power register
//		for (board = 0; board < NUM_RELAY_BOARDS; board++) {
                for (board = 0; board < NUM_RELAY_BOARDS; board++) {
                  if (CONNECTED_BOARD_KRM[board] == 0){
                    continue;
                  }
                  else{
                	  new_relay_state = relay_read_reg_spi(board+1);

                	  if (new_relay_state != current_relay_state[board]) {
                		  port_status.BoardIndex = board;
                          tmp_xor = current_relay_state[board] ^ new_relay_state;
                                
                                for(k=0; k<8; k++) {
                                  if ((tmp_xor >> k) == 1 ) {
                                  
                                      port_status.PortNumber = k; 
                                      port_status.PortStatus = ((new_relay_state&(1<<k) )>> k); 
                                      
                                      if (relay_write_output_queue(&port_status) != pdTRUE) {
                                              msg_dbg_tsk(("Error in power_control_thread: can not write to output queue\n\r"));
                                      }                                     
                                  }
                                } // end for

                           current_relay_state[board] = new_relay_state;
                	 } // end if new state
                  } //end else
                } // end for

		vTaskDelayUntil( &xLastWakeTime, RELAY_SCAN_PERIOD_MS/portTICK_RATE_MS );
	} // while(1)
}
//------------------------------------------------------------------
// internal fucntions


/**
 *
 * 	@brief Posts data to tcp_client_queue.
 * 	If queue is full - wait for QUEUE_WRITE_TICKS_TO_WAIT system ticks
 *
 * 	Queue item is uint16_t word:
 * 		Eight MSB - board number
 * 		Eight LSB - new relay state
 *
 *
 * 	@param  board Relay board number
 *
 * 	@param  new_relay_state New relay state
 *
 * 	@return pdTRUE if successfully posted data to queue, else - pdFALSE
 *
 */
portBASE_TYPE relay_write_output_queue(struct relay_port_status_t     *port_status)
{
       
	return xQueueSend(tcp_client_queue, port_status, QUEUE_WRITE_TICKS_TO_WAIT);
}

portBASE_TYPE temp_write_output_queue(temp_status_t *temp_status)
{
       
	return xQueueSend(tcp_tempsensor_queue, temp_status, QUEUE_WRITE_TICKS_TO_WAIT);
}



