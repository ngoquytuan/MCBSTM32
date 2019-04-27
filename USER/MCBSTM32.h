/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FUC_H
#define __FUC_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void WWDG_Init(void);
void test_eeprom(void);
void sw_eeprom_stm32(void);
void GPIO_config(void);
void tasks(void);
void hardware_init(void);
#define USE_UART1
//#define USE_UART2
//#define TEST_UART
//#define USE_EMU_EEPROM
//#define TEST_EMU_EEPROM
#define USE_LCD16x2 // MUST CONFIG GPIO MAP WITH LCD
#define TEST_LCD16x2
#define USE_INTERNAL_RTC 
//#define TEST_INTERNAL_RTC
#define USE_ADC_DMA 
#define USE_KALMAN
#define TEST_ADC
#define TEST_TIM
#define USE_I2C
//#define USE_24C32
//#define TEST_24C32
/*
#ifdef TEST_24C32
#endif
*/
#endif /* __FUC_H */

