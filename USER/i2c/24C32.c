#include "i2c.h"
#include "delay.h"

uint8_t EEPROM_init(void)
{
	uint16_t timeOut = 0xffff;
	// Check connection to EEPROM
	I2C_GenerateSTART(I2C1,ENABLE); // Send START condition
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT))// Wait for EV5
	{
		if(timeOut-- == 0) 
			{
				#ifdef DEBUG
				printf("I2C connection error\r\n");
				#endif
				return 0;
			}
	}
	
	I2C_SendData(I2C1, EEPROM_ADDRESS_WRITE);
	
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) // Wait for EV6
	{
		if(timeOut-- == 0) 
			{
				#ifdef DEBUG
				printf("Device not respond \r\n");
				#endif
				return 0;
			}
	}
	I2C_GenerateSTOP(I2C1,ENABLE);
	#ifdef DEBUG
	printf("EEPROM responded!\r\n");
	#endif
	return 1;
}

/** 
  * @Author Tuan - 12/12/2015 
  * @brief  EEPROM_ReadReg
  * @param  None
  * @retval None
  */
uint8_t EEPROM_ReadReg(uint16_t addReg)
{
	uint16_t timeOut = 0xffff;
	uint32_t temp=0;
	uint8_t tempValue;
	/* Gui Start condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	/* cho time out */
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_SB) == RESET)
	{
		if(timeOut-- == 0)
			return 0;
	}
	/* Gui address slave de ghi */
	I2C_SendData(I2C1, EEPROM_ADDRESS_WRITE);
	timeOut = 0xffff;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if(timeOut-- == 0)
			return 0;
	}
		/* clear addr flag */
	(void) I2C1->SR2;
	I2C_SendData(I2C1, addReg>>8);
	/* clear addr flag */
	(void) I2C1->SR2;
	I2C_SendData(I2C1, addReg&0xff);
	/* EV8_2: Wait until BTF is set before programming the STOP */
	while ((I2C1->SR1 & 0x00004) != 0x000004);
	/* Gui Stop condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
	
	/* Gui Start condition */
		I2C_GenerateSTART(I2C1, ENABLE);
		/* cho time out */
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_SB) == RESET)
		{
		if(timeOut-- == 0)
			return 0;
		}
		/* Gui address slave de doc */
		I2C_SendData(I2C1, EEPROM_ADDRESS_READ);
		timeOut = 0xffff;
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
		{
			if(timeOut-- == 0)
				return 0;
		}
		/* clear ack */
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		/* clear addr flag */
		(void) I2C1->SR2;
		/* stop */
		I2C_GenerateSTOP(I2C1, ENABLE);
		/* Cho den khi co du lieu den (RXNE = 1) EV7 */
		while (I2C_GetFlagStatus(I2C1, I2C_FLAG_RXNE) == RESET);
		tempValue = I2C_ReceiveData(I2C1);
		while ((I2C1->CR1&0x200) == 0x200);		
		/* Enable ACK cho luot nhan tiep theo */
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		return tempValue;
}

/** 
  * @Author Tuan - 12/12/2015 
  * @brief  EEPROM_WriteReg
  * @param  None
  * @retval error 0, ok 1
  */
uint8_t EEPROM_WriteReg(uint16_t WriteAddr, uint8_t Data)
{
	uint16_t timeOut = 0xffff;
	
	/*!< Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	//timeOut = 0xffff;
//	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_SB) == RESET)
//	{
//		if(timeOut-- == 0) return 0;
//	}
	/*!< Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
		if(timeOut-- == 0)
		{printf("err EV5\r\n");
		return 0;}
	}
	/*!< Send EEPROM address for write */
	I2C_SendData(I2C1, EEPROM_ADDRESS_WRITE);
	/*!< Test on EV6 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	//while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if(timeOut-- == 0)
		{printf("err ev6\r\n");
		return 0;}
	}
		/* clear addr flag */
	//(void) I2C1->SR2;
	/*!< Send the EEPROM's internal address to write to : MSB of the address first */
	I2C_SendData(I2C1, (uint8_t)((WriteAddr & 0xFF00) >> 8));//8 bit dau
	/*!< Test on EV8 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{printf("err ev8\r\n");
		return 0;}
	}
	/*!< Send the EEPROM's internal address to write to : LSB of the address */	
		/* clear addr flag */
	//(void) I2C1->SR2;
	//while ((I2C1->SR1 & 0x00004) != 0x000004);
	I2C_SendData(I2C1, (uint8_t)(WriteAddr & 0x00FF));//8 bit sau
	/*!< Test on EV8 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{printf("err ev8\r\n");
		return 0;}
	}
	//(void) I2C1->SR2;
	//while ((I2C1->SR1 & 0x00004) != 0x000004);
	I2C_SendData(I2C1, Data);
	//(void) I2C1->SR2;

	/* EV8_2: Wait until BTF is set before programming the STOP */
	//while ((I2C1->SR1 & 0x00004) != 0x000004);
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{printf("err ev8\r\n");
		return 0;}
	}
	/* Gui Stop condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
	return 1;
}

/** 
  * @Author Tuan - 12/12/2015
  * @brief  EEPROM_W_Regs
  * @param  EEPROM_W_Regs
  * @retval True if OK, False if something was wrong
  */
uint8_t EEPROM_W_Regs(uint16_t addr,uint16_t length,uint8_t* pdata)
{
	uint16_t timeOut = 0xffff;
	uint16_t i;
	
	/*!< Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	/*!< Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if(timeOut-- == 0)
		{
			#ifdef DEBUG
			printf("err EV5\r\n");
			#endif
			return 0;
		}
	}
	/*!< Send EEPROM address for write */
	I2C_SendData(I2C1, EEPROM_ADDRESS_WRITE);
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if(timeOut-- == 0)
		return 0;
	}
	/*!< Send the EEPROM's internal address to write to : MSB of the address first */
	I2C_SendData(I2C1, (uint8_t)((addr & 0xFF00) >> 8));//8 bit dau
	/*!< Test on EV8 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{printf("err ev8\r\n");
		return 0;}
	}
	/*!< Send the EEPROM's internal address to write to : LSB of the address */	
		/* clear addr flag */
	//(void) I2C1->SR2;
	//while ((I2C1->SR1 & 0x00004) != 0x000004);
	I2C_SendData(I2C1, (uint8_t)(addr & 0x00FF));//8 bit sau
	/*!< Test on EV8 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{printf("err ev8\r\n");
		return 0;}
	}
	
	for(i=0;i<length;i++)
	{
		I2C_SendData(I2C1, *(pdata+i));
		while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{printf("err ev8\r\n");
		return 0;}
		}
	}
	
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{printf("err ev8\r\n");
		return 0;}
		}
	/* Gui Stop condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
	return 1;
}
