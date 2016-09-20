/* dumpFreq.ino Example sketch for IRLib2
 * Illustrates how to simultaneously decode an IR stream and
 * measure modulation frequency using the IRfrequency class in 
 * the IRLibFreq library. You must connect an IR learner such 
 * as TSMP58000 to a hardware interrupt pin. Rather than use the
 * IRrecv receiver we recommend using the IRrecvPCI or IRrecvLoop
 * receiver because the 50 microsecond interrupts from IRrecv
 * can cause inaccurate results.
 */
#include <IRLibAll.h>//Note this also includes "IRLibFreq.h"

//NOTE: Previous versions of IRLib required you to pass the 
// interrupt number to be passed to the built-in "attachInterrupt()"
// function however starting with IRLib 2.x you now pass the pin number.
#define RECEIVER_PIN 2
#define FREQUENCY_PIN 3

IRrecvPCI myReceiver(RECEIVER_PIN);
//IRrecvLoop myReceiver(RECEIVER_PIN); //comment out previous line and un-comment this for loop version
IRfrequency myFreq(FREQUENCY_PIN);
IRdecode myDecoder;

void setup() {
  Serial.begin(9600);
  delay(2000);while(!Serial);//delay for Leonardo
    if(F_CPU<16000000) {
    Serial.println(F("WARNING: CPU speed less than 16 MHz will be inaccurate"));
    Serial.print(F("for frequencies over 45 kHz. Your CPU is:"));
    Serial.print(F_CPU/1000000,DEC); Serial.println(F(" Mhz"));
  }
  int8_t intrNum=digitalPinToInterrupt(FREQUENCY_PIN);
  if(intrNum==NOT_AN_INTERRUPT) {
    Serial.println(F("Invalid frequency pin number."));
    while (1) {};//infinite loop because of fatal error
  }
  Serial.print(F("Frequency interrupt="));  Serial.print(intrNum,DEC);
  Serial.print(F(" Pin=")); Serial.println(FREQUENCY_PIN,DEC);
  intrNum=digitalPinToInterrupt(RECEIVER_PIN);
    if (intrNum==NOT_AN_INTERRUPT) {
    Serial.println(F("Invalid receiver pin number."));
    while (1) {};//infinite loop because of fatal error
  }
  Serial.print(F("Receiver interrupt="));  Serial.print(intrNum,DEC);
  Serial.print(F(" Pin=")); Serial.println(RECEIVER_PIN,DEC);

  myReceiver.enableIRIn(); // Start the receiver
  myFreq.enableFreqDetect();//starts interrupt routine to compute frequency
}

void loop() {
  if (myReceiver.getResults()) {
    myFreq.disableFreqDetect();//Stop interrupt as soon as possible
    myDecoder.decode();
    myFreq.dumpResults(false);//Change to "true" for more detail
    myDecoder.dumpResults();
    myReceiver.enableIRIn(); 
    myFreq.enableFreqDetect();//Zero out previous results and restart ISR
  }
}

