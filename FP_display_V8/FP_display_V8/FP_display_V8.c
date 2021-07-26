/*
 * FP_display_V7.c
 *
 * Created: 11/07/2021 09:18:42
 * Author : Mark
 */ 


#include "Project.h"






volatile char TCA0_counter;
volatile char display_digit;







int main(void)
{	
	//CLKCTRL_OSC20MCALIBA = 0x60;
	CLKCTRL_MCLKCTRLB |= CLKCTRL_PDIV_8X_gc | 1;				//Generates 2MHz Peripheral clock
	PORTC.OUTSET &= ~PIN0_bm;									//Led port
	PORTC.DIRSET |= PIN0_bm;									//output low
   
   PORTC.OUTSET &= ~PIN1_bm;									//Led port
   PORTC.DIRSET |= PIN1_bm;										//output low
   
   
   PORTC.DIR &= ~PIN3_bm;										//Configure comm port as input
   PORTC.OUT &= ~(PIN3_bm);										//I/O pin low when configured as output
   PORTC.PIN3CTRL |= PORT_PULLUPEN_bm;							//Pull-up enabled
   while (PORTC.IN & (PIN3_bm));								//Wait for low pulse from UNO
  while (!(PORTC.IN & (PIN3_bm)));								//Wait for end of pulse
   
   
   display_digit = 0;											//Next digit to be refreshed or updated
    
   
  Rx_symbol = 'a';
  Rx_symbol_bkp = Rx_symbol;
  TCA0_counter = 0;
   Start_TCA0();												//Display (2mS) tick rate
    
	test = 'A';
	sei();
	
	while(1);}



/*****************************************************************************************************************************/
	void Start_TCA0(void){
	TCA0_counter = 0;
	TCA0_SINGLE_CNT = 0;										//Initialise counter
	TCA0_SINGLE_CMP0 = display_tick;							//2mS period for 2MHz clock
	TCA0_SINGLE_CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | 1;			//Start clock with 2MHz clock
	TCA0_SINGLE_INTCTRL |= TCA_SINGLE_CMP0EN_bm;}				//Interrupt flag on compare match zero/*****************************************************************************************************************************/	void Display_driver(void){	PORTC.OUTTGL |= PIN0_bm;	display_digit += 1;}				ISR (TCA0_CMP0_vect){											//Tx works but not Rx	TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;	TCA0_counter += 1;	if (TCA0_counter <= 7)		{Display_driver();	inc_display_clock; return;}		cmp0_bkp = TCA0_SINGLE_CMP0 + display_tick;	inc_comms_clock;	PORTC.DIR |= PIN3_bm;										//Output low: Start bit	comms_transaction();	TCA0_SINGLE_CMP0 = cmp0_bkp;	TCA0_counter = 0;}						