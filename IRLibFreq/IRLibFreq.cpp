/* IRLibFreq.cpp
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * This class facilitates detection of frequency of an IR signal. It requires
 * a different kind of receiver specifically a TSMP58000 or equivalent device 
 * connected to the hardware interrupt pin. Create an instance of the object 
 * passing the interrupt number.
 */
#include <Arduino.h>
#include <avr/interrupt.h>
#include "IRLibFreq.h"

volatile FREQUENCY_BUFFER_TYPE *IRfreqTimes;
volatile uint8_t IRfreqCount;
IRfrequency::IRfrequency(uint8_t pin) {
  intrNum=digitalPinToInterrupt(pin);
  //ISR cannot be passed parameters. If I declare the buffer global it would
  //always eat RAN even if this object was not declared. So we make global pointer
  //and copy the address to it. ISR still puts data in the object.
  IRfreqTimes= & (Time_Stamp[0]);
};

// Note ISR handler cannot be part of a class/object
void IRfreqISR(void) {
   IRfreqTimes[IRfreqCount++]=micros();
}

void IRfrequency::enableFreqDetect(void){
  attachInterrupt(intrNum,IRfreqISR,FALLING);
  for(i=0; i<256; i++) Time_Stamp[i]=0;
  IRfreqCount=0;
  results=0.0;
  samples=0;
};

/* Test to see if we have collected at least one full buffer of data.
 * Note values are always zeroed before beginning so any non-zero data
 * in the final elements means we have collected at least a buffer full.
 * By chance the final might be zero so we test two of them. Would be
 * nearly impossible for two consecutive elements to be zero unless
 * we had not yet collected data.
 */
bool IRfrequency::haveData(void) {
  return (Time_Stamp[255] || Time_Stamp[254]);
};

void IRfrequency::disableFreqDetect(void){
  detachInterrupt(intrNum);
 };

void IRfrequency::computeFreq(void){
   samples=0; sum=0;
   for(i=1; i<256; i++) {
     uint8_t interval=Time_Stamp[i]-Time_Stamp[i-1];
	 if(interval>50 || interval<10) continue;//ignore extraneous results
	 sum+=interval;//accumulate usable intervals
	 samples++;    //account usable intervals
   };
   if(sum)
     results=(double) samples/(double)sum*1000;
   else
     results= 0.0;
};
 
void IRfrequency::dumpResults(bool detail) {
  computeFreq();
  Serial.print(F("Number of samples:")); Serial.print(samples,DEC);
  Serial.print(F("\t  Total interval (us):")); Serial.println(sum,DEC); 
  Serial.print(F("Avg. interval(us):")); Serial.print(1.0*sum/samples,2);
  Serial.print(F("\t Aprx. Frequency(kHz):")); Serial.print(results,2);
  Serial.print(F(" (")); Serial.print(int(results+0.5),DEC);
  Serial.println(F(")"));
  if(detail) {
    for(i=1; i<256; i++) {
      uint16_t interval=Time_Stamp[i]-Time_Stamp[i-1];
      Serial.print(interval,DEC); Serial.print("\t");
      if ((i % 4)==0)Serial.print(F("\t "));
      if ((i % 8)==0)Serial.println();
      if ((i % 32)==0)Serial.println();
    }
    Serial.println();
  }
};
