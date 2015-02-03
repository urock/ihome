#ifndef power_control_thread_H
#define power_control_thread_H

#include "sys_include.h"
#include "tcp_protocol.h"

#define RELAY_SCAN_PERIOD_MS		100



// exported functions
void power_control_thread(void *pParams);



// internal functions
portBASE_TYPE relay_write_output_queue(struct relay_port_status_t     *port_status);
portBASE_TYPE temp_write_output_queue(temp_status_t     *temp_status);


#endif // relay_thread_H
