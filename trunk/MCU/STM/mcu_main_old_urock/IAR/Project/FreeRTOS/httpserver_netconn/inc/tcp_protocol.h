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
 *
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
 */

// Command IDs
#define CID_RELAY			0x01	// commands for working with relay boards
#define CID_CONFIG			0x02	//
#define CID_ERROR			0xFF


// Command Parameter

// Command Parameters for CID_RELAY
#define GET_RSTATE_REQ		0x01
#define GET_RSTATE_ANS		0x02
#define SET_RSTATE_REQ		0x03
#define SET_RSTATE_ACK		0x04

#define REPORT_RSTATE_REQ		0x05
#define REPORT_RSTATE_ACK		0x06


// Command Parameters for CID_CONFIG
#define GET_CONFIG_T_REQ		0x01	// host is reading mcu_config_t
#define GET_CONFIG_T_ANS		0x02
#define SET_CONFIG_T_REQ		0x03
#define SET_CONFIG_T_ACK		0x04


// Common Command Parameters
#define MSG_ERROR		0xFF


// Error codes
#define ERROR_CID_NACK		1		// cmd id not recognized
#define ERROR_CPAR_NACK		2		// cmd parameter not recognized
#define ERROR_DATA			3







#endif // __TCP_PROTOCOL_H
