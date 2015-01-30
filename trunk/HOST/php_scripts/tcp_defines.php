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



function parse_tcp_answer($input_bin_str, &$config_t) 
// function parse_tcp_answer($input_bin_str) 
{
				
	switch (ord($input_bin_str[0])) {
	
		case CID_RELAY:
			echo "Received CID_RELAY\n";
			
			return "CID_RELAY";
			
			break;
			
		case CID_CONFIG:
			
			echo "Received CID_CONFIG\n";
			
			if (ord($input_bin_str[1]) == GET_CONFIG_T_ANS) {
				
				echo "GET_CONFIG_T_ANS\n";
				
				# input_bin_str[2,3] - length of message in bytes
					
	
				$config_t["SN"][0] = ord($input_bin_str[4]);
				$config_t["SN"][1] = ord($input_bin_str[5]);
				$config_t["SN"][2] = ord($input_bin_str[6]);
				$config_t["SN"][3] = ord($input_bin_str[7]);

				$config_t["stm_mac"][0] = ord($input_bin_str[8]);
				$config_t["stm_mac"][1] = ord($input_bin_str[9]);
				$config_t["stm_mac"][2] = ord($input_bin_str[10]);
				$config_t["stm_mac"][3] = ord($input_bin_str[11]);
				$config_t["stm_mac"][4] = ord($input_bin_str[12]);
				$config_t["stm_mac"][5] = ord($input_bin_str[13]);
				
				$config_t["stm_ip"][0] = ord($input_bin_str[14]);
				$config_t["stm_ip"][1] = ord($input_bin_str[15]);
				$config_t["stm_ip"][2] = ord($input_bin_str[16]);
				$config_t["stm_ip"][3] = ord($input_bin_str[17]);
					
				$config_t["stm_mask"][0] = ord($input_bin_str[18]);
				$config_t["stm_mask"][1] = ord($input_bin_str[19]);
				$config_t["stm_mask"][2] = ord($input_bin_str[20]);
				$config_t["stm_mask"][3] = ord($input_bin_str[21]);

				$config_t["stm_gw"][0] = ord($input_bin_str[22]);
				$config_t["stm_gw"][1] = ord($input_bin_str[23]);
				$config_t["stm_gw"][2] = ord($input_bin_str[24]);
				$config_t["stm_gw"][3] = ord($input_bin_str[25]);
				
				$stm_port = $input_bin_str[26] . $input_bin_str[27]; 
				
				
				$ar1 = unpack('S', $stm_port); // v key (little endian) is also ok
				//var_dump($ar1);
				printf("stm port = %d\n",$ar1[1]); // first element of array has index = 1
				$config_t[stm_tcp_server_port] = $ar1[1];

				
				$config_t["host_ip"][0] = ord($input_bin_str[28]);
				$config_t["host_ip"][1] = ord($input_bin_str[29]);
				$config_t["host_ip"][2] = ord($input_bin_str[30]);
				$config_t["host_ip"][3] = ord($input_bin_str[31]);
				
				$host_port = $input_bin_str[32] . $input_bin_str[33]; 

				$ar2 = unpack('S', $host_port); // v key (little endian) is also ok
				printf("host port = %d\n",$ar2[1]); // first element of array has index = 1
				$config_t[host_tcp_server_port] = $ar2[1];
				
										 
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


function config_t_to_bin_str($c)
{
/*
$config_t= array(
	"SN"		=> array(0,0,0,1),
	"stm_mac"	=> array(0,0,0,0,0,0),
	"stm_ip" 	=> array(0,0,0,2),
	"stm_mask"	=> array(0,0,0,1),
	"stm_gw" 	=> array(0,0,0,2),

	"stm_tcp_server_port" 	=> 0,
	"host_ip" 	=> array(0,0,0,2),
	"host_tcp_server_port"	=> 0
	); 
*/
$SN = pack("CCCC",$c["SN"][0],$c["SN"][1],$c["SN"][2],$c["SN"][3]);
$STM_MAC = pack("CCCCCC",	$c["stm_mac"][0],$c["stm_mac"][1],$c["stm_mac"][2],
							$c["stm_mac"][3],$c["stm_mac"][4],$c["stm_mac"][5]); 

$STM_IP = pack("CCCC",$c["stm_ip"][0],$c["stm_ip"][1],$c["stm_ip"][2],$c["stm_ip"][3]);
$STM_MK = pack("CCCC",$c["stm_mask"][0],$c["stm_mask"][1],$c["stm_mask"][2],$c["stm_mask"][3]);
$STM_GW = pack("CCCC",$c["stm_gw"][0],$c["stm_gw"][1],$c["stm_gw"][2],$c["stm_gw"][3]);

$STM_PORT = pack("S",$c["stm_tcp_server_port"]);

$HST_IP = pack("CCCC",$c["host_ip"][0],$c["host_ip"][1],$c["host_ip"][2],$c["host_ip"][3]);

$HST_PORT = pack("S",$c["host_tcp_server_port"]);

$bin_str = $SN . $STM_MAC . $STM_IP . $STM_MK . $STM_GW . $STM_PORT . $HST_IP . $HST_PORT; 

return $bin_str;

//	return "0000";
}

?>
