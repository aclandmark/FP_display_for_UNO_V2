





void comms_transaction (void){	if (!(transaction_type))
	{Rx_symbol = Rx_transaction();
	if((unsigned char)Rx_symbol == 0xFF)return;
	if (!(byte_counter)){byte_counter = 1; transaction_type = Rx_symbol;return;}}
	
	
	switch (transaction_type){
		case 'A':																//Receive long number string
		Rx_symbol = Rx_transaction();
		
		if(Rx_symbol == 1){Rx_symbol = 0; cr_keypress = 1;}
		
		display_buffer[byte_counter - 1] = Rx_symbol;
		byte_counter += 1;
		if(byte_counter == 10)	//9 Use 10 if last character is carriage return
		{transaction_complete = 1; byte_counter = 0; }
		break;
		
		case 'C':																//Receive long number
		Rx_symbol = Rx_transaction();
		Long_Num_from_UNO = (Long_Num_from_UNO << 8) | Rx_symbol; byte_counter += 1;
		if (byte_counter == 5){transaction_complete = 1; byte_counter = 0;
		}
		
		break;
		
		case 'E':																//Return long number string in binary
		
		//Tx_transaction(test_symbol++);
		Tx_transaction(data_byte[data_byte_ptr++]);
		
		byte_counter += 1;
		if(byte_counter == 5)
		{transaction_complete = 1; byte_counter = 0;data_byte_ptr = 0;}
		
		/*byte_counter += 1;
		switch(byte_counter){
			case 2: Tx_transaction(Long_Num_to_UNO >> 24); break;
			case 3: Tx_transaction(Long_Num_to_UNO >> 16); break;
			case 4: Tx_transaction(Long_Num_to_UNO >> 8); break;
			case 5: Tx_transaction(Long_Num_to_UNO); transaction_complete = 1; byte_counter = 0; break; }*/
		
		break;
		
		}
}



char Rx_transaction (void){
	char Rx_char = 0;
	
	wait_for_clock_tick;		PORTC.DIR &= ~PIN3_bm;										//Restore WPU		inc_comms_clock;	wait_for_clock_tick;		Rx_char = 0;	for(int m = 0; m <= 7; m++){								//Receive character		inc_comms_clock;		wait_for_clock_tick;		Rx_char = Rx_char << 1;										//Shift left one bit	if (PORTC_IN & PIN3_bm)	Rx_char |= 1;}						//Add new bit	return Rx_char;}



		
	/****************************************************************************************************************************/	
	void Tx_transaction (char symbol){				for(int m = 0; m <= 7; m++){								//Transmit first character
			wait_for_clock_tick;
			if (symbol & (1 << (7-m)))PORTC.DIR &= ~PIN3_bm;			//WPU
			else PORTC.DIR |= PIN3_bm;									//Output low
		inc_comms_clock;}		wait_for_clock_tick;
	PORTC.DIR &= ~PIN3_bm;}
	
		
		
		
		
		
		
		
		
		
		
		
		