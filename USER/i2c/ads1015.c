#include "ads1015.h"
uint8_t   m_i2cAddress;
uint8_t   m_conversionDelay;
uint8_t   m_bitShift;
adsGain_t m_gain;
void setGain(adsGain_t gain)
{
  m_gain = gain;
}

adsGain_t getGain(void)
{
  return m_gain;
}

void ADS1015_init(uint8_t i2cAddress) 
{
   m_i2cAddress = i2cAddress;
   m_conversionDelay = ADS1015_CONVERSIONDELAY;
   m_bitShift = 4;
   m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
}
/**************************************************************************/
uint8_t ADS1115_init(void)
{
	uint16_t timeOut = 0xffff;
	
	m_i2cAddress = ADS1x15_ADDR;
  m_conversionDelay = ADS1115_CONVERSIONDELAY;
  m_bitShift = 0;
  m_gain = GAIN_TWOTHIRDS; /* +/- 6.144V range (limited to VDD +0.3V max!) */
	// Check connection to ADS1115
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
		I2C_SendData(I2C1, ADS1x15_ADDR);
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) // Wait for EV6
	{
		if(timeOut-- == 0) 
			{
				#ifdef DEBUG
				printf("ADS1115 not respond \r\n");
				#endif
				return 0;
			}
	}
	I2C_GenerateSTOP(I2C1,ENABLE);
	#ifdef DEBUG
	printf("ADS1115 responded!\r\n");
	#endif
	return 1;
}

static uint8_t i2cread(void) {
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
	I2C_SendData(I2C1, 0x90);
	timeOut = 0xffff;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if(timeOut-- == 0)
			return 0;
	}
		/* clear addr flag */
	(void) I2C1->SR2;
	I2C_SendData(I2C1, 0);

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
		I2C_SendData(I2C1, 0x91);
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

static void i2cwrite(uint8_t x) {

	
	uint16_t timeOut = 0xffff;
	
	/*!< Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/*!< Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
		if(timeOut-- == 0)
		{
			//printf("err EV5\r\n");
		  break;
		}
	}
	/*!< Send EEPROM address for write */
	I2C_SendData(I2C1, ADS1x15_ADDR);
	/*!< Test on EV6 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	//while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if(timeOut-- == 0)
		{
			//printf("err ev6\r\n");
		break;}
	}

	/*!< Send the EEPROM's internal address to write to : LSB of the address */	
	I2C_SendData(I2C1, (uint8_t)(0));//8 bit sau
	/*!< Test on EV8 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{
			//printf("err ev8\r\n");
		break;}
	}
	
	I2C_SendData(I2C1, x);
	
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{
			//printf("err ev8\r\n");
		break;}
	}
	/* Gui Stop condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/**************************************************************************/
/*!
    @brief  Writes 16-bits to the specified destination register
*/
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value) {
  uint16_t timeOut = 0xffff;
	
	/*!< Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);

	/*!< Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
		if(timeOut-- == 0)
		{
			//printf("err EV5\r\n");
		break;}
	}
	/*!< Send EEPROM address for write */
	i2cAddress = 0x90;
	I2C_SendData(I2C1, i2cAddress);
	/*!< Test on EV6 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	//while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if(timeOut-- == 0)
		{
			//printf("err ev6\r\n");
		break;}
	}

	/*!< Send the EEPROM's internal address to write to : LSB of the address */	
	I2C_SendData(I2C1, (uint8_t)(reg));//8 bit sau
	/*!< Test on EV8 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{
			//printf("err ev8\r\n");
		break;}
	}
	
	I2C_SendData(I2C1, (uint8_t)(value>>8));
	
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{
			//printf("err ev8\r\n");
		break;}
	}
		
	I2C_SendData(I2C1, (uint8_t)(value & 0xFF));
	
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{
			//printf("err ev8\r\n");
		break;}
	}
	/* Gui Stop condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
}

/**************************************************************************/
/*!
    @brief  Writes 16-bits to the specified destination register
*/
/**************************************************************************/
uint16_t readRegister(uint8_t i2cAddress,uint8_t addReg)
{
	uint16_t timeOut = 0xffff;
	uint32_t temp=0;
	uint8_t tempValue1,tempValue2;
	uint16_t tempValue;
	/* Gui Start condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	/* cho time out */
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_SB) == RESET)
	{
		if(timeOut-- == 0)
			return 0;
	}
	/* Gui address slave de ghi */
	i2cAddress = ADS1x15_ADDR;
	I2C_SendData(I2C1, i2cAddress);
	timeOut = 0xffff;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if(timeOut-- == 0)
			return 0;
	}
		/* clear addr flag */
	(void) I2C1->SR2;
	I2C_SendData(I2C1, ADS1015_REG_POINTER_CONVERT);

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
		I2C_SendData(I2C1, ADS1x15_ADDR|1);
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
		
		tempValue1 = I2C_ReceiveData(I2C1);
		while ((I2C1->CR1&0x200) == 0x200);	
		tempValue2 = I2C_ReceiveData(I2C1);
		while ((I2C1->CR1&0x200) == 0x200);			
		/* Enable ACK cho luot nhan tiep theo */
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		tempValue = (tempValue1<<8) | tempValue2;
		return tempValue;
}



static void adsdelay_ms(__IO uint32_t num)
{
	__IO uint32_t index = 0;
	
	/* Default system clock is 72MHz */
	for(index = (9 * 720 * num); index != 0; index--);
}

uint8_t ADSwriteRegister(uint8_t reg, uint16_t value) {
	uint16_t timeOut;
	/*!< Send START condition */
	I2C_GenerateSTART(I2C1, ENABLE);
  /*!< Test on EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
  {
		if(timeOut-- == 0)
		{
			//printf("err EV5\r\n");
		return 0;}
	}
	/*!< Send EEPROM address for write */
	I2C_SendData(I2C1, ADS1x15_ADDR);
  /*!< Test on EV6 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	//while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if(timeOut-- == 0)
		{
			//printf("err ev6\r\n");
		return 0;}
	}
	/*!< Send the EEPROM's internal address to write to : LSB of the address */	
	I2C_SendData(I2C1, (uint8_t)(reg));//8 bit sau
		/*!< Test on EV8 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{
			//printf("err ev8\r\n");
		break;}
	}
	I2C_SendData(I2C1, (uint8_t)(value>>8));
		/*!< Test on EV8 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{
			//printf("err ev8\r\n");
		return 0;}
	}
		
	I2C_SendData(I2C1, (uint8_t)(value & 0xFF));
		/*!< Test on EV8 and clear it */
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{
			//printf("err ev8\r\n");
		return 0;}
	}
	timeOut = 0xffff;
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
		if(timeOut-- == 0)
		{
			//printf("err ev8\r\n");
		return 0;}
	}
	/* Gui Stop condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
	return 1;
}

uint16_t ADSreadRegister(uint8_t i2cAddress,uint8_t addReg)
{
	uint16_t timeOut = 0xffff;
	uint32_t temp=0;
	uint8_t tempValue1,tempValue2;
	uint16_t tempValue;
	/* Gui Start condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	/* cho time out */
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_SB) == RESET)
	{
		if(timeOut-- == 0)
			return 0;
	}
	/* Gui address slave de ghi */
	i2cAddress = 0x90;
	I2C_SendData(I2C1, i2cAddress);
	timeOut = 0xffff;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if(timeOut-- == 0)
			return 0;
	}
		/* clear addr flag */
	(void) I2C1->SR2;
	I2C_SendData(I2C1, ADS1015_REG_POINTER_CONVERT);

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
		I2C_SendData(I2C1, 0x91);
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
		
		tempValue1 = I2C_ReceiveData(I2C1);
		while ((I2C1->CR1&0x200) == 0x200);	
		tempValue2 = I2C_ReceiveData(I2C1);
		while ((I2C1->CR1&0x200) == 0x200);			
		/* Enable ACK cho luot nhan tiep theo */
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		tempValue = (tempValue1<<8) | tempValue2;
		return tempValue;
}
/**************************************************************************/
/*!
    @brief  Gets a single-ended ADC reading from the specified channel
*/
/**************************************************************************/
uint16_t readADC_SingleEnded(uint8_t channel) {
	uint16_t config;
  if (channel > 3)
  {
    return 0;
  }
  
  // Start with default values
  config          = ADS1015_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS   | // 1600 samples per second (default)
                    ADS1015_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

  // Set PGA/voltage range
  config |= m_gain;
  //printf("channel:%d,config:%d\r\n",channel,config);
  // Set single-ended input channel
  switch (channel)
  {
    case (0):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
      break;
  }

  // Set 'start single-conversion' bit
  config |= ADS1015_REG_CONFIG_OS_SINGLE;
  //printf("channel:%d,config:%d\r\n",channel,config);
  // Write config register to the ADC
  //writeRegister(m_i2cAddress, ADS1015_REG_POINTER_CONFIG, config);
  ADSwriteRegister(ADS1015_REG_POINTER_CONFIG,config);
  // Wait for the conversion to complete
  adsdelay_ms(30);
  
  // Read the conversion results
  // Shift 12-bit results right 4 bits for the ADS1015
  return readRegister(m_i2cAddress, ADS1015_REG_POINTER_CONVERT) >> m_bitShift;  
}
