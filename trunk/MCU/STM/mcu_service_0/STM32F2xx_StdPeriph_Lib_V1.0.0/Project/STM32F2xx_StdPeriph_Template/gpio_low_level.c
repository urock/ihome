

#include "main.h"
#include "gpio_low_level.h"



// leds section

GPIO_TypeDef* LEDS_GPIO_PORT[NUM_LEDS] = {Led0_GPIO_PORT, Led1_GPIO_PORT, Led2_GPIO_PORT, Led3_GPIO_PORT, Led4_GPIO_PORT, Led5_GPIO_PORT, Led6_GPIO_PORT}; 

const uint16_t LEDS_PIN[NUM_LEDS] = {Led0_PIN, Led1_PIN, Led2_PIN, Led3_PIN, Led4_PIN, Led5_PIN, Led6_PIN}; 

const uint32_t LEDS_CLK[NUM_LEDS] = {Led0_GPIO_CLK, Led1_GPIO_CLK, Led2_GPIO_CLK, Led3_GPIO_CLK, Led4_GPIO_CLK, Led5_GPIO_CLK, Led6_GPIO_CLK}; 


void init_leds(void)
{
	int i;
	
	for(i=0; i< NUM_LEDS; i++) {
	  init_led_hardware((LED_TYPE)i);
	  led_off((LED_TYPE)i);
	}
}

void init_led_hardware(LED_TYPE led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_AHB1PeriphClockCmd(LEDS_CLK[led], ENABLE);
  GPIO_InitStructure.GPIO_Pin = LEDS_PIN[led];
  	
  // configure as output
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(LEDS_GPIO_PORT[led], &GPIO_InitStructure);
}


// leds are active low, so invert output
void led_on(LED_TYPE led)
{
	LEDS_GPIO_PORT[led]->BSRRH = LEDS_PIN[led];  
}

void led_off(LED_TYPE led)
{
	LEDS_GPIO_PORT[led]->BSRRL = LEDS_PIN[led];  
}

void led_toggle(LED_TYPE led)
{
	LEDS_GPIO_PORT[led]->ODR ^= LEDS_PIN[led];
}
