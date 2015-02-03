#ifndef __SEND2HOST_H__
#define __SEND2HOST_H__

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
//***************************************
#include "semphr.h"

//****************************************
#include <lwip/ip_addr.h>
#include <stdio.h>
#include <string.h>
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "netbuf.h"
//#include "udp.h"
//**********************************************
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "memp.h"
#include "api.h"
//#include "get_portinfo.h"

#include "main.h"

#define MAX_CMDSTR      170 //Максимальная длина команды в sendText_t

/* ECHO protocol states */
enum client_states
{
  ES_NOT_CONNECTED = 0,
  ES_CONNECTED,
  ES_RECEIVED,
  ES_CLOSING,
};


/* structure to be passed as argument to the tcp callbacks */
struct client
{
  enum client_states state; /* connection status */
  struct tcp_pcb *pcb;          /* pointer on the current tcp_pcb */
  struct pbuf *p_tx;            /* pointer on pbuf to be transmitted */
};
//----------------------------------------
/**---------------------------------------------
* @brief Структура для создания посылки на ХОСТ компьютер по ETH
* @param char cmd[MAX_CMDSTR] - строка с командой, int len - длина этой строки
*
*/
struct sendText_t
{
  char cmd[MAX_CMDSTR];
  uint8_t len;
};

//Приоритет задачи
#define CLIENT_HTTP      ( tskIDLE_PRIORITY + 1 )     

/**
* @brief HOST PC parameters (IP, web server port)
*/
#define IP_ADDRESS( a, b, c, d ) ( ( (int)d << 24 ) + ( (int)c << 16 ) + ( (int)b << 8 ) + (int)a )

//void send_to_host(char buf1[]);
/**
* @brief Function for initialisation client
*/
void vNetconnClient_init(void *buf);
//void vNetconnClient(void *buf);
void vNetconnClient(void *buf);
//void send_to_host_init(void);

#endif /* __SEND2HOST_H__ */