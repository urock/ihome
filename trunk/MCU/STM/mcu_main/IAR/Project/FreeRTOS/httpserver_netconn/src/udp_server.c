/**
  ******************************************************************************
  * @file    server.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11/20/2009
  * @brief   A sample UDP/TCP server application.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "udp_server.h"



extern mcu_config_t	mcu_config;


/* Private define ------------------------------------------------------------*/
#define UDP_SERVER_PORT    7   /* define the UDP local connection port */
#define UDP_CLIENT_PORT    4   /* define the UDP remote connection port */




/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void UDP_server_init(void)
{
   struct udp_pcb *upcb;                                 
   
   /* Create a new UDP control block  */
   upcb = udp_new();
   
   /* Bind the upcb to the UDP_PORT port */
   /* Using IP_ADDR_ANY allow the upcb to be used by any local interface */
   udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT);
   
   /* Set a receive callback for the upcb */
   udp_recv(upcb, udp_server_callback, NULL);
  
}

/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void udp_server_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{

    
  uint8_t *data;
  uint8_t datalen;

   if ( p == NULL)
   {
     pbuf_free(p);
     return;
   }
   
   data = p->payload;
   /*Check request string*/
   if( strncmp(data, "iHome?",6)==0 )
   {
        datalen=0;
        
        //Put id data to buffer (ROSTA+RDM_MAC+RDM_IP+RDM_SERIAL+RBD_SERIAL)
        data = (uint8_t*)p->payload;
        //ROSTA - 5Bytes
        for(uint8_t i=0; i<4; i++)
        {
          *data++=mcu_config.SN[i];
          datalen++;          
        }

//        //CRC
//        *data= calc_CRC8_general((uint8_t*)p->payload, (uint16_t)datalen);
//        datalen++;
        
	p->tot_len = datalen;
	p->len = p->tot_len;
   
  /* Connect to the remote client */
	udp_connect(upcb, addr, UDP_CLIENT_PORT);
    
  /* Tell the client that we have accepted it */
	udp_send(upcb, p);

  /* free the UDP connection, so we can accept new clients */
	udp_disconnect(upcb);
	
  /* Bind the upcb to IP_ADDR_ANY address and the UDP_PORT port*/
  /* Be ready to get a new request from another client */  
	udp_bind(upcb, IP_ADDR_ANY, UDP_SERVER_PORT);
	
  /* Set a receive callback for the upcb */
	udp_recv(upcb, udp_server_callback, NULL);   
 	
	pbuf_free(p);
   }   
}

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
