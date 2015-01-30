#ifndef __INIT_CONST_H
#define __INIT_CONST_H


/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "stm322xg_eval.h"
#include "stm32f2xx_sdio.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_i2c.h"
#include "stm32f2xx.h"
#include "croutine.h"
#include "timers.h"

//-------------- Debug ---------------------------------------------------------
#define Debug_my        0

// ---------- Состояния автомата (my_task) при нажатой кнопке ------------------
#define button_On        1    
#define button_Off       0   
    
#define state_button_Off       0       
#define state_button_On        1           

//----------- Приоритеты задач -------------------------------------------------
/*--------------- Tasks Priority -------------*/
/**
* @brief Task Priority
*/       
#define HTTP_SERVER_TASK_PRIO   ( tskIDLE_PRIORITY + 2 )
#define INPUTS_TASK_PRIO        ( tskIDLE_PRIORITY + 3 )
#define ADC_TASK_PRIO           ( tskIDLE_PRIORITY + 4 )
#define PortInfo_TASK_PRIO      ( tskIDLE_PRIORITY + 5 )
#define ONEWIRE_TASK_PRIO       ( tskIDLE_PRIORITY + 6 )

#define HTTP_Cient_TASK_PRIO    ( tskIDLE_PRIORITY + 7 )
#define ConfigPort_TASK_PRIO    ( tskIDLE_PRIORITY + 8 )
#define PULSE_TASK_PRIO         ( tskIDLE_PRIORITY + 9 )

#define DHCP_TASK_PRIO          ( tskIDLE_PRIORITY + 10)


//------------Константы для передачи по Ethernet -------------------------------
#define begin_pack        "GET /objects/?object=Port"
#define end_pack          " HTTP/1.1\r\nHost: 192.168.1.111\r\nConnection: Close\r\n\r\n\0"
#define status_pack       "&op=m&m=changedStatus&status="
#define mode_pack         "&op=m&m=changedMode&mode="
#define value_pack        "&op=m&m=changedValue&val="
#define enable_pack       "&op=m&m=changedActive&enable="
#define key_pack          "&op=m&m=changedKey&key="
#define temper_pack       "GET /objects/?object=tempSensor&op=m&m=tempChanged&temp="



//---------Command -------------------------------------------------------------
//#define  SetStatus      0
//#define  SetMode        1
//#define  SetValue       2
//#define  SetKey         3
//#define  SetEnable      4
//
//#define  GetStatus      5
//#define  GetMode        6
//#define  GetValue       7
//#define  GetKey         8
//#define  GetEnable      9
//
//#define  WriteFlash     10
//#define  ClearFlash     11

//typedef enum 
//{
//SetStatus     = 0,
//SetMode       = 1,
//SetValue      = 2,
//SetKey        = 3,
//SetEnable     = 4,
//GetStatus     = 5,
//GetMode       = 6,
//GetValue      = 7,
//GetKey        = 8,
//GetEnable     = 9,
//WriteFlash    = 10,
//ClearFlash    = 11
//} Cmd_TypeDef;


//-------CPLD ------------------
#define Max_Light_cmd_cpld 4  // Длина команды в CPLD
#define Max_Light_ID_cpld  4  // Максимальная длина ID CPLD
#define Cmd_Read_ID_cpld   1  // Команда для считывания ID из платы командв 0001
#define Cmd_auto_cpld      2  // Команда перевода cpld в автоматический режим (нормальный режим)
#define Cmd_manual_cpld    3  // Команда перевода cpld в ручной режим - на плате power передаются in0 на out0 и т.д.

#define ID_I8O8_relay      1  // ID платы имеющий 8 входов и 8 выходов

//------ FLASH -----------------
#define USER_FLASH_BASE 		0x080E0000  // Адрес куда начинается зап
#define USER_FLASH_INFO 		0x080C0000


//-------- Outputs ---------------
#define MAX_PORTs      32 // 40  пока используем только верхний разъем

//---- Распиновка разъема на каких пинах будут назначены входы и выходы
#define MIN_PORT_IN     8
#define MAX_PORT_IN     15
#define MIN_PORT_OUT    0
#define MAX_PORT_OUT    7
//-------------------------------

#define PROG_VERTION  1

//#define MAX_PORTs_out      12 // 24 на оба разъема
//#define MAX_PORTs_in       7 //14  на оба разъема -   посмотреть еще нижние !!!! есть ли там 8 входов под прерывания
//#define MAX_PORTs_in_shift     MAX_PORTs -  MAX_PORTs_in  // Будем использовать Port18 и Port 19  //Если нужно использоват все 7 портов то нужно установить 13 - это будет 7 портов
//#define PORTs_in_beign_shift           13  // С какого порта начинается конфигурация inputs, до порта 19 - это 7 портов

#define Off_port 0  
#define On_port 1  


//----------- LEDS -------------------------------
/**
* @brief Control LEDs mcu board (Led_TypeDef)
*/
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
} Led_TypeDef;

/** @addtogroup STM322xG_EVAL_LOW_LEVEL_LED
   * @brief LEDS control pins
  */
#define LEDn                             4

#define LED1_PIN                         GPIO_Pin_5
#define LED1_GPIO_PORT                   GPIOE
#define LED1_GPIO_CLK                    RCC_AHB1Periph_GPIOE
  
#define LED2_PIN                         GPIO_Pin_6
#define LED2_GPIO_PORT                   GPIOE
#define LED2_GPIO_CLK                    RCC_AHB1Periph_GPIOE
  
#define LED3_PIN                         GPIO_Pin_13
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCC_AHB1Periph_GPIOC
  
#define LED4_PIN                         GPIO_Pin_14
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK                    RCC_AHB1Periph_GPIOC

/**
* @brief LED STATUS pins
*/
#define LED_alarm                        GPIO_Pin_3
#define LED_alarm_PORT                   GPIOE
#define LED_alerm_CLK                    RCC_AHB1Periph_GPIOE
  
#define LED_warning                      GPIO_Pin_4
#define LED_warning_PORT                 GPIOE
#define LED_warning_CLK                  RCC_AHB1Periph_GPIOE
  
#define LED_status                       GPIO_Pin_2
#define LED_status_PORT                  GPIOE
#define LED_status_CLK                   RCC_AHB1Periph_GPIOE



//------ UARTs ----------------------------------------
/**
* @brief UART port mcu_board
*/
typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;

/** @addtogroup STM322xG_EVAL_LOW_LEVEL_COM
  * @{
  */
#define COMn                             2
/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define EVAL_COM1                        USART1
//#define EVAL_COM1_CLK                    RCC_APB1Periph_USART1
#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource9
#define EVAL_COM1_TX_AF                  GPIO_AF_USART1
#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_RX_AF                  GPIO_AF_USART1
#define EVAL_COM1_IRQn                   USART1_IRQn

/**
 * @brief Definition for COM port2, connected to USART2
 */ 
#define EVAL_COM2                        USART2
//#define EVAL_COM2_CLK                    RCC_APB1Periph_USART2
#define EVAL_COM2_TX_PIN                 GPIO_Pin_5
#define EVAL_COM2_TX_GPIO_PORT           GPIOD
#define EVAL_COM2_TX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM2_TX_SOURCE              GPIO_PinSource5
#define EVAL_COM2_TX_AF                  GPIO_AF_USART2
#define EVAL_COM2_RX_PIN                 GPIO_Pin_6
#define EVAL_COM2_RX_GPIO_PORT           GPIOD
#define EVAL_COM2_RX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define EVAL_COM2_RX_SOURCE              GPIO_PinSource6
#define EVAL_COM2_RX_AF                  GPIO_AF_USART2
#define EVAL_COM2_IRQn                   USART2_IRQn
//------------------------------------

//------------------------------------------------------------------------------

/**
* @brief  Every port have her number (Ports_TypeDef)
*/
typedef enum 
{
  Port0 = 0,
  Port1 = 1,
  Port2 = 2,
  Port3 = 3,
  Port4 = 4,
  Port5 = 5,
  Port6 = 6,
  Port7 = 7,
  Port8 = 8,
  Port9 = 9,
  Port10 = 10,
  Port11 = 11,
  Port12 = 12,
  Port13 = 13,
  Port14 = 14,
  Port15 = 15,
  Port16 = 16,
  Port17 = 17,
  Port18 = 18,
  Port19 = 19,
  Port20 = 20,
  Port21 = 21,
  Port22 = 22,
  Port23 = 23,
  Port24 = 24,
  Port25 = 25,
  Port26 = 26,
  Port27 = 27,
  Port28 = 28,
  Port29 = 29,
  Port30 = 30,
  Port31 = 31
    ,
  
  Port100 = 100   // error port
} Ports_TypeDef;

//------- Inputs --------------------
/**
* @brief  Mode Port  (GPIO, Interrupt) (PortIO_TypeDef)
*/
typedef enum 
{  
  PORT_MODE_GPIO = 0,
  PORT_MODE_EXTI = 1
} PortIO_TypeDef;
//-----------------------------------
/**
* @brief State of Ports (On, Off) (PortStatus_TypeDef)
*/
typedef enum 
{  
  Off = Off_port,
  On = On_port
} PortStatus_TypeDef;
//-----------------------------------
/**
* @brief Mode of Ports (OUT, IN, PWM, ADC) (PortMode_TypeDef)
*/
typedef enum 
{  
 OUT_pin = 0,
 IN_pin = 1,
 PWM_OUT_pin = 2,
 ADC_IN_pin = 3,
} PortMode_TypeDef;
//----------------------------------------
/**
* @brief Enable \ Disable Ports (PortEnable_TypeDef)
*/
typedef enum 
{  
  disable = 0,
  enable = 1
} PortEnable_TypeDef;

//-----------------------------------------
/**
* @brief Structure Ports  (PORT_struct)
* @param num_port, value, status, mode, key, enable
*/
  struct PORT_struct {
      uint8_t num_port ;
      int value ;  // value port for dimmer
      PortStatus_TypeDef status ; // status port (on\off)
      PortMode_TypeDef mode ;   // (out,in,pwm.adc)
      uint8_t key ;    // key for other port
      PortEnable_TypeDef enable ; // enabled port
  };
//------------------------------------------
/**
* @brief  Структура хранящая информацию о программе и настройках
*/  
   struct info_struct_t {
     uint8_t ver ;
     uint8_t ID_board_flash ;          
   };

/**
* @brief Number Ports of MCU
*/
//#define PORTn                             32
//------------------------------------------------------
/**
* @brief Pins for Connector UP (OUTPUTs)  
*/
//---- LEDS STM board -------------------------------------  
#define Port0_PIN                         GPIO_Pin_8
#define Port0_GPIO_PORT                   GPIOB
#define Port0_GPIO_CLK                    RCC_AHB1Periph_GPIOB
#define Port0_EXTI_LINE                   EXTI_Line8
#define Port0_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
#define Port0_EXTI_PIN_SOURCE             EXTI_PinSource8
#define Port0_EXTI_IRQn                   EXTI9_5_IRQn     
     
#define Port1_PIN                         GPIO_Pin_7
#define Port1_GPIO_PORT                   GPIOB
#define Port1_GPIO_CLK                    RCC_AHB1Periph_GPIOB
#define Port1_EXTI_LINE                   EXTI_Line7
#define Port1_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
#define Port1_EXTI_PIN_SOURCE             EXTI_PinSource7
#define Port1_EXTI_IRQn                   EXTI9_5_IRQn 
  
#define Port2_PIN                         GPIO_Pin_6
#define Port2_GPIO_PORT                   GPIOB
#define Port2_GPIO_CLK                    RCC_AHB1Periph_GPIOB
#define Port2_EXTI_LINE                   EXTI_Line6
#define Port2_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
#define Port2_EXTI_PIN_SOURCE             EXTI_PinSource6
#define Port2_EXTI_IRQn                   EXTI9_5_IRQn 
   
  
#define Port3_PIN                         GPIO_Pin_5
#define Port3_GPIO_PORT                   GPIOB
#define Port3_GPIO_CLK                    RCC_AHB1Periph_GPIOB
#define Port3_EXTI_LINE                   EXTI_Line5
#define Port3_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
#define Port3_EXTI_PIN_SOURCE             EXTI_PinSource5
#define Port3_EXTI_IRQn                   EXTI9_5_IRQn 
   

#define Port4_PIN                         GPIO_Pin_4
#define Port4_GPIO_PORT                   GPIOB
#define Port4_GPIO_CLK                    RCC_AHB1Periph_GPIOB
#define Port4_EXTI_LINE                   EXTI_Line4
#define Port4_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
#define Port4_EXTI_PIN_SOURCE             EXTI_PinSource4
#define Port4_EXTI_IRQn                   EXTI4_IRQn 
   
  
#define Port5_PIN                         GPIO_Pin_3
#define Port5_GPIO_PORT                   GPIOB
#define Port5_GPIO_CLK                    RCC_AHB1Periph_GPIOB
#define Port5_EXTI_LINE                   EXTI_Line3
#define Port5_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
#define Port5_EXTI_PIN_SOURCE             EXTI_PinSource3
#define Port5_EXTI_IRQn                   EXTI3_IRQn 
   
  
#define Port6_PIN                         GPIO_Pin_4
#define Port6_GPIO_PORT                   GPIOD
#define Port6_GPIO_CLK                    RCC_AHB1Periph_GPIOD
#define Port6_EXTI_LINE                   EXTI_Line4
#define Port6_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOD
#define Port6_EXTI_PIN_SOURCE             EXTI_PinSource4
#define Port6_EXTI_IRQn                   EXTI4_IRQn 
   
  
#define Port7_PIN                         GPIO_Pin_3
#define Port7_GPIO_PORT                   GPIOD
#define Port7_GPIO_CLK                    RCC_AHB1Periph_GPIOD
#define Port7_EXTI_LINE                   EXTI_Line3
#define Port7_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOD
#define Port7_EXTI_PIN_SOURCE             EXTI_PinSource3
#define Port7_EXTI_IRQn                   EXTI3_IRQn 
   

#define Port8_PIN                         GPIO_Pin_2
#define Port8_GPIO_PORT                   GPIOD
#define Port8_GPIO_CLK                    RCC_AHB1Periph_GPIOD
#define Port8_EXTI_LINE                   EXTI_Line2
#define Port8_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOD
#define Port8_EXTI_PIN_SOURCE             EXTI_PinSource2
#define Port8_EXTI_IRQn                   EXTI2_IRQn 
   
  
#define Port9_PIN                         GPIO_Pin_1
#define Port9_GPIO_PORT                   GPIOD
#define Port9_GPIO_CLK                    RCC_AHB1Periph_GPIOD
#define Port9_EXTI_LINE                   EXTI_Line1
#define Port9_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOD
#define Port9_EXTI_PIN_SOURCE             EXTI_PinSource1
#define Port9_EXTI_IRQn                   EXTI1_IRQn 
  
#define Port10_PIN                         GPIO_Pin_0
#define Port10_GPIO_PORT                   GPIOD
#define Port10_GPIO_CLK                    RCC_AHB1Periph_GPIOD
#define Port10_EXTI_LINE                   EXTI_Line0
#define Port10_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOD
#define Port10_EXTI_PIN_SOURCE             EXTI_PinSource0
#define Port10_EXTI_IRQn                   EXTI0_IRQn 
   
  
#define Port11_PIN                         GPIO_Pin_12
#define Port11_GPIO_PORT                   GPIOC
#define Port11_GPIO_CLK                    RCC_AHB1Periph_GPIOC
#define Port11_EXTI_LINE                   EXTI_Line12
#define Port11_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOC
#define Port11_EXTI_PIN_SOURCE             EXTI_PinSource12
#define Port11_EXTI_IRQn                   EXTI15_10_IRQn 
   
   
#define Port12_PIN                         GPIO_Pin_11
#define Port12_GPIO_PORT                   GPIOC
#define Port12_GPIO_CLK                    RCC_AHB1Periph_GPIOC
#define Port12_EXTI_LINE                   EXTI_Line11
#define Port12_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOC
#define Port12_EXTI_PIN_SOURCE             EXTI_PinSource11
#define Port12_EXTI_IRQn                   EXTI15_10_IRQn 
    
#define Port13_PIN                         GPIO_Pin_10
#define Port13_GPIO_PORT                   GPIOC
#define Port13_GPIO_CLK                    RCC_AHB1Periph_GPIOC
#define Port13_EXTI_LINE                   EXTI_Line10
#define Port13_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOC
#define Port13_EXTI_PIN_SOURCE             EXTI_PinSource10
#define Port13_EXTI_IRQn                   EXTI15_10_IRQn 
  
#define Port14_PIN                         GPIO_Pin_11
#define Port14_GPIO_PORT                   GPIOA
#define Port14_GPIO_CLK                    RCC_AHB1Periph_GPIOA
#define Port14_EXTI_LINE                   EXTI_Line11
#define Port14_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOA
#define Port14_EXTI_PIN_SOURCE             EXTI_PinSource11
#define Port14_EXTI_IRQn                   EXTI15_10_IRQn 
  
#define Port15_PIN                         GPIO_Pin_12
#define Port15_GPIO_PORT                   GPIOA
#define Port15_GPIO_CLK                    RCC_AHB1Periph_GPIOA
#define Port15_EXTI_LINE                   EXTI_Line12
#define Port15_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOA
#define Port15_EXTI_PIN_SOURCE             EXTI_PinSource12
#define Port15_EXTI_IRQn                   EXTI15_10_IRQn 
      
   
#define Port16_PIN                         GPIO_Pin_9 
#define Port16_GPIO_PORT                   GPIOE
#define Port16_GPIO_CLK                    RCC_AHB1Periph_GPIOE
#define Port16_EXTI_LINE                   EXTI_Line9
#define Port16_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOE
#define Port16_EXTI_PIN_SOURCE             EXTI_PinSource9
#define Port16_EXTI_IRQn                   EXTI9_5_IRQn 
  
#define Port17_PIN                         GPIO_Pin_10
#define Port17_GPIO_PORT                   GPIOE
#define Port17_GPIO_CLK                    RCC_AHB1Periph_GPIOE
#define Port17_EXTI_LINE                   EXTI_Line10
#define Port17_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOE
#define Port17_EXTI_PIN_SOURCE             EXTI_PinSource10
#define Port17_EXTI_IRQn                   EXTI15_10_IRQn
  
#define Port18_PIN                         GPIO_Pin_11
#define Port18_GPIO_PORT                   GPIOE
#define Port18_GPIO_CLK                    RCC_AHB1Periph_GPIOE
#define Port18_EXTI_LINE                   EXTI_Line11
#define Port18_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOE
#define Port18_EXTI_PIN_SOURCE             EXTI_PinSource11
#define Port18_EXTI_IRQn                   EXTI15_10_IRQn 
    
#define Port19_PIN                         GPIO_Pin_12
#define Port19_GPIO_PORT                   GPIOE
#define Port19_GPIO_CLK                    RCC_AHB1Periph_GPIOE
#define Port19_EXTI_LINE                   EXTI_Line12
#define Port19_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOE
#define Port19_EXTI_PIN_SOURCE             EXTI_PinSource12
#define Port19_EXTI_IRQn                   EXTI15_10_IRQn
  
#define Port20_PIN                         GPIO_Pin_13
#define Port20_GPIO_PORT                   GPIOE
#define Port20_GPIO_CLK                    RCC_AHB1Periph_GPIOE
#define Port20_EXTI_LINE                   EXTI_Line13
#define Port20_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOE
#define Port20_EXTI_PIN_SOURCE             EXTI_PinSource13
#define Port20_EXTI_IRQn                   EXTI15_10_IRQn
  
#define Port21_PIN                         GPIO_Pin_14
#define Port21_GPIO_PORT                   GPIOE
#define Port21_GPIO_CLK                    RCC_AHB1Periph_GPIOE
#define Port21_EXTI_LINE                   EXTI_Line14
#define Port21_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOE
#define Port21_EXTI_PIN_SOURCE             EXTI_PinSource14
#define Port21_EXTI_IRQn                   EXTI15_10_IRQn
   
  
#define Port22_PIN                         GPIO_Pin_14
#define Port22_GPIO_PORT                   GPIOB
#define Port22_GPIO_CLK                    RCC_AHB1Periph_GPIOB
#define Port22_EXTI_LINE                   EXTI_Line14
#define Port22_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
#define Port22_EXTI_PIN_SOURCE             EXTI_PinSource14
#define Port22_EXTI_IRQn                   EXTI15_10_IRQn
   

#define Port23_PIN                         GPIO_Pin_15
#define Port23_GPIO_PORT                   GPIOB
#define Port23_GPIO_CLK                    RCC_AHB1Periph_GPIOB
#define Port23_EXTI_LINE                   EXTI_Line15
#define Port23_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
#define Port23_EXTI_PIN_SOURCE             EXTI_PinSource15
#define Port23_EXTI_IRQn                   EXTI15_10_IRQn
   
  
#define Port24_PIN                         GPIO_Pin_12
#define Port24_GPIO_PORT                   GPIOD
#define Port24_GPIO_CLK                    RCC_AHB1Periph_GPIOD
#define Port24_EXTI_LINE                   EXTI_Line12
#define Port24_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOD
#define Port24_EXTI_PIN_SOURCE             EXTI_PinSource12
#define Port24_EXTI_IRQn                   EXTI15_10_IRQn
   
  
#define Port25_PIN                         GPIO_Pin_13
#define Port25_GPIO_PORT                   GPIOD
#define Port25_GPIO_CLK                    RCC_AHB1Periph_GPIOD
#define Port25_EXTI_LINE                   EXTI_Line13
#define Port25_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOD
#define Port25_EXTI_PIN_SOURCE             EXTI_PinSource13
#define Port25_EXTI_IRQn                   EXTI15_10_IRQn
   

#define Port26_PIN                         GPIO_Pin_14
#define Port26_GPIO_PORT                   GPIOD
#define Port26_GPIO_CLK                    RCC_AHB1Periph_GPIOD
#define Port26_EXTI_LINE                   EXTI_Line14
#define Port26_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOD
#define Port26_EXTI_PIN_SOURCE             EXTI_PinSource14
#define Port26_EXTI_IRQn                   EXTI15_10_IRQn
   
  
#define Port27_PIN                         GPIO_Pin_15
#define Port27_GPIO_PORT                   GPIOD
#define Port27_GPIO_CLK                    RCC_AHB1Periph_GPIOD
#define Port27_EXTI_LINE                   EXTI_Line15
#define Port27_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOD
#define Port27_EXTI_PIN_SOURCE             EXTI_PinSource15
#define Port27_EXTI_IRQn                   EXTI15_10_IRQn
   
  
#define Port28_PIN                         GPIO_Pin_6
#define Port28_GPIO_PORT                   GPIOC
#define Port28_GPIO_CLK                    RCC_AHB1Periph_GPIOC
#define Port28_EXTI_LINE                   EXTI_Line6
#define Port28_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOC
#define Port28_EXTI_PIN_SOURCE             EXTI_PinSource6
#define Port28_EXTI_IRQn                   EXTI9_5_IRQn
   

#define Port29_PIN                         GPIO_Pin_7
#define Port29_GPIO_PORT                   GPIOC
#define Port29_GPIO_CLK                    RCC_AHB1Periph_GPIOC
#define Port29_EXTI_LINE                   EXTI_Line7
#define Port29_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOC
#define Port29_EXTI_PIN_SOURCE             EXTI_PinSource7
#define Port29_EXTI_IRQn                   EXTI9_5_IRQn
   
  
#define Port30_PIN                         GPIO_Pin_8
#define Port30_GPIO_PORT                   GPIOC
#define Port30_GPIO_CLK                    RCC_AHB1Periph_GPIOC
#define Port30_EXTI_LINE                   EXTI_Line8
#define Port30_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOC
#define Port30_EXTI_PIN_SOURCE             EXTI_PinSource8
#define Port30_EXTI_IRQn                   EXTI9_5_IRQn
   
  
#define Port31_PIN                         GPIO_Pin_9
#define Port31_GPIO_PORT                   GPIOC
#define Port31_GPIO_CLK                    RCC_AHB1Periph_GPIOC
#define Port31_EXTI_LINE                   EXTI_Line9
#define Port31_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOC
#define Port31_EXTI_PIN_SOURCE             EXTI_PinSource9
#define Port31_EXTI_IRQn                   EXTI9_5_IRQn
   
//------------------------------------------------------------------------------   
//#define PWM_PIN                         GPIO_Pin_6
//#define PWM_GPIO_PORT                   GPIOA
//#define PWM_GPIO_CLK                    RCC_AHB1Periph_GPIOA
//#define PWM_EXTI_LINE                   EXTI_Line4
//#define Port4_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
//#define Port4_EXTI_PIN_SOURCE             EXTI_PinSource4
//#define Port4_EXTI_IRQn                   EXTI4_IRQn 
   
//------------------------------------------------------------------------------
   
void LEDInit(Led_TypeDef Led);
void LEDOn(Led_TypeDef Led);
void LEDOff(Led_TypeDef Led);
void LEDToggle(Led_TypeDef Led);

void LED_alarm_Init(void);
void LED_warning_Init(void);
void LED_status_Init(void);

void LED_alarm_On(void);
void LED_warning_On(void);
void LED_status_On(void);

void LED_alarm_Off(void);
void LED_warning_Off(void);
void LED_status_Off(void);

void LED_alarm_Toggle(void);
void LED_warning_Toggle(void);
void LED_status_Toggle(void);

void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct); 


uint8_t OW_Send(uint8_t sendReset, uint8_t *command, uint8_t cLen, uint8_t *data, uint8_t dLen, uint8_t readStart);
uint8_t OW_Scan(uint8_t *buf, uint8_t num);

#endif /* __INIT_CONST_H */