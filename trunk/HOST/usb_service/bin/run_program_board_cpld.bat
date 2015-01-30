@echo off 

set ihome_test=.\ihome_usb_utility.exe

set cpld_file=..\data\cpld_relay_key_moves.xsvf

%ihome_test% -device 0 -programm_cpld %cpld_file% 1

pause