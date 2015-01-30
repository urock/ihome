<?php

error_reporting(E_ERROR); 


$broadcast_string = "iHome?";

//destination port
$port1 = 7;
//host port
$port2 = 4; 


// init Sender Socket
$sock1 = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP); 
if ($sock1 === false) {
    echo "Error in socket_create 1(): error: " . socket_strerror(socket_last_error()) . "\n";
	exit; 
}
if (!socket_set_option($sock1, SOL_SOCKET, SO_BROADCAST, 1)) {
	socket_close($sock1);
    echo "Error in socket_set_option 1(): error: " . socket_strerror(socket_last_error()) . "\n";
	exit; 	
}


// init Receiver Socket
$sock2 = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP); 
if ($sock2 === false) {
    socket_close($sock1);
	echo "Error in socket_create 2(): error: " . socket_strerror(socket_last_error()) . "\n";
	exit; 
}

// bind receiver socket on host receiver port
if(!socket_bind($sock2, "0.0.0.0", $port2)) {
    socket_close($sock1);
	socket_close($sock2);
    echo "socket_bind failed: ".socket_strerror(socket_last_error())."\n";
	exit;
} 

if (!socket_set_option($sock2,SOL_SOCKET,SO_RCVTIMEO,array("sec"=>1,"usec"=>0))) {
    socket_close($sock1);
	socket_close($sock2);
    echo "socket_set_option 2 failed: ".socket_strerror(socket_last_error())."\n";
	exit;
}

 
socket_sendto($sock1, $broadcast_string, strlen($broadcast_string), 0, '255.255.255.255', $port1); 


$from_ip = '';
$from_port = ''; 

while(socket_recvfrom($sock2, $buf, 100, 0, $from_ip, $from_port)) { 
	echo "Got answer from $from_ip" . PHP_EOL;


	// SN
	printf("%x:",ord($buf[0]));
	printf("%x:",ord($buf[1]));
	printf("%x:",ord($buf[2]));
	printf("%x",ord($buf[3]));

	
	echo PHP_EOL;

}


exit;



?>