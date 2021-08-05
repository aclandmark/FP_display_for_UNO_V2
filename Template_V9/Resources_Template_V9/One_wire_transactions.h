

volatile char One_wire_Rx_char, One_wire_Tx_char;
volatile int One_wire_Rx_int;
volatile char Tx_complete, Rx_complete;

#define Tx_clock_1     		200			//10K Baud rate				//92		//84		//100 		//142		//71	//80	//100
#define Rx_clock_1     		200										//92	//84	//100		//142		//71	//80	//100
#define Start_clock_1    	TCNT0 = 0;  TCCR0B = (1 << CS01);
#define Half_Rx_clock_1 	100										//46	//42	//50		//70		//35	//40	//50

#define wait_for_comms_tick \
OCR0A +=  Rx_clock_1;\
while (!(TIFR0 & (1 << OCF0A)));\
TIFR0 = 0xFF;

#define wait_for_half_comms_tick \
OCR0A =  Half_Rx_clock_1;\
while (!(TIFR0 & (1 << OCF0A)));\
TIFR0 = 0xFF;  

#define wait_for_free_line \
line_counter = 0;\
Start_clock_1;\
OCR0A =  0;\
while(1){\
wait_for_comms_tick;\
if (PINC & (1 << PINC4))counter += 1;\
else counter = 0;\
if (counter == 18)break;}


#define PINC4_down  ((PINC & 0x10)^0x10)