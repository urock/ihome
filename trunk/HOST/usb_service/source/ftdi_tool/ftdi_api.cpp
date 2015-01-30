#include "stdafx.h"
#include "ftdi_api.h"


#define FTDI_MAX_DEVICES		8



int ftdi_get_com_num(FT_HANDLE	*ftHandle, int dev_num)
{

	FT_STATUS	ftStatus; 


	int			numDevs;
	wchar_t		*BufPtrs[FTDI_MAX_DEVICES];
	wchar_t		Buffer1[64];
	wchar_t		Buffer2[64];
	BufPtrs[0] = Buffer1;
	BufPtrs[1] = Buffer2;
	BufPtrs[2] = NULL;

	LONG lComPortNumber;

	//printf("ROSTA FTDI TOOL ver %s\n",APP_VERSION); 

//	printf("\nOpening device # %d\n",dev_num);

	ftStatus =  FT_ListDevices(BufPtrs,&numDevs,FT_LIST_ALL|FT_OPEN_BY_DESCRIPTION); 
	if (ftStatus == FT_OK) {
//		printf("\nNumber of devices in system = %d\n",numDevs);
//		wprintf(L"%ws\n",Buffer1); 
	} else {
		printf("Error in FT_ListDevices\n");
		return -1;
	}

	if ((dev_num + 1) > numDevs) {
		printf("Error dev_num > numDevs\n");
		return -2;
	}

	

ftStatus = FT_Open(dev_num,ftHandle);
if (ftStatus != FT_OK) {
	printf("FT_Open failed\n"); 
	return -3;
}



ftStatus = FT_GetComPortNumber(*ftHandle,&lComPortNumber);
if (ftStatus == FT_OK) {
	if (lComPortNumber == -1) {
		printf("No COM port assigned\n");
		return -4;
	}
	else {
		printf("\nlComPortNumber = %d\n\n",lComPortNumber);
	}
} else {
	printf("FT_GetComPortNumber FAILED!\n"); 
	return -5; 
}

FT_Close(ftHandle);

	return (int)lComPortNumber;
}





int ftdi_init(FT_HANDLE	*ftHandle, int dev_num)
{

	FT_STATUS	ftStatus; 
	FTDCB		ftDCB; 
//	wchar_t		Buf[64]; 
	FTTIMEOUTS	ftTS; 


//	printf("\nStart init FTDI device\n"); 

/*
	ftStatus = FT_ListDevices(0,Buf,FT_LIST_BY_INDEX|FT_OPEN_BY_DESCRIPTION); 
 
	*ftHandle = FT_W32_CreateFile(Buf,GENERIC_READ|GENERIC_WRITE,0,0, 
			  OPEN_EXISTING, 
			  FILE_ATTRIBUTE_NORMAL | FT_OPEN_BY_DESCRIPTION, 
			  0); 
*/

	int			numDevs;
	wchar_t		*BufPtrs[FTDI_MAX_DEVICES];
	wchar_t		Buffer1[64];
	wchar_t		Buffer2[64];
	BufPtrs[0] = Buffer1;
	BufPtrs[1] = Buffer2;
	BufPtrs[2] = NULL;

	

//	printf("\nOpening device # %d\n",dev_num);

	ftStatus =  FT_ListDevices(BufPtrs,&numDevs,FT_LIST_ALL|FT_OPEN_BY_DESCRIPTION); 
	if (ftStatus == FT_OK) {
//		printf("\nNumber of devices in system = %d\n",numDevs);
//		wprintf(L"%ws\n",Buffer1); 
	} else {
		printf("Error in FT_ListDevices\n");
		return -1;
	}

	if ((dev_num + 1) > numDevs) {
		printf("Error dev_num > numDevs\n");
		return -2;
	}

	

	*ftHandle = FT_W32_CreateFile(BufPtrs[dev_num],GENERIC_READ|GENERIC_WRITE,0,0, 
			  OPEN_EXISTING, 
			  FILE_ATTRIBUTE_NORMAL | FT_OPEN_BY_DESCRIPTION, 
			  0); 	


	if (ftHandle == INVALID_HANDLE_VALUE) {
		printf("Error opening FTDI device\n");
		return -1; 
	}



	if (FT_W32_GetCommState(*ftHandle,&ftDCB)) {
	
		ftDCB.BaudRate		= 3000000; // 115200; //1500000;	//3000000; //921600;	
		ftDCB.fBinary		= 1;
		ftDCB.fParity		= 0;
		ftDCB.fOutxCtsFlow	= 0;
		ftDCB.fOutxDsrFlow	= 0;
		ftDCB.fDtrControl	= 1;
		ftDCB.fRtsControl	= 1; 	
		ftDCB.ByteSize		= 8;
		ftDCB.Parity		= 0; 
		ftDCB.StopBits		= 0; 


		if (FT_W32_SetCommState(*ftHandle,&ftDCB)) {
			//printf("\nFT_W32_SetCommState OK! \n"); 
//			if (FT_W32_GetCommState(*ftHandle,&ftDCB)) {
//				//printf("\nGetCommState 2 OK!\n");
//				printf("\tBaudRate\t= %d\n", ftDCB.BaudRate);
//
//				printf("\tByteSize\t= %d bits\n", ftDCB.ByteSize);
//				switch (ftDCB.Parity) {
//				case 0 : printf("\tParity  \tNone\n");	break;
//				case 1 : printf("\tParity  \tOdd\n");	break;
//				case 2 : printf("\tParity  \tEven\n");	break;
//				case 3 : printf("\tParity  \tMark\n");	break;
//				case 4 : printf("\tParity  \tSpace\n"); break;
//				}
////				printf("\tParity  \t= %d\n", ftDCB.Parity); /* 0-4=None,Odd,Even,Mark,Space			*/
//				switch(ftDCB.StopBits) {
//				case 0:
//					printf("\tStopBits\t= 1\n");
//					break;
//				case 1:
//					printf("\tStopBits\t= 1.5\n");
//					break;
//				case 2:
//					printf("\tStopBits\t= 2\n");
//					break;
//
//				}
//
////				printf("\tStopBits\t= %d\n", ftDCB.StopBits); /* 0,1,2 = 1, 1.5, 2					*/
//			} else {
//				printf("FT_W32_GetCommState 2 FAILED\n"); 
//				return -2;
//			}

		} else {
			printf("FT_W32_SetCommState FAILED\n"); 
			return -3;
		}


	} else {
		printf("FT_W32_GetCommState FAILED\n"); 
		return -4; 
		
	}

	

	if (FT_W32_GetCommTimeouts(*ftHandle,&ftTS)) {



	}
	else {
		printf("Error setting Comm Timeouts\n");
		return -5;
	} 


	ftTS.ReadIntervalTimeout = 20; 
	ftTS.ReadTotalTimeoutMultiplier = 500; 
	ftTS.ReadTotalTimeoutConstant = 1000; 
	ftTS.WriteTotalTimeoutMultiplier = 1; 
	ftTS.WriteTotalTimeoutConstant = 10; 
 
	if (!FT_W32_SetCommTimeouts(*ftHandle,&ftTS)) {
		printf("Error setting Comm Timeouts\n");
		return -6;
	}

	/*
	if (FT_W32_GetCommTimeouts(*ftHandle,&ftTS)) {

	}
	else {
		printf("Error setting Comm Timeouts 2\n");
		return -7;
	} 
	*/



//	printf("Success!!\n"); 

	return 1; 
}






	// On Rosta USB2COM TTL board 
	// DTR signal is connected to BOOT0 pin of STM32
	//	BOOT0 = 0 - boot from flash memory
	//	BOOT0 = 1 - boot from system memory - init boot loader 

	// RTS signal == RST of STM32. rst is active low

	// Set(Dtr,Rts) - pull to Low

FT_STATUS ftdi_set_dtr_high_and_rst(FT_HANDLE ftHandle)		// system memory
{
	FT_STATUS	ftStatus;



	ftStatus = FT_ClrDtr(ftHandle);		// DTR = 1
	if (!FT_SUCCESS(ftStatus)) {
		printf("Error in Set DTR Low\n"); 
		return ftStatus; 
	}

	Sleep(100); 

	ftStatus = FT_SetRts(ftHandle);		// RTS = 0
	if (!FT_SUCCESS(ftStatus)) {
		printf("Error in Set DTR Low\n"); 
		return ftStatus; 
	}
	Sleep(100); 

	ftStatus = FT_ClrRts(ftHandle);		// RTS = 1
	if (!FT_SUCCESS(ftStatus)) {
		printf("Error in Set DTR Low\n"); 
		return ftStatus; 
	}

	return FT_OK;
}



FT_STATUS ftdi_set_dtr_low_and_rst(FT_HANDLE ftHandle)			// flash memory
{
	FT_STATUS	ftStatus;

	// ClrDtr - dtr = 1

	ftStatus = FT_SetDtr(ftHandle);		// DTR = 0
	if (!FT_SUCCESS(ftStatus)) {
		printf("Error in Set DTR Low\n"); 
		return ftStatus; 
	}

	// init rts at 0

	Sleep(100); 

	ftStatus = FT_SetRts(ftHandle);		// RTS = 0
	if (!FT_SUCCESS(ftStatus)) {
		printf("Error in Set DTR Low\n"); 
		return ftStatus; 
	}


	Sleep(2500); 

	//ftStatus = FT_ClrRts(ftHandle);		// RTS = 1
	//if (!FT_SUCCESS(ftStatus)) {
	//	printf("Error in Set DTR Low\n"); 
	//	return ftStatus; 
	//}

	return FT_OK;
}







int ftdi_send_byte(FT_HANDLE	ftHandle, unsigned char data8)
{
	DWORD BytesWritten;

	if (FT_W32_WriteFile(ftHandle, &data8, 1, &BytesWritten, NULL))  
		if (BytesWritten == 1)
			return 1;
		else 
			return -1;
	else	
		return -2;
	
}

int ftdi_receive_byte(FT_HANDLE	ftHandle, unsigned char *data8)
{
	DWORD dwRead;
	
	if (FT_W32_ReadFile(ftHandle, data8, 1, &dwRead, NULL)) 
		if (dwRead == 1) 
			return 1;
		else
			return -1;
	else
		return -2;
}


int ftdi_send_buffer(FT_HANDLE	ftHandle, unsigned char *buffer, int length)
{
	DWORD BytesWritten;

	if (FT_W32_WriteFile(ftHandle, buffer, length, &BytesWritten, NULL))  
		if (BytesWritten == length)
			return 1;
		else 
			return -1;
	else	
		return -2;
	
}



int ftdi_receive_buffer(FT_HANDLE	ftHandle, unsigned char *buffer, int length)
{
	DWORD dwRead;
	
	if (FT_W32_ReadFile(ftHandle, buffer, length, &dwRead, NULL)) 
		if (dwRead == length) 
			return 1;
		else
			return -1;
	else
		return -2;
}


// ON RC47 iss 1
// BOOT0 = CBUS3 
// RST_N = not CBUS2 
// pins_val		CBUS3	CBUS2		RST_N	BOOT0
//  	0			0		0		1		0
//  	1			0		1		0		0
//  	2			1		0		1		1
//  	3			1		1		0		1


int ftdi_set_cbus_pins(FT_HANDLE ftHandle, int pins_val)
{
	FT_STATUS	ftStatus; 

	unsigned char	val8; 

	switch(pins_val) {

	case 0: val8 = 0xC0; printf("Setting BOOT0 = 0; RST_N = 1"); break;
	case 1: val8 = 0xC4; printf("Setting BOOT0 = 0; RST_N = 0"); break;
	case 2: val8 = 0xC8; printf("Setting BOOT0 = 1; RST_N = 1"); break;
	case 3: val8 = 0xCC; printf("Setting BOOT0 = 1; RST_N = 0"); break;

	default:
		printf("Error! Not supported value for pins... Exitting\n");
		return -1;
	}

	ftStatus =  FT_SetBitMode(ftHandle, val8, 0x20);

	if (ftStatus == FT_OK) 
		printf("\tOK!!\n");
	else {
		printf("\nError in ftdi_set_cbus_pins.. Exitting\n"); 
		return -2;
	}

	return 0; 
}




// programs CBUS 2,3 for IO mode
int ftdi_program_eeprom(FT_HANDLE ftHandle)
{

	FT_STATUS	ftStatus; 



	FT_PROGRAM_DATA ftData; 
	char ManufacturerBuf[32]; 
	char ManufacturerIdBuf[16]; 
	char DescriptionBuf[64]; 
	char SerialNumberBuf[16]; 
 
	ftData.Signature1 = 0x00000000; 
	ftData.Signature2 = 0xffffffff; 
	ftData.Version = 0x00000005;    // EEPROM structure with FT232H extensions 
	ftData.Manufacturer = ManufacturerBuf; 
	ftData.ManufacturerId = ManufacturerIdBuf; 
 

	ftData.Description = DescriptionBuf; 
	ftData.SerialNumber = SerialNumberBuf;

	printf("Going to read eeprom\n");

	ftStatus =  FT_EE_Read(ftHandle, &ftData);

	if (ftStatus == FT_OK) 
		printf("OK!!\n");
	else {
		printf("Error reading Eeprom.. Exitting\n"); 
		return -1;
	}


	
	printf("CBUS0 = 0x%x\n",ftData.Cbus0);
	printf("CBUS1 = 0x%x\n",ftData.Cbus1);
	printf("CBUS2 = 0x%x\n",ftData.Cbus2);
	printf("CBUS3 = 0x%x\n",ftData.Cbus3);
	printf("CBUS4 = 0x%x\n",ftData.Cbus4);

	ftData.Signature1 = 0x00000000; 
	ftData.Signature2 = 0xffffffff; 
	ftData.Version = 0x00000005;    // EEPROM structure with FT232H extensions 

	printf("Let's write Eeprom\n"); 

	ftData.Cbus2 = 0xa;	// got these values after writting to eeprom from gui
	ftData.Cbus3 = 0xa;


	ftStatus =  FT_EE_Program(ftHandle, &ftData);

	if (ftStatus == FT_OK) 
		printf("Programing Eeprom finished successfully\n");
	else {
		printf("Error writting Eeprom.. Exitting\n"); 
		return -2;
	}

	printf("\nNow Reset FTDI\n");

	ftStatus = FT_CyclePort(ftHandle);
	if (ftStatus == FT_OK) {
		printf("FTDI Reset OK!\n");
		FT_Close(ftHandle);
	}
	else {
		printf("FTDI Reset Error!\n");
		return -3; 
	}


	return 0; 
}