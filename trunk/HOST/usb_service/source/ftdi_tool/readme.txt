fdti_tool

	Программа для работы с микросхемой FTDI
	
	Запуск
		Usage:										
		printf("\t ftdi_tool.exe -device X -program_ftdi_eeprom \n");
		printf("\t ftdi_tool.exe -device X -pins [val] \n");
		printf("\t ftdi_tool.exe -device X -return_com_number\n");		
		
		
	
	
	
	
Changelog


	V 1.1 :::::
	
		Добавлена функция возвращения номера ком порта устройства

	V 1.0 :::::
	
		После записи значений в EEPROM подается команда CyclePort, которая ресетит микросхему FTDI, после чего можно работать без выключения питания
	

			FTDI CyclePort after FTDI EEPROM programming
		
		