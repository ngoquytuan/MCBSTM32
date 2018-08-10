#include "stm32f10x.h"                  // Device header
#include "GPIO_STM32F10x.h"             // Keil::Device:GPIO

#include "tm1637.h"


void _tm1637Start(void);
void _tm1637Stop(void);
void _tm1637ReadResult(void);
void _tm1637WriteByte(unsigned char b);
void _tm1637DelayUsec(unsigned int i);
void _tm1637ClkHigh(void);
void _tm1637ClkLow(void);
void _tm1637DioHigh(void);
void _tm1637DioLow(void);

// Configuration.

#define CLK_PORT GPIOB
#define DIO_PORT GPIOB
#define CLK_PIN GPIO_Pin_12
#define DIO_PIN GPIO_Pin_13
#define CLK_PORT_CLK_ENABLE GPIO_PortClock   (CLK_PORT, true)
#define DIO_PORT_CLK_ENABLE GPIO_PortClock   (DIO_PORT, true)


const char segmentMap[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, // 0-7
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, // 8-9, A-F
    0x00
};


void tm1637Init(void)
{
    //CLK_PORT_CLK_ENABLE;
    //DIO_PORT_CLK_ENABLE;
		
		//GPIO_PinConfigure(CLK_PORT, CLK_PIN, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT10MHZ);
    //GPIO_PinConfigure(DIO_PORT, DIO_PIN, GPIO_OUT_PUSH_PULL, GPIO_MODE_OUT10MHZ);
	
	GPIO_PortClock   (GPIOB, true);
	GPIO_PinConfigure(GPIOB, 12, GPIO_AF_OPENDRAIN, GPIO_MODE_OUT2MHZ);
	GPIO_PortClock   (GPIOB, true);
	GPIO_PinConfigure(GPIOB, 13, GPIO_AF_OPENDRAIN, GPIO_MODE_OUT2MHZ);
	
		tm1637SetBrightness(6);
}

void TM1637_Disp_Dot(unsigned char seg1,unsigned char seg2,unsigned char seg3,unsigned char seg4)
	{
		
		_tm1637Start();
    _tm1637WriteByte(0x40);
    _tm1637ReadResult();
    _tm1637Stop();

    _tm1637Start();
    _tm1637WriteByte(0xc0);
    _tm1637ReadResult();


    _tm1637WriteByte(seg1);
    _tm1637ReadResult();
		
		_tm1637WriteByte(seg2);
    _tm1637ReadResult();
		
		_tm1637WriteByte(seg3);
    _tm1637ReadResult();
		
		_tm1637WriteByte(seg4);
    _tm1637ReadResult();

    _tm1637Stop();
	}
void tm1637DisplayDecimal(int v, int displaySeparator)
{
    unsigned char digitArr[4];
	int i;
    for ( i = 0; i < 4; ++i) {
        digitArr[i] = segmentMap[v % 10];
        if (i ==  displaySeparator) {
            digitArr[i] |= 1 << 7;
        }
        v /= 10;
    }

    _tm1637Start();
    _tm1637WriteByte(0x40);
    _tm1637ReadResult();
    _tm1637Stop();

    _tm1637Start();
    _tm1637WriteByte(0xc0);
    _tm1637ReadResult();

    for ( i = 0; i < 4; ++i) {
        _tm1637WriteByte(digitArr[3 - i]);
        _tm1637ReadResult();
    }

    _tm1637Stop();
}

// Valid brightness values: 0 - 8.
// 0 = display off.
void tm1637SetBrightness(char brightness)
{
    // Brightness command:
    // 1000 0XXX = display off
    // 1000 1BBB = display on, brightness 0-7
    // X = don't care
    // B = brightness
    _tm1637Start();
    _tm1637WriteByte(0x87 + brightness);
    _tm1637ReadResult();
    _tm1637Stop();
}

void _tm1637Start(void)
{
    _tm1637ClkHigh();
    _tm1637DioHigh();
    _tm1637DelayUsec(2);
    _tm1637DioLow();
}

void _tm1637Stop(void)
{
    _tm1637ClkLow();
    _tm1637DelayUsec(2);
    _tm1637DioLow();
    _tm1637DelayUsec(2);
    _tm1637ClkHigh();
    _tm1637DelayUsec(2);
    _tm1637DioHigh();
}

void _tm1637ReadResult(void)
{
    _tm1637ClkLow();
    _tm1637DelayUsec(5);
    // while (dio); // We're cheating here and not actually reading back the response.
    _tm1637ClkHigh();
    _tm1637DelayUsec(2);
    _tm1637ClkLow();
}

void _tm1637WriteByte(unsigned char b)
{
  int i;  
	for ( i = 0; i < 8; ++i) {
        _tm1637ClkLow();
        if (b & 0x01) {
            _tm1637DioHigh();
        }
        else {
            _tm1637DioLow();
        }
        _tm1637DelayUsec(3);
        b >>= 1;
        _tm1637ClkHigh();
        _tm1637DelayUsec(3);
    }
}

void _tm1637DelayUsec(unsigned int i)
{
  int j;  
//	for (; i>0; i--) {
//        for ( j = 0; j < 10; ++j) {
//             
//        }
//    }
	for(j=0;j<20*i;j++);
}

void _tm1637ClkHigh(void)
{
    //HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
	GPIO_PinWrite(CLK_PORT, CLK_PIN, 1);
}

void _tm1637ClkLow(void)
{
    //HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
	GPIO_PinWrite(CLK_PORT, CLK_PIN, 0);
}

void _tm1637DioHigh(void)
{
    //HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_SET);
	GPIO_PinWrite(DIO_PORT, DIO_PIN, 1);
}

void _tm1637DioLow(void)
{
    //HAL_GPIO_WritePin(DIO_PORT, DIO_PIN, GPIO_PIN_RESET);
	GPIO_PinWrite(DIO_PORT, DIO_PIN, 0);
}

