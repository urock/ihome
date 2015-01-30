<?php

echo 'PHP Version:' . phpversion() . "\n";


include 'crc.php';
include 'tcp_defines.php';

echo 'PHP Version:' . phpversion() . "\n";

// process cmd line
$c_argc = count($argv);

if (($argv[1] == "-config") && ($argv[2] == "set")) {
	
	write_mcu_config($argv); 
} 

if (($c_argc == 3) &&  ($argv[1] == "-config") && ($argv[2] == "get")) {
	
	read_mcu_config(); 
} 

if (($c_argc == 4) &&  ($argv[1] == "-relay") && ($argv[2] == "get")) {

	$relay_board = $argv[3];
	
	relay_get_state($relay_board); 
} 

if (($c_argc == 5) &&  ($argv[1] == "-relay") && ($argv[2] == "set")) {

	$relay_board = $argv[3];
	$relay_state = $argv[4];
	
	relay_set_state($relay_board, $relay_state); 
} 

exit; 



function write_mcu_config($argv) 
{

echo "Entered Write Config function\n"; 


$conffff= array(
	"SN"		=> array(1,2,3,4),
	"stm_mac"	=> array(2,0,0,0,0,1),
	"stm_ip" 	=> array(192,168,0,23),
	"stm_mask"	=> array(255,255,255,0),
	"stm_gw" 	=> array(192,168,0,254),

	"stm_tcp_server_port" 	=> 10001,
	"host_ip" 	=> array(192,168,0,178),
	"host_tcp_server_port"	=> 111
	); 
	

$bin_str = config_t_to_bin_str($conffff); 

// echo "Created bin string: \n" . bin2hex($bin_str)."\n"; 
// echo "String len = " . strlen($bin_str) . "\n";

// return "1"; 


$CID = CID_CONFIG;				// command ID;				1 byte
$PAR = SET_CONFIG_T_REQ;		// command parametr			1 byte
$LEN = strlen($bin_str);						// data length in bytes		2 bytes


# create bin array
$binarydata = pack("CCn",$CID,$PAR,$LEN);


$tcp_answer = tcp_send_command($binarydata.$bin_str); 
echo "Got answer from ihome: \n" . bin2hex($tcp_answer)."\n"; 

$ret_code = parse_tcp_answer($tcp_answer, $config_t);
# $ret_code = parse_tcp_answer($tcp_answer);

echo $ret_code . PHP_EOL;




}




function read_mcu_config() 
{

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

echo "Entered Read Config function\n"; 

$CID = CID_CONFIG;				// command ID;				1 byte
$PAR = GET_CONFIG_T_REQ;		// command parametr			1 byte
$LEN = 0;						// data length in bytes		2 bytes


// create bin array
$binarydata = pack("CCn",$CID,$PAR,$LEN);


$tcp_answer = tcp_send_command($binarydata); 
echo "Got answer from ihome: \n" . bin2hex($tcp_answer)."\n"; 

$ret_code = parse_tcp_answer($tcp_answer, $config_t);
//$ret_code = parse_tcp_answer($tcp_answer);

echo $ret_code . PHP_EOL;

var_dump($config_t); 



}



function relay_get_state($relay_board) 
{

$CID = CID_RELAY;				// command ID;				1 byte
$PAR = GET_RSTATE_REQ;			// command parametr			1 byte
$LEN = 1;						// data length in bytes		2 bytes

$DAT = $relay_board;			// data payload	


printf("relay_board = 0x%x\n",$relay_board); 



// create bin array
$binarydata = pack("CCnC",$CID,$PAR,$LEN,$DAT);

echo bin2hex($binarydata)."\n"; 


/*
// calc crc
$CRC = crc8($binarydata);
printf("Final CRC = 0x%x\n",$CRC); 

// concatenate with crc
$binarydata = $binarydata . pack("C",$CRC);

echo bin2hex($binarydata)."\n"; 
*/

$tcp_answer = tcp_send_command($binarydata); 
echo "Got answer from ihome: " . bin2hex($tcp_answer)."\n"; 


}



function relay_set_state($relay_board, $relay_state) 
{

$CID = CID_RELAY;				// command ID;				1 byte
$PAR = SET_RSTATE_REQ;			// command parametr			1 byte
$LEN = 2;			// data length in bytes		2 bytes

$DAT1 = $relay_board;			// data payload	
$DAT2 = $relay_state;

printf("relay_board = 0x%x\n",$relay_board); 
printf("relay_state = 0x%x\n",$relay_state); 



// create bin array
$binarydata = pack("CCnCC",$CID,$PAR,$LEN,$DAT1,$DAT2);

echo bin2hex($binarydata)."\n"; 


/*
// calc crc
$CRC = crc8($binarydata);
printf("Final CRC = 0x%x\n",$CRC); 

// concatenate with crc
$binarydata = $binarydata . pack("C",$CRC);

echo bin2hex($binarydata)."\n"; 
*/

$tcp_answer = tcp_send_command($binarydata); 
echo "Got answer from ihome: " . bin2hex($tcp_answer)."\n"; 


}





function tcp_send_command($bin_str) {


$address1 = "192.168.0.23"; 
$service_port = 10001; 

error_reporting(E_ALL);

do {
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

if ($socket === false) {
    echo "Error in socket_create(): error: " . socket_strerror(socket_last_error()) . "\n";
	break; 
} 


echo "Connecting to '$address1' on port '$service_port'...";

$result = socket_connect($socket, $address1, $service_port);
if ($result === false) {
    echo "Error in socket_connect().\nError: ($result) " . socket_strerror(socket_last_error($socket)) . "\n";
	break;
} else {
    echo "OK.\n";
}


echo "Sending CMD...";
socket_write($socket, $bin_str, strlen($bin_str));
echo "OK.\n";

echo "Reading answer...\n";


$out = socket_read($socket, 64);
#echo bin2hex($out)."\n"; 

echo "Closing socket...";
socket_close($socket);
echo "OK.\n\n";
} while(0); 

return $out;

}


?>