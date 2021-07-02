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


#define Baud_rate_19200	 0,174
#define Baud_rate_42K	 0,80
//160 is OK 120/36 is OK	100/30 is OK	80/24.4 is OK				50/16.8 is OK
#define Stop_Timer_TCB TCB0.CTRLA &= 0xFE;

void Start_Timer_TCA(char, unsigned char);
void UART_Tx(char, char, unsigned char);
unsigned char ReverseByte (unsigned char);
void Start_Timer_TCB(void);


volatile int TCB0_counter;

int main(void)
{
  char symbol;
  
  TCB0_counter = 0;
  
  PORTA.DIR &= ~PIN3_bm;											//Configure as input
  PORTA.PIN3CTRL |= PORT_PULLUPEN_bm;								//Pull-up DOES work
  
 PORTC.DIR &= ~PIN3_bm;											//Configure as input
PORTC.PIN3CTRL |= PORT_PULLUPEN_bm;								//Pull-up not working
while (PORTC.IN & (PIN3_bm));

 PORTC.DIR |= PIN3_bm;											//Configure pin as output
 PORTC.OUT |= PIN3_bm;											//Set output high; Start bit
 
 PORTC.DIRSET |= PIN0_bm;										//Configure as output
 PORTC.OUTSET |= PIN0_bm;
 _delay_ms(10);

/*******************************************/
sei();
 Start_Timer_TCB();
/**********************************************/
 symbol = 'A';
 while(1){
 if(!(TCB0_counter)){Stop_Timer_TCB; UART_Tx (symbol++, Baud_rate_42K );
	 if (symbol == 91)symbol = 65;TCB0_counter += 1;
	Start_Timer_TCB(); }
 }}
  
  
  
void Start_Timer_TCA(char prescaler, unsigned char step_value){
	TCA0.SPLIT.CTRLD |= TCA_SINGLE_SPLITM_bm;						//Split registers
	TCA0.SPLIT.LPER = 0xFF;											//Set top value
	TCA0.SPLIT.CTRLB |= TCA_SPLIT_LCMP0EN_bm;						//Low byte Compare 0
	TCA0.SPLIT.LCMP0 = 0xFF - step_value;							//First compare value
	TCA0.SPLIT.CTRLA = (prescaler*2) + 1;}
	
	
	
	void UART_Tx(char symbol, char prescaler, unsigned char step_value){
	
	cli();
	
	PORTC.DIR |= PIN3_bm;														//Configure pin as output
	PORTC.OUT |= PIN3_bm;														//Set output high; Start bit
	Start_Timer_TCA(prescaler, step_value);
	
	while (!(TCA0.SPLIT.INTFLAGS & TCA_SPLIT_LCMP0_bm));			//waiting for compare
	TCA0.SPLIT.LCMP0 -= step_value;									//Next compare value
	TCA0.SPLIT.INTFLAGS |= TCA_SPLIT_LCMP0_bm;
	
	PORTC.OUT &= ~(PIN3_bm);
	while (!(TCA0.SPLIT.INTFLAGS & TCA_SPLIT_LCMP0_bm));			//waiting for compare
	TCA0.SPLIT.LCMP0 -= step_value;									//Next compare value
	TCA0.SPLIT.INTFLAGS |= TCA_SPLIT_LCMP0_bm;
	
	for(int m = 0; m <= 7; m++){												//Eight data bits
		if (symbol & (1 << (7-m)))PORTC.OUT |= PIN3_bm;
		else PORTC.OUT &= ~(PIN3_bm);
		while (!(TCA0.SPLIT.INTFLAGS & TCA_SPLIT_LCMP0_bm));			//waiting for compare
		TCA0.SPLIT.LCMP0 -= step_value;									//Next compare value
		TCA0.SPLIT.INTFLAGS |= TCA_SPLIT_LCMP0_bm;}
	
	PORTC.OUT |= PIN3_bm;														//Stop bit
	while (!(TCA0_SINGLE_INTFLAGS & 0x01));										//Wait for OVF
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;									//Reset OVF flag
	while (!(TCA0.SPLIT.INTFLAGS & TCA_SPLIT_LCMP0_bm));			//waiting for compare
	//TCA0.SPLIT.LCMP0 -= step_value;									//Next compare value
	TCA0.SPLIT.INTFLAGS |= TCA_SPLIT_LCMP0_bm;
	TCA0_SINGLE_CTRLA = 0;													//Stop timer
	sei();
	}


void Start_Timer_TCB(void){
	TCB0.INTCTRL |= 1;
	TCB0.CCMP = 6666;													//Set top value for 2ms interrupt
	TCB0.CTRLA |= +1;}

ISR(TCB0_INT_vect){
	TCB0.INTFLAGS |= 1;
TCB0_counter += 1;
TCB0_counter = TCB0_counter%9;
if (!(TCB0_counter)){
//TCB0_counter = 0;
if(PORTC.OUT & PIN0_bm)PORTC.OUT &= ~PIN0_bm;
else PORTC.OUT |= PIN0_bm;}}

