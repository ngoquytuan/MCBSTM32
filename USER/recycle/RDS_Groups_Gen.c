#include "stm32f10x.h"
#include "RDS_Groups_Gen.h"
//#include "eeprom_stm.h"
#include "test_uart_user.h"
//#include "RDS_EepromMgt.h"
//#include "RDS_IOMgt.h"


/* include system */
//#include <avr/io.h>
//#include <stdlib.h>
//#include <avr/pgmspace.h>   
//#include <avr/interrupt.h>

/* local constant */

/* static variable  */

/* static variable  */
//static  T_RDS_EEPROM_data  * pt_EEPROM_Data;  /* pointer on EEPROM data structure */

/*
There are 4 0A message to finish full 0A infomation
The 0A group
allow to transmit basic data (TA, M/S, DI, C1/C0) as well as Alternatives Frequency (AF) and the name of radio (PS Name)
TA: Traffic Annoucement : set to "1" if traffic data are being diffused, otherwise this flag is set to "0" .(see also description flag TP)
MS:Music / speech : If a musical emission is in course of transmission, the value "1" is set (default value if unused function), otherwise the value "0" indicate a speech broadcasting.
DI: 	Decoder Information: This bit allow to indicate to the receiver the type of transmission used, associated to 2 bits C1/C0 it is possible to transmit 4 bits data:
Alternative Frequency :
This word of 16bits allows to transmit alternative frequencies. These frequencies allow the receiver to switch to another transmitter diffusing the same radio in case of bad reception.
This function is very useful for a radio car because it allow to know quickly the other near transmitters and so to commute quickly when car moving .
The frequency is coded by step of 100KHz from 87.6MHz, a list of frequencies must be always preceded by the consequentive number of the list (from 1 to 25 frequencies)
1 ~ Frequency 87.6 MHz
...
204 ~ Frequency 107.9 MHz
*/
unsigned char  RDS_BufferGroup0A_1[RDS_SIZE_FRAME];    /* first  0A buffer group  */
unsigned char  RDS_BufferGroup0A_2[RDS_SIZE_FRAME];    /* second 0A buffer group  */
unsigned char  RDS_BufferGroup0A_3[RDS_SIZE_FRAME];    /* third  0A buffer group  */
unsigned char  RDS_BufferGroup0A_4[RDS_SIZE_FRAME];    /* fourth 0A buffer group  */

unsigned char  RDS_BufferGroup1[RDS_SIZE_FRAME];    /* first buffer group  */
static  unsigned char  RDS_BufferGroup2[RDS_SIZE_FRAME];    /* second buffer group */ 
static T_E_RDS_BUFF_STATE RDS_Buf1State;                    /* buffer 1 state */
static T_E_RDS_BUFF_STATE RDS_Buf2State;                    /* buffer 2 state */

static unsigned char PSNameIndex;                           /* current Index for PS Name */   

static unsigned char RTxtIndex;                             /* current Index for RadioText */   

static unsigned char AFIndex;                               /* current Index for AF */   
uint16_t checkCurrentPICode;
static uint16_t CurrentPICode;                              /* current PICode */
static uint16_t CurrentPICodeCRCBlock1;                     /* current CRC of PICode for bloc1 */


static unsigned char * ptCharToSend;                        /*  current index pointer of next char to send */


static unsigned char SelectGroup;                          /* selected group generate, used to select the next */

/* global data */
 unsigned char CurrentPSName[RDS_SIZE_PSNAME]="SWAT RDS";        /* current PS Name */
 unsigned char CurrentRTxt[RDS_SIZE_RADIOTEXT]="\r";       /* current RadioText ended by 0x0d '\r' , unused data should be set to \r for optimisation */
 unsigned char CurrentTxtAB;                          /* current Text A/B , each change clear line text */
 unsigned short CurrentTP;                            /* current TP */
 unsigned short CurrentPTY;                           /* current PTY */
 unsigned char CurrentTA;                             /* current TA */
 unsigned char CurrentMS;                             /* current MS */
 unsigned char CurrentDI;                             /* current DI */

uint8_t EEP_get_NbAF;
/* externam data */
extern unsigned char RDSDumpBuf;
extern uint8_t current_ID;

/*  */
void RDSGEN_Init(void)
{
    //unsigned char TmpIndex;
    /* initialise PSNAME */
//    for (TmpIndex=0; TmpIndex <RDS_SIZE_PSNAME; TmpIndex++)
//    {
//        CurrentPSName[TmpIndex] = EEP_get_PSName(TmpIndex);
//    }
    /* initialise Radiotext */
//    for (TmpIndex=0; TmpIndex <RDS_SIZE_RADIOTEXT; TmpIndex++)
//    {
//        CurrentRTxt[TmpIndex] = EEP_get_Radiotext(TmpIndex);
//    }
    
    /* initialise CurrentPICode for optimisation time process */
    //CurrentPICode=EEP_get_PICode();
    
		
    /* initialise CurrentDataBloc2 for optimisation time */
    /* TP as b10 */
    /* PTY as b5 to b9 */
    //CurrentTP = EEP_get_TP()  << 10 ; 
    //CurrentPTY= EEP_get_PTY() << 5  ;
    //CurrentTA = EEP_get_TA()  << 4  ;  /* it's not necessary to shift TA, because testing with hardware input */
    //CurrentMS = EEP_get_MS()  << 3  ;
    //CurrentDI = EEP_get_DI();    
			//; PI 1234
			//; PS SWAT RDS
			//; PTY 0 
//		CurrentPSName[0] = 0x3E;
//		CurrentPSName[1] = 0x39;
//		CurrentPSName[2] = 0x07;
//		CurrentPSName[3] = 0x06;
//		CurrentPSName[4] = 0x23;
//		CurrentPSName[5] = 0x54;
//		CurrentPSName[6] = '<';
//		CurrentPSName[7] = 0x37;
		
		CurrentPICode=0x1234;
		CurrentPICode = (CurrentPICode&0xFF00) + current_ID;
		checkCurrentPICode = CurrentPICode;
		/* initialise CurrentPICodeCRCBlock1 for optimisation process */
    CurrentPICodeCRCBlock1 = RDSGEN_ComputeCRC(CurrentPICode, RDS_OFFSET_A);
    CurrentTP = 0  << 10 ; 
    CurrentPTY= 0 << 5  ;
    CurrentTA = 0  << 4  ;  /* it's not necessary to shift TA, because testing with hardware input */
    CurrentMS = 0  << 3  ;
    
    RTxtIndex=0;
    PSNameIndex=0;
    //ptCharToSend=RDS_BufferGroup1;
    AFIndex=0;
    CurrentTxtAB=0;
    //fix DI|C1|C0
		CurrentDI = 0x04;//DI|C1|C0
		RDSGEN_Gen0A(RDS_BufferGroup0A_1);
		CurrentDI = 0x05;//DI|C1|C0
		RDSGEN_Gen0A(RDS_BufferGroup0A_2);
		CurrentDI = 0x06;//DI|C1|C0
		RDSGEN_Gen0A(RDS_BufferGroup0A_3);
		CurrentDI = 0x07;//DI|C1|C0
		RDSGEN_Gen0A(RDS_BufferGroup0A_4);
		
    /*  generate first and second buffer before start clock*/
    //RDSGEN_Gen0A(RDS_BufferGroup1);
    //RDS_Buf1State=E_RDS_BUF_STATE_IN_USED;
    //USART1_SendStr_(RDS_BufferGroup1);
		
    //RDSGEN_Gen0A(RDS_BufferGroup2);
    //RDS_Buf2State=E_RDS_BUF_STATE_READY;
    //USART1_SendStr_(RDS_BufferGroup2);
    //SelectGroup=0;
    

}

/* generating 0A group */                                
void RDSGEN_Gen0A(unsigned char BufGroup[RDS_SIZE_FRAME])
{
   unsigned char TempI;
   uint16_t TempData;
   uint16_t TempCRC;
   

   /* clear buffer */
   for (TempI=0;TempI<RDS_SIZE_FRAME;TempI++)
   {
       BufGroup[TempI]=0;
   }
   
   /* bloc1 */
   RDSGEN_SetBlock1(CurrentPICode,CurrentPICodeCRCBlock1,BufGroup) ; 

   /* Bloc 2 */
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |                                   TempData                    |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |b15|b14|b13|b12|b11|b10|b9 |b8 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |b3 |b2 |b1 |b0 |b0 |b0 |b4 |b3 |b2 |b1 |b0 |b0 |b0 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |      Group type   |TP |   PTY             |TA |MS |DI | PSName|
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
/* CurrentPTY , CurrentTP  and CurrentMS  previously shift left */

   TempData =  (RDS_GROUP_TYPE_0A<< 11 ) | CurrentTP | CurrentPTY | CurrentTA | CurrentMS | CurrentDI;  
 
/* No need this because I fix it above (in RDS_init)*/
//   TempData |= (PSNameIndex >>1);  /* PSNameIndex /2 */
//   switch( PSNameIndex >>1)
//   {
//   	case 0:
//   	     TempData |=(CurrentDI & 0x08) <<3;   /* select b3 bit and shift left */
//   		break;  
//   	case 1:
//   	     TempData |=(CurrentDI & 0x04) <<3;   /* select b2 bit and shift left */
//   		break;   		
//   	case 2:
//   	     TempData |=(CurrentDI & 0x02) <<3;   /* select b1 bit and shift left */
//   		break;  		
//   	case 3:
//   	     TempData |=(CurrentDI & 0x01) <<3;   /* select b0 bit and shift left */
//   		break;  
//   }

	 //TempData=0x0a49;
   TempCRC = RDSGEN_ComputeCRC(TempData,RDS_OFFSET_B);
   RDSGEN_SetBlock2(TempData,TempCRC,BufGroup); 

   /* Bloc 3 */
   
   /* AF code will be implemented in following version */
   if (EEP_get_NbAF == 0)
   {
      TempData =  RDS_AF_NOAF <<8 ;            /* NoAF code */
      TempData |= RDS_AF_FILLER_CODE ;         /* Filler code */
   }
   else
   {
        /* if begin of AF list, start with number of AF , followed by first AF */
        if (AFIndex == 0)
        {
            TempData = EEP_get_NbAF<<8;
            TempData |= get_AF(AFIndex++);
        }
        else
        {
            /* next AF */
            TempData = get_AF(AFIndex++) <<8;
            /* check if end of list , for filler code */
            if (AFIndex == EEP_get_NbAF)
            {
                TempData |= RDS_AF_FILLER_CODE ;        /* Filler code */
            }
        }
        /* test if End of AF list for restart */
        if (AFIndex == EEP_get_NbAF)
        {
            AFIndex = 0;        /* restart AF liste */
        }
   }
	 //TempData=0x1234;
   TempCRC = RDSGEN_ComputeCRC(TempData,RDS_OFFSET_C);
   RDSGEN_SetBlock3(TempData,TempCRC,BufGroup) ; 
   
   /* bloc 4 */
   TempData = (CurrentPSName[PSNameIndex++]) <<8;
   TempData |= (CurrentPSName[PSNameIndex++]);
   if (PSNameIndex >=RDS_SIZE_PSNAME)
   {
      PSNameIndex=0;
   }
	 //TempData=0x6174;
   TempCRC = RDSGEN_ComputeCRC(TempData,RDS_OFFSET_D);
   RDSGEN_SetBlock4(TempData,TempCRC,BufGroup); 
   /* debug */ 
//   RS_DebugPrintfStrPGM(PSTR("0A "));
}


/* generating 2A group */          
void RDSGEN_Gen2A(unsigned char BufGroup[RDS_SIZE_FRAME])
{
   unsigned char TempI;
   uint16_t TempData;
   uint16_t TempCRC;
   

   /* clear buffer */
   for (TempI=0;TempI<RDS_SIZE_FRAME;TempI++)
   {
       BufGroup[TempI]=0;
   }
   
   /* bloc1 */
   RDSGEN_SetBlock1(CurrentPICode,CurrentPICodeCRCBlock1,BufGroup) ; 

   /* Bloc 2 */
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |                                   TempData                    |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |b15|b14|b13|b12|b11|b10|b9 |b8 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |b3 |b2 |b1 |b0 |b0 |b0 |b4 |b3 |b2 |b1 |b0 |b0 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |   Group type  |B0 |TP |   PTY             |TAB|  TxtSeg       |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+

/* Every change of TextA/B clear line of reciever */
/* Line should be ended by '\r'(0x0D), for optimisation unused char in memory should be set to '\r' */
/* the new line char '\n' (0x0A) clear line of reciever */

/* CurrentPTY , CurrentTP  and CurrentMS  previously shift left */

   TempData = CurrentPTY  | (RDS_GROUP_TYPE_2A<< 11 ) | CurrentTP ; 
   if(CurrentTxtAB!=0)
   {
       TempData |= 0x10;
   }
   TempData |= (RTxtIndex>>2);   /*  radio text index /4, 4 char for each group send */ 
   TempCRC = RDSGEN_ComputeCRC(TempData,RDS_OFFSET_B);
   RDSGEN_SetBlock2(TempData,TempCRC,BufGroup); 
   
   /* block 3 */
   TempData = CurrentRTxt[RTxtIndex]<<8;
   RTxtIndex++;
   TempData |= CurrentRTxt[RTxtIndex];
   RTxtIndex++;
   TempCRC = RDSGEN_ComputeCRC(TempData,RDS_OFFSET_C);
   RDSGEN_SetBlock3(TempData,TempCRC,BufGroup); 
   
   /* block 4 */
   TempData = CurrentRTxt[RTxtIndex]<<8;
   RTxtIndex++;
   TempData |= CurrentRTxt[RTxtIndex];
   /* test end of line or end of array */
   /* unused char previously set to \r, so juste check las char of current group */
   if ((RTxtIndex==(RDS_SIZE_RADIOTEXT-1)) || (CurrentRTxt[RTxtIndex] == RDS_RTXT_ENDLINE) )
   {
      /* restart to begin line */
      RTxtIndex=0;
   }
   else
   {
      /* select next char */
      RTxtIndex++;
   }
   TempCRC = RDSGEN_ComputeCRC(TempData,RDS_OFFSET_D);
   RDSGEN_SetBlock4(TempData,TempCRC,BufGroup); 
   
}   


/* This function read next char and switch to second buffer if necessary */
/* function called by Interrupt, buffer state flag must be in critical section before writing it */

unsigned char RDSGEN_ReadNextData(void)
{
  unsigned char NextChar;
  
  
  /* read new char */
  NextChar=*ptCharToSend;
  /* inc ptCharToSend */
  ptCharToSend++;
  /* test if end of table */
  
  /* test buffer used */
  if (RDS_Buf1State ==E_RDS_BUF_STATE_IN_USED)
  {
      /* test end of buffer */
      if(ptCharToSend == &RDS_BufferGroup1[RDS_SIZE_FRAME])
      {
         /* if buffer2 is ready -> use it*/
         if (RDS_Buf2State ==E_RDS_BUF_STATE_READY)
         {
             ptCharToSend = &RDS_BufferGroup2[0];
             RDS_Buf2State= E_RDS_BUF_STATE_IN_USED;
             RDS_Buf1State= E_RDS_BUF_STATE_FREE;
         }
         else
         {
             /* error no buffer ready !!!!!*/
             /* resend last buffer to wait new data available */
             ptCharToSend = &RDS_BufferGroup1[0];
             //RS_PrintfStrPGM(PSTR("ERR:0 BUF\r\n"));
         }
      }
  }
  else
  {
     /* Buffer2 is used */
       /* test end of buffer */
      if(ptCharToSend == &RDS_BufferGroup2[RDS_SIZE_FRAME])
      {
         /* if buffer1 is ready -> use it*/
         if (RDS_Buf1State ==E_RDS_BUF_STATE_READY)
         {
             ptCharToSend = &RDS_BufferGroup1[0];
             RDS_Buf1State= E_RDS_BUF_STATE_IN_USED;
             RDS_Buf2State= E_RDS_BUF_STATE_FREE;
         }
         else
         {
             /* error no buffer ready !!!!!*/
             /* resend last buffer to wait new data available */
             ptCharToSend = &RDS_BufferGroup2[0];
             //RS_PrintfStrPGM(PSTR("ERR:0 BUF\r\n"));
         }
      } 
  }
  return (NextChar);
}


/* compute RDS CRC from 16b data and 10b offset g(x) = x10 + x8 + x7 + x5 + x4 + x3 + 1*/
uint16_t RDSGEN_ComputeCRC(uint16_t Data, uint16_t Offset)
{
// Word data used is a 16bits size 
// check word is a 10bits size
//
//   g(x) = x10 + x8 + x7 + x5 + x4 + x3 + 1
// 
//   +---------------+--------+--------+------------+--------+-------------^
//   |               |        |        |            |        |             |
//   +->b1->b2->b3->XOR->b4->XOR->b5->XOR->b6->b7->XOR->b8->XOR->b9->b10->XOR  +->Output
//                                                                         ^   |  MSB first
//                                                                         |   |
//   --------------------------------------------------------------------->+---+
/* local define */
#define RDS_SELECT_B0  0x0001
#define RDS_SELECT_B3  0x0008
#define RDS_SELECT_B4  0x0010
#define RDS_SELECT_B5  0x0020
#define RDS_SELECT_B7  0x0080
#define RDS_SELECT_B8  0x0100
#define RDS_SELECT_B10 0x0400
#define RDS_SELECT_B11 0x0800
#define RDS_SELECT_B15 0x8000
#define RDS_SELECT_10BITS 0x03FF
#define RDS_NBR_BCL 16
#define RDS_NBR_BCL_CTRL 10


   uint16_t ResultCRC=0;     /* current result , only first 10b used */
   unsigned char LoopIndex;  /* index for loop */
   
   /* main loop  of CRC processing */
   for (LoopIndex=0; LoopIndex<RDS_NBR_BCL; LoopIndex++)
   {
      /* logical shift left of Result CRC */
      ResultCRC=ResultCRC<<1;
      
      /*IF MSB of data = 1 */
      /* THEN */
      if ((Data & RDS_SELECT_B15) !=0)
      {
         /* Result = XOR with used bits b10 "g(x)=x10" */
         ResultCRC ^= RDS_SELECT_B10;
      /* ENDIF */
      }
      /* IF (b10 of result =1) */
      /* THEN */
      if ((ResultCRC & RDS_SELECT_B10)!=0)
      {
          /* Result = XOR with used bits in "x8+x7+x5+x4+x3+1" ---> (b8,b7,b5,b4,b3,b0) */
          ResultCRC ^= (RDS_SELECT_B8 | RDS_SELECT_B7 | RDS_SELECT_B5 | RDS_SELECT_B4 | RDS_SELECT_B3 | RDS_SELECT_B0);
      /* ENDIF */
      }
      /* logical shift left of input data */
      Data=Data<<1;
   } /* END loop */
   
   /* adding offset with XOR and reset unused bits */
   ResultCRC = (ResultCRC ^ Offset) & RDS_SELECT_10BITS;
   
   /*return result CRC */
   return(ResultCRC);
}




/* set data and checkword bock1 in buffer */
void RDSGEN_SetBlock1(uint16_t Data, uint16_t CheckWord, uint8_t Buffer[RDS_SIZE_FRAME])
{
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |         Buffer [0]            |            Buffer[1]          |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |b15|b14|b13|b12|b11|b10|b9 |b8 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |                              Data    block1                   |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+

// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |         Buffer [2]            |            Buffer[3]          |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |b9 |b8 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |                       |  
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |                Checkword   block1     |                       |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     Buffer[0]  = (uint8_t)(Data >>8);                    /* higher byte of data */
     Buffer[1]  = (uint8_t)Data;                          /* lower byte of data */
     Buffer[2]  = (uint8_t)(CheckWord>>2);                /* higher byte of checkword */
     Buffer[3] |= (uint8_t)(CheckWord<<6);                /* lower byte of checkword */ 
     
     
}

/* set data and checkword bock2 in buffer */
void RDSGEN_SetBlock2(uint16_t Data, uint16_t CheckWord, uint8_t Buffer[RDS_SIZE_FRAME])
{
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |         Buffer [3]            |            Buffer[4]          |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |       |b15|b14|b13|b12|b11|b10|b9 |b8 |b7 |b6 |b5 |b4 |b3 |b2 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |       |                      Data   block2                    |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+

// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |         Buffer [5]            |            Buffer[6]          |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |b1 |b0 |b9 |b8 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |               |                      |  
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |  Data |                Checkword  block2      |               |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     
     Buffer[3] |= (uint8_t) (Data>>10);
     Buffer[4]  = (uint8_t) (Data>>2);
     Buffer[5]  = (((uint8_t)Data)<<6) | ((uint8_t)(CheckWord>>4));
     Buffer[6] |= ((uint8_t)CheckWord)<<4;
     
 

}

/* set data and checkword bock3 in buffer */
void RDSGEN_SetBlock3(uint16_t Data, uint16_t CheckWord, uint8_t Buffer[RDS_SIZE_FRAME])
{
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |         Buffer [6]            |            Buffer[7]          |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |               |b15|b14|b13|b12|b11|b10|b9 |b8 |b7 |b6 |b5 |b4 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |               |              Data   block3                    |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+

// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |         Buffer [8]            |            Buffer[9]          |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |b3 |b2 |b1 |b0 |b9 |b8 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |       |                      |  
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |  Data  block3 |        Checkword  block3              |       |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
		 //CheckWord=0x3c6;
		 Buffer[6] |= (uint8_t) (Data>>12);
     Buffer[7]  = (uint8_t) (Data>>4);
     Buffer[8]  = (((uint8_t)Data)<<4) | ((uint8_t)(CheckWord >>6));
		 Buffer[9] |= ((uint8_t)CheckWord)<<2;


}

/* set data and checkword bock4 in buffer */
void RDSGEN_SetBlock4(uint16_t Data, uint16_t CheckWord, uint8_t Buffer[RDS_SIZE_FRAME])
{
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |         Buffer [9]            |            Buffer[10]         |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |                       |b15|b14|b13|b12|b11|b10|b9 |b8 |b7 |b6 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |                       |      Data   block4                    |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+

// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |         Buffer [11]           |            Buffer[12]         |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+                
// |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |b5 |b4 |b3 |b2 |b1 |b0 |b9 |b8 |b7 |b6 |b5 |b4 |b3 |b2 |b1 |b0 |                      |  
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
// |  Data  block4         |    Checkword  block4                  |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+

     
     
     Buffer[9] |= (uint8_t) (Data>>14); 
     Buffer[10] = (uint8_t) (Data>>6); 
     Buffer[11] = (((uint8_t)Data)<<2) | ((uint8_t)(CheckWord >>8)) ; /* higher bit of checkword */
     Buffer[12] = (uint8_t)CheckWord;                          /* lower byte of checkword */

}

/* Check buffer free and fill it if OK */
//void RDSGEN_CheckFreeBuf(void)
//{
//   /* temporary buffer  pointer */
//   unsigned char * ptBuffer=&RDS_BufferGroup1[0]; 
//   unsigned char BufferAvailable;   /* set to !=0 if buffer free */
//   
//   BufferAvailable=0;
//    
//   if (RDS_Buf1State==E_RDS_BUF_STATE_FREE )
//   {
//       /* filling buffer */
//       RDS_Buf1State=E_RDS_BUF_STATE_IN_PREPARATION;
//       ptBuffer = &RDS_BufferGroup1[0];
//       BufferAvailable=1;
//   }
//    if (RDS_Buf2State==E_RDS_BUF_STATE_FREE )
//   {
//       /* filling buffer */
//       RDS_Buf2State=E_RDS_BUF_STATE_IN_PREPARATION;
//       ptBuffer = &RDS_BufferGroup2[0];
//       BufferAvailable=2;
//   }
//    
//   if (BufferAvailable !=0)
//   {
////       RDSIO_SetActLed(1);    /* for time measure debug */
//       switch(RDSGEN_SelectNextGroup() )
//       {
//          case RDS_GROUP_TYPE_0A :
//               RDSGEN_Gen0A(ptBuffer);
//               break; 
//          case RDS_GROUP_TYPE_2A :
//               RDSGEN_Gen2A(ptBuffer);
//               break; 
//       }
//       switch(BufferAvailable)
//       {
//          case 1:
//            {
//               RDS_Buf1State=E_RDS_BUF_STATE_READY;             
//               break;
//            }
//          case 2:
//            {
//               RDS_Buf2State=E_RDS_BUF_STATE_READY;

//               break;
//            }
//       }
////       RDSIO_SetActLed(0);    /* for time measure debug */
//       //RDSIO_ToggleActLed();    /* toggle Activity LED on each group generate ---> ~6Hz */
//   }  /* endif test on buffer available */


//}

/* return next group type to generate */
//unsigned char RDSGEN_SelectNextGroup(void)
//{
//    /* for first version only group 0A and 2A could be sent, 0A is always enable */
//    /* a more complex algo will be implemented in next version */
//    unsigned char TmpGroupRate;
//    
//    //TmpGroupRate =EEP_get_GroupRate((RDS_GROUP_TYPE_2A & RDS_GROUP_TYPE_SELECT),(RDS_GROUP_TYPE_2A>>RDS_GROUP_TYPE_SHIFT));
//    
//    if (TmpGroupRate==0)
//    {
//        /* 2A group disable , so return GROUP0A */
//        return (RDS_GROUP_TYPE_0A);      
//    } 
//    else
//    {
//        /* 2A group enable */
//        /* incrementing SelectGroup counter */
//        SelectGroup++;
//        if (SelectGroup > TmpGroupRate)
//        {
//           SelectGroup=0;
//           return (RDS_GROUP_TYPE_2A);      
//        }
//        else
//        {
//           return (RDS_GROUP_TYPE_0A);            
//        }
//    } 
//}

///* IO Mgt functions  */
//unsigned char RDSIO_ReadTAInput(void)             /* read TA input return 1  if active level */
//{
//	//doc TA va tra ve gia tri
//	return(0);
//}

//void RDSIO_ToggleActLed(void)                     /* change current state of Activity LED */
//{
////Tat bat led chang?
//}

//void RDSIO_Init(void)                             /* initialisation */
//{
//	//Tao ra vai con LED nao
//}
//void RDSIO_SetTALed(unsigned char NewState)       /* set TA LED 0: OFF, 1 LED ON */
//{
//	//lam den sang theo TA nao
//}
//void RDSIO_ToggleTALed(void)                      /* change current state of TA LED */
//{
//}
//void RDSIO_SetActLed(unsigned char NewState)      /* set Activity LED 0: OFF, 1 LED ON */
//{
//	
//}
//eeprom



/* return current MS */
//Music / speech : If a musical emission is in course of transmission,
//the value "1" is set (default value if unused function),
//	otherwise the value "0" indicate a speech broadcasting
//uint8_t EEP_get_MS(void)
//{
//	return(1);// Tra ve gia tri MS
//}

/* return current NbAF */
/*
This word of 16bits allows to transmit alternative frequencies. These frequencies allow the receiver to switch to another transmitter diffusing the same radio in case of bad reception.
This function is very useful for a radio car because it allow to know quickly the other near transmitters and so to commute quickly when car moving .
The frequency is coded by step of 100KHz from 87.6MHz, a list of frequencies must be always preceded by the consequentive number of the list (from 1 to 25 frequencies)
*/
//uint8_t EEP_get_NbAF(void)
//{
//	return(8);// tra ve so luong kenh phu fix 8
//}
/* return current AF[Index] */
uint8_t get_AF(uint8_t Index)
{
	unsigned char AF_FIX[8]={1,2,3,4,5,6,7,8};
	return(AF_FIX[Index]);
	//return(EEP_AF_DEF_VAL - RDS_AF_FIRST_AF_FRQ);// Tra ve gia tri AF
}

/* return current PICode */
//uint16_t EEP_get_PICode(void)
//{
//	return(0xACD7);// tra VE MA PI
//}
///* return current radiotext[Index] */
//unsigned char EEP_get_Radiotext(uint8_t Index)
//{
//	//return(rds_text_swat[Index]);
//}
/* return current GroupRate[IndexType,IndexGroup] */
//uint8_t EEP_get_GroupRate(uint8_t IndexType,uint8_t IndexGroup) 
//{
//	return(10);
//}
/* return current PTY */
//uint8_t EEP_get_PTY(void)
//{
//	return(10);
//}
/* return current PSNAme[Index] */
//unsigned char EEP_get_PSName(uint8_t Index)
//{
//	//return (eeprom_read_byte(&(pt_EEPROM_Data->PSName[Index])));
//	//return PSName_swat[Index];
//}
/* return current TP */
//Traffic Program. This flag allow to inform receiver that program can carries traffic annoucement when it is set.
//uint8_t EEP_get_TP(void)
//{
//	//PTY 18 Children’s Progs
//	return(18);
//}	
/* return current TA */
//Traffic Annoucement : 
//set to "1" if traffic data are being diffused,
//otherwise this flag is set to "0" .(see also description flag TP)
//uint8_t EEP_get_TA(void)
//{
//return(0);
//}	

/* return current DI */
//	
//Decoder Information
//uint8_t EEP_get_DI(void)
//{
//return(0x03);
//}

//uint16_t eeprom_read_byte (uint16_t *addr)
//			 {
//					uint16_t data_out;	
//				  EE_ReadVariable( *addr,&data_out );
//					return(data_out);
//			 }
//       uint16_t eeprom_read_word ( uint16_t *addr)
//			 {
//			 }
//       void eeprom_write_byte (uint8_t *addr, uint8_t val)
//			 {
//			 }
//       void eeprom_write_word (uint16_t *addr, uint16_t val)
//			 {
//			 }
			 
/* EEPROM validity control, return 0 if OK,else  -1  */
//char EEP_Check(void)
//{ 
//    char Result;
//    unsigned int TmpCheckSum;  /* temporary check sum  value */
//    unsigned int Index;        /* loop index for Checksum compute */	
//    uint16_t * pt_EEP_data;      /* temporary pointer on EEPROM data */
//	


//	Result=0;   /*  OK by default */
//	TmpCheckSum=0;
//	pt_EEP_data = (uint16_t *) pt_EEPROM_Data;
//	
//	/* Compute chechsum loop without including validity control value  */
//	for (Index = (sizeof(T_RDS_EEPROM_data) -sizeof(uint16_t));Index !=0; Index--)
//	{
//	     TmpCheckSum += eeprom_read_byte(pt_EEP_data);
//		 pt_EEP_data++;
//	}
//	
//	if ( TmpCheckSum != eeprom_read_word(&(pt_EEPROM_Data->Validity_EEPROM)))
//	{
//	   Result =-1;
//	}
//	return Result;
//}

/* default EEPROM initialisation */
//void EEP_SetDefault(void)
//{}
/* pointer initialisation , validity control and default initialisation if no valid data */
//void EEP_Init(void)
//{
//    char ResultCheck;    /* checking EEPROM result  */
//	char E2END;// I dont known what is this
//	
//    /* init pointer on data structure at end of EEPROM memory  delimited by E2END */
//    pt_EEPROM_Data = (T_RDS_EEPROM_data  *) (E2END - sizeof( T_RDS_EEPROM_data));
//	
//	/* validity EEPROM control  */
//	ResultCheck = EEP_Check();

//	/* IF data are not valid */
//	/* THEN */
//	if (ResultCheck != 0)
//	{
//	    /* default EERPOM init */
//		EEP_SetDefault();
//	/* ENDIF */
//	}
	

 
//}	
				 

