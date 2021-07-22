
volatile char One_wire_mode;


void UART_TX_1_wire(void){
One_wire_Rx_char = 0;
Rx_complete = 0;
One_wire_mode = 5;											//Rx_Tx mode
PCICR |= 1 << PCIE1;                                          //set up IPC
PCMSK1 |= 1 << PCINT12;
sei();
while(!(Rx_complete));

if (One_wire_Rx_char)sendChar(One_wire_Rx_char);
cli();}




void UART_Tx_1_wire(void){
//One_wire_Rx_char = 0;
Tx_complete = 0;
One_wire_mode = 1;											//Rx_Tx mode
PCICR |= 1 << PCIE1;                                          //set up IPC
PCMSK1 |= 1 << PCINT12;
sei();
while(!(Tx_complete));

//if (One_wire_Rx_char)sendChar(One_wire_Rx_char);
cli();}

void UART_Rx_1_wire(void){
One_wire_Rx_char = 0;
One_wire_mode = 2;											//Rx_Tx mode
PCICR |= 1 << PCIE1;                                          //set up IPC
PCMSK1 |= 1 << PCINT12;
sei();
while (!(One_wire_Rx_char));
sendChar(One_wire_Rx_char);
cli();}


/****************************************************************/
ISR(PCINT1_vect){                                           //Receive character: UART RX_1_wire
if(PINC4_down){												//if start bit

PCMSK1 &= (~(1 << PCINT12));                                  //clear IPC
PCICR &= (~(1 << PCIE1));


if(One_wire_mode == 5){								//Transmit character
Start_clock_1;
wait_for_half_comms_tick;
Rx_complete = 1;

PORTC |= (1 << PORTC4);                               //WPU

wait_for_comms_tick;//////////////////////////////////////////////////////////

DDRC |= (1 << DDC4);                                  //OH
wait_for_comms_tick;
 for(int m = 0; m <= 7; m++){                        //Eight data bits
    if (One_wire_Tx_char & (1 << (7-m)))
    PORTC |= (1 << PORTC4);                          //OH
    else PORTC &= ~(1 << PORTC4);                    //OL
   wait_for_comms_tick;}

DDRC &= ~(1 << PINC4);
PORTC &= ~(1 << PORTC4);                             //Restore Tri
TCCR0B = 0;}







if(One_wire_mode == 1){								//Transmit character
Start_clock_1;
//wait_for_half_comms_tick;
wait_for_comms_tick;
//Rx_complete = 1;

PORTC |= (1 << PORTC4);                               //WPU
wait_for_comms_tick;


DDRC |= (1 << DDC4);                                  //OH
//wait_for_comms_tick;
 for(int m = 0; m <= 7; m++){                        //Eight data bits
    if (One_wire_Tx_char & (1 << (7-m)))
    PORTC |= (1 << PORTC4);                          //OH
    else PORTC &= ~(1 << PORTC4);                    //OL
   wait_for_comms_tick;}

Tx_complete = 1;

DDRC &= ~(1 << PINC4);
PORTC &= ~(1 << PORTC4);                             //Restore Tri
TCCR0B = 0;}

if(One_wire_mode == 2){								//Receive character
Start_clock_1;
wait_for_half_comms_tick;

for (int m = 0; m <= 7;m++){
  wait_for_comms_tick;
  
One_wire_Rx_char = One_wire_Rx_char << 1;
if (PINC & (1 << PINC4)){One_wire_Rx_char |= 1;}}
TCCR0B = 0;}
}}


