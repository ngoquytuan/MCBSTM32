/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void RTC_Configuration(void);
void Time_Adjust(void);
uint32_t Time_Regulate(uint32_t Tmp_HH,uint32_t Tmp_MM,uint32_t Tmp_SS);
void Time_Show(void);
void RTC_Init(void);
void Time_Display(uint32_t TimeVar);


#endif /* __RTC_H */

