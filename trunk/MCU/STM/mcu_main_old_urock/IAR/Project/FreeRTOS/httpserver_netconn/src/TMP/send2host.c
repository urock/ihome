#include "send2host.h"


struct ip_addr remote_ip; 

extern const portTickType xTicksToWait;
extern xQueueHandle xQueue_sendText_t ,xQueue_port_struct;

/**
* @brief Initialisation Client task
* @param buf -string to send on web server ������ ������� �������� ���� ���������� ����� ������� FIFO
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
* @param buf -string to send on web server ������ ������� �������� ���� ���������� ����� ������� FIFO
*/
void vNetconnClient(void *buf)
{
  portBASE_TYPE xStatus;
//  const portTickType xTicksToWait = 300 / portTICK_RATE_MS;  // ���� ����� ������ ��� ������ ��� ������ ������ ��\� ������� 
  struct sendText_t lReceivedValue;  // ���� ��������� ��������� ������������ ������ �� �������
  
   
  uint8_t connErr, writeErr;  // ������ ��� ��������
  uint8_t fl_send = 1;   // ���� ������� 
  struct netconn *xNetConn = NULL;  // ��������� �� ����������
  
  xStatus = xQueueReceive( xQueue_sendText_t, &lReceivedValue, xTicksToWait); // ����� ������ �� �������
  if (xStatus != pdPASS){
    LED_alarm_On();
  }
//***************************************************
  IP4_ADDR( &remote_ip, 192, 168, 0, 111 );    // ����� ���� ��������

  while(fl_send){   //������������� � ��� ����� 1 - �.�. ����� �������� �����
  xNetConn = netconn_new ( NETCONN_TCP ); // ������������� ����������
 
  while ( xNetConn == NULL ) { 
  netconn_delete ( xNetConn );
  xNetConn = netconn_new ( NETCONN_TCP );   // �������� ���������� ���������� ��������� ���
}
//---------- Connection --------
  connErr = netconn_connect ( xNetConn, &remote_ip, 80 );  
 
  if ( connErr != ERR_OK )
{
  netconn_delete ( xNetConn );
  continue;
} 

// ---------Write data --------------------
  writeErr = netconn_write(xNetConn, lReceivedValue.cmd, lReceivedValue.len, NETCONN_NOCOPY);  //���� ������ � ethernet
  if (writeErr != ERR_OK){
    while(netconn_delete(xNetConn) != 0);
    continue;
  }
//---------------------------------------------  
  netconn_delete(xNetConn);
  fl_send = 0; // ����� �������� !!! ����� �� ������ ������� � 1 � ����� �������.
}// end while fl_send
LED_warning_Toggle();  // ���������� ��� ���� ����� �� ethernet
fl_send = 1;  // ���������� � 1 ����� ����� ���� ��������� ��������� �����!!!
vTaskDelete( NULL );
vTaskDelay(200);  // ������ �������� ��� �������� ������ ���� ������ ��
//*******************************************************************
}