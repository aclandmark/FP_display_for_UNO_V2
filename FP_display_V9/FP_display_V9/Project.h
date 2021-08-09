



#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#define display_tick	4000	//2mS assuming a clock of 2.22MHz (OSCM20 generates 17.76MHz instead of 16MHz)	
#define comms_tick		200	//111		//500mS per transaction
#define half_comms_tick	100	//55		

#define wait_for_clock_tick \while (!(TCA0_SINGLE_INTFLAGS & TCA_SINGLE_CMP0EN_bm));\TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;

#define inc_display_clock			TCA0_SINGLE_CMP0 += display_tick;
#define inc_comms_clock				TCA0_SINGLE_CMP0 += comms_tick;
#define inc_half_comms_clock		TCA0_SINGLE_CMP0 += half_comms_tick;


void Start_TCA0(void);
void UART_transaction(void);
void comms_transaction(void);

void Display_driver(void);
void Transmit_data_byte (char);
char Receive_data_byte (void);
void Char_definition(void);

volatile char Tx_symbol, Rx_symbol, Rx_symbol_bkp;
volatile char transaction_type, transaction_complete;
volatile int byte_counter;

volatile int cmp0_bkp;
volatile char cal_factor;
volatile char test_symbol= 1;
char data[10];

char display_buffer[15];		//12
char temp_buffer[15];		//8
volatile int display_ptr;
volatile long Long_Num_from_UNO, Long_Num_to_UNO;
volatile char cr_keypress;
unsigned char data_byte[4];
volatile int data_byte_ptr;
volatile float Float_Num_to_UNO, Float_Num_from_UNO;
volatile char * char_ptr, * char_ptr_2;
volatile float * float_ptr, *float_ptr_2;

#define display_buffer2temp \
null_bit_counter = 0; \
clear_temp_buffer;\
for(int m = 0; m <= 14; m++)\
{if(!(display_buffer[14-m]))null_bit_counter += 1; else break;}\
for(int m = 0; m <= 14-null_bit_counter; m++)\
temp_buffer[m] = display_buffer[14-null_bit_counter - m];

#define temp2display_buffer \
null_bit_counter = 0;\
for(int m = 0; m <= 14; m++)\
{if(!(temp_buffer[14-m]))null_bit_counter += 1; else break;}\
for(int m = 0; m <= 14-null_bit_counter; m++)\
display_buffer[m] = temp_buffer[14-null_bit_counter - m];


#include "../Resources/One_wire_transactions.c"
#include "../Resources/Display_header.h"
#include "../Resources/FPN_subroutines.c"

