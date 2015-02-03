#ifndef PORTS_LOW_LEVEL_H
#define PORTS_LOW_LEVEL_H

#include "sys_include.h"

// status leds - active low

#define NUM_LEDS					3

// status led
#define Alarm_Led_PIN                    GPIO_Pin_2
#define Alarm_Led_GPIO_PORT              GPIOE
#define Alarm_Led_GPIO_CLK               RCC_AHB1Periph_GPIOE

// alarm led
#define Warning_Led_PIN                    GPIO_Pin_3
#define Warning_Led_GPIO_PORT              GPIOE
#define Warning_Led_GPIO_CLK               RCC_AHB1Periph_GPIOE

// warning led
#define Status_Led_PIN                    GPIO_Pin_4
#define Status_Led_GPIO_PORT              GPIOE
#define Status_Led_GPIO_CLK               RCC_AHB1Periph_GPIOE

typedef enum
{ 
  Status_Led = 0,
  Warning_Led,
  Alarm_Led
}LED_TYPE;


void init_leds(void); 
void init_led_hardware(LED_TYPE led); 
void led_on(LED_TYPE led);
void led_off(LED_TYPE led);
void led_toggle(LED_TYPE led);


#endif // PORTS_LOW_LEVEL_H
