/*******************************************************/
/* file: ports.h                                       */
/* abstract:  This file contains extern declarations   */
/*            for providing stimulus to the JTAG ports.*/
/*******************************************************/

#ifndef ports_dot_h
#define ports_dot_h


// JTAG Bus pins

// SYS_TMS			PD2 (out)			-- 
// SYS_TCK			PD3 (out)			-- 
// SYS_TDO			PD0 (in)			-- 
// SYS_TDI			PD1 (out)			-- 

#define MCU_SYS_JTAG_PORT			GPIOD
#define MCU_SYS_JTAG_GPIO_CLK		        RCC_AHB1Periph_GPIOD
#define MCU_SYS_TMS_PIN				GPIO_Pin_2
#define MCU_SYS_TCK_PIN          		GPIO_Pin_3
#define MCU_SYS_TDO_PIN				GPIO_Pin_0
#define MCU_SYS_TDI_PIN				GPIO_Pin_1


// BOARD_TMS		SPI _NSS	PA15 (out)			-- 
// BOARD_TCK		SPI3_SCK	PC10 (out)			-- 
// BOARD_TDO		SPI3_MISO	PC11 (in)			-- 
// BOARD_TDI		SPI3_MOSI	PC12 (out)			-- 

#define MCU_BOARD_JTAG_PORT			GPIOC
#define MCU_BOARD_JTAG_GPIO_CLK		        RCC_AHB1Periph_GPIOC
#define MCU_BOARD_TCK_PIN        	        GPIO_Pin_10
#define MCU_BOARD_TDO_PIN			GPIO_Pin_11
#define MCU_BOARD_TDI_PIN			GPIO_Pin_12

#define MCU_BOARD_TMS_PORT			GPIOA
#define MCU_BOARD_TMS_GPIO_CLK		        RCC_AHB1Periph_GPIOB
#define MCU_BOARD_TMS_PIN			GPIO_Pin_15

// sel_mode
// cpld_mode	sel_xc3		PB4	 (out)	 - must be 1 for jtag operation

#define CPLD_MODE_PORT			GPIOB
#define CPLD_MODE_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define CPLD_MODE_PIN			GPIO_Pin_4


// sel_channel_0	sel_xc0		PD4
// sel_channel_1	sel_xc1		PD7
// sel_channel_2	sel_xc2		PB3

#define CPLD_SEL_CHAN_0_PORT		GPIOD
#define CPLD_SEL_CHAN_0_GPIO_CLK	RCC_AHB1Periph_GPIOD
#define CPLD_SEL_CHAN_0_PIN		GPIO_Pin_4

#define CPLD_SEL_CHAN_1_PORT		GPIOD
#define CPLD_SEL_CHAN_1_GPIO_CLK	RCC_AHB1Periph_GPIOD
#define CPLD_SEL_CHAN_1_PIN		GPIO_Pin_7

#define CPLD_SEL_CHAN_2_PORT		GPIOB
#define CPLD_SEL_CHAN_2_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define CPLD_SEL_CHAN_2_PIN		GPIO_Pin_3




void sys_jtag_init();
void sys_jtag_deinit();
void board_jtag_init();
void board_jtag_deinit();


void set_cpld_mode_high();
void set_cpld_mode_low();
void set_cpld_channel(unsigned char channel);


/* these constants are used to send the appropriate ports to setPort */
/* they should be enumerated types, but some of the microcontroller  */
/* compilers don't like enumerated types */
#define TCK (short) 0
#define TMS (short) 1
#define TDI (short) 2

void xsvf_data_init();

/* set the port "p" (TCK, TMS, or TDI) to val (0 or 1) */
extern void setPort(short p, short val);

/* read the TDO bit and store it in val */
extern unsigned char readTDOBit();

/* make clock go down->up->down*/
extern void pulseClock();

/* read the next byte of data from the xsvf file */
extern void readByte(unsigned char *data);

extern void waitTime(long microsec);

#endif
