setMode -bs 
setCable -port xsvf -file cpld_relay_bbb_id87.xsvf 
addDevice -p 1 -file cpld_relay_bbb_id87.jed 
Erase -p 1   
Program -p 1 -e  
closeCable 
quit 
