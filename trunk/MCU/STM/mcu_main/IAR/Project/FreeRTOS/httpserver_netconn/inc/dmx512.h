/*
 * dmx512.h
 *
 *  Version 1.0.0
 */
//------------------------------------------------------------------------------
#ifndef DMX512_H_
#define DMX512_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "stm32f2xx.h"
#include "croutine.h"
#include "stm32f2xx_dma.h"
#include "main.h"

//---- Конфигурация USART2 для DMX512 ------------------------------------------

#define USART_DMX512                          USART2  
#define USART_DMX512_CLK                      RCC_APB1Periph_USART2 
#define USART_DMX512_AF                       GPIO_AF_USART2

#define USART2_CLK_INIT                       RCC_APB1PeriphClockCmd
/*
*  USART2 назначен на порт D  
* TX - 5
* RX - 6   
*/
#define USART_DMX512_TX_PIN                   GPIO_Pin_5                
#define USART_DMX512_TX_GPIO_CLK              RCC_AHB1Periph_GPIOD
#define USART_DMX512_TX_SOURCE                GPIO_PinSource5
#define USART_DMX512_TX_PORT                  GPIOD      
#define USART_DMX512_TX_AF                    GPIO_AF_USART2                 
 //-----------------------------------------------------------------------------
//---- Конфигурация DMA1 для USART2   ------------------------------------------
#define USART_DMX512_DMA                       DMA1
#define USART_DMX512_DMA_CLK                   RCC_AHB1Periph_DMA1
   
#define USART_DMX512_TX_DMA_CHANNEL            DMA_Channel_4
#define USART_DMX512_TX_DMA_STREAM             DMA1_Stream6
   
#define USART_DMX512_TX_DMA_FLAG_FEIF          DMA_FLAG_FEIF6
#define USART_DMX512_TX_DMA_FLAG_DMEIF         DMA_FLAG_DMEIF6
#define USART_DMX512_TX_DMA_FLAG_TEIF          DMA_FLAG_TEIF6
#define USART_DMX512_TX_DMA_FLAG_HTIF          DMA_FLAG_HTIF6
#define USART_DMX512_TX_DMA_FLAG_TCIF          DMA_FLAG_TCIF6
#define USART_DMX512_TX_DMA_FLAG_TC            DMA_FLAG_TC6

//--- Описание структур --------------------------------------------------------
/**
* DMX512_devices - число устройств или адресов для DMX512 интерфейса.
* 0 - адрес всегда должен быть равен 0 - см описание интерфейса.
* адресация цстройств начинается с адреса 1
*/
#define DMX512_devices  513

struct dmx512_t{  //В этой структуре храниятся последние данные устройств DMX
  uint8_t data[DMX512_devices]; // массив данных значений DNX устройств
  uint8_t adr;          // адрес старого значения устройства DNX
  uint8_t smooth_step;  // шаг плавного изменения значения
  uint8_t smooth_time;  // время между ближайшами шагами изменения значений
  uint8_t direction;    // 1 - Увеличениеб 0 - уменьшение значения DMX устройства
  uint8_t flag_send;    // флаг 1- необходимо передать данные в DNX устройтво
                        // 0 - данные переданы   
};

//-- Описание функций ----------------------------------------------------------   
void init_dma_uart2(void);
void DMX512_SendData(void);   
void vSendDMX512 (void * dev);
void DMX512_Break();
void DMX_smoot_add (void);  
void DMX_smoot_minus (void);  


#endif /* DMX512_H_ */