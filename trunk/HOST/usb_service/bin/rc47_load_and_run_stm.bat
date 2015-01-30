@echo off 
rem  BOOT0 = CBUS3 
rem  RST_N = not CBUS2 
rem  pins_val	CBUS3	CBUS2		RST_N	BOOT0
rem 	0			0		0		1		0
rem 	1			0		1		0		0
rem 	2			1		0		1		1
rem 	3			1		1		0		1


rem Define data files
set rc47_stm_firmware=..\data\mcu_main_03.hex
set ftdi_tool=.\ftdi_tool.exe

set stm_flash_loader=..\stm_flash_loader\STMFlashLoader.exe


call %ftdi_tool% -device 0 -return_com_number
if %ERRORLEVEL% EQU -1 (
  echo.
  echo FTDI Error.
  echo Make sure the board is connected or wait for Windows to recognise USB device
  echo.
  pause
  exit 1
)
set com_num=%errorlevel%


rem BOOT0 = 1; RST_N = 0; 
call %ftdi_tool% -device 0 -pins 3
if errorlevel 1 (
  goto rc47_ftdi_error
)

rem BOOT0 = 1; RST_N = 1; 
call %ftdi_tool% -device 0 -pins 2
if errorlevel 1 (
  goto rc47_ftdi_error
)



echo.
echo.
echo.
echo ***********************************************************
echo Start STMFlashLoarder 
echo **********************************************************
echo.
echo.
rem STMFlashLoader.exe -c --pn %com_num% --br 115200 -i STM32_High-density_512K -p --dwp -e --all -d --a 08000000 --fn rc47_stm_1.hex --v
%stm_flash_loader% -c --pn %com_num% --br 115200 -i STM32_High-density_512K -e --sec 4 0 1 2 3 -d --a 08000000 --fn %rc47_stm_firmware%
pause
echo **********************************************************
echo END STMFlashLoarder 
echo **********************************************************
echo.  
echo. 



rem BOOT0 = 0; RST_N = 0; 
call %ftdi_tool% -device 0 -pins 1
if errorlevel 1 (
  goto rc47_ftdi_error
)

 
rem BOOT0 = 0; RST_N = 1; 
call %ftdi_tool% -device 0 -pins 0 
if errorlevel 1 (
  goto rc47_ftdi_error
)

pause

exit 1



:rc47_ftdi_error
echo ********** rc47_ftdi set pins error **********

pause