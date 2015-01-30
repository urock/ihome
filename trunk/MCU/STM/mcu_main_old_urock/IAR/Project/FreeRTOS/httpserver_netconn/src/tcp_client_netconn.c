#include "main.h"
#include "tcp_client_netconn.h"
#include "tcp_protocol.h"


struct ip_addr remote_ip_global;

extern xSemaphoreHandle xUSARTSemaphore;

extern xQueueHandle tcp_client_queue ;



err_t tcp_send_buffer(
						//struct netconn 		*conn,
						struct ip_addr 		*addr,
						u16_t 				port,
						uint8_t 			*data_in,
						uint8_t 				size_in,
						uint8_t				*data_out,
						uint8_t 				*size_out
					)
{
	err_t tcp_error;

	struct netbuf *inbuf;

	uint8_t *buf;
	uint16_t buflen;

	size_t	i;

	struct netconn 		*conn = NULL;


	msg_dbg_tsk(("tcp_send_buffer: enter\n"));

	conn = netconn_new ( NETCONN_TCP );

	if (conn == NULL) {
		msg_dbg_tsk(("client_netconn_thread: Error in netconn_new\n"));
		//while(1);
	}

	tcp_error = netconn_connect (conn, addr, port);

	if ( tcp_error != ERR_OK ) {
		msg_dbg_tsk(("tcp_send_buffer: netconn_connect failed\n"));
		return tcp_error;
	}

	// send data
	tcp_error = netconn_write(conn, data_in, size_in, NETCONN_NOCOPY);
	if (tcp_error != ERR_OK) {
		msg_dbg_tsk(("tcp_send_buffer: Error in netconn_write\n"));
		netconn_close(conn);
		return tcp_error;
	}

	// check response
	inbuf = netconn_recv(conn);
	if (inbuf != NULL) {

		// copy data from inbuf to data_out buf
		netbuf_data(inbuf, (void**)&buf, &buflen);

		if (buflen > MAX_TCP_MSG_SIZE) {
			msg_dbg_tsk(("tcp_send_buffer: buflen > MAX_TCP_MSG_SIZE\n"));
			tcp_error = ERR_IF;
		}

		for (i=0; i<buflen; i++)
			data_out[i] = buf[i];

		*size_out = buflen;

		// free inbuf
		netbuf_delete(inbuf);


	} else {
		msg_dbg_tsk(("tcp_send_buffer: inbuf = NULL\n"));
		return ERR_IF;
	}

	netconn_close(conn);

	netconn_delete(conn);

	return tcp_error;
}



/**
  * @brief  http server thread
  * @param arg: pointer on argument(not used here)
  * @retval None
  */
static void client_netconn_thread(void *arg)
{


	uint16_t 	data16;
	uint8_t		board;
	uint8_t 	relay_state;

	uint8_t	tcp_cmd[MAX_TCP_MSG_SIZE];
	uint8_t	tcp_answer[MAX_TCP_MSG_SIZE];

	uint8_t		cmd_len;
	uint8_t 	answer_len;

	int i;


	portTickType 			xLastWakeTime;

//	struct netconn 		*xNetConn = NULL;


	msg_dbg_tsk(("client_netconn_thread: enter\n"));

//	xNetConn = netconn_new ( NETCONN_TCP );
//
//	if (xNetConn == NULL) {
//		msg_dbg_tsk(("client_netconn_thread: Error in netconn_new\n"));
//		while(1);
//	}


	// remote server address
	IP4_ADDR( &remote_ip_global, HOST_IP_ADDR0, HOST_IP_ADDR1, HOST_IP_ADDR2, HOST_IP_ADDR3 );

	xLastWakeTime = xTaskGetTickCount();

	while(1) {

		// TODO: change to blocking read from queue
		if (xQueueReceive(tcp_client_queue, &data16, 100) == pdTRUE) {
			board 			= (uint8_t)(data16 >> 8);
			relay_state = (uint8_t)data16;
			msg_dbg_tsk(("client_netconn_thread: board = %d; new state = %d\n\r",board,relay_state));

			// form command
			tcp_cmd[0] = CID_RELAY;
			tcp_cmd[1] = REPORT_RSTATE_REQ;
			tcp_cmd[2] = 0; // MSB of length
			tcp_cmd[3] = 2;	// LSB of length
			tcp_cmd[4] = board;
			tcp_cmd[5] = relay_state;

			cmd_len = 6;


			// send command and get answer
			if (tcp_send_buffer( &remote_ip_global, HOST_PORT, tcp_cmd, cmd_len, tcp_answer, &answer_len)
					!= ERR_OK) {
				msg_dbg_tsk(("client_netconn_thread: Error in tcp_send_buffer\n"));
				continue;
			}
			// check crc and errors

			msg_dbg_tsk(("client_netconn_thread: tcp_answer = "));
			for (i=0; i<answer_len; i++)
				msg_dbg_tsk(("%x",tcp_answer[i]));
			msg_dbg_tsk(("\n"));

		}


		vTaskDelayUntil( &xLastWakeTime, TCP_CLIENT_PERIOD_MS/portTICK_RATE_MS );
	}


}




void tcp_client_netconn_init()
{
  sys_thread_new("NetconnServer", client_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCP_SERVER_THREAD_PRIO);
}

			   
