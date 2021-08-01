



#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define display_tick	4000	//2mS assuming a clock of 2.22MHz (OSCM20 generates 17.76MHz instead of 16MHz)	
#define comms_tick		200	//111		//500mS per transaction
#define half_comms_tick	100	//55		

#define wait_for_clock_tick \

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
volatile char transaction_type;
volatile char transaction_type = 1;
volatile int transaction_counter = 0;
volatile int cmp0_bkp;
volatile char cal_factor, transaction_complete;
char data[30];

char display_buffer[8];
volatile int display_ptr;


#include "../Resources/One_wire_transactions.c"
#include "../Resources/Display_header_V2A.h"

