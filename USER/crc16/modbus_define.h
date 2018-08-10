/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MODBUSDEF_H
#define __MODBUSDEF_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#define MODBUS_TYPE_MASTER 0
#if (MODBUS_TYPE_MASTER==1)
#define MODBUS_SLAVE_ADDRESS 0xF7
#endif

#define MODBUS_TYPE_SLAVE 1
#if (MODBUS_TYPE_SLAVE==1)
#define MODBUS_TYPE MODBUS_TYPE_SLAVE
//#define MODBUS_SERIAL_TYPE MODBUS_RTU     //use MODBUS_ASCII for ASCII mode
#define MODBUS_SERIAL_RX_BUFFER_SIZE 64
#define MODBUS_SERIAL_BAUD 115200


#define MODBUS_GETDATA_TIMEOUT 40
#define MODBUS_ADDRESS 0x11
#endif

#define int1   uint8_t
#define int8   uint8_t
#define int16  uint16_t
#define int32  uint32_t

#define bit_set(d,b) (d|=(1<<b)) 
#define bit_clear(d,b) (d&=~(1<<b))
#define bit_test(a,b) (a & (1<<b))!=0 
#define make16(msb, lsb)  ((msb << 8) | lsb)
#define make8(var,offset) ((var >> (offset*8)) & 0xff)
#define _false 0
#define _true  1
#define FALSE 0
#define TRUE  1




void modbus_timeout_now(void);

#endif /* __MODBUSDEF_H */

