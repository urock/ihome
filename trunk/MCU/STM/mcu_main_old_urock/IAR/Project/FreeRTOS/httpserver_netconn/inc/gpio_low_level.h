#ifndef PORTS_LOW_LEVEL_H
#define PORTS_LOW_LEVEL_H

#include "sys_include.h"

// status leds - active low

#define NUM_LEDS					3

// status led
#define Led0_PIN                    GPIO_Pin_2
#define Led0_GPIO_PORT              GPIOE
#define Led0_GPIO_CLK               RCC_AHB1Periph_GPIOE

// alarm led
#define Led1_PIN                    GPIO_Pin_3
#define Led1_GPIO_PORT              GPIOE
#define Led1_GPIO_CLK               RCC_AHB1Periph_GPIOE

// warning led
#define Led2_PIN                    GPIO_Pin_4
#define Led2_GPIO_PORT              GPIOE
#define Led2_GPIO_CLK               RCC_AHB1Periph_GPIOE

typedef enum
{ 
  LED0 = 0,
  LED1,
  LED2
}LED_TYPE;


void init_leds(void); 
void init_led_hardware(LED_TYPE led); 
void led_on(LED_TYPE led);
void led_off(LED_TYPE led);
void led_toggle(LED_TYPE led);


#endif // PORTS_LOW_LEVEL_H
