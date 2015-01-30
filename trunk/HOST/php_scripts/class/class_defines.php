<?php
class MCU
{
    // properties
    public $SN = '0.0.0.0';
	public $stm_mac = '0.0.0.0.0.0';
	public $stm_ip = '0.0.0.0';
	public $stm_mask = '0.0.0.0';
	public $stm_gw = '0.0.0.0';

	public $stm_port = 10001;
	public $host_ip = '0.0.0.0';
	public $host_port = 0; 	
	
	function __construct($SN, $IP){
		$this->SN = $SN;
		$this->stm_ip = $IP;
	}


	
    // methods
    public function show_properties() {
		
		echo "\nPrint MCU properties\n"; 
	
        echo "Serial Number = $this->SN " . PHP_EOL;
        echo "STM MAC = $this->stm_mac " . PHP_EOL;
        echo "STM IP = $this->stm_ip " . PHP_EOL;
        echo "STM Mask = $this->stm_mask " . PHP_EOL;
        echo "STM Gateway= $this->stm_gw " . PHP_EOL;
        echo "STM Port = $this->stm_port " . PHP_EOL;
        echo "Host IP = $this->host_ip " . PHP_EOL;		
		echo "Host Port = $this->host_port " . PHP_EOL;
		}
		
	
	// read properties from device
	public function get_properties() {

		echo "\nEntered Read Config function\n";
		
		// create bin array
		$CID = CID_CONFIG;				// command ID;				1 byte
		$PAR = GET_CONFIG_T_REQ;		// command parametr			1 byte
		$LEN = 0;						// data length in bytes		2 bytes
		$binarydata = pack("CCn",$CID,$PAR,$LEN);

		$tcp_answer = $this->tcp_send_command($binarydata); 
		$ret_code = parse_tcp_answer($tcp_answer, $out_str);
		echo $ret_code . PHP_EOL;
		
		//echo bin2hex($out_str); 
		$this->string_to_mcu_properties($out_str); 	
	}
	
	// write properties to device
	public function set_properties() {
	

		echo "\nEntered Write Config function\n"; 
	

		$bin_str = $this->mcu_properties_to_string(); 

		echo "Created bin string: \n" . bin2hex($bin_str)."\n"; 
		echo "String len = " . strlen($bin_str) . "\n";

		$this->string_to_mcu_properties($bin_str); 	
		

		$CID = CID_CONFIG;				// command ID;				1 byte
		$PAR = SET_CONFIG_T_REQ;		// command parametr			1 byte
		$LEN = strlen($bin_str);						// data length in bytes		2 bytes


		# create bin array
		$binarydata = pack("CCn",$CID,$PAR,$LEN);


		$tcp_answer = $this->tcp_send_command($binarydata.$bin_str); 
		echo "Got answer from ihome: \n" . bin2hex($tcp_answer)."\n"; 

		$ret_code = parse_tcp_answer($tcp_answer, $config_t);
		# $ret_code = parse_tcp_answer($tcp_answer);

		echo $ret_code . PHP_EOL;

	
	}
	
	

	private function mcu_properties_to_string() {

		$arr = explode(".", $this->SN); 
		$SN = pack("CCCC",$arr[0],$arr[1],$arr[2],$arr[3]);

		$arr = explode(".", $this->stm_mac); 
		$STM_MAC = pack("CCCCCC",$arr[0],$arr[1],$arr[2],$arr[3],$arr[4],$arr[5]); 

		$arr = explode(".", $this->stm_ip); 
		$STM_IP = pack("CCCC",$arr[0],$arr[1],$arr[2],$arr[3]);

		$arr = explode(".", $this->stm_mask); 
		$STM_MK = pack("CCCC",$arr[0],$arr[1],$arr[2],$arr[3]);

		$arr = explode(".", $this->stm_gw); 
		$STM_GW = pack("CCCC",$arr[0],$arr[1],$arr[2],$arr[3]);

		$STM_PORT = pack("S", $this->stm_port);

		$arr = explode(".", $this->host_ip); 
		$HST_IP = pack("CCCC",$arr[0],$arr[1],$arr[2],$arr[3]);

		$HST_PORT = pack("S", $this->host_port);

		$bin_str = $SN . $STM_MAC . $STM_IP . $STM_MK . $STM_GW . $STM_PORT . $HST_IP . $HST_PORT; 

		return $bin_str;

	}
		
	private function string_to_mcu_properties($c) {
	
		$this->SN = ord($c[0]).'.'.ord($c[1]).'.'.ord($c[2]).'.'.ord($c[3]);
		$this->stm_mac = ord($c[4]).'.'.ord($c[5]).'.'.ord($c[6]).'.'.ord($c[7]).'.'.ord($c[8]).'.'.ord($c[9]);

		$this->stm_ip = ord($c[10]).'.'.ord($c[11]).'.'.ord($c[12]).'.'.ord($c[13]);
		$this->stm_mask = ord($c[14]).'.'.ord($c[15]).'.'.ord($c[16]).'.'.ord($c[17]);
		$this->stm_gw = ord($c[18]).'.'.ord($c[19]).'.'.ord($c[20]).'.'.ord($c[21]);
		
		// unpack returns array, we need first element.
		$arr = unpack('S', $c[22].$c[23]); // v key (little endian) is also ok
		$this->stm_port = $arr[1]; 
			
		$this->host_ip = ord($c[24]).'.'.ord($c[25]).'.'.ord($c[26]).'.'.ord($c[27]);	
		
		// unpack returns array, we need first element.
		$arr = unpack('S', $c[28].$c[29]); // v key (little endian) is also ok
		$this->host_port = $arr[1]; 
	}




	private function tcp_send_command($bin_str) {

		$stm_ip = $this->stm_ip;  
		$stm_port = $this->stm_port; 

		error_reporting(E_ALL);

		do {
		$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);

		if ($socket === false) {
			echo "Error in socket_create(): error: " . socket_strerror(socket_last_error()) . "\n";
			break; 
		} 


		echo "Connecting to '$stm_ip' on port '$stm_port'...";

		$result = socket_connect($socket, $stm_ip, $stm_port);
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


		echo "Closing socket...";
		socket_close($socket);
		echo "OK.\n\n";
		} while(0); 

		return $out;

	}
	
}


function find_devices() 
{
	
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
	
	$return_str = ''; 

	while(socket_recvfrom($sock2, $buf, 100, 0, $from_ip, $from_port)) { 
		//echo "Got answer from $from_ip" . PHP_EOL;
		
		$return_str = $return_str.ord($buf[0]).'.'.ord($buf[1]).'.'.ord($buf[2]).'.'.ord($buf[3])."-$from_ip".PHP_EOL;
			



	}

	// $return_str = $return_str.'0.0.0.2-192.168.0.24'.PHP_EOL;
	// $return_str = $return_str.'0.0.0.3-192.168.0.25'.PHP_EOL;
	
	return $return_str;

}


?>