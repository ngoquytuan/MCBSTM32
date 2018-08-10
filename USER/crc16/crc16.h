/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CRC16_H
#define __CRC16_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

extern uint32_t modbus_sticks;

uint16_t crc16(volatile uint8_t *ptrToArray,uint8_t inputSize);
uint16_t makecrc16(char *ptrToArray,uint8_t inputSize); //A standard CRC algorithm
//void updateModbusData( void);
void modbus_slave_exe(uint8_t *uart_buff,uint8_t length);

void read_for_me(char _func);
void modbus_master_exe(uint8_t *uart_buff,uint8_t length);
void incomming_modbus_serial(char c);
uint8_t swap_bits(uint8_t c);
#endif /* __CRC16_H */

