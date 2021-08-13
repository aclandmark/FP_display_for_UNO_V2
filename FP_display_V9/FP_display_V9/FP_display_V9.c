/*
 
 */ 


#include "Project.h"



volatile char busy_flag;
volatile char TCA0_counter;


int main(void)
{	
	
	
	char cal_factor;
	char null_bit_counter;
	int array_ptr;
	
	
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
    
   busy_flag = 0;												//Data procesing in progress: Do not poll UNO
   data_byte_ptr = 0;											//Points to next FPN/long byte to be transfered to the UNO
   cr_keypress = 0;												//Set to 1 when user presses carriage return 
	byte_counter = 0;											//Counts bytes sent to or received from UNO
	transaction_complete = 0;										//Set to 1 when a data transfer is complete
 transaction_type = 0;											//Data/string transfer to/from UNO 
  TCA0_counter = 0;												//Counts TCAO interrupts
   Start_TCA0();												//Display (2mS) tick rate
    
	sei();
	
	while(1){
		while(!(transaction_complete));							//Wait here for requests from the UNO
		transaction_complete = 0;
		busy_flag = 1;											//Do not contact UNO until data processing is complete
		
	switch (transaction_type){									//Transaction complete: Process the data
		
	case 'A':													//If "cr" detected convert string from UNO to long number
	if(cr_keypress == 1){
	cr_keypress = 0;
	display_buffer2temp;										//Reverse string prior to conversion to binary
	Long_Num_to_UNO = atol(temp_buffer);	
	for(int m = 0; m <= 3; m++)
	data_byte[m] = Long_Num_to_UNO >> (8*(3-m));}				//Split long number into 4 bytes for re-transmission
	break;
		
	case 'B':													//If "cr" detected convert string from UNO to float
	if(cr_keypress == 1){
	cr_keypress = 0;
	check_for_dp;
	Add_dp_if_missing;											//FP arithmetic requires numbers with a dp
	display_buffer2temp;										//Reverse string prior to conversion to a FPN
	extract_dp;													//Display merges the dp with a digit  											
	Float_Num_to_UNO = atof(temp_buffer);
	char_ptr = (char*)&Float_Num_to_UNO;						//Split the number into bytes ready for return to the UNO
	for (int m = 0; m <= 3; m++)
	{data_byte[m] = *char_ptr;
	char_ptr += 1;}
	}break;
		
		
	case 'C':													//Convert long from UNO to string
	clear_display_buffer;
	clear_temp_buffer;
	ltoa(Long_Num_from_UNO, temp_buffer, 10);
	temp2display_buffer;										//Reverse string prior to display	
	break;
			
	case 'D': 													//Convert float from UNO to string
	Float_Num_from_UNO  = *float_ptr_2;
	ftoaL(Float_Num_from_UNO);
	Combine_dp;		 
	Insert_sign;
	Remove_lagging_zeros;
	temp2display_buffer;										//Reverse string prior to display
	if (expt){Left_justify_number;
	Add_exponent ();}
	break;
		
	case 'E':													//Return binary number (long or float) to UNO
	for(int m = 0; m <= 7; m++)
	temp_buffer[m] = display_buffer[m];							//Flash display
	clear_display_buffer;
	_delay_ms(100);
	for(int m = 0; m <= 7; m++)
	display_buffer[m] = temp_buffer[m];
	break;}
	
	
	transaction_type = 0;
	busy_flag = 0;
	
	
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
	TCA0_SINGLE_INTCTRL |= TCA_SINGLE_CMP0EN_bm;}				//Interrupt flag on compare match zero/*****************************************************************************************************************************/	void Display_driver(void){		clear_digits; clear_display;	switch(display_ptr){	case 0: digit_0; break;		case 1: digit_1; break;	case 2: digit_2; break;	case 3: digit_3; break;	case 4: digit_4; break;	case 5: digit_5; break;	case 6: digit_6; break;	case 7: digit_7; break;}	Char_definition();	display_ptr += 1; 	if(display_ptr == 8)display_ptr = 0;}				ISR (TCA0_CMP0_vect){											//Tx works but not Rx	TCA0_SINGLE_INTFLAGS |= TCA_SINGLE_CMP0EN_bm;	TCA0_counter += 1;	if (TCA0_counter <= 8)		{Display_driver();	inc_display_clock; return;}		cmp0_bkp = TCA0_SINGLE_CMP0 + display_tick;	inc_comms_clock;	PORTC.DIR |= PIN3_bm;										//Output low: Start bit	if (!(busy_flag))comms_transaction();	TCA0_SINGLE_CMP0 = cmp0_bkp;	TCA0_counter = 0;}										void Char_definition()
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
			case '9': nine; break;
			case '-': minus; break;
			
			case 'E': case 'e': exponent; break;
			case ('0' | 0x80): zero_point; break;
			case ('1' | 0x80): one_point; break;
			case ('2' | 0x80): two_point; break;
			case ('3' | 0x80): three_point; break;
			case ('4' | 0x80): four_point; break;
			case ('5' | 0x80): five_point; break;
			case ('6' | 0x80): six_point; break;
			case ('7' | 0x80): seven_point; break;
			case ('8' | 0x80): eight_point; break;
			case ('9' | 0x80): nine_point; break;
			case ('-' | 0x80): minus_point; break;}			
			}
