/*
 * onewire.c
 *
 *  Version 1.0.3
 */

#include "onewire.h"

DMA_InitTypeDef  DMA_InitStructureTX_1WIRE, DMA_InitStructureRX_1WIRE;
//----Внешние переменные -------------------------------------------------------
extern xTimerHandle	xOneWireSendTimer;
extern uint8_t send_timer;
extern xSemaphoreHandle xUSARTSemaphore;
//extern xQueueHandle tcp_tempsensor_queue;
//------------------------------------------------------------------------------

//---Переменные ----------------------------------------------------------------
// Буфер для приема/передачи по 1-wire
uint8_t ow_buf[8];

//#define OW_0	0x00
//#define OW_1	0xff
//#define OW_R_1	0xff
//------------------------------------------------------------------------------

//----- Описание функций -------------------------------------------------------
/**-----------------------------------------------------------------------------
* @brief инициализирует USART6 и DMA2 для шины 1-wire
* @return void
*-----------------------------------------------------------------------------
*/
void OW_Init(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
       
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(USART_1WIRE_DMA_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(USART_1WIRE_CLK, ENABLE);
       
    GPIO_InitStructure.GPIO_Pin = USART_1WIRE_TX_PIN | USART_1WIRE_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
// Выводы сделал открытый коллектор    
     GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(USART_1WIRE_TX_PORT, &GPIO_InitStructure); 
    
    GPIO_PinAFConfig(USART_1WIRE_TX_PORT, USART_1WIRE_TX_SOURCE, USART_1WIRE_TX_AF);    
    GPIO_PinAFConfig(USART_1WIRE_RX_PORT, USART_1WIRE_RX_SOURCE, USART_1WIRE_RX_AF);
//--- Устанавливаем скорость передачи 115200 см спецификацию   
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART_1WIRE, &USART_InitStructure);
    
//--- Запускаем USART6    
    USART_Cmd(USART_1WIRE, ENABLE);
    
//---- Настраиваем DMA для передатчика -----------------------------------------    
    DMA_DeInit(USART_1WIRE_TX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_1WIRE_TX_DMA_STREAM) != DISABLE)  //Дожидаемся пока будет DeInit
    {
    }
    
    DMA_InitStructureTX_1WIRE.DMA_Channel = USART_1WIRE_TX_DMA_CHANNEL;
    DMA_InitStructureTX_1WIRE.DMA_DIR = DMA_DIR_MemoryToPeripheral; // Transmit
    DMA_InitStructureTX_1WIRE.DMA_Memory0BaseAddr = (uint32_t) ow_buf;  // буфер для хранения данных о 1-wire устройствах
    DMA_InitStructureTX_1WIRE.DMA_PeripheralBaseAddr = (uint32_t) &(USART_1WIRE->DR);
    DMA_InitStructureTX_1WIRE.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructureTX_1WIRE.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructureTX_1WIRE.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructureTX_1WIRE.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructureTX_1WIRE.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructureTX_1WIRE.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructureTX_1WIRE.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructureTX_1WIRE.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructureTX_1WIRE.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructureTX_1WIRE.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
        
//---- Настраиваем DMA для приемника -------------------------------------------
    DMA_DeInit(USART_1WIRE_RX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_1WIRE_RX_DMA_STREAM) != DISABLE) //Дожидаемся пока будет DeInit
    {
    }    
    DMA_InitStructureRX_1WIRE.DMA_Channel = USART_1WIRE_RX_DMA_CHANNEL;
    DMA_InitStructureRX_1WIRE.DMA_DIR = DMA_DIR_PeripheralToMemory; // Receive
    DMA_InitStructureRX_1WIRE.DMA_Memory0BaseAddr = (uint32_t) ow_buf;  // буфер для хранения данных о 1-wire устройствах
    DMA_InitStructureRX_1WIRE.DMA_PeripheralBaseAddr = (uint32_t) &(USART_1WIRE->DR);
    DMA_InitStructureRX_1WIRE.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructureRX_1WIRE.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructureRX_1WIRE.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructureRX_1WIRE.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructureRX_1WIRE.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructureRX_1WIRE.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructureRX_1WIRE.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructureRX_1WIRE.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructureRX_1WIRE.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructureRX_1WIRE.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
 }
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief функция преобразует один байт в восемь, для передачи через USART
* @param ow_byte - байт, который надо преобразовать
* @param ow_bits - ссылка на буфер, размером не менее 8 байт
* r@eturn void
* -----------------------------------------------------------------------------
*/
void OW_toBits(uint8_t ow_byte, uint8_t *ow_bits) {
	uint8_t i;
	for (i = 0; i < 8; i++) {
		if (ow_byte & 0x01) {
			*ow_bits = OW_1;
		} else {
			*ow_bits = OW_0;
		}
		ow_bits++;
		ow_byte = ow_byte >> 1;
	}
}
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief обратное преобразование - из того, что получено через USART опять собирается байт
* @param ow_bits - ссылка на буфер, размером не менее 8 байт
* @return void
*-----------------------------------------------------------------------------
*/
uint8_t OW_toByte(uint8_t *ow_bits) {
	uint8_t ow_byte, i;
	ow_byte = 0;
	for (i = 0; i < 8; i++) {
		ow_byte = ow_byte >> 1;
		if (*ow_bits == OW_R_1) {
			ow_byte |= 0x80;
		}
		ow_bits++;
	}

	return ow_byte;
}
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief осуществляет сброс и проверку на наличие устройств на шине 1- wire
* @return ow_ok или устройство не найдено
*-----------------------------------------------------------------------------
*/
uint8_t OW_Reset() {
	uint8_t ow_presence;
	USART_InitTypeDef USART_InitStructure;
//----Устанавливаем скорость USART для для заданной длины импульса reset
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART_1WIRE, &USART_InitStructure);

	// отправляем 0xf0 на скорости 9600 
	USART_ClearFlag(USART_1WIRE, USART_FLAG_TC); //Очищаем флаг передачи ТС - transfer comlete
	USART_SendData(USART_1WIRE, 0xf0);  //Посылаем reset (0f) в шину 1-wire - сначала передается младший, а потом старший разряды

	while (USART_GetFlagStatus(USART_1WIRE, USART_FLAG_TC) == RESET) {  //Дожидаемся пока не отправим все данные из уарта
          
            #ifdef OW_GIVE_TICK_RTOS
		taskYIELD();
            #endif
	}

	ow_presence = USART_ReceiveData(USART_1WIRE); //После reset устройства на шине должны опустить сигнал в 0
        
//--- Востанавливаем первоначальные установки USART6
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_Init(USART_1WIRE, &USART_InitStructure);
//--- Проверка на присутствие устройств 1-wire на шине
	if (ow_presence != 0xf0) { // Если устройство откликнулось
		return OW_OK;
	}

	return OW_NO_DEVICE;  //Устройств не найдено
}
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------
* @brief внутренняя процедура. Передает указанное число бит по интерфейсу 1-wire
* @param nbits - число бит которое нужно передать по интерфейсу 1-wire
* @return void
*------------------------------------------------------------------------
*/
void OW_SendBits(uint8_t nbits){
    portBASE_TYPE xStatus;
  
    DMA_DeInit(USART_1WIRE_TX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_1WIRE_TX_DMA_STREAM) != DISABLE)   //Ожидаем окончания процедуры DeInit        
    {      
    }
    
    DMA_DeInit(USART_1WIRE_RX_DMA_STREAM);
    while (DMA_GetCmdStatus(USART_1WIRE_RX_DMA_STREAM) != DISABLE)   //Ожидаем окончания процедуры DeInit        
    {      
    }
    
    DMA_InitStructureTX_1WIRE.DMA_BufferSize = nbits;
    DMA_InitStructureRX_1WIRE.DMA_BufferSize = nbits;
    DMA_Init(USART_1WIRE_TX_DMA_STREAM, &DMA_InitStructureTX_1WIRE);
    DMA_Init(USART_1WIRE_RX_DMA_STREAM, &DMA_InitStructureRX_1WIRE);

    USART_ClearFlag(USART_1WIRE, USART_FLAG_RXNE | USART_FLAG_TC | USART_FLAG_TXE);    //Обязательно нужно сбрасывать эти флаги
    USART_DMACmd(USART_1WIRE, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
    
    DMA_ClearFlag(USART_1WIRE_TX_DMA_STREAM, USART_1WIRE_TX_DMA_FLAG_TCIF);  //Обязательно нужно сбрасывать эти флаги  
    DMA_ClearFlag(USART_1WIRE_RX_DMA_STREAM, USART_1WIRE_RX_DMA_FLAG_TCIF);  
    
    DMA_Cmd(USART_1WIRE_TX_DMA_STREAM, ENABLE);
    DMA_Cmd(USART_1WIRE_RX_DMA_STREAM, ENABLE);


    //Взводим таймер, если таймер доходит до 0, то ошибка при передачи и выходим из цикла, чтобы не зависла программа, передаем ошибку на ХОСТ
    xOneWireSendTimer = xTimerCreate("OneWireSendTimer", 1000/portTICK_RATE_MS, pdFALSE, 0, vOneWireSendTimerFunction);
   /* Вьшолнить сброс только первого интервального таймера. * Именно он начнет отсчитывать время сразу после запуска планировщика * Остальные 2 таймера после запуска планировщика останутся в пассивном состоянии. */
    xTimerReset(xOneWireSendTimer, 0);
    send_timer = 0;
       
    while((DMA_GetFlagStatus(USART_1WIRE_TX_DMA_STREAM, USART_1WIRE_TX_DMA_FLAG_TCIF) == RESET)&&(DMA_GetFlagStatus(USART_1WIRE_RX_DMA_STREAM, USART_1WIRE_RX_DMA_FLAG_TCIF) == RESET)&&(send_timer == 0));

    //    Без таймера программа может зависнуть в этом месте
    while ((USART_GetFlagStatus(USART_1WIRE, USART_FLAG_TC) == RESET)&&(send_timer == 0))
      {
      }
    //--- Останавливаем и удаляем таймер ---------
//    xStatus = xTimerStop(xOneWireSendTimer,100);
//    if (send_timer == 1){
//    	msg_dbg(("Error in recieve data on 1-wire bus \n"));
//    }
//    xStatus = xTimerDelete(xOneWireSendTimer, 100);
    
    DMA_ClearFlag(USART_1WIRE_TX_DMA_STREAM, USART_1WIRE_TX_DMA_FLAG_HTIF | USART_1WIRE_TX_DMA_FLAG_TCIF);    //Очищаем флаги ДМА
    DMA_ClearFlag(USART_1WIRE_RX_DMA_STREAM, USART_1WIRE_RX_DMA_FLAG_HTIF | USART_1WIRE_RX_DMA_FLAG_TCIF);    //Очищаем флаги ДМА    
    DMA_Cmd(USART_1WIRE_TX_DMA_STREAM, DISABLE);
    DMA_Cmd(USART_1WIRE_RX_DMA_STREAM, DISABLE);
    
    USART_DMACmd(USART_1WIRE, USART_DMAReq_Tx | USART_DMAReq_Rx, DISABLE);  // Конец работы для UART
}
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief процедура общения с шиной 1-wire
* @param sendReset - посылать RESET в начале общения.
* 		OW_SEND_RESET или OW_NO_RESET
* command - массив байт, отсылаемых в шину. Если нужно чтение - отправляем OW_READ_SLOT
* cLen - длина буфера команд, столько байт отошлется в шину
* data - если требуется чтение, то ссылка на буфер для чтения
* dLen - длина буфера для чтения. Прочитается не более этой длины
* readStart - с какого символа передачи начинать чтение (нумеруются с 0)
*		можно указать OW_NO_READ, тогда можно не задавать data и dLen
* @return удвчно или не удачно отослали массив байт
*!ВНИМАНИЕ readStart зависит от посылаемой команды, т.к. ТХ связан с RX то что передаю (клманды), то и принимаю - вот это мы пропускаем и считываем только данные
* ----------------------------------------------------------------------------
*/
uint8_t OW_Send(uint8_t sendReset, uint8_t *command, uint8_t cLen,
		uint8_t *data, uint8_t dLen, uint8_t readStart) {

	// если требуется сброс - сбрасываем и проверяем на наличие устройств
	if (sendReset == OW_SEND_RESET) {
		if (OW_Reset() == OW_NO_DEVICE) {
			return OW_NO_DEVICE;
		}
	}

	while (cLen > 0) {

		OW_toBits(*command, ow_buf);
		command++;
		cLen--;

		OW_SendBits(8);

		// если прочитанные данные кому-то нужны - выкинем их в буфер
		if (readStart == 0 && dLen > 0) {
			*data = OW_toByte(ow_buf);
			data++;
			dLen--;
		} else {
			if (readStart != OW_NO_READ) {
				readStart--;
			}
		}
	}

	return OW_OK;
}
//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief Данная функция осуществляет сканирование сети 1-wire и записывает найденные
* @param  ID устройств в массив buf, по 8 байт на каждое устройство.
* переменная num ограничивает количество находимых устройств, чтобы не переполнить
* буфер.
* @return - количество найденых устройст на шине 1-wire
-----------------------------------------------------------------------------*/
uint8_t OW_Scan(uint8_t *buf, uint8_t num) {

        uint8_t found = 0;
        uint8_t *lastDevice;
        uint8_t *curDevice = buf;
        uint8_t numBit, lastCollision, currentCollision, currentSelection;

        lastCollision = 0;
        while (found < num) {
                numBit = 1;
                currentCollision = 0;

                // посылаем команду на поиск устройств
                OW_Send(OW_SEND_RESET, (uint8_t*)"\xf0", 1, 0, 0, OW_NO_READ);

                for (numBit = 1; numBit <= 64; numBit++) {
                        // читаем два бита. Основной и комплементарный
                        OW_toBits(OW_READ_SLOT, ow_buf);
                        OW_SendBits(2);

                        if (ow_buf[0] == OW_R_1) {
                                if (ow_buf[1] == OW_R_1) {
                                        // две единицы, где-то провтыкали и заканчиваем поиск
                                        return found;
                                } else {
                                        // 10 - на данном этапе только 1
                                        currentSelection = 1;
                                }
                        } else {
                                if (ow_buf[1] == OW_R_1) {
                                        // 01 - на данном этапе только 0
                                        currentSelection = 0;
                                } else {
                                        // 00 - коллизия
                                        if (numBit < lastCollision) {
                                                // идем по дереву, не дошли до развилки
                                                if (lastDevice[(numBit - 1) >> 3]
                                                                & 1 << ((numBit - 1) & 0x07)) {
                                                        currentSelection = 1;

                                                        // если пошли по правой ветке, запоминаем номер бита
                                                        if (currentCollision < numBit) {
                                                                currentCollision = numBit;
                                                        }
                                                } else {
                                                        currentSelection = 0;
                                                }
                                        } else {
                                                if (numBit == lastCollision) {
                                                        currentSelection = 0;
                                                } else {
                                                        // идем по правой ветке
                                                        currentSelection = 1;

                                                        // если пошли по правой ветке, запоминаем номер бита
                                                        if (currentCollision < numBit) {
                                                                currentCollision = numBit;
                                                        }
                                                }
                                        }
                                }
                        }

                        if (currentSelection == 1) {
                                curDevice[(numBit - 1) >> 3] |= 1 << ((numBit - 1) & 0x07);
                                OW_toBits(0x01, ow_buf);
                        } else {
                                curDevice[(numBit - 1) >> 3] &= ~(1 << ((numBit - 1) & 0x07));
                                OW_toBits(0x00, ow_buf);
                        }
                        OW_SendBits(1);
                }
                found++;
                lastDevice = curDevice;
                curDevice += 8;
                if (currentCollision == 0)
                        return found;

                lastCollision = currentCollision;
        }

        return found;
}

//------------------------------------------------------------------------------

/**-----------------------------------------------------------------------------
* @brief Чтение температуры с датчиков.DS18b20+ и передача ее на ХОСТ по ETH
* @param
* @return void
* !ВНИМАНИЕ в функции OW_Send параметр readStart зависит от посылаемой команды, т.к. ТХ связан с RX то что передаю (клманды), то и принимаю - вот это мы пропускаем и считываем только данные
*/
void vGetTemperature (void * dev){
//-------- Переменные -----------------------------------------   
  struct owidbuf_t       owidbuf[MAX_DS18]; // Переменная где хранится информация о всех датчиках ID, Temperature
  temp_status_t          temp_status[MAX_DS18];
  
  uint8_t buf[9]; //Буфер для чтения данных из датчика
  uint8_t idbuf[MAX_DS18][8]; //Массив где хранятся ID датчиков
  uint8_t i;  // Переменная для цикла
  uint8_t j;  // Переменная для цикла
  uint8_t found; // кол-во найденых устройств 1-wire
  signed char integer_temp = 0; //Температура целая часть    
  signed char frac_temp;        //Температура дробная часть

  portBASE_TYPE xStatus;
//--------------------------------------------------------------
  
  OW_Init();  //Инициализация шины 1-wire
  found = OW_Scan((uint8_t *)idbuf, MAX_DS18); // Ищем устройства на шине 1-wire
// Выводим кол-во обнаруж датчиков  
  msg_dbg_tsk(("Found 1-wire device: %d \n\r",found));

//---------Посылаем ID найденых датчиков на ХОСТ ------------------------------
  for(j = 0; j < found; j++ ){
      for(i = 0; i < 8; i++){
        temp_status[j].id[i] = idbuf[j][i];
        msg_dbg_tsk(("%2x:",idbuf[j][i]));
      }
    msg_dbg_tsk(("\n\r"));  //выводим на терминал
    temp_status[j].mode = ADDDEV_1WIRE;
    if (temp_write_output_queue(&temp_status[j]) != pdTRUE) {
       msg_dbg_tsk(("Error in power_control_thread: can not write to output queue\n\r"));
    }     
  }
  
//-----Подготовка массива данных ----------------------------------------------  

  for (j = 0; j < found; j++){  //Создаем массив для доступа к каждому устройству на шине по его ID
    owidbuf[j].id[0] = 0x55;  //Команда обращения по ID на шине 1-wire
    for(i = 0; i < 8; i++){
      owidbuf[j].id[i+1] = idbuf[j][i];  //Записываем в массив ID
    }
    owidbuf[j].id[9] = 0xbe;  //Продолжаем дописывать массив - команда чтения всех регистров датчика DS18B20
    
    for (i=10; i < 15; i++){
       owidbuf[j].id[i] = 0xff;  //Пишем 5 раз ff в массив что бы потом можно было считать 5 байт из датчика
       //Для того чтобы считать байт нужно послать байт ff
    }    
  }
//---- Измерение температуры с датчиков температуры --------------------------  
  for (;;){  // В цикле измеряем температуру со всех датчиков 
/*----------------------------------------------------------------------------*/    
//#ifdef Debug_my
//  taskENTER_CRITICAL();
//    size_t mem = xPortGetFreeHeapSize();
//    printf ("Free Heap size in temperature Task = %d \n\n", mem);
//  taskEXIT_CRITICAL();
//#endif
/*----------------------------------------------------------------------------*/
  
    OW_Send(OW_SEND_RESET, "\xcc\x44", 2, NULL, NULL, OW_NO_READ); // Измеряем температуру на всех датчиках
    vTaskDelay(1000);  // Ждем около  1 сек пока закончится процесс измерения температуры !!!ОБЯЗАТЕЛЬНО
    
    for (j = 0; j < found; j++){ // Считываем температуру для каждого датчика
      //Посылаем 15 байт, считываем 13 байт, но в буфер кладем, только с десятого, т.е. в буфере будет 3 байта
    OW_Send(OW_SEND_RESET, owidbuf[j].id, 15, buf, 13, 10); // Считываем температуру  
    
/****************************************************************/ 

      frac_temp    = buf[0] & 0x0f;                            //получить дробную часть      
      integer_temp = (buf[0]>>4) | ((buf[1] & 0x0f)<<4);      //получить целую часть 
      
//-------- Запоминаем температуру для передачи ее на ХОСТ --------------
        temp_status[j].int_path = integer_temp;      
        temp_status[j].float_path = ((frac_temp * 10)/16);      
        for(i = 0; i < 8; i++){  // Записываем ID датчика в структуру
          temp_status[j].id[i] = idbuf[j][i];
        }        
//----------------------------------------------------------------------
        
      //если температура отрицательная
      if(integer_temp < 0)
      {
        integer_temp = 0 - integer_temp - 1;
        owidbuf[j].temp  = integer_temp * 10;                            //учитываем целую часть
        frac_temp = frac_temp | 0xf0;
        frac_temp = 0 - frac_temp ;                                 
      }
      //если температура положительная
      else {
        owidbuf[j].temp  = integer_temp * 10;                    //учитываем целую часть        
      }

      owidbuf[j].temp =  owidbuf[j].temp + ((frac_temp * 10)/16);                  //учитываем дробную часть //Чтобы получить реальную температуру, нужно это число разделить на 10.
//      printf("Temperature %d\n\n", owidbuf[j].temp);
      
//------------------------------------------------------------------------------
//---- Отправляем температуру и ID датчика на ХОСТ ----------------------------- 
      msg_dbg_tsk(("Tem  1-wire device: %d \n\r", owidbuf[j].temp));      
      msg_dbg_tsk(("Tem  Int path: %d \n\r", temp_status[j].int_path));      
      msg_dbg_tsk(("Tem  float path: %d \n\r", temp_status[j].float_path)); 
      
      temp_status[j].mode = SENDTEMP_1WIRE;  // Посылаем температуру на ХОСТ
      if (temp_write_output_queue(&temp_status[j]) != pdTRUE) {
        msg_dbg_tsk(("Error in power_control_thread: can not write to output queue\n\r"));
      } 
      
//------------------------------------------------------------------------------      
      
      
    } //end for перебор датчиков температуры
//**********************************************************************/
    // Задержка в 5 минут - каждая строка по минуте
  vTaskDelay(request_1wire);
  }  
}  

