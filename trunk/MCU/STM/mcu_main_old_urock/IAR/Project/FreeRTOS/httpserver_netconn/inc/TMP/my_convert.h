#ifndef __MY_CONVERT_H__
#define __MY_CONVERT_H__
#include <stdlib.h>
#include <stdio.h>
#include "set_ports.h"
/**
* @brief Header Function convert string to integer
*/
struct str2int_t{
  char str[5];
  int   val;
};

void my_convert_int2str (void);
int my_convert_str2int (const char *str);
void my_convert_toPort_struct (uint8_t port, const char *str, const char *how_str);
PortMode_TypeDef my_convert_str2PortModeTypedef(const char *str, Ports_TypeDef port_set);
PortStatus_TypeDef my_convert_str2PortStatusTypeDef(const char *str);
PortEnable_TypeDef my_convert_str2PortEnableTypeDef(const char *str);
#endif /* __MY_CONVERT_H__ */