@echo off 

set ihome_test=.\ihome_usb_utility.exe

:: set cpld_file=..\data\cpld_relay_1_2.xsvf
set cpld_file=..\data\cpld_relay_2.xsvf

%ihome_test% -device 0 -programm_cpld %cpld_file% 1
%ihome_test% -device 0 -programm_cpld %cpld_file% 2
%ihome_test% -device 0 -programm_cpld %cpld_file% 3
%ihome_test% -device 0 -programm_cpld %cpld_file% 4
%ihome_test% -device 0 -programm_cpld %cpld_file% 5
%ihome_test% -device 0 -programm_cpld %cpld_file% 6
%ihome_test% -device 0 -programm_cpld %cpld_file% 7
%ihome_test% -device 0 -programm_cpld %cpld_file% 8

pause