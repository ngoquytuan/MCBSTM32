//#define HSE_VALUE    ((uint32_t)4332000)
#include "stm32f10x.h"

	
void USART1_Init(void);
void USART1_SendStr_(char *str);
void USART2_Init(void);
void USART2_SendStr_(char *str);
void TIM6_Config(void);
void USER_UART_NVIC(void);
void TIM_Config();
void USART1_SendChar_(char ch);


