/*
 * FP_display_V7.c
 *
 * Created: 11/07/2021 09:18:42
 * Author : Mark
 */ 


#include "Project.h"






volatile char TCA0_counter;


int main(void)
{	char cal_factor;
	char null_bit_counter;
	
	CPU_CCP = 0xD8;
	CLKCTRL_MCLKCTRLB = CLKCTRL_PDIV_8X_gc | 1;				//Generates 2MHz Peripheral clock
	
	cal_factor = CLKCTRL_OSC20MCALIBA - 1;					//Use -1 for both PCBs
	CPU_CCP = 0xD8;
	CLKCTRL_OSC20MCALIBA = cal_factor;
	
	for(int m = 0; m <= 7; m++)display_buffer[m] = 0;		 
	display_ptr = 0;
	
	Set_display_ports;
	   
   PORTC.DIR &= ~PIN3_bm;										//Configure comm port as input
   PORTC.OUT &= ~(PIN3_bm);										//I/O pin low when configured as output
   PORTC.PIN3CTRL |= PORT_PULLUPEN_bm;							//Pull-up enabled
   while (PORTC.IN & (PIN3_bm));								//Wait for low pulse from UNO
  while (!(PORTC.IN & (PIN3_bm)));								//Wait for end of pulse
   
   
   byte_counter = 0;
 transaction_complete = 0;
 transaction_type = 0;
  TCA0_counter = 0;
   Start_TCA0();												//Display (2mS) tick rate
    
	sei();
	
	while(1){
		while(!(transaction_complete));
		transaction_complete = 0;
	
	switch (transaction_type){
		
		case 'A':															//Convert string from UNO to binary number
		for(int m = 0; m <= 7; m++)temp_buffer[m]=0;
		null_bit_counter = 0;
		for(int m = 0; m <= 7; m++){if(!(display_buffer[7-m]))null_bit_counter += 1; else break;}
		for(int m = 0; m <= 7-null_bit_counter; m++)
		temp_buffer[m] = display_buffer[7-null_bit_counter - m];
		Long_Num_to_UNO = atol(temp_buffer);
		transaction_type = 0;
		break;
		
		
		case 'C':															//Convert binary from UNO to string
		for(int m = 0; m <= 7; m++){temp_buffer[m] = 0;display_buffer[m] = 0;}
		ltoa(Long_Num_from_UNO, temp_buffer, 10);
		
		null_bit_counter = 0;
		for(int m = 0; m <= 7; m++){if(!(temp_buffer[7-m]))null_bit_counter += 1; else break;}
		for(int m = 0; m <= 7-null_bit_counter; m++)
		display_buffer[m] = temp_buffer[7-null_bit_counter - m];
		transaction_type = 0;
	
	break;
	
	case 'E':transaction_type = 0;break;}}
	
	}



/*****************************************************************************************************************************/
	void Start_TCA0(void){
	TCA0_counter = 0;
	TCA0_SINGLE_CNT = 0;										//Initialise counter
	TCA0_SINGLE_CMP0 = display_tick;							//2mS period for 2MHz clock
	TCA0_SINGLE_CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | 1;			//Start clock with 2MHz clock
	TCA0_SINGLE_INTCTRL |= TCA_SINGLE_CMP0EN_bm;}				//Interrupt flag on compare match zero/*****************************************************************************************************************************/	void Display_driver(void){		clear_digits; clear_display;		switch(display_ptr){	case 0: digit_0; break;		case 1: digit_1; break;	case 2: digit_2; break;	case 3: digit_3; break;	case 4: digit_4; break;	case 5: digit_5; break;	case 6: digit_6; break;	case 7: digit_7; break;}	Char_definition();	display_ptr += 1;  	if(display_ptr == 8)display_ptr = 0;	}				ISR (TCA0_CMP0_vect){											//Tx works but not Rx	TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;	TCA0_counter += 1;	if (TCA0_counter <= 8)	//7		{Display_driver();	inc_display_clock; return;}		cmp0_bkp = TCA0_SINGLE_CMP0 + display_tick;	inc_comms_clock;	PORTC.DIR |= PIN3_bm;										//Output low: Start bit	comms_transaction();	TCA0_SINGLE_CMP0 = cmp0_bkp;	TCA0_counter = 0;}										void Char_definition()
		{switch (display_buffer[display_ptr]){
			case '0': zero; break;
			case '1': one; break;
			case '2': two; break;
			case '3': three; break;
			case '4': four; break;
			case '5': five; break;
			case '6': six; break;
			case '7': seven; break;
			case '8': eight; break;
			case '9': nine; break;}} 








/*
#include "Project.h"
volatile char TCA0_counter;
volatile char display_digit;

int main(void)
{	char cal_factor;
	
	CPU_CCP = 0xD8;
	CLKCTRL_MCLKCTRLB = CLKCTRL_PDIV_8X_gc | 1;				//Generates 2MHz Peripheral clock
	
	cal_factor = CLKCTRL_OSC20MCALIBA - 1;					//Use -1 for both PCBs
	CPU_CCP = 0xD8;
	CLKCTRL_OSC20MCALIBA = cal_factor;
	
	for(int m = 0; m <= 7; m++)display_buffer[m] = m + '0';		 
	display_ptr = 0;
	
	Set_display_ports;
	   
   PORTC.DIR &= ~PIN3_bm;										//Configure comm port as input
   PORTC.OUT &= ~(PIN3_bm);										//I/O pin low when configured as output
   PORTC.PIN3CTRL |= PORT_PULLUPEN_bm;							//Pull-up enabled
   while (PORTC.IN & (PIN3_bm));								//Wait for low pulse from UNO
  while (!(PORTC.IN & (PIN3_bm)));								//Wait for end of pulse
   
   
   display_digit = 0;											//Next digit to be refreshed or updated
 
  TCA0_counter = 0;
   Start_TCA0();												//Display (2mS) tick rate
    
	sei();		
		transaction_complete = 0;								//Calibration code
		while(1){
			while(!(transaction_complete));
			transaction_complete = 0;
		transaction_type = 1;}}

	void Start_TCA0(void){
	TCA0_counter = 0;
	TCA0_SINGLE_CNT = 0;										//Initialise counter
	TCA0_SINGLE_CMP0 = display_tick;							//2mS period for 2MHz clock
	TCA0_SINGLE_CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | 1;			//Start clock with 2MHz clock
	TCA0_SINGLE_INTCTRL |= TCA_SINGLE_CMP0EN_bm;}				//Interrupt flag on compare match zero	void Display_driver(void){	clear_digits; clear_display;		switch(display_ptr){	case 0: digit_0; break;		case 1: digit_1; break;	case 2: digit_2; break;	case 3: digit_3; break;	case 4: digit_4; break;	case 5: digit_5; break;	case 6: digit_6; break;	case 7: digit_7; break;}	Char_definition();	display_ptr += 1;  //display_ptr = display_ptr%7;	if(display_ptr == 8)display_ptr = 0;	}				ISR (TCA0_CMP0_vect){											//Tx works but not Rx	TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;	TCA0_counter += 1;	if (TCA0_counter <= 8)	//7		{Display_driver();	inc_display_clock; return;}		cmp0_bkp = TCA0_SINGLE_CMP0 + display_tick;	inc_comms_clock;	PORTC.DIR |= PIN3_bm;										//Output low: Start bit	comms_transaction();	TCA0_SINGLE_CMP0 = cmp0_bkp;	TCA0_counter = 0;}										void Char_definition()
		{switch (display_buffer[display_ptr]){
			case '0': zero; break;
			case '1': one; break;
			case '2': two; break;
			case '3': three; break;
			case '4': four; break;
			case '5': five; break;
			case '6': six; break;
			case '7': seven; break;
			case '8': eight; break;
			case '9': nine; break;}} */
		