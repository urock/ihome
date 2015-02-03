
#include "dmx512.h"
//----- Глобальные переменные --------------------------------------------------
DMA_InitTypeDef  DMA_InitStructureTX_DMX512;
struct dmx512_t dmx, dmx_old;   // dmx - храним текущее значение устройств DMX
                                // dmx_old - храним предыдущее значение устройств DMX
//extern xSemaphoreHandle xUSARTSemaphore;
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* Функция инициализации последовательного интерфейса USART2 - используется для DMX512
* и канала DMA на TX, в данном релизе не используется приема информации от DMX512 устройств, 
* только передача. 
*/
void init_dma_uart2() {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
       
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(USART_DMX512_DMA_CLK, ENABLE);
    RCC_APB1PeriphClockCmd(USART_DMX512_CLK, ENABLE);
      
    GPIO_InitStructure.GPIO_Pin = USART_DMX512_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
    GPIO_Init(USART_DMX512_TX_PORT, &GPIO_InitStructure); 
    
    GPIO_PinAFConfig(USART_DMX512_TX_PORT, USART_DMX512_TX_SOURCE, USART_DMX512_TX_AF);    
   
/*
* частота канала 250000 - определена стандартом
* передается 8 бит, 1 бит -(low)стартовый бит
* 2 бит (hight) - стоповые биты    
*/    
    USART_InitStructure.USART_BaudRate = 250000;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_2;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(USART_DMX512, &USART_InitStructure);

// Включаем USART bynthatqc    
    USART_Cmd(USART_DMX512, ENABLE);
    
// Определяем DMA для выбранного USART2    
    DMA_DeInit(USART_DMX512_TX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_DMX512_TX_DMA_STREAM) != DISABLE)  //Дожидаемся пока будет DeInit
    {
    }
    
    DMA_InitStructureTX_DMX512.DMA_Channel = USART_DMX512_TX_DMA_CHANNEL;
    DMA_InitStructureTX_DMX512.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Transmit
    DMA_InitStructureTX_DMX512.DMA_Memory0BaseAddr = (uint32_t) dmx.data;  //будем передавать данные из этого буфера
    DMA_InitStructureTX_DMX512.DMA_BufferSize = DMX512_devices;  // количество передаваемых данных
    DMA_InitStructureTX_DMX512.DMA_PeripheralBaseAddr = (uint32_t)&USART_DMX512->DR;
    DMA_InitStructureTX_DMX512.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructureTX_DMX512.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructureTX_DMX512.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructureTX_DMX512.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructureTX_DMX512.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructureTX_DMX512.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructureTX_DMX512.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructureTX_DMX512.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructureTX_DMX512.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructureTX_DMX512.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;       
}

/**-----------------------------------------------------------------------------
* @brief Согласно интерфейсу DMX512 в начале пакета необходимо сделать break 
* примерно на 100мкс задержку MaB более 44мкс. см. спецификацию 
*-------------------------------------------------------------------------------
*/
void DMX512_Break() {
	USART_InitTypeDef USART_InitStructure;        
//-- Делаем переход из состояния hight в состояние low примерно на 100 - 200мкс
//-- но не более 1с, для этого меняем настройки USART        
	USART_InitStructure.USART_BaudRate = 20000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_Init(USART_DMX512, &USART_InitStructure);

	USART_ClearFlag(USART_DMX512, USART_FLAG_TC); //Очищаем флаг передачи ТС - transfer comlete      
	USART_SendData(USART_DMX512, 0xC0);  //Посылаем break - сначала передается младший а потом старший разряды

  //@todo: установить счетчик чтобы небыло зависаний
	while (USART_GetFlagStatus(USART_DMX512, USART_FLAG_TC) == RESET) {  //Дожидаемся пока не отправим все данные из уарта
          
              #ifdef OW_GIVE_TICK_RTOS
		taskYIELD();
              #endif
	}
// ---- Возвращаем первоначальные настройки USART
      USART_InitStructure.USART_BaudRate = 250000;
      USART_InitStructure.USART_WordLength = USART_WordLength_8b;
      USART_InitStructure.USART_StopBits = USART_StopBits_2;
      USART_InitStructure.USART_Parity = USART_Parity_No;
      USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
      USART_InitStructure.USART_Mode = USART_Mode_Tx;
      USART_Init(USART_DMX512, &USART_InitStructure);  
}

//------------------------------------------------------------------------------
/**-----------------------------------------------------------------------------
* Посылаем данные в интерфейс USART по DMA
*
*
*/
void DMX512_SendData(void){  
  
    DMA_DeInit(USART_DMX512_TX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_DMX512_TX_DMA_STREAM) != DISABLE)   //Ожидаем окончания процедуры DeInit        
    {      
    }  
          
    DMA_Init(USART_DMX512_TX_DMA_STREAM, &DMA_InitStructureTX_DMX512);

    USART_ClearFlag(USART_DMX512, USART_FLAG_RXNE | USART_FLAG_TC | USART_FLAG_TXE);    //Обязательно нужно сбрасывать эти флаги
    USART_DMACmd(USART_DMX512, USART_DMAReq_Tx, ENABLE);                        // Запускаем USART с DMA
    DMA_ClearFlag(USART_DMX512_TX_DMA_STREAM, USART_DMX512_TX_DMA_FLAG_TCIF);  //Обязательно нужно сбрасывать эти флаги      
    DMA_Cmd(USART_DMX512_TX_DMA_STREAM, ENABLE);                               // Стартуем канал DMA
    
/*
* @todo:Сюда нужно вставить счетчик, чтобы программа не зависала
*/    
    while((DMA_GetFlagStatus(USART_DMX512_TX_DMA_STREAM, USART_DMX512_TX_DMA_FLAG_TCIF) == RESET));           
    
    while (USART_GetFlagStatus(USART_DMX512, USART_FLAG_TC) == RESET) //Дожидаемся пока послдний бит уйднт из UART
      {
      }    
    
    DMA_ClearFlag(USART_DMX512_TX_DMA_STREAM, USART_DMX512_TX_DMA_FLAG_HTIF | USART_DMX512_TX_DMA_FLAG_TCIF);    //Очищаем флаги ДМА    
    DMA_Cmd(USART_DMX512_TX_DMA_STREAM, DISABLE);        
    USART_DMACmd(USART_DMX512, USART_DMAReq_Tx, DISABLE);  // Конец работы для UART
}
//------------------------------------------------------------------------------

//------- Task Send DMX512 Data ------------------------------------------------
/**-----------------------------------------------------------------------------
* Задача в которой призводится обработка переданных данных с web интерфейса для
* устройст DMX512
* В этой функции нужно менять содержимое массива dmx_data и запускать функции
* DMX512_Break(); DMX512_SendData();   
*/
void vSendDMX512 (void * dev){
//-----Описание переменных -------------------  
//   uint8_t direction;  // Увеличивается или уменьшается значение у устройства DMX
//--------------------------------------------   
   init_dma_uart2();  // Инициализация USART2 и DMA   
   dmx.data[0] = 0;
   
   while(1){      
     if (dmx.flag_send == 1){
       if (dmx.smooth_step == 0){
        DMX512_Break();
        DMX512_SendData();                        
        dmx.flag_send = 0;
       }
       else {
         
        if (dmx.direction == 1) 
          DMX_smoot_add();               
        else
        DMX_smoot_minus ();
       }
       dmx.flag_send = 0;
       dmx_old = dmx;  // Сохранили значения устройств DMX
       
//      msg_dbg_tsk(("DMX512: %d \n\r", dmx.data[1]));      
     }  
     vTaskDelay(1000);     
   }    
}
//------------------------------------------------------------------------------
/*
* Плавное уведичение яркости
*
*/
void DMX_smoot_add (void){  
  uint8_t val;
  val = dmx.data[dmx.adr];
      while (dmx_old.data[dmx.adr] < val){
// -- Не даем перейти через 255                
        if (dmx_old.data[dmx.adr] + dmx.smooth_step <= 255){
          dmx_old.data[dmx.adr] = dmx_old.data[dmx.adr] + dmx.smooth_step;
        }
        else{
          dmx_old.data[dmx.adr] = 255;
        }        
// -- Не даем перейти через заданное значение                
        if (dmx_old.data[dmx.adr] > val){
          dmx.data[dmx.adr] = val;
          dmx_old.data[dmx.adr] = val;
        }
        else{
        dmx.data[dmx.adr] = dmx_old.data[dmx.adr];
        }
        DMX512_Break();
        DMX512_SendData();                    
      vTaskDelay(dmx.smooth_time);        
  }  
}

//------------------------------------------------------------------------------
/*
*  Плавное уменьшение яркости
*
*/
void DMX_smoot_minus (void){
  uint8_t val;
  val = dmx.data[dmx.adr];
      while (dmx_old.data[dmx.adr] > val){
// -- Не даем перейти через 0        
        if (dmx_old.data[dmx.adr] - dmx.smooth_step >= 0){
          dmx_old.data[dmx.adr] = dmx_old.data[dmx.adr] - dmx.smooth_step;
        }
        else{
          dmx_old.data[dmx.adr] = 0;
        }
// -- Не даем перейти через заданное значение        
        if (dmx_old.data[dmx.adr] < val){
          dmx.data[dmx.adr] = val;
          dmx_old.data[dmx.adr] = val;
        }
        else{
        dmx.data[dmx.adr] = dmx_old.data[dmx.adr];
        }
        DMX512_Break();
        DMX512_SendData();                    
        vTaskDelay(dmx.smooth_time); 
   }
}  
