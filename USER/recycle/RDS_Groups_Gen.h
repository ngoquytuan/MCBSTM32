/* include application */
/* definition of constants  */
#include "stm32f10x.h"
/* Common RDS constant */
#define RDS_SIZE_FRAME       13          /* 13 bytes for a frame*/
#define RDS_SIZE_PSNAME      8           /* 8 bytes for PS NAME */
#define RDS_SIZE_RADIOTEXT   64          /* 64 bytes for Radio text group 2A */
#define RDS_SIZE_RADIOTEXT_B 32          /* 32 bytes for Radio text group 2B */
#define RDS_SIZE_GROUPS      16          /* 16 groups A and 16 groups B --> 32 groups total */
#define RDS_SIZE_TYPE_GROUPS 2           /* 2 groups type : A and B */
#define RDS_SIZE_AFTAB       25          /* 25 Alternat freq max for AF functionality */
// Offical document , page 59 annex A
#define RDS_OFFSET_A         0xFC         /* Offset A for checkword computing block 1   */
#define RDS_OFFSET_B         0x198        /* Offset B for checkword computing block 2   */
#define RDS_OFFSET_C         0x168        /* Offset C for checkword computing block 3 XA   */
#define RDS_OFFSET_CPRIME    0x350        /* Offset C' for checkword computing block 3 XB */
#define RDS_OFFSET_D         0x1B4        /* Offset D for checkword computing block 4   */

#define RDS_GROUP_TYPE_0A    0x00         /* group type code  */
#define RDS_GROUP_TYPE_0B    0x01         /* group type code  */
#define RDS_GROUP_TYPE_1A    0x02         /* group type code  */
#define RDS_GROUP_TYPE_1B    0x03         /* group type code  */
#define RDS_GROUP_TYPE_2A    0x04         /* group type code  */
#define RDS_GROUP_TYPE_2B    0x05         /* group type code  */
#define RDS_GROUP_TYPE_3A    0x06         /* group type code  */
#define RDS_GROUP_TYPE_3B    0x07         /* group type code  */
#define RDS_GROUP_TYPE_4A    0x08         /* group type code  */
#define RDS_GROUP_TYPE_4B    0x09         /* group type code  */
#define RDS_GROUP_TYPE_5A    0x0A         /* group type code  */
#define RDS_GROUP_TYPE_5B    0x0B         /* group type code  */
#define RDS_GROUP_TYPE_6A    0x0B         /* group type code  */
#define RDS_GROUP_TYPE_6B    0x0D         /* group type code  */
#define RDS_GROUP_TYPE_7A    0x0E         /* group type code  */
#define RDS_GROUP_TYPE_7B    0x0F         /* group type code  */
#define RDS_GROUP_TYPE_8A    0x10         /* group type code  */
#define RDS_GROUP_TYPE_8B    0x11         /* group type code  */
#define RDS_GROUP_TYPE_9A    0x12         /* group type code  */
#define RDS_GROUP_TYPE_9B    0x13         /* group type code  */
#define RDS_GROUP_TYPE_10A   0x14         /* group type code  */
#define RDS_GROUP_TYPE_10B   0x15         /* group type code  */
#define RDS_GROUP_TYPE_11A   0x16         /* group type code  */
#define RDS_GROUP_TYPE_11B   0x17         /* group type code  */
#define RDS_GROUP_TYPE_12A   0x18         /* group type code  */
#define RDS_GROUP_TYPE_12B   0x19         /* group type code  */
#define RDS_GROUP_TYPE_13A   0x1A         /* group type code  */
#define RDS_GROUP_TYPE_13B   0x1B         /* group type code  */
#define RDS_GROUP_TYPE_14A   0x1C         /* group type code  */
#define RDS_GROUP_TYPE_14B   0x1D         /* group type code  */
#define RDS_GROUP_TYPE_15A   0x1E         /* group type code  */
#define RDS_GROUP_TYPE_15B   0x1F         /* group type code  */

#define RDS_GROUP_TYPE_SELECT 0x01        /* select 1st bit to determine type 0A/0B according b0 */
#define RDS_GROUP_TYPE_SHIFT  1           /* 1 right shift to determine group number from RDS_GROUP_TYPE_define */

#define RDS_AF_NOAF          224          /* code for no AF  */
#define RDS_AF_FILLER_CODE   205          /* filler code for AF  */
#define RDS_AF_FIRST_AF      1            /* code for Freq 87.6 MHz and step frequency by 100KHz */
#define RDS_AF_FIRST_AF_FRQ  875          /* first freq 87.6 MHz for AF in 100KHz lsb equal to 1 */
#define RDS_RTXT_ENDLINE     '\r'
/* local type */
typedef enum
{
  E_RDS_BUF_STATE_FREE,             /* buffer free */
  E_RDS_BUF_STATE_IN_USED,          /* buffer in used  by handler RDS clock*/
  E_RDS_BUF_STATE_IN_PREPARATION,   /* buffer in preparation by RDS generate function */
  E_RDS_BUF_STATE_READY            /* buffer ready to sending data */
} T_E_RDS_BUFF_STATE;



/* initialisation */
void RDSGEN_Init(void); 
/* generating 0A group */                                
void RDSGEN_Gen0A(unsigned char BufGroup[RDS_SIZE_FRAME]);
/* generating 2A group */          
void RDSGEN_Gen2A(unsigned char BufGroup[RDS_SIZE_FRAME]);   
/*select next data and return it */
unsigned char RDSGEN_ReadNextData(void);
/* compute RDS CRC from 16b data and 10b offset g(x) = x10 + x8 + x7 + x5 + x4 + x3 + 1*/
uint16_t RDSGEN_ComputeCRC(uint16_t Data, uint16_t Offset);

/* set data and checkword bock1 in buffer */
void RDSGEN_SetBlock1(uint16_t Data, uint16_t CheckWord, uint8_t Buffer[RDS_SIZE_FRAME]);
/* set data and checkword bock2 in buffer */
void RDSGEN_SetBlock2(uint16_t Data, uint16_t CheckWord, uint8_t Buffer[RDS_SIZE_FRAME]);
/* set data and checkword bock3 in buffer */
void RDSGEN_SetBlock3(uint16_t Data, uint16_t CheckWord, uint8_t Buffer[RDS_SIZE_FRAME]);
/* set data and checkword bock4 in buffer */
void RDSGEN_SetBlock4(uint16_t Data, uint16_t CheckWord, uint8_t Buffer[RDS_SIZE_FRAME]);

/* Check buffer free and fill it if OK */
void RDSGEN_CheckFreeBuf(void);
/* return next group type to generate */
//unsigned char RDSGEN_SelectNextGroup(void);


/* default value for reseting EEPROM data */

#define EEP_PTY_DEF_VAL   10               /* PTY default  value  */
#define EEP_PSNAME_DEF_VAL "TEST RDS"      /* PSNAME default value */
#define EEP_TP_DEF_VAL 1                   /* TP default value */
#define EEP_TA_DEF_VAL 0                   /* TA default value */
#define EEP_DI_DEF_VAL 1                   /* DI  default value 1= stereo  */
#define EEP_MS_DEF_VAL 1                   /* MS default value */
#define EEP_PI_DEF_VAL 0x1234              /* PI code default value */
#define EEP_NBAF_DEF_VAL 0                 /* NBAF number of Alternat frequencies default value  */
#define EEP_AF_DEF_VAL 876                 /* AF default value 87.6 MHz = first valid data */
#define EEP_RTXT_DEF_VAL "**** RDS coder V1.2 08/2007 ATmega32+TDA7330 RS232 USB SPI     \r" /* default value of radio text, max 64 byte ended by 0x0d (\r) */

#define EEP_CODER_ADDR_DEF_VAL 1           /* adress coder default value */
#define EEP_SITE__ADDR_DEF_VAL 1           /* site adress default value */

#define EEP_PASSWD_DEF_VAL 0               /* Passwd default value */
#define EEP_STEREO_MODE_DEF_VAL 0          /* StereoMode default value : 1=stereo / 0=mono */
#define EEP_GROUPRATE_2A_DEF_VAL 4         /* group rate for 2A type default value : 1 group 2A for each 4 group 0A */

/* definition of data structure  */

typedef struct  { 
  /* data specific for rds frame encoding */
  uint16_t  PICode;                             /* PI code   */
  uint8_t   PTY;                                /* PTY program type */
  uint8_t   PSName[RDS_SIZE_PSNAME];            /* PS Name */
  uint8_t   TP;                                 /* TP Traffic possible  */
  uint8_t   TA;                                 /* TA Traffic announcement */
  uint8_t   DI;                                 /* DI Decoder Info  */
  uint8_t   MS;                                 /* MS  Music speech */
  uint8_t   NbAF;                               /* NbAF  Number of AF  */
  uint8_t   AFTab[RDS_SIZE_AFTAB];              /* AF table , coding AF 0x01 = 87.6MHz, 0x02=87.7...0xCC=107.9 */
  uint8_t   RadioTxt[RDS_SIZE_RADIOTEXT];       /* radio text table end string '\r' */

  uint8_t   GroupRate[RDS_SIZE_TYPE_GROUPS][RDS_SIZE_GROUPS];      /* percent of groups for generating */
                                                                   /* Total must be equal to 100% with [0][0] correspond to group 0A */
                                                                   /* [0][1] : 1A   [0][2]:2A  .... [1][0] : 0B */
  /* data specific for universal interface protocol */
  uint8_t   CoderAdress;                        /* coder address of RDS coder */
  uint16_t  SiteAdress;                         /* Site address of RDS coder */

  /* data specific for RDS operator */
  uint16_t  Passwd;                             /* Passwd protection */
  uint8_t   StereoMode;                         /* Config stereo if !=0 (else mono) */

  


  /* End of applicative data */
  uint16_t  Validity_EEPROM;                    /* checksum Validity control od EEPROM data */   
} T_RDS_EEPROM_data ;




/* EEPROM Management functions  */
//void EEP_Init(void);                             /* initialisation */
//void EEP_SetDefault(void);                       /* initialisation default EEPROM data */
//char EEP_Check(void);                            /* EEPROM validity control ,return 0 if OK,else  -1 */
//uint16_t EEP_update_valid(void);                 /* compute validity checksum, memorise and return value */

///* set new PI */
//void EEP_set_PICode(uint16_t NewPICode);           
///* set new PTY */
//void EEP_set_PTY(uint8_t NewPTY);           
///* set new PSNAME */
//void EEP_set_PSName(uint8_t NewPSName[RDS_SIZE_PSNAME]);           
///* set new TP */
//void EEP_set_TP(uint8_t NewTP);           
///* set new TA */
//void EEP_set_TA(uint8_t NewTA);           
///* set new DI */
//void EEP_set_DI(uint8_t NewDI);           
///* set new MS */
//void EEP_set_MS(uint8_t NewMS);           
///* set new NbAF */
//void EEP_set_NbAF(uint8_t NewNbAF);           
///* set new AF */
//void EEP_set_AF(uint8_t NewNbAF,uint8_t Index);           
///* set new RadioText  */
//void EEP_set_RadioText(uint8_t NewRT[RDS_SIZE_RADIOTEXT]);
///* set new group rate */
//void EEP_set_GroupRate(uint8_t NewGroupRate,uint8_t IndexType, uint8_t IndexGroup);           
///* set new CoderAdress */
//void EEP_set_CoderAdress(uint8_t NewCoderAdress);           
///* set new SiteAdress */
//void EEP_set_SiteAdress(uint16_t NewSiteAdress);           


//void EEP_set_Passwd(uint16_t NewPasswd);           /* set new passwd */
///* set new StereoMode */
//void EEP_set_StereoMode(uint8_t NewStereoMode);           



///* return current PICode */
//uint16_t EEP_get_PICode(void);               
//              
//              
///* return current MS */
//uint8_t EEP_get_MS(void);               
///* return current NbAF */
//uint8_t EEP_get_NbAF(void);               
///* return current AF[Index] */
uint8_t get_AF(uint8_t Index);               

///* return current radiotext[Index] */
//unsigned char EEP_get_Radiotext(uint8_t Index);
///* return current GroupRate[IndexType,IndexGroup] */
//uint8_t EEP_get_GroupRate(uint8_t IndexType,uint8_t IndexGroup); 
///* return current PTY */
//uint8_t EEP_get_PTY(void);               
///* return current PSNAme[Index] */
//unsigned char EEP_get_PSName(uint8_t Index); 
///* return current TP */
//uint8_t EEP_get_TP(void);               
/* return current TA */
//uint8_t EEP_get_TA(void);               

/* return current DI */
//uint8_t EEP_get_DI(void); 
/* return current CoderAdress */
//uint8_t EEP_get_CoderAdress(void);               
/* return current SiteAdress */
//uint16_t EEP_get_SiteAdress(void);               


//uint16_t EEP_get_Passwd(void);               /* return current passwd */
/* return current configuration StereoMode */
//uint8_t EEP_get_StereoMode(void); 

/* IO Mgt functions  */
//void RDSIO_Init(void);                             /* initialisation */
//void RDSIO_SetTALed(unsigned char NewState);       /* set TA LED 0: OFF, 1 LED ON */
//void RDSIO_ToggleTALed(void);                      /* change current state of TA LED */
//void RDSIO_SetActLed(unsigned char NewState);      /* set Activity LED 0: OFF, 1 LED ON */
//unsigned char RDSIO_ReadTAInput(void);             /* read TA input return 1  if active level */
//void RDSIO_ToggleActLed(void);                     /* change current state of Activity LED */

//eeprom fuction
       //uint16_t eeprom_read_byte ( uint16_t *addr);
       //uint16_t eeprom_read_word ( uint16_t *addr);
       //void eeprom_write_byte (uint8_t *addr, uint8_t val);
       //void eeprom_write_word (uint16_t *addr, uint16_t val);
			 



