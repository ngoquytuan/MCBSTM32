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
#endif /* __FUC_H */

