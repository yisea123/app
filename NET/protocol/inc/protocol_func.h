#ifndef _PROTOCOL_FUNC_H_
#define _PROTOCOL_FUNC_H_

#include "stm32f10x.h"

unsigned char add_checksum (unsigned char *buf, unsigned int len);  

void send_command_ack( void *input_data, uint8_t uart_idx);

void parse_board_test_request(uint8_t *outputdata, uint8_t *inputdata); 

void print_board_test_request(uint8_t *data);

void print_push_medicine_request(uint8_t *data);

void parse_push_medicine_request(uint8_t *outputdata, uint8_t *inputdata);  

void print_replenish_medicine_request(uint8_t *data);
  
void parse_replenish_medicine_request(uint8_t *outputdata, uint8_t *inputdata);

void print_replenish_complete_request(uint8_t *data);

void parse_replenish_complete_request(uint8_t *outputdata, uint8_t *inputdata);

void parse_track_runtime_calc_request(uint8_t *outputdata, uint8_t *inputdata);

void print_track_runtime_calc_request(uint8_t *data);

void parse_message_ack(uint8_t *outputdata, uint8_t *inputdata); 
	
void print_status_report_request(uint8_t *data);

void FactoryFuncTest(void);

#endif
