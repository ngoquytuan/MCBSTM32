/* following constants depend of hardware mapping */
#define RDS_DATA_OUT_REG PORTC /* PORT C used for RDS data Output */
#define RDS_DATA_OUT_DIR DDRC  /* PORT DDRC used for RDS data output */
#define RDS_DATA_OUT_BIT 7     /* bit 7 used as RDS data output */

#define RDS_TALED_OUT_REG PORTB /* PORT B used for TA LED  */
#define RDS_TALED_OUT_DIR DDRB  /* PORT DDRB used for TA LED  output */
#define RDS_TALED_OUT_BIT 2     /* bit b2 used as TA LED output */

#define RDS_ACTLED_OUT_REG PORTB /* PORT B used for Activity LED  */
#define RDS_ACTLED_OUT_DIR DDRB  /* PORT DDRB used for Activity LED  output */
#define RDS_ACTLED_OUT_BIT 0     /* bit b0 used as Activity LED output */

#define RDS_57KEXT_OUT_REG PORTC /* PORT C used for 57K external select clock (stereo mode)  */
#define RDS_57KEXT_OUT_DIR DDRC  /* PORT DDRC used for 57K EXT  output */
#define RDS_57KEXT_OUT_BIT 3     /* bit b3 used to select 57K external clock if 1 */

#define RDS_57KINT_OUT_REG PORTC /* PORT C used for 57K internal select  clock (mono mode)*/
#define RDS_57KINT_OUT_DIR DDRC  /* PORT DDRC used for 57K INT  output */
#define RDS_57KINT_OUT_BIT 4     /* bit b4 used to select 57K internal clock if 1 */

#define RDS_OC1A_OUT_REG PORTD    /* PORT D used for 1187.5  output clock */
#define RDS_OC1A_OUT_REG_IN PIND  /* PORT D input used for 1187.5  output clock */
#define RDS_OC1A_OUT_DIR DDRD     /* PORT DDRD used for 1187.5  output clockt */
#define RDS_OC1A_OUT_BIT 5        /* bit b5 used to select 1187.5  output clock if 1 */

#define RDS_TA_IN_REG PORTD       /* PORT D used for TA input */
#define RDS_TA_IN_REG_IN PIND     /* PORT D input used for TA input */
#define RDS_TA_IN_DIR DDRD        /* PORT DDRD used for TA input  */
#define RDS_TA_IN_BIT 7           /* bit b7 used to select TA input */





/* IO Mgt functions  */
//void RDSIO_Init(void);                             /* initialisation */
//void RDSIO_SetTALed(unsigned char NewState);       /* set TA LED 0: OFF, 1 LED ON */
//void RDSIO_ToggleTALed(void);                      /* change current state of TA LED */
//void RDSIO_SetActLed(unsigned char NewState);      /* set Activity LED 0: OFF, 1 LED ON */
//void RDSIO_ToggleActLed(void);                     /* change current state of Activity LED */
//unsigned char RDSIO_ReadTAInput(void);             /* read TA input return 1  if active level */
