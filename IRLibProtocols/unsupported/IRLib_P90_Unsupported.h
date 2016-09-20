/* IRLib_P90_Unsupported.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */
/*
 * In general we welcome the addition of new protocols for consumer electronic
 * devices such as TVs, DVD, Blu-ray, cable box etc. We will assign you
 * an official protocol number and you can be a permanent part of the library.
 * However more obscure protocols especially those used for air-conditioners
 * will not be made official protocols but we do welcome that you submit your code
 * to be placed in IRLibProtocols/unsupported folder. Protocols for air-conditioners
 * generally have very long sequences which require an extraordinary long buffer
 * and they generally are not widely used among a variety of manufacturers.
 * Similarly protocols used by toys such as IR controlled helicopters and drones
 * and electronic pets are so obscure that they would not be of wide interest
 * so we will probably not make them an official protocol number. But we do welcome
 * that you submit such code and put it in the unsupported folder.
 * 
 * When creating a new general-purpose protocol should start with a copy of
 * IRLibProtocols/IRLib_P99_Additional.h and rename it to the next available
 * protocol number for example it might be IRLibProtocols/IRLib_P15_Sharp.h or similar.
 *
 * However for air-conditioners, toys, and other unsupported uses fusion start with
 * a copy of IRLibProtocols/unsupported/IRLib_P90_Unsupported.h and rename it
 * to something like IRLibProtocols/unsupported/IRLib_P90_MyToyRobot.h. Note
 * that you still maintain protocol number 90 which is supported in IRLibCombo.h.
 * If you ever need to use more than one unsupported protocol you can use 91, 92 etc.
 * however in the official version of the library all unsupported protocols will be 90.
 * This is dummy code that you can copy and rename and modify when implementing new protocols.
 */

#ifndef IRLIB_PROTOCOL_90_H
#define IRLIB_PROTOCOL_90_H
#define IR_SEND_PROTOCOL_90		case 90: IRsendUnsupported::send(data); break;
#define IR_DECODE_PROTOCOL_90	if(IRdecodeUnsupported::decode()) return true;
#ifdef IRLIB_HAVE_COMBO
	#define PV_IR_DECODE_PROTOCOL_90 ,public virtual IRdecodeUnsupported
	#define PV_IR_SEND_PROTOCOL_90   ,public virtual IRsendUnsupported
#else
	#define PV_IR_DECODE_PROTOCOL_90  public virtual IRdecodeUnsupported
	#define PV_IR_SEND_PROTOCOL_90    public virtual IRsendUnsupported
#endif

#ifdef IRLIBSENDBASE_H
class IRsendUnsupported: public virtual IRsendBase {
  public:
    void IRsendUnsupported::send(uint32_t data) {
    //void IRsendUnsupported::send(uint32_t data, uint32_t data2)//optional form
      /*********
       *  Insert your code here.
       *********/
    };
};
#endif  //IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H
class IRdecodeUnsupported: public virtual IRdecodeBase {
  public:
    bool IRdecodeUnsupported::decode(void) {
      IRLIB_ATTEMPT_MESSAGE(F("Unsupported"));
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

#endif //IRLIB_PROTOCOL_90_H
