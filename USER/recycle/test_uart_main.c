/*******************************************************
This program was created by swat

Project : Test on Open CMX
Version : 1
Date    : 1/14/2017
Author  : swat
Company : None
Comments: 


Chip type               : STM32F103RDT6
Program type            : Application
External Clock frequency: 8.000000 MHz
*******************************************************/
//Inclue USE_STDPERIPH_DRIVER, STM32F10X_HD
//#define HSE_VALUE    ((uint32_t)4332000)
#include <stdio.h>
//#include <stdlib.h>
#include "GPIO_STM32F10x.h"
#include "test_uart_user.h"
#include "main.h"
#include "delay.h"
#include "lcd16x2_tuan.h"
//#include "eeprom_stm.h"
//#include "RDS_Groups_Gen.h"
#define LED(state) GPIO_PinWrite(GPIOA, 11, state);
RCC_ClocksTypeDef abc;
//DAC_InitTypeDef  DAC_InitStructure;
#include "tm1637.h"








/******************RTC***********************************************/
#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */
__IO uint32_t TimeDisplay = 0;
void RTC_Configuration(void);
void NVIC_Configuration(void);
uint32_t Time_Regulate(uint32_t Tmp_HH,uint32_t Tmp_MM,uint32_t Tmp_SS);
void Time_Adjust(void);
void Time_Show(void);
void Time_Display(uint32_t TimeVar);
uint8_t USART_Scanf(uint32_t value);
#define ledtoggle(x) GPIOB->ODR ^= x
uint8_t ledbit = 1;
/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
  if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
  {
    /* Clear the RTC Second interrupt */
    RTC_ClearITPendingBit(RTC_IT_SEC);
    /* Toggle LED1 */
		ledbit = !ledbit;
    GPIO_PinWrite(GPIOB, 8, ledbit);

    /* Enable time update */
    TimeDisplay = 1;

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    
  }
}

/******************RTC***********************************************/







///* Private typedef -----------------------------------------------------------*/
///* Private define ------------------------------------------------------------*/
///* Private macro -------------------------------------------------------------*/
///* Private variables ---------------------------------------------------------*/
//ErrorStatus  HSEStartUpStatus;
//FLASH_Status FlashStatus;
////uint16_t VarValue = 0;
///* Virtual address defined by the user: 0xFFFF value is prohibited */
//uint16_t VirtAddVarTab[NumbOfVar] = {0x5555, 0x6666, 0x7777};
//uint16_t eeprom_temp;

//GPIO_InitTypeDef GPIO_InitStruct;

#define ONTIME        1
#define STOP          0

uint32_t test1=0,t1=0,t2=0,bt_scan=0,tx1_count;
uint8_t UART_DATA[20];
uint8_t UART1_DATA[20];
char checku2=0,checku1=0;

#define ENABLE       1
#define DISABLE      0
char slave_debug = ENABLE;

#define MASTER       'M'
#define SLAVE        'S'
char typeOfdevice = MASTER; //'M' is master, 'S' is slave
char idOfSlave = 2;
const char totalOfSlave = 2;// => slave 0, slave 1, slave 2
char CurrentSlave = 0;
#define LOST_CON        1
#define NOMA_CON        2
char slave_com[totalOfSlave];// for checking communication between master and slave
char slave_com_timeout[totalOfSlave];
#define ANG_ERR        1
#define ANG_OK         2
char slave_error[totalOfSlave];// for checking slave's angle error
void slave_stt_init(void)
{
	char i;
	for(i=0;i<totalOfSlave;i++)
	{
		slave_com[i] = LOST_CON;
		slave_error[i] = ANG_ERR;
		slave_com_timeout[i] = 5;
	}
}
uint32_t time_to_check_slave = 5;
uint32_t time_to_check_sensor =5;//5s check sensor
uint32_t time_to_update_7seg = 0;
uint32_t flag_check_7seg = 0;
unsigned char HEXInStringToDec(unsigned char data)
      {
            if((data>47)&&(data<58)) return (data-48);// 0...9
            else if((data>63)&&(data<71)) return (data-55);//A..F
						else return 0;
      }
//printf("S>%03X-%03X-%02X-%02X-%02X<  ",rawx,rawy,buff_temp1,buff_temp2,lux);			
int rawx,rawy,offsetrawx,offsetrawy;//SCA100
int deltax,deltay,abs_deltax,abs_deltay;
unsigned char buff_temp1,buff_temp2;//ds18b20
unsigned char lux;//cambienquang			
float temp_ds1820;			
uint8_t check=0;
uint32_t cnt; 
#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80
			
const char segmentMap2[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, // 0-7
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, // 8-9, A-F
    0x00
};
//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D

//
//      0
//     ---
//  5 |   | 1
//     -6-
//  4 |   | 2
//     ---
//      3   7<---dot

//const uint8_t dig[] = {0x3f,0x06};
	
const uint8_t digitToSegment[10] = {
 // XGFEDCBA
  0x3f,//0b00111111,    // 0
  0x06,//0b00000110,    // 1
  0x5b,//0b01011011,    // 2
  0x4f,//0b01001111,    // 3
  0x66,//0b01100110,    // 4
  0x6d,//0b01101101,    // 5
  0x7d,//0b01111101,    // 6
  0x07,//0b00000111,    // 7
  0x7f,//0b01111111,    // 8
  0x6f,//0b01101111,    // 9
  };

	// Module connection pins (Digital Pins) , GPIOB
#define CLK GPIO_PIN_12
#define DIO GPIO_PIN_13


	
	
void tm1637Init(void);
void tm1637DisplayDecimal(int v, int displaySeparator);
void tm1637SetBrightness(char brightness);
void TM1637_Disp_Dot(unsigned char seg1,unsigned char seg2,unsigned char seg3,unsigned char seg4);
unsigned char seg_temp1,seg_temp2,seg_temp3,seg_temp4;	

uint8_t time_out_lost_signal = 0;// = 0 is lost sensor 
void find_data(void)
{
	uint8_t temp1,temp2,temp3;
	

	if((UART_DATA[0] == SLAVE) &&(UART_DATA[18] == '<'))
	{
		time_out_lost_signal = 5;//<--------------------------------------------------------------
		
		temp1 = HEXInStringToDec(UART_DATA[2]);
		temp2 = HEXInStringToDec(UART_DATA[3]);
		temp3 = HEXInStringToDec(UART_DATA[4]);
		rawx = (temp1<<8) + (temp2<<4) + temp3;

		temp1 = HEXInStringToDec(UART_DATA[6]);
		temp2 = HEXInStringToDec(UART_DATA[7]);
		temp3 = HEXInStringToDec(UART_DATA[8]);
		rawy = (temp1<<8) + (temp2<<4) + temp3;

		temp1 = HEXInStringToDec(UART_DATA[10]);
		temp2 = HEXInStringToDec(UART_DATA[11]);
		buff_temp1 = (temp1<<4) + temp2;

		temp1 = HEXInStringToDec(UART_DATA[13]);
		temp2 = HEXInStringToDec(UART_DATA[14]);
		buff_temp2 = (temp1<<4) + temp2;

		temp1 = HEXInStringToDec(UART_DATA[16]);
		temp2 = HEXInStringToDec(UART_DATA[17]);
		lux = (temp1<<4) + temp2;
		
		temp_ds1820 = ((float)(buff_temp1&0x0f))/16;            // Lay phan thuc cua gia tri nhiet 
		buff_temp1 =((buff_temp1&0xf0)>>4)|((buff_temp2&0x0f)<<4) ;    // Lay phan nguyen cua gia tri nhiet do
		
		temp_ds1820=temp_ds1820+buff_temp1;
	}

			
	
}
char one_of_slave_err = 1;// 1 means contain an error

#define NO_ERR     0  
#define ERR_SENSOR 1
#define ERR_ANGLE  2
#define ERR_COMMUN 3
#define ERR_SLAVE  4
char error = NO_ERR;
void check_angle_err(void)
{
	if((offsetrawx - rawx) > 7 )      error = ERR_ANGLE;// goc x nho hon -0.25 do
	else if((rawx - offsetrawx) > 14) error = ERR_ANGLE; //goc x lon hon 0.5 do
	else if((offsetrawy - rawy) > 7)  error = ERR_ANGLE; //goc y nho hon -0.25 do
	else if((rawy - offsetrawy) > 14) error = ERR_ANGLE; //goc y lon hon 0.5 do
	//else return NO_ERR;
}
//__IO uint8_t ubSelectedWavesForm = 1;
//__IO uint8_t ubKeyPressed = SET; 

//Doan nay chi fix data chuan theo 4 group 0B de test
//			; PI 1234
//			; PS swat
//			; RT Chan
//			; PTY 18 Children’s Progs
//			; Group 0B segment 0
//			; 1234 06a 0a4c 310 1234 3c6 7377 24a
//			; 12 34 1a 82 93 31 01 23 4f 19 cd de 4a 
//char group0Bseg0[13] = {0x12,0x34,0x1a,0x82,0x93,0x31,0x01,0x23,0x4f,0x19,0xcd,0xde,0x4A};
//			; Group 0B segment 1
//			; 1234 06a 0a49 1f4 1234 3c6 6174 232
//			; 12 34 1a 82 92 5f 41 23 4f 19 85 d2 32 
//char group0Bseg1[13] = {0x12,0x34,0x1a,0x82,0x92,0x5f,0x41,0x23,0x4f,0x19,0x85,0xd2,0x32};
//			; Group 0B segment 2
//			; 1234 06a 0a4a 33f 1234 3c6 2020 0dc
//			; 12 34 1a 82 92 b3 f1 23 4f 18 80 80 dc 
//char group0Bseg2[13] = {0x12,0x34,0x1a,0x82,0x92,0xb3,0xf1,0x23,0x4f,0x18,0x80,0x80,0xdc};
//			; Group 0B segment 3
//			; 1234 06a 0a4b 286 1234 3c6 2020 0dc
//			; 12 34 1a 82 92 e8 61 23 4f 18 80 80 dc 
//char group0Bseg3[13] = {0x12,0x34,0x1a,0x82,0x92,0xe8,0x61,0x23,0x4f,0x18,0x80,0x80,0xdc};

//char diff_group0Bseg0[13],diff_group0Bseg1[13],diff_group0Bseg2[13],diff_group0Bseg3[13];
//extern uint16_t checkCurrentPICode;
//extern unsigned char  RDS_BufferGroup0A_1[RDS_SIZE_FRAME];    /* first  0A buffer group  */
//extern unsigned char  RDS_BufferGroup0A_2[RDS_SIZE_FRAME];    /* second 0A buffer group  */
//extern unsigned char  RDS_BufferGroup0A_3[RDS_SIZE_FRAME];    /* third  0A buffer group  */
//extern unsigned char  RDS_BufferGroup0A_4[RDS_SIZE_FRAME];    /* fourth 0A buffer group  */
//extern unsigned char CurrentPSName[RDS_SIZE_PSNAME];

//unsigned char input[13]={0x12,0x34,0x1a,0x82,0x93,0x31,0x01,0x23,0x4f,0x19,0xcd,0xde,0x4A};
//unsigned char output[13];
/*
void convert(unsigned char* arr_in,unsigned char* arr_out)
{
char index1,index2;
char pre_out,cur_out;
pre_out=0;
	for(index1=0;index1<13;index1++)
	{
		arr_out[index1]=0;
	}
    for(index2=0;index2<13;index2++)
    {
        for(index1=0;index1<8;index1++)
        {
        cur_out=(((arr_in[index2]&(1<<(7-index1)))>>(7-index1))^ pre_out);
        arr_out[index2]=arr_out[index2]+(cur_out<<(7-index1));
        pre_out=cur_out;
        }
    }
		//printf("\r\n%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x",arr_in[0],arr_in[1],arr_in[2],arr_in[3],arr_in[4],arr_in[5],arr_in[6],arr_in[7],arr_in[8],arr_in[9],arr_in[10],arr_in[11],arr_in[12]);
		//printf("\r\n%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\r\n",output[0],output[1],output[2],output[3],output[4],output[5],output[6],output[7],output[8],output[9],output[10],output[11],output[12]);
}
*/
//char numOfMess=0,timesOfSend=0;
//void nextMess(void)
//{
//	
//	if(numOfMess==0) 			{/*printf("Change message to 0\r\n");*/convert(RDS_BufferGroup0A_1,output);}
//	else if(numOfMess==1) {/*printf("Change message to 1\r\n");*/convert(RDS_BufferGroup0A_2,output);}
//	else if(numOfMess==2) {/*printf("Change message to 2\r\n");*/convert(RDS_BufferGroup0A_3,output);}
//	else if(numOfMess==3) {/*printf("Change message to 3\r\n");*/convert(RDS_BufferGroup0A_4,output);}
//	numOfMess++;
//	if(numOfMess>3) numOfMess=0;
//}

// USART Receiver buffer
#define RX_BUFFER_SIZE0 20


uint16_t USART1_Time_Out; 
uint8_t USART1_index=0,USART1_rx_data_buff[RX_BUFFER_SIZE0];

uint8_t USART1_Time_Out_Over, USART1_Time_Out_Running,USART1_process;
void USART1_IRQHandler(void)
{
	char USART1_data;
	/* RXNE handler */
  if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART1_data=(USART1->DR & (uint16_t)0x01FF);
		USART1_Time_Out=50;// 50ms
		USART1_Time_Out_Over=0;
		USART1_Time_Out_Running=1;
		USART1_rx_data_buff[USART1_index++]=USART1_data;

		if(USART1_index==RX_BUFFER_SIZE0) USART1_index=0;
			
	}

}
void TIM2_IRQHandler(void)
{
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			//Timeout for ???
			testwd++;
			one_more_count++;
			if(countToChangeFreq) countToChangeFreq++;
			//for USART1 rec time out
			if(USART1_Time_Out_Running==1)
			{
				USART1_Time_Out--;
				if(USART1_Time_Out==0) USART1_Time_Out_Over=1;
			}
		}
}

char current_bit=0;
char next_bit;
uint32_t new_address=0;
void DMA2_Channel4_5_IRQHandler(void)
{
		if(DMA_GetITStatus(DMA2_IT_TC4) == SET) 
		{
				DMA_ClearITPendingBit(DMA2_IT_TC4);
				DMA2_Channel4->CMAR = new_address;
				current_bit++;

		}
}



//void WWDG_IRQHandler (void)
//{

//if (WWDG_GetFlagStatus ())
//  {
//  WWDG_SetCounter (0x40);
//  WWDG_ClearFlag ();
//  }

//}

int main(void)
{
	//char ibits,ibytes;
	//char pre_bit,cur_bit,nex_bit;
	SystemInit();
	SystemCoreClockUpdate();
	
	/* DMA1 clock enable */
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* GPIOA clock enable (to be used with DAC) */
  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                         
  /* DAC Periph clock enable */
  //RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	
	//WWDG_SetPrescaler (WWDG_Prescaler_8); // WWDG clock counter = (PCLK1(38.988MHz) / 4096) / 8 = 1.18kHz (~ 840 µs)
	
	 /* Set Window value to 80; WWDG counter should be refreshed only when the counter
    is below 80 (and greater than 64) otherwise a reset will be generated */
	//WWDG_SetWindowValue (80); // value must be less than 0x80
  /* Enable WWDG and set counter value to 127, WWDG timeout = ~780 us * 64 = 49.92 ms 
     In this case the refresh window is: 
           ~840 * (127-80) = 39.4ms < refresh window < ~840 * 64 = 53.8ms
  */
	
	
	
	//GPIO config
	GPIO_PortClock   (GPIOC , true);
	//TX1 RD485
	GPIO_PortClock   (GPIOB, true);
	GPIO_PinConfigure(GPIOB, 8, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT10MHZ);
	GPIO_PinWrite(GPIOB, 8, 1);
	//GPIO_PortWrite(GPIOB, 8,0);
	//RL1
	GPIO_PortClock   (GPIOB, true);
	GPIO_PinConfigure(GPIOB, 9, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT10MHZ);
	GPIO_PinWrite(GPIOB, 9, 0);
	//RL8
	//GPIO_PortClock   (GPIOA, true);
	GPIO_PinConfigure(GPIOA, 5, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT10MHZ);
	GPIO_PinWrite(GPIOA, 5, 0);
	USART1_Init();
	delay_ms(1);
	printf("Kiem tra uart 1\r\n");
	//check clock CPU
	RCC_GetClocksFreq(&abc);
	printf(">Thach anh: \r\nADCCLK:%d\r\nHCLK:%d\r\nPCLK1:%d\r\nPCLK2:%d\r\nSYSCLK:%d",abc.ADCCLK_Frequency,abc.HCLK_Frequency,abc.PCLK1_Frequency,abc.PCLK2_Frequency,abc.SYSCLK_Frequency);
	LCD_Init();
	LCD_Clear();
	LCD_Puts("Sometime it last");
	LCD_Gotoxy(1,0);
	LCD_Puts(" in love");
	
	/***********************RTC*********************************/
	/* NVIC configuration */
  NVIC_Configuration();
	  if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
  {
    /* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

    printf("\r\n\n RTC not yet configured....");

    /* RTC Configuration */
    RTC_Configuration();

    printf("\r\n RTC configured....");

    /* Adjust time by values entered by the user on the hyperterminal */
    Time_Adjust();

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\n\n Power On Reset occurred....");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\n\n External Reset occurred....");
    }

    printf("\r\n No need to configure RTC....");
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    
    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
  }
	
	#ifdef RTCClockOutput_Enable
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Disable the Tamper Pin */
  BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
                                 functionality must be disabled */

  /* Enable RTC Clock Output on Tamper Pin */
  BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
  #endif

  /* Clear reset flags */
  RCC_ClearFlag();
	//Time_Adjust();
  printf("\r\n Good");
  /* Display time in infinite loop */
  Time_Show();
	/***********************RTC*********************************/
	while(1)
	{
		GPIO_PinWrite(GPIOB, 8, 1);
		delay_ms(1000);
		GPIO_PinWrite(GPIOB, 8, 0);
		delay_ms(1000);
	}
	/*Load_data_from_EEPROM*/
/** 
  * @Author Tuan - 22/10/2014 
  * @brief  Load_data_from_EEPROM
  * @param  None
  * @retval None
  */
	  /* Unlock the Flash Program Erase controller */
  //FLASH_Unlock();

  /* EEPROM Init */
  //EE_Init();

/* --- Store successively many values of the three variables in the EEPROM ---*/

  //EE_WriteVariable(0,123 );
  

	//EE_ReadVariable(0,&eeprom_temp);
	//printf("eeprom:%d",eeprom_temp);
	
	/* initialise EEPROM mgt */
  //EEP_Init();

	
	//RDSGEN_Init();
	//printf("\r\n%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x",RDS_BufferGroup1[0],RDS_BufferGroup1[1],RDS_BufferGroup1[2],RDS_BufferGroup1[3],RDS_BufferGroup1[4],RDS_BufferGroup1[5],RDS_BufferGroup1[6],
	//RDS_BufferGroup1[7],RDS_BufferGroup1[8],RDS_BufferGroup1[9],RDS_BufferGroup1[10],RDS_BufferGroup1[11],RDS_BufferGroup1[12]);
	/*
	printf("\r\nGroup 0A 1:%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x",
	RDS_BufferGroup0A_1[0],RDS_BufferGroup0A_1[1],RDS_BufferGroup0A_1[2],RDS_BufferGroup0A_1[3],RDS_BufferGroup0A_1[4],
	RDS_BufferGroup0A_1[5],RDS_BufferGroup0A_1[6],RDS_BufferGroup0A_1[7],RDS_BufferGroup0A_1[8],RDS_BufferGroup0A_1[9],
	RDS_BufferGroup0A_1[10],RDS_BufferGroup0A_1[11],RDS_BufferGroup0A_1[12]);	
	printf("\r\nGroup 0A 2:%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x",
	RDS_BufferGroup0A_2[0],RDS_BufferGroup0A_2[1],RDS_BufferGroup0A_2[2],RDS_BufferGroup0A_2[3],RDS_BufferGroup0A_2[4],
	RDS_BufferGroup0A_2[5],RDS_BufferGroup0A_2[6],RDS_BufferGroup0A_2[7],RDS_BufferGroup0A_2[8],RDS_BufferGroup0A_2[9],
	RDS_BufferGroup0A_2[10],RDS_BufferGroup0A_2[11],RDS_BufferGroup0A_2[12]);
	printf("\r\nGroup 0A 3:%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x",
	RDS_BufferGroup0A_3[0],RDS_BufferGroup0A_3[1],RDS_BufferGroup0A_3[2],RDS_BufferGroup0A_3[3],RDS_BufferGroup0A_3[4],
	RDS_BufferGroup0A_3[5],RDS_BufferGroup0A_3[6],RDS_BufferGroup0A_3[7],RDS_BufferGroup0A_3[8],RDS_BufferGroup0A_3[9],
	RDS_BufferGroup0A_3[10],RDS_BufferGroup0A_3[11],RDS_BufferGroup0A_3[12]);
	printf("\r\nGroup 0A 4:%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x",
	RDS_BufferGroup0A_4[0],RDS_BufferGroup0A_4[1],RDS_BufferGroup0A_4[2],RDS_BufferGroup0A_4[3],RDS_BufferGroup0A_4[4],
	RDS_BufferGroup0A_4[5],RDS_BufferGroup0A_4[6],RDS_BufferGroup0A_4[7],RDS_BufferGroup0A_4[8],RDS_BufferGroup0A_4[9],
	RDS_BufferGroup0A_4[10],RDS_BufferGroup0A_4[11],RDS_BufferGroup0A_4[12]);
	*/
	//convert(group0Bseg0,output);
	//convert(RDS_BufferGroup0A_1,output);
	//numOfMess=1;
	//printf("\r\n%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x",input[0],input[1],input[2],input[3],input[4],input[5],input[6],input[7],input[8],input[9],input[10],input[11],input[12]);
	//printf("\r\n%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\r\n",output[0],output[1],output[2],output[3],output[4],output[5],output[6],output[7],output[8],output[9],output[10],output[11],output[12]);
	//kiem tra dong thu 2 nay xem co dung la 0xfe 0x11 0x8e 0x7c 0xe2 0x21 0xff 0xe1 0x13 0xc6 0x06 0x0f 0x73
	//convert(input,output);
	//printf("\r\n%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x",input[0],input[1],input[2],input[3],input[4],input[5],input[6],input[7],input[8],input[9],input[10],input[11],input[12]);
	//printf("\r\n%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\r\n",output[0],output[1],output[2],output[3],output[4],output[5],output[6],output[7],output[8],output[9],output[10],output[11],output[12]);
	//DMA_NVIC_Config();
	//current_bit=0;
	//DMA2_Channel4->CMAR = (uint32_t)&A000; 
	

	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE); 
	//WWDG_Enable (0x7F); 
	//shutdown DAC
	//shutdown=1;
	while(1)
		{
			
			/* Update WWDG counter */
			WWDG_SetCounter(127);
			/*
			if(next_bit==current_bit)
			{
				next_bit++;
				if(next_bit==104) next_bit=0;
				
				ibytes= current_bit/8;
				ibits = current_bit%8;
				
					//if(i==104) {new_address=(uint32_t)&Nothing;printf(" END\r\n");}//ket thuc chuoi du lieu
					//else
				
						cur_bit = ((output[ibytes] & (1<<(7-ibits)))>>(7-ibits));
					
					if(ibits==0)// diem dau tien cua moi byte
					{
							if(ibytes==0)// diem dau tien cua chuoi
							{
									pre_bit = output[12]&1;// diem cuoi cung
							}
							else if(ibytes != 0)
							{
								pre_bit = output[ibytes-1] & 1;
							}
							 
					}
					else if (ibits!=0) pre_bit = ((output[ibytes] & (1<<(8-ibits)))>>(8-ibits));
					
					if(ibits == 7)//diem cuoi cua moi byte
					{
						if(ibytes == 12) //byte cuoi cung, bit cuoi cung
						{
							nex_bit = (output[0]&(1<<7))>>7;//diem dau tien cua chuoi
						}
						else if(ibytes != 12)
						{
							nex_bit = (output[ibytes+1]&(1<<7))>>7;
						}
					}
					else if(ibits != 7) nex_bit = ((output[ibytes] & (1<<(6-ibits)))>>(6-ibits));
					//process bits
								 if((pre_bit==0)&&(cur_bit==0)&&(nex_bit==0)) 			{new_address=(uint32_t)&A000;}//printf("A");}
								 else if((pre_bit==1)&&(cur_bit==1)&&(nex_bit==1))	{new_address=(uint32_t)&H111;}//printf("H");}
								 else if((pre_bit==1)&&(cur_bit==1)&&(nex_bit==0)) 	{new_address=(uint32_t)&G110;}//printf("G");}
								 else if((pre_bit==0)&&(cur_bit==0)&&(nex_bit==1))  {new_address=(uint32_t)&B001;}//printf("B");}
								 else if((pre_bit==1)&&(cur_bit==0)&&(nex_bit==1)) 	{new_address=(uint32_t)&F101;}//printf("F");}
								 else if((pre_bit==0)&&(cur_bit==1)&&(nex_bit==0))  {new_address=(uint32_t)&C010;}//printf("C");}
								 else if((pre_bit==1)&&(cur_bit==0)&&(nex_bit==0))  {new_address=(uint32_t)&E100;}//printf("E");}
								 else if((pre_bit==0)&&(cur_bit==1)&&(nex_bit==1))  {new_address=(uint32_t)&D011;}//printf("D");}
							

						
					
					
				
			}*/
			
			
			//Allow process USART data buffer			
			if(USART1_Time_Out_Over==1) 
				{
					USART1_Time_Out_Running=0;
					USART1_Time_Out_Over=0;
					USART1_process=1;
				}
				// Each message will be transmit 'totalTimeOfSend' times, so total 4 groups will x 'totalTimeOfSend' 
			if(mess_count >200) shutdown = 1;
				else if(mess_count > (4*totalTimeOfSend))
				{
					
					//if(send_done == 0) printf(">SENT0x%02X<\r\n",(uint8_t)(checkCurrentPICode&0xFF));
					//send_done =1;
					//toggle led
					LED(1);
				}
			else if(mess_count>1) { LED(0); }

		//Shutdown encoder
		if(one_more_count>5000){
			one_more_count=0;			
			//if(shutdown == 1) DMA_Cmd(DMA2_Channel4, DISABLE);
			//else if(shutdown == 0) DMA_Cmd(DMA2_Channel4, ENABLE);
		}
		//Enable this if you want to check WWDG 
		/*
		if(testwd>0x2F00)
		{
			testwd=0;
			printf("Ahihi do cho\r\n");
			while(1);
		}
		*/
		/*
		if(countToChangeFreq > 10000) // stop send cmd 
		{
			countToChangeFreq = 0;//stop couter
			CurrentPSName[0] = 'R';
			CurrentPSName[1] = 'D';
			CurrentPSName[2] = 'S';
			CurrentPSName[3] = ' ';
			CurrentPSName[4] = 'G';
			CurrentPSName[5] = 'O';
			CurrentPSName[6] = 'o';
			CurrentPSName[7] = 'D';
			printf(">SENTCMD<\r\n");
			RDSGEN_Init();
		}*/
		//Process the message from MCU_ENCODE
		
			if(USART1_process==1)
					{
						//Transmit RDS with ID : >ON_0x01<   0x01 is ID
						/*
						if((USART1_rx_data_buff[0]=='>')&&(USART1_rx_data_buff[1]=='O')&&(USART1_rx_data_buff[2]=='N')&&(USART1_rx_data_buff[3]=='_')&&(USART1_rx_data_buff[8]=='<'))   
						{
							//>ON_0x01<
							//Get the ID
							if(shutdown ==1) {shutdown=0;DMA_Cmd(DMA2_Channel4, ENABLE);}//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< call turn on dac
							current_ID =(HexInCharToDec(USART1_rx_data_buff[7])+(HexInCharToDec(USART1_rx_data_buff[6])<<4));
							RDSGEN_Init();
							mess_count = 0;
							send_done = 0;
							LED(0);
							//printf("checkCurrentPICode : %x\r\n",checkCurrentPICode);//>ON_0x01<
							//printf("current_ID : %d\r\n",current_ID);//>ON_0x01<
						}
						else if((USART1_rx_data_buff[0]=='>')&&(USART1_rx_data_buff[1]=='O')&&(USART1_rx_data_buff[2]=='F')&&(USART1_rx_data_buff[3]=='F')&&(USART1_rx_data_buff[8]=='<'))   
						{
							//if received message >OFF_ALL< >>> turn off the encoder
							//one_more_count = 5000;
							shutdown=1;
							DMA_Cmd(DMA2_Channel4, DISABLE);
						}
						  //Transmit data control over PS field
							//PS[8] == [>][Frequency][frequency][volume][threshold][Not use][<][CRC8]
							//New frequency = Frequency.frequency 
							//check for new control command ctrlCmd
						else if(crc8(USART1_rx_data_buff,7) == USART1_rx_data_buff[7])
						{
									CurrentPSName[0] = USART1_rx_data_buff[0];//0x3E;//>
									CurrentPSName[1] = USART1_rx_data_buff[1];//0x39;//F
									CurrentPSName[2] = USART1_rx_data_buff[2];//0x07;//f
									CurrentPSName[3] = USART1_rx_data_buff[3];//0x06;//v
									CurrentPSName[4] = USART1_rx_data_buff[4];//0x23;//thres
									CurrentPSName[5] = USART1_rx_data_buff[5];//0x54;//not use
									CurrentPSName[6] = USART1_rx_data_buff[6];//0x3C//'<';
									CurrentPSName[7] = USART1_rx_data_buff[7];//0x37;//crc
									printf("%s",CurrentPSName);
									RDSGEN_Init();
									//started couting...
									countToChangeFreq = 1;
									//Cmd to change frequency reciver, transmit this message in 2 minutes then come back PS
									//printf("Control cmd\r\n");
						}
						else if(crc8(USART1_rx_data_buff,8) == USART1_rx_data_buff[9]) // change new PS
							{
									CurrentPSName[0] = USART1_rx_data_buff[0];
									CurrentPSName[1] = USART1_rx_data_buff[1];
									CurrentPSName[2] = USART1_rx_data_buff[2];
									CurrentPSName[3] = USART1_rx_data_buff[3];
									CurrentPSName[4] = USART1_rx_data_buff[4];
									CurrentPSName[5] = USART1_rx_data_buff[5];
									CurrentPSName[6] = USART1_rx_data_buff[6];
									CurrentPSName[7] = USART1_rx_data_buff[7];
									RDSGEN_Init();
							}*/
						//printf("%s",USART1_rx_data_buff);
						
						
						USART1_process=0;
						for(USART1_index=0;USART1_index<RX_BUFFER_SIZE0;USART1_index++)
                            {
                            USART1_rx_data_buff[USART1_index]=0;
                            }  
                            USART1_index=0;
					}

		
		}
		
}

//void DMA_config(void)
//{
//	DMA_InitTypeDef            DMA_InitStructure;
//	/* DMA1 clock enable (to be used with DAC) */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);       
//    /* DMA1 channel 1 configuration */
//          DMA_DeInit(DMA2_Channel4);// moi DMA se co 1 phan cung nhat dinh duy nhat

//          DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)DAC_DHR12R2_ADDRESS;// tim loi mat o trong datasheet roi cong tru lung tung moi ra
//          DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&A000;// cai mang dan don muon tao
//          DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
//          DMA_InitStructure.DMA_BufferSize = 304;// so phan tu cua mang 
//          DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//          DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//          DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//          DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//          DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//          DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//          DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//          DMA_Init(DMA2_Channel4, &DMA_InitStructure);
// 
//          /* Enable DMA1 Channel 1 */
//          DMA_Cmd(DMA2_Channel4, ENABLE);
//}
//void DMA_NVIC_Config(void)
//{
//	NVIC_InitTypeDef  NVIC_InitStructure;
//	//NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
//	
//	DMA_ITConfig(DMA2_Channel4,DMA_IT_TC,ENABLE);
//}
//void DAC_Config(void)
//	{
//     /* DAC Periph clock enable */
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
//                 
//          DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
//          DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;//ko hieu cai nay tao ra tan so bao nhieu
//          DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
//					DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_TriangleAmplitude_4095;
//                 
//          /* DAC Channel1 Init */
//          DAC_Init(DAC_Channel_1, &DAC_InitStructure);
//          
//                    /* Enable DAC Channel2 */
//          DAC_Cmd(DAC_Channel_1, ENABLE);
//           
//          /* Enable DMA for DAC Channel2 */
//          //DAC_DMACmd(DAC_Channel_1, ENABLE);//dma2channel3
//					//DAC_SetChannel1Data(DAC_Align_12b_R, 0xf00);
//		
//		DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
//          DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
//          DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
//					
//                 
//          /* DAC Channel1 Init */
//          DAC_Init(DAC_Channel_2, &DAC_InitStructure);
//          
//                    /* Enable DAC Channel2 */
//          DAC_Cmd(DAC_Channel_2, ENABLE);
//           
//          /* Enable DMA for DAC Channel2 */
//          DAC_DMACmd(DAC_Channel_2, ENABLE);
//					DAC_SetChannel2Data(DAC_Align_12b_R, 0x100);// tao gia tri ban dau
//   }
	

//CRC8 caculator swat 25.03.2017
uint8_t crc8(uint8_t *datainput,uint8_t datalength)
{
  uint16_t value,iscan;
  value = datainput[0];
  for(iscan=1;iscan<datalength;iscan++)
    {
      value += datainput[iscan];
      if(value > 0xFF) value -= 256;
      }
      return value;
  }
						
		
/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the RTC.
  * @param  None
  * @retval None
  */
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval Current time RTC counter value
  */
uint32_t Time_Regulate(uint32_t Tmp_HH,uint32_t Tmp_MM,uint32_t Tmp_SS)
{

  printf("\r\n==============Time Settings=====================================");
  printf("\r\n  Please Set Hours");


  printf(":  %d", Tmp_HH);
  printf("\r\n  Please Set Minutes");

  printf(":  %d", Tmp_MM);
  printf("\r\n  Please Set Seconds");

  printf(":  %d", Tmp_SS);

  /* Return the value to store in RTC counter register */
  return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
}

/**
  * @brief  Adjusts time.
  * @param  None
  * @retval None
  */
void Time_Adjust(void)
{
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter(Time_Regulate(11,11,0));
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/**
  * @brief  Displays the current time.
  * @param  TimeVar: RTC counter value.
  * @retval None
  */
void Time_Display(uint32_t TimeVar)
{
  uint32_t THH = 0, TMM = 0, TSS = 0;
  
  /* Reset RTC Counter when Time is 23:59:59 */
  if (RTC_GetCounter() == 0x0001517F)
  {
     RTC_SetCounter(0x0);
     /* Wait until last write operation on RTC registers has finished */
     RTC_WaitForLastTask();
  }
  
  /* Compute  hours */
  THH = TimeVar / 3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600) / 60;
  /* Compute seconds */
  TSS = (TimeVar % 3600) % 60;

  printf("Time: %0.2d:%0.2d:%0.2d\r\n", THH, TMM, TSS);
}

/**
  * @brief  Shows the current time (HH:MM:SS) on the Hyperterminal.
  * @param  None
  * @retval None
  */   
void Time_Show(void)
{
  

  /* Infinite loop */
  while (1)
  {
    /* If 1s has been elapsed */
    if (TimeDisplay == 1)
    {
      /* Display current time */
      Time_Display(RTC_GetCounter());
      TimeDisplay = 0;
    }

  }
}




/**
  * @brief  Gets numeric values from the hyperterminal.
  * @param  None
  * @retval None
  */
uint8_t USART_Scanf(uint32_t value)
{
  uint32_t index = 0;
  uint32_t tmp[2] = {0, 0};

  while (index < 2)
  {

  }
  /* Calculate the Corresponding value */
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  /* Checks */
  if (index > value)
  {
    printf("\n\rPlease enter valid number between 0 and %d", value);
    return 0xFF;
  }
  return index;
}
	



