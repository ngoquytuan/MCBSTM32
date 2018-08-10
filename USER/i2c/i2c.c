#include "i2c.h"
#include "delay.h"
/*I2C_Config*/
/** 
  * @Author Tuan - 19/10/2014 
	* @brief  I2C_Config : Turn on RCC, REMAP, GPIO, config I2C
  * @param  None
  * @retval None
  */
uint8_t I2C_Config(void)
{
	uint16_t timeOut = 0xffff;
	GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
	/* Remap of I2C (no need this case)*/
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_I2C1,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	/* Thiet lap GPIO */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//PB6 is SCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;// PB7 is SDA
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* reset sate */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);
	/* Release I2C1 from reset state */
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE);
  /* Thiet lap che chay cho I2C */
	I2C_InitStructure.I2C_ClockSpeed = 300000;
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStructure);
	I2C_Cmd(I2C1,ENABLE);
	while (I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY)) // Wait until I2C free
	{
		if(timeOut-- == 0) {
			#ifdef DEBUG
			printf("I2C ERR\r\n");
			#endif
			return 0;
		}
	}
	#ifdef DEBUG
	printf("I2C1 init done!\r\n");
	#endif
	return 1;
}



/*I2C_ReadReg*/
/** 
  * @Author Tuan - 12/Jul/2018 
  * @brief  I2C_ReadReg
  * @param  None
  * @retval None
  */
uint8_t I2C_ReadReg(uint8_t deviceaddr,uint8_t addReg)
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
	I2C_SendData(I2C1, deviceaddr);
	timeOut = 0xffff;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if(timeOut-- == 0)
			return 0;
	}
		/* clear addr flag */
	temp = I2C1->SR2;
	
	I2C_SendData(I2C1, addReg);
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
		I2C_SendData(I2C1, deviceaddr+1);
		timeOut = 0xffff;
		while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
		{
			if(timeOut-- == 0)
				return 0;
		}
		/* clear ack */
		I2C_AcknowledgeConfig(I2C1, DISABLE);
		/* clear addr flag */
		temp = I2C1->SR2;
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
  * @Author Tuan - 12/Jul/2018 
  * @brief  I2C_WriteReg
  * @param  None
  * @retval None
  */
uint8_t I2C_WriteReg(uint8_t deviceaddr,uint8_t addReg, uint8_t Data)
{
	uint16_t timeOut = 0xffff;
	uint32_t temp =0;
	temp = timeOut-timeOut;
	/* Gui Start condition */
	I2C_GenerateSTART(I2C1, ENABLE);
	/* cho time out */
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_SB) == RESET)
	{
		if(timeOut-- == 0)
		return 0;
	}
	/* Gui address slave de ghi */
	I2C_SendData(I2C1, deviceaddr);
	timeOut = 0xffff;
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_ADDR) == RESET)
	{
		if(timeOut-- == 0)
		return 0;
	}
		/* clear addr flag */
	temp = I2C1->SR2;
	I2C_SendData(I2C1, addReg);
		/* clear addr flag */
	temp = I2C1->SR2;
	I2C_SendData(I2C1, Data);
	/* EV8_2: Wait until BTF is set before programming the STOP */
	while ((I2C1->SR1 & 0x00004) != 0x000004);
	/* Gui Stop condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
	return 1;
}



