/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H
#define __TIM_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void tim_RCC_Configuration(void);
void tim_GPIO_Configuration(void);
void tim_NVIC_Configuration(void);
void tim_ex (void);
void tim_irq_fuc(void);

#endif /* __TIM_H */