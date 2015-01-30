----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    18:45:07 12/27/2013 
-- Design Name: 
-- Module Name:    top - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;



entity control_cpld_01 is
	Port ( 
		
	sel_channel		: in  std_logic_vector (2 downto 0);
	sel_mode			: in  std_logic; 	-- 0 - spi / 1 - jtag
	

	stm_bus_0		: in  std_logic; 	-- spi_clk  / tck
	stm_bus_1		: out std_logic; 	-- spi_miso / tdo
	stm_bus_2		: in  std_logic; 	-- spi_mosi / tdi
	stm_bus_3		: in  std_logic; 	-- spi_cs   / tms
	

	-- connected boards
	board_spi_clk	: out std_logic_vector(7 downto 0);
	board_spi_miso	: in  std_logic_vector(7 downto 0);	
	board_spi_mosi	: out std_logic_vector(7 downto 0);	
	board_spi_cs	: out std_logic_vector(7 downto 0);
	
	board_tck		: out std_logic_vector(7 downto 0);
	board_tdo		: in  std_logic_vector(7 downto 0);	
	board_tdi		: out std_logic_vector(7 downto 0);		
	board_tms		: out std_logic_vector(7 downto 0)
	
	);
end control_cpld_01;

architecture rtl of control_cpld_01 is

signal	channel	: integer range 0 to 7;

begin

channel	<= conv_integer(sel_channel); 


process(sel_mode, channel, stm_bus_0, stm_bus_2, stm_bus_3, 
				board_spi_miso, board_tdo) 
begin

	
	stm_bus_1			<= '0'; 
		
	for i in 0 to 7 loop
	
		board_spi_clk(i) 	<= '0'; 
		board_spi_mosi(i)	<= '0';
		board_spi_cs(i)	<= '0';
		board_tck(i)		<= '0'; 
		board_tdi(i)		<= '0'; 
		board_tms(i)		<= '0'; 	
	
		if (i = channel) then
	
			if (sel_mode = '0') then 		-- spi
		
			
				board_spi_clk(i) 	<= stm_bus_0; 
				board_spi_mosi(i)	<= stm_bus_2;
				board_spi_cs(i)	<= stm_bus_3;	
				
				stm_bus_1	<= board_spi_miso(channel); 
				
			else 								-- jtag
			
				board_tck(i)		<= stm_bus_0; 
				board_tdi(i)		<= stm_bus_2; 
				board_tms(i)		<= stm_bus_3; 		

				stm_bus_1	<= board_tdo(channel); 
	
			end if; 
			
		
		end if; 

	end loop;

end process;


end rtl;

