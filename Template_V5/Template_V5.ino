
 
#define device_type 1606
#define text_bytes  2048

#define Tx_clock_1            104
#define Rx_clock_1            104
#define Start_clock_1         TCCR0B = (1 << CS01);
#define Half_Rx_clock_1         52

#define PINC4_down  ((PINC & 0x10)^0x10)

#include "Resources_Template_V5.h"
char mode = 0;

volatile char One_wire_Rx_char, One_wire_Tx_char;


//ALL ports except the programming port are WPU, the programming port is tri-tate

int main (void){ 
char keypress;
//char symbol;
//while(1);

setup_328_HW;                                           //see "Resources\ATMEGA_Programmer.h"
P_counter = 0;                                          //Used to check parity of data received from UPDI
User_prompt;

sendString("\r\nPress 'a' to program target or AOK to run taget code\r\n");
if(waitforkeypress() == 'a') 

{sendString("\r\nUNO_UPDI_Programer_V1a (also works on an Atmega 328 clocked at 8MHz)\r\n\r\n");
sendString ("Press any key to start\r\n\r\n");
waitforkeypress();

/********************************Programmer target connection sequence************************************/
Timer_T1_sub_with_interrupt(T1_delay_100ms);
sei();
contact_target;
configure_updi;                                       //Increases UPDI clock to 16MHz and reduce guard band to 2 cycles
print_out_SIB;
if(UPDI_timeout)
{sendString("Power Cycle Required\r\n");
while(1);}
else{cli();TCCR1B = 0;
TIMSK1 &= (~(1 << TOIE1));}

initialise_NVM_programming;
sendString("Programming fuses\r\n");
write_fuse (WDTCFG, 0x0);                             //Default value: WDT under program control
write_fuse (BODCFG,0x0);                              //Default value: BOD dissabled
write_fuse (OSCCFG, 0x7E);                            //Default value: 20MHz internal clock
write_fuse (SYSCFG0, 0xF7);                           //UPDI enabled, EEPROM preserved at chip erase
write_fuse (SYSCFG1, 0xFD);                           //16mS SUT
write_fuse (APPEND, data_blocks);                      //For Attiny 1606:2Kbyte data partition for text
write_fuse (BOOTEND, 0x0);                            //Default value: No boot partition

sendString("\r\nProgram flash with hex? -y- or AOK");
if (waitforkeypress() == 'y')
{ mode = 'H';
  sendString("\r\nSend file  ");
  Program_Flash_Hex();
}
Verify_Flash_Hex();
sendString("\r\nFuses:\tWDT, BOD, OSC,SYS0\tSYS1, APPEND, BOOTEND\r\n\t");
read_out_fuses();

sendString("\r\n\r\nProgram flash with text? -y- or AOK");
if (waitforkeypress() == 'y')
{ mode = 'T';
  Program_Flash_Text();}

sendString("\r\n\r\nUnlocking device");
write_fuse (0x128A, 0xC5);                              //Device unlocked
sendString("\r\n-y- to Lock device or AOK");
if (waitforkeypress() == 'y')
{write_fuse (0x128A, 0x0);
sendString("\r\nDevice locked\r\n");}
else sendString("\r\nDevice unlocked\r\n");

UPDI_reset;

sendString("Sesion terminated\r\n\r\n");
Dissable_UPDI_sesion;}

/************Code in this section is test code.  It is not part of the UPDI programmer******************
******I*****Its purpose is to relay signals from the UNO UART port to the ATtiny UART ****************
******************so that the operation of the progammer can be tested easily*************************/


sendString("\r\nRun trial application? -y- or AOK (POR may be required)\r\n");
if(waitforkeypress() == 'y'){
sendString("AK to download text from ATtiny1606\r\n");


waitforkeypress();
DDRC |= (1 << DDC4);                                                  //outhigh
PORTC &= (~(1 << PORTC4));                                            //low pulse
_delay_ms(1);
DDRC &= ~(1 << DDC4);                                                 //Tri state   The default state
counter = 0;

for (int m = 0; m <= 23; m++){
One_wire_Tx_char = 2;
for(int m = 0; m <= 91; m++){
UART_Rx_Tx_1_wire();
One_wire_Tx_char += 1;
if(One_wire_Rx_char)
sendChar(One_wire_Rx_char);}
sendString("\r\n");}}

/**********************************End to test code section*********************************************/
SW_reset;
return 1;}




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

_delay_us(36);                                              //12.2
for (int m = 0; m <= 7;m++){
One_wire_Rx_char = One_wire_Rx_char << 1;
_delay_us(72);
if (PINC & (1 << PINC4)){One_wire_Rx_char |= 1;}}

_delay_us(108);
   DDRC |= (1 << PINC4);                              //OL start bit
  _delay_us(72);
  
  for(int m = 0; m <= 7; m++){                        //Eight data bits
    if (One_wire_Tx_char & (1 << (7-m)))
    DDRC &= (~(1 << PINC4));                          //OH
    else DDRC |= (1 << PINC4);                        //OL
    _delay_us(72);}
  
  DDRC &= ~(1 << PINC4);}}       


/*
Programmer subroutiness
 */

/*******************************************************************************************************************************/
void UART_Tx(unsigned int data_byte_T){               //starts Hi Z
  unsigned char parity = 0;
  DDRC |= (1 << DDC0);                                //start bit
  clock_delay_T;

for (int n = 0; n <= 7; n++){
  if (data_byte_T & (1 << n))
  {DDRC &= (~(1 << DDC0)); parity += 1;}
 else {DDRC |= (1 << DDC0);}clock_delay_T;}           //clock data out

if (parity%2){DDRC &= (~(1 << DDC0));} 
else {DDRC |= (1 << DDC0);}
  clock_delay_T;                                      //clock parity bit
  DDRC &= (~(1 << DDC0));                             //initiate stop bit 
  clock_delay_T;                                      //Stop bits
  clock_delay_T;}


/************************************************************************************************/
unsigned char UART_Rx(void){
  unsigned char data_byte_R = 0;
    char parity = 0;
                          
  while (PINC & (1 << PINC0));                        //wait for start bit
  clock_delay_R;                                      //The full delay is required 
  for (int n= 0; n <= 7; n++){clock_delay_R;
    if (PINC & (1 << PINC0))
    {data_byte_R |= (1 << n); parity += 1;}}
    clock_delay_R;

 if ((PINC & (1 << PINC0)) && (parity%2)); 
 else P_counter++;                                      //Parity often received in error?????
 
 clock_delay_R;                                         //stop bit
 clock_delay_R;                                         //stop bit  
 return data_byte_R;}


/*********************************************************************************/
ISR(USART_RX_vect){if (mode =='H')upload_hex();
                    if (mode =='T')upload_text();}
                    
ISR(TIMER1_OVF_vect) {
  if(!(T1OVFM)){UPDI_timeout = 1;TCCR1B = 0;}
  
  if(T1OVFM == 1){
    if(text_started == 3){T1OVFM = 0;
    endoftext -= 1; 
  TCCR1B = 0; TIMSK1 &= (~(1 << TOIE1));       
  inc_w_pointer; store[w_pointer] = 0;                         
  inc_w_pointer; store[w_pointer] = 0;}}}


 