



#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define display_tick	5600	//2mS assuming a clock of 2.8MHz	
#define comms_tick		140		//20K Baud rate (2.8MHz clock not 2MHz????)	500mS per transaction
#define half_comms_tick	70		

#define wait_for_clock_tick \\while (!(TCA0_SINGLE_INTFLAGS & TCA_SINGLE_CMP0EN_bm));\TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;

#define inc_display_clock			TCA0_SINGLE_CMP0 += display_tick;
#define inc_comms_clock				TCA0_SINGLE_CMP0 += comms_tick;
#define inc_half_comms_clock		TCA0_SINGLE_CMP0 += half_comms_tick;


void Start_TCA0(void);
void UART_transaction(void);
void comms_transaction(void);

void Display_driver(void);
void Tx_transaction (char);
char Rx_transaction (void);


volatile char Tx_symbol, Rx_symbol, Rx_symbol_bkp;
volatile char transaction_type;
volatile char transaction_type = 1;
volatile int transaction_counter = 0;
volatile int cmp0_bkp;
volatile char test;


char data[10];

#include "../Resources/One_wire_transactions.c"


