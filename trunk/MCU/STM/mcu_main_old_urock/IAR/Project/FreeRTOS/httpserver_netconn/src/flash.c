/** \file user_flash.c
 *  \brief File containing functions to work with STM Flash memory

	Last Flash Sector is used to store user data
	Sector size = 128 Kbytes
	Sector BASE Address = 0x080E0000
	Sector High Address = 0x080FFFFF
	
 */
 
   
#include "main.h"
#include "flash.h"


void init_config_t(mcu_config_t *config_out)
{

	read_buf_from_flash(MCU_CONFIG_BASE, (uint8_t *)config_out, sizeof(mcu_config_t));

	if (!is_valid(config_out)) {

		set_default(config_out);

		msg_dbg(("mcu_config_t is not valid, setting defaults\n\r"));

	}



}

void set_default(mcu_config_t *c)
{
	c->SN[0] = 0; c->SN[1] = 0; c->SN[2] = 0; c->SN[3] = 1;

	c->stm_mac[0] = MAC_ADDR0; c->stm_mac[1] = MAC_ADDR1; c->stm_mac[2] = MAC_ADDR2;
	c->stm_mac[3] = MAC_ADDR3; c->stm_mac[4] = MAC_ADDR4; c->stm_mac[5] = MAC_ADDR5;

	c->stm_ip[0] = IP_ADDR0; c->stm_ip[1] = IP_ADDR1; c->stm_ip[2] = IP_ADDR2; c->stm_ip[3] = IP_ADDR3;

	c->stm_mask[0] = NETMASK_ADDR0; c->stm_mask[1] = NETMASK_ADDR1; c->stm_mask[2] = NETMASK_ADDR2; c->stm_mask[3] = NETMASK_ADDR3;

	c->stm_gw[0] = GW_ADDR0; c->stm_gw[1] = GW_ADDR1; c->stm_gw[2] = GW_ADDR2; c->stm_gw[3] = GW_ADDR3;

	c->stm_tcp_server_port = TCP_SERVER_PORT;

	c->host_ip[0] = HOST_IP_ADDR0; c->host_ip[1] = HOST_IP_ADDR1; c->host_ip[2] = HOST_IP_ADDR2; c->host_ip[3] = HOST_IP_ADDR3;

	c->host_tcp_server_port = HOST_PORT;

}

int is_valid(mcu_config_t *c)
{
	// for now just check serial number filed
	// TODO: add CRC check

	//if SN = 26.04.20.14 - then is valid
	if ((c->SN[0] == 26) && (c->SN[1] == 04) && (c->SN[2] == 20) && (c->SN[3] == 14))
		return 1;
	else
		return 0;
}



int write_config_to_flash(mcu_config_t *c)
{

	return write_buf_to_flash((uint8_t *)c, MCU_CONFIG_BASE, sizeof(mcu_config_t));
}

/** 
* @brief Writes buffer to Sector 11 Flash
* @param src_buf Pointer to write buffer
* @param dst_address - flash address - MUST belong to Sector 11
* @param len Number of bytes to write
*
* *
* @return <0 on error
 */
int write_buf_to_flash(uint8_t *src_buf, uint32_t dst_address, uint16_t len)
{
	int i; 
	
	uint32_t 	Address = 0; 
	
	// check is flash dst_address belongs to Sector 11
	if (!IS_SECTOR_11(dst_address))
		return FLASH_WRONG_ADDR;

	// Unlock the Flash to enable the flash control register access 
	FLASH_Unlock();
    

	//Clear pending flags (if any) 
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	//FLASH_OB_WRPConfig(OB_WRP_Sector_11, DISABLE);


	if (FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3) != FLASH_COMPLETE)
		return FLASH_ERROR_ERASE;
	


	Address = dst_address;
        
		
	for (i=0; i<len; i++) {
		if (FLASH_ProgramByte(Address,src_buf[i]) != FLASH_COMPLETE)
			return FLASH_ERROR_WRITE; 
		Address++;
	}


	
	FLASH_Lock(); 
	
	return FLASH_OK; 
	
}


/** 
* @brief Reads buffer from Flash
* @param src_address Flash address
* @param dst_buf Pointer to read buffer
* @param len Number of bytes to read
*
*	Flash Addess is defined by USER_FLASH_BASE
*
 */
void read_buf_from_flash(uint32_t src_address, uint8_t *dst_buf, uint16_t len)
{
	uint16_t 	i;
	uint8_t 	*ptr_flash; 
        
			
	ptr_flash =(uint8_t*)src_address;
		
	for (i=0; i<len; i++)
		dst_buf[i] = *ptr_flash++;
}

