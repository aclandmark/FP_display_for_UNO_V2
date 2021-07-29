
/*
#define	seg_a 	(1 << PB5)
#define	seg_b 	(1 << PB4)
#define	seg_c 	(1 << PB1)
#define	seg_d 	(1 << PB3)
#define	seg_e 	(1 << PC2)
#define	seg_f 	(1 << PC1)
#define	seg_g 	(1 << PC0)
#define	DP		(1 << PB2)

#define	digit_0		PORTA |= (1 << PA1);
#define	digit_1		PORTA |= (1 << PA2);
#define	digit_2		PORTA |= (1 << PA3);
#define	digit_3		PORTB |= (1 << PB0);
#define	digit_4		PORTA |= (1 << PA4);
#define	digit_5		PORTA |= (1 << PA5);
#define	digit_6		PORTA |= (1 << PA6);
#define	digit_7		PORTA |= (1 << PA7);
*/

#define	seg_a 	PIN5_bm
#define	seg_b 	PIN4_bm
#define	seg_c 	PIN1_bm
#define	seg_d 	PIN3_bm
#define	seg_e 	PIN2_bm
#define	seg_f 	PIN1_bm
#define	seg_g 	PIN0_bm
#define	DP		PIN2_bm

#define	digit_0		PORTA.OUT |= PIN1_bm;
#define	digit_1		PORTA.OUT |= PIN2_bm;
#define	digit_2		PORTA.OUT |= PIN3_bm;
#define	digit_3		PORTB.OUT |= PIN0_bm;
#define	digit_4		PORTA.OUT |= PIN4_bm;
#define	digit_5		PORTA.OUT |= PIN5_bm;
#define	digit_6		PORTA.OUT |= PIN6_bm;
#define	digit_7		PORTA.OUT |= PIN7_bm;


//All ports are outputs
#define Set_display_ports \
PORTA.DIR |= (PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm);\
PORTB.DIR |= (PIN0_bm | PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm);\
PORTC.DIR |= (PIN0_bm | PIN1_bm | PIN2_bm);

//#define clear_digits Set all digits low
#define clear_digits \
PORTA.OUT &= (~(PIN1_bm | PIN2_bm | PIN3_bm | PIN4_bm | PIN5_bm | PIN6_bm | PIN7_bm));\
PORTB.OUT &= (~(PIN0_bm));

//#define clear_display Set all segments high
#define clear_display \
PORTB.OUT |= (seg_a | seg_b | seg_c | seg_d | DP);\
PORTC.OUT |= (seg_e | seg_f | seg_g);

#define clear_display_buffer	for(int m = 0; m<=3; m++)display_buf[m] = 0;


#define one_U 	PORTB.OUT &= ~(seg_b);
#define ONE_U 	PORTC.OUT &= ~(seg_f);
#define one_L 	PORTB.OUT &= ~(seg_c);
#define ONE_L 	PORTC.OUT &= ~(seg_e);
#define one 	PORTB.OUT &= ~(seg_b | seg_c);
#define two 	PORTB.OUT &= (~(seg_a | seg_b | seg_d));				PORTC.OUT &= (~(seg_e | seg_g));
#define three 	PORTB.OUT &= (~(seg_a | seg_b | seg_c | seg_d));		PORTC.OUT &= (~(seg_g));
#define four 	PORTB.OUT &= (~(seg_b | seg_c));						PORTC.OUT &= (~(seg_f | seg_g)); 
#define five 	PORTB.OUT &= (~(seg_a | seg_c | seg_d ));				PORTC.OUT &= (~(seg_f  | seg_g));
#define six 	PORTB.OUT &= (~(seg_c | seg_d ));						PORTC.OUT &= (~(seg_e | seg_f | seg_g));
#define seven 	PORTB.OUT &= (~(seg_a | seg_b | seg_c)); 
#define eight 	PORTB.OUT &= (~(seg_a | seg_b | seg_c| seg_d));			PORTC.OUT &= (~(seg_e  | seg_f | seg_g));
#define nine	PORTB.OUT &= (~(seg_a | seg_b | seg_c));				PORTC.OUT &= (~( seg_f | seg_g));
#define zero	PORTB.OUT &= (~(seg_a | seg_b | seg_c | seg_d ));		PORTC.OUT &= (~( seg_f  | seg_e  ));
#define minus															PORTC.OUT &= (~(seg_g));
#define exponent	PORTB &= (~(seg_a | seg_d));						PORTC.OUT &= (~(seg_e | seg_f | seg_g ));

#define zero_point		zero; PORTB &= ~DP;
#define one_point		one; PORTB &= ~DP;
#define two_point		two;  PORTB &= ~DP;
#define three_point		three;  PORTB &= ~DP;
#define four_point		four;  PORTB &= ~DP;
#define five_point		five;  PORTB &= ~DP;
#define six_point		six;  PORTB &= ~DP;
#define seven_point		seven;  PORTB &= ~DP;
#define eight_point		eight;  PORTB &= ~DP;
#define nine_point		nine;  PORTB &= ~DP;
#define minus_point		minus; PORTB &= ~DP;

