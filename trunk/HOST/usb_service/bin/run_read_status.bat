@echo off 

set ihome_test=.\ihome_usb_utility.exe
%ihome_test% -device 0 -read_status

pause