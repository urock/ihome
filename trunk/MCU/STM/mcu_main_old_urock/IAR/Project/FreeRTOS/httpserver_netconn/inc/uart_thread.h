/*
 * uart_thread.h
 *
 *  Created on: 07 џэт. 2014 у.
 *      Author: rosta
 */

#ifndef UART_THREAD_H_
#define UART_THREAD_H_

#include "sys_include.h"

void thread_comport_out(void *pParams);
void thread_comport_in(void *pParams);

#endif /* UART_THREAD_H_ */
