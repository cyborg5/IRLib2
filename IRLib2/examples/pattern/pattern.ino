/* pattern.ino Example sketch for IRLib2
 *  Send an easily recognized pattern of bits from one Arduino
 *  to another to verify that your protocol is working. Load
 *  this sketch to send a signal. Use the "dump" sketch on the other
 *  Arduino to receive codes. Open the serial monitor and type the  
 *  number Of the protocol want to test.
 */
#include <IRLibSendBase.h>    // First include the send base
//Now include only the protocols you wish to actually use.
//The was numbered protocol should be first but remainder can be any order.
#include <IRLib_P01_NEC.h>    
#include <IRLib_P02_Sony.h>
#include <IRLib_P03_RC5.h>
#include <IRLib_P04_RC6.h>
#include <IRLib_P05_Panasonic_Old.h>
#include <IRLib_P06_JVC.h>
#include <IRLib_P07_NECx.h>
#include <IRLib_P08_Samsung36.h>
#include <IRLib_P09_GICable.h>
#include <IRLib_P10_DirecTV.h>
#include <IRLib_P11_RCMM.h>
#include <IRLib_P12_CYKM.h>
#include <IRLibCombo.h>     // After all protocols, include this
// All of the above automatically creates a universal sending
// class called "IRsend" containing only the protocols you want.
// Now declare an instance of that sender.

IRsend mySender;

void setup() {
  Serial.begin(9600);
  delay(2000); while (!Serial); //delay for Leonardo
  Serial.println(F("Type the protocol number:1-12 Or '-1' for all"));
}
void sendOne(uint8_t protocol, uint32_t data, uint16_t data2=0,uint8_t khz=0) {
  Serial.print(F("Protocol #")); Serial.print(protocol,DEC);
  Serial.print(F("\t")); Serial.print(Pnames(protocol));
  Serial.print(F("\tdata=0x")); Serial.print(data,HEX);
  if(data2) {
    Serial.print(F("\tdata2=0x")); Serial.print(data2,HEX);
    Serial.print(F(" ")); Serial.print(data2,DEC);
    Serial.print(F(" dec"));
  }
  if(khz) {
    mySender.send(protocol,data,data2,khz);
    Serial.print(F("  khz=")); Serial.println(khz,DEC);
  } else {
    mySender.send(protocol,data,data2);
    Serial.println();
  }
  delay(2000);
}
#define PATTERN1 0x12345678
#define ALL1S    0xFFFFFFFF

void doTest (uint8_t P) {
  switch (P) {  
    case 1: sendOne(1,PATTERN1);      //NEC Regular data
            sendOne(1,REPEAT_CODE);   //NEC Special ditto repeat code
            sendOne(1,PATTERN1,40);   //Pioneer is NEC with 40 kHz modulation
            break;
    case 2: sendOne(2,PATTERN1,8);   //Sony 8 bits
            sendOne(2,PATTERN1,12);   //Sony 12 bits
            sendOne(2,PATTERN1,15);   //Sony 15 bits
            sendOne(2,PATTERN1,20);   //Sony 20 bits
            break;      //Note: Sony always sends three copies of signal
    case 3: sendOne(3,ALL1S,13);      //RC5 
            sendOne(3,ALL1S,14);      //RC5-F7
            sendOne(3,ALL1S,14,57);   //RC5-F7-57
            break;
    case 4: sendOne(4,0x0ffff,20);    //RC6-0-16 Original Phillips RC6
            sendOne(4,0xcfffff,24);   //RC6-6-20 Used by some Sky and Sky+ remotes
            sendOne(4,0xcfffff,28);   //RC6-6-24 a.k.a. "Replay" protocol
            sendOne(4,ALL1S,32);      //RC6-6-32 a.k.a. "MCE" protocol
            break;
    case 5: sendOne(5,ALL1S);         //Panasonic_Old 22 bits used by some SA and Cisco cable boxes
            break;
    case 6: //JVC use "true" for first frame, "false" for repeats
            //When "true" it automatically sends one repeat. Use "false"
            //for additional repeats. The 2 lines below will actually send 
            //a total of 3 frames... A first and 2 repeats.
            sendOne(6,PATTERN1,true); 
            sendOne(6,PATTERN1,false); 
            break;      
    case 7: sendOne(7,PATTERN1);      //NECx used by many Samsung TVs
            sendOne(7,REPEAT_CODE);   //Some varieties use ditto 
            break;
    case 8: sendOne(8,0x12345,0x1234);//Samsung36 16 bit address +20 data
            break;
    case 9: sendOne(9,PATTERN1);      //G.I.Cable 16 bits with ditto repeat
            sendOne(9,REPEAT_CODE);   //This will report NEC if both protocols used
            break;
    case 10:  sendOne(10,PATTERN1);     //DirecTV default no repeat, 38 khz
              sendOne(10,PATTERN1,true);//3rd parameter is repeat flag
              sendOne(10,PATTERN1,false,40);//4th is khz, 38, 40, 57 are legal
              break;
    case 11:  sendOne(11,ALL1S,12);   //RCMM Phillips protocol used by U-Verse
              sendOne(11,ALL1S,24);   //also 24 or 32 bit possible
              sendOne(11,ALL1S,32);
              break;
    case 12:  sendOne(12,CYKM_MOUSE_MOVE+CYKM_DIR_RIGHT);//Move mouse right
              sendOne(12,CYKM_MOUSE_MOVE+CYKM_DIR_UP);   //Move mouse up
              sendOne(12,CYKM_MOUSE_MOVE+CYKM_DIR_LEFT); //Move mouse left
              sendOne(12,CYKM_MOUSE_MOVE+CYKM_DIR_DOWN); //Move mouse down
              break;    
  };
  
}
void loop() {
  if (Serial.available () > 0) {
    int16_t i = Serial.parseInt();
    if(i==-1) {
      for(i=1;i<=12;i++) {
        doTest(i);
      };
    } else {
      doTest(i);
    }
  }
}
