#ifndef __1WIRE_H
#define __1WIRE_H

#include "init_const.h"
#include "stm32f2xx_tim.h"

void RCC_Configuration (void);
void TIM3_Configuration (void);
void Delay_US (uint16_t useconds);
void Delay_MS (uint16_t mseconds);
void Set_Port_input (void);
void Set_Port_output (void);
void Set_Port_Up (void);
void Set_Port_Down (void);
uint8_t ow_reset (void);
uint8_t ow_read_byte (void);
void ow_write_byte (uint8_t byte);
uint16_t ow_search (void);

//uint8_t ds_reset_pulse(uint16_t PinMask);
//void ds_write_bit(uint8_t bit,uint16_t PinMask);
//uint16_t ds_read_bit(uint16_t PinMask);
//void ds_write_byte(uint8_t byte, uint16_t PinMask);
//uint8_t ds_read_byte(uint16_t PinMask);
//uint8_t ds_start_convert_single(uint8_t PinNumb);
//uint8_t ds_read_data_single(uint8_t *buff, uint8_t PinNumb);
//signed int ds_read_temperature(uint8_t PinNumb);



#endif /* __1WIRE_H */