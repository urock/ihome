<?php

include 'tcp_defines.php';






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





?>