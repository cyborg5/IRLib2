/* IRLibRecvLoop.h 
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * You should include this header in your sketch if you're using the IRLibRecvLoop receiver.
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

#ifndef IRLibRecvLoop_h
#define IRLibRecvLoop_h
#include "IRLibRecvBase.h"

class IRrecvLoop: public IRrecvBase {
public:
  IRrecvLoop(uint8_t recvPin):IRrecvBase(recvPin){};
  bool getResults(void);
};

#endif //IRLibRecvLoop_h
