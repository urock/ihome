/*
 * uart_thread.c
 *
 */


#include "main.h"
#include "uart_thread.h"
#include "uart_low_level.h"

extern xQueueHandle power_control_queue ;
extern xQueueHandle tcp_client_queue ;

extern xSemaphoreHandle xUSARTSemaphore;


void thread_comport_out(void *pParams)
{
	uint16_t 	data16;
	uint8_t		board;
	uint8_t 	new_relay_state;
	while(1) {

		if (xQueueReceive(tcp_client_queue, &data16, 100) == pdTRUE) {
			board 			= (uint8_t)(data16 >> 8);
			new_relay_state = (uint8_t)data16;
			msg_dbg_tsk(("thread_comport_out: board = %d; new state = %d\n\r",board,new_relay_state));
		}

		vTaskDelay(10/portTICK_RATE_MS);
	}

}



void thread_comport_in(void *pParams)
{
	uint8_t ch1, ch2;
	uint16_t data16;


	while(1) {

		if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {

			ch1 = USART_ReceiveData(USART1);

			msg_dbg_tsk(("thread_comport_in:  %d \n\r", ch1));

			data16 = (uint16_t)ch1;

			xQueueSend(power_control_queue, &data16, QUEUE_WRITE_TICKS_TO_WAIT);
		}

		vTaskDelay(10/portTICK_RATE_MS);
	}


}
