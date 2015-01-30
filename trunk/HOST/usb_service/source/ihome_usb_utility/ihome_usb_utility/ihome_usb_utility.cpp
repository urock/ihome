// ihome_usb_utility.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ftdi_api.h"
#include "cpld.h"

#define	APP_NAME		"IHOME USB UTILITY"
#define APP_VERSION		"0.1"


// link ftd2xx library - DLL library for i386 platform
#pragma comment(lib, "ftd2xx.lib")

FT_HANDLE		ftHandle;


void print_usage(void) 
{

		printf("Usage:\t\t\t Description:\n");
		printf("\t app_name -device X -programm_cpld [xsvf file name] [cpld selector: 0 - sys cpld, 1-8 - board cpld]\n");		
		printf("\t app_name -device X -read_status \n");	
}


int _tmain(int argc, _TCHAR* argv[])
{
	int dev_num; 

	unsigned int cpld_selector;

	printf("\n*********************************************************\n");
	printf("\n%s verision %s started!\n\n",APP_NAME, APP_VERSION); 

	/////////////////////////////////////////////////////////////////////////
	// open device

	if (!(wcscmp(argv[1],L"-device")))	{						// argv[1] == "-device"
		swscanf(argv[2],L"%d",&dev_num);		
//		printf("device = %d\n",dev_num); 
	} else {
		print_usage();
		return -1; 
	}


	if (ftdi_init(&ftHandle, dev_num) < 0) {
		printf("Error init FTDI\n"); 
		return -1; 
	}



	if ((argc == 6) && (!(wcscmp(argv[3], L"-programm_cpld")))) {

		

		printf("\nSet MC mode to boot from flash memory\n");

		swscanf(argv[5],L"%d",&cpld_selector);	

		rosta_prog_cpld_over_ftdi_stm(ftHandle, argv[4], (unsigned char)cpld_selector); 	


	}  else if ((argc == 4) && (!(wcscmp(argv[3], L"-read_status")))) {

		printf("\n\nReading RC47 Status\n");


		//if (rc47_read_status(&stm_status, &max_status) < 0)
		//	printf("Error in rc47_read_status\n"); 



	} else if ((argc == 4) && (!(wcscmp(argv[3], L"-read_monitor")))) {

		//printf("\n\nReading RC47 temperatures and voltages\n");


		//if (stm_read_max_data(&max_status)) {
		//	printf("Error in stm_read_max_data\n");
		//}



	}  else {

		
		print_usage();


	}


	FT_W32_CloseHandle(ftHandle); 
	return 0;
}

