@echo off

rem modify these two vars
set cpld_file_name=cpld_relay_bbb_id87

set xsvf_file=%cpld_file_name%.xsvf
set jed_file=%cpld_file_name%.jed
set path_to_ise_settings=C:\Xilinx\14.6\ISE_DS\settings64.bat

set impact_file=new_impact.cmd


echo. 
echo **************************************************
echo Create %impact_file% file
echo **************************************************
echo. 
echo setMode -bs > %impact_file%
echo setCable -port xsvf -file %xsvf_file% >> %impact_file%
echo addDevice -p 1 -file %jed_file% >> %impact_file%
rem no verify
echo Erase -p 1  >> %impact_file% 
echo Program -p 1 -e  >> %impact_file%
echo closeCable >> %impact_file%
echo quit >> %impact_file%
echo. 
echo. 
echo **************************************************
echo Start converting jed file to xsvf for RC47 board
echo **************************************************
echo. 
echo. 
echo. 
call %path_to_ise_settings%
impact -batch %impact_file%
echo. 
echo.
 