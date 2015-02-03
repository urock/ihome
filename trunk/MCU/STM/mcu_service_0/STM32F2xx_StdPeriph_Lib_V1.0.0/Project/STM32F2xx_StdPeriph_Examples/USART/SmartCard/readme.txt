/**
  @page USART_Smartcard  USART Smart Card example
  
  @verbatim
  ******************** (C) COPYRIGHT 2011 STMicroelectronics *******************
  * @file    USART/SmartCard/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Description of the USART Smart Card example.
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

This example describes a firmware Smartcard Interface based on the STM32F2xx USART
peripheral. The main purpose of this firmware example is to provide resources 
facilitating the development of an application using the USART peripheral in 
smartcard mode.

The firmware interface is composed of library source files developed in order to
support ISO7816-3/4 specification, an application example is also provided.

This example is based on the AN2598 "Smartcard interface with the STM32F101xx 
and STM32F103xx" application note. For more details, please refer to AN2598 
available on www.st.com


@par Directory contents 

    - USART/SmartCard/platform_config.h   Evaluation board specific configuration file
    - USART/SmartCard/stm32f2xx_conf.h    Library Configuration file
    - USART/SmartCard/stm32f2xx_it.h      Interrupt handlers header file
    - USART/SmartCard/smartcard.h         Smart card Header file
    - USART/SmartCard/stm32f2xx_it.c      Interrupt handlers
    - USART/SmartCard/smartcard.c         Smart Card firmware functions
    - USART/SmartCard/main.c              Main program  
    - USART/SmartCard/system_stm32f2xx.c  STM32F2xx system source file


@par Hardware and Software environment

  - This example runs on STM32F2xx Devices.
  
  - This example has been tested with STM322xG-EVAL RevB and can be easily tailored
    to any other development board.

  - STM322xG-EVAL Set-up 
    - Use LED1, LED2, LED3 and LED4 connected respectively to PG.06, PG.08, PI.09
      and PC.07 pins
    - Make sure that JP21 is closed.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Copy all source files from this example folder to the template folder under
   Project\STM32F2xx_StdPeriph_Template
 - Open your preferred toolchain
 - Add the "smartcard.c" file to the project source list  
 - Rebuild all files and load your image into target memory
 - Run the example 
 
 * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
 */
