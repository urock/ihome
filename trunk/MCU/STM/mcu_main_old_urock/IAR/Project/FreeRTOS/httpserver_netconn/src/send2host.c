#include "send2host.h"


struct ip_addr remote_ip; 

//extern const portTickType xTicksToWait;
extern xQueueHandle xQueue_sendText_t; // ,xQueue_port_struct;

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
//      printf ("See errors %d in file 'projdefs.h' \n", xReturn);    
  }
  
}

//*******************************************************************************
/**
* @brief Function send on web server GET request
* @param buf -string to send on web server Сейчас сделана передача этих параметров через очередь FIFO
*/
void vNetconnClient(void *buf)
{
  portBASE_TYPE xStatus;
  portTickType xTicksToWait = 300 / portTICK_RATE_MS;;
  struct sendText_t lReceivedValue;  // Сюда считываем структуру передаваемых данных из очереди
  
  uint8_t connErr, writeErr;  // Ошибки при передачи
  uint8_t fl_send = 1;   // Флаг посылки 
  struct netconn *xNetConn = NULL;  // указатель на соединения
  for (;;){
        xStatus = xQueueReceive( xQueue_sendText_t, &lReceivedValue, xTicksToWait); // Берем данные из очереди
        if (xStatus != pdTRUE){
        }
        else {
      //***************************************************
              IP4_ADDR( &remote_ip, HOST_IP_ADDR0, HOST_IP_ADDR1, HOST_IP_ADDR2, HOST_IP_ADDR3 );    // Адрес куда передаем
//              IP4_ADDR( &remote_ip, 192, 168, 0, 103 );    // Адрес куда передаем

              while(fl_send){   //Первоначально у нас стоит 1 - т.е. нужно отослать пакет
              xNetConn = netconn_new ( NETCONN_TCP ); // Устанавливаем соединение
             
      //        while ( xNetConn == NULL ) { 
      //        netconn_delete ( xNetConn );
      //        xNetConn = netconn_new ( NETCONN_TCP );   // Пытаемся установить соединение несколько раз
      //      }
            //---------- Connection --------
              connErr = netconn_connect ( xNetConn, &remote_ip, 80 );  
             
              if ( connErr != ERR_OK )
            {
              netconn_delete ( xNetConn );
              continue;
            } 
              else {
                // ---------Write data --------------------
                  writeErr = netconn_write(xNetConn, lReceivedValue.cmd, lReceivedValue.len, NETCONN_NOCOPY);  //ишем данные в ethernet
                  if (writeErr != ERR_OK){
                    while(netconn_delete(xNetConn) != 0);
                    continue;
                  }
                //---------------------------------------------  
                  netconn_delete(xNetConn);
                  fl_send = 0; // Пакет отослали !!! НУЖНО НЕ ЗАБЫТЬ ВЕРНУТЬ в 1 В КОНЦЕ ФУНКЦИИ.
              }
            }// end while fl_send
        }
      fl_send = 1;  // Возвращаем в 1 чтобы можно было отправить следующий пакет!!!
  } //end for
//vTaskDelete( NULL );
vTaskDelay(200);
//*******************************************************************
}