#ifndef __USER_FLASH_H
#define __USER_FLASH_H


#define FLASH_SECTOR_11_BASE 		0x080E0000
#define FLASH_SECTOR_11_END 		0x080FFFFF

#define IS_SECTOR_11(ADDRESS) (((ADDRESS) >= FLASH_SECTOR_11_BASE) && ((ADDRESS) < FLASH_SECTOR_11_END))


// return codes 
#define FLASH_OK				1
#define FLASH_WRONG_ADDR		-1
#define FLASH_ERROR_ERASE		-2
#define FLASH_ERROR_WRITE		-3


#define MCU_CONFIG_BASE			FLASH_SECTOR_11_BASE


void init_config_t(mcu_config_t *config_out);
void set_default(mcu_config_t *c);
int is_valid(mcu_config_t *c);

int write_config_to_flash(mcu_config_t *c);

int write_buf_to_flash(uint8_t *src_buf, uint32_t dst_address, uint16_t len);
void read_buf_from_flash(uint32_t src_address, uint8_t *dst_buf, uint16_t len);


#endif // __USER_FLASH_H
