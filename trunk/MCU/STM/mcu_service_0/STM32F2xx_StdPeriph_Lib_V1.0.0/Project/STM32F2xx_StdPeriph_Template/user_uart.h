#ifndef __USER_UART_H
#define __USER_UART_H




#define STATE_IDLE 							0

#define STATE_WRITE_XSVF					3
#define STATE_READ_XSVF						4
#define STATE_PROG_CPLD						5



void user_usart1_init(int baud_date); // to ftdi

//void user_usart_init(void);

void user_put_char(uint8_t ch); 
void user_put_char_no_wait(uint8_t ch);

void user_usart_irq_handler(void); 
void user_tx_buf_usart(uint8_t *buf, uint16_t buf_len); 


#endif // __USER_UART_H
