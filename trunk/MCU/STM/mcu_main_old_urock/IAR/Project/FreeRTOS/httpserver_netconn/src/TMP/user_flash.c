/** \file user_flash.c
 *  \brief File containing functions to work with STM Flash memory

	Last Flash Sector is used to store user data
	Sector size = 128 Kbytes
	Sector BASE Address = 0x080E0000
	Sector High Address = 0x080FFFFF
	
 */
 
   
#include "main.h"
#include "user_flash.h"



/** 
* @brief Writes buffer to Flash at constant address 
* @param buf Pointer to write buffer
* @param len Number of DWORDs (32 bits) to write
*
* Flash Addess is defined by USER_FLASH_BASE
*
* @return <0 on error
 */
int write_buf_to_flash(uint8_t *buf, int len, int address)
{
	int i; 
	
	uint32_t 	Address = 0; 
	
	// Unlock the Flash to enable the flash control register access 
	FLASH_Unlock();
    

	//Clear pending flags (if any) 
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	//FLASH_OB_WRPConfig(OB_WRP_Sector_11, DISABLE);
        if (address == USER_FLASH_INFO){
          if (FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3) != FLASH_COMPLETE)
                  return FLASH_ERROR_ERASE;            
        }
        
        if (address == USER_FLASH_BASE){
          if (FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3) != FLASH_COMPLETE)
                  return FLASH_ERROR_ERASE;  
        }
	

//	Address = USER_FLASH_BASE;
        Address = address;
        
		
	for (i=0; i<len; i++) {
		if (FLASH_ProgramByte(Address,buf[i]) != FLASH_COMPLETE) 
			return FLASH_ERROR_WRITE; 
		Address++;
	}


	
	FLASH_Lock(); 
	
	return FLASH_OK; 
	
}


/** 
* @brief Reads buffer from Flash from constant address 
* @param buf Pointer to read buffer
* @param len Number of DWORDs (32 bits) to read
*
*	Flash Addess is defined by USER_FLASH_BASE
*
* @ return <0 on error
 */
void read_buf_from_flash(uint8_t *buf, int len, int address)
{
	int i; 
	uint8_t 	*ptr_flash; 
        
			
	ptr_flash =(uint8_t*)address;
		
	for (i=0; i<len; i++)
		buf[i] = *ptr_flash++; 
}


//-------------------------------------------------
/** 
* @brief Writes buffer to Flash at constant address 
* @param buf Pointer to write buffer
* @param len Number of DWORDs (32 bits) to write
*
* Flash Addess is defined by USER_FLASH_BASE
*
* @return <0 on error
 */
int clear_flash(int address)
//int write_buf_to_flash(uint8_t *buf, int len, int address)
{
	// Unlock the Flash to enable the flash control register access 
	FLASH_Unlock();
    

	//Clear pending flags (if any) 
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

	//FLASH_OB_WRPConfig(OB_WRP_Sector_10, DISABLE);
        if (address == USER_FLASH_INFO){
          if (FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3) != FLASH_COMPLETE)
                  return FLASH_ERROR_ERASE;            
        }
        
        if (address == USER_FLASH_BASE){
          if (FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3) != FLASH_COMPLETE)
                  return FLASH_ERROR_ERASE;  
        }
	
	FLASH_Lock(); 
}
