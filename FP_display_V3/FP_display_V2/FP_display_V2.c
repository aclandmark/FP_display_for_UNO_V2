/*
 * FP_display_V2.c
ATtiny 1606 UART I/O PC3 pin 15:	Connected to SKT2 pin 2 (A4) Atmega 328 PC4


Clock setup:
Fuse OSC.CFG (p36) OSC20M runs at 20MHz on factory calibration

Register 
MCLKCTRLA (p84) OSC20M selected as main clock
MCLKCTRLB (p85) pre-scaler set to 6 gives CLK_PER(peripheral) of 3.333333 MHz

TCA.CTRLA sets the timer TCA clock frequency: A value of 5 gives CLK_PER/64 = 52.083 KHz 
Consider time of a 500ms.  TCA will count to  26042

Consider 19.2K Baud rate. Set TCA.CTRLA to 0 The counter reaches 174.

Tx frame Set output and start counter
Tx bit: Wait for OVF, Set output, clear OVF
At end of frame:	Wait for OVF, clear it and stop counter


TIMING SCHEME
Start TCB 2mS interrupt to drive the 8 digit display
On the eighth interrupt halt TCB and start TCA set up for 40K UART 250uS
At stop bit halt TCA and restart TCB 

UART Transmit first then receive
If zero ignore
if 'A': prepare to receive FP string of 12 bytes then return FPN 4 bytes
if 'B'
if 'C' prepare to receive FPN 4 bytes





*/ 


#define F_CPU 3333333UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define Baud_rate_13889	 0,240
#define Baud_rate_19200	 0,174
#define Baud_rate_42K	 0,80
//160 is OK 120/36 is OK	100/30 is OK	80/24.4 is OK				50/16.8 is OK

#define Baud_rate_timer \
while (!(TCA0.SPLIT.INTFLAGS & TCA_SPLIT_LCMP0_bm));\
TCA0.SPLIT.LCMP0 -= step_value;\
TCA0.SPLIT.INTFLAGS |= TCA_SPLIT_LCMP0_bm;

#define Stop_Timer_TCB TCB0.CTRLA &= 0xFE;

void Start_Baud_rate_Generator(char, unsigned char);
void UART_Tx(char, char, unsigned char);
unsigned char ReverseByte (unsigned char);
void Start_Timer_TCB(void);
void UART_Rx(char, unsigned char);
void UART_Rx_Tx(char, unsigned char);
void UART_Tx_Rx(char, char, unsigned char);



volatile int TCB0_counter;
volatile char Rx_symbol;

volatile char test_counter = 0;

int main(void)
{
TCB0_counter = 0;

PORTC.DIR &= ~PIN3_bm;											//Configure as input
PORTC.OUT &= ~(PIN3_bm);										//I/O pin low when configured as output
PORTC.PIN3CTRL |= PORT_PULLUPEN_bm;								//Pull-up enabled
while (PORTC.IN & (PIN3_bm));									//Wait for low pulse from UNO

 _delay_ms(5);

PORTC.DIRSET |= PIN0_bm;										//Configure as output

for (int m = 0; m <= 23; m++){
Rx_symbol = 1;
for(int m = 0; m <=91; m++){
UART_Tx_Rx(Rx_symbol + 33, Baud_rate_13889);
//_delay_ms(1);
}}
PORTC.OUTSET |= PIN0_bm;
PORTC.DIRSET |= PIN1_bm;
 }
 
 
 
 
 
 void UART_Tx_Rx(char Tx_symbol, char prescaler, unsigned char step_value){
	 	 
	 cli();
	 Start_Baud_rate_Generator(prescaler, step_value);
	Baud_rate_timer;

	 PORTC.DIR |= PIN3_bm;											//Output low
	Baud_rate_timer;												//Start bit

	 for(int m = 0; m <= 7; m++){									//Eight data bits
		 if (Tx_symbol & (1 << (7-m)))PORTC.DIR &= ~PIN3_bm;		//WPU
		 else PORTC.DIR |= PIN3_bm;									//Output low
	 Baud_rate_timer;
	 }

	 PORTC.DIR &= ~PIN3_bm;											//WPU Stop bit
	Baud_rate_timer;
	 TCA0_SINGLE_CTRLA = 0;											//Stop baud rate generator

	 sei();
	 	 
	 Rx_symbol = 0;
	 PORTC.PIN3CTRL |=  PORT_ISC_FALLING_gc;						//Interrupt on falling edge
	 sei();
	 while(!(Rx_symbol));
	 PORTC.PIN3CTRL |= PORT_ISC_INTDISABLE_gc;}

	
ISR(PORTC_PORT_vect){
	if((PORTC.IN & PIN3_bm) ^ PIN3_bm){
		_delay_us(36);
		for (int m = 0; m <= 7;m++){
			Rx_symbol = Rx_symbol << 1;
			_delay_us(72);
			if (PORTC.IN & (PIN3_bm)){Rx_symbol |= 1;}}
			_delay_us(72);	
			}}	
	
	
	
 /***********************************************************************************************************************************/
 void Start_Baud_rate_Generator
 (char prescaler, unsigned char step_value){							//TCA timer
	 TCA0.SPLIT.CTRLD |= TCA_SINGLE_SPLITM_bm;						//Split registers
	 TCA0.SPLIT.LPER = 0xFF;											//Set top value
	 TCA0.SPLIT.CTRLB |= TCA_SPLIT_LCMP0EN_bm;						//Low byte Compare 0
	 TCA0.SPLIT.LCMP0 = 0xFF - step_value;							//First compare value
 TCA0.SPLIT.CTRLA = (prescaler*2) + 1;}

 
 
 
 
 