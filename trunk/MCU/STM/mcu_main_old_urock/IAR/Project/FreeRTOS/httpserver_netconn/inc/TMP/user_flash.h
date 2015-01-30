#ifndef __USER_FLASH_H
#define __USER_FLASH_H

#include "stm32f2xx_flash.h"
#include "init_const.h"

//#define USER_FLASH_BASE 		0x080E0000  // Адрес куда начинается зап
//#define USER_FLASH_INFO 		0x080C0000


// return codes 
#define FLASH_OK			1
#define FLASH_ERROR_ERASE		-1
#define FLASH_ERROR_WRITE		-2 

int write_buf_to_flash(uint8_t *buf, int len, int address);
void read_buf_from_flash(uint8_t *buf, int len, int address);
int clear_flash(int address);

#endif // __USER_FLASH_H