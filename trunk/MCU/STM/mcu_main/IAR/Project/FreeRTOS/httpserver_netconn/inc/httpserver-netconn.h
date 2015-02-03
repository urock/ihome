#ifndef __HTTPSERVER_NETCONN_H__
#define __HTTPSERVER_NETCONN_H__

//#include "get_portinfo.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "fs.h"
#include "string.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm322xg_eval.h"
#include "main.h"
#include <stdlib.h>
#include <stdio.h>
//#include "send2host.h"
#include "parsing_html.h"
//#include "my_tasks.h"
#include "my_convert.h"
//#include "init_const.h"
//#include "user_flash.h"
#include "spi_low_level.h"


void http_server_netconn_init(void);
void DynWebPage(struct netconn *conn);

#endif /* __HTTPSERVER_NETCONN_H__ */
