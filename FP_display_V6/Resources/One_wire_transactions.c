





char Tx_Rx_transaction (char symbol){	char Rx_char;
	
	for(int m = 0; m <= 7; m++){								//Transmit character
		wait_for_clock_tick;
		if (symbol & (1 << (7-m)))PORTC.DIR &= ~PIN3_bm;			//WPU
		else PORTC.DIR |= PIN3_bm;									//Output low
	inc_comms_clock;}		wait_for_clock_tick;		PORTC.DIR &= ~PIN3_bm;										//Restore WPU	Rx_char = 0;	for(int m = 0; m <= 7; m++){								//Receive character		inc_comms_clock;		wait_for_clock_tick;		Rx_char = Rx_char << 1;										//Shift left one bit	if (PORTC_IN & PIN3_bm)	Rx_char |= 1;}						//Add new bit		inc_half_comms_clock;										//Re-align UNO and 1606 clocks	inc_display_clock;											//Prepare to refresh the displayreturn Rx_char;}