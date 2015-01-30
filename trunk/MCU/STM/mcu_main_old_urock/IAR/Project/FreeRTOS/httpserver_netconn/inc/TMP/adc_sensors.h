#ifndef ADC_SENSORS_H_
#define ADC_SENSORS_H_

#include "stm32f2xx.h"
#include "init_const.h"

//------------------------------------------------------------------------------
#define ADC_PORTn                             10
//------------------------------------------------------------------------------
typedef enum 
{
  ADC_Port0 = 0,
  ADC_Port1 = 1,
  ADC_Port2 = 2,
  ADC_Port3 = 3,
  ADC_Port4 = 4,
  ADC_Port5 = 5,
  ADC_Port6 = 6,
  ADC_Port7 = 7,
  ADC_Port8 = 8,
  ADC_Port9 = 9,
  
  ADC_Port100 = 100   // error port
} ADC_Ports_TypeDef;
    
typedef enum 
{
  GAS = 0,              // Датчик газа
  WATER = 1,            // Датчик протечки воды   
  LIGTH = 2,            // Датчик света
  TEMPERATURE = 3,      // Датчик теапературы
  PRESSURE = 4          // Датчик давления
} Sensors_TypeDef;


   
//--------- ADC PORTS ----------------------------------------------------------
#define ADC0_PIN                         GPIO_Pin_0
#define ADC0_GPIO_PORT                   GPIOA
#define ADC0_GPIO_CLK                    RCC_AHB1Periph_GPIOA
//#define ADC0_EXTI_LINE                   EXTI_Line0
//#define ADC0_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOA
//#define ADC0_EXTI_PIN_SOURCE             EXTI_PinSource0
//#define ADC0_EXTI_IRQn                   EXTI0_IRQn
      
#define ADC3_PIN                         GPIO_Pin_3
#define ADC3_GPIO_PORT                   GPIOA
#define ADC3_GPIO_CLK                    RCC_AHB1Periph_GPIOA
//#define ADC3_EXTI_LINE                   EXTI_Line3
//#define ADC3_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOA
//#define ADC3_EXTI_PIN_SOURCE             EXTI_PinSource3
//#define ADC3_EXTI_IRQn                   EXTI3_IRQn
   
#define ADC4_PIN                         GPIO_Pin_4
#define ADC4_GPIO_PORT                   GPIOA
#define ADC4_GPIO_CLK                    RCC_AHB1Periph_GPIOA
//#define ADC4_EXTI_LINE                   EXTI_Line4
//#define ADC4_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOA
//#define ADC4_EXTI_PIN_SOURCE             EXTI_PinSource4
//#define ADC4_EXTI_IRQn                   EXTI4_IRQn
   
#define ADC5_PIN                         GPIO_Pin_5
#define ADC5_GPIO_PORT                   GPIOA
#define ADC5_GPIO_CLK                    RCC_AHB1Periph_GPIOA
//#define ADC5_EXTI_LINE                   EXTI_Line5
//#define ADC5_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOA
//#define ADC5_EXTI_PIN_SOURCE             EXTI_PinSource5
//#define ADC5_EXTI_IRQn                   EXTI9_5_IRQn
   
#define ADC6_PIN                         GPIO_Pin_6
#define ADC6_GPIO_PORT                   GPIOA
#define ADC6_GPIO_CLK                    RCC_AHB1Periph_GPIOA
//#define ADC6_EXTI_LINE                   EXTI_Line6
//#define ADC6_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOA
//#define ADC6_EXTI_PIN_SOURCE             EXTI_PinSource6
//#define ADC6_EXTI_IRQn                   EXTI9_5_IRQn
   
#define ADC8_PIN                         GPIO_Pin_0
#define ADC8_GPIO_PORT                   GPIOB
#define ADC8_GPIO_CLK                    RCC_AHB1Periph_GPIOB
//#define ADC8_EXTI_LINE                   EXTI_Line0
//#define ADC8_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
//#define ADC8_EXTI_PIN_SOURCE             EXTI_PinSource0
//#define ADC8_EXTI_IRQn                   EXTI0_IRQn
   
#define ADC9_PIN                         GPIO_Pin_1
#define ADC9_GPIO_PORT                   GPIOB
#define ADC9_GPIO_CLK                    RCC_AHB1Periph_GPIOB
//#define ADC9_EXTI_LINE                   EXTI_Line1
//#define ADC9_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOB
//#define ADC9_EXTI_PIN_SOURCE             EXTI_PinSource1
//#define ADC9_EXTI_IRQn                   EXTI1_IRQn
   
#define ADC10_PIN                         GPIO_Pin_0
#define ADC10_GPIO_PORT                   GPIOC
#define ADC10_GPIO_CLK                    RCC_AHB1Periph_GPIOC
//#define ADC10_EXTI_LINE                   EXTI_Line0
//#define ADC10_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOC
//#define ADC10_EXTI_PIN_SOURCE             EXTI_PinSource0
//#define ADC10_EXTI_IRQn                   EXTI0_IRQn

#define ADC12_PIN                         GPIO_Pin_2
#define ADC12_GPIO_PORT                   GPIOC
#define ADC12_GPIO_CLK                    RCC_AHB1Periph_GPIOC
//#define ADC12_EXTI_LINE                   EXTI_Line2
//#define ADC12_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOC
//#define ADC12_EXTI_PIN_SOURCE             EXTI_PinSource2
//#define ADC12_EXTI_IRQn                   EXTI2_IRQn

#define ADC13_PIN                         GPIO_Pin_3
#define ADC13_GPIO_PORT                   GPIOC
#define ADC13_GPIO_CLK                    RCC_AHB1Periph_GPIOC
//#define ADC13_EXTI_LINE                   EXTI_Line3
//#define ADC13_EXTI_PORT_SOURCE            EXTI_PortSourceGPIOC
//#define ADC13_EXTI_PIN_SOURCE             EXTI_PinSource3
//#define ADC13_EXTI_IRQn                   EXTI3_IRQn


//---- Конфигурация ADC ------------------------------------------
#define ADCx                     ADC1   
#define ADCx_CLK                 RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOС
   
#define ADC_CH                   10  //Количество используемых каналов DMA
#define DMA_ADC_STREAM           DMA2_Stream0 
#define DMA_ADC_CHANNEL          DMA_Channel_0

#define ADC_CHANNEL_0            ADC_Channel_0
#define ADC_CHANNEL_3            ADC_Channel_3
#define ADC_CHANNEL_4            ADC_Channel_4
#define ADC_CHANNEL_5            ADC_Channel_5
#define ADC_CHANNEL_6            ADC_Channel_6
#define ADC_CHANNEL_8            ADC_Channel_8
#define ADC_CHANNEL_9            ADC_Channel_9
#define ADC_CHANNEL_10           ADC_Channel_10
#define ADC_CHANNEL_12           ADC_Channel_12
#define ADC_CHANNEL_13           ADC_Channel_13


#define ADC_GPIO_PORT_A     GPIOA  
#define ADC_GPIO_PORT_B     GPIOB  
#define ADC_GPIO_PORT_C     GPIOC
   
#define ADCx_PIN_0         GPIO_Pin_0
#define ADCx_PIN_3         GPIO_Pin_3
#define ADCx_PIN_4         GPIO_Pin_4
#define ADCx_PIN_5         GPIO_Pin_5
#define ADCx_PIN_6         GPIO_Pin_6
#define ADCx_PIN_8         GPIO_Pin_8
#define ADCx_PIN_9         GPIO_Pin_9
#define ADCx_PIN_10        GPIO_Pin_10
#define ADCx_PIN_12        GPIO_Pin_12
#define ADCx_PIN_13        GPIO_Pin_13   
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
/**
* @brief Structure ADC_Ports  (ADC_PORT_struct)
* @param num_port, min_value, max_value, enable, key
*/
  struct ADC_PORT_struct {
      uint8_t num_port ;
      int min_value ;  
      int max_value ;  
      int read_value;  // Считанное значение
      uint8_t key ;    // key for other port
      PortEnable_TypeDef enable ; // enabled port
      uint8_t name; // Имя датчика (wather, GAS, ligth e t.c.)
  };
//------------------------------------------------------------------------------
void adc_sensors_Init(void);
#endif /* ADC_SENSORS_H_ */