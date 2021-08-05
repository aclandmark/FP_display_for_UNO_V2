

char receiveChar(void);
char isCharavailable (int);
char waitforkeypress(void);
char wait_for_return_key(void);
char decimal_digit (char);
void UART_Rx_1_wire(void);
void UART_Tx_1_wire(void);






long Int_from_KBD(void){                                            //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long I_number;
char SREG_BKP;

SREG_BKP = SREG;
sei();

cr_keypress = 0;                                                        //Set to one when carriage return keypress terminates the string
for(int n = 0; n<=8; n++) display_buffer[n] = 0;                     //Clear the buffer used for the string

while(1){
keypress = waitforkeypress();
if ((!(decimal_digit(keypress)))
&& (keypress != '-'))continue;                            				//Ignore keypress if it is not OK
display_buffer[0] = keypress;
break;}
 
Send_int_num_string;
while(1){
if ((keypress = wait_for_return_key())  =='\r')break;                 //Detect return key press (i.e \r or\r\n)
if ((decimal_digit(keypress)) || (keypress == '\b')\
 || (keypress == '-'))

{if (keypress == '\b'){                         						//Backspace key
for (int n = 0; n <= 7; n++)
display_buffer[n] = display_buffer[n + 1];}
else
{for(int n = 8; n>=1; n--)                                            //Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;  }                                      //Add new keypress           

Send_int_num_string;}}                                                  //Update display includes "cr_keypress"                                                 
cr_keypress = 1;                                                        //End of string; return key press detected

Send_int_num_string;
cr_keypress = 0;




One_wire_Tx_char = 'E'; UART_Tx_1_wire();
for(int m = 0; m <= 3; m++){
UART_Rx_1_wire(); num_byte[m] = One_wire_Rx_char;}

for(int m = 0; m <= 3; m++){
Long_Num_from_UNO = Long_Num_from_UNO << 8;
Long_Num_from_UNO |= num_byte[m];}




SREG = SREG_BKP;
return Long_Num_from_UNO;}



/************************************************************************************************************/
char wait_for_return_key(void){  
char keypress,temp;
keypress = waitforkeypress();
if((keypress == '\r') || (keypress == '\n')){
if (isCharavailable(1)){temp = receiveChar();}keypress = '\r';}
return keypress;}



/************************************************************************************************************/
char decimal_digit (char data){
if (((data > '9') || (data < '0')) )return 0;
else return 1;}



