#include "crc16.h"
/* A fairly simple Modbus compliant 16 Bit CRC algorithm.
*  Returns 1 if the crc check is positive, returns 0 if it fails.
*  Appends
*/
uint16_t crc16(volatile uint8_t *ptrToArray,uint8_t inputSize) //A standard CRC algorithm
{
	uint16_t out=0xffff;
	uint16_t carry,l;
	unsigned char n;
	inputSize++;
	for ( l=0; l<inputSize; l++) {
		out ^= ptrToArray[l];
		for (n = 0; n < 8; n++) {
			carry = out & 1;
			out >>= 1;
			if (carry) out ^= 0xA001;
		}
	}
	//out=0x1234;
	if ((ptrToArray[inputSize]==out%256) && (ptrToArray[inputSize+1]==out/256)) //check
	{
		return 1;
	} else { 
		ptrToArray[inputSize]=out%256; //append Lo
		ptrToArray[inputSize+1]=out/256; //append Hi
		return 0;	
	}
}

uint16_t makecrc16(char *ptrToArray,uint8_t inputSize) //A standard CRC algorithm
{
	uint16_t out=0xffff;
	uint16_t carry,l;
	unsigned char n;
	inputSize++;
	for ( l=0; l<inputSize; l++) {
		out ^= ptrToArray[l];
		for (n = 0; n < 8; n++) {
			carry = out & 1;
			out >>= 1;
			if (carry) out ^= 0xA001;
		}
	}
	//out=0x1234;
	if ((ptrToArray[inputSize]==out%256) && (ptrToArray[inputSize+1]==out/256)) //check
	{
		return 1;
	} else { 
		ptrToArray[inputSize]=out%256; //append Lo
		ptrToArray[inputSize+1]=out/256; //append Hi
		return 0;	
	}
}

uint8_t swap_bits(uint8_t c)
{
   return ((c&1)?128:0)|((c&2)?64:0)|((c&4)?32:0)|((c&8)?16:0)|((c&16)?8:0)
          |((c&32)?4:0)|((c&64)?2:0)|((c&128)?1:0);
}


/*
#define CRC_16    0x8005      //bit pattern (1)1000 0000 0000 0101
#define CRC_CCITT 0x1021      //bit pattern (1)0001 0000 0010 0001
#define CRC_32    0x04C11DB7  //bit pattern (1)0000 0100 1100 0001 0001 1101 1011 0111


int generate_8bit_crc(char* data, int16 length, int pattern)
{
   int   *current_data;
   int   crc_byte;
   int16 byte_counter;
   int   bit_counter;

   current_data = data;
   crc_byte = *current_data++;

   for(byte_counter=0; byte_counter < (length-1); byte_counter++)
   {
      for(bit_counter=0; bit_counter < 8; bit_counter++)
      {
         if(!bit_test(crc_byte,7))
         {
            crc_byte <<= 1;
            bit_test(*current_data, 7 - bit_counter) ?
               bit_set(crc_byte,0) : bit_clear(crc_byte,0);
            continue;
         }
         crc_byte <<= 1;
         bit_test(*current_data, 7 - bit_counter) ?
            bit_set(crc_byte,0) : bit_clear(crc_byte,0);
         crc_byte ^= pattern;
      }
      current_data++;
   }
   for(bit_counter=0; bit_counter < 8; bit_counter++)
   {
      if(!bit_test(crc_byte,7))
      {
         crc_byte <<= 1;
         continue;
      }
      crc_byte <<= 1;
      crc_byte ^= pattern;
   }
   return crc_byte;
}


int16 generate_16bit_crc(char* data, int16 length, int16 pattern)
{
   int   *current_data;
   int16 crc_Dbyte;
   int16 byte_counter;
   int   bit_counter;

   current_data = data + 2;
   crc_Dbyte =  make16(data[0], data[1]);

   for(byte_counter=0; byte_counter < (length-2); byte_counter++)
   {
      for(bit_counter=0; bit_counter < 8; bit_counter++)
      {
         if(!bit_test(crc_Dbyte,15))
         {
            crc_Dbyte <<= 1;
            bit_test(*current_data, 7 - bit_counter) ?
               bit_set(crc_Dbyte,0) : bit_clear(crc_Dbyte,0);
            continue;
         }
         crc_Dbyte <<= 1;
         bit_test(*current_data, 7 - bit_counter) ?
            bit_set(crc_Dbyte,0) : bit_clear(crc_Dbyte,0);
         crc_Dbyte ^= pattern;
      }
      current_data++;
   }

   for(bit_counter=0; bit_counter < 16; bit_counter++)
   {
      if(!bit_test(crc_Dbyte,15))
      {
         crc_Dbyte <<= 1;
         continue;
      }
      crc_Dbyte <<= 1;
      crc_Dbyte ^= pattern;
   }

   return crc_Dbyte;
}

int32 generate_32bit_crc(char* data, int16 length, int32 pattern)
{
   int   *current_data;
   int32 crc_Dbyte;
   int16 byte_counter;
   int   bit_counter;

   current_data = data + 4;
   crc_Dbyte =  make32(data[0], data[1], data[2], data[3]);

   for(byte_counter=0; byte_counter < (length-4); byte_counter++)
   {
      for(bit_counter=0; bit_counter < 8; bit_counter++)
      {
         if(!bit_test(crc_Dbyte,31))
         {
         crc_Dbyte <<= 1;
         bit_test(*current_data, 7 - bit_counter) ?
            bit_set(crc_Dbyte,0) : bit_clear(crc_Dbyte,0);
            continue;
         }
         crc_Dbyte <<= 1;
         bit_test(*current_data, 7 - bit_counter) ?
            bit_set(crc_Dbyte,0) : bit_clear(crc_Dbyte,0);
         crc_Dbyte ^= pattern;
      }
      current_data++;
   }

   for(bit_counter=0; bit_counter < 32; bit_counter++)
   {
      if(!bit_test(crc_Dbyte,31))
      {
         crc_Dbyte <<= 1;
         continue;
      }
      crc_Dbyte <<= 1;
      crc_Dbyte ^= pattern;
   }

   return crc_Dbyte;
}
*/