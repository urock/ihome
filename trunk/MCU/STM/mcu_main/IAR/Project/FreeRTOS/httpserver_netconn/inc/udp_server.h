#ifndef _UDP_SERVER_H
#define _UDP_SERVER_H

#include "main.h"
#include "lwip/pbuf.h"
#include "lwip/udp.h"


void UDP_server_init(void);
void udp_server_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);


#endif // _UDP_SERVER_H
