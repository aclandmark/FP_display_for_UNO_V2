






	
	for(int m = 0; m <= 7; m++){								//Transmit character
		wait_for_clock_tick;
		if (symbol & (1 << (7-m)))PORTC.DIR &= ~PIN3_bm;			//WPU
		else PORTC.DIR |= PIN3_bm;									//Output low
	inc_comms_clock;}