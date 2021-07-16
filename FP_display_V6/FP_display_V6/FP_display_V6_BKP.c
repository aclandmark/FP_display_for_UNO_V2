/*
 * FP_display_V6.c
 *
 * Created: 11/07/2021 09:18:42
 * Author : Mark
 */ 
void Start_TCA0(void);
void UART_transaction(void);
void Comms_transaction(char);
void Display_driver(void);


volatile char TCA0_counter;
volatile char display_digit;


#define F_CPU 2000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{	char Tx_symbol = 0;
	
	CLKCTRL_MCLKCTRLB |= CLKCTRL_PDIV_8X_gc;					//Generates 2MHz Peripheral clock
	PORTC.OUTSET &= ~PIN0_bm;									//Led port
	PORTC.DIRSET |= PIN0_bm;									//output low
   
   PORTC.DIR &= ~PIN3_bm;										//Configure comm port as input
   PORTC.OUT &= ~(PIN3_bm);										//I/O pin low when configured as output
   PORTC.PIN3CTRL |= PORT_PULLUPEN_bm;							//Pull-up enabled
   while (PORTC.IN & (PIN3_bm));									//Wait for low pulse from UNO
   //_delay_ms(5);

   
   
   display_digit = 0;											//Next digit to be refreshed or updated
   //sei();
   Start_TCA0();												//Slow (2mS) tick rate
      while(1){
   for (int m = 0; m < 7; m++)
   {Display_driver();											//Drive digits zero to six
   TCA0_SINGLE_CMP0 += 8000;}									//Maintain slow tick rate
   Display_driver();											//Drive digit 7
   TCA0_SINGLE_CMP0 += 2000;									//Swap to fast tick rate
   Comms_transaction(Tx_symbol);}
   
   
   /*while(1){TCA0_counter = 0;
    while (TCA0_counter < 8); 
	TCA0_counter = 0;
    TCA0_SINGLE_INTCTRL &= ~TCA_SINGLE_CMP0EN_bm;				//Disable interrupt
    while(TCA0_counter < 8)UART_transaction();
	TCA0_SINGLE_INTCTRL |= TCA_SINGLE_CMP0EN_bm;}*/
}

void Start_TCA0(void){
TCA0_counter = 0;
TCA0_SINGLE_CNT = 0;										//Initialise counter
TCA0_SINGLE_CMP0 = 8000;									//2mS period for 2MHz clock
//TCA0_SINGLE_CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | 1;			//Start clock with 2MHz clock
TCA0_SINGLE_CTRLA = TCA_SINGLE_CLKSEL_DIV256_gc | 1;		//Start clock with 7.8125KHz clock (512mS tick)TCA0_SINGLE_INTCTRL |= TCA_SINGLE_CMP0EN_bm;}				//Interrupt flag on compare match zeroISR(TCA0_CMP0_vect){TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;				//Clear interrupt flag	TCA0_counter += 1;//Display_driver();TCA0_SINGLE_CMP0 += 8000; PORTC.OUT |= PIN0_bm; _delay_ms(150); PORTC.OUT &= ~PIN0_bm;} void UART_transaction(void){while(!(TCA0_SINGLE_INTFLAGS & TCA_SINGLE_CMP0EN_bm)); 	TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;				//Clear interrupt flagTCA0_SINGLE_CMP0 += 4000;	TCA0_counter += 1;	PORTC.OUT |= PIN0_bm;_delay_ms(15);PORTC.OUT &= ~PIN0_bm;}void Display_driver(void){		PORTC.OUT |= PIN0_bm;	_delay_ms(250);	PORTC.OUT &= ~PIN0_bm;	while (!(TCA0_SINGLE_INTFLAGS & TCA_SINGLE_CMP0EN_bm));		//Wait for interrupt flag	TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;				//Clear interrupt flag	display_digit += 1;	}		void Comms_transaction(char symbol){			for (int m = 0; m <= 10; m++){				PORTC.OUT |= PIN0_bm;		_delay_ms(50);		PORTC.OUT &= ~PIN0_bm;				while (!(TCA0_SINGLE_INTFLAGS & TCA_SINGLE_CMP0EN_bm));		//Wait for interrupt flag		TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;		TCA0_SINGLE_CMP0 += 2000;	}	}