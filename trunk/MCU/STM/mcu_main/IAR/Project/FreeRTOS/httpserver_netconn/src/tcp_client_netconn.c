#include "main.h"
#include "tcp_client_netconn.h"
#include "tcp_protocol.h"

struct ip_addr remote_ip_global;

extern xSemaphoreHandle xUSARTSemaphore;
extern xQueueHandle tcp_client_queue ;
extern xQueueHandle tcp_tempsensor_queue;


err_t tcp_send_buffer(
						//struct netconn 		*conn,
	struct ip_addr 		*addr,
	u16_t 				port,
	uint8_t 			*data_in,
	uint8_t 			size_in,
	uint8_t				*data_out,
	uint8_t 			*size_out
	)

{
	err_t tcp_error;

	struct netbuf *inbuf;

	uint8_t *buf;
	uint16_t buflen;
	size_t	i;

	struct netconn 		*conn = NULL;

	msg_dbg_tsk(("tcp_send_buffer: enter\n\r"));

	conn = netconn_new ( NETCONN_TCP );

	if (conn == NULL) {
		msg_dbg_tsk(("client_netconn_thread: Error in netconn_new\n\r"));
	}

	tcp_error = netconn_connect (conn, addr, port);

	if ( tcp_error != ERR_OK ) {
		msg_dbg_tsk(("tcp_send_buffer: netconn_connect failed\n\r"));
		return tcp_error;
	}

	// send data
	tcp_error = netconn_write(conn, data_in, size_in, NETCONN_NOCOPY);
	if (tcp_error != ERR_OK) {
		msg_dbg_tsk(("tcp_send_buffer: Error in netconn_write\n\r"));
		netconn_close(conn);
		return tcp_error;
	}        
	// check response
//        conn->recv_timeout=1000;              
       
	inbuf = netconn_recv(conn);
	if (inbuf != NULL) {

		// copy data from inbuf to data_out buf
		netbuf_data(inbuf, (void**)&buf, &buflen);

		if (buflen > MAX_TCP_MSG_SIZE) {
			msg_dbg_tsk(("tcp_send_buffer: buflen > MAX_TCP_MSG_SIZE\n\r"));
			tcp_error = ERR_IF;
		}

		for (i=0; i<buflen; i++)
			data_out[i] = buf[i];

		*size_out = buflen;

		// free inbuf
		netbuf_delete(inbuf);
        
	} else {
		msg_dbg_tsk(("tcp_send_buffer: inbuf = NULL\n\r"));
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
void client_netconn_thread(void *dev)
{
	uint8_t	        tcp_cmd[MAX_TCP_MSG_SIZE];
	uint8_t	        tcp_answer[MAX_TCP_MSG_SIZE];
	uint8_t			cmd_len;
	uint8_t		 	answer_len;
        
    struct relay_port_status_t     port_status;
    temp_status_t           temp_status;

	int i;

	portTickType 			xLastWakeTime;

	msg_dbg_tsk(("client_netconn_thread: enter\n\r"));

	// remote server address
	IP4_ADDR( &remote_ip_global, HOST_IP_ADDR0, HOST_IP_ADDR1, HOST_IP_ADDR2, HOST_IP_ADDR3 );

	xLastWakeTime = xTaskGetTickCount();

	while(1) {
//------------------- Посылаем состояние реле -------------------------------------------
		// TODO: change to blocking read from queue
		if (xQueueReceive(tcp_client_queue, &port_status, 100) == pdTRUE) {

			msg_dbg_tsk(("client_netconn_thread:  board = %d; port = %d; state = %d\n\r", port_status.BoardIndex,port_status.PortNumber, port_status.PortStatus));
                     
                        // form command
                        tcp_cmd[0] = CID_RELAY;
                        tcp_cmd[1] = REPORT_PORTSTA_REQ;
                        tcp_cmd[2] = 0; // MSB of length
                        tcp_cmd[3] = 3;	// LSB of length
                        tcp_cmd[4] = port_status.BoardIndex;
                        tcp_cmd[5] = port_status.PortNumber;
                        tcp_cmd[6] = port_status.PortStatus;
                        cmd_len = 7;

                        // send command and get answer
                        if (tcp_send_buffer( &remote_ip_global, HOST_PORT, tcp_cmd, cmd_len, tcp_answer, &answer_len)
                                  != ERR_OK) {
                              msg_dbg_tsk(("client_netconn_thread: Error in tcp_send_buffer\n\r"));
                              continue;
                         }
                         // check crc and errors

                         msg_dbg_tsk(("client_netconn_thread: tcp_answer = "));
                         for (i=0; i<answer_len; i++)
                              msg_dbg_tsk(("%x",tcp_answer[i]));
                              msg_dbg_tsk(("\n\r"));                                                
		}
// --------------- Посылаем температуру -----------------------------------------------------                
                if (xQueueReceive(tcp_tempsensor_queue, &temp_status, 100) == pdTRUE) {
                        for(int i = 0; i < 8; i++){
                          msg_dbg_tsk(("%x:", temp_status.id[i]));
                        }
                        msg_dbg_tsk(("\n\r"));
                  
			msg_dbg_tsk(("client_netconn_thread:  temp_int = %d; temp_float = %d;\n\r", temp_status.int_path,temp_status.float_path));
                        if (temp_status.mode == SENDTEMP_1WIRE){
                            // form command
                            tcp_cmd[0] = CID_TEMP;
                            tcp_cmd[1] = REPORT_TEMP_REQ;
                            tcp_cmd[2] = 0; // MSB of length
                            tcp_cmd[3] = 3;	// LSB of length
                            tcp_cmd[4] = temp_status.id[0];
                            tcp_cmd[5] = temp_status.id[1];
                            tcp_cmd[6] = temp_status.id[2];
                            tcp_cmd[7] = temp_status.id[3];
                            tcp_cmd[8] = temp_status.id[4];
                            tcp_cmd[9] = temp_status.id[5];
                            tcp_cmd[10] = temp_status.id[6];
                            tcp_cmd[11] = temp_status.id[7];
                            tcp_cmd[12] = temp_status.int_path;
                            tcp_cmd[13] = temp_status.float_path;

                            cmd_len = 14;
                        }
                        else if(temp_status.mode == ADDDEV_1WIRE){
                            // form command
                            tcp_cmd[0] = CID_TEMP;
                            tcp_cmd[1] = REPORT_ADD_TEMP_REQ;
                            tcp_cmd[2] = 0; // MSB of length
                            tcp_cmd[3] = 3;	// LSB of length
                            tcp_cmd[4] = temp_status.id[0];
                            tcp_cmd[5] = temp_status.id[1];
                            tcp_cmd[6] = temp_status.id[2];
                            tcp_cmd[7] = temp_status.id[3];
                            tcp_cmd[8] = temp_status.id[4];
                            tcp_cmd[9] = temp_status.id[5];
                            tcp_cmd[10] = temp_status.id[6];
                            tcp_cmd[11] = temp_status.id[7];
                            
                            cmd_len = 12;                          
                        }

                        // send command and get answer
                        if (tcp_send_buffer( &remote_ip_global, HOST_PORT, tcp_cmd, cmd_len, tcp_answer, &answer_len)
                                  != ERR_OK) {
                              msg_dbg_tsk(("client_netconn_thread: Error in tcp_send_buffer\n\r"));

                              continue;
                         }
                         // check crc and errors

                         msg_dbg_tsk(("client_netconn_thread: tcp_answer = "));
                         for (i=0; i<answer_len; i++)
                              msg_dbg_tsk(("%x",tcp_answer[i]));
                              msg_dbg_tsk(("\n\r"));                                                
                  
                  
                }


		vTaskDelayUntil( &xLastWakeTime, TCP_CLIENT_PERIOD_MS/portTICK_RATE_MS );
	}


}




//void tcp_client_netconn_init()
//{
//  sys_thread_new("NetconnServer", client_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCP_SERVER_THREAD_PRIO);
//}

			   
