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
	
	for(int m = 0; m <= 11; m++)display_buffer[m] = 0;		 
	display_ptr = 0;
	
	Set_display_ports;
	   
   PORTC.DIR &= ~PIN3_bm;										//Configure comm port as input
   PORTC.OUT &= ~(PIN3_bm);										//I/O pin low when configured as output
   PORTC.PIN3CTRL |= PORT_PULLUPEN_bm;							//Pull-up enabled
  ////////////////// while (PORTC.IN & (PIN3_bm));								//Wait for low pulse from UNO
 /////////////////// while (!(PORTC.IN & (PIN3_bm)));								//Wait for end of pulse
   
   data_byte_ptr = 0;
   cr_keypress = 0;
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
		transaction_type = 0;
		if(cr_keypress == 1){
		
		for(int m = 0; m <= 7; m++)temp_buffer[m]=0;
		null_bit_counter = 0;
		for(int m = 0; m <= 7; m++){if(!(display_buffer[7-m]))null_bit_counter += 1; else break;}
		for(int m = 0; m <= 7-null_bit_counter; m++)
		temp_buffer[m] = display_buffer[7-null_bit_counter - m];
		//transaction_type = 0;
		
		//if(cr_keypress == 1)
		//{
		cr_keypress = 0;
		Long_Num_to_UNO = atol(temp_buffer);		//3579;//
		for(int m = 0; m <= 3; m++)data_byte[m] = Long_Num_to_UNO >> (8*(3-m));		//= m+3;//
		/*data_byte[0] = 0;
		data_byte[1] = 4;
		data_byte[2] = 5;
		data_byte[3] = 6;*/
		
		
		
		}
		
		
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
	
	case 'E':transaction_type = 0;break;
	
	
	}
	
	
	
	
	
	}
		
		
		
		
		
		//transaction_type = 1;
		/*transaction_complete = 0;								//Calibration code
		while(1){
			while(!(transaction_complete));
			transaction_complete = 0;*/
		//transaction_type = 1;
		//}
		
		
		
		}



/*****************************************************************************************************************************/
	void Start_TCA0(void){
	TCA0_counter = 0;
	TCA0_SINGLE_CNT = 0;										//Initialise counter
	TCA0_SINGLE_CMP0 = display_tick;							//2mS period for 2MHz clock
	TCA0_SINGLE_CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | 1;			//Start clock with 2MHz clock
	TCA0_SINGLE_INTCTRL |= TCA_SINGLE_CMP0EN_bm;}				//Interrupt flag on compare match zero
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