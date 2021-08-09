
 
#define device_type 1606
//long Int_from_KBD(void);

#include "Resources_Template_V9.h"
#include "Resources_UPDI_programmer.h"

char mode = 0;

//ALL ports except the programming port are WPU, the programming port is tri-tate

int main (void){ 
char keypress, line_counter;
long int_num;
float FPN;
float test = 2.0;
char FP_string[12];




setup_328_HW;                                           //see "Resources\ATMEGA_Programme
PORTC &= (~(1 << PORTC4));                               //One way comms for template requires port to be set to Tri state   
P_counter = 0;                                          //Used to check parity of data received from UPDI


User_prompt;

/*************************************************/

sendString("\r\nPress 'a' to program target or AOK to run taget code\r\n\r\n");
if(waitforkeypress() == 'a'){

//********************************Programmer target connection sequence************************************
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
write_fuse (APPEND, 0);
write_fuse (BOOTEND, 0);


sendString("\r\nProgram flash with hex? -y- or AOK");
if (waitforkeypress() == 'y')
{ mode = 'H';
sendString("\r\nSend file  ");
Program_Flash_Hex();}

Verify_Flash_Hex();
sendString("\r\nFuses:\tWDT, BOD, OSC,SYS0\tSYS1, APPEND, BOOTEND\r\n\t");
read_out_fuses();

UPDI_reset; 
Dissable_UPDI_sesion;}

/*************************************************/

/************Code in this section is test code.  It is not part of the UPDI programmer******************
******I*****Its purpose is to relay signals from the UNO UART port to the ATtiny UART ****************
******************so that the operation of the progammer can be tested easily*************************/


sendString("\r\nRun trial application? -y- or AOK (POR may be required)\r\n\r\n");
if(waitforkeypress() == 'y'){
//PORTC &= (~(1 << PORTC4));                                                              //Tri state   The default state


while(1){
User_prompt_template;

if (User_response =='i'){
sendString("Enter integer from KBD (terminate in cr)\r\n");

while (int_num = Int_from_KBD()){
sendString("AK to do arithmetic\r\n");
waitforkeypress();
Long_Num_to_UNO = Long_Num_from_UNO * 2;
send_int_num(Long_Num_to_UNO);
sendString("New number?\r\n");}}

if (User_response =='f'){
 sendString("Enter FPN from KBD (terminate in cr)\r\n"); 
 FPN =  Float_from_KBD();
 waitforkeypress();
 FPN = FPN * test;
send_float_num(FPN);
  ftoaL(FPN, FP_string);
for(int m = 0; m <=11; m++)if(FP_string[m])sendChar(FP_string[m]);
  
  }
}}

/**********************************End to test code section*********************************************/
SW_reset;
return 1;}




/*
Programmer subroutiness
 */

/***************************************************************************************************************************************/
void ftoaL(float Fnum, char FP_string[]){
  int afterpoint = 0;
  long ipart, Fnum_int;
  char sign = '+';
  signed char expt;
  
  if (Fnum < 0){sign = '-'; Fnum *= (-1);}                  //Convert negative numbers to positive ones and set the sign character
  
  for(int m = 0; m <= 15; m++) FP_string[m] = 0;                //Clear the floating point array
  
  Fnum_int = (long)Fnum;                            //Obtain integer part of the number
  
  if (Fnum_int < 10)afterpoint = 5;                     //Number of decimal places is matched to display length
  if ((Fnum_int >= 10) && (Fnum_int < 100))afterpoint = 4;
  if ((Fnum_int >= 100) && (Fnum_int < 1000))afterpoint = 3;
  if ((Fnum_int >= 1000) && (Fnum_int < 10000))afterpoint = 2;
  
  expt = 0;                                 //Convert very large and small numbers to scientific form
  if (Fnum  >= 10000) {while (Fnum >= 10)
  {Fnum /= 10; expt += 1;}afterpoint = 5;}
  
  if(Fnum < 0.01) {while (Fnum < 1){Fnum *= 10; expt -= 1;}}
  
                                        //FP to askii routines taken from "https://www.geeksforgeeks.org/convert-floating-point-number-string/"
  ipart = (long)Fnum;                             //Obtain integer part of FP number
  float fpart = Fnum - (float)ipart;                      //Obtain floating part
  long i = longToStr(ipart, FP_string, 0);                  //Convert integer part to string

  if (afterpoint != 0){                           //Add Decimal part to the string
    FP_string[i] = '.';
    fpart = fpart * pow(10,afterpoint);
  longToStr((long)fpart, FP_string + i + 1, afterpoint);}

 
  //expt = Format_for_Display(FP_string, sign, expt);
}



/***************************************************************************************************************************************/
long longToStr(long x, char str[], int d)
{
  int i = 0;
  while (x)
  {   str[i++] = (x%10) + '0';
  x = x/10; }
  
  while (i < d)
  str[i++] = '0';
  reverse(str, i);
  str[i] = '\0';
return i; }


/***************************************************************************************************************************************/
void reverse(char *str, int len)
{
  int i=0, j=len-1, temp;
  while (i<j)
  {   temp = str[i];
    str[i] = str[j];
    str[j] = temp;
  i++; j--; }}

  




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
ISR(USART_RX_vect){if (mode =='H')upload_hex();}
//                    if (mode =='T')upload_text();}
                    
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
