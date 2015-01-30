#include "stdafx.h"
#include "stm_host.h"




int rosta_prog_cpld_over_ftdi_stm(FT_HANDLE ftHandle, LPCWSTR cpld_file_name_prt, unsigned char cpld_selector)
{


	int				k=0, bResult, l, fileLength;
	DWORD			count;
	HANDLE			hFile;

	unsigned char	*wr_buf, *rd_buf; 
	void			*heap_p; 

	unsigned char cpld_sel = cpld_selector; 

	printf("\n\n===============================================\n\n");

	printf("Programming CPLD with %ws file\n",cpld_file_name_prt);

	

	hFile = CreateFile(cpld_file_name_prt,
				GENERIC_READ,
                0,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);

	if (hFile == INVALID_HANDLE_VALUE)  {
		printf("Cannot open file %ws\n",cpld_file_name_prt);
		return -1;
	}

	fileLength = SetFilePointer(hFile, 0, NULL, FILE_END);
	//printf("file length %d\n", fileLength);
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);


	

/////////////////////////////////////////////////////////////////////////

	//printf("Creating buffer\n\n");

	heap_p = HeapCreate(HEAP_NO_SERIALIZE,fileLength*2,0); 
	if (!heap_p) {
		printf("Error creating Heap! Exiting... \n");
		return -1;
	}

	wr_buf = (unsigned char *)HeapAlloc(heap_p,HEAP_ZERO_MEMORY,fileLength); // file buffer
	if (!wr_buf) {
		printf("Error allocationg memory from Heap! Exiting...\n");
		return -1;
	}


	rd_buf = (unsigned char *)HeapAlloc(heap_p,HEAP_ZERO_MEMORY,fileLength); // file buffer
	if (!rd_buf) {
		printf("Error allocationg memory for rd_buf! Exiting...\n");
		return -1;
	}

	//printf("Reading file.. \n\n"); 

	bResult = ReadFile(hFile,wr_buf,fileLength,&count,NULL); 
	CloseHandle(hFile);

	if (!bResult) {
		printf("Error reading from file\n\n"); 
		return -1;
	}

	if (count != fileLength) {
		printf("Error Read less than file length elements from file\n"); 
		return -1;
	}



	// send buffer to stm

	unsigned short c = 0;
	unsigned short c_len = 1024;

	k = 0; 

	printf("\nSending data to STM: ");

	while  ((c + c_len) < (unsigned short)fileLength) {

		if (stm_write_cpld_file(&wr_buf[c], c, c_len) < 0) {
			printf("Error writting xsvf file to STM\n");
			return -2;
		}

		c += c_len; 

		printf("%d ", k++);
	}


	c_len = (unsigned short)fileLength - c; 


	if (stm_write_cpld_file(&wr_buf[c], c, c_len) < 0) {
		printf("Error writting xsvf file to STM\n");
		return -2;
	}	

	printf("%d DONE\n", k);

	///////////////////////////////////////////////////////////////////////////////////
	printf("\nReading data back: ");

	k = 0; 
	c = 0;
	c_len = 1024; 
	while  ((c + c_len) < (unsigned short)fileLength) {

		if (stm_read_cpld_file(&rd_buf[c], c, c_len) < 0) {
			printf("Error writting xsvf file to STM\n");
			return -2;
		}

		c += c_len; 

		printf("%d ", k++);
	}


	c_len = (unsigned short)fileLength - c; 

	if (stm_read_cpld_file(&rd_buf[c], c, c_len) < 0) {
		printf("Error writting xsvf file to STM\n");
		return -2;
	}

	printf("%d DONE\n", k);

	for (l=0; l<fileLength; l++) 
		if (wr_buf[l] != rd_buf[l]) {
			printf("Compare buffers failed wr_buf[%d] = %d, rd_buf[%d] = %d\n",l,wr_buf[l],l,rd_buf[l]);
			return -3;
		}

	printf("\nCompare buffers OK!\n"); 

	printf("Send command to program CPLD...   "); 

	if (stm_prog_cpld(cpld_sel) < 0)  {
		printf("ERROR!\n");
		return CPLD_ERROR;
	} else
		printf("OK!\n");
		 

	return 0;
}