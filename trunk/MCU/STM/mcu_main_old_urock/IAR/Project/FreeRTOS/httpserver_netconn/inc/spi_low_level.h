#ifndef __SPI_CPLD_H
#define __SPI_CPLD_H

#include "sys_include.h"


/* SPI to CPLD Interface pins  */ 

// PA15	SPI3_NSS
// PC10	SPI3_SCK
// PC11	SPI3_MISO
// PC12	SPI3_MOSI
#define CPLD_SPI                           SPI3
#define CPLD_SPI_CLK                       RCC_APB1Periph_SPI3
#define CPLD_SPI_CLK_INIT                  RCC_APB1PeriphClockCmd

#define CPLD_SPI_SCK_PIN                   GPIO_Pin_10
#define CPLD_SPI_SCK_GPIO_PORT             GPIOC
#define CPLD_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOC
#define CPLD_SPI_SCK_SOURCE                GPIO_PinSource10
#define CPLD_SPI_SCK_AF                    GPIO_AF_SPI3

#define CPLD_SPI_MISO_PIN                  GPIO_Pin_11
#define CPLD_SPI_MISO_GPIO_PORT            GPIOC
#define CPLD_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define CPLD_SPI_MISO_SOURCE               GPIO_PinSource11
#define CPLD_SPI_MISO_AF                   GPIO_AF_SPI3

#define CPLD_SPI_MOSI_PIN                  GPIO_Pin_12
#define CPLD_SPI_MOSI_GPIO_PORT            GPIOC
#define CPLD_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define CPLD_SPI_MOSI_SOURCE               GPIO_PinSource12
#define CPLD_SPI_MOSI_AF                   GPIO_AF_SPI3


/*  Relay boards chip select pins  */

#define SPI_NSS_PIN 					GPIO_Pin_15
#define SPI_NSS_GPIO_PORT 				GPIOA
#define SPI_NSS_GPIO_CLK 				RCC_AHB1Periph_GPIOA



// sel_mode
// cpld_mode	sel_xc3		PB4	 (out)	 - must be 	1 for jtag operation
//													0 for SPI mode

#define CPLD_MODE_PORT			GPIOB
#define CPLD_MODE_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define CPLD_MODE_PIN			GPIO_Pin_4


// sel_channel_0	sel_xc0		PD4
// sel_channel_1	sel_xc1		PD7
// sel_channel_2	sel_xc2		PB3

#define CPLD_SEL_CHAN_0_PORT		GPIOD
#define CPLD_SEL_CHAN_0_GPIO_CLK	RCC_AHB1Periph_GPIOD
#define CPLD_SEL_CHAN_0_PIN			GPIO_Pin_4

#define CPLD_SEL_CHAN_1_PORT		GPIOD
#define CPLD_SEL_CHAN_1_GPIO_CLK	RCC_AHB1Periph_GPIOD
#define CPLD_SEL_CHAN_1_PIN			GPIO_Pin_7

#define CPLD_SEL_CHAN_2_PORT		GPIOB
#define CPLD_SEL_CHAN_2_GPIO_CLK	RCC_AHB1Periph_GPIOB
#define CPLD_SEL_CHAN_2_PIN			GPIO_Pin_3


void set_cpld_mode_high();
void set_cpld_mode_low();
void set_cpld_channel(unsigned char channel);
void sys_cpld_bus_init();


#define SPI_DUMMY_BYTE         0xFF


// each type of board, connected by spi to mcu board should have uniq ID
#define RELAY_ID			0x84


/* CPLD cmds -----------------------------------------------------------------*/

#define CPLD_CMD_READ		0x01
#define CPLD_CMD_WRITE		0x02
#define CPLD_CMD_READ_ID	0x03




// exported functions
void init_spi(void);
void relay_write_reg_spi(uint8_t board, uint8_t data_val);
uint8_t relay_read_reg_spi(uint8_t board);
uint8_t spi_read_id(uint8_t board);


// internal functions
void SPI_LowLevel_Init(void);
void SPI_LowLevel_DeInit(void);
uint8_t SPI_SendRecieve(uint8_t byte);
uint8_t SPI_Send_Command(uint8_t board, uint8_t cmd, uint8_t reg_val);


void spi_nss_init(void);
void spi_nss_low();
void spi_nss_high();



#endif // __SPI_CPLD_H
