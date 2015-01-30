#include "stdafx.h"
#include "stm_host.h"
#include "ftdi_api.h"


extern FT_HANDLE ftHandle; 


//int stm_read_status(stm_status_t* stm_status_ptr)
//{
//	struct cmd_frame_s	frame; 
//
//	frame.cmd_code = CMD_READ_STATUS; 
//
//	if (stm_send_cmd_frame(&frame)) {
//		printf("Error sending cmd CMD_READ_STATUS \n");
//		return -1;
//	}
//
//	
//	// receive data from stm
//	if (ftdi_receive_buffer(ftHandle, (unsigned char *)stm_status_ptr, sizeof(stm_status_t)) < 0) {
//		printf("Error receining stm_status_t\n");
//		return -2; 
//	}
//
//
//	return 0; 
//
//}
//


// host writes xsvf file from xcvf_buf to stm RAM
int stm_write_cpld_file(unsigned char *xcvf_buf, unsigned short xsvf_position, unsigned short xsvf_buf_len)
{
	struct cmd_frame_s	frame; 

	int ret_val;
	unsigned char read_buf; 

	frame.cmd_code = CMD_WRITE_XSVF; 
	frame.cmd_data.xsvf_data.xsvf_position = xsvf_position;
	frame.cmd_data.xsvf_data.xsvf_len = xsvf_buf_len;

	if (stm_send_cmd_frame(&frame)) {
		printf("Error sending cmd CMD_WRITE_XSVF \n");
		return -1;
	}


	//printf("Done sending command CMD_WRITE_XSVF!\n"); 

	// send data to stm
	if (ftdi_send_buffer(ftHandle, (unsigned char *)xcvf_buf, xsvf_buf_len) < 0) {
		printf("Error sending buffer\n");
		return -2; 
	}

	// get ack from stm
	ret_val = ftdi_receive_byte(ftHandle, &read_buf);		// get acknowledge from STM
	if ( ret_val == -2) {		
		printf("Error receiving byte from ftdi\n"); 
		return -5; 
	} else if (ret_val == -1) {		
		printf("TimeOut receiving byte from ftdi\n"); 
		return RS232_TIMEOUT; 
	}

	if (( read_buf == CMD_CMLP) ) 
		return 0;
	 

	if ( read_buf == CMD_ERROR ) {	// chech acknowledge
		printf("CMD_ERROR\n");
		return NACK;
	} else {
		printf("Bad ACK from STm: 0x%x\n",read_buf);
		return BAD_ACK;
	} 

	return 0; 
}


// host read xsvf file from stm RAM to xcvf_buf
int stm_read_cpld_file(unsigned char *xcvf_buf, unsigned short xsvf_position, unsigned short xsvf_buf_len)
{
	struct cmd_frame_s	frame; 
	frame.cmd_code = CMD_READ_XSVF; 
	frame.cmd_data.xsvf_data.xsvf_position = xsvf_position;
	frame.cmd_data.xsvf_data.xsvf_len = xsvf_buf_len;


	if (stm_send_cmd_frame(&frame)) {
		printf("Error sending cmd CMD_READ_XSVF \n");
		return -1;
	}

	// receive data from stm
	if (ftdi_receive_buffer(ftHandle, (unsigned char *)xcvf_buf, xsvf_buf_len) < 0) {
		printf("Error receining buffer\n");
		return -2; 
	}

	return 0; 
}


// triggers reprogram cpld process; should be called after stm_write_cpld_file and stm_read_cpld_file
// unsigned char		cpld_selector; 	// 0 - sys cpld, 1-8 - board cpld
int stm_prog_cpld(unsigned char		cpld_selector)
{
	struct cmd_frame_s	frame; 
	int ret_val;
	unsigned char read_buf; 

	frame.cmd_code = CMD_PROG_CPLD; 
	frame.cmd_data.prog_cpld_data.cpld_selector = cpld_selector; 

	if (stm_send_cmd_frame(&frame)) {
		printf("Error sending cmd CMD_PROG_CPLD \n");
		return -1;
	}

	printf("\nCMD_PROG_CPLD sent. Waitting for completion...\n"); 

	Sleep(15000);   

	printf("Done wait\n");

	// get ack from stm
	ret_val = ftdi_receive_byte(ftHandle, &read_buf);		// get acknowledge from STM
	if ( ret_val == -2) {		
		printf("Error receiving byte from ftdi\n"); 
		return -5; 
	} else if (ret_val == -1) {		
		printf("TimeOut receiving byte from ftdi\n"); 
		return RS232_TIMEOUT; 
	}

	//printf("Ack received = 0x%x\n", read_buf);

	if (( read_buf == CMD_CMLP) ) {
		//printf("OK completing  CMD_PROG_CPLD\n");
		return 0;
	}

	if ( read_buf == CMD_ERROR ) {	// chech acknowledge

		printf("Error completing  CMD_PROG_CPLD "); 
		ret_val = ftdi_receive_byte(ftHandle, &read_buf);		// get acknowledge from STM
		if ( ret_val == -2) {		
			printf("\n\nError receiving byte from ftdi\n"); 
			return -5; 
		} else if (ret_val == -1) {		
			printf("\n\nTimeOut receiving byte from ftdi\n"); 
			return RS232_TIMEOUT; 
		}

		 printf("Error code = %d\n", read_buf);

		return NACK;
	} else {
		printf("Bad ACK from STm: 0x%x\n",read_buf);
		return BAD_ACK;
	} 

	return 0; 
}

    /*******************************************************************\
    *                                                                   *
		Отправляет команду и проверяет статус приема команды
		
		Возвращает 0 в случае успеха, либо код ошибки.
		
    *                                                                   *
    \*******************************************************************/

int stm_send_cmd_frame(struct cmd_frame_s * frame)
{
	int ret_val;
	unsigned char read_buf, wr_byte = 0; 

	if (ftdi_send_byte(ftHandle,frame->cmd_code) < 0) {  			
		printf("Error sending byte ftdi\n");
		return -3; 
	}

	switch (frame->cmd_code) {


	case CMD_WRITE_XSVF: 

		//printf("CMD_WRITE_XSVF\n"); 
		wr_byte = (unsigned char)(frame->cmd_data.xsvf_data.xsvf_position & 0xFF); 
		//printf("wr_byte1 = 0x%x\n",wr_byte);

		if (ftdi_send_byte(ftHandle,wr_byte) < 0) {  			
			printf("Error sending byte ftdi\n");
			return -3; 
		}

		wr_byte = (unsigned char)((frame->cmd_data.xsvf_data.xsvf_position & 0xFF00) >> 8); 
		//printf("wr_byte2 = 0x%x\n",wr_byte);

		if (ftdi_send_byte(ftHandle,wr_byte) < 0) {  			
			printf("Error sending byte ftdi\n");
			return -3; 
		}

		wr_byte = (unsigned char)(frame->cmd_data.xsvf_data.xsvf_len & 0xFF); 
		//printf("wr_byte1 = 0x%x\n",wr_byte);

		if (ftdi_send_byte(ftHandle,wr_byte) < 0) {  			
			printf("Error sending byte ftdi\n");
			return -3; 
		}

		wr_byte = (unsigned char)((frame->cmd_data.xsvf_data.xsvf_len & 0xFF00) >> 8); 
		//printf("wr_byte2 = 0x%x\n",wr_byte);

		if (ftdi_send_byte(ftHandle,wr_byte) < 0) {  			
			printf("Error sending byte ftdi\n");
			return -3; 
		}
		break;

	case CMD_READ_XSVF: 


		//printf("CMD_READ_XSVF\n"); 

		wr_byte = (unsigned char)(frame->cmd_data.xsvf_data.xsvf_position & 0xFF); 
		//printf("wr_byte1 = 0x%x\n",wr_byte);

		if (ftdi_send_byte(ftHandle,wr_byte) < 0) {  			
			printf("Error sending byte ftdi\n");
			return -3; 
		}

		wr_byte = (unsigned char)((frame->cmd_data.xsvf_data.xsvf_position & 0xFF00) >> 8); 
		//printf("wr_byte2 = 0x%x\n",wr_byte);

		if (ftdi_send_byte(ftHandle,wr_byte) < 0) {  			
			printf("Error sending byte ftdi\n");
			return -3; 
		}

		wr_byte = (unsigned char)(frame->cmd_data.xsvf_data.xsvf_len & 0xFF); 
		//printf("wr_byte1 = 0x%x\n",wr_byte);

		if (ftdi_send_byte(ftHandle,wr_byte) < 0) {  			
			printf("Error sending byte ftdi\n");
			return -3; 
		}

		wr_byte = (unsigned char)((frame->cmd_data.xsvf_data.xsvf_len & 0xFF00) >> 8); 
		//printf("wr_byte2 = 0x%x\n",wr_byte);

		if (ftdi_send_byte(ftHandle,wr_byte) < 0) {  			
			printf("Error sending byte ftdi\n");
			return -3; 
		}
		break;

	case CMD_PROG_CPLD:
		wr_byte = (unsigned char)(frame->cmd_data.prog_cpld_data.cpld_selector); 
		//printf("wr_byte2 = 0x%x\n",wr_byte);

		if (ftdi_send_byte(ftHandle,wr_byte) < 0) {  			
			printf("Error sending byte ftdi\n");
			return -3; 
		}


		break;

	default:					break; 

	}


	//printf("Receiving byte\n"); 
	ret_val = ftdi_receive_byte(ftHandle, &read_buf);		// get acknowledge from STM
	if ( ret_val == -2) {		
		printf("Error receiving byte from ftdi\n"); 
		return -5; 
	} else if (ret_val == -1) {		
		printf("TimeOut receiving byte from ftdi\n"); 
		return RS232_TIMEOUT; 
	}

	if (( read_buf == CMD_ACK) ) 
		return 0;
	 

	if ( read_buf == CMD_NACK ) {	// chech acknowledge
		printf("CMD_NACK\n");
		return NACK;
	} else {
		printf("Bad ACK from STm: %x\n",read_buf);
		return BAD_ACK;
	} 




	return 0; 
}


