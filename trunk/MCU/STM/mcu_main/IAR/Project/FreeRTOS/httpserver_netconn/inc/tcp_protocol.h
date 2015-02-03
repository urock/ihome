#ifndef __TCP_PROTOCOL_H
#define __TCP_PROTOCOL_H

#define MAX_TCP_MSG_SIZE	128

/*********
 *
 * Формат сообщения:
 * 					байты
 * CommandID		1
 * CommandPAR		1
 * DataLEN			2		(MSB первый LSB второй)
 * Data				n
 * CRC8						algorithm?
 *
 * Сообщения могут быть инициированы как хостом, так платой МК.
 * Все сообщения предполагают ответ в рамках открытого TCP соединения.
 *
 * Сообщения, связанные с платами Реле:
 * Хост -> МК
 *
 * 1. GET RELAY STATE
 * 1.1 Хост генерирует сообщение:
 *
 * 		CID 	= CID_RELAY 			Сообщения, связанные с платой реле
 * 		CPAR 	= GET_RSTATE_REQ 			Хост читает состояние
 * 		LEN 	= n 			n - количество плат, состояние которых надо вернуть
 * 		DAT 	= BoardIndex_i, BoardIndex_i+1 ...  - номера плат
 * 		CRC8
 *
 * МК, приняв это сообщение, должен ответить следующим сообщением, причем данные о состоянии
 * можно взять из глобального массива
 * 		int current_relay_state[NUM_RELAY_BOARDS];  (пока он локальный для потока relay_thread)
 *
 * 1.2 МК отвечает

 * 		CID 	= CID_RELAY 			Сообщения, связанные с платой реле
 * 		CPAR 	= GET_RSTATE_ANS 			МК отвечает на запрос чтения состояния
 * 		LEN 	= 2*n 			n - количество плат, состояние которых надо вернуть
 * 		DAT 	= {BoardIndex, RelayState}
 * 		CRC8
 *
 * 2. SET RELAY STATE
 * 2.1 Хост генерирует сообщение:
 *
 * 		CID 	= CID_RELAY 			Сообщения, связанные с платой реле
 * 		CPAR 	= SET_RSTATE_REQ 			Хост устанавливает состояние
 * 		LEN 	= 2*n 			n - количество плат, состояние которых надо установить
 * 		DAT 	= {BoardIndex, RelayState}
 * 		CRC8
 *
 * 2.2 МК отвечает
 * 		CID 	= CID_RELAY 			Сообщения, связанные с платой реле
 * 		CPAR 	= SET_RSTATE_ACK 			МК отвечает, что команда установки состояния принята
 * 		LEN 	= 0x00 			0 байта данных
 * 		CRC8
 *

//////////////////////////////////////////////////

*	SET RELAY PORT	- Хост устанавливает состоияние только одного порта
 *
	
 * 		CID 	= CID_RELAY 			Сообщения, связанные с платой реле
 * 		CPAR 	= SET_RPORT_REQ 		Хост устанавливает состояние только одного порта
 * 		LEN 	= 3 			
 * 		DAT 	= {BoardIndex, RelayPort, PortStatus}
 * 		CRC8


* 		CID 	= CID_RELAY 			Сообщения, связанные с платой реле
 * 		CPAR 	= SET_RPORT_ACK 		МК отвечает, что команда установки состояния принята
 * 		LEN 	= 0x00 			0 байта данных
 * 		CRC8
 

//////////////////////////////////////////////////////////////////////////////////
 * 	МК -> Хост
 *
 * 	МК - Клиент
 *
 * 	3. REPORT RELAY STATE
 * 	3.1 МК генерирует сообщение
 * 		CID 	= CID_RELAY 			Сообщения, связанные с платой реле
 * 		CPAR 	= REPORT_RSTATE_REQ 			МК сообщает, что изменилось состояние одной или нескольких плат реле
 * 		LEN 	= 2*n 			n - количество плат реле с новым состоянием
 * 		DAT 	= {BoardIndex, RelayState} * n
 * 		CRC8
 *
 * 	3.2 Хост отвечает
 *
	 * 	CID 	= CID_RELAY 			Сообщения, связанные с платой реле
	 * 	CPAR 	= REPORT_RSTATE_ACK 			Хост отвечает, что команда принята
	 * 	LEN 	= 0x00 			0 байта данных
	 * 	CRC8
 *


 *
 * 	4. REPORT PORT STATUS
 * 	4.1 МК генерирует сообщение
 * 		CID 	= CID_RELAY 			Сообщения, связанные с платой реле
 * 		CPAR 	= REPORT_PORTSTA_REQ 			МК сообщает, что изменилось состояние одного порта 
 * 		LEN 	= 3 			                3 байта
 * 		DAT 	= {BoardIndex, PortNumber, PortStatus}

                        где BoardIndex   - 0-7
                            PortNumber   - 0-7 
                            PortStatus - 0,1
 *
 * 	4.2 Хост отвечает
 *
	 * 	CID 	= CID_RELAY 			Сообщения, связанные с платой реле
	 * 	CPAR 	= REPORT_PORTSTA_ACK 			Хост отвечает, что команда принята
	 * 	LEN 	= 0x00 			0 байта данных
	 * 	CRC8
 *
 */
//------------------------------------------------------------------------------
// Command IDs
#define CID_RELAY			0x01	// commands for working with relay boards
#define CID_CONFIG			0x02	// конфигурирование микроконтроллера
#define CID_TEMP            0x03    // чтение температурных датчиков с шины 1-wire
#define CID_DMX             0x04    // установка значений для DMX512 устройств
#define CID_ERROR			0xFF    // различные ошибки
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Command Parameter
//------------------------------------------------------------------------------
// Command Parameters for CID_RELAY
#define GET_RSTATE_REQ		0x01    // получить состояние платы расширения
#define GET_RSTATE_ANS		0x02    // ответ получен
#define SET_RSTATE_REQ		0x03    // установить состояние порта платы расширения
#define SET_RSTATE_ACK		0x04    // установка свершилась

#define REPORT_RSTATE_REQ	0x05    //      
#define REPORT_RSTATE_ACK	0x06    //

#define REPORT_PORTSTA_REQ	0x07    //
#define REPORT_PORTSTA_ACK	0x08    //

#define SET_RPORT_REQ       0x09    // установить определенный пин в порте на плате расширения
#define SET_RPORT_ACK       0x0a    // установка свершилась

//------------------------------------------------------------------------------

// Command Parameters for CID_CONFIG
#define GET_CONFIG_T_REQ		0x01	// host is reading mcu_config_t
#define GET_CONFIG_T_ANS		0x02
#define SET_CONFIG_T_REQ		0x03
#define SET_CONFIG_T_ACK		0x04

//------------------------------------------------------------------------------
// Command Parameters for CID_TEMP
#define REPORT_TEMP_REQ		0x01    // получить температуру с датчика
#define REPORT_TEMP_ACK		0x02    // температура считана

#define REPORT_ADD_TEMP_REQ	0x03    // получить ID датчика температуры
#define REPORT_ADD_TEMP_ACK	0x04    // ID получено

//------------------------------------------------------------------------------

// Command Parameters for CID_DMX
#define SET_VAL_DMX_REQ		0x01    // установить значение для определенного устройства DMX
#define SET_VAL_DMX_ACK		0x02    // значение установлено

#define SET_RGB_DMX_REQ		0x03    // установить RGB значение для определенного устройства DMX
#define SET_RGB_DMX_ACK		0x04    // значение RGB установлено
   
//------------------------------------------------------------------------------
// Common Command Parameters
#define MSG_ERROR		0xFF


// Error codes
#define ERROR_CID_NACK		1		// cmd id not recognized
#define ERROR_CPAR_NACK		2		// cmd parameter not recognized
#define ERROR_DATA		3


#define RELAY                   1
//----1-wire TEMPERATURE ---------------
#define TEMP_SENSOR             2   //set param relay_port_status_t ->device

#define ADDDEV_1WIRE            1
#define SENDTEMP_1WIRE          2   //set param relay_port_status_t ->device
//------------------------------------------------------------------------------   

//-----Описание структур -------------------------------------------------------
// power control thread output data
// client_netconn_thread input data
struct relay_port_status_t {
  uint8_t BoardIndex;
  uint8_t PortNumber;  
  uint8_t PortStatus;  
}; 
//------------------------------------------------------------

// power control thread input data
struct  relay_data_t {
  uint8_t MsgType; 	// 0 - set state	1 - set port status
  uint8_t byte1;	// BoardIndex		BoardIndex;
  uint8_t byte2; 	// RelayState		PortNumber;  
  uint8_t byte3;	// dummy		PortStatus;  
} ; 
//------------------------------------------------------------
//--Структура для передачи данных датчика и температуры
typedef struct  {
  uint8_t id[8];  
  uint8_t int_path;  
  uint8_t float_path;  
  uint8_t mode; // search device or send temperature
} temp_status_t; 
//------------------------------------------------------------


#endif // __TCP_PROTOCOL_H
