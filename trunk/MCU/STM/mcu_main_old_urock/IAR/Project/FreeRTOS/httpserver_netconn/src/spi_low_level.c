/** \file spi_cpld.c
 *  \brief File containing functions to work with CPLD over SPI bus



 */

#include "main.h"
#include "spi_low_level.h"






/**
  * \brief  Initializes SPI controller in polling mode

  * \return None
  */
void init_spi(void)
{
  SPI_InitTypeDef  SPI_InitStructure;

  SPI_LowLevel_Init();
    

  // SPI configuration 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;

  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(CPLD_SPI, &SPI_InitStructure);

  // Enable the CPLD_SPI  
  SPI_Cmd(CPLD_SPI, ENABLE);
}




void SPI_LowLevel_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;


  // NSS
  RCC_AHB1PeriphClockCmd(SPI_NSS_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin = SPI_NSS_PIN;

  spi_nss_high();

  // configure as output
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(SPI_NSS_GPIO_PORT, &GPIO_InitStructure);


  // Enable the SPI clock 
  CPLD_SPI_CLK_INIT(CPLD_SPI_CLK, ENABLE);

  // Enable GPIO clocks 
  RCC_AHB1PeriphClockCmd(CPLD_SPI_SCK_GPIO_CLK | CPLD_SPI_MISO_GPIO_CLK | 
                         CPLD_SPI_MOSI_GPIO_CLK, ENABLE);
  
  // SPI pins configuration ************************************************

  // Connect SPI pins to AF
  GPIO_PinAFConfig(CPLD_SPI_SCK_GPIO_PORT, CPLD_SPI_SCK_SOURCE, CPLD_SPI_SCK_AF);
  GPIO_PinAFConfig(CPLD_SPI_MISO_GPIO_PORT, CPLD_SPI_MISO_SOURCE, CPLD_SPI_MISO_AF);
  GPIO_PinAFConfig(CPLD_SPI_MOSI_GPIO_PORT, CPLD_SPI_MOSI_SOURCE, CPLD_SPI_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
        
  // SPI SCK pin configuration 
  GPIO_InitStructure.GPIO_Pin = CPLD_SPI_SCK_PIN;
  GPIO_Init(CPLD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  // SPI MOSI pin configuration 
  GPIO_InitStructure.GPIO_Pin =  CPLD_SPI_MOSI_PIN;
  GPIO_Init(CPLD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  // SPI MISO pin configuration 
  GPIO_InitStructure.GPIO_Pin =  CPLD_SPI_MISO_PIN;
  GPIO_Init(CPLD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);



  // init pins for sys cpld mode and channel selection
  sys_cpld_bus_init();


}



void SPI_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Disable the CPLD_SPI  ***********************************************
  SPI_Cmd(CPLD_SPI, DISABLE);
  
  // DeInitializes the CPLD_SPI ******************************************
  SPI_I2S_DeInit(CPLD_SPI);
  

  // CPLD_SPI Periph clock disable ***************************************
  CPLD_SPI_CLK_INIT(CPLD_SPI_CLK, DISABLE);

      
  // Configure all pins used by the SPI as input floating *****************
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

  GPIO_InitStructure.GPIO_Pin = CPLD_SPI_SCK_PIN;
  GPIO_Init(CPLD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CPLD_SPI_MISO_PIN;
  GPIO_Init(CPLD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = CPLD_SPI_MOSI_PIN;
  GPIO_Init(CPLD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

}




/*
  * Sends a byte through the SPI interface and return the byte received
  *         from the SPI bus.
  * @param  byte: byte to send.
  * @retval The value of the received byte.
  */
uint8_t SPI_SendRecieve(uint8_t byte)
{
  // Loop while DR register in not emplty 
  while (SPI_I2S_GetFlagStatus(CPLD_SPI, SPI_I2S_FLAG_TXE) == RESET);

  //  Send byte through the SPI peripheral 
  SPI_I2S_SendData(CPLD_SPI, byte);

  // Wait to receive a byte 
  while (SPI_I2S_GetFlagStatus(CPLD_SPI, SPI_I2S_FLAG_RXNE) == RESET);

  // Return the byte read from the SPI bus 
  return SPI_I2S_ReceiveData(CPLD_SPI);
}





/**
  * \brief  Sends two bytes commend to relay board
	
	\param cmd Selects opperation and register to access
	\param reg_val Register data to write

  * \return The value of the received byte
  */
uint8_t SPI_Send_Command(uint8_t board, uint8_t cmd, uint8_t reg_val)
{
	uint8_t ret_val;
  
	spi_nss_low();
	
	SPI_SendRecieve(cmd);
	ret_val = SPI_SendRecieve(reg_val);
	
	spi_nss_high();
	
	return ret_val; 	
}




// board - board index in range [1..8]
void relay_write_reg_spi(uint8_t board, uint8_t data_val)
{

	//select channel
	// sel mode = 0 - SPI
	set_cpld_mode_low();
	// sel channel
	set_cpld_channel(board);

	SPI_Send_Command(board, CPLD_CMD_WRITE, data_val);
}


// board - board index in range [1..8]
uint8_t relay_read_reg_spi(uint8_t board)
{
	//select channel
	// sel mode = 0 - SPI
	set_cpld_mode_low();
	// sel channel
	set_cpld_channel(board);

	return SPI_Send_Command(board, CPLD_CMD_READ, 1);
}



// board - board index in range [1..8]
uint8_t spi_read_id(uint8_t board)
{
	//select channel
	// sel mode = 0 - SPI
	set_cpld_mode_low();
	// sel channel
	set_cpld_channel(board);

	return SPI_Send_Command(board, CPLD_CMD_READ_ID, 1);
}



void spi_nss_low()
{
	SPI_NSS_GPIO_PORT->BSRRH = SPI_NSS_PIN;
}

void spi_nss_high()
{
	SPI_NSS_GPIO_PORT->BSRRL = SPI_NSS_PIN;
}




// functions for CPLD on MCU board
// select channel and mode

void sys_cpld_bus_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;


	RCC_AHB1PeriphClockCmd(CPLD_MODE_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(CPLD_SEL_CHAN_0_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(CPLD_SEL_CHAN_1_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(CPLD_SEL_CHAN_2_GPIO_CLK, ENABLE);


	// outputs
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

}

void set_cpld_mode_high()	// sel JTAG mode
{
	CPLD_MODE_PORT->BSRRL = CPLD_MODE_PIN;
}

void set_cpld_mode_low()	// sel SPI mode
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


