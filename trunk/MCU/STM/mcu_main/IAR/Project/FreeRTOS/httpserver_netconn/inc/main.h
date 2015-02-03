/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    07-October-2011
  * @brief   This file contains all the functions prototypes for the main.c 
  *          file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

 /* Includes ------------------------------------------------------------------*/
#include "sys_include.h"
#include "FreeRTOSConfig.h"
#include "onewire.h"
#include "timers.h"
#include "tcp_client_netconn.h"

#define MCU_FIRMWARE_VERSION		1


// freeRTOS tasks priorities
#define TCP_SERVER_THREAD_PRIO      ( tskIDLE_PRIORITY + 1 )
#define TCP_CLIENT_THREAD_PRIO      ( tskIDLE_PRIORITY + 1 )

#define thread_comport_in_PRIO    	( tskIDLE_PRIORITY + 1 )
#define thread_comport_out_PRIO    	( tskIDLE_PRIORITY + 1 )
	
#define POWER_CNTRL_PRIO    		( tskIDLE_PRIORITY + 1 )
#define ONEWIRE_TASK_PRIO    		( tskIDLE_PRIORITY + 1 )    
#define DMX512_TASK_PRIO    		( tskIDLE_PRIORITY + 1 )    


#define TCP_CLIENT_PERIOD_MS		1
	
	
#define QUEUE_WRITE_TICKS_TO_WAIT		100
//#define QUEUE_READ_TICKS_TO_WAIT		10
	
#define NUM_RELAY_BOARDS				8

   
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

//#define USE_DHCP       /* enable DHCP, if disabled static address is used*/


//------------------------------------------------------------------
/* Uncomment SERIAL_DEBUG to enables retarget of printf to  serial port for debug purpose */   

#define SERIAL_DEBUG
   
#ifdef SERIAL_DEBUG

#define msg_dbg(message) printf message
	
	
#define msg_dbg_tsk( message )									\
	do {											\
	  if ( xSemaphoreTake(xUSARTSemaphore, portMAX_DELAY) == pdTRUE) {			\
		msg_dbg( message );								\
		xSemaphoreGive(xUSARTSemaphore); 						\
	  }											\
	} while(0) 										\

//////////////////////////////////////////////////////////////////////////////////////
#else	// SERIAL_DEBUG not defined
	  
#define msg_dbg_tsk( message )
	
#define msg_dbg(message)

#endif 	// SERIAL_DEBUG
   

typedef struct {
  //uint8_t	fversion; 					//!< STM Firmware version
  uint8_t	SN[4];						//!< MCU Board serial number
  uint8_t	stm_mac[6];					//!< STM MAC Address 	  [0] - MSB, [5] - LSB
  uint8_t	stm_ip[4];					//!< STM IP Address 	  [0] - MSB, [3] - LSB
  uint8_t	stm_mask[4];				//!< STM Net Mask 		  [0] - MSB, [3] - LSB
  uint8_t	stm_gw[4];					//!< STM Gateway 		  [0] - MSB, [3] - LSB

  uint16_t	stm_tcp_server_port;
  uint8_t	host_ip[4];					//!< HOST IP Address 	  [0] - MSB, [3] - LSB
  uint16_t	host_tcp_server_port;

} mcu_config_t;


//TODO: Сделать динамические адреса и хранить их в флеш микроконтроллера
/* MAC ADDRESS*/
#define MAC_ADDR0   02
#define MAC_ADDR1   00
#define MAC_ADDR2   00
#define MAC_ADDR3   00
#define MAC_ADDR4   00
#define MAC_ADDR5   01
 

// STM Server Side

/*STM IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1
#define IP_ADDR3   30
   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1  

#define TCP_SERVER_PORT		10001


// STM Client Side



#define HOST_IP_ADDR0   192
#define HOST_IP_ADDR1   168
#define HOST_IP_ADDR2   1
#define HOST_IP_ADDR3   111

#define HOST_PORT		111





/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */  
void Time_Update(void);
void Delay(uint32_t nCount);
void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName );

//---- Прототипы вызываемых функций при достижении таймеров заданного значения --
void vAutoReloadTimerFunction(xTimerHandle xTimer);
void vOneWireSendTimerFunction(xTimerHandle xTimer);

void my_task_TaskList (void *dev);



#endif /* __MAIN_H */



