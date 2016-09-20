/* IRLib_P12_Tutorial.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */
/*
 * This is dummy code that you can copy and rename and modify when implementing new protocols.
 */

#ifndef IRLIB_PROTOCOL_12_H
#define IRLIB_PROTOCOL_12_H
#define IR_SEND_PROTOCOL_12		case 12: IRsendTutorial::send(data); break;
#define IR_DECODE_PROTOCOL_12	if(IRdecodeTutorial::decode()) return true;
#ifdef IRLIB_HAVE_COMBO
	#define PV_IR_DECODE_PROTOCOL_12 ,public virtual IRdecodeTutorial
	#define PV_IR_SEND_PROTOCOL_12   ,public virtual IRsendTutorial
#else
	#define PV_IR_DECODE_PROTOCOL_12  public virtual IRdecodeTutorial
	#define PV_IR_SEND_PROTOCOL_12    public virtual IRsendTutorial
#endif

#ifdef IRLIBSENDBASE_H
class IRsendTutorial: public virtual IRsendBase {
  public:
    void send(uint32_t data) {
      if(data==REPEAT_CODE) {
        enableIROut(39);
        mark (490*18); space(2205);//actually "490*4.5"
        mark (490); space(220);delay(87);//actually 490*178 or "space(87220);"
      } else {
        sendGeneric(data,16, 490*18, 490*9, 490, 490, 490*9, 2205/*(4.5*490)*/, 39, true);
        space(37*490);
      }
    }
};
#endif  //IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H
class IRdecodeTutorial: public virtual IRdecodeBase {
  public:
    bool IRdecodeTutorial::decode(void) {
      IRLIB_ATTEMPT_MESSAGE(F("Tutorial"));
      /*********
       *  Insert your code here. Return false if it fails. Don't forget to include the
       *  following lines or equivalent somewhere in the code.
       *  
       *  bits = 32;	//Substitute proper value here
       *  value = data;	//return data in "value"
       *  protocolNum = ADDITIONAL;	//be sure to set the protocol type here.
       */
      return true;
    }
};

#endif //IRLIBDECODEBASE_H

#define IRLIB_HAVE_COMBO

#endif //IRLIB_PROTOCOL_12_H
