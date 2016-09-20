/* freq.ino Example sketch for IRLib2
 * Illustrates how to measure modulation frequency using the
 * IRfrequency class in the IRLibFreq library. You must connect 
 * an IR learner such as TSMP58000 to a hardware interrupt pin.
 */

#include <IRLibFreq.h>  

#define FREQUENCY_PIN 3
//NOTE: Previous versions of IRLib required you to pass the 
// interrupt number to be passed to the built-in "attachInterrupt()"
// function however starting with IRLib 2.x you now pass the pin number.
IRfrequency My_Freq(FREQUENCY_PIN);

void setup() {
  Serial.begin(9600);
  delay(2000);while(!Serial);//delay for Leonardo
  if(F_CPU<16000000) {
    Serial.println(F("WARNING: CPU speed less than 16 MHz will be inaccurate"));
    Serial.print(F("for frequencies over 45 kHz. Your CPU is:"));
    Serial.print(F_CPU/1000000,DEC); Serial.println(F(" Mhz"));
  }
  //Let's double check to make sure this pin will work
  int8_t intrNum=digitalPinToInterrupt(FREQUENCY_PIN);
  if (intrNum==NOT_AN_INTERRUPT) {
    Serial.println(F("Invalid frequency pin number."));
    while (1) {};//infinite loop because of fatal error
  }
  Serial.print(F("Interrupt="));  Serial.print(intrNum,DEC);
  Serial.print(F(" Pin=")); Serial.println(FREQUENCY_PIN,DEC);
  My_Freq.enableFreqDetect();//starts interrupt routine to compute frequency
}

void loop() {
  if (My_Freq.haveData()) {
    delay(500);  //it couldn't hurt to collect a little more just in case
    My_Freq.disableFreqDetect();
    My_Freq.dumpResults(true);//Change to "true" for more detail
    My_Freq.enableFreqDetect();//Zero out previous results and restart ISR
  }
}

