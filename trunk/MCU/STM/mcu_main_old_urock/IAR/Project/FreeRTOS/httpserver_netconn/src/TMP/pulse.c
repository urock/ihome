#include "pulse.h"


/**
* @brief Function init task my_tasks_vConfigPort
* @param dev - Device structure parsing_html_device_pt
* @return - None
*/
void make_pulse_init(void * pt)
{ 
   portBASE_TYPE xReturn;
   xReturn =  xTaskCreate(make_pulse, "PULSE", configMINIMAL_STACK_SIZE, pt ,PULSE_TASK_PRIO, NULL);
   if (xReturn != pdPASS){
     printf ("See errors %d in file 'projdefs.h' \n", xReturn);    
     LED_alarm_On();   
   }
//   
//   printf ("pdPASS = %d\n\n", xReturn);
}
//---------------------------------------------------
void make_pulse (void * pt)
{
  volatile struct PORT_struct *port ;  // Передаем порт подключенный к кнопке
  Ports_TypeDef port_num;
  
  port = (volatile struct PORT_struct *)pt;  
  port_num = getPort_name(port->num_port);  
  
//------------------------------------------------------------------------------    
    set_ports_PORTOn(port_num);
    LEDOn(LED4);
//    printf ("Port On %d \n", port_num);
//    vTaskDelay(10 / portTICK_RATE_MS);  
    vTaskDelay(10);
    set_ports_PORTOff(port_num);
    LEDOff(LED4);
//    printf ("Port Off %s \n", port_num);
//    vTaskDelay(10 / portTICK_RATE_MS);
    vTaskDelay(10);
    set_ports_PORTOn(port_num);
    LEDOn(LED4);
    vTaskDelete( NULL );   
//------------------------------------------------------------------------------
  
}
