





void comms_transaction (void){	
	switch (transaction_type){
				
		case 1: 
		Rx_symbol = RX_transaction();
		if((unsigned char)Rx_symbol == 0xFF)Rx_symbol = Rx_symbol_bkp;
		
		else{ Rx_symbol_bkp = Rx_symbol; data [transaction_counter++] = Rx_symbol;
			
		if(transaction_counter == 10) {	
			
		transaction_type = 2; transaction_counter = 0;}}
		break;
		
		case 2: 
		//Tx_symbol = Rx_symbol - 32 + transaction_counter++;
		
		Tx_symbol = data [transaction_counter++] - 32;
		
		Tx_transaction(Tx_symbol);
		if(transaction_counter == 10){transaction_type = 1; transaction_counter = 0;}
		break;
	}
}


char RX_transaction (void){
	char Rx_char = 0;
	
	wait_for_clock_tick;		PORTC.DIR &= ~PIN3_bm;										//Restore WPU		inc_comms_clock;/////////////////////////////////////	wait_for_clock_tick;///////////////////////////////////////////////			Rx_char = 0;	for(int m = 0; m <= 7; m++){								//Receive character		inc_comms_clock;		wait_for_clock_tick;		Rx_char = Rx_char << 1;										//Shift left one bit	if (PORTC_IN & PIN3_bm)	Rx_char |= 1;}						//Add new bit		inc_half_comms_clock;										//Re-align UNO and 1606 clocks	inc_display_clock;											//Prepare to refresh the displayreturn Rx_char;}




char Rx_transaction (void){
	char Rx_char = 0;
	
	wait_for_clock_tick;										//End of start bit	inc_comms_clock;
	
	wait_for_clock_tick;										//End of start bit	inc_half_comms_clock;										//Middle of first bit			PORTC.DIR &= ~PIN3_bm;										//Restore WPU	Rx_char = 0;	for(int m = 0; m <= 7; m++){								//Receive character		wait_for_clock_tick;		Rx_char = Rx_char << 1;										//Shift left one bit	if (PORTC_IN & PIN3_bm)	Rx_char |= 1;						//Add new bit	if (m <= 6)inc_comms_clock;}			inc_half_comms_clock;										//Re-align UNO and 1606 clocks	wait_for_clock_tick;	inc_display_clock;											//Prepare to refresh the displayreturn Rx_char;}

	
		
	/****************************************************************************************************************************/	
	void Tx_transaction (char symbol){				for(int m = 0; m <= 7; m++){								//Transmit first character
			wait_for_clock_tick;
			if (symbol & (1 << (7-m)))PORTC.DIR &= ~PIN3_bm;			//WPU
			else PORTC.DIR |= PIN3_bm;									//Output low
		inc_comms_clock;}														//Re-align UNO and 1606 clocks	wait_for_clock_tick;
	PORTC.DIR &= ~PIN3_bm;
	inc_display_clock;}											//Prepare to refresh the display	
		
	
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		