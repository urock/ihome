/**
  @page TIM_ExtTrigger_Synchro TIM External Trigger Synchro example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    TIM/ExtTrigger_Synchro/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Description of the TIM External Trigger Synchro example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Example Description 

This example shows how to synchronize TIM peripherals in cascade mode with an
external trigger.
In this example three timers are used:

1/TIM1 is configured as Master Timer:
    - Toggle Mode is used
    - The TIM1 Enable event is used as Trigger Output 

2/TIM1 is configured as Slave Timer for an external Trigger connected to TIM1 
  TI2 pin (TIM1 CH2 configured as input pin):
    - The TIM1 TI2FP2 is used as Trigger Input
    - Rising edge is used to start and stop the TIM1: Gated Mode.

3/TIM3 is slave for TIM1 and Master for TIM4,
    - Toggle Mode is used
    - The ITR1(TIM1) is used as input trigger 
    - Gated mode is used, so start and stop of slave counter
      are controlled by the Master trigger output signal(TIM1 enable event).
    - The TIM3 enable event is used as Trigger Output. 

4/TIM4 is slave for TIM3,
    - Toggle Mode is used
    - The ITR2(TIM3) is used as input trigger
    - Gated mode is used, so start and stop of slave counter
      are controlled by the Master trigger output signal(TIM3 enable event).

   The TIM1CLK is fixed to 120 MHZ, the Prescaler is equal to 4 so the TIMx clock 
   counter is equal to 24 MHz.
   The TIM3CLK  and TIM4CLK are fixed to 60 MHZ, the Prescaler is equal to 4 
   so the TIMx clock counter is equal to 12 MHz. 
   The Three Timers are running at: 
   TIMx frequency = TIMx clock counter/ 2*(TIMx_Period + 1) = 162.1 KHz.
   
The starts and stops of the TIM1 counters are controlled by the external trigger.
The TIM3 starts and stops are controlled by the TIM1, and the TIM4 starts and 
stops are controlled by the TIM3.  

@par Directory contents 

  - TIM/ExtTrigger_Synchro/stm32f2xx_conf.h    Library Configuration file
  - TIM/ExtTrigger_Synchro/stm32f2xx_it.c      Interrupt handlers
  - TIM/ExtTrigger_Synchro/stm32f2xx_it.h      Interrupt handlers header file
  - TIM/ExtTrigger_Synchro/main.c              Main program 
  - TIM/ExtTrigger_Synchro/system_stm32f2xx.c  STM32F2xx system source file

@note The "system_stm32f2xx.c" is generated by an automatic clock configuration 
      tool and can be easily customized to your own configuration. 
      To select different clock setup, use the "STM32F2xx_Clock_Configuration_V1.0.0.xls" tool.
  
@par Hardware and Software environment 

  - This example runs on STM32F2xx Devices.
  
  - This example has been tested with STM322xG-EVAL RevB and can be easily tailored
    to any other development board.

  - STM322xG-EVAL Set-up 
    - Connect an external trigger, with a frequency <= 40KHz, to the TIM1 CH2 
      pin (PE.11). In this example the frequency is equal to 5 KHz.

    - Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1 CH1 (PA.08)
      - TIM3 CH1 (PB.04)
      - TIM4 CH1 (PB.06)    
  
@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F2xx_StdPeriph_Template
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
   
 * <h3><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h3>
 */
