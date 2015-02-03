/** \file main.h 
 *  \brief Header file with main RC47 defines
 */

#ifndef __MAIN_H
#define __MAIN_H


#include "stm32f2xx.h"
//#include "stm32_eval.h"
#include <stdio.h>



#define STM_FIRMWARE_VER		02		//!< Init value for stm_status.fversion

//#define NDEBUG						//!< Defines FTDI_BAUDRATE and disables printf
//#ifdef NDEBUG
//	#define FTDI_BAUDRATE		1000000 
//#else
//	#define FTDI_BAUDRATE		115200
//#endif

//#define FTDI_BAUDRATE		3000000
#define FTDI_BAUDRATE		115200

#ifdef NDEBUG
#define printf(format, args...) ((void)0)
#endif // NDEBUG

// ERROR Codes
#define		STATUS_OK					1
#define 	STATUS_ERROR				-1


#define RX_BLOCK_SIZE		(64*1024)

/** \struct stm_status_t
 *  \brief General Error and Status structure for RC47 board
 *
		This stucture initialiases once on power up and then can be transfered 
		to host (USB or PCIe) upon request
		
 */
typedef struct {
  uint8_t	fversion; 					//!< STM Firmware version
  uint8_t	i2c_error;					//!< Set on error on I2C bus 
  char	serial_number[9];
   
} stm_status_t;




void Timer_Interrupt_Service(void); 



#endif //__MAIN_H