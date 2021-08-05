



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
void Tx_transaction (char);
char Rx_transaction (void);
void Char_definition(void);

volatile char Tx_symbol, Rx_symbol, Rx_symbol_bkp;
volatile char transaction_type, transaction_complete;
volatile int byte_counter;

volatile int cmp0_bkp;
volatile char cal_factor;
volatile char test_symbol= 1;
char data[10];

char display_buffer[12];
char temp_buffer[8];
volatile int display_ptr;
volatile long Long_Num_from_UNO, Long_Num_to_UNO;
volatile char cr_keypress;
unsigned char data_byte[4];
volatile int data_byte_ptr;

#include "../Resources/One_wire_transactions.c"
#include "../Resources/Display_header_V2A.h"


