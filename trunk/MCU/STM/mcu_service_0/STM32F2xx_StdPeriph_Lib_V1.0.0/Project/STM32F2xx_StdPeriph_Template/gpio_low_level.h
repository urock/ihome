#ifndef PORTS_LOW_LEVEL_H
#define PORTS_LOW_LEVEL_H



// status leds - active low

//PE0	MCU_STATUS
//PE1	MCU_ALARM

//PE2	MCU_WARRNING
//PE3	MCU_LED0
//PE4	MCU_LED1
//PE5	MCU_LED2
//PE6	MCU_LED3



#define NUM_LEDS					7

// MCU_STATUS 
#define Led0_PIN                    GPIO_Pin_0
#define Led0_GPIO_PORT              GPIOE
#define Led0_GPIO_CLK               RCC_AHB1Periph_GPIOE

// MCU_ALARM
#define Led1_PIN                    GPIO_Pin_1
#define Led1_GPIO_PORT              GPIOE
#define Led1_GPIO_CLK               RCC_AHB1Periph_GPIOE

// MCU_WARRNING
#define Led2_PIN                    GPIO_Pin_2
#define Led2_GPIO_PORT              GPIOE
#define Led2_GPIO_CLK               RCC_AHB1Periph_GPIOE

// MCU_LED0
#define Led3_PIN                    GPIO_Pin_3
#define Led3_GPIO_PORT              GPIOE
#define Led3_GPIO_CLK               RCC_AHB1Periph_GPIOE

// MCU_LED1
#define Led4_PIN                    GPIO_Pin_4
#define Led4_GPIO_PORT              GPIOE
#define Led4_GPIO_CLK               RCC_AHB1Periph_GPIOE

// MCU_LED2
#define Led5_PIN                    GPIO_Pin_5
#define Led5_GPIO_PORT              GPIOE
#define Led5_GPIO_CLK               RCC_AHB1Periph_GPIOE

// MCU_LED3
#define Led6_PIN                    GPIO_Pin_6
#define Led6_GPIO_PORT              GPIOE
#define Led6_GPIO_CLK               RCC_AHB1Periph_GPIOE

typedef enum
{ 
  LED_STATUS = 0,
  LED_ALARM,
  LED_WARNING,
  LED0,
  LED1,
  LED2,
  LED3
}LED_TYPE;


void init_leds(void); 
void init_led_hardware(LED_TYPE led); 
void led_on(LED_TYPE led);
void led_off(LED_TYPE led);
void led_toggle(LED_TYPE led);


#endif // PORTS_LOW_LEVEL_H
