#include "adcdma.h"
/******************ADC***********************************************/
__IO uint16_t ADCConvertedValue;
/******************ADC***********************************************/  
  
  void adcPA1_DMA_init(void)
  {
//#define ADC1_DR_Address    ((uint32_t)0x4001244C)
    ADC_InitTypeDef ADC_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;		
		GPIO_InitTypeDef GPIO_InitStructure;
		
		/* Enable clock DMA1 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
        /* Enable clock ADC1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
			/* Configure PA.01 (ADC Channel 1) as analog input -------------------------*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		/* DMA1 channel1 configuration ----------------------------------------------*/
		DMA_DeInit(DMA1_Channel1);
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(ADC1->DR));//ADC1_DR_Address;
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_InitStructure.DMA_BufferSize = 1;
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority = DMA_Priority_High;
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel1, &DMA_InitStructure);
		/* Enable DMA1 channel1 */
		DMA_Cmd(DMA1_Channel1, ENABLE);
		
		/* ADC1 configuration ------------------------------------------------------*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = ENABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 1;
		ADC_Init(ADC1, &ADC_InitStructure);

		/* ADC1 regular channel14 configuration */ 
		ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);

		/* Enable ADC1 DMA */
		ADC_DMACmd(ADC1, ENABLE);
		
		/* Enable ADC1 */
		ADC_Cmd(ADC1, ENABLE);

		/* Enable ADC1 reset calibration register */   
		ADC_ResetCalibration(ADC1);
		/* Check the end of ADC1 reset calibration register */
		while(ADC_GetResetCalibrationStatus(ADC1));

		/* Start ADC1 calibration */
		ADC_StartCalibration(ADC1);
		/* Check the end of ADC1 calibration */
		while(ADC_GetCalibrationStatus(ADC1));
			 
		/* Start ADC1 Software Conversion */ 
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  }
///////////////////////////////////////Neu su dung 2 kenh/////////////////////////////////////////	
GPIO_InitTypeDef myGPIO;
ADC_InitTypeDef myADC;
DMA_InitTypeDef DMA_InitStructure;
 __IO uint32_t ADC_DualConvertedValueTab[2];
	
	//Configure other I/O pins
void GPIO_config_ADC(void)
{
    // initialize LED C8, 9 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_StructInit(&myGPIO);
    myGPIO.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    myGPIO.GPIO_Mode = GPIO_Mode_Out_PP;
    myGPIO.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &myGPIO);
}
void adc_config()
{ 
  // PC1, PC2? analog input ??? ????.  
  myGPIO.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2; //set to pin 1, 2
  myGPIO.GPIO_Mode = GPIO_Mode_AIN; //set as analog input
  GPIO_Init(GPIOC, &myGPIO); //set to C1, C2

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
  RCC_ADCCLKConfig(RCC_PCLK2_Div6); //clock for ADC (max 14MHz, 72/6=12MHz)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //enable ADC clock

  // DMA1 channel1 configuration ----------------------------------------------
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_DualConvertedValueTab;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word; // 32bit
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word; // 32bit
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  // Enable DMA1 Channel1
  DMA_Cmd(DMA1_Channel1, ENABLE);
	
  //configure ADC1 parameters
  myADC.ADC_Mode = ADC_Mode_Independent;
  myADC.ADC_ScanConvMode = ENABLE;
  myADC.ADC_ContinuousConvMode = ENABLE;
  myADC.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  myADC.ADC_DataAlign = ADC_DataAlign_Right;
  myADC.ADC_NbrOfChannel  = 2;
  ADC_Init(ADC1, &myADC);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_55Cycles5); //PC1 as Input
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_55Cycles5); //PC2 as Input
  ADC_DMACmd(ADC1, ENABLE);

  //enable
  ADC_Cmd(ADC1, ENABLE);

  //Calibrate ADC *optional?
  ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while(ADC_GetCalibrationStatus(ADC1));

  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
	
