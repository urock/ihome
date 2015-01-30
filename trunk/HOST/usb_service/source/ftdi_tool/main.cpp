// rsp529_config_ftdi.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "ftdi_api.h"




// link ftd2xx library - DLL library for i386 platform
#pragma comment(lib, "ftd2xx.lib")


FT_HANDLE		ftHandle; 


void print_usage(void);

int _tmain(int argc, _TCHAR* argv[])
{



	int			dev_num, pins_val;
	
	int			com_num; 



	/////////////////////////////////////////////////////////////////////////
	// open device

	if (!(wcscmp(argv[1],L"-device")))	{						// argv[1] == "-device"
		swscanf(argv[2],L"%d",&dev_num);		
//		printf("device = %d\n",dev_num); 
	} else {
		print_usage();
		return -1; 
	}

	if ((argc == 4) && (!(wcscmp(argv[3], L"-return_com_number")))) {

		com_num = ftdi_get_com_num(&ftHandle, dev_num); 

		return com_num;
	}



	if (ftdi_init(&ftHandle, dev_num) < 0) {
		printf("Error init FTDI\n"); 
		return -2;	
	}



	/////////////////////////////////////////////////////////////////////////////

	if ((argc == 4) && (!(wcscmp(argv[3], L"-program_ftdi_eeprom")))) {

		printf("\nGoing to program FTDI Eeprom to set CBUS2(3) to I/O mode\n");

		ftdi_program_eeprom(ftHandle); 

		// after ftdi_program_eeprom() device is closed 
		return 0; 

	} else if ((argc == 5) && (!(wcscmp(argv[3], L"-pins")))) {

//		printf("\nSet CBUS pins..\n");

		swscanf(argv[4],L"%d",&pins_val);

		if (ftdi_set_cbus_pins(ftHandle,pins_val) < 0) 
			return 1;	// error
		else 
			return 0; 

	}   else {

		
		print_usage();


	}


	FT_W32_CloseHandle(ftHandle); 
	return 0;

}



void print_usage(void) 
{

		printf("Usage:\t\t\t Description:\n");
		printf("\t ftdi_tool -device X -program_ftdi_eeprom \n");
		printf("\t ftdi_tool -device X -pins [val] \n");
		printf("\t ftdi_tool.exe -device X -return_com_number\n");	
}



