#include "main.h"
#include "power_cntrl_thread.h"
#include "spi_low_level.h"


extern xQueueHandle power_control_queue ;
extern xQueueHandle tcp_client_queue ;

extern xSemaphoreHandle xUSARTSemaphore;

extern int CONNECTED_BOARD_KRM[8];

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


	portTickType 	xLastWakeTime;

	msg_dbg_tsk(("Entering power_control_thread...\n\r"));


	xLastWakeTime = xTaskGetTickCount();

	// start endless loop
	while(1) {

		// read data from input queue until it is empty
		while(relay_read_input_queue(&board, &new_relay_state) == pdTRUE) {

			// check input parameter
//			if (board >= NUM_RELAY_BOARDS) {
			if (board >=NUM_RELAY_BOARDS) {                          
//				msg_dbg_tsk(("Error in power_control_thread: board >= NUM_RELAY_BOARDS\n\r"));
                                msg_dbg_tsk(("Error in power_control_thread: board >= NUM_RELAY_BOARDS\n\r"));
				continue;
			}

			// for every read value update power register in relay board
			if (new_relay_state != current_relay_state[board]) {

				current_relay_state[board] = new_relay_state;

				// relay_write_reg_spi need board index in [1..8] range, so + 1
				relay_write_reg_spi(board+1, new_relay_state);
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

				current_relay_state[board] = new_relay_state;

				if (relay_write_output_queue(board, new_relay_state) != pdTRUE) {
					msg_dbg_tsk(("Error in power_control_thread: can not write to output queue\n\r"));
				}


			}
                  }

		}


		vTaskDelayUntil( &xLastWakeTime, RELAY_SCAN_PERIOD_MS/portTICK_RATE_MS );

	} // while(1)
}




// internal fucntions

/**
 * 	function relay_read_input_queue
 *
 * 	Reads power_control_queue. If queue is empty - exit immediately.
 * 	Queue item is uint16_t word:
 * 		Eight MSB - board number
 * 		Eight LSB - new relay state
 *
 *
 * 	@param  *board A pointer to uint8_t variable to hold relay board number
 *
 * 	@param  *new_relay_state A pointer to uint8_t variable to hold new relay state
 *
 * 	@return pdTRUE if successfully read data from queue, else - pdFALSE
 *
 */
portBASE_TYPE relay_read_input_queue(uint8_t *board, uint8_t *new_relay_state)
{
	uint16_t data16;

	// if queue is empty - exit immediately
	if (xQueueReceive(power_control_queue, &data16, 0) == pdTRUE) {
		*board 			= (uint8_t)(data16 >> 8);
		*new_relay_state = (uint8_t)data16;
		return pdTRUE;
	}

	return pdFALSE;
}

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
portBASE_TYPE relay_write_output_queue(uint8_t board, uint8_t new_relay_state)
{
	uint16_t data16;

	data16 = (((uint16_t)board) << 8) + new_relay_state;
        
//        return pdTRUE;

	return xQueueSend(tcp_client_queue, &data16, QUEUE_WRITE_TICKS_TO_WAIT);
}




