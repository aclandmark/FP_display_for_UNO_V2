
 
#define device_type 1606
//#define text_bytes  2048
#define text_bytes  0                                     //No space reserved for text



#include "Resources_Template_V9.h"
char mode = 0;

//ALL ports except the programming port are WPU, the programming port is tri-tate

int main (void){ 
char keypress, line_counter;

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
write_fuse (OSCCFG, 0x01);                            //Default value: 16MHz internal clock  0x7D
write_fuse (SYSCFG0, 0xF7);                           //UPDI enabled, EEPROM preserved at chip erase
write_fuse (SYSCFG1, 0xFD);                           //16mS SUT
write_fuse (APPEND, data_blocks);                      //For Attiny 1606: Nothing reserved for text
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
PORTC &= (~(1 << PORTC4));                                            //Tri state   The default state
sendString("Enter numbers from KBD (terminate in cr)\r\n");


long Long_Num_to_UNO;
long Long_Num_from_UNO = 0;                                                                //Recover binary number
unsigned char num_byte[4];


while(1){

Int_from_KBD();

One_wire_Tx_char = 'E'; UART_Tx_1_wire();
for(int m = 0; m <= 3; m++){
UART_Rx_1_wire(); num_byte[m] = One_wire_Rx_char;}

for(int m = 0; m <= 3; m++){
Long_Num_from_UNO = Long_Num_from_UNO << 8;
Long_Num_from_UNO |= num_byte[m];}



sendString("AK to do arithmetic\r\n");
waitforkeypress();
Long_Num_to_UNO = Long_Num_from_UNO * 2;                                                  //Do arithmetic and display resuly
One_wire_Tx_char = 'C'; UART_Tx_1_wire();
for(int m = 0; m <= 3; m++){
One_wire_Tx_char = Long_Num_to_UNO >> (8 * (3 - m)); UART_Tx_1_wire();}

sendString("New number?\r\n");
}

while(1);}

/**********************************End to test code section*********************************************/
SW_reset;
return 1;}




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


 /**********************************************************************************************************************/
 /***************************************************************************************************************************************/
void Int_from_KBD(void){                                            //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long I_number;
char SREG_BKP;

SREG_BKP = SREG;

//sendChar('Z');

sei();

cr_keypress = 0;                                                        //Set to one when carriage return keypress terminates the string
for(int n = 0; n<=8; n++) display_buffer[n] = 0;                     //Clear the buffer used for the string

while(1){
keypress = waitforkeypress();
if ((!(decimal_digit(keypress)))
&& (keypress != '-'))continue;                            //Ignore keypress if it is not OK
display_buffer[0] = keypress;
break;}
//sendChar(keypress);

//int_string_to_display();//////////////////////////////////////////////////////////////////// 
One_wire_Tx_char = 'A'; UART_Tx_1_wire();
for(int m = 0; m <= 7; m++){One_wire_Tx_char = display_buffer[m]; UART_Tx_1_wire();}
One_wire_Tx_char = cr_keypress;  UART_Tx_1_wire();

while(1){
if ((keypress = wait_for_return_key())  =='\r')break;                 //Detect return key press (i.e \r or\r\n)
if ((decimal_digit(keypress)) || (keypress == '\b')\
 || (keypress == '-'))

{if (keypress == '\b'){                         //Backspace key
for (int n = 0; n <= 7; n++)
display_buffer[n] = display_buffer[n + 1];}
else
{for(int n = 8; n>=1; n--)                                            //Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;  }                                               //Add new keypress           

//sendChar(display_buffer[0]);

//int_string_to_display();////////////////////////////////////////////////////////////////////////////////////////////////
One_wire_Tx_char = 'A'; UART_Tx_1_wire();
for(int m = 0; m <= 7; m++){One_wire_Tx_char = display_buffer[m]; UART_Tx_1_wire();}
One_wire_Tx_char = cr_keypress;  UART_Tx_1_wire();
 }}                                                                     //Update display includes "cr_keypress"                                                 
cr_keypress = 1;                                                        //End of string; return key press detected

//int_string_to_display()//////////////////////////////////////////////////////////////////////////////////
One_wire_Tx_char = 'A'; UART_Tx_1_wire();
for(int m = 0; m <= 7; m++){One_wire_Tx_char = display_buffer[m]; UART_Tx_1_wire();}
One_wire_Tx_char = cr_keypress;  UART_Tx_1_wire();

cr_keypress = 0;
SREG = SREG_BKP;

/*TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);                      //Activate TWI and wait for contact from display pcb 
while (!(TWCR & (1 << TWINT)));
I_number =  byte(receive_byte_with_Ack());                              //Build up the number as each byte is received
I_number = (I_number << 8) + byte(receive_byte_with_Ack());
I_number = (I_number << 8) + byte(receive_byte_with_Ack());
I_number = (I_number << 8) + byte(receive_byte_with_Nack());
TWCR = (1 << TWINT);*/
//return I_number;
}


char wait_for_return_key(void){  
char keypress,temp;
keypress = waitforkeypress();
if((keypress == '\r') || (keypress == '\n')){
if (isCharavailable(1)){temp = receiveChar();}keypress = '\r';}
return keypress;}

char decimal_digit (char data){
if (((data > '9') || (data < '0')) )return 0;
else return 1;}
