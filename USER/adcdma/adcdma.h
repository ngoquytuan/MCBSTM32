/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADCDMA_H
#define __ADCDMA_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern __IO uint16_t ADCConvertedValue;
void adcPA1_DMA_init(void);

#endif /* __ADCDMA_H */

