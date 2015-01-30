<?php
include 'tcp_client.php'; 
include 'class_defines.php';
include 'func_defines.php';


echo 'PHP Version:' . phpversion() . "\n\n";
echo "iHome PHP application started\n\n"; 
echo "Searching for devices...\n";

$dev_str = find_devices();
// find devices returns one line for each device
$arr1 = explode(PHP_EOL, $dev_str); 

$num_devs = 0;

for($i = 0; $i<(count($arr1) - 1); $i++) {
	// device string format: sn[3].sn[2].sn[1].sn[0]-ip[3].ip[2].ip[1].ip[0]
	$arr2 = explode('-',$arr1[$i]); 
	echo "Found device: SN = $arr2[0]; IP = $arr2[1]\n";
	
	$mcu[$i] = new MCU($arr2[0], $arr2[1]);
	
	$num_devs++;
}

for($i = 0; $i < $num_devs; $i++) 
	$mcu[$i]->show_properties();

exit; 



$mcu[0]->show_properties();


$mcu[0]->get_properties();

$mcu[0]->show_properties();

$mcu[0]->stm_gw = '192.168.0.143'; 

$mcu[0]->set_properties();

$mcu[0]->show_properties();

$mcu[0]->get_properties();

$mcu[0]->show_properties();

//dosmth();

//var_dump($mcu1);


?>