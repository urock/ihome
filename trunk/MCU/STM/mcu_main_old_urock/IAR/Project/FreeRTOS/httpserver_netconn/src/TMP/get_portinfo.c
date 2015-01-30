#include "get_portinfo.h"

  extern volatile struct PORT_struct PORTs[MAX_PORTs];
  extern const portTickType xTicksToWait;
  extern xQueueHandle xQueue_sendText_t;// ,xQueue_port_struct;
                               
/**
* @Function return statuses of PORTs to HOST PC
* @param  void
* @return    void
* @brief Функция опрашивает состояние всех портов и передает эти состояния на ХОСТ машину по Ethernet  
*/
void vGet_portinfo (void){
  portBASE_TYPE xStatus;
//  const portTickType xTicksToWait = 100 / portTICK_RATE_MS;  // Макс число тактов для чтения или записи аднных из\в очередь 
  struct sendText_t send2host_gi; // Структура хранящая состояние порта, передается в очередь
  
  // Просто локальные переменные
  int val;
  char str[5];
  uint8_t i_gi;
  char port_gi[3], stat_gi[2], ena_gi[2], val_gi[4], key_gi[4], mode_gi[2];   
//  char send_com_gi[MAX_CMDSTR]; //="GET /objects/?object=Port0&op=m&m=changedStatus&status=1 HTTP/1.1\r\nHost: 192.168.0.111\r\nConnection: close\r\n\r\n\0";;
  for (i_gi = MIN_PORT_OUT; i_gi < MAX_PORT_OUT ; i_gi ++){
    
    val = PORTs[i_gi].num_port;  //Port int to string
    sprintf(str, "%d", val);
    strcpy(port_gi,str);
    
    val = PORTs[i_gi].status;  //Status int to string
    sprintf(str, "%d", val);
    strcpy(stat_gi,str);
    
    val = PORTs[i_gi].enable; //Enable int to string
    sprintf(str, "%d", val);
    strcpy(ena_gi,str);
    
    val = PORTs[i_gi].value;  //Value int to string
    sprintf(str, "%d", val);
    strcpy(val_gi,str);

    val = PORTs[i_gi].key;  //Key int to string
    sprintf(str, "%d", val);
    strcpy(key_gi,str);

    val = PORTs[i_gi].mode; //Mode int to string
    sprintf(str, "%d", val);
    strcpy(mode_gi,str);
    
// String for Status   
    send2host_gi.cmd[0]=0;
    strcat(strcpy(send2host_gi.cmd, begin_pack), port_gi);
    strcat(send2host_gi.cmd, status_pack);
    strcat(send2host_gi.cmd, stat_gi);
    strcat(send2host_gi.cmd, end_pack);
           
    send2host_gi.len = strlen(send2host_gi.cmd);                  //length of send_com
    xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_gi, xTicksToWait );
    if (xStatus != pdPASS){
      LED_alarm_On();
      printf ("(get_port_info) The queue is full, can't to write to queue, error is = %d, status = %d\n", xStatus ,stat_gi);
    }
    vNetconnClient_init(NULL);  
    vTaskDelay(200);
    
//// String for Active
    send2host_gi.cmd[0]=0;
    strcat(strcpy(send2host_gi.cmd, begin_pack), port_gi);
    strcat(send2host_gi.cmd, enable_pack);
    strcat(send2host_gi.cmd, ena_gi);
    strcat(send2host_gi.cmd, end_pack);
           
    send2host_gi.len = strlen(send2host_gi.cmd);                  //length of send_com
    xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_gi, xTicksToWait );
    if (xStatus != pdPASS){
      LED_alarm_On();
      printf ("(get_port_info) The queue is full, can't to write to queue, error is = %d, enable = %d\n", xStatus ,ena_gi);
    }
    
    vNetconnClient_init(NULL);     
    vTaskDelay(200);

//////// String for Key    
    send2host_gi.cmd[0]=0;
    strcat(strcpy(send2host_gi.cmd, begin_pack), port_gi);
    strcat(send2host_gi.cmd, key_pack);
    strcat(send2host_gi.cmd, key_gi);
    strcat(send2host_gi.cmd, end_pack);
           
    send2host_gi.len = strlen(send2host_gi.cmd);                  //length of send_com
    xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_gi, xTicksToWait );
    if (xStatus != pdPASS){
      LED_alarm_On();
      printf ("(get_port_info) The queue is full, can't to write to queue, error is = %d, key = %d\n", xStatus ,key_gi);
    }
    
    vNetconnClient_init(NULL); 
    vTaskDelay(200);    
    
//////// String for Mode
    send2host_gi.cmd[0]=0;
    strcat(strcpy(send2host_gi.cmd, begin_pack), port_gi);
    strcat(send2host_gi.cmd, mode_pack);
    strcat(send2host_gi.cmd, mode_gi);
    strcat(send2host_gi.cmd, end_pack);
           
    send2host_gi.len = strlen(send2host_gi.cmd);                  //length of send_com
    xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_gi, xTicksToWait );
    if (xStatus != pdPASS){
      LED_alarm_On();
      printf ("(get_port_info) The queue is full, can't to write to queue, error is = %d, mode = %d\n", xStatus ,mode_gi);
    }
    
    vNetconnClient_init(NULL); 
    vTaskDelay(200);

//////// String for Value
    send2host_gi.cmd[0]=0;
    strcat(strcpy(send2host_gi.cmd, begin_pack), port_gi);
    strcat(send2host_gi.cmd, value_pack);
    strcat(send2host_gi.cmd, val_gi);
    strcat(send2host_gi.cmd, end_pack);
           
    send2host_gi.len = strlen(send2host_gi.cmd);                  //length of send_com
    xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_gi, xTicksToWait );
    if (xStatus != pdPASS){
      LED_alarm_On();
      printf ("(get_port_info) The queue is full, can't to write to queue, error is = %d, value = %d\n", xStatus ,val_gi);
    }
    
    vNetconnClient_init(NULL);         
    vTaskDelay(200);
 } 
  vTaskDelete( NULL );
  
}