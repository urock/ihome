/** \file user_uart.c
 *  \brief File containing functions to work with USART interface with USB HOST

RC47 	HOST USART

USART_1 TX	- PA9

USART_1 RX	- PA10

 */

#include "main.h"
#include "user_uart.h"
#include "stm_host.h"







uint8_t *UsartTxBufferPtr; 	//!< Pointer to Buffer set to transmit over USART1
uint16_t UsartNumBytes; 	//!< USART1 Transmit Total Byte Number

uint8_t uart_char; 			//!< Buffer byte used when tramsitting one byte


uint8_t state = STATE_IDLE; //!< State var used in USART ISR RX state machine
uint8_t ss = 0; 			//!< Yet another State var used in USART ISR RX state machine

struct cmd_frame_s uframe; //!< Cmd from host received by USART

__IO uint8_t need_uframe_dpc; //!< Flag set in USART ISR to schedule delayed procedure call for USART command


__IO uint8_t usart_stm_ready = 0;			//!< Flag set in USART DPC when smth is done; 
__IO uint8_t usart_ready_to_send_data = 0;	//!< flag set in ISR to tell DPC to finish data transfer

// Buffer_Block_Rx is used to store data buffers while data transfer with host by USART
uint8_t Buffer_Block_Rx[RX_BLOCK_SIZE];  

int buf_index = 0; 







/** 
 *  \brief Init USART1 to use interrupts
 	\param baud_date USART Baud Rate (Can be up to 3M)

	\return None
 */
void user_usart1_init(int baud_date)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);

  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);		// RX
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,  GPIO_AF_USART1);    	// TX

  // Configure USART Tx as alternate function  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure USART Rx as alternate function  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  
  USART_InitStructure.USART_BaudRate = baud_date;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  
  	// USART Line Interrupt 
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);	  


  // USART configuration 
  USART_Init(USART1, &USART_InitStructure);
    
  // Enable USART 
  USART_Cmd(USART1, ENABLE);  
  
  /* Enable the USART1 Receive interrupt: this interrupt is generated when the 
     EVAL_COM1 receive data register is not empty */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  
  
}





#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */






PUTCHAR_PROTOTYPE
{
  
  user_put_char(ch); 

  return ch;
}



/** 
 *  \brief Sends byte over USART1 and waits for completeion of transfer
 	\param ch Byte to be transferred

	\return None
 */
void user_put_char(uint8_t ch)
{
  
  UsartNumBytes = 1; 
  uart_char = ch; 
  UsartTxBufferPtr = &uart_char; 
    /* Enable the USART1 Transmit interrupt: this interrupt is generated when the 
     EVAL_COM1 transmit data register is empty */  
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}
}

/** 
 *  \brief Sends byte over USART1 without waiting for completeion of transfer
 	\param ch Byte to be transferred

	\return None
 */
void user_put_char_no_wait(uint8_t ch)
{
  UsartNumBytes = 1; 
  uart_char = ch; 
  UsartTxBufferPtr = &uart_char; 
    /* Enable the USART1 Transmit interrupt: this interrupt is generated when the 
     EVAL_COM1 transmit data register is empty */  
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 
}

/** 
 *  \brief Sends buffer over USART1 and waits for completeion of transfer
 	\param buf Pointer to global array to be transferred
	\param buf_len Number of bytes to be transfered

	\return None
 */
void user_tx_buf_usart(uint8_t *buf, uint16_t buf_len) {
  
  UsartNumBytes = buf_len; 
  UsartTxBufferPtr = buf; 
    /* Enable the USART1 Transmit interrupt: this interrupt is generated when the 
     EVAL_COM1 transmit data register is empty */  
  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
  {}  
}









/** 
 *  \brief USART1 ISR. Sends data from UsartTxBufferPtr and handles received data in state machine fashion 
	
Transmitting data:
	1. 	Program calls user_put_char or user_tx_buf_usart which sets UsartTxBufferPtr and UsartNumBytes and enables 
		Transmit interrupt (USART_IT_TXE) which is generated when tx data register is empty
	2.	USART_IT_TXE interrupt is generated where byte is trasmitted and UsartNumBytes decreased
	3. 	Bytes are transmitted one by one. After transmitting the last one USART_IT_TXE is disabled
	
Receiving data:
	1.	USART1 Receive interrupt (USART_IT_RXNE) is enabled at init time 
	2. 	New received byte is written to rx_byte and then analyzed.
	3. 	Depending on the command code state machine will receiv needed amount of data (frame size) and schedule DPC (stm_host.c)
	4.	DPC will do some work and finish request from host

\return None
 */
void user_usart_irq_handler(void)
{
  
  uint8_t rx_byte; 
  
  if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  {   
    /* Write one byte to the transmit data register */
    USART_SendData(USART1, *UsartTxBufferPtr++);
	UsartNumBytes--; 

    if(UsartNumBytes == 0) {
      /* Disable the EVAL_COM1 Transmit interrupt */
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }	 
  }
  
  
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
	  
	   	rx_byte = (uint8_t)  USART_ReceiveData(USART1);
		
			switch(state) {
		  
			case STATE_IDLE:
				uframe.cmd_code = rx_byte;
		  
				switch (uframe.cmd_code) {
	
					case CMD_WRITE_XSVF: 
						state = STATE_WRITE_XSVF; ss = 0;
						break; 
		
					case CMD_READ_XSVF: 
						state = STATE_READ_XSVF; ss = 0;
						break; 
						
					case CMD_PROG_CPLD:
					  	state = STATE_PROG_CPLD;
					 	break;
						
					default:
						need_uframe_dpc = 1;		
				
		
			
				}
		  
				// disable timer interrupt during transfer data
				// it will be enabled after complition of uframe_dpc
				TIM_ITConfig(TIM3, TIM_IT_Update , DISABLE);	  
						  
				break;
		
			// end  STATE_IDLE
				
	
				
				
			case STATE_WRITE_XSVF:
				switch (ss) {
				case 0: uframe.cmd_data.xsvf_data.xsvf_position = rx_byte; 			ss = 1; break; 
				case 1: uframe.cmd_data.xsvf_data.xsvf_position += (rx_byte << 8);	ss = 2; break; 
				case 2: uframe.cmd_data.xsvf_data.xsvf_len = rx_byte; 				ss = 3; break; 	
				case 3: 
					uframe.cmd_data.xsvf_data.xsvf_len += (rx_byte << 8);			ss = 4; 
					buf_index = 0; 
					user_put_char_no_wait(CMD_ACK); 
					
					break; 
				case 4: 
				  
					Buffer_Block_Rx[uframe.cmd_data.xsvf_data.xsvf_position + buf_index++] = rx_byte;
					
					if (buf_index == uframe.cmd_data.xsvf_data.xsvf_len) {  // got all data
						user_put_char_no_wait(CMD_CMLP); 
						ss = 0; state = STATE_IDLE;		
					}
				  
					break; 
				  
				default: ss = 0; state = STATE_IDLE; break;
				}
				
				break; 
				
			// end  STATE_WRITE_XSVF
				
	
			case STATE_READ_XSVF:
				switch (ss) {
				case 0: uframe.cmd_data.xsvf_data.xsvf_position = rx_byte; 			ss = 1; break; 
				case 1: uframe.cmd_data.xsvf_data.xsvf_position += (rx_byte << 8);	ss = 2; break; 
				case 2: uframe.cmd_data.xsvf_data.xsvf_len = rx_byte; 				ss = 3; break; 	
				case 3: 
					uframe.cmd_data.xsvf_data.xsvf_len += (rx_byte << 8);			ss = 4; 
					ss = 0; 
					state = STATE_IDLE;		
					need_uframe_dpc = 1; 
					
					break; 
	
				  
				default: ss = 0; state = STATE_IDLE; break;
				}
				
				break; 
			// end  STATE_READ_XSVF		
				
				
			
			case STATE_PROG_CPLD:
			  	uframe.cmd_data.prog_cpld_data.cpld_selector = rx_byte;
				state = STATE_IDLE;		
				need_uframe_dpc = 1; 
				break;
				
			// end  CMD_PROG_CPLD		
				
				
				  
			default:
				break; 
		  
		}
	
	
	
	
  }

  
}






		





// undocumented not used here functions

//void user_get_string_usart(uint8_t *str);
//int usart_get_num_decimal(void);
//int usart_get_num_hex(void);
//
//uint16_t user_get_char(void);
//uint16_t user_get_char(void)
//{
//  uint16_t c; 
//  // Loop until the Read data register flag is SET 
//  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
//  c = USART_ReceiveData(USART1);
//  return (c);
//}

//void user_get_string_usart(uint8_t *str)
//{
//	int i=0;
//	while(1) {
//		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
//		
//		str[i] = (uint8_t)  USART_ReceiveData(USART1);
//		
//		if ((str[i] != 13) || (str[i] != 10) ) {
//			USART_SendData(USART1, (uint8_t) str[i]);
//
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//			  {}		  
//		}
//		if ( (i > 1) && (str[i-1] == 13) && (str[i] == 10) ) // end of string in Hercules
//		  break; 
//		
//		i++;
//			  
//	}
//	
//	// add null terminator
//	str[i-1]= 0; 
//	
//}
//
//int usart_get_num_decimal(void)
//{
//	char str[12];
//	int i=0, num;
//	
//	for(i=0; i<12; i++)
//	  str[i] = 0; 
//	
//	i=0; 
//	
//	while(1) {
//		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
//		
//		str[i] = (uint8_t)  USART_ReceiveData(USART1);
//		
//		if ((str[i] != 13) || (str[i] != 10) ) {
//			USART_SendData(USART1, (uint8_t) str[i]);
//
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//			  {}		  
//		}
//		if ( (i > 1) && (str[i-1] == 13) && (str[i] == 10) ) // end of string in Hercules
//		  break; 
//		
//		i++;
//			  
//	}
//	
//	// add null terminator
//	str[i-1]= 0; 
//	
//	sscanf(str,"%d",&num);
//	
//	return num;
//	
//}
//
//int usart_get_num_hex(void)
//{
//	char str[12];
//	int i=0, num;
//	
//	for(i=0; i<12; i++)
//	  str[i] = 0; 
//	
//	i=0; 
//	
//	while(1) {
//		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
//		
//		str[i] = (uint8_t)  USART_ReceiveData(USART1);
//		
//		if ((str[i] != 13) || (str[i] != 10) ) {
//			USART_SendData(USART1, (uint8_t) str[i]);
//
//			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
//			  {}		  
//		}
//		if ( (i > 1) && (str[i-1] == 13) && (str[i] == 10) ) // end of string in Hercules
//		  break; 
//		
//		i++;
//			  
//	}
//	
//	// add null terminator
//	str[i-1]= 0; 
//	
//	sscanf(str,"%x",&num);
//	
//	return num;
//	
//}

