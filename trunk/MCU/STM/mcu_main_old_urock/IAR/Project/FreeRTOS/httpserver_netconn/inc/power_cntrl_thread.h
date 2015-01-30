#ifndef power_control_thread_H
#define power_control_thread_H

#include "sys_include.h"


#define RELAY_SCAN_PERIOD_MS		100



// exported functions
void power_control_thread(void *pParams);



// internal functions
portBASE_TYPE relay_read_input_queue(uint8_t *board, uint8_t *new_relay_state);
portBASE_TYPE relay_write_output_queue(uint8_t board, uint8_t new_relay_state);

#endif // relay_thread_H
