



void UART_Rx_Tx_1_wire(){
One_wire_Rx_char = 0;

PCICR |= 1 << PCIE1;                                          //set up IPC
PCMSK1 |= 1 << PCINT12;
sei();
while (!(One_wire_Rx_char));
cli();
PCMSK1 &= (~(1 << PCINT12));                                  //clear IPC
PCICR &= (~(1 << PCIE1));}

/****************************************************************/
ISR(PCINT1_vect){                                           //Receive character: UART RX_1_wire
if(PINC4_down){                                             //if start bit
PCMSK1 &= (~(1 << PCINT12));                                  //clear IPC
PCICR &= (~(1 << PCIE1));

Start_clock_1;
wait_for_half_comms_tick;

for (int m = 0; m <= 7;m++){
  wait_for_comms_tick;
  
One_wire_Rx_char = One_wire_Rx_char << 1;
if (PINC & (1 << PINC4)){One_wire_Rx_char |= 1;}}
sendChar(One_wire_Rx_char);
/*************************TRANSMIT**********************************************/
PORTC |= (1 << PORTC4);                               //WPU
DDRC |= (1 << DDC4);                                  //OH
wait_for_comms_tick;
 for(int m = 0; m <= 7; m++){                        //Eight data bits
    if (One_wire_Tx_char & (1 << (7-m)))
    PORTC |= (1 << PORTC4);                          //OH
    else PORTC &= ~(1 << PORTC4);                    //OL
   wait_for_comms_tick;}

//One_wire_Tx_char += 1;								//For test purposes
//if(One_wire_Tx_char == 91)One_wire_Tx_char = 'A';	//Forr test purposes
/**************************************************************************/
DDRC &= ~(1 << PINC4);
PORTC &= ~(1 << PORTC4);                             //Restore Tri
TCCR0B = 0;
}}


