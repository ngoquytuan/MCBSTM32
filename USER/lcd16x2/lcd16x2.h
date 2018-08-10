
#ifndef __LCD_16X2_H
#define __LCD_16X2_H

//LCD marco
#define   LCD_RS(state)       GPIO_PinWrite(GPIOC, 12, state)
#define   LCD_RW(state)       GPIO_PinWrite(GPIOC, 11, state)
#define   LCD_EN(state)       GPIO_PinWrite(GPIOC, 10, state)

#define   LCD_D4(state)       GPIO_PinWrite(GPIOC, 0, state)
#define   LCD_D5(state)       GPIO_PinWrite(GPIOC, 1, state)
#define   LCD_D6(state)       GPIO_PinWrite(GPIOC, 2, state)
#define   LCD_D7(state)       GPIO_PinWrite(GPIOC, 3, state)
#define		LCD_on							GPIO_PinWrite(GPIOC, 8, 1);
#define		LCD_off							GPIO_PinWrite(GPIOC, 8, 0);

void LCD_GPIOConfiguration(void);

/*******************************************************************************
Noi Dung    :   Gui tin hieu Enable den LCD.
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/
 void LCD_Enable(void);
 /*******************************************************************************
Noi Dung    :   Gui 4 bit du lieu den LCD.
Tham Bien   :   Data: 4 bit thap cua Data chua 4 bit du lieu can gui.
Tra Ve      :   Khong.
********************************************************************************/

 void LCD_Send4Bit( char  Data );
 /*******************************************************************************
Noi Dung    :   Gui 1 byte du lieu den LCD.
Tham Bien   :   command: byte du lieu can gui.
Tra Ve      :   Khong.
********************************************************************************/
 void LCD_SendCommand (char  command );
 /*******************************************************************************
Noi Dung    :   Khoi tao LCD.
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/
 void LCD_Init ( void );
 /*******************************************************************************
Noi Dung    :   Thiet lap vi tri con tro LCD.
Tham Bien   :   x: vi tri cot cua con tro. x = 0 - 15.
                y: vi tri hang cua con tro. y= 0,1.
Tra Ve      :   Khong.
********************************************************************************/
void LCD_Gotoxy(char  y, char  x);
 /*******************************************************************************
Noi Dung    :   Xoa noi dung hien thi tren LCD.
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/
void LCD_Clear(void);
 /*******************************************************************************
Noi Dung    :   Viet 1 ki tu len LCD.
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/
 void LCD_Putc ( char  Data );
 /*******************************************************************************
Noi Dung    :   Viet 1 chuoi ki tu len LCD.
Tham Bien   :   Khong.
Tra Ve      :   Khong.
********************************************************************************/
void LCD_Puts (char *s);
 /*******************************************************************************
Noi Dung    :   printf len LCD.
Tham Bien   :   giong nhu printf.
Tra Ve      :   so ki tu duoc in len man hinh LCD.
********************************************************************************/
int lcdprintf(const char *fmt,...);
#endif

