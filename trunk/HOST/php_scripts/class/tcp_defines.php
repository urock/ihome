<?php

define("CID_RELAY",0x01);
define("CID_CONFIG",0x02);
define("CID_ERROR",0xFF);


# Command Parameters for CID_RELAY
define("GET_RSTATE_REQ",0x01);
define("GET_RSTATE_ANS",0x02);
define("SET_RSTATE_REQ",0x03);
define("SET_RSTATE_ACK",0x04);

define("REP_RSTATE_REQ",0x05);
define("REP_RSTATE_ACK",0x06);


# Command Parameters for CID_CONFIG
define("GET_CONFIG_T_REQ",0x01);		# host is reading mcu_config_t
define("GET_CONFIG_T_ANS",0x02);
define("SET_CONFIG_T_REQ",0x03);
define("SET_CONFIG_T_ACK",0x04);


# Common Command Parameters
define("MSG_ERROR",0xFF);


# Error codes
define("ERROR_CID_NACK",0x01);  		# cmd id not recognized
define("ERROR_CPAR_NACK",0x02);			# cmd parameter not recognized
define("ERROR_DATA",0x03);



function parse_tcp_answer($input_bin_str, &$output_bin_str) 
{
				
	switch (ord($input_bin_str[0])) {
	
		case CID_RELAY:
			echo "Received CID_RELAY\n";
			
			return "CID_RELAY";
			
			break;
			
		case CID_CONFIG:
			
			
			if (ord($input_bin_str[1]) == GET_CONFIG_T_ANS) {
							
				# input_bin_str[2,3] - length of message in bytes
				$output_bin_str = substr($input_bin_str,4);
				return "GET_CONFIG_T_ANS"; 

			 } elseif (ord($input_bin_str[1]) == SET_CONFIG_T_ACK)  {
								
				return "SET_CONFIG_T_ACK"; 
				
			 } elseif (ord($input_bin_str[1]) == MSG_ERROR) {
				return "MSG_ERROR"; 
			 } else {
			 
				break;
			}
			
		case CID_ERROR:
			return "Commad Not Acknowledged"; 
	}
}




?>
