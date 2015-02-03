/*******************************************************/
/* file: ports.c                                       */
/* abstract:  This file contains the routines to       */
/*            output values on the JTAG ports, to read */
/*            the TDO bit, and to read a byte of data  */
/*            from the prom                            */
/* Revisions:                                          */
/* 12/01/2008:  Same code as before (original v5.01).  */
/*              Updated comments to clarify instructions.*/
/*              Add print in setPort for xapp058_example.exe.*/
/*******************************************************/
#include "ports.h"
#include "main.h"
#include "user_timer.h"

// this buffer contains xsvf file received from host
extern uint8_t Buffer_Block_Rx[RX_BLOCK_SIZE];  

static uint8_t  *xsvf_data;

int jtag_mode = 1; 		// what to configure? 1 - CPLD on MCU board; 0 - CPLD on POWER_* boards

// init pins for configuring CPLD on MCU board
void sys_jtag_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	board_jtag_deinit(); 
		  
	RCC_AHB1PeriphClockCmd(MCU_SYS_JTAG_GPIO_CLK, ENABLE);
	
	// TMS, TCK, TDI - outputs
	GPIO_InitStructure.GPIO_Pin = MCU_SYS_TMS_PIN | MCU_SYS_TCK_PIN | MCU_SYS_TDI_PIN; 	
		  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MCU_SYS_JTAG_PORT, &GPIO_InitStructure);  
	
	// TDO - input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Pin = MCU_SYS_TDO_PIN;
	GPIO_Init(MCU_SYS_JTAG_PORT, &GPIO_InitStructure);  	
	
	
	jtag_mode = 1; 

}

// init pins for configuring CPLD on POWER_* boards
void board_jtag_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
		  
	sys_jtag_deinit();
	
	RCC_AHB1PeriphClockCmd(MCU_BOARD_JTAG_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(MCU_BOARD_TMS_GPIO_CLK, ENABLE);
	
	RCC_AHB1PeriphClockCmd(CPLD_MODE_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(CPLD_SEL_CHAN_0_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(CPLD_SEL_CHAN_1_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(CPLD_SEL_CHAN_2_GPIO_CLK, ENABLE);
	
	
	
	// outputs
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// TCK, TDI 
	GPIO_InitStructure.GPIO_Pin = MCU_BOARD_TCK_PIN | MCU_BOARD_TDI_PIN; 	
	GPIO_Init(MCU_BOARD_JTAG_PORT, &GPIO_InitStructure);  

	// TMS 
	GPIO_InitStructure.GPIO_Pin = MCU_BOARD_TMS_PIN; 	
	GPIO_Init(MCU_BOARD_TMS_PORT, &GPIO_InitStructure);  

	// TDO - input
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Pin = MCU_BOARD_TDO_PIN;
	GPIO_Init(MCU_BOARD_JTAG_PORT, &GPIO_InitStructure);  	


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	// init cpld_mode pin
	GPIO_InitStructure.GPIO_Pin = CPLD_MODE_PIN; 	
	GPIO_Init(CPLD_MODE_PORT, &GPIO_InitStructure);  	
	
  	// init sel_channel_x pins
	GPIO_InitStructure.GPIO_Pin = CPLD_SEL_CHAN_0_PIN; 	
	GPIO_Init(CPLD_SEL_CHAN_0_PORT, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin = CPLD_SEL_CHAN_1_PIN; 	
	GPIO_Init(CPLD_SEL_CHAN_1_PORT, &GPIO_InitStructure);  	

	GPIO_InitStructure.GPIO_Pin = CPLD_SEL_CHAN_2_PIN; 	
	GPIO_Init(CPLD_SEL_CHAN_2_PORT, &GPIO_InitStructure);  	

	
	jtag_mode = 0; 

}


	
void set_cpld_mode_high()
{
	CPLD_MODE_PORT->BSRRL = CPLD_MODE_PIN;	
}

void set_cpld_mode_low()
{
	CPLD_MODE_PORT->BSRRH = CPLD_MODE_PIN;
}

void set_cpld_channel(unsigned char channel)
{
  switch(channel) {
	  case 1:			
	CPLD_SEL_CHAN_0_PORT->BSRRH = CPLD_SEL_CHAN_0_PIN;		// 0
	CPLD_SEL_CHAN_1_PORT->BSRRH = CPLD_SEL_CHAN_1_PIN;		// 0
	CPLD_SEL_CHAN_2_PORT->BSRRH = CPLD_SEL_CHAN_2_PIN;  	// 0
		  break;
	
	  case 2:
	CPLD_SEL_CHAN_0_PORT->BSRRL = CPLD_SEL_CHAN_0_PIN;		// 1
	CPLD_SEL_CHAN_1_PORT->BSRRH = CPLD_SEL_CHAN_1_PIN;		// 0
	CPLD_SEL_CHAN_2_PORT->BSRRH = CPLD_SEL_CHAN_2_PIN;  	// 0
	break;
	
	  case 3:
	CPLD_SEL_CHAN_0_PORT->BSRRH = CPLD_SEL_CHAN_0_PIN;		// 0
	CPLD_SEL_CHAN_1_PORT->BSRRL = CPLD_SEL_CHAN_1_PIN;		// 1
	CPLD_SEL_CHAN_2_PORT->BSRRH = CPLD_SEL_CHAN_2_PIN;  	// 0
	break;
	
	  case 4:
	CPLD_SEL_CHAN_0_PORT->BSRRL = CPLD_SEL_CHAN_0_PIN;		// 1
	CPLD_SEL_CHAN_1_PORT->BSRRL = CPLD_SEL_CHAN_1_PIN;		// 1
	CPLD_SEL_CHAN_2_PORT->BSRRH = CPLD_SEL_CHAN_2_PIN;  	// 0
	break;
	
	  case 5:
	CPLD_SEL_CHAN_0_PORT->BSRRH = CPLD_SEL_CHAN_0_PIN;		// 0
	CPLD_SEL_CHAN_1_PORT->BSRRH = CPLD_SEL_CHAN_1_PIN;		// 0
	CPLD_SEL_CHAN_2_PORT->BSRRL = CPLD_SEL_CHAN_2_PIN;  	// 1
	break;
	
	  case 6:
	CPLD_SEL_CHAN_0_PORT->BSRRL = CPLD_SEL_CHAN_0_PIN;		// 1
	CPLD_SEL_CHAN_1_PORT->BSRRH = CPLD_SEL_CHAN_1_PIN;		// 0
	CPLD_SEL_CHAN_2_PORT->BSRRL = CPLD_SEL_CHAN_2_PIN;  	// 1
	break;
	
	  case 7:
	CPLD_SEL_CHAN_0_PORT->BSRRH = CPLD_SEL_CHAN_0_PIN;		// 0
	CPLD_SEL_CHAN_1_PORT->BSRRL = CPLD_SEL_CHAN_1_PIN;		// 1
	CPLD_SEL_CHAN_2_PORT->BSRRL = CPLD_SEL_CHAN_2_PIN;  	// 1
	break;
	
	  case 8:
	CPLD_SEL_CHAN_0_PORT->BSRRL = CPLD_SEL_CHAN_0_PIN;		// 1
	CPLD_SEL_CHAN_1_PORT->BSRRL = CPLD_SEL_CHAN_1_PIN;		// 1
	CPLD_SEL_CHAN_2_PORT->BSRRL = CPLD_SEL_CHAN_2_PIN;  	// 1
	break;
	
	  default: 
		  break;
  }
}


void sys_jtag_deinit()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	// TMS, TCK, TDI, TDO - inputs
	GPIO_InitStructure.GPIO_Pin = MCU_SYS_TMS_PIN | MCU_SYS_TCK_PIN | MCU_SYS_TDI_PIN | MCU_SYS_TDO_PIN; 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
	GPIO_Init(MCU_SYS_JTAG_PORT, &GPIO_InitStructure);  	  
}

void board_jtag_deinit()
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  

	// TCK, TDI, TDO - inputs
	GPIO_InitStructure.GPIO_Pin = MCU_BOARD_TCK_PIN | MCU_BOARD_TDO_PIN | MCU_BOARD_TDI_PIN; 	
	GPIO_Init(MCU_BOARD_JTAG_PORT, &GPIO_InitStructure);  	  

	// TMS - inputs
	GPIO_InitStructure.GPIO_Pin = MCU_BOARD_TMS_PIN; 	
	GPIO_Init(MCU_BOARD_TMS_PORT, &GPIO_InitStructure);  	  
}



  
/* setPort:  Implement to set the named JTAG signal (p) to the new value (v).*/

void setPort(short p,short val)
{
  
  if (jtag_mode == 1) {		// system CPLD
  	if (p==TMS) 
		if (val & 1) 										// 1
			MCU_SYS_JTAG_PORT->BSRRL = MCU_SYS_TMS_PIN;
		else												// 0
			MCU_SYS_JTAG_PORT->BSRRH = MCU_SYS_TMS_PIN;
	  
    if (p==TDI)
		if (val & 1) 										// 1
			MCU_SYS_JTAG_PORT->BSRRL = MCU_SYS_TDI_PIN;
		else												// 0
			MCU_SYS_JTAG_PORT->BSRRH = MCU_SYS_TDI_PIN;
		
    if (p==TCK) 
		if (val & 1) 										// 1
			MCU_SYS_JTAG_PORT->BSRRL = MCU_SYS_TCK_PIN;
		else												// 0
			MCU_SYS_JTAG_PORT->BSRRH = MCU_SYS_TCK_PIN;
		
  } else {					// board CPLD

  	if (p==TMS) 
		if (val & 1) 										// 1
			MCU_BOARD_TMS_PORT->BSRRL = MCU_BOARD_TMS_PIN;
		else												// 0
			MCU_BOARD_TMS_PORT->BSRRH = MCU_BOARD_TMS_PIN;
	  
    if (p==TDI)
		if (val & 1) 										// 1
			MCU_BOARD_JTAG_PORT->BSRRL = MCU_BOARD_TDI_PIN;
		else												// 0
			MCU_BOARD_JTAG_PORT->BSRRH = MCU_BOARD_TDI_PIN;
		
    if (p==TCK) 
		if (val & 1) 										// 1
			MCU_BOARD_JTAG_PORT->BSRRL = MCU_BOARD_TCK_PIN;
		else												// 0
			MCU_BOARD_JTAG_PORT->BSRRH = MCU_BOARD_TCK_PIN;	
	
  }

}




/* toggle tck LH.  No need to modify this code.  It is output via setPort. */
void pulseClock()
{
    setPort(TCK,0);  /* set the TCK port to low  */
    setPort(TCK,1);  /* set the TCK port to high */
}



void xsvf_data_init()
{
  xsvf_data = Buffer_Block_Rx; 
}

/* readByte:  Implement to source the next byte from your XSVF file location */
/* read in a byte of data from the prom */
void readByte(unsigned char *data)
{
    *data=*xsvf_data++;
}

/* readTDOBit:  Implement to return the current value of the JTAG TDO signal.*/
/* read the TDO bit from port */
unsigned char readTDOBit()
{
  if (jtag_mode == 1) 
	return GPIO_ReadInputDataBit(MCU_SYS_JTAG_PORT, MCU_SYS_TDO_PIN);
  else
	return GPIO_ReadInputDataBit(MCU_BOARD_JTAG_PORT, MCU_BOARD_TDO_PIN);
	
}

/* waitTime:  Implement as follows: */
/* REQUIRED:  This function must consume/wait at least the specified number  */
/*            of microsec, interpreting microsec as a number of microseconds.*/
/* REQUIRED FOR SPARTAN/VIRTEX FPGAs and indirect flash programming:         */
/*            This function must pulse TCK for at least microsec times,      */
/*            interpreting microsec as an integer value.                     */
/* RECOMMENDED IMPLEMENTATION:  Pulse TCK at least microsec times AND        */
/*                              continue pulsing TCK until the microsec wait */
/*                              requirement is also satisfied.               */
void waitTime(long microsec)
{
    static long tckCyclesPerMicrosec    = 2; /* must be at least 1 */
    long        tckCycles   = microsec * tckCyclesPerMicrosec;
    long        i;

    /* This implementation is highly recommended!!! */
    /* This implementation requires you to tune the tckCyclesPerMicrosec 
       variable (above) to match the performance of your embedded system
       in order to satisfy the microsec wait time requirement. */
    for ( i = 0; i < tckCycles; ++i )
    {
		// wait for 500 ns
		TIM_SetCounter(TIM2,0); 
		while(TIM_GetCounter(TIM2) < 10); // dt = 50 ns; 
        pulseClock();
    }


}
