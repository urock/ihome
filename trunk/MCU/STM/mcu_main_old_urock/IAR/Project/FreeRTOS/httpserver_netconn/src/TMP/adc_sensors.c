#include "adc_sensors.h"
extern volatile struct ADC_PORT_struct ADC_PORTs[ADC_PORTn];

//-------------------------------------
__IO uint16_t ADC1ConvertedValue[ADC_CH];



//------------------------------------------------------------------------------
/** 
* @brief Description for ports GPIOM_PORT[PORTn]
*/
GPIO_TypeDef* GPIOM_ADC_PORT[ADC_PORTn] = {ADC0_GPIO_PORT,ADC3_GPIO_PORT, ADC4_GPIO_PORT, ADC5_GPIO_PORT,
                                       ADC6_GPIO_PORT, ADC8_GPIO_PORT, ADC9_GPIO_PORT, ADC10_GPIO_PORT,
                                       ADC12_GPIO_PORT, ADC13_GPIO_PORT
};

/** 
* @brief Description for pins GPIOM_PIN[PORTn]
*/
const uint16_t GPIOM_ADC_PIN[ADC_PORTn] = {ADC0_PIN, ADC3_PIN, ADC4_PIN, ADC5_PIN,
                                   ADC6_PIN, ADC8_PIN, ADC9_PIN, ADC10_PIN,
                                   ADC12_PIN, ADC13_PIN
};

/** 
* @brief Description for clocks GPIOM_CLK[PORTn]
*/
const uint32_t GPIOM_ADC_CLK[ADC_PORTn] = {ADC0_GPIO_CLK, ADC3_GPIO_CLK, ADC4_GPIO_CLK, ADC5_GPIO_CLK,
                                   ADC6_GPIO_CLK, ADC8_GPIO_CLK, ADC9_GPIO_CLK, ADC10_GPIO_CLK,
                                   ADC12_GPIO_CLK, ADC13_GPIO_CLK
};
//------------------------------------------------------------------------------


void adc_sensors_Init(void){
    GPIO_InitTypeDef            GPIO_InitStructure;
    ADC_InitTypeDef             ADC_InitStructure;
    ADC_CommonInitTypeDef       ADC_CommonInitStructure;
    DMA_InitTypeDef             DMA_InitStructureADC;
    uint8_t i;
    
    /* Enable ADC1, DMA2 and GPIO clocks ****************************************/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB , ENABLE);  //ADCx_CLK
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  DMA_DeInit(DMA_ADC_CHANNEL);
  /* DMA2 Stream0 channel2 configuration **************************************/
  DMA_InitStructureADC.DMA_Channel = DMA_ADC_CHANNEL;  
  DMA_InitStructureADC.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);
  DMA_InitStructureADC.DMA_Memory0BaseAddr = (uint32_t)&ADC1ConvertedValue;
  DMA_InitStructureADC.DMA_DIR = DMA_DIR_PeripheralToMemory; 
  DMA_InitStructureADC.DMA_BufferSize = ADC_CH;  //1
  DMA_InitStructureADC.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructureADC.DMA_MemoryInc = DMA_MemoryInc_Enable; //DMA_MemoryInc_Disable;
  DMA_InitStructureADC.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructureADC.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructureADC.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructureADC.DMA_Priority = DMA_Priority_High;
  DMA_InitStructureADC.DMA_FIFOMode = DMA_FIFOMode_Enable;     //Disable    
  DMA_InitStructureADC.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructureADC.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructureADC.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA_ADC_STREAM, &DMA_InitStructureADC);
  DMA_Cmd(DMA_ADC_STREAM, ENABLE);
//********************************************************************************
  GPIO_InitStructure.GPIO_Pin = GPIOM_ADC_PIN[0];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOM_ADC_PORT[0], &GPIO_InitStructure);  

  GPIO_InitStructure.GPIO_Pin = GPIOM_ADC_PIN[1];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOM_ADC_PORT[1], &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIOM_ADC_PIN[2];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOM_ADC_PORT[2], &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIOM_ADC_PIN[3];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOM_ADC_PORT[3], &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIOM_ADC_PIN[4];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOM_ADC_PORT[4], &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIOM_ADC_PIN[5];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOM_ADC_PORT[5], &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIOM_ADC_PIN[6];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOM_ADC_PORT[6], &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIOM_ADC_PIN[7];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOM_ADC_PORT[7], &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIOM_ADC_PIN[8];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOM_ADC_PORT[8], &GPIO_InitStructure); 

  GPIO_InitStructure.GPIO_Pin = GPIOM_ADC_PIN[9];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOM_ADC_PORT[9], &GPIO_InitStructure); 

//********************************************************************************  
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;  
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; //DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = ADC_CH; //1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel7 configuration *************************************/
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_0, 1, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_3, 2, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_4, 3, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_5, 4, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_6, 5, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_8, 6, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_9, 8, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_10, 9, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_12, 12, ADC_SampleTime_480Cycles);
  ADC_RegularChannelConfig(ADC1, ADC_CHANNEL_13, 13 , ADC_SampleTime_480Cycles);

 /* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

  
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
  
  for (i = 0 ; i < ADC_PORTn; i++){
    ADC_PORTs[i].enable = Off;
    ADC_PORTs[i].key = 0;
    ADC_PORTs[i].max_value = 0;
    ADC_PORTs[i].min_value = 0;
    ADC_PORTs[i].name = 0;
    ADC_PORTs[i].num_port = i;
    ADC_PORTs[i].read_value = 0;        
  } // end for   
  
  //********************************************************************************************************
 }





