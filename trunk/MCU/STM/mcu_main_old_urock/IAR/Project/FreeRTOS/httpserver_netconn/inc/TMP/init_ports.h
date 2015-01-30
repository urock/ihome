#ifndef __INIT_PORTS_H__
#define __INIT_PORTS_H__

#include "init_const.h"
#include "stm32f2xx.h"


// ------ Init Ports
/**
* @brief Function for init Ports
*/
void init_ports_PORTOut(Ports_TypeDef Port);
void init_ports_PORTIn(Ports_TypeDef Port);
//void init_ports_PORTPwm (Ports_TypeDef Port);
void init_ports_PORTPwm (void);
void init_ports_PORTAdc (Ports_TypeDef Port);
void init_ports_Interrupt(Ports_TypeDef Port, PortIO_TypeDef Port_Mode, EXTITrigger_TypeDef tr);

#endif /* __INIT_PORTS_H__ */
