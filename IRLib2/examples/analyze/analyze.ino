/* analyze.ino Example sketch for IRLib2
 * This sketch is used to analyze unknown protocols. It receives
 * repeated values and averages the results. You have to press the 
 * same key on a remote repeatedly. If you press a different key
 * it will reset and compute a new average.
 */

//This includes everything. Not generally recommended.
#include <IRLibDecodeBase.h>
#include <IRLib_P02_Sony.h>
#include <IRLib_P09_GICable.h>
#include <IRLibCombo.h>

IRdecode myDecoder;   //create decoder
//Recommend using either the PCI or Loop receivers
#include <IRLibRecvPCI.h>
IRrecvPCI  myReceiver(2); //receiver on pin 2
//#include <IRLibRecvLoop.h>
//IRrecvLoop myReceiver(2);//receiver on pin 2

uint16_t sampleCount,i, lowSpace, lowMark, highMark, highSpace, interval,
            balm,aalm,bahm,aahm,bals,aals,bahs,aahs;
uint8_t bacm,aacm,bacs,aacs, markCount,spaceCount,oldProtocol;
uint16_t accumulatedTime[RECV_BUF_LENGTH];
uint32_t markAvg, spaceAvg,baam,aaam,baas,aaas, oldValue;

void setup() {
  Serial.begin(9600);
  delay(1000);while(!Serial);
  myReceiver.enableIRIn();
  sampleCount=0;oldValue=0; oldProtocol=UNKNOWN;
  Serial.println(F("Send a signal repeatedly. We will report averages and statistics."));
}
void Tab(void) {Serial.print("\t");};
void loop() {
  if (myReceiver.getResults()) {
    myDecoder.decode();
    if( (myDecoder.protocolNum != oldProtocol) || (myDecoder.value != oldValue)) {
      Serial.println(F("Resetting counters"));
      for(i=0;i<RECV_BUF_LENGTH;i++) {
        accumulatedTime[i]=0;
      };
      sampleCount=0;oldValue= myDecoder.value; oldProtocol=myDecoder.protocolNum;
    };
    sampleCount++;
    Serial.print(F("\nSamples=")); Serial.println(sampleCount,DEC);
    lowSpace = lowMark=65535;
    highSpace = highMark=0;
    markAvg= spaceAvg= markCount= spaceCount=0;
    for(i=0;i<recvGlobal.decodeLength;i++){
      accumulatedTime[i]+=recvGlobal.decodeBuffer[i];
      recvGlobal.decodeBuffer[i]= accumulatedTime[i]/sampleCount;//Put back average so DumpResults can report
    }
    myDecoder.dumpResults();
    //Perform additional analysis
    for(i=3;i<recvGlobal.decodeLength;i++){ //Compute low, high and average mark and space
      interval=recvGlobal.decodeBuffer[i];
      if(i % 2) {
        markAvg += interval; lowMark=min(lowMark, interval);  highMark=max(highMark, interval);markCount++;
      } else {
        spaceAvg += interval; lowSpace=min(lowSpace, interval);  highSpace=max (highSpace, interval);spaceCount++;
      }
      recvGlobal.decodeBuffer[i]= accumulatedTime[i]/sampleCount;//Put back average so DumpResults can report
    }
    markAvg /= markCount; spaceAvg /= spaceCount;
    //Now compute below average highs and lows and above average highs and lows
    balm=bals=aalm=aals=32766;
    bahm=bahs=aahm=aahs=baam=baas=aaam=aaas=0;
    bacm=bacs=aacm=aacs=0;
    for(i=3;i<recvGlobal.decodeLength;i++){
      interval=recvGlobal.decodeBuffer[i];
      if(i % 2) {
        if (interval>markAvg) {
          aalm=min(aalm,interval); aahm=max(aahm,interval);aacm++;aaam+=interval;
        } else {
          balm=min(balm,interval); bahm=max(bahm,interval);bacm++;baam+=interval;
        }
      } else {
        if (interval>spaceAvg) {
          aals=min(aals,interval); aahs=max(aahs,interval);aacs++;aaas+=interval;
        } else {
          bals=min(bals,interval); bahs=max(bahs,interval);bacs++;baas+=interval;
        }
      }
    }
    baam/=bacm;baas/=bacs;aaam/=aacm;aaas/=aacs;
    Serial.println(F("\t #\tLow\tHigh\tAvg.")); 
    Serial.print(F("Mark\t"));Serial.print(markCount,DEC);Tab();
      Serial.print(lowMark,DEC);Tab();Serial.print(highMark,DEC);Tab();
      Serial.println(markAvg,DEC);
    Serial.print(F("Space\t"));Serial.print(spaceCount,DEC);Tab();
      Serial.print(lowSpace,DEC);Tab();Serial.print(highSpace,DEC);Tab();
      Serial.println(spaceAvg,DEC);

    Serial.println(F("\nMarks\t\t #\tLow\tHigh\tAvg.")); 
    Serial.print(F("Above Avg\t"));Serial.print(aacm,DEC);Tab();
      Serial.print(aalm,DEC);Tab();Serial.print(aahm,DEC);Tab();
      Serial.println(aaam,DEC);
    Serial.print(F("Belows Avg.\t"));Serial.print(bacm,DEC);Tab();
      Serial.print(balm,DEC);Tab();Serial.print(bahm,DEC);Tab();
      Serial.println(baam,DEC);
    
    Serial.println(F("\nSpaces\t\t #\tLow\tHigh\tAvg.")); 
    Serial.print(F("Above Avg.\t"));Serial.print(aacs,DEC);Tab();
      Serial.print(aals,DEC);Tab();Serial.print(aahs,DEC);Tab();
      Serial.println(aaas,DEC);
    Serial.print(F("Below Avg.\t"));Serial.print(bacs,DEC);Tab();
      Serial.print(bals,DEC);Tab();Serial.print(bahs,DEC);Tab();
      Serial.println(baas,DEC);

#define RATIO(n,d) Tab(); Serial.print(float(n)/float(d));
    Serial.print(F("\nRatios\t\tvs   Marks\tvs   Spaces"));
    Serial.print(F("\n\t\tBelow\tAbove\tBelow\tAbove\n"));
    Serial.print(F("Head Mark"));RATIO(recvGlobal.decodeBuffer[1],baam);
      RATIO(recvGlobal.decodeBuffer[1],aaam);
      RATIO(recvGlobal.decodeBuffer[1],baas);
      RATIO(recvGlobal.decodeBuffer[1],aaas);      
    Serial.print(F("\nHead Space"));RATIO(recvGlobal.decodeBuffer[2],baam);
      RATIO(recvGlobal.decodeBuffer[2],aaam);
      RATIO(recvGlobal.decodeBuffer[2],baas);
      RATIO(recvGlobal.decodeBuffer[2],aaas);
    Serial.println(F("\n\n\t\t\tLow\tHigh\tAvg.")); 
    Serial.print(F("Space vs Avg Mark"));RATIO(baas,markAvg);RATIO(aaas,markAvg);RATIO(spaceAvg,markAvg);
    Serial.print(F("\nMark vs Avg Space"));RATIO(baam,spaceAvg);RATIO(aaam,spaceAvg);RATIO(markAvg,spaceAvg);
    Serial.print(F("\n\nMark Above/Below="));RATIO(aaam,baam);
    Serial.print(F("\nSpace Above/Below="));RATIO(aaas,baas);
    Serial.println();
    myReceiver.enableIRIn();
  };
}

