/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void USART1_Init(void);
void USART1_SendStr_(char *str);
void USART2_Init(void);
void USART2_SendStr_(char *str);
void USER_UART_NVIC(void);
void USART1_SendChar_(char ch);
void USART2_SendChar_(char ch);
void u2Transmit(char *str,char length);

#endif /* __UART_H */

