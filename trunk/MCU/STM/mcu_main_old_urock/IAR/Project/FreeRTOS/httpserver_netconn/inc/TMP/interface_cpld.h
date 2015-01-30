#ifndef __INTERFACE_CPLD_H
#define __INTERFACE_CPLD_H

#include "init_const.h"
#include "set_ports.h"
#include "user_flash.h"
#include "main.h"


void interface_cpld_init (void);
void interface_cpld_clk(Ports_TypeDef Port_clk);
unsigned char Read_ID_cpld(Ports_TypeDef Port_clk, Ports_TypeDef Port_data_out, Ports_TypeDef Port_data_in, unsigned char data);

#endif /* __INTERFACE_CPLD_H */