@echo off 

set ihome_test=.\ihome_usb_utility.exe

set cpld_file=..\data\sysl_cpld_for_prog_BOARDs.xsvf

%ihome_test% -device 0 -programm_cpld %cpld_file% 0 

pause