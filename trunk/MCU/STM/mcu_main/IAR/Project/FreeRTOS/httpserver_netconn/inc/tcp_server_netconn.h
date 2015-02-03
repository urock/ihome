#ifndef __HTTPSERVER_NETCONN_H__
#define __HTTPSERVER_NETCONN_H__


#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "fs.h"


void tcp_server_netconn_init(void);








// internal functions
void send_simple_answer(struct netconn *conn, uint8_t cid, uint8_t cpar, uint8_t len, uint8_t dat);



#endif /* __HTTPSERVER_NETCONN_H__ */
