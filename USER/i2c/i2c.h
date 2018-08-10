/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H
#define __I2C_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* dinh nghia giao tiep giua ds1307 va stm32f10x qua I2C */
	 #define I2C_DS1307		I2C1
	 #define DS1307_SCL		GPIO_Pin_8
	 #define DS1307_SDA		GPIO_Pin_9
	 #define RCC_APB2Periph_DS1307_GPIO		RCC_APB2Periph_GPIOB
	 #define RCC_APB1Periph_DS1307_I2C		RCC_APB1Periph_I2C1
		/* Dinh nghia cac thanh ghi cua ds1307 */
	 #define DS1307_ADDRESS_READ	0xD1
	 #define DS1307_ADDRESS_WRITE	0xD0
	 #define DS1307_SECOND_REG 		0x00
	 #define DS1307_MINUTE_REG 		0x01
	 #define DS1307_HOURS_REG 		0x02
	 #define DS1307_DAY_REG				0x03
	 #define DS1307_DATE_REG			0x04
	 #define DS1307_MONTH_REG			0x05
	 #define DS1307_YEAR_REG			0x06
	 #define DS1307_CONTROL_REG		0x07
	 
		#define EEPROM_ADDRESS_WRITE	0xAE
		#define EEPROM_ADDRESS_READ	0xAF
		uint8_t EEPROM_init(void);
		uint8_t EEPROM_WriteReg(uint16_t WriteAddr, uint8_t Data);
		uint8_t EEPROM_ReadReg(uint16_t addReg);
		uint8_t EEPROM_W_Regs(uint16_t addr,uint16_t length,uint8_t* pdata);
	 /*Fuctions*/
	 uint8_t I2C_Config(void);
	 uint8_t I2C_ReadReg(uint8_t deviceaddr,uint8_t addReg);
	 uint8_t I2C_WriteReg(uint8_t deviceaddr,uint8_t addReg, uint8_t Data);

	 uint8_t DS1307_WriteReg(uint8_t addReg, uint8_t Data);
	 uint8_t DS1307_Set_Time(uint8_t sec,uint8_t min,uint8_t hr,uint8_t day,uint8_t date,uint8_t month, uint16_t year);
	 uint8_t DS1307_init(unsigned char rs,unsigned char sqwe,unsigned char out);
	 uint8_t DS1307_ReadReg(uint8_t addReg);
	 #define DEBUG
#endif /* __I2C_H */