/*******************************************************
This program was created by tuannq

Project : Test on MCBSTM32 kit
Version : 1
Date    : 10/Aug/2018
Author  : tuannq
Company : None
Comments: 


Chip type               : STM32F103RBT6
Program type            : Examples
External Clock frequency: 8.000000 MHz
*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "GPIO_STM32F10x.h"
#include "main.h"

//#define LED(state) GPIO_PinWrite(GPIOA, 11, state);


///* Private typedef -----------------------------------------------------------*/
///* Private define ------------------------------------------------------------*/
///* Private macro -------------------------------------------------------------*/
///* Private variables ---------------------------------------------------------*/

//GPIO_WriteBit(GPIOB, GPIO_Pin_9, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_9)));
int main(void)
{
	
	hardware_init();

	while(1)
	{
		/* Update WWDG counter */
		WWDG_SetCounter(127);
    tasks();		
		
	}

		
}




	
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
 PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, (uint8_t) ch);
	return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}

#endif	



