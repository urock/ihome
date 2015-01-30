#include "my_convert.h"
extern volatile struct PORT_struct PORTs[MAX_PORTs];
extern struct str2int_t str2int_gi;
/**
* @brief Function convert string to integer
* @param str - string for cinverting
* @return convert integer
*/
void my_convert_int2str (void){
        sprintf(str2int_gi.str, "%d", str2int_gi.val); // «аписать в device_t.port число, содержащеес€ в строке port_str
}
//---------------------------------------

/**
* @brief Function convert string to integer
* @param str - string for cinverting
* @return convert integer
*/
int my_convert_str2int (const char *str){
        uint8_t port;
        sscanf(str, "%d", &port); // «аписать в device_t.port число, содержащеес€ в строке port_str
        return port;
}
//---------------------------------------
/**
* @brief Function Convert str to structure Port_struct
* param port - number of port, *str - string to convert, how_str - tamplate 
* @return - void
*/
void my_convert_toPort_struct (uint8_t port, const char *str, const char *how_str)
{
  PORTs[port].num_port = port;
  Ports_TypeDef port_set = getPort_name(port);    
// IF tamplate is "mode"
  if (strcmp(how_str,"SetMode")==0){ //begin if mode
      PORTs[port].mode =my_convert_str2PortModeTypedef(str, port_set);
  }// end if mode
  
// IF tamplate is "status"
  if (strcmp(how_str,"SetStatus")==0){ //begin if status
      PORTs[port].status =my_convert_str2PortStatusTypeDef(str);
  }// end if status

// IF tamplate is "enable"
  if (strcmp(how_str,"SetEnable")==0){ //begin if enable
      PORTs[port].enable =my_convert_str2PortEnableTypeDef(str);
  }// end if enable
  
  
  if (strcmp(how_str,"GetStatus")==0){  //begin if status
      PORTs[port].status = getStatus(port);
  }// end if status
  
  if (strcmp(how_str,"GetEnable")==0){  //begin if enable
    PORTs[port].enable = getEnable(port);
  }// end if enable
  
  //value and key is desimal
  
}
      
      
//------------------------------------------------------
PortMode_TypeDef my_convert_str2PortModeTypedef(const char *str, Ports_TypeDef port_set){
  if (strcmp(str,"Out_pin")== 0){
    init_ports_PORTOut(port_set);
    return OUT_pin;
  }
  else if (strcmp(str,"In_pin")== 0){
    init_ports_PORTIn(port_set);
    return IN_pin;
  }
  else if (strcmp(str,"Pwm_pin")== 0){
//    init_ports_PORTPwm(port_set);
    return PWM_OUT_pin;
  }
  else if (strcmp(str,"Adc_pin")== 0){
    init_ports_PORTAdc(port_set);
    return ADC_IN_pin;
  }
  else{
    init_ports_PORTIn(port_set);
    return IN_pin;    
  }
}

//------------------------------------------------------------------------------      
PortStatus_TypeDef my_convert_str2PortStatusTypeDef(const char *str){
  if (strcmp(str,"On")== 0){
    return On;
  }
  else if (strcmp(str,"Off")== 0){
    return Off;
  }
}

//------------------------------------------------------------------------------

PortEnable_TypeDef my_convert_str2PortEnableTypeDef(const char *str){
  if (strcmp(str,"Enable")== 0){
    return enable;
  }
  else if (strcmp(str,"Disable")== 0){
    return disable;
  }  
}