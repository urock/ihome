@echo off 

rem Define bin files
set ftdi_tool=ftdi_tool.exe

rem Program FTDI EEPROM
call %ftdi_tool% -device 0 -program_ftdi_eeprom

pause