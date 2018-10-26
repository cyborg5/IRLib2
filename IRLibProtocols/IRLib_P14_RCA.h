/* IRLib_P14_RCA.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */

/*
 * This is code implements the TV codes for an RCA CRCRN04GR remote control.
 */

#ifndef IRLIB_PROTOCOL_14_H
#define IRLIB_PROTOCOL_14_H
#define IR_SEND_PROTOCOL_14		case 14: IRsendRCA::send(data); break;
#define IR_DECODE_PROTOCOL_14	if(IRdecodeRCA::decode()) return true;
#ifdef IRLIB_HAVE_COMBO
	#define PV_IR_DECODE_PROTOCOL_14 ,public virtual IRdecodeRCA
	#define PV_IR_SEND_PROTOCOL_14   ,public virtual IRsendRCA
#else
	#define PV_IR_DECODE_PROTOCOL_14  public virtual IRdecodeRCA
	#define PV_IR_SEND_PROTOCOL_14    public virtual IRsendRCA
#endif

#ifdef IRLIBSENDBASE_H
class IRsendRCA: public virtual IRsendBase {
  public:
    void IRsendRCA::send(uint32_t data) {
    //void IRsendRCA::send(uint32_t data, uint32_t data2)//optional form
      /*********
       *  Insert your code here.
       *********/
    };
};
#endif  //IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H
class IRdecodeRCA: public virtual IRdecodeBase {
  public:
    bool IRdecodeRCA::decode(void) {
      IRLIB_ATTEMPT_MESSAGE(F("Protocol_14"));
      /*********
       *  Insert your code here. Return false if it fails. 
       *  Don't forget to include the following lines or 
       *  equivalent somewhere in the code.
       *  
       *  bits = 32;	//Substitute proper value here
       *  value = data;	//return data in "value"
       *  protocolNum = Protocol_14;	//set the protocol number here.
       */
      return true;
    }
};

#endif //IRLIBDECODEBASE_H

#define IRLIB_HAVE_COMBO

#endif //IRLIB_PROTOCOL_14_H
