#ifndef __FTDI_API_H
#define __FTDI_API_H


int ftdi_init(FT_HANDLE	*ftHandle, int dev_num);

int ftdi_send_byte(FT_HANDLE	ftHandle, unsigned char data8);
int ftdi_receive_byte(FT_HANDLE	ftHandle, unsigned char *data8); 
int ftdi_send_buffer(FT_HANDLE	ftHandle, unsigned char *buffer, int length);
int ftdi_receive_buffer(FT_HANDLE	ftHandle, unsigned char *buffer, int length);
FT_STATUS ftdi_set_dtr_high_and_rst(FT_HANDLE ftHandle); 
FT_STATUS ftdi_set_dtr_low_and_rst(FT_HANDLE ftHandle);
int ftdi_set_cbus_pins(FT_HANDLE ftHandle, int pins_val);
int ftdi_program_eeprom(FT_HANDLE ftHandle); 

#endif // __FTDI_API_H