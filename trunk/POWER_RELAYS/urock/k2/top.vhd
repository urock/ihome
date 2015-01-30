-- Description: 	SPI slave controller assuming CPOL = 0, CPHA = 1;
--						
--					 	SPI Master sets bits at rising edge, slave captures them at falling
--		

--						SPI Chip Select is active low


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity cpld_relay_3_u2 is


	port(
		clk25 				: in std_logic;
		port_switch			: in std_logic;
		led_switch			: out std_logic;
		-- pb inputs	
		pb_in 				: in  std_logic_vector (7 downto 0);
		--pb_in_mv 			: in  std_logic_vector (7 downto 0);  --Для датчиков движения
		
		-- led outputs
		power_out 		: out std_logic_vector (7 downto 0);
		
		-- test pins 
		spi_out_test1	: out std_logic;
		spi_out_test2	: out std_logic;
		
		-- stm spi
		spi_cs					: in	std_logic;
		spi_clk					: in	std_logic;
		spi_i						: in	std_logic;	
		spi_o						: out	std_logic;
		
		stm_unused				: in  std_logic_vector(11 downto 0)

	);

end cpld_relay_3_u2;


architecture top of cpld_relay_3_u2 is	   


constant RELAY_ID				: std_logic_vector(7 downto 0) := "00100000";--X"84";

signal power_state			: std_logic_vector (7 downto 0):= "00010000";
--signal moves_state			: std_logic_vector (7 downto 0):= "00001000";

signal pb_in_n					: std_logic_vector (7 downto 0):= "00000000";
signal pb_in_tick				: std_logic_vector (7 downto 0);



type spi_stm is (IDLE_ST, CMD_ST);
signal state	: spi_stm := IDLE_ST;


constant CPLD_CMD_READ		: std_logic_vector(3 downto 0) := "0001";
constant CPLD_CMD_WRITE		: std_logic_vector(3 downto 0) := "0010"; 
constant CPLD_CMD_READ_ID	: std_logic_vector(3 downto 0) := "0011"; 
--constant CPLD_CMD_READ_MV	: std_logic_vector(3 downto 0) := "0100";   -- move sensor


signal re		: std_logic := '0'; --read enable
signal we		: std_logic := '0'; --write enable 
signal we_en	: std_logic := '0'; 
signal we_n		: std_logic := '0'; 
signal we_d		: std_logic := '0';
 
signal spi_data_reg		: std_logic_vector (7 downto 0):= "00000000";

signal Cnt8		: std_logic_vector(2 downto 0);
signal Reg		: std_logic_vector(3 downto 0);
signal cmd		: std_logic_vector(3 downto 0);


signal led_switch_r	: std_logic := '0';

signal port_switch_n	: std_logic;
signal port_switch_d	: std_logic;
signal port_switch_t	: std_logic;

begin  


-- SPI controller

-- output process
spi_o	<= spi_data_reg(7);-- when spi_cs = '0' and re = '1' else 'Z'; 

spi_out_test1 <= re; 
spi_out_test2 <= we; 
--spi_o	<= spi_clk	when spi_cs = '0' 	else 'Z'; 

-- SPI impulse counter 
process (spi_clk, spi_cs)
begin
if spi_cs = '1' then
	Cnt8 <= "000";
elsif rising_edge(spi_clk) then	
	Cnt8 <= Cnt8 + 1;
end if;
end process;



-- least 2 bits register
process (spi_clk, spi_cs)
begin
if spi_cs = '1'  then
	Reg <= "0000";
elsif falling_edge(spi_clk)then
	Reg <= Reg(2 downto 0) & SPI_i;
end if;
end process;

cmd	<= Reg(2 downto 0) & SPI_i; 
--cmd	<= Reg; 


-- spi state machine
process (spi_clk, spi_cs)
begin
if spi_cs = '1'  then
	
	state <= IDLE_ST; 
	re	<= '0'; 
	we	<= '0'; 
--	we_en	<= '0';
	
elsif falling_edge(spi_clk)then
	
	if state = IDLE_ST then
	
		if Cnt8 = "000" then		-- end of first byte
				
			state <= CMD_ST; 
				
			case cmd is 
				when CPLD_CMD_READ 		=> 	
					re		<= '1'; 
					spi_data_reg	<= power_state; 
						
				when CPLD_CMD_WRITE 		=>
					we		<= '1'; 
					
				when CPLD_CMD_READ_ID	=> 	
					re	<= '1'; 
					spi_data_reg	<= RELAY_ID;
					
--				when CPLD_CMD_READ_MV	=>
--					re		<= '1'; 
--					spi_data_reg	<= moves_state; 
--				
				when others => null; 
			end case; 
		end if; 
	
	else 
		state <= CMD_ST; 
		
--		if Cnt8 = "000" then -- end of second byte
--		
--			state <= IDLE_ST; 
--			re	<= '0'; 
--			we	<= '0'; 
--			we_en	<= '0';	
--		end if; 
			
	end if; 
	
	
	-- spi register
	if we = '1' then
		spi_data_reg <= spi_data_reg(6 downto 0) & SPI_i;
	elsif re = '1' then
		spi_data_reg <= spi_data_reg(6 downto 0) & SPI_i; --spi_data_reg(7);		
	end if; 
						
end if;
end process;


	-- generate tick
	process(clk25)
	begin

		if rising_edge(clk25) then
			
			for i in 0 to 7 loop
				pb_in_n(i)		<= not pb_in(i);
				pb_in_tick(i)	<= pb_in(i) and pb_in_n(i);
			end loop;			
			
			
			port_switch_n	<= not port_switch;
			port_switch_t	<= port_switch and port_switch_n; 
			
			
			if port_switch_t = '1' then			
				led_switch_r	<= not led_switch_r; 
			end if;
			
			
		end if;
	end process;
	
	
	led_switch <= led_switch_r; 
--	led_switch <= '1'; 
	
	

	process(clk25)
	begin
		if rising_edge(clk25) then
			we_d <= we; 
			
			we_en 	<= we_d and (not we); 
			

			if we_en = '1' then
				power_state	<= spi_data_reg; 
			else 
--				for i in 0 to 7 loop
--					if pb_in_tick(i) = '1' then
--						power_state(i)	<= not power_state(i); 
--					end if; 
--				end loop;
				
				
				if pb_in_tick(0) = '1' or pb_in_tick(2) = '1' then
					power_state(0)	<= not power_state(0); 
				end if; 

				if pb_in_tick(1) = '1' or pb_in_tick(3) = '1' then
					power_state(1)	<= not power_state(1); 
				end if; 

				if pb_in_tick(4) = '1' or pb_in_tick(6) = '1' then
					power_state(2)	<= not power_state(2); 
				end if; 

				if pb_in_tick(5) = '1' or pb_in_tick(7) = '1' then
					power_state(3)	<= not power_state(3); 
				end if; 


			end if; 

			
		end if;	
	end process;

power_out	<= "0000" & power_state(3 downto 0); 

--
----state moves
--	process (clk25)
--	begin
--	if rising_edge(clk25) then
--		moves_state <= not pb_in_mv;
--	end if;
--	end process;

	
end top;