/* serialRemote.ino Example sketch for IRLib2
 * Illustrates how to send an IR signal based on data from a
 * serial port. You can type the data into the serial monitor
 * or send it from some other application such as the Python
 * program included in this folder.
 * 
 * Send the data as 2 or 3 values. Separate them by any non-hex character.
 * The first is protocol number in decimal, followed by data value,
 * optionally followed by number of bits.
 */

//This includes everything. Not generally recommended.
#include "IRLibAll.h"

IRsend mySender; //create an instance of the sending object

uint8_t protocol;
uint32_t code;
uint16_t bits;//also used as "address" on some protocols greater than 32 bits

void setup() {
  Serial.begin(9600);
  delay(2000);while(!Serial);//delay for Leonardo
  Serial.println(F("Enter protocol number, data in hex, and bits separated by commas."));
}

long parseHex (void) {
  long Value=0; char C;delay(100);
  while (Serial.available()>0) {
     C= tolower(Serial.read());
     if ((C>='0')&&(C<='9'))
       C=C-'0';
     else
       if ((C>='a') && (C<='f'))
         C=C-'a'+10;
       else
         return Value;
     Value=  C+(Value<<4);
  };
  return Value;
}
void parseDelimiter () {
  char C;
  while(Serial.available()>0) {
    C=tolower(Serial.peek());
    if( (C>='0') && (C<='9') )return;
    if( (C>='a') && (C<='f') )return;
    C=Serial.read();//throwaway delimiters
    delay (5);
  }
}

void loop() {
  if (Serial.available ()>0) {
    protocol = Serial.parseInt (); parseDelimiter();
    code     = parseHex ();        parseDelimiter();
    bits     = Serial.parseInt (); parseDelimiter();
    Serial.print("Prot:");  Serial.print(protocol);
    Serial.print(" Code:"); Serial.print(code,HEX);
    Serial.print(" Bits:"); Serial.println(bits);
    mySender.send(protocol, code, bits);   
  }
}
