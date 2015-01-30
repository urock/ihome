<?php


function crc8($bin_str)
{
	$crc = 0x00;
	for ($i1=0;$i1<strlen($bin_str);$i1++) {
        $crc = ($crc) ^ (ord($bin_str[$i1]));
        for($i=0; $i<8; $i++)		
			if (($crc & 0x01) == 0x01) 
				$crc = ((($crc) >> 1) ^ (0x8C));
			else 
				$crc = (($crc) >> 1);
	}
	return $crc;
}

?>