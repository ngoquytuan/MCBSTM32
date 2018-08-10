//http://www.simplymodbus.ca/FC03.htm
#include "crc16.h"
#include "uart.h"
#include "modbus_define.h"
#include "delay.h"
#include "modbus_phy_layer.h"
#include "modbus_app_layer.h"

#if (MODBUS_TYPE_SLAVE==1)
/*************************************************************************************/
	 uint8_t coils = 0x27;
   uint8_t inputs = 176;
   int16 hold_regs[] = {0x8822,0x7700,0x6600,0x5500,0x4400,0x3300,0x2200,0x1100};
   int16 input_regs[] = {0x1100,0x2200,0x3300,0x4400,0x5500,0x6600,0x7700,0x8800};
   int16 event_count = 0;
	 //uint8_t MODBUS = 1;// on modbus mode
/*************************************************************************************/	 



/*
read_FIFO_queue_rsp
Input:     int8        address            Slave Address
           int16       func               function to respond to
           exception   error              exception response to send
Output:    void
*/
void modbus_exception_rsp(  uint8_t address,   uint16_t func, exception error)
{
   //printf("modbus_exception_rsp\r\n");
	 modbus_serial_send_start(address, func|0x80);
   modbus_serial_putc(error);
   modbus_serial_send_stop();
}
/*
read_coils_rsp
Input:     int8       address            Slave Address
           int8       byte_count         Number of bytes being sent
           int8*      coil_data          Pointer to an array of data to send
Output:    void
*/
void modbus_read_coils_rsp(uint8_t address, uint8_t byte_count, uint8_t* coil_data)
{
   uint8_t i;

   modbus_serial_send_start(address, FUNC_READ_COILS);

   modbus_serial_putc(byte_count);

   for(i=0; i < byte_count; ++i)
   {
      modbus_serial_putc(*coil_data);
      coil_data++;
   }

   modbus_serial_send_stop();
}
/*
read_discrete_input_rsp
Input:     int8       address            Slave Address
           int8       byte_count         Number of bytes being sent
           int8*      input_data         Pointer to an array of data to send
Output:    void
*/
void modbus_read_discrete_input_rsp(  uint8_t address,   uint8_t byte_count,
                                      uint8_t *input_data)
{
   uint8_t i;

   modbus_serial_send_start(address, FUNC_READ_DISCRETE_INPUT);

   modbus_serial_putc(byte_count);

   for(i=0; i < byte_count; ++i)
   {
      modbus_serial_putc(*input_data);
      input_data++;
   }

   modbus_serial_send_stop();
}
/*
read_holding_registers_rsp
Input:     int8       address            Slave Address
           int8       byte_count         Number of bytes being sent
           int8*      reg_data           Pointer to an array of data to send
Output:    void
*/
void modbus_read_holding_registers_rsp(  uint8_t address,   uint8_t byte_count,
                                          uint16_t *reg_data)
{
   uint8_t i;

   modbus_serial_send_start(address, FUNC_READ_HOLDING_REGISTERS);

   modbus_serial_putc(byte_count);

   for(i=0; i < byte_count; i+=2)
   {
      modbus_serial_putc(((*reg_data)>>8)&0xff);
      modbus_serial_putc((*reg_data)&0xff);
      reg_data++;
   }

   modbus_serial_send_stop();
}
/*
read_input_registers_rsp
Input:     int8       address            Slave Address
           int8       byte_count         Number of bytes being sent
           int8*      input_data         Pointer to an array of data to send
Output:    void
*/
void modbus_read_input_registers_rsp(  uint8_t address,   uint8_t byte_count,
                                          uint16_t *input_data)																					
{
   int8 i;

   modbus_serial_send_start(address, FUNC_READ_INPUT_REGISTERS);

   modbus_serial_putc(byte_count);

   for(i=0; i < byte_count; i+=2)
   {
      modbus_serial_putc(((*input_data)>>8)&0xff);
      modbus_serial_putc((*input_data)&0xff);
      input_data++;
   }

   modbus_serial_send_stop();
}

/*
write_single_coil_rsp
Input:     int8       address            Slave Address
           int16      output_address     Echo of output address received
           int16      output_value       Echo of output value received
Output:    void
*/
void modbus_write_single_coil_rsp(  uint8_t address,   uint16_t output_address,
                                      uint16_t output_value)
{
   modbus_serial_send_start(address, FUNC_WRITE_SINGLE_COIL);

   modbus_serial_putc((output_address>>8)&0xff);
   modbus_serial_putc(output_address&0xff);

   modbus_serial_putc((output_value>>8)&0xff);
   modbus_serial_putc(output_value&0xff);

   modbus_serial_send_stop();
}
/*
read_exception_status_rsp
Input:     int8       address            Slave Address
Output:    void
*/
void modbus_read_exception_status_rsp(  uint8_t address,   uint8_t data)
{
   modbus_serial_send_start(address, FUNC_READ_EXCEPTION_STATUS);
   modbus_serial_send_stop();
}

/*
write_single_register_rsp
Input:     int8       address            Slave Address
           int16      reg_address        Echo of register address received
           int16      reg_value          Echo of register value received
Output:    void
*/
void modbus_write_single_register_rsp(  uint8_t address,   uint16_t reg_address,
                                          uint16_t reg_value)
{
   modbus_serial_send_start(address, FUNC_WRITE_SINGLE_REGISTER);

   modbus_serial_putc((reg_address>>8)&0xff);
   modbus_serial_putc(reg_address&0xff);

   modbus_serial_putc((reg_value>>8)&0xff);
   modbus_serial_putc(reg_value&0xff);

   modbus_serial_send_stop();
}

/*
write_multiple_coils_rsp
Input:     int8       address            Slave Address
           int16      start_address      Echo of address to start at
           int16      quantity           Echo of amount of coils written to
Output:    void
*/
void modbus_write_multiple_coils_rsp(  uint8_t address,   uint16_t start_address,
                                          uint16_t quantity)
{
   modbus_serial_send_start(address, FUNC_WRITE_MULTIPLE_COILS);

   modbus_serial_putc((start_address>>8)&0xff);
   modbus_serial_putc(start_address&0xff);

   modbus_serial_putc((quantity>>8)&0xff);
   modbus_serial_putc(quantity&0xff);

   modbus_serial_send_stop();
}

/*
write_multiple_registers_rsp
Input:     int8       address            Slave Address
           int16      start_address      Echo of address to start at
           int16      quantity           Echo of amount of registers written to
Output:    void
*/
void modbus_write_multiple_registers_rsp(  uint8_t address,   uint16_t start_address,
                                              uint16_t quantity)
{
   modbus_serial_send_start(address, FUNC_WRITE_MULTIPLE_REGISTERS);

   modbus_serial_putc((start_address>>8)&0xff);
   modbus_serial_putc(start_address&0xff);

   modbus_serial_putc((quantity>>8)&0xff);
   modbus_serial_putc(quantity&0xff);

   modbus_serial_send_stop();
}
//---------------------------------------------------------------MODBUS_APP_LAYER_C

//------------------------------------------------------------------------------------------------------
void modbus_slave_exe(uint8_t *uart_buff,uint8_t length)
{
 uint8_t i,j,data;

	//printf("UART BUFF:%s ; %d\r\n",uart_buff,length);
	for(i=0;i<length;i++)
	 incomming_modbus_serial( *(uart_buff+i));
	modbus_timeout_now(); 
	 if(modbus_kbhit() == TRUE)
	 {
	 //check address against our address, 0 is broadcast
      if((modbus_rx.address == MODBUS_ADDRESS) || modbus_rx.address == 0)
      {
				//printf("FUNC:%d\r\n",modbus_rx.func);
				switch(modbus_rx.func)
         {
					 case FUNC_READ_COILS:    //read coils
					 case FUNC_READ_DISCRETE_INPUT:    //read inputs	 
						 if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 8 || modbus_rx.data[3]+modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {

                  if(modbus_rx.func == FUNC_READ_COILS)
                     data = coils>>(modbus_rx.data[1]);      //move to the starting coil
                  else
                     data = inputs>>(modbus_rx.data[1]);      //move to the starting input

                  data = data & (0xFF>>(8-modbus_rx.data[3]));  //0 out values after quantity

                  if(modbus_rx.func == FUNC_READ_COILS)
                     modbus_read_coils_rsp(MODBUS_ADDRESS, 0x01, &data);
                  else
                     modbus_read_discrete_input_rsp(MODBUS_ADDRESS, 0x01, &data);

                  event_count++;
               }
               break;
					case FUNC_READ_HOLDING_REGISTERS: //printf("READ HOLDING REGISTERS\r\n");
          case FUNC_READ_INPUT_REGISTERS:
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 8 || modbus_rx.data[3]+modbus_rx.data[1] > 8)// user define!!!
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  if(modbus_rx.func == FUNC_READ_HOLDING_REGISTERS)
									   modbus_read_holding_registers_rsp(MODBUS_ADDRESS,(modbus_rx.data[3]*2),hold_regs+modbus_rx.data[1]);
                  else
										 modbus_read_input_registers_rsp(MODBUS_ADDRESS,(modbus_rx.data[3]*2),input_regs+modbus_rx.data[1]);

                  event_count++;
               }
               break;
					case FUNC_WRITE_SINGLE_COIL:      //write coil
               if(modbus_rx.data[0] || modbus_rx.data[3] || modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else if(modbus_rx.data[2] != 0xFF && modbus_rx.data[2] != 0x00)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_VALUE);
               else
               {								 
								 if(modbus_rx.data[2] == 0xFF)
                     bit_set(coils,modbus_rx.data[1]);
                  else
                     bit_clear(coils,modbus_rx.data[1]);

                  modbus_write_single_coil_rsp(MODBUS_ADDRESS,modbus_rx.data[1],((int16)(modbus_rx.data[2]))<<8);

                  event_count++;
               }
							 break;
           case FUNC_WRITE_SINGLE_REGISTER:
               if(modbus_rx.data[0] || modbus_rx.data[1] >= 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  hold_regs[modbus_rx.data[1]] = make16(modbus_rx.data[2],modbus_rx.data[3]);

                  modbus_write_single_register_rsp(MODBUS_ADDRESS,
                               make16(modbus_rx.data[0],modbus_rx.data[1]),
                               make16(modbus_rx.data[2],modbus_rx.data[3]));
               }
               break;
					 case FUNC_WRITE_MULTIPLE_COILS:
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 8 || modbus_rx.data[3]+modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  

                  modbus_rx.data[5] = swap_bits(modbus_rx.data[5]);

                  for(i=modbus_rx.data[1],j=0; i < modbus_rx.data[1]+modbus_rx.data[3]; ++i,++j)
                  {
                     if(bit_test(modbus_rx.data[5],j))
                        bit_set(coils,(7-i));
                     else
                        bit_clear(coils,(7-i));
                  }

                  modbus_write_multiple_coils_rsp(MODBUS_ADDRESS,
                                 make16(modbus_rx.data[0],modbus_rx.data[1]),
                                 make16(modbus_rx.data[2],modbus_rx.data[3]));

                  event_count++;
               }
               break;
            case FUNC_WRITE_MULTIPLE_REGISTERS:
               if(modbus_rx.data[0] || modbus_rx.data[2] ||
                  modbus_rx.data[1] >= 8 || modbus_rx.data[3]+modbus_rx.data[1] > 8)
                  modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_DATA_ADDRESS);
               else
               {
                  int i,j;

                  for(i=0,j=5; i < modbus_rx.data[4]/2; ++i,j+=2)
                     hold_regs[i] = make16(modbus_rx.data[j],modbus_rx.data[j+1]);

                  modbus_write_multiple_registers_rsp(MODBUS_ADDRESS,
                                 make16(modbus_rx.data[0],modbus_rx.data[1]),
                                 make16(modbus_rx.data[2],modbus_rx.data[3]));

                  event_count++;
               }
               break;
					default:    //We don't support the function, so return exception
               modbus_exception_rsp(MODBUS_ADDRESS,modbus_rx.func,ILLEGAL_FUNCTION);
					     //printf("We don't support the function, so return exception\r\n");
				 }
			}
	 }
	
}
#endif

#if (MODBUS_TYPE_MASTER==1)
#define MODBUS_SLAVE_ADDRESS 0xF7

/*
read_coils
Input:     int8       address            Slave Address
           int16      start_address      Address to start reading from
           int16      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception master_modbus_read_coils(uint8_t address, uint16_t start_address, uint16_t quantity)
{
   modbus_serial_send_start(address, FUNC_READ_COILS);

   modbus_serial_putc(make8(start_address,1));
   modbus_serial_putc(make8(start_address,0));

   modbus_serial_putc(make8(quantity,1));
   modbus_serial_putc(make8(quantity,0));

   modbus_serial_send_stop();

   return 0;
}

/*
read_discrete_input
Input:     int8       address            Slave Address
           int16      start_address      Address to start reading from
           int16      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception master_modbus_read_discrete_input(uint8_t address, uint16_t start_address, uint16_t quantity)
{
   modbus_serial_send_start(address, FUNC_READ_DISCRETE_INPUT);

   modbus_serial_putc(make8(start_address,1));
   modbus_serial_putc(make8(start_address,0));

   modbus_serial_putc(make8(quantity,1));
   modbus_serial_putc(make8(quantity,0));

   modbus_serial_send_stop();

   return 0;
}
/*
read_holding_registers
Input:     int8       address            Slave Address
           int16      start_address      Address to start reading from
           int16      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception master_modbus_read_holding_registers(uint8_t address, uint16_t start_address, uint16_t quantity)
{
   modbus_serial_send_start(address, FUNC_READ_HOLDING_REGISTERS);

   modbus_serial_putc(make8(start_address,1));
   modbus_serial_putc(make8(start_address,0));

   modbus_serial_putc(make8(quantity,1));
   modbus_serial_putc(make8(quantity,0));

   modbus_serial_send_stop();

   return 0;
}
/*
read_input_registers
Input:     int8       address            Slave Address
           int16      start_address      Address to start reading from
           int16      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception master_modbus_read_input_registers(uint8_t address, uint16_t start_address, uint16_t quantity)
{
   modbus_serial_send_start(address, FUNC_READ_INPUT_REGISTERS);

   modbus_serial_putc(make8(start_address,1));
   modbus_serial_putc(make8(start_address,0));

   modbus_serial_putc(make8(quantity,1));
   modbus_serial_putc(make8(quantity,0));

   modbus_serial_send_stop();


   return 0;
}
void read_for_me(char _func)
{
	if(_func ==1) master_modbus_read_coils(MODBUS_SLAVE_ADDRESS,0,8);
	else if(_func ==2) master_modbus_read_discrete_input(MODBUS_SLAVE_ADDRESS,0,8);
	else if(_func ==3) master_modbus_read_holding_registers(MODBUS_SLAVE_ADDRESS,0,8);
	else if(_func ==4) master_modbus_read_input_registers(MODBUS_SLAVE_ADDRESS,0,8);
		 
	 
}
void modbus_master_exe(uint8_t *uart_buff,uint8_t length)
{ 
	uint8_t i;
	//printf("UART BUFF:%s ; %d\r\n",uart_buff,length);
	for(i=0;i<length;i++)
	 incomming_modbus_serial( *(uart_buff+i));
	modbus_timeout_now();
	
	if(modbus_kbhit() == TRUE)
	 {
	   if((modbus_rx.address == MODBUS_SLAVE_ADDRESS) || modbus_rx.address == 0)
      {
				//printf("FUNC:%d\r\n",modbus_rx.func);
				switch(modbus_rx.func)
         {
						case FUNC_READ_COILS:    //read coils
										printf("Coils: ");
										for(i=1; i < (modbus_rx.len); ++i)
										printf("%02X ", modbus_rx.data[i]);
										break;
						case FUNC_READ_DISCRETE_INPUT:    //read inputs	
										printf("Inputs: ");
										for(i=1; i < (modbus_rx.len); ++i)
										printf("%02X ", modbus_rx.data[i]);
										break;
						case FUNC_READ_HOLDING_REGISTERS: 
										printf("Holding Registers: ");
										for(i=1; i < (modbus_rx.len); ++i)
										printf("%02X ", modbus_rx.data[i]);
										break;
						case FUNC_READ_INPUT_REGISTERS:
										printf("Input Registers: ");
										for(i=1; i < (modbus_rx.len); ++i)
										printf("%02X ", modbus_rx.data[i]);
						//default:    //We don't support the function, so return exception

						/*Started at 1 since 0 is quantity of coils*/
						
				 }
			}				
		 
	 }

}
#endif


