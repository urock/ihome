/* Includes ------------------------------------------------------------------*/
#include "httpserver-netconn.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define WEBSERVER_THREAD_PRIO    ( tskIDLE_PRIORITY + 1 )

extern const portTickType xTicksToWait;


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32_t nPageHits = 0;
uint8_t port;
uint8_t val;
uint8_t error = 0;

uint8_t	wr_val=0;

  struct netbuf *inbuf;
  char* buf;
  uint8_t *buf_ptr;
  
  u16_t buflen;
  
//  int val_http;
//  char str_http[5];
//  char port_http[3], status_http[2], enable_http[2], value_http[4], key_http[4], mode_http[2];   
//  struct sendText_t send2host_http; // —труктура хран€ща€ состо€ние порта, передаетс€ в очередь
//
//extern volatile struct PORT_struct PORTs[MAX_PORTs];
//extern volatile struct info_struct_t info_struct;
//extern unsigned char ID;
//extern xQueueHandle xQueue_sendText_t;
//
extern char store_str[10];
uint8_t board;
uint8_t inputs;
uint8_t outputs;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief serve tcp connection  
  * @param conn: pointer on connection structure 
  * @retval None
  */
void http_server_serve(struct netconn *conn) 
{
//  portBASE_TYPE xStatus;
//  struct fs_file * file;
  /* Read the data from the port, blocking if nothing yet there. 
   We assume the request (the part we care about) is in one netbuf */
  inbuf = netconn_recv(conn);
  //*****************************************************************************
  if (inbuf != NULL)
  {
    if (netconn_err(conn) == ERR_OK) 
    {
      netbuf_data(inbuf, (void**)&buf, &buflen);
    
      /* Is this an HTTP GET command? (only check the first 5 chars, since
      there are other formats for GET, and we're keeping it very simple )*/
      if ((buflen >=5) && (strncmp(buf, "GET /", 5) == 0))
      {
//*****************************************************************************
// ----------------  Set PORT Config -------------------------------------------        
        //-----------------------------------------------
  if(strncmp(buf, "GET /Board", 10) == 0)  //Set status port
        {
          error = parsing_html_parsingGET(buf,"Board");  // parsim config porta and add it to struct parsing_html_device_pt          
            if (error == 0){
            board = my_convert_str2int(store_str);  // определ€ем порт (переводим из строки в число)
          }
          error = parsing_html_parsingGET(buf,"Inputs");  // parsim config porta and add it to struct parsing_html_device_pt                    
            if (error == 0){
            inputs = my_convert_str2int(store_str);
            }
          error = parsing_html_parsingGET(buf,"Outputs");  // parsim config porta and add it to struct parsing_html_device_pt                    
            if (error == 0){
            outputs = my_convert_str2int(store_str);
            }
            
          relay_write_reg_spi(board,outputs);
            
        }

//------------------------------------------------------------------------------        
        else if(strncmp(buf, "GET /SetKey", 11) == 0) //Set port key
        {
//          error = parsing_html_parsingGET(buf,"port");  // parsim config porta and add it to struct parsing_html_device_pt          
//          if (error == 0){
//            port = my_convert_str2int(store_str);  // устанавливаем порт
//
//            error = parsing_html_parsingGET(buf,"SetKey");  // parsim config porta and add it to struct parsing_html_device_pt          
//            if (error == 0){
//              PORTs[port].key =  my_convert_str2int(store_str);              
//            }
//            else{ // error key
//              
//            }
//          }
//          else{ //error stky
//            
//          }
        }        
        
//------------------------------------------------------------------------------        
        else if(strncmp(buf, "GET /SetEnable", 14) == 0) //Set port enable
        {
//          error = parsing_html_parsingGET(buf,"port");  // parsim config porta and add it to struct parsing_html_device_pt          
//          if (error == 0){
//            port = my_convert_str2int(store_str);  // устанавливаем порт
//
//            error = parsing_html_parsingGET(buf,"SetEnable");  // parsim config porta and add it to struct parsing_html_device_pt          
//            if (error == 0){
//               my_convert_toPort_struct(port, store_str, "SetEnable"); 
//            }
//            else{ // error key
//              
//            }
//          }
//          else{ //error stky
//            
//          }
        }        
        
//------------------------------------------------------------------------------
        else if(strncmp(buf, "GET /GetStatus", 14) == 0)  // get port status
        {
//          error = parsing_html_parsingGET(buf,"port");  // parsim config porta and add it to struct PORT_struct          
//          
//          val_http = PORTs[port].num_port;  //Port int to string
//          sprintf(str_http, "%d", val_http);
//          strcpy(port_http,str_http);
//            
//          val_http = PORTs[port].status;  //Status int to string
//          sprintf(str_http, "%d", val_http);
//          strcpy(status_http, str_http);
//
//          send2host_http.cmd[0]=0;
//          strcat(strcpy(send2host_http.cmd, begin_pack), port_http);
//          strcat(send2host_http.cmd, status_pack);
//          strcat(send2host_http.cmd, status_http);
//          strcat(send2host_http.cmd, end_pack);
//                   
//          send2host_http.len = strlen(send2host_http.cmd);                  //length of send_com
//
//          xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_http, xTicksToWait );
//            if (xStatus != pdPASS){
//              LED_alarm_On();
//              printf("(httpserver-netconn)Can't to write port status, error is %d ", xStatus);
//            }
//          
//          vNetconnClient_init(NULL);       
        }
//------------------------------------------------------------------------------        
        else if(strncmp(buf, "GET /GetKey", 11) == 0)  // get port key
        {
//          error = parsing_html_parsingGET(buf,"port");  // parsim config porta and add it to struct PORT_struct          
//          
//          val_http = PORTs[port].num_port;  //Port int to string
//          sprintf(str_http, "%d", val_http);
//          strcpy(port_http,str_http);
//            
//          val_http = PORTs[port].key;  //Status int to string
//          sprintf(str_http, "%d", val_http);
//          strcpy(key_http, str_http);
//
//          send2host_http.cmd[0]=0;
//          strcat(strcpy(send2host_http.cmd, begin_pack), port_http);
//          strcat(send2host_http.cmd, key_pack);
//          strcat(send2host_http.cmd, key_http);
//          strcat(send2host_http.cmd, end_pack);
//                   
//          send2host_http.len = strlen(send2host_http.cmd);                  //length of send_com
//
//          xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_http, xTicksToWait );
//            if (xStatus != pdPASS){
//              LED_alarm_On();
//              printf("(httpserver-netconn)Can't to write port key, error is %d ", xStatus);
//            }
//          
//          vNetconnClient_init(NULL);       
        }
//------------------------------------------------------------------------------                
//        else if(strncmp(buf, "GET /STM32F2x7TASKS.html", 24) == 0)
//        {
//           /* Load dynamic page */
//           DynWebPage(conn);
//        }
        
//------------------------------------------------------------------------------        
        else if(strncmp(buf, "GET /GetEnable", 14) == 0)  // get port enable
        {
//          error = parsing_html_parsingGET(buf,"port");  // parsim config porta and add it to struct PORT_struct          
//
//          val_http = PORTs[port].num_port;  //Port int to string
//          sprintf(str_http, "%d", val_http);
//          strcpy(port_http,str_http);
//            
//          val_http = PORTs[port].enable;  //Status int to string
//          sprintf(str_http, "%d", val_http);
//          strcpy(enable_http, str_http);
//
//          send2host_http.cmd[0]=0;
//          strcat(strcpy(send2host_http.cmd, begin_pack), port_http);
//          strcat(send2host_http.cmd, enable_pack);
//          strcat(send2host_http.cmd, enable_http);
//          strcat(send2host_http.cmd, end_pack);
//                   
//          send2host_http.len = strlen(send2host_http.cmd);                  //length of send_com
//
//          xStatus = xQueueSendToBack( xQueue_sendText_t, &send2host_http, xTicksToWait );
//            if (xStatus != pdPASS){
//              LED_alarm_On();
//              printf("(httpserver-netconn)Can't to write port enable, error is %d ", xStatus);
//            }
//          
//          vNetconnClient_init(NULL);                 
        }
//------------------------------------------------------------------------------    
        else if(strncmp(buf, "GET /WriteFlash", 15) == 0) //Write to flash
        {
//          //«апись содержимого структуры PORTs во флеш пам€ть.
//          uint8_t size;
//          size = sizeof(PORTs);  // вычисл€ем размер структуры
//          buf_ptr =(uint8_t*) &PORTs[0];  // устанавливаем указатель на адрес структуры
//          write_buf_to_flash(buf_ptr, size, USER_FLASH_BASE);  // пишим структуру во флеш пам€ть
//          
//          info_struct.ver = PROG_VERTION;   //¬ерси€ программы       
//          info_struct.ID_board_flash = ID;  //«аписываем ID подключенной платы
//          
//          size = sizeof(info_struct);  // вычисл€ем размер структуры
//          buf_ptr =(uint8_t*) &info_struct;  // устанавливаем указатель на адрес структуры
//          write_buf_to_flash(buf_ptr, size, USER_FLASH_INFO);  // пишим структуру во флеш пам€ть          
//        }        
////------------------------------------------------------------------------------                
//        else if(strncmp(buf, "GET /ClearFlash", 15) == 0) //Write to flash
//        {
//          //ќчищаем флкш пам€ть
//          clear_flash(USER_FLASH_BASE);  // пишим структуру во флеш пам€ть
//          clear_flash(USER_FLASH_INFO);  // пишим структуру во флеш пам€ть                    
        }        


  
        
        else  //if receive unknow command then nuthing to do
        {
//          /* Load Error page */
//          file = fs_open("/404.html");
//          netconn_write(conn, (const unsigned char*)(file->data), (size_t)file->len, NETCONN_NOCOPY);
//          fs_close(file);
        }
      }      
    }
  }
  /* Close the connection (server closes in HTTP) */
  if (conn !=NULL){
  netconn_close(conn);
  }
  
  /* Delete the buffer (netconn_recv gives us ownership,
   so we have to make sure to deallocate the buffer) */
  netbuf_delete(inbuf);
}


/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void http_server_netconn_thread(void *arg)
{ 
  struct netconn *conn, *newconn;
  err_t err;
  
  /* Create a new TCP connection handle */
  conn = netconn_new(NETCONN_TCP);
  
  if (conn!= NULL)
  {
    /* Bind to port 80 (HTTP) with default IP address */
    err = netconn_bind(conn, NULL, 80);
    
    if (err == ERR_OK)
    {
      /* Put the connection into LISTEN state */
      netconn_listen(conn);
  
      while(1) 
      {
        /* accept any icoming connection */
        newconn = netconn_accept(conn);
      
        /* serve connection */
        http_server_serve(newconn);
      
        /* delete connection */
        netconn_delete(newconn);
      }
    }
    else
    {
      printf("can not bind netconn\n ");
    }
  }
  else
  {
    printf("can not create netconn \n ");
  }
}

/**
  * @brief  Initialize the HTTP server (start its thread) 
  * @param  none
  * @retval None
  */
void http_server_netconn_init()
{
  sys_thread_new("HTTP", http_server_netconn_thread, NULL, DEFAULT_THREAD_STACKSIZE, WEBSERVER_THREAD_PRIO);
}

/**
  * @brief  Create and send a dynamic Web Page. This page contains the list of 
  *         running tasks and the number of page hits. 
  * @param  conn pointer on connection structure 
  * @retval None
  */
//void DynWebPage(struct netconn *conn)
//{
////  portCHAR PAGE_BODY[512];
////  portCHAR pagehits[10] = {0};
////
////  memset(PAGE_BODY, 0,512);
////
////  /* Update the hit count */
////  nPageHits++;
////  sprintf(pagehits, "%d", nPageHits);
////  strcat(PAGE_BODY, pagehits);
////  strcat((char *)PAGE_BODY, "<pre><br>Name          State  Priority  Stack   Num" );
////  strcat((char *)PAGE_BODY, "<br>---------------------------------------------<br>");
////    
////  /* The list of tasks and their status */
////  vTaskList((signed char *)(PAGE_BODY + strlen(PAGE_BODY)));
////  strcat((char *)PAGE_BODY, "<br><br>---------------------------------------------");
////  strcat((char *)PAGE_BODY, "<br>B : Blocked, R : Ready, D : Deleted, S : Suspended<br>");
////
////  /* Send the dynamically generated page */
////  //netconn_write(conn, PAGE_START, strlen((char*)PAGE_START), NETCONN_COPY);
////  netconn_write(conn, PAGE_BODY, strlen(PAGE_BODY), NETCONN_COPY);
//}
