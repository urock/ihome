#ifndef __STM_HOST_H
#define __STM_HOST_H


/*****************************************************************************************	

	Функции STM, доступные хосту
	
		
	4. Работа с CPLD
		4.1 Write xsvf file to stm ram (xsvf_file_length)
		4.2 Read xsvf file from stm ram (xsvf_file_length)

*****************************************************************************************/


// cmd_code_defines
#define CMD_READ_STATUS			0x01
//#define CMD_READ_MAX_DATA		0x02
//#define CMD_READ_ADC_DATA		0x03
//
//#define CMD_PLX_RST				0x07
//#define CMD_V7_RST				0x08
//#define CMD_READ_SD_CARD			0x09
#define CMD_WRITE_XSVF			0x10
#define CMD_READ_XSVF			0x11
#define CMD_PROG_CPLD			0x12
//#define CMD_TEST_WIDE_BUS		0x13
//#define CMD_PLX_I2C_READ		0x14
//#define CMD_PLX_I2C_WRITE		0x15

//#define CMD_SP6_RECONFIG		0x04
//
//#define CMD_INIT_EEP			0x16
//#define CMD_READ_EEPR			0x17
//#define CMD_WRITE_EEPR			0x18
//
//#define CMD_READ_SDIO			0x19
//#define CMD_WRITE_SDIO			0x1A
//
//#define CMD_READ_IDROM			0x1B
//#define CMD_WRITE_IDROM			0x1C

#define CMD_CHECK_READY			0xA0
#define STM_READY				0xA1
#define STM_BUSY				0xA2

#define CMD_ACK					0xFF
#define CMD_CMLP				0xEE
#define CMD_NACK				0xF0
#define CMD_ERROR				0xE0

#define CPLD_ERROR				-10

// local status codes
#define CMD_NOT_SUPPORTED		-1
#define RS232_TIMEOUT			-2
#define BAD_ACK					-4
#define NACK					-5




	// CMD_WRITE_XSVF, CMD_READ_XSVF
		struct xsvf_str {
			unsigned short		xsvf_position; 	// current transfer position
			unsigned short		xsvf_len;		// current transfer length in bytes; = 128
		};

	// CMD_PROG_CPLD
		struct prog_cpld_str {
			unsigned char		cpld_selector; 	// 0 - sys cpld, 1-8 - board cpld
		};
		
	// command frame
		struct cmd_frame_s {
			unsigned char cmd_code;					// from cmd_code_defines
			union {

				struct xsvf_str				xsvf_data;		
				struct prog_cpld_str		prog_cpld_data; 

				
			} cmd_data;
		};
		
		
		




// function prototypes

int stm_send_cmd_frame(struct cmd_frame_s * frame);
//int stm_read_status(stm_status_t* stm_status_ptr);


int stm_write_cpld_file(unsigned char *xcvf_buf, unsigned short xsvf_position, unsigned short xsvf_buf_len);
int stm_read_cpld_file(unsigned char *xcvf_buf, unsigned short xsvf_position, unsigned short xsvf_buf_len);
int stm_prog_cpld(unsigned char		cpld_selector);

#endif // __STM_HOST_H