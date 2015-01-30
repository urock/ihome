#ifndef __IHOME_BOARD_H__
#define __IHOME_BOARD_H__

// Write the port of ihome_board
//typedef enum 
//{
//  Port0 = 0,
//  Port1 = 1,
//  Port2 = 2,
//  Port3 = 3
//} Ports_TypeDef;
//
//#define Ports                             4

//#define Port0_PIN                         GPIO_Pin_6
//#define Port0_GPIO_PORT                   GPIOG
//#define Port0_GPIO_CLK                    RCC_AHB1Periph_GPIOG  
//  
//#define Port1_PIN                         GPIO_Pin_8
//#define Port1_GPIO_PORT                   GPIOG
//#define Port1_GPIO_CLK                    RCC_AHB1Periph_GPIOG  
//  
//#define Port2_PIN                         GPIO_Pin_9
//#define Port2_GPIO_PORT                   GPIOI
//#define Port2_GPIO_CLK                    RCC_AHB1Periph_GPIOI  
//  
//#define Port3_PIN                         GPIO_Pin_7
//#define Port3_GPIO_PORT                   GPIOC
//#define Port3_GPIO_CLK                    RCC_AHB1Periph_GPIOC


//Write BUTTON
//#define BUTTONn                          3 /*!< Joystick pins are connected to 
//                                                an IO Expander (accessible through 
//                                                I2C1 interface) */
//
//**
// * @brief Wakeup push-button
// */
//#define WAKEUP_BUTTON_PIN                GPIO_Pin_0
//#define WAKEUP_BUTTON_GPIO_PORT          GPIOA
//#define WAKEUP_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOA
//#define WAKEUP_BUTTON_EXTI_LINE          EXTI_Line0
//#define WAKEUP_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
//#define WAKEUP_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource0
//#define WAKEUP_BUTTON_EXTI_IRQn          EXTI0_IRQn 
//
//**
// * @brief Tamper push-button
// */
//#define TAMPER_BUTTON_PIN                GPIO_Pin_13
//#define TAMPER_BUTTON_GPIO_PORT          GPIOC
//#define TAMPER_BUTTON_GPIO_CLK           RCC_AHB1Periph_GPIOC
//#define TAMPER_BUTTON_EXTI_LINE          EXTI_Line13
//#define TAMPER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOC
//#define TAMPER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource13
//#define TAMPER_BUTTON_EXTI_IRQn          EXTI15_10_IRQn 
//
//**
// * @brief Key push-button
// */
//#define KEY_BUTTON_PIN                   GPIO_Pin_15
//#define KEY_BUTTON_GPIO_PORT             GPIOG
//#define KEY_BUTTON_GPIO_CLK              RCC_AHB1Periph_GPIOG
//#define KEY_BUTTON_EXTI_LINE             EXTI_Line15
//#define KEY_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOG
//#define KEY_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource15
//#define KEY_BUTTON_EXTI_IRQn             EXTI15_10_IRQn
//

#endif /* __IHOME_BOARD__ */