//////////////////////////////////////////////////////////////////////////////////////////
////                                modbus_app_layer.h                                ////
////                                                                                  ////
////    Application layer of the MODBUS protocol driver for serial communications.    ////
////                                                                                  ////
////  Refer to documentation at http://www.modbus.org for more information on MODBUS. ////
////                                                                                  ////
//////////////////////////////////////////////////////////////////////////////////////////
////                                                                                  ////
//// Revision history:                                                                ////
////  July 20, 2011       Seperated App Layer functions into this file from modbus.c  ////
////                                                                                  ////
//////////////////////////////////////////////////////////////////////////////////////////
////                (C) Copyright 1996, 2013 Custom Computer Services                 ////
////        This source code may only be used by licensed users of the CCS            ////
////        C compiler.  This source code may only be distributed to other            ////
////        licensed users of the CCS C compiler.  No other use,                      ////
////        reproduction or distribution is permitted without written                 ////
////        permission.  Derivative programs created using this software              ////
////        in object code form are not restricted in any way.                        ////
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef MODBUS_APP_LAYER_H
#define MODBUS_APP_LAYER_H
#include "modbus_define.h"
#include "modbus_phy_layer.h"

//////////////////////////////////////////////////////////////////////////////////////////
//// Shared API                                                                       ////
////                                                                                  ////
////  modbus_init()                                                                   ////
////    - Initialize modbus serial communication system                               ////
////                                                                                  ////
////  modbus_kbhit()                                                                  ////
////    - Used to check if a packet has been received.                                ////
////                                                                                  ////
//////////////////////////////////////////////////////////////////////////////////////////

// Purpose:    Initialize RS485 communication. Call this before
//             using any other RS485 functions.
// Inputs:     None
// Outputs:    None
void modbus_init(void);

// Purpose:    Get a message from the RS485 bus and store it in a buffer
// Inputs:     None
// Outputs:    TRUE if a message was received
//             FALSE if no message is available
// Note:       Data will be filled in at the modbus_rx struct:
uint8_t modbus_kbhit(void);

#if (MODBUS_TYPE==MODBUS_TYPE_MASTER)
//////////////////////////////////////////////////////////////////////////////////////////
//// Master Api                                                                       ////
////                                                                                  ////
////  All master API functions return 0 on success.                                   ////
////                                                                                  ////
////  exception modbus_read_coils(address,start_address,quantity)                     ////
////    - Wrapper for function 0x01(read coils) in the MODBUS specification.          ////
////                                                                                  ////
////  exception modbus_read_discrete_input(address,start_address,quantity)            ////
////    - Wrapper for function 0x02(read discret input) in the MODBUS specification.  ////
////                                                                                  ////
////  exception modbus_read_holding_registers(address,start_address,quantity)         ////
////    - Wrapper for function 0x03(read holding regs) in the MODBUS specification.   ////
////                                                                                  ////
////  exception modbus_read_input_registers(address,start_address,quantity)           ////
////    - Wrapper for function 0x04(read input regs) in the MODBUS specification.     ////
////                                                                                  ////
////  exception modbus_write_single_coil(address,output_address,on)                   ////
////    - Wrapper for function 0x05(write single coil) in the MODBUS specification.   ////
////                                                                                  ////
////  exception modbus_write_single_register(address,reg_address,reg_value)           ////
////    - Wrapper for function 0x06(write single reg) in the MODBUS specification.    ////
////                                                                                  ////
////  exception modbus_read_exception_status(address)                                 ////
////    - Wrapper for function 0x07(read void status) in the MODBUS specification.    ////
////                                                                                  ////
////  exception modbus_diagnostics(address,sub_func,data)                             ////
////    - Wrapper for function 0x08(diagnostics) in the MODBUS specification.         ////
////                                                                                  ////
////  exception modbus_get_comm_event_counter(address)                                ////
////    - Wrapper for function 0x0B(get comm event count) in the MODBUS specification.////
////                                                                                  ////
////  exception modbus_get_comm_event_log(address)                                    ////
////    - Wrapper for function 0x0C(get comm event log) in the MODBUS specification.  ////
////                                                                                  ////
////  exception modbus_write_multiple_coils(address,start_address,quantity,*values)   ////
////    - Wrapper for function 0x0F(write multiple coils) in the MODBUS specification.////
////    - Special Note: values is a pointer to an uint8_t array, each byte represents 8  ////
////                    coils.                                                        ////
////                                                                                  ////
////  exception modbus_write_multiple_registers(address,start_address,quantity,*values)///
////    - Wrapper for function 0x10(write multiple regs) in the MODBUS specification. ////
////    - Special Note: values is a pointer to an uint8_t array                          ////
////                                                                                  ////
////  exception modbus_report_slave_id(address)                                       ////
////    - Wrapper for function 0x11(report slave id) in the MODBUS specification.     ////
////                                                                                  ////
////  exception modbus_read_file_record(address,byte_count,*request)                  ////
////    - Wrapper for function 0x14(read file record) in the MODBUS specification.    ////
////                                                                                  ////
////  exception modbus_write_file_record(address,byte_count,*request)                 ////
////    - Wrapper for function 0x15(write file record) in the MODBUS specification.   ////
////                                                                                  ////
////  exception modbus_mask_write_register(address,reference_address,AND_mask,OR_mask)////
////    - Wrapper for function 0x16(read coils) in the MODBUS specification.          ////
////                                                                                  ////
////  exception modbus_read_write_multiple_registers(address,read_start,read_quantity,////
////                            write_start,write_quantity, *write_registers_value)   ////
////    - Wrapper for function 0x17(read write mult regs) in the MODBUS specification.////
////                                                                                  ////
////  exception modbus_read_FIFO_queue(address,FIFO_address)                          ////
////    - Wrapper for function 0x18(read FIFO queue) in the MODBUS specification.     ////
////                                                                                  ////
//////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
The following structs are used for read/write_sub_request.  These
functions take in one of these structs.
Please refer to the MODBUS protocol specification if you do not
understand the members of the structure.
********************************************************************/
typedef struct _modbus_read_sub_request
{
    uint8_t reference_type;
    uint16_t file_number;
    uint16_t record_number;
    uint16_t record_length;
} modbus_read_sub_request;

typedef struct _modbus_write_sub_request
{
    uint8_t reference_type;
    uint16_t file_number;
    uint16_t record_number;
    uint16_t record_length;
    uint16_t data[MODBUS_SERIAL_RX_BUFFER_SIZE-8];
} modbus_write_sub_request;

/********************************************************************
The following functions are defined in the MODBUS protocol.  Please
refer to http://www.modbus.org for the purpose of each of these.
All functions take the slaves address as their first parameter.
Each function returns the exception code received from the response.
The function will return 0 if there were no errors in transmission.
********************************************************************/

/*
read_coils
Input:     uint8_t       address            Slave Address
           uint16_t      start_address      Address to start reading from
           uint16_t      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception modbus_read_coils( uint8_t address,  uint16_t start_address,  uint16_t quantity);

/*
read_discrete_input
Input:     uint8_t       address            Slave Address
           uint16_t      start_address      Address to start reading from
           uint16_t      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception modbus_read_discrete_input(  uint8_t address,   uint16_t start_address,   uint16_t quantity);
/*
read_holding_registers
Input:     uint8_t       address            Slave Address
           uint16_t      start_address      Address to start reading from
           uint16_t      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception modbus_read_holding_registers(  uint8_t address,   uint16_t start_address,   uint16_t quantity);

/*
read_input_registers
Input:     uint8_t       address            Slave Address
           uint16_t      start_address      Address to start reading from
           uint16_t      quantity           Amount of addresses to read
Output:    exception                     0 if no error, else the exception
*/
exception modbus_read_input_registers(  uint8_t address,   uint16_t start_address,   uint16_t quantity);

/*
write_single_coil
Input:     uint8_t       address            Slave Address
           uint16_t      output_address     Address to write into
           int1       on                 true for on, false for off
Output:    exception                     0 if no error, else the exception
*/
exception modbus_write_single_coil(  uint8_t address,   uint16_t output_address, uint8_t on);

/*
write_single_register
Input:     uint8_t       address            Slave Address
           uint16_t      reg_address        Address to write into
           uint16_t      reg_value          Value to write
Output:    exception                     0 if no error, else the exception
*/
exception modbus_write_single_register(  uint8_t address,   uint16_t reg_address,   uint16_t reg_value);

/*
read_exception_status
Input:     uint8_t       address            Slave Address
Output:    exception                     0 if no error, else the exception
*/
exception modbus_read_exception_status(  uint8_t address);

/*
diagnostics
Input:     uint8_t       address            Slave Address
           uint16_t      sub_func           Subfunction to send
           uint16_t      data               Data to send, changes based on subfunction
Output:    exception                     0 if no error, else the exception
*/
exception modbus_diagnostics(  uint8_t address,   uint16_t sub_func,   uint16_t data);

/*
get_comm_event_couter
Input:     uint8_t       address            Slave Address
Output:    exception                     0 if no error, else the exception
*/
exception modbus_get_comm_event_counter(  uint8_t address);

/*
get_comm_event_log
Input:     uint8_t       address            Slave Address
Output:    exception                     0 if no error, else the exception
*/
exception modbus_get_comm_event_log(  uint8_t address);

/*
write_multiple_coils
Input:     uint8_t       address            Slave Address
           uint16_t      start_address      Address to start at
           uint16_t      quantity           Amount of coils to write to
           int1*      values             A pointer to an array holding the values to write
Output:    exception                     0 if no error, else the exception
*/
exception modbus_write_multiple_coils(  uint8_t address,   uint16_t start_address,   uint16_t quantity,
                             uint8_t *values);

/*
write_multiple_registers
Input:     uint8_t       address            Slave Address
           uint16_t      start_address      Address to start at
           uint16_t      quantity           Amount of coils to write to
           uint16_t*     values             A pointer to an array holding the data to write
Output:    exception                     0 if no error, else the exception
*/
exception modbus_write_multiple_registers(  uint8_t address,   uint16_t start_address,   uint16_t quantity,
                             uint16_t *values);

/*
report_slave_id
Input:     uint8_t       address            Slave Address
Output:    exception                     0 if no error, else the exception
*/
exception modbus_report_slave_id(  uint8_t address);

/*
read_file_record
Input:     uint8_t                address            Slave Address
           uint8_t                byte_count         Number of bytes to read
           read_sub_request*   request            Structure holding record information
Output:    exception                              0 if no error, else the exception
*/
exception modbus_read_file_record(  uint8_t address,   uint8_t byte_count,
                            modbus_read_sub_request *request);

/*
write_file_record
Input:     uint8_t                address            Slave Address
           uint8_t                byte_count         Number of bytes to read
           read_sub_request*   request            Structure holding record/data information
Output:    exception                              0 if no error, else the exception
*/
exception modbus_write_file_record(  uint8_t address,   uint8_t byte_count,
                            modbus_write_sub_request *request);

/*
mask_write_register
Input:     uint8_t       address            Slave Address
           uint16_t      reference_address  Address to mask
           uint16_t      AND_mask           A mask to AND with the data at reference_address
           uint16_t      OR_mask            A mask to OR with the data at reference_address
Output:    exception                              0 if no error, else the exception
*/
exception modbus_mask_write_register(  uint8_t address,   uint16_t reference_address,
                             uint16_t AND_mask,   uint16_t OR_mask);

/*
read_write_multiple_registers
Input:     uint8_t       address                Slave Address
           uint16_t      read_start             Address to start reading
           uint16_t      read_quantity          Amount of registers to read
           uint16_t      write_start            Address to start writing
           uint16_t      write_quantity         Amount of registers to write
           uint16_t*     write_registers_value  Pointer to an aray us to write
Output:    exception                         0 if no error, else the exception
*/
exception modbus_read_write_multiple_registers(  uint8_t address,   uint16_t read_start,
                                      uint16_t read_quantity,   uint16_t write_start,
                                      uint16_t write_quantity,
                                      uint16_t *write_registers_value);

/*
read_FIFO_queue
Input:     uint8_t       address           Slave Address
           uint16_t      FIFO_address      FIFO address
Output:    exception                    0 if no error, else the exception
*/
exception modbus_read_FIFO_queue(  uint8_t address,   uint16_t FIFO_address);

#else
//////////////////////////////////////////////////////////////////////////////////////////
//// Slave API                                                                        ////
////                                                                                  ////
////  void modbus_read_coils_rsp(address,byte_count,*coil_data)                       ////
////    - Wrapper to respond to 0x01(read coils) in the MODBUS specification.         ////
////                                                                                  ////
////  void modbus_read_discrete_input_rsp(address,byte_count,*input_data)             ////
////    - Wrapper to respond to 0x02(read discret input) in the MODBUS specification. ////
////                                                                                  ////
////  void modbus_read_holding_registers_rsp(address,byte_count,*reg_data)            ////
////    - Wrapper to respond to 0x03(read holding regs) in the MODBUS specification.  ////
////                                                                                  ////
////  void modbus_read_input_registers_rsp(address,byte_count,*input_data)            ////
////    - Wrapper to respond to 0x04(read input regs) in the MODBUS specification.    ////
////                                                                                  ////
////  void modbus_write_single_coil_rsp(address,output_address,output_value)          ////
////    - Wrapper to respond to 0x05(write single coil) in the MODBUS specification.  ////
////                                                                                  ////
////  void modbus_write_single_register_rsp(address,reg_address,reg_value)            ////
////    - Wrapper to respond to 0x06(write single reg) in the MODBUS specification.   ////
////                                                                                  ////
////  void modbus_read_exception_status_rsp(address, data)                            ////
////    - Wrapper to respond to 0x07(read void status) in the MODBUS specification.   ////
////                                                                                  ////
////  void modbus_diagnostics_rsp(address,sub_func,data)                              ////
////    - Wrapper to respond to 0x08(diagnostics) in the MODBUS specification.        ////
////                                                                                  ////
////  void modbus_get_comm_event_counter_rsp(address,status,event_count)              ////
////    - Wrapper to respond to 0x0B(get comm event count) in the MODBUS specification////
////                                                                                  ////
////  void modbus_get_comm_event_log_rsp(address,status,event_count,message_count,    ////
////                                   *events, events_len)                           ////
////    - Wrapper to respond to 0x0C(get comm event log) in the MODBUS specification. ////
////                                                                                  ////
////  void modbus_write_multiple_coils_rsp(address,start_address,quantity)            ////
////    - Wrapper to respond to 0x0F(write multiple coils) in the MODBUS specification////
////                                                                                  ////
////  void modbus_write_multiple_registers_rsp(address,start_address,quantity)        ////
////    - Wrapper to respond to 0x10(write multiple regs) in the MODBUS specification.////
////                                                                                  ////
////  void modbus_report_slave_id_rsp(address,slave_id,run_status,*data,data_len)     ////
////    - Wrapper to respond to 0x11(report slave id) in the MODBUS specification.    ////
////                                                                                  ////
////  void modbus_read_file_record_rsp(address,byte_count,*request)                   ////
////    - Wrapper to respond to 0x14(read file record) in the MODBUS specification.   ////
////                                                                                  ////
////  void modbus_write_file_record_rsp(address,byte_count,*request)                  ////
////    - Wrapper to respond to 0x15(write file record) in the MODBUS specification.  ////
////                                                                                  ////
////  void modbus_mask_write_register_rsp(address,reference_address,AND_mask,OR_mask) ////
////    - Wrapper to respond to 0x16(read coils) in the MODBUS specification.         ////
////                                                                                  ////
////  void modbus_read_write_multiple_registers_rsp(address,*data,data_len)           ////
////    - Wrapper to respond to 0x17(read write mult regs) in the MODBUS specification////
////                                                                                  ////
////  void modbus_read_FIFO_queue_rsp(address,FIFO_len,*data)                         ////
////    - Wrapper to respond to 0x18(read FIFO queue) in the MODBUS specification.    ////
////                                                                                  ////
////  void modbus_exception_rsp(uint8_t address, uint16_t func, exception error)            ////
////    - Wrapper to send an exception response.  See exception list below.           ////
////                                                                                  ////
//// Exception List:                                                                  ////
////  ILLEGAL_FUNCTION, ILLEGAL_DATA_ADDRESS, ILLEGAL_DATA_VALUE,                     ////
////  SLAVE_DEVICE_FAILURE, ACKNOWLEDGE, SLAVE_DEVICE_BUSY, MEMORY_PARITY_ERROR,      ////
////  GATEWAY_PATH_UNAVAILABLE, GATEWAY_TARGET_NO_RESPONSE                            ////
////                                                                                  ////
//////////////////////////////////////////////////////////////////////////////////////////

/********************************************************************
The following structs are used for read/write_sub_request_rsp.  These
functions take in one of these structs.  Please refer to the MODBUS
protocol specification if you do not understand the members of the
structure.
********************************************************************/
typedef struct _modbus_read_sub_request_rsp
{
     uint8_t record_length;
     uint8_t reference_type;
     uint16_t data[((MODBUS_SERIAL_RX_BUFFER_SIZE)/2)-3];
} modbus_read_sub_request_rsp;

typedef struct _modbus_write_sub_request_rsp
{
     uint8_t reference_type;
     uint16_t file_number;
     uint16_t record_number;
     uint16_t record_length;
     uint16_t data[((MODBUS_SERIAL_RX_BUFFER_SIZE)/2)-8];
} modbus_write_sub_request_rsp;

/********************************************************************
The following slave functions are defined in the MODBUS protocol.
Please refer to http://www.modbus.org for the purpose of each of
these.  All functions take the slaves address as their first
parameter.
********************************************************************/

/*
read_coils_rsp
Input:     uint8_t       address            Slave Address
           uint8_t       byte_count         Number of bytes being sent
           uint8_t*      coil_data          Pointer to an array of data to send
Output:    void
*/
void modbus_read_coils_rsp(  uint8_t address,   uint8_t byte_count,   uint8_t* coil_data);

/*
read_discrete_input_rsp
Input:     uint8_t       address            Slave Address
           uint8_t       byte_count         Number of bytes being sent
           uint8_t*      input_data         Pointer to an array of data to send
Output:    void
*/
void modbus_read_discrete_input_rsp(  uint8_t address,   uint8_t byte_count,
                                      uint8_t *input_data);

/*
read_holding_registers_rsp
Input:     uint8_t       address            Slave Address
           uint8_t       byte_count         Number of bytes being sent
           uint8_t*      reg_data           Pointer to an array of data to send
Output:    void
*/
void modbus_read_holding_registers_rsp(  uint8_t address,   uint8_t byte_count,
                                          uint16_t *reg_data);

/*
read_input_registers_rsp
Input:     uint8_t       address            Slave Address
           uint8_t       byte_count         Number of bytes being sent
           uint8_t*      input_data         Pointer to an array of data to send
Output:    void
*/
void modbus_read_input_registers_rsp(  uint8_t address,   uint8_t byte_count,
                                          uint16_t *input_data);

/*
write_single_coil_rsp
Input:     uint8_t       address            Slave Address
           uint16_t      output_address     Echo of output address received
           uint16_t      output_value       Echo of output value received
Output:    void
*/
void modbus_write_single_coil_rsp(  uint8_t address,   uint16_t output_address,
                                      uint16_t output_value);

/*
write_single_register_rsp
Input:     uint8_t       address            Slave Address
           uint16_t      reg_address        Echo of register address received
           uint16_t      reg_value          Echo of register value received
Output:    void
*/
void modbus_write_single_register_rsp(  uint8_t address,   uint16_t reg_address,
                                          uint16_t reg_value);

/*
read_exception_status_rsp
Input:     uint8_t       address            Slave Address
Output:    void
*/
void modbus_read_exception_status_rsp(  uint8_t address,   uint8_t data);

/*
diagnostics_rsp
Input:     uint8_t       address            Slave Address
           uint16_t      sub_func           Echo of sub function received
           uint16_t      data               Echo of data received
Output:    void
*/
void modbus_diagnostics_rsp(  uint8_t address,   uint16_t sub_func,   uint16_t data);
/*
get_comm_event_counter_rsp
Input:     uint8_t       address            Slave Address
           uint16_t      status             Status, refer to MODBUS documentation
           uint16_t      event_count        Count of events
Output:    void
*/
void modbus_get_comm_event_counter_rsp(  uint8_t address,   uint16_t status,
                                          uint16_t event_count);

/*
get_comm_event_counter_rsp
Input:     uint8_t       address            Slave Address
           uint16_t      status             Status, refer to MODBUS documentation
           uint16_t      event_count        Count of events
           uint16_t      message_count      Count of messages
           uint8_t*      events             Pointer to event data
           uint8_t       events_len         Length of event data in bytes
Output:    void
*/
void modbus_get_comm_event_log_rsp(  uint8_t address,   uint16_t status,
                                      uint16_t event_count,   uint16_t message_count,
                                      uint8_t *events,   uint8_t events_len);

/*
write_multiple_coils_rsp
Input:     uint8_t       address            Slave Address
           uint16_t      start_address      Echo of address to start at
           uint16_t      quantity           Echo of amount of coils written to
Output:    void
*/
void modbus_write_multiple_coils_rsp(  uint8_t address,   uint16_t start_address,
                                          uint16_t quantity);

/*
write_multiple_registers_rsp
Input:     uint8_t       address            Slave Address
           uint16_t      start_address      Echo of address to start at
           uint16_t      quantity           Echo of amount of registers written to
Output:    void
*/
void modbus_write_multiple_registers_rsp(  uint8_t address,   uint16_t start_address,
                                              uint16_t quantity);

/*
report_slave_id_rsp
Input:     uint8_t       address            Slave Address
           uint8_t       slave_id           Slave Address
           uint8_t       run_status         Are we running?
           uint8_t*      data               Pointer to an array holding the data
           uint8_t       data_len           Length of data in bytes
Output:    void
*/
void modbus_report_slave_id_rsp(  uint8_t address,   uint8_t slave_id,   int1 run_status,
                                uint8_t *data,   uint8_t data_len);

/*
read_file_record_rsp
Input:     uint8_t                     address            Slave Address
           uint8_t                     byte_count         Number of bytes to send
           read_sub_request_rsp*    request            Structure holding record/data information
Output:    void
*/
void modbus_read_file_record_rsp(  uint8_t address,   uint8_t byte_count,
                                    modbus_read_sub_request_rsp *request);

/*
write_file_record_rsp
Input:     uint8_t                     address            Slave Address
           uint8_t                     byte_count         Echo of number of bytes sent
           write_sub_request_rsp*   request            Echo of Structure holding record information
Output:    void
*/
void modbus_write_file_record_rsp(  uint8_t address,   uint8_t byte_count,
                                    modbus_write_sub_request_rsp *request);

/*
mask_write_register_rsp
Input:     uint8_t        address            Slave Address
           uint16_t       reference_address  Echo of reference address
           uint16_t       AND_mask           Echo of AND mask
           uint16_t       OR_mask            Echo or OR mask
Output:    void
*/
void modbus_mask_write_register_rsp(  uint8_t address,   uint16_t reference_address,
                             uint16_t AND_mask,   uint16_t OR_mask);

/*
read_write_multiple_registers_rsp
Input:     uint8_t        address            Slave Address
           uint16_t*      data               Pointer to an array of data
           uint8_t        data_len           Length of data in bytes
Output:    void
*/
void modbus_read_write_multiple_registers_rsp(  uint8_t address,   uint8_t data_len,
                                                  uint16_t *data);

/*
read_FIFO_queue_rsp
Input:     uint8_t        address            Slave Address
           uint16_t       FIFO_len           Length of FIFO in bytes
           uint16_t*      data               Pointer to an array of data
Output:    void
*/
void modbus_read_FIFO_queue_rsp(  uint8_t address,   uint16_t FIFO_len,   uint16_t *data);

void modbus_exception_rsp(  uint8_t address,   uint16_t func, exception error);
#endif //MODBUS_TYPE

#endif //MODBUS_APP_LAYER_H
