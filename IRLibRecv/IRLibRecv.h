/* IRLibRecv.h 
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 *
 * You should include this header in your sketch if you're using the original IRLibRecv
 * receiver class which uses the 50us interrupt timer to poll the input pin.
 * While this is generally accurate enough for everyday purposes, it may be difficult to
 * port to other hardware unless you know a lot about hardware timers and interrupts. Also
 * when trying to analyze unknown protocols, the 50us granularity may not be sufficient.
 * In that case use either the IRLibRecvLoop or the IRLibRecvPCI class.
 * Applications that do sending only SHOULD NOT include this header.  
 */

#ifndef IRLibRecv_h
#define IRLibRecv_h
#include "IRLibRecvBase.h"

class IRrecv: public IRrecvBase {
public:
  IRrecv(uint8_t recvPin):IRrecvBase(recvPin){};
  void enableIRIn(void); //call to initialize or resume receiving
  bool getResults(void); //returns true if new frame of data has been received
  void disableIRIn(void); //ISR runs continuously once started. Use this if you want to stop.
  void setFrameTimeout(uint16_t frameTimeout); //maximum gap determines end of frame.
};
#endif //IRLibRecv_h
