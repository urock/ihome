/** \file user_timer.c
 *  \brief File containing Timer Init and delay functions

RC47 STM program uses 3 counters:
	1. Systick - 1 msec interrupts - used for makeing Delay in millisecs
	2. TIM2	- dt = 50 ns - used for precise timing measurments
	3. TIM3 - 1 sec interrupts - used from trigger periodic work

Presice timing measurments example (TIM2)

/code
TIM_SetCounter(TIM2,0);
code_to_be_measured();
t2 = TIM_GetCounter(TIM2);
/endcode

*/

#include "main.h"
#include "user_timer.h"



static __IO uint32_t TimingDelay;

uint32_t I2C_Timeout;
uint8_t  I2C_Watch; 


/** 
 *  \brief Timers Init
 	

	\return None
 */
void user_timer_init1(void)
{

//  
   // Setup SysTick Timer for 1 msec interrupts  
	if (SysTick_Config(SystemCoreClock /1000))
	{ 
		while (1);
	}	
  
  	
	TIM2_Config(20000000); // set TIM2_count_clk to 20 MHz; dt = 50 ns
	
	
	TIM3_Config(10000,1000); // TIM3_count_clk to 10 kHz, update event every 300 msec
  	// TIM3 uses interrupt, but it is enabled later in main function after all init is complete
	
}



	
// 	tim_count_clk - value in Hz

void TIM2_Config(uint32_t tim_count_clk)
{
	uint16_t PrescalerValue = 0;
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  
  /*******************************************************
  
	
   tim_system_clk = SystemCoreClock/2 = 120 MHz / 2 = 60 MHz
  
	TIM_count_clk = tim_system_clk/(PrescalerValue + 1);
    
  *******************************************************/


 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	 
 

//enable clk
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	  
// Compute the prescaler value
//	SystemCoreClock = 120 MHz
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / tim_count_clk) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 0xFFFFFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  
  TIM_Cmd(TIM2, ENABLE);

  
}



// 	tim_count_clk - value in Hz
void TIM3_Config(uint32_t tim_count_clk, uint16_t period)
{
	uint16_t PrescalerValue = 0;  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
  
	  /* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	// Compute the prescaler value
	//	SystemCoreClock = 120 MHz
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / tim_count_clk) - 1;
	  
	
	  
	
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
	
//	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
  

  
}


/** 
 *  \brief Wait function
 	
	\param nTime Miliseconds to wait
	\return None
 */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00) { 
		TimingDelay--;
	}
  

}


