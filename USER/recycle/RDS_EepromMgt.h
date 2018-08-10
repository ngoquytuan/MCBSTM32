/* include application */
//#include "main.h"
#include "stm32f10x.h"

/* default value for reseting EEPROM data */

#define EEP_PTY_DEF_VAL   10               /* PTY default  value  */
#define EEP_PSNAME_DEF_VAL "TEST RDS"      /* PSNAME default value */
#define EEP_TP_DEF_VAL 1                   /* TP default value */
#define EEP_TA_DEF_VAL 0                   /* TA default value */
#define EEP_DI_DEF_VAL 1                   /* DI  default value 1= stereo  */
#define EEP_MS_DEF_VAL 1                   /* MS default value */
#define EEP_PI_DEF_VAL 0xF000              /* PI code default value */
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
void EEP_Init(void);                             /* initialisation */
void EEP_SetDefault(void);                       /* initialisation default EEPROM data */
char EEP_Check(void);                            /* EEPROM validity control ,return 0 if OK,else  -1 */
uint16_t EEP_update_valid(void);                 /* compute validity checksum, memorise and return value */

/* set new PI */
void EEP_set_PICode(uint16_t NewPICode);           
/* set new PTY */
void EEP_set_PTY(uint8_t NewPTY);           
/* set new PSNAME */
void EEP_set_PSName(uint8_t NewPSName[RDS_SIZE_PSNAME]);           
/* set new TP */
void EEP_set_TP(uint8_t NewTP);           
/* set new TA */
void EEP_set_TA(uint8_t NewTA);           
/* set new DI */
void EEP_set_DI(uint8_t NewDI);           
/* set new MS */
void EEP_set_MS(uint8_t NewMS);           
/* set new NbAF */
void EEP_set_NbAF(uint8_t NewNbAF);           
/* set new AF */
void EEP_set_AF(uint8_t NewNbAF,uint8_t Index);           
/* set new RadioText  */
void EEP_set_RadioText(uint8_t NewRT[RDS_SIZE_RADIOTEXT]);
/* set new group rate */
void EEP_set_GroupRate(uint8_t NewGroupRate,uint8_t IndexType, uint8_t IndexGroup);           
/* set new CoderAdress */
void EEP_set_CoderAdress(uint8_t NewCoderAdress);           
/* set new SiteAdress */
void EEP_set_SiteAdress(uint16_t NewSiteAdress);           


void EEP_set_Passwd(uint16_t NewPasswd);           /* set new passwd */
/* set new StereoMode */
void EEP_set_StereoMode(uint8_t NewStereoMode);           



/* return current PICode */
uint16_t EEP_get_PICode(void);               
/* return current PTY */
uint8_t EEP_get_PTY(void);               
/* return current PSNAme[Index] */
unsigned char EEP_get_PSName(uint8_t Index);               
/* return current TP */
uint8_t EEP_get_TP(void);               
/* return current TA */
uint8_t EEP_get_TA(void);               
/* return current DI */
uint8_t EEP_get_DI(void);               
/* return current MS */
uint8_t EEP_get_MS(void);               
/* return current NbAF */
uint8_t EEP_get_NbAF(void);               
/* return current AF[Index] */
uint8_t EEP_get_AF(uint8_t Index);               
/* return current radiotext[Index] */
unsigned char EEP_get_Radiotext(uint8_t Index);
/* return current GroupRate[IndexType,IndexGroup] */
uint8_t EEP_get_GroupRate(uint8_t IndexType,uint8_t IndexGroup);               
/* return current CoderAdress */
uint8_t EEP_get_CoderAdress(void);               
/* return current SiteAdress */
uint16_t EEP_get_SiteAdress(void);               


uint16_t EEP_get_Passwd(void);               /* return current passwd */
/* return current configuration StereoMode */
uint8_t EEP_get_StereoMode(void);   