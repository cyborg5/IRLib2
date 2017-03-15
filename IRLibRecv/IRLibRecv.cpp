/* IRrecv.cpp
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * This module implements the original IRrecv receiver class which uses the 50us
 * interrupt timer to poll the input pin. While this is generally accurate enough for
 * everyday purposes, it may be difficult to port to other hardware unless you know a lot
 * about hardware timers and interrupts. Also when trying to analyze unknown protocols, 
 * the 50us granularity may not be sufficient. In that case use either the IRrecvLoop or 
 * the IRrecvPCI class.
 */
 
#include "IRLibRecv.h"
#include "IRLibHardware.h" //needed for IRLib_didIROut
#if !defined (__SAMD21G18A__)
  #include <avr/interrupt.h>
#endif
  
/* Initializes receiving and starts up the 50us interrupt timer. Call this to initialize
 * and call it again to resume receiving after completing the decoding. Previous versions of
 * IRLib had a "resume" method but you should use this in either initializing or resuming.
 */ 
void IRrecv::enableIRIn(void) {
//If newDataAvailable means that we did an auto resume and while the decoder
//was processing the first frame, the second frame completed. We came here
//essentially saying "get me another frame" but the another frame was already here.
//Therefore we get out, the decoder will call getResults which will be true and
//everything proceeds as normal.
  if(recvGlobal.newDataAvailable) 
    return;
  recvGlobal.decoderWantsData=true;//otherwise he wouldn't have called
//We cannot simply restart the receiver because if it did an auto resume then it is
//already running.  If state is STATE_FINISHED it means the ISR finished and did not auto
//resume. If IRLib_didIROut is true then the send object shut us down due to the hardware
//conflict between PWM output and the timer interrupt. It means we need to reinitialize
//everything.
  if( (recvGlobal.currentState==STATE_FINISHED) || IRLib_didIROut ) {
    IRrecvBase::enableIRIn();
    recvGlobal.timer = 0;
    // setup pulse clock timer interrupt
    noInterrupts();
    IR_RECV_CONFIG_TICKS();
    IR_RECV_ENABLE_INTR;
    interrupts();
  }
}

/* Even when not sampling inputs and recording pulse timing, the ISR remains active
 * continues to interrupt every 50us however it remains in a do-nothing state. If 
 * the user wants to truly shut down the ISR they can call this method.
 */
void IRrecv::disableIRIn(void) {
  IR_RECV_DISABLE_INTR; //Defined in IRLibTimer.h disables 50us timer interrupt
  IRrecvBase::disableIRIn(); //Base method resets a variety variables
}

/* Returns true if a frame of data is available in the buffer. Most of the
 * handling is done by the base method. The parameter passed the number of
 * microseconds per timing interval (usually 50)
 */
bool IRrecv::getResults(void) {
  if (recvGlobal.newDataAvailable) {
    IRrecvBase::getResults(USEC_PER_TICK);
    return true;
  }
  return false;
}

/* Call this method to specify how long of a space designates
 * the end of the frame data. You cannot set the value directly
 * but have to do it by this method.
 */
void IRrecv::setFrameTimeout(uint16_t frameTimeout) {
  recvGlobal.frameTimeoutTicks=frameTimeout/USEC_PER_TICK;
  IRrecv::setFrameTimeout(frameTimeout);
}

/*
 * This interrupt service routine is used by IRrecv.  Interrupt is triggered every
 * 50 us using hardware timers. At each interrupt, input pin is polled to see if it is a
 * MARK or SPACE. We count the number of these 50 us ticks to estimate the length
 * of each MARK or SPACE interval. This code is implemented as a state machine.
 * A description of each state is given in the code below. More details on the ISR macro
 * see http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__interrupts.html
 */
IR_RECV_INTR_NAME {
//If the receiver pin is low, data is a MARK, if high, data is a SPACE
  #define IR_MARK 0
  #define IR_SPACE 1
  uint8_t irData = digitalRead(recvGlobal.recvPin);
  recvGlobal.timer++; // One more 50us tick
  IR_CLEAR_INTERRUPT
  if (recvGlobal.recvLength >= RECV_BUF_LENGTH) {    // Buffer overflow
    IRLib_IRrecvComplete(4);
  }
  switch(recvGlobal.currentState) {
  case STATE_READY_TO_BEGIN:
    // The state machine is initialized in STATE_READY_TO_BEGIN which means that it is 
    // looking for the initial MARK signal of a frame of data. It continues to count ticks 
    // in order to measure the interval of the gap between data frames. When it finds the 
    // initial MARK, it records the length of the gap, resets the tick counter,
    // and changes the state to STATE_TIMING_MARK. 
    if (irData == IR_MARK) {
	  recvGlobal.recvLength = 0;
      recvGlobal.recvBuffer[recvGlobal.recvLength++] = recvGlobal.timer;
      recvGlobal.timer = 0;
      recvGlobal.currentState = STATE_TIMING_MARK;
    }
    break;
  case STATE_TIMING_MARK: 
    // This state means that the previous sample was a MARK. If it reads a SPACE 
    // then it records the number of ticks, resets the counter, and changes the state to
    // STATE_TIMING_SPACE. If it was still a MARK we just keep counting.
    if (irData == IR_SPACE) {   // MARK ended, record time
      recvGlobal.recvBuffer[recvGlobal.recvLength++] = recvGlobal.timer;
      recvGlobal.timer = 0;
      recvGlobal.currentState = STATE_TIMING_SPACE;
    }
    break;
  case STATE_TIMING_SPACE: 
    // This state means that the previous sample was a SPACE. If it reads a MARK
    // then it records the number of ticks, reset the counter, and changes the state to
    // STATE_TIMING_MARK. However if it is still a SPACE, we need to see if it was so big
    // that we are seeing the gap between frames. If the gap is long, we need to finish
    // and somehow indicate that we have completed a full frame data.
    if (irData == IR_MARK) { // SPACE just ended, record it
      recvGlobal.recvBuffer[recvGlobal.recvLength++] = recvGlobal.timer;
      recvGlobal.timer = 0;
      recvGlobal.currentState = STATE_TIMING_MARK;
    } 
    else { 
      // If we get here we know that the previous was a SPACE and it still is a SPACE.
      // Now we have to see if the SPACE has gotten is long enough that we know it
      // must be the end of the frame.
      if (recvGlobal.timer > recvGlobal.frameTimeoutTicks) {
        IRLib_IRrecvComplete(5);
      } 
    }
    break;
  case STATE_FINISHED: 
    // The state means that we completed receiving a frame and are waiting for
    // the user to ask us for another one. We will keep counting the time of the
    // gap and reset that timer if we ever see a MARK.
    if (irData == IR_MARK) {
      recvGlobal.timer = 0;
    }
    break;
  }
  IRLib_doBlink();
}
