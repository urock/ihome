#ifndef __SET_PORTS_H__
#define __SET_PORTS_H__

#include "init_ports.h"
#include "init_const.h"
#include "parsing_html.h"
#include "main.h"

// --- set value in ports
void set_ports_PORTOn(Ports_TypeDef Port);
void set_ports_PORTOff(Ports_TypeDef Port);
void set_ports_PORTToggle(Ports_TypeDef Port);

//void set_ports_setAllValues(Ports_TypeDef Port, PortMode_TypeDef mode, int status, int def_value);
void set_ports_setAllValues(volatile struct PORT_struct *device);

void set_ports_interrupt(Ports_TypeDef Port, uint8_t mode);
Ports_TypeDef getPort_name (uint8_t pt);
PortMode_TypeDef getPinMode (uint8_t pt);

void set_ports_ToggleState_Ports( volatile struct PORT_struct); // Изменение состояния вертуального порта
uint32_t set_ports_Interrupt_GetState(Ports_TypeDef Port);

PortStatus_TypeDef getStatus(uint8_t stat);
PortEnable_TypeDef getEnable(uint8_t ena);

uint8_t getIntPinMode (PortMode_TypeDef pt);
uint8_t getIntStatus(PortStatus_TypeDef stat);
uint8_t getIntEnable(PortEnable_TypeDef ena);

#endif /* __SET_PORTS_H__ */