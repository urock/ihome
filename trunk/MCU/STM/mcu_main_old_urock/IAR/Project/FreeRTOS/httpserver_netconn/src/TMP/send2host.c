#include "send2host.h"


struct ip_addr remote_ip; 

extern const portTickType xTicksToWait;
extern xQueueHandle xQueue_sendText_t ,xQueue_port_struct;

/**
* @brief Initialisation Client task
* @param buf -string to send on web server Сейчас сделана передача этих параметров через очередь FIFO
* return - None
*/
void vNetconnClient_init(void *buf)
{
  portBASE_TYPE xReturn;
  
  xReturn = xTaskCreate(vNetconnClient, ( signed char * )"Client", configMINIMAL_STACK_SIZE * 2, NULL , HTTP_Cient_TASK_PRIO, NULL);
  if (xReturn != pdPASS){
      printf ("See errors %d in file 'projdefs.h' \n", xReturn);    
      LED_alarm_On();
  }
  
}

//*******************************************************************************
//void send_to_host(char buf1[])
/**
* @brief Function send on web server GET request
* @param buf -string to send on web server Сейчас сделана передача этих параметров через очередь FIFO
*/
void vNetconnClient(void *buf)
{
  portBASE_TYPE xStatus;
//  const portTickType xTicksToWait = 300 / portTICK_RATE_MS;  // Макс число тактов для чтения или записи аднных из\в очередь 
  struct sendText_t lReceivedValue;  // Сюда считываем структуру передаваемых данных из очереди
  
   
  uint8_t connErr, writeErr;  // Ошибки при передачи
  uint8_t fl_send = 1;   // Флаг посылки 
  struct netconn *xNetConn = NULL;  // указатель на соединения
  
  xStatus = xQueueReceive( xQueue_sendText_t, &lReceivedValue, xTicksToWait); // Берем данные из очереди
  if (xStatus != pdPASS){
    LED_alarm_On();
  }
//***************************************************
  IP4_ADDR( &remote_ip, 192, 168, 0, 111 );    // Адрес куда передаем

  while(fl_send){   //Первоначально у нас стоит 1 - т.е. нужно отослать пакет
  xNetConn = netconn_new ( NETCONN_TCP ); // Устанавливаем соединение
 
  while ( xNetConn == NULL ) { 
  netconn_delete ( xNetConn );
  xNetConn = netconn_new ( NETCONN_TCP );   // Пытаемся установить соединение несколько раз
}
//---------- Connection --------
  connErr = netconn_connect ( xNetConn, &remote_ip, 80 );  
 
  if ( connErr != ERR_OK )
{
  netconn_delete ( xNetConn );
  continue;
} 

// ---------Write data --------------------
  writeErr = netconn_write(xNetConn, lReceivedValue.cmd, lReceivedValue.len, NETCONN_NOCOPY);  //ишем данные в ethernet
  if (writeErr != ERR_OK){
    while(netconn_delete(xNetConn) != 0);
    continue;
  }
//---------------------------------------------  
  netconn_delete(xNetConn);
  fl_send = 0; // Пакет отослали !!! НУЖНО НЕ ЗАБЫТЬ ВЕРНУТЬ в 1 В КОНЦЕ ФУНКЦИИ.
}// end while fl_send
LED_warning_Toggle();  // Показываем что идет обмен по ethernet
fl_send = 1;  // Возвращаем в 1 чтобы можно было отправить следующий пакет!!!
vTaskDelete( NULL );
vTaskDelay(200);  // Делаем задержку для удаления мусора этой задачи ОС
//*******************************************************************
}