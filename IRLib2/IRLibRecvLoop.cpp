/* IRLibRecvLoop.cpp
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * This module implements the IRLibRecvLoop receiver. 
 * Other interrupt driven receivers allow you to do other things and call getResults at 
 * your leisure to see if perhaps a sequence has been received. Typically you would put
 * getResults in your loop and it would return false until the sequence had been received.
 * However because this receiver uses no interrupts, it takes control of your program when
 * you call getResults and doesn't let go until it's got something to show you. The advantage
 * is you don't need interrupts which would make it easier to use on non-standard hardware 
 * and will allow you to use any digital input pin. Timing of this routine is only as 
 * accurate as your "micros();" Note the enableIRIn method should still be called because 
 * initializes the input pin even though that method does nothing else. Similarly 
 * disableIRin doesn't do anything. Also auto resume is meaningless because there is no
 * multitasking possible.
 */

#include "IRLibRecvLoop.h"

bool IRrecvLoop::getResults(void) {
  uint8_t oldState=HIGH;uint8_t newState;
  uint32_t startTime, endTime;
  startTime=micros();
  recvGlobal.currentState=STATE_RUNNING;
  while(recvGlobal.currentState==STATE_RUNNING) {
    while(oldState==(newState=digitalRead(recvGlobal.recvPin))) { //while pin unchanged
      //if SPACE and not initial gap test for timeout
      if(oldState && recvGlobal.recvLength) {//oldState=HIGH means SPACE.
        //If it's a very long wait
        if( (micros()-startTime) > recvGlobal.frameTimeout) {
          recvGlobal.currentState= STATE_FINISHED;
          break; //exit the pin unchanged loop
        }
      }
    }
    IRLib_doBlink();
    recvGlobal.recvBuffer[recvGlobal.recvLength]=(endTime=micros()) - startTime;
    oldState=newState;
    startTime=endTime;
    if(++recvGlobal.recvLength>=RECV_BUF_LENGTH) {
      recvGlobal.currentState=STATE_FINISHED;
    }
  }
  recvGlobal.recvLength--;//ignore final SPACE
  recvGlobal.didAutoResume=false;//no multi processing possible with IRrecvLoop
  IRrecvBase::getResults();
  return true;
}
