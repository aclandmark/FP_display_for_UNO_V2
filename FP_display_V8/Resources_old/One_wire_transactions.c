






	switch (transaction_type){
				
		case 1: 
		Rx_symbol = Rx_transaction();
		if((unsigned char)Rx_symbol == 0xFF);
		else{ 
			data [transaction_counter++] = Rx_symbol;
		if(transaction_counter == 25) {	
		transaction_type = 2; transaction_counter = 0;}}
		break;
		
		case 2: 
		Tx_symbol =	data [transaction_counter++] - 32;
		Tx_transaction(Tx_symbol);
		if(transaction_counter == 25){transaction_type = 1; 
			transaction_counter = 0; transaction_complete = 1;}
		break;
	}
}


char Rx_transaction (void){
	char Rx_char = 0;
	
	wait_for_clock_tick;



		
	/****************************************************************************************************************************/	
	void Tx_transaction (char symbol){
			wait_for_clock_tick;
			if (symbol & (1 << (7-m)))PORTC.DIR &= ~PIN3_bm;			//WPU
			else PORTC.DIR |= PIN3_bm;									//Output low
		inc_comms_clock;}
	PORTC.DIR &= ~PIN3_bm;}
	
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		