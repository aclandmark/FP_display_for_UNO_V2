






	switch (transaction_type){
				
		case 1: 
		Rx_symbol = Rx_transaction();
		if((unsigned char)Rx_symbol == 0xFF)Rx_symbol = Rx_symbol_bkp;
		else{ Rx_symbol_bkp = Rx_symbol;
		transaction_type = 2;transaction_counter = 0;}
		break;
		
		case 2: Tx_symbol = Rx_symbol - 32 + transaction_counter++;
		Tx_transaction(Tx_symbol);
		if(transaction_counter == 6)transaction_type = 1;
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
		
	
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		