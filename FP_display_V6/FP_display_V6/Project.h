



#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define display_tick 390	
#define comms_tick 27	
#define half_comms_tick 13


#define wait_for_clock_tick \while (!(TCA0_SINGLE_INTFLAGS & TCA_SINGLE_CMP0EN_bm));\TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;

#define inc_display_clock			TCA0_SINGLE_CMP0 += display_tick;
#define inc_comms_clock				TCA0_SINGLE_CMP0 += comms_tick;
#define inc_half_comms_clock		TCA0_SINGLE_CMP0 += half_comms_tick;


void Start_TCA0(void);
void UART_transaction(void);
char Tx_Rx_transaction(char);
void Display_driver(void);




#include "../Resources/One_wire_transactions.c"


