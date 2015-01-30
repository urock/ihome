#include "set_ports.h"
extern const uint32_t GPIOM_CLK[MAX_PORTs];
extern const uint16_t GPIOM_PIN[MAX_PORTs];
extern GPIO_TypeDef* GPIOM_PORT[MAX_PORTs];
extern const uint16_t PORT_EXTI_LINE[MAX_PORTs];
extern const uint8_t PORT_EXTI_PORT_SOURCE[MAX_PORTs];
extern const uint8_t PORT_EXTI_PIN_SOURCE[MAX_PORTs];
extern const uint8_t PORT_EXTI_IRQn[MAX_PORTs];
extern volatile struct PORT_struct PORTs[MAX_PORTs];
/**-------------- Set Ports ----------------------*/
//void set_ports_setAllValues(Ports_TypeDef Port, PortMode_TypeDef mode, int status, int def_value)
/**
* @brief Функция пока не реализована (должна инициализировать все порты, уже есть аналог LED_Init
* @param - PORT_struct
* @return - void
*
*/
void set_ports_setAllValues(volatile struct PORT_struct *device)
{  
//  if (device->mode == OUT_pin)
//  {
//      init_ports_PORTOut(device->num_port);      
//      if (device->status == On){
//        set_ports_PORTOn(device->num_port);
//      }
//      else
//      {
//        set_ports_PORTOff(device->num_port);
//      }
//  }
  
//  
//  switch(device->mode)
//  {
//    case 0:{
//      
//    break;
//  }
//    case 1:{   //interrupt 
//      set_ports_interrupt(device->Port,INP_pin);
//    break;
//  }
//    case 2:{  //PWM
//  
//    break;
//  }
//    case 3:{ //ADC
//  
//    break;
//  }
// } //end switch
}
//-------------------------------------------------
/**
* @brief  Turns selected LED On.
* @param  Led: Specifies the Led to be set on. 
*   This parameter can be one of following parameters:
* @retval None
*/
void set_ports_PORTOn(Ports_TypeDef Port)
{
  GPIOM_PORT[Port]->BSRRL = GPIOM_PIN[Port];
}

/**
* @brief  Turns selected LED Off.
* @param  Led: Specifies the Led to be set off. 
*   This parameter can be one of following parameters:
* @retval None
*/
void set_ports_PORTOff(Ports_TypeDef Port)
{
  GPIOM_PORT[Port]->BSRRH = GPIOM_PIN[Port];  
}

/**
* @brief  Toggles the selected LED.
* @param  Led: Specifies the Led to be toggled. 
*   This parameter can be one of following parameters:
* @retval None
*/
void set_ports_PORTToggle(Ports_TypeDef Port)
{
  GPIOM_PORT[Port]->ODR ^= GPIOM_PIN[Port];
}

/**
* @brief  Returns the selected Button state.
* @param  Button: Specifies the Button to be checked.
*   This parameter can be one of following parameters:    
*     @arg BUTTON_WAKEUP: Wakeup Push Button
*     @arg BUTTON_TAMPER: Tamper Push Button  
*     @arg BUTTON_KEY: Key Push Button 
*     @arg BUTTON_RIGHT: Joystick Right Push Button 
*     @arg BUTTON_LEFT: Joystick Left Push Button 
*     @arg BUTTON_UP: Joystick Up Push Button 
*     @arg BUTTON_DOWN: Joystick Down Push Button
*     @arg BUTTON_SEL: Joystick Sel Push Button    
* @retval The Button GPIO pin value.
*/
uint32_t set_ports_Interrupt_GetState(Ports_TypeDef Port)
{
  return GPIO_ReadInputDataBit(GPIOM_PORT[Port], GPIOM_PIN[Port]);
}
//-------------------------------------------------
/**
* @brief Function convert integer to Port_TypeDef 
* @param pt - number of Port
* @return Port_TypeDef (Port1)
*/
Ports_TypeDef getPort_name (uint8_t pt)
{
  switch (pt)
  {
  case 0:
    {
      return (Port0);  
      break;
    } //end case 0
    
  case 1:
    {
      return (Port1);  
      break;
    } //end case1

  case 2:
    {
      return (Port2);  
      break;
    }  //end case 2

  case 3:
    {
      return (Port3);  
      break;
    } //end case 3

  case 4:
    {
      return (Port4);  
      break;
    } //end case 4

  case 5:
    {
      return (Port5);  
      break;
    } //end case 5

  case 6:
    {
      return (Port6);  
      break;
    } //end case 6

  case 7:
    {
      return (Port7);  
      break;
    }  //end case 7

  case 8:
    {
      return (Port8);  
      break;
    }  //end case 8

  case 9:
    {
      return (Port9);  
      break;
    }  //end case 9

  case 10:
    {
      return (Port10);  
      break;
    }  //end case 10

  case 11:
    {
      return (Port11);  
      break;
    }  //end case 11

  case 12:
    {
      return (Port12);  
      break;
    }  //end case 12

  case 13:
    {
      return (Port13);  
      break;
    }  //end case 13

  case 14:
    {
      return (Port14);  
      break;
    }  //end case 14

  case 15:
    {
      return (Port15);  
      break;
    } //end case 15
    
  case 16:
    {
      return (Port16);  
      break;
    } //end case 16 

  case 17:
    {
      return (Port17);  
      break;
    }  //end case 17

  case 18:
    {
      return (Port18);  
      break;
    } //end case 18

  case 19:
    {
      return (Port19);  
      break;
    } //end case 19

  case 20:
    {
      return (Port20);  
      break;
    } //end case 20

  case 21:
    {
      return (Port21);  
      break;
    } //end case 21

  case 22:
    {
      return (Port22);  
      break;
    }  //end case 22

  case 23:
    {
      return (Port23);  
      break;
    }  //end case 23

  case 24:
    {
      return (Port24);  
      break;
    }  //end case 24

  case 25:
    {
      return (Port25);  
      break;
    }  //end case 25
  case 26:
    {
      return (Port26);  
      break;
    } //end case 26
    
  case 27:
    {
      return (Port27);  
      break;
    } //end case27

  case 28:
    {
      return (Port28);  
      break;
    }  //end case 28

  case 29:
    {
      return (Port29);  
      break;
    } //end case 29

  case 30:
    {
      return (Port30);  
      break;
    } //end case 30

  case 31:
    {
      return (Port31);  
      break;
    } //end case 31

  default : {
    return (Port100);  // error Port
    break;
  } //end case default
        
  } // end switch
    
} //end function


//*********************************************
/**
* @brief Function set Raise or Falling interrupt
* @param port - number of Port, mode - Interrupt (Raise or fallling)
* @arguments
*         EXTI_Trigger_Rising
*         EXTI_Trigger_Falling
*         EXTI_Trigger_Rising_Falling
*         EXTITrigger_TypeDef;
* @return void
*/
void set_ports_interrupt(Ports_TypeDef Port, uint8_t mode)
{
  switch (mode){
  case 2:
    {
      init_ports_Interrupt(Port,PORT_MODE_EXTI, EXTI_Trigger_Rising);
      break;
    }
  case 3:
    {
      init_ports_Interrupt(Port,PORT_MODE_EXTI, EXTI_Trigger_Falling);
      break;
    }
  case 4:
    {
      init_ports_Interrupt(Port,PORT_MODE_EXTI, EXTI_Trigger_Rising_Falling);
      break;
    }
  }
  
}

//---------------------------
/**
* @brief  Function convert integer to PortMode_TypeDef
* param pt - mode pins (OUT, IN, PWM, ADC)
* @return PortMode_TypeDef (OUT_pin, INP_pin, PWM_OUT_pin, ADC_IN_pin)
*/
PortMode_TypeDef getPinMode (uint8_t pt)
{
  switch (pt)
  {
    case 0:
      {
      return (OUT_pin);
      break;
      }
    case 1:
      {
      return (IN_pin);
      break;
      }
    case 2:
      {
      return (PWM_OUT_pin);
      break;
      }
    case 3:
      {
      return (ADC_IN_pin);
      break;
      }
  default :{
      return (IN_pin);
      break;    
  }
  } //switch
}

//---------------------------
/**
* @brief  Function convert integer to PortMode_TypeDef
* param pt - mode pins (OUT, IN, PWM, ADC)
* @return PortMode_TypeDef (OUT_pin, INP_pin, PWM_OUT_pin, ADC_IN_pin)
*/
uint8_t getIntPinMode (PortMode_TypeDef pt)
{
  uint8_t a;
  if (pt == OUT_pin){
    a = 0;
  };
  
  if (pt == IN_pin){
    a = 1;
  };

  if (pt == PWM_OUT_pin){
    a = 2;
  };

  if (pt == ADC_IN_pin){
    a = 2;
  };
  return a;
}


//---------------------------------------------
/**
* @brief Function convert integer to PortStatus_TypeDef
* @param stat - status (Off, On)  
* @return PortStatus_TypeDef (Off, On)
*/
PortStatus_TypeDef getStatus(uint8_t stat){
    switch (stat){
    case 0:
      {
        return (Off);
        break;
      }
    case 1:
      {
        return (On);
        break;
      }
    default:
      {
        return (Off);
        break;
      }
      
    } // end switch
  
}


//---------------------------------------------
/**
* @brief Function convert integer to PortStatus_TypeDef
* @param stat - status (Off, On)  
* @return PortStatus_TypeDef (Off, On)
*/
uint8_t getIntStatus(PortStatus_TypeDef stat){
  uint8_t pt;
  if (stat == Off){
    pt = Off_port;
  };
  
  if (stat == On){
    pt = On_port;
  };  
  return pt;
}
//---------------------------------------------
/**
* @brief Function convert integer to PortStatus_TypeDef
* @param stat - status (Off, On)  
* @return PortEnable_TypeDef (disable, enable)
*/
PortEnable_TypeDef getEnable(uint8_t ena){
    switch (ena){
    case 0:
      {
        return (disable);
        break;
      }
    case 1:
      {
        return (enable);
        break;
      }
    default:
      {
        return (disable);
        break;
      }
      
    } //end switch ena
  
}
//------------------------------------------------------------------------------
/**
* @brief Function convert integer to PortStatus_TypeDef
* @param stat - status (Off, On)  
* @return PortEnable_TypeDef (disable, enable)
*/
uint8_t getIntEnable(PortEnable_TypeDef ena){
  uint8_t en_;
  if (ena == disable){
    en_ = 0;
  };
  
  if (ena == enable){
    en_ = 1;
  };  
  return en_;
}

/************************
* @brief Переключение выключателя в противоположное положение 
* @param Port_struct Port номер порта
* @return void
*/
//---------------------------------------------------------------------------
void set_ports_ToggleState_Ports( volatile struct PORT_struct Port)
{
  if (Port.status == Off){
    Port.status = On;
  }
  else
  {
    Port.status = Off;
  }
  
}
//---------------------------------------------------------------


