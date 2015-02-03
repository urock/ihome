#ifndef __STM_HOST_H
#define __STM_HOST_H




// cmd_code_defines
#define CMD_READ_STATUS			0x01
//#define CMD_READ_MAX_DATA		0x02
//#define CMD_READ_ADC_DATA		0x03

// not used cmds
//#define CMD_READ_LIMITS			0x04
//#define CMD_WRITE_LIMITS		0x05
//#define CMD_CLEAR_STM_FLASH		0x06

//#define CMD_PLX_RST				0x07
//#define CMD_V7_RST				0x08
//#define CMD_READ_SD_CARD			0x09
#define CMD_WRITE_XSVF			0x10
#define CMD_READ_XSVF			0x11
#define CMD_PROG_CPLD			0x12

//#define CMD_TEST_WIDE_BUS		0x13
//#define CMD_PLX_I2C_READ		0x14
//#define CMD_PLX_I2C_WRITE		0x15
//
//#define CMD_INIT_EEP			0x16
//#define CMD_READ_EEPR			0x17
//#define CMD_WRITE_EEPR			0x18
//
//#define CMD_READ_SDIO			0x19
//#define CMD_WRITE_SDIO			0x1A
//
//
//#define CMD_READ_IDROM			0x1B
//#define CMD_WRITE_IDROM			0x1C


//#define CMD_SP6_RECONFIG		0x04

#define CMD_CHECK_READY			0xA0
#define STM_READY				0xA1
#define STM_BUSY				0xA2

#define CMD_ACK					0xFF
#define CMD_CMLP				0xEE
#define CMD_NACK				0xF0
#define CMD_ERROR				0xE0




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
		
	
		


		
		

void stm_host_comm(void);		
void read_status_handler(void);





void read_xsvf_handler(void);
void prog_cpld_handler(void);

#endif // __STM_HOST_H